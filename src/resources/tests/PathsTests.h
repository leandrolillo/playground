/*
 * PathsTests.h
 *
 *  Created on: Oct 24, 2022
 *      Author: leandro
 */

#ifndef SRC_RESOURCES_TESTS_PATHSTESTS_H_
#define SRC_RESOURCES_TESTS_PATHSTESTS_H_

#include <Tests.h>
#include <Paths.h>

class PathsTests : public UnitTest {
public:
	PathsTests() {
        logger = LoggerFactory::getLogger("PathsTests");
        logger->addAppender(LoggerFactory::getAppender("stdout"));

        this->addTest("Paths::add", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&PathsTests::testAdd));
        this->addTest("Paths::getDirname", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&PathsTests::testGetDirname));
        this->addTest("Paths::getBasename", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&PathsTests::testGetBasename));
    }

	void testGetDirname(PlaygroundRunner *runner) {
		String actual = Paths::getDirname("");
		assertEquals(defaultAssertMessage, "", actual);

		actual = Paths::getDirname("/home");
		assertEquals(defaultAssertMessage, "/home", actual);

		actual = Paths::getDirname(Paths::add(runner->getResourceManager()->getRootFolder(), "tests/fileToParse.txt"));
		assertEquals(defaultAssertMessage, Paths::add(runner->getResourceManager()->getRootFolder(), "tests"), actual);

		/** If it does not exist, returns the path as is */
		actual = Paths::getDirname(Paths::add(runner->getResourceManager()->getRootFolder(), "tests/unexistingFileToParse.txt"));
		assertEquals(defaultAssertMessage, Paths::add(runner->getResourceManager()->getRootFolder(), "tests/unexistingFileToParse.txt"), actual);

	}

	void testGetBasename(PlaygroundRunner *runner) {
		String actual = Paths::getBasename("");
		assertEquals(defaultAssertMessage, "", actual);

		actual = Paths::getBasename(runner->getResourceManager()->getRootFolder() + "tests/fileToParse.txt");
		assertEquals(defaultAssertMessage, "fileToParse.txt", actual);

		/** If file does not exist, what should this return?*/
		actual = Paths::getBasename(runner->getResourceManager()->getRootFolder() + "tests/unExistingFileToParse.txt");
		assertEquals(defaultAssertMessage, "unExistingFileToParse.txt", actual);
	}

    void testAdd(PlaygroundRunner *runner)
    {
    	//relative path
    	String actual = Paths::add("/home/assets", "resource.json");
    	assertEquals(defaultAssertMessage, "/home/assets/resource.json", actual);

    	actual = Paths::add(" /home/assets/ ", " resource.json ");
    	assertEquals(defaultAssertMessage, "/home/assets/resource.json", actual);

    	actual = Paths::add("/home/assets/", "./resource.json");
    	assertEquals(defaultAssertMessage, "/home/assets/resource.json", actual);

    	actual = Paths::add("/home/assets", "./resource.json");
    	assertEquals(defaultAssertMessage, "/home/assets/resource.json", actual);

    	//Absolute path
    	actual = Paths::add("/home/assets", "/resource.json");
    	assertEquals(defaultAssertMessage, "/resource.json", actual);

    	actual = Paths::add("/home/assets/", "/resource.json");
    	assertEquals(defaultAssertMessage, "/resource.json", actual);

    	//relative to home
    	actual = Paths::add("/home/assets", "~/resource.json");
    	assertEquals(defaultAssertMessage, "/home/assets/resource.json", actual);

    	actual = Paths::add("/home/assets/", "~/resource.json");
    	assertEquals(defaultAssertMessage, "/home/assets/resource.json", actual);

    	actual = Paths::add("./target/../../media", "~/images/basketball.png");
    	assertEquals(defaultAssertMessage, "./target/../../media/images/basketball.png", actual);

    	// edge cases
    	actual = Paths::add("", "resource.json");
    	assertEquals(defaultAssertMessage, "resource.json", actual);

    	actual = Paths::add("/home/assets/", "");
    	assertEquals(defaultAssertMessage, "/home/assets/", actual);

    	actual = Paths::add("", "");
    	assertEquals(defaultAssertMessage, "", actual);

    }

};



#endif /* SRC_RESOURCES_TESTS_PATHSTESTS_H_ */
