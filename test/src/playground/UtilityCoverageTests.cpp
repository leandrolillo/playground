#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <vector>

#include "mathMatchers.h"
#include "JsonParser.h"
#include "Logger.h"
#include "StringFormatter.h"
#include "StringUtils.h"

namespace {

String writeTempFile(const String &contents) {
  auto filePath = std::filesystem::temp_directory_path()
      / ("playground-test-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + ".txt");

  std::ofstream output(filePath);
  output << contents;
  output.close();

  return filePath.string();
}

String formatVarArgsHelper(const char *format, ...) {
  va_list args;
  va_start(args, format);
  String result = StringFormatter::formatVarArgs(format, &args);
  va_end(args);

  return result;
}

class MemoryAppender : public Appender {
public:
  std::vector<String> messages;

  MemoryAppender(const String &target = "memory") : Appender(target) {
  }

  void append(const char *message) override {
    messages.emplace_back(message);
  }

  void flush() override {
  }

  String joined() const {
    String joinedMessages;
    for (const auto &message : messages) {
      joinedMessages += message;
    }
    return joinedMessages;
  }
};

}

TEST_CASE("StringFormatter coverage") {
  CHECK(StringFormatter::format("Hello %s %d", "world", 7) == "Hello world 7");
  CHECK(formatVarArgsHelper("%0.2f %s", 1.25, "done") == "1.25 done");
  CHECK(StringFormatter::formatVarArgs("%s", null).empty());
}

TEST_CASE("StringUtils coverage") {
  CHECK(StringUtils::trim(" \t padded text \n") == "padded text");
  CHECK(StringUtils::trim(" \t \n ").empty());
  CHECK(StringUtils::toLowercase(" MiXeD-Case_123 ") == " mixed-case_123 ");

  std::vector<String> empty;
  CHECK(StringUtils::join(empty, '/') == "");

  std::vector<String> single {"token"};
  CHECK(StringUtils::join(single, '/') == "token");

  std::vector<String> multiple {"one", "two", "three"};
  CHECK(StringUtils::join(multiple, '/') == "one/two/three");
}

TEST_CASE("TextParser coverage") {
  SECTION("Read string") {
    String filename = writeTempFile("\"hello world\"");
    FileParser fileParser(filename);
    TextParser parser(fileParser);

    CHECK(parser.readString() == "hello world");
  }

  SECTION("Read string throws for non strings") {
    String filename = writeTempFile("token");
    FileParser fileParser(filename);
    TextParser parser(fileParser);

    CHECK_THROWS_AS(parser.readString(), std::domain_error);
  }

  SECTION("Read boolean values") {
    String filename = writeTempFile("false");
    FileParser fileParser(filename);
    TextParser parser(fileParser);

    CHECK_FALSE(parser.readBoolean());
  }

  SECTION("Read boolean throws for invalid values") {
    String filename = writeTempFile("invalid");
    FileParser fileParser(filename);
    TextParser parser(fileParser);

    CHECK_THROWS_AS(parser.readBoolean(), std::domain_error);
  }

  SECTION("Read unsigned integer throws for invalid values") {
    String filename = writeTempFile("invalid");
    FileParser fileParser(filename);
    TextParser parser(fileParser);

    CHECK_THROWS_AS(parser.readUnsignedInteger(), std::domain_error);
  }

  SECTION("Read real values") {
    String filename = writeTempFile("-12.5e2 +10 .25 1.0E-2");
    FileParser fileParser(filename);
    TextParser parser(fileParser);

    CHECK_THAT(parser.readReal(), Catch::Matchers::WithinAbs(-1250.0, 0.000001));
    CHECK_THAT(parser.readReal(), Catch::Matchers::WithinAbs(10.0, 0.000001));
    CHECK_THAT(parser.readReal(), Catch::Matchers::WithinAbs(0.25, 0.000001));
    CHECK_THAT(parser.readReal(), Catch::Matchers::WithinAbs(0.01, 0.000001));
  }
}

TEST_CASE("JsonParser coverage") {
  SECTION("Read vector and string arrays") {
    String filename = writeTempFile("[<1,2>,<3.5,4.5>] [<1,2,3>,<4,5,6>] [\"alpha\",\"beta\"]");
    FileParser fileParser(filename);
    JsonParser parser(fileParser);

    auto vector2Array = parser.readVector2Array();
    REQUIRE(vector2Array.size() == 2);
    CHECK_THAT(vector2Array.at(0), EqualsVector(vector2(1, 2)));
    CHECK_THAT(vector2Array.at(1), EqualsVector(vector2(3.5, 4.5)));

    auto vector3Array = parser.readVector3Array();
    REQUIRE(vector3Array.size() == 2);
    CHECK_THAT(vector3Array.at(0), EqualsVector(vector3(1, 2, 3)));
    CHECK_THAT(vector3Array.at(1), EqualsVector(vector3(4, 5, 6)));

    CHECK(parser.readStringArray() == std::vector<String>({"alpha", "beta"}));
  }

  SECTION("Read empty arrays") {
    String filename = writeTempFile("[] [] []");
    FileParser fileParser(filename);
    JsonParser parser(fileParser);

    CHECK(parser.readIntegerArray().empty());
    CHECK(parser.readUnsignedIntegerArray().empty());
    CHECK(parser.readRealArray().empty());
  }

  SECTION("Read token allows unquoted values") {
    String filename = writeTempFile("token");
    FileParser fileParser(filename);
    JsonParser parser(fileParser);

    CHECK(parser.readToken() == "token");
  }

  SECTION("Read element throws on unexpected token") {
    String filename = writeTempFile("]");
    FileParser fileParser(filename);
    JsonParser parser(fileParser);

    CHECK_THROWS_AS(parser.readStartArray(), std::domain_error);
  }
}

TEST_CASE("Logger coverage") {
  SECTION("Specific appenders only receive matching levels") {
    Logger logger("unit-test");
    MemoryAppender infoAppender("info");

    logger.addAppender(LogLevel::INFO, &infoAppender);
    logger.info("hello %s", "logger");
    logger.warn("skip me");

    auto output = infoAppender.joined();
    CHECK_THAT(output, Catch::Matchers::ContainsSubstring("INFO"));
    CHECK_THAT(output, Catch::Matchers::ContainsSubstring("unit-test"));
    CHECK_THAT(output, Catch::Matchers::ContainsSubstring("hello logger"));
    CHECK(output.find("skip me") == String::npos);
  }

  SECTION("Log level filters info and keeps warnings and errors") {
    Logger logger("unit-test");
    MemoryAppender appender;

    logger.addAppender(&appender);
    logger.setLogLevel(LogLevel::WARN);
    logger.info("hidden");
    logger.warn("visible");
    logger.error("boom");

    auto output = appender.joined();
    CHECK(output.find("hidden") == String::npos);
    CHECK_THAT(output, Catch::Matchers::ContainsSubstring("WARNING"));
    CHECK_THAT(output, Catch::Matchers::ContainsSubstring("visible"));
    CHECK_THAT(output, Catch::Matchers::ContainsSubstring("ERROR"));
    CHECK_THAT(output, Catch::Matchers::ContainsSubstring("boom"));
  }

  SECTION("LoggerFactory reuses appenders and loggers") {
    LoggerFactory::doNotLogToFile();
    LoggerFactory::setDefaultLogLevel(LogLevel::DEBUG);

    auto *firstAppender = LoggerFactory::getAppender("stdout");
    auto *secondAppender = LoggerFactory::getAppender("stdout");
    auto *firstLogger = LoggerFactory::getLogger("factory-test");
    auto *secondLogger = LoggerFactory::getLogger("factory-test");

    CHECK(firstAppender == secondAppender);
    CHECK(firstLogger == secondLogger);
    CHECK(firstLogger != LoggerFactory::getLogger("factory-test-other"));

    LoggerFactory::setDefaultLogLevel(DEFAULT_LOG_LEVEL);
    LoggerFactory::setLogFileName("playground.log");
  }
}
