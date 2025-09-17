#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "Playground.h"
#include "chrono"
#include "thread"

TEST_CASE("Playground Test case")
{
  Playground playground("");
  REQUIRE(1 == 1);

  CHECK(playground.toString() != "");
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
