#include <catch2/catch_test_macros.hpp>
#include "Playground.h"
#
TEST_CASE("Playground Test case")
{
  Playground playground("");
  REQUIRE(1 == 1);
}
