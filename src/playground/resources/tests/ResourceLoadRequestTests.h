/*
 * ResourceManagerTests.h
 *
 *  Created on: Oct 25, 2022
 *      Author: leandro
 */

#pragma once

#include <Tests.h>
#include "../ResourceLoadRequest.h"

class ResourceLoadRequestTests: public UnitTest
{
public:

	ResourceLoadRequestTests() {
        logger = LoggerFactory::getLogger("ResourceLoadRequestTests");
        logger->addAppender(LoggerFactory::getAppender("stdout"));

        this->addTest("ResourceLoadRequestTests::testToString", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ResourceLoadRequestTests::testToString));
        this->addTest("ResourceLoadRequestTests::testIsValid", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ResourceLoadRequestTests::testIsValid));
        this->addTest("ResourceLoadRequestTests::testFilePath", static_cast<void (UnitTest::*)(PlaygroundRunner *)>(&ResourceLoadRequestTests::testFilePath));
    }

	void testToString(PlaygroundRunner *runner) {
		ResourceLoadRequest request("/test/filename.json");
		String actual = request.acceptMimeType("test/mimetype").withAdditionalLabels(std::set<String> {"additionalLabel"}).toString();
		assertEquals("Unexpected toString", "[test/mimetype]<-[application/json] [/test/filename.json]", actual);
	}

	void testIsValid(PlaygroundRunner *runner) {
		ResourceLoadRequest request("tests/fileToParse.txt");
		bool actual = request.acceptMimeType("test/mimetype").withAdditionalLabels(std::set<String> {"additionalLabel"}).isValid();
		assertTrue(assertMessage(request.errors()), actual);

		/** Should be able to guess mimetype from filename even with object name*/
		ResourceLoadRequest requestAgain("tests/fileToParse.txt/objectName");
		actual = requestAgain.withParent(runner->getResourceManager()->getRootFolder()).acceptMimeType("test/mimetype").withAdditionalLabels(std::set<String> {"additionalLabel"}).isValid();
		assertTrue(assertMessage(requestAgain.errors()), actual);

		ResourceLoadRequest anotherRequest("");
		actual = anotherRequest.isValid();
		assertTrue(assertMessage(anotherRequest.errors()), !actual);

		ResourceLoadRequest yetAnotherRequest("/tests/filename");
		actual = yetAnotherRequest.acceptMimeType("test/mimetype").isValid();
		assertTrue(assertMessage(yetAnotherRequest.errors()), !actual); //expect could not guess input mimetype
	}

	void testFilePath(PlaygroundRunner *runner) {
		/* Just filename*/
		ResourceLoadRequest request("/tests/filename.json");
		String actual = request.getFilePath();
		assertEquals(defaultAssertMessage, "/tests/filename.json", actual);

		/* Uri with name - relative to root */
		ResourceLoadRequest requestB("tests/fileToParse.txt/name");
		actual = requestB.withParent(runner->getResourceManager()->getRootFolder()) .getFilePath();
		assertEquals(defaultAssertMessage, Paths::add(runner->getResourceManager()->getRootFolder(), "tests/fileToParse.txt"), actual);

		/* Uri with name - and parent - returns the object as well since filename does not exist */
		ResourceLoadRequest requestC("tests/filename.json/name");
		actual = requestC.withParent("/home").withParent("leandro").getFilePath();
		assertEquals(defaultAssertMessage, "/home/leandro/tests/filename.json/name", actual);
	}

	void testSimpleUri(PlaygroundRunner *runner) {
//		Resource parent(0, "test/mimetype");
//		parent.setFileName("/tests/filename.json");
//
//		ResourceLoadRequest request("child/filename.json");
//		//test fileParser
//
//
//
//		ResourceLoadRequest requestWithParent("/test/filename.json");
//		requestWithParent.withParent(&parent);
//		request.getFilePath() == "/tests/child/filename.json";
//		//test fileParser
//
//		ResourceLoadRequest requestFromFileParser(fileparser);
//		requestWithParent.withParent(&parent);
//		request.getFilePath() == "/tests/child/filename.json";
//		//test fileParser
	}


};
