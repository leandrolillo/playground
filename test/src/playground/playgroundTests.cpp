#include <catch2/catch_test_macros.hpp>
#include "Playground.h"

TEST_CASE("Playground Test case")
{
  Playground playground("");
  REQUIRE(1 == 1);
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
  Chronometer stopwatch;

  real elapsed = 0;
  stopwatch.start();
  do {
    elapsed += stopwatch.update();
  } while (stopwatch.getTotalTime() < 10.0f);

  CHECK(stopwatch.getTotalTime() == elapsed);
}
