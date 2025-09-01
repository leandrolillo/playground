#include <catch2/catch_test_macros.hpp>
#include "Paths.h"
#include "ResourceManagerMock.h"


TEST_CASE("Paths") {
  ResourceManagerMock resourceManager("resources");

  SECTION("GetDirname") {
    String actual = Paths::getDirname("");
    CHECK("" == actual);

    actual = Paths::getDirname("/home");
    CHECK("/home" == actual);

    actual = Paths::getDirname(Paths::add(resourceManager.getRootFolder(), "fileToParse.txt"));
    CHECK(resourceManager.getRootFolder() == actual);

    /** If it does not exist, returns the path as is */
    actual = Paths::getDirname(Paths::add(resourceManager.getRootFolder(), "tests/unexistingFileToParse.txt"));
    CHECK(Paths::add(resourceManager.getRootFolder(), "tests/unexistingFileToParse.txt") == actual);
  }

  SECTION("GetBasename") {
    String actual = Paths::getBasename("");
    CHECK("" == actual);

    actual = Paths::getBasename(resourceManager.getRootFolder() + "tests/fileToParse.txt");
    CHECK("fileToParse.txt" == actual);

    /** If file does not exist, what should this return?*/
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
    CHECK("/home/assets/resource.json" == actual);

    actual = Paths::add("/home/assets/", "~/resource.json");
    CHECK("/home/assets/resource.json" == actual);

    actual = Paths::add("./target/../../media", "~/images/basketball.png");
    CHECK("./target/../../media/images/basketball.png" == actual);

    // edge cases
    actual = Paths::add("", "resource.json");
    CHECK("resource.json" == actual);

    actual = Paths::add("/home/assets/", "");
    CHECK("/home/assets/" == actual);

    actual = Paths::add("", "");
    CHECK("" == actual);
  }
}
