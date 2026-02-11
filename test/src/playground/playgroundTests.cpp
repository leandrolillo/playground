#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "chrono"
#include "thread"

#include "mathMatchers.h"
#include "PlaygroundMock.h"
#include"JsonParser.h"

class TestPlaygroundRunner : public PlaygroundRunner {
public:
  TestPlaygroundRunner(Playground &container) : PlaygroundRunner(container) {

  }

  unsigned char getId() const override {
    return 0;
  }
};

class NotValidPlaygroundRunner {
public:
  NotValidPlaygroundRunner(Playground &container) {

  }

};

TEST_CASE("Playground Test case")
{
  PlaygroundMock playground("");
  playground.withName("TestPlayground");
  REQUIRE(1 == 1);

  CHECK(playground.toString() != "");

  TestPlaygroundRunner &runner = playground.addRunner<TestPlaygroundRunner>();
  CHECK(playground.getRunnersCount() == 1);
  TestPlaygroundRunner &theSameRunner = playground.addRunner<TestPlaygroundRunner>();
  CHECK(playground.getRunnersCount() == 1);

  //playground.addRunner<NotValidPlaygroundRunner>(); //this should fail at compile time.
}

TEST_CASE("StringUtils") {
  auto actual = StringUtils::split("", '/');
  CHECK(actual == std::vector<String> { "" });

  actual = StringUtils::split("children", '/');
  CHECK(actual == std::vector<String> { "children"});

  actual = StringUtils::split("/children", '/');
  CHECK(actual == std::vector<String> { "", "children"});

  actual = StringUtils::split("parent/children", '/');
  CHECK(actual == std::vector<String> { "parent", "children"});

  actual = StringUtils::split("/parent/children", '/');
  CHECK(actual == std::vector<String> { "", "parent", "children"});

  actual = StringUtils::split("parent//children", '/');
  CHECK(actual == std::vector<String> { "parent", "", "children"});



  actual = StringUtils::split("grand-parent/parent/children", '/');
  CHECK(actual == std::vector<String> { "grand-parent", "parent", "children"});

  actual = StringUtils::split("/grand-parent/parent/children", '/'); //starting separator causes a "" in the result.
  CHECK(actual == std::vector<String> { "", "grand-parent", "parent", "children"});
}

TEST_CASE("Chronometer") {
  SECTION("Elapsed Time") { //This test is not working very good or there's an issue with chronometer
    Chronometer stopwatch;

    real elapsed = 0;
    stopwatch.start();
    do {
      elapsed += stopwatch.update();
      std::this_thread::sleep_for(100ms);
    } while (stopwatch.getTotalTime() < 10.0f);

    CHECK_THAT(stopwatch.getTotalTime(), Catch::Matchers::WithinAbs(10, 0.2)); //<--0.2 is too much - review this
  }

  SECTION("Total time") {
    Chronometer stopwatch;


    stopwatch.start();
    auto start = std::chrono::steady_clock::now();
    steady_clock::time_point end;

    do {
      stopwatch.update();
      end = std::chrono::steady_clock::now();
    } while (duration_cast<seconds>(end - start) < 3s);

    CHECK((unsigned int)stopwatch.getTotalTime() == 3);
  }
}

TEST_CASE("Parsers") {
  SECTION("FileParser") {
    String token;

    FileParser commentFileParser("resources/commentFileToParse.txt");
    CHECK(FileParser::eof == commentFileParser.peekToken());
    CHECK(FileParser::eof == commentFileParser.takeToken());
    commentFileParser.close();

    FileParser emptyFileParser("resources/emptyFileToParse.txt");
    CHECK(FileParser::eof == emptyFileParser.peekToken());
    CHECK(FileParser::eof == emptyFileParser.takeToken());
    commentFileParser.close();

    FileParser fileParser("resources/fileToParse.txt");
    CHECK("{" == fileParser.peekToken());
    CHECK("{" == fileParser.takeToken());
    CHECK("\"" == fileParser.takeToken());
    CHECK("property" == fileParser.takeToken());
    CHECK("\"" == fileParser.takeToken());
    CHECK(":" == fileParser.takeToken());
    CHECK("\"" == fileParser.takeToken());
    CHECK("value" == fileParser.takeToken());
    CHECK("\"" == fileParser.takeToken());
    CHECK("}" == fileParser.takeToken());
    CHECK(FileParser::eof == fileParser.peekToken());
    CHECK(FileParser::eof == fileParser.takeToken());
  }

  SECTION("JsonParser") {
    using namespace Catch::Matchers;

    FileParser fileParser("resources/jsonTestFile.json");
    JsonParser parser(fileParser);

    String token;
    parser.readStartObject();
    while ((token = parser.readToken()) != END_OBJECT && token != FileParser::eof) {
      parser.readValueSeparator();

      if(token == "stringProperty") {
        CHECK("value" == parser.readString());
      } else if(token == "integerProperty") {
        CHECK(-10 == parser.readInteger());
      } else if(token == "unsignedIntegerProperty") {
        CHECK(10 == parser.readUnsignedInteger());
      } else if(token == "realProperty") {
        CHECK_THAT(parser.readReal(), Catch::Matchers::WithinRel(1e-12, 0.001));
      } else if(token == "booleanProperty") {
        CHECK(true == parser.readBoolean());
      } else if(token == "vector2Property") {
        CHECK_THAT(parser.readVector2(), EqualsVector(vector2(1, 2)));
      } else if(token == "vector3Property") {
        CHECK_THAT(parser.readVector3(), EqualsVector(vector3(3, 4, 5)));
      } else if(token == "stringList") {
        CHECK_THAT(parser.readStringArray(), Equals(std::vector<String>{"one", "two", "three"}));
      } else if(token == "integerList") {
        CHECK_THAT(parser.readIntegerArray(), Equals(std::vector<int> {-3, -2, -1, 0, 1, 2, 3}));
      } else if(token == "unsignedIntegerList") {
        CHECK_THAT(parser.readUnsignedIntegerArray(), Equals(std::vector<unsigned int> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
      } else if(token == "realList") {
        CHECK_THAT(parser.readRealArray(), Equals(std::vector<real> {1.1, 1.2, 1.3, 1.4, 1.5, 1.0E12, 1.0e-12, 1.6, 1.7, 1. }));
      } else if(token == "vector2List") {
        parser.readVector2Array();
        //TODO: Check array of vector2
        //CHECK_THAT(parser.readVector2Array(), Equals(std::vector<vector2> {vector2(1.0, 2.0), vector2(3.0, 4.0)}));
      } else if(token == "vector3List") {
        parser.readVector3Array();
        //TODO: Check array of vector3
        //CHECK_THAT(parser.readVector3Array(), Equals(std::vector<vector3> {vector3(1.0, 2.0, 3.0), vector3(4.0, 5.0, 6.0)}));
      } else {
        //TODO: We should support a way of reading and discarding unused properties
      }


      if (parser.peekToken() == ",") {
        parser.readToken();
      }
    }
  }
}
