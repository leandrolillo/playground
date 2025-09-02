#include <catch2/catch_test_macros.hpp>
#include "Paths.h"
#include "ResourceManagerMock.h"


TEST_CASE("Paths") {
  ResourceManagerMock resourceManager("resources");

  SECTION("Normalized Path") {
    String actual = Paths::normalized("");
    CHECK (actual == "");

    actual = Paths::normalized("children.txt");
    CHECK (actual == "children.txt");

    actual = Paths::normalized("/children.txt");
    CHECK (actual == "/children.txt");

    actual = Paths::normalized("~/children.txt");
    CHECK (actual == "/children.txt");

    actual = Paths::normalized("parent/children.txt");
    CHECK (actual == "parent/children.txt");

    actual = Paths::normalized("parent//children.txt");
    CHECK (actual == "parent/children.txt");

    actual = Paths::normalized("parent/../children.txt");
    CHECK (actual == "children.txt");

    REQUIRE_THROWS(Paths::normalized("parent/../../children.txt")); //do not go over the root

    //TODO: Not sure remaining tests add any value
    actual = Paths::normalized("parent/../parent/children.txt");
    CHECK (actual == "parent/children.txt");

    actual = Paths::normalized("~/parent/children.txt");
    CHECK (actual == "/parent/children.txt");

    actual = Paths::normalized("./parent/children.txt");
    CHECK (actual == "parent/children.txt");

    actual = Paths::normalized("./parent//children.txt");
    CHECK (actual == "parent/children.txt");

    actual = Paths::normalized("/parent/children.txt");
    CHECK (actual == "/parent/children.txt");

    actual = Paths::normalized("./grand-parent//parent/../parent/children.txt");
    CHECK (actual == "grand-parent/parent/children.txt");
  }

  SECTION("GetDirname") {
    String actual = Paths::getDirname("");
    CHECK("" == actual);

    actual = Paths::getDirname("/home");
    CHECK("/home" == actual);

    actual = Paths::getDirname(Paths::add(resourceManager.getRootFolder(), "fileToParse.txt"));
    CHECK(resourceManager.getRootFolder() == actual);

    /** If it does not exist, returns the path as is */ //TODO: Review if this is fine - dirname always removes the last portion.
    actual = Paths::getDirname(Paths::add(resourceManager.getRootFolder(), "tests/unexistingFileToParse.txt"));
    CHECK(Paths::add(resourceManager.getRootFolder(), "tests/unexistingFileToParse.txt") == actual);
  }

  SECTION("GetBasename") {
    String actual = Paths::getBasename("");
    CHECK("" == actual);

    actual = Paths::getBasename(resourceManager.getRootFolder() + "tests/fileToParse.txt");
    CHECK("fileToParse.txt" == actual);

    actual = Paths::getBasename(resourceManager.getRootFolder() + "a/very/long/path/with/subfolders/fileToParse.txt");
    CHECK("fileToParse.txt" == actual);


    actual = Paths::getBasename(resourceManager.getRootFolder() + "tests/unExistingFileToParse.txt");
    CHECK("unExistingFileToParse.txt" == actual);
  }

  SECTION("Add") {
    //relative path
    String actual = Paths::add("/home/assets", "resource.json");
    CHECK("/home/assets/resource.json" == actual);

    actual = Paths::add(" /home/assets/ ", " resource.json ");
    CHECK("/home/assets/resource.json" == actual);

    actual = Paths::add("/home/assets/", "./resource.json");
    CHECK("/home/assets/resource.json" == actual);

    actual = Paths::add("/home/assets", "./resource.json");
    CHECK("/home/assets/resource.json" == actual);

    //Absolute path
    actual = Paths::add("/home/assets", "/resource.json");
    CHECK("/resource.json" == actual);

    actual = Paths::add("/home/assets/", "/resource.json");
    CHECK("/resource.json" == actual);

    //relative to home
    actual = Paths::add("/home/assets", "~/resource.json");
    CHECK("/resource.json" == actual);

    actual = Paths::add("/home/assets/", "~/resource.json");
    CHECK("/resource.json" == actual);

    // edge cases
    actual = Paths::add("", "resource.json");
    CHECK("resource.json" == actual);

    actual = Paths::add("/home/assets/", "");
    CHECK("/home/assets" == actual);

    actual = Paths::add("", "");
    CHECK("" == actual);
  }
}
