/*
 * ResourceLoadResponseTest.h
 *
 *  Created on: Sep 4, 2023
 *      Author: llillo
 */

#pragma once

#include <Tests.h>
#include <ResourceLoadResponse.h>
#include "mock/ResourceManagerMock.h"


class ResourceLoadResponseTests: public UnitTest
{
public:

	ResourceLoadResponseTests() {
        logger = LoggerFactory::getLogger("ResourceLoadResponseTests");
        logger->addAppender(LoggerFactory::getAppender("stdout"));

        this->addTest("ResourceLoadResponseTests::testFilePath", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ResourceLoadResponseTests::testFilePath));
        this->addTest("ResourceLoadResponseTests::issueWithRootPathsBeingRelative", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ResourceLoadResponseTests::issueWithRootPathsBeingRelative));
    }

	void testFilePath(PlaygroundRunner *runner) {
		ResourceManagerMock resourceManager("./target/../../media");
		ResourceLoadRequest request("geometry/basketball.json");

		ResourceLoadResponse response(request, resourceManager);

		String actual = response.getFullPath("~/images/basketball.png");

		assertEquals(defaultAssertMessage, (String)std::__fs::filesystem::absolute("./target/../../media") + "/images/basketball.png", actual);

	}


	void issueWithRootPathsBeingRelative(PlaygroundRunner *runner) {
		String rootFolder("./target/../../media");

		ResourceManagerMock resourceManager(rootFolder);
		ResourceLoadRequest request("geometry/basketball.json");
		ResourceLoadResponse response(request, resourceManager);

		String actual = response.getFullPath("~/images/basketball.png");
		actual = Paths::normalize(actual, rootFolder);

		assertEquals(defaultAssertMessage, (String)std::__fs::filesystem::absolute(rootFolder) + "/images/basketball.png", actual);
		//assertEquals(defaultAssertMessage, rootFolder + "/images/basketball.png", actual); // this is the failure before the fix in resource manager constructor.

	}

};
