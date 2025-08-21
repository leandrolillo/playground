/*
 * TestRunner.h
 *
 *  Created on: 06/02/2013
 *      Author: Lean
 */

#ifndef TESTRUNNER_H_
#define TESTRUNNER_H_

#include<Math3d.h>
#include<Logger.h>
#include<Playground.h>

#include<map>

#define defaultAssertMessage StringFormatter::format("Assertion Failed in [%s], at line [%d]", __FILE__, __LINE__)
#define assertMessage(message) StringFormatter::format("Assertion Failed in [%s], at line [%d] ", __FILE__, __LINE__).append(message)

class UnitTest {
    std::map<String, void (UnitTest::*)(PlaygroundRunner *)> tests;
protected:
    Logger *logger = LoggerFactory::getLogger("UnitTest");

public:
    UnitTest() {
        logger->addAppender(LoggerFactory::getAppender("stdout"));
    }

    String toString() {
        return logger->getBasename();
    }

    void addTest(String name, void (UnitTest::*testFunction)(PlaygroundRunner *))
    {
        tests[name] = testFunction;
    }

    unsigned int getNumberOfTests() {
        return this->tests.size();
    }

    std::vector<String> run(PlaygroundRunner *runner) {
        std::vector<String> failedTests;

        //logger->info("Running [%d] Tests...", getNumberOfTests());

        for(auto iterator : tests)
        {
            try {
                logger->debug("- Running test [%s]...", iterator.first.c_str());
                void (UnitTest::*currentTest)(PlaygroundRunner *);
                currentTest = iterator.second;

                (this->*currentTest)(runner);

                //logger->info("Test [%s] passed.", iterator.first.c_str());
            } catch (Exception &e) {
                logger->error("TEST FAILED:[%s] FAILED: %s", iterator.first.c_str(), e.toString().c_str());
                failedTests.push_back(iterator.first);
            } catch(...) {
                logger->error("TEST FAILED:[%s] FAILED", iterator.first.c_str());
                failedTests.push_back(iterator.first);
            }

        }

        return failedTests;
    }

    virtual ~UnitTest() {

    }

    void assertTrue(bool condition)
    {
        assertTrue("", condition);
    }
    void assertTrue(const String &message, bool condition)
    {
        if(!condition) {
            assertFail(message);
        }
    }

    void assertEquals(const String &message, const String &expected, const String &actual)
    {
        assertTrue(message + ". Expected: [" + expected + "]. Actual: [" + actual + "]", expected == actual);
    }

    void assertEquals(const String &message, void *expected, void *actual)
    {
        assertTrue(message, expected == actual);
    }

    void assertEquals(const String &message, unsigned int expected, unsigned int actual)
    {
        assertTrue(StringFormatter::format("%s. Expected: [%d]. Actual: [%d]", message.c_str(), expected, actual), expected == actual);
    }

    void assertRealEquals(const String &message, real expected, real actual)
    {
        assertTrue(StringFormatter::format("%s. Expected: [%f]. Actual: [%f]", message.c_str(), expected, actual), equalsZero(expected - actual));
    }

    void assertFalse(const String &message, bool condition)
    {
        if(condition)
        	assertFail(message);
    }
    void assertFalse(bool condition)
    {
        assertFalse("", condition);
    }

    void assertFail(const String &message) {
        throw Exception(message.c_str());
    }

    void assertNotNull(const String &message, void *pointer) {
    	assertFalse(StringFormatter::format("%s. Expected: [%d]. Actual: [%d]", message.c_str(), null, pointer), pointer == null);
    }

    void assertNull(const String &message, void *pointer) {
    	assertEquals(message, null, pointer);
    }

    void assertEquals(const String &message, const vector &expected, const vector &actual)
    {
        String vectorMessage = StringFormatter::format("%s. Expected %s, got %s:",
                message.c_str(),
                expected.toString("%.2f").c_str(),
                actual.toString("%.2f").c_str());
        assertRealEquals(vectorMessage, expected.x, actual.x);
        assertRealEquals(vectorMessage, expected.y, actual.y);
        assertRealEquals(vectorMessage, expected.z, actual.z);
    }

    void assertEquals(const String &message, const matriz &expected, const matriz &actual)
        {
            String vectorMessage = StringFormatter::format("%s. Expected:\n%s, got\n%s",
                    message.c_str(),
                    expected.toString("%.2f").c_str(),
                    actual.toString("%.2f").c_str());
            assertRealEquals(vectorMessage + "Element (0, 0): ", expected(0,0), actual(0,0));
            assertRealEquals(vectorMessage + "Element (0, 1)", expected(0,1), actual(0,1));
            assertRealEquals(vectorMessage + "Element (0, 2)", expected(0,2), actual(0,2));
            assertRealEquals(vectorMessage + "Element (0, 3)", expected(0,3), actual(0,3));

            assertRealEquals(vectorMessage + "Element (1, 0)", expected(1,0), actual(1,0));
            assertRealEquals(vectorMessage + "Element (1, 1)", expected(1,1), actual(1,1));
            assertRealEquals(vectorMessage + "Element (1, 2)", expected(1,2), actual(1,2));
            assertRealEquals(vectorMessage + "Element (1, 3)", expected(1,3), actual(1,3));

            assertRealEquals(vectorMessage + "Element (2, 0)", expected(2,0), actual(2,0));
            assertRealEquals(vectorMessage + "Element (2, 1)", expected(2,1), actual(2,1));
            assertRealEquals(vectorMessage + "Element (2, 2)", expected(2,2), actual(2,2));
            assertRealEquals(vectorMessage + "Element (2, 3)", expected(2,3), actual(2,3));

            assertRealEquals(vectorMessage + "Element (3, 0)", expected(3,0), actual(3,0));
            assertRealEquals(vectorMessage + "Element (3, 1)", expected(3,1), actual(3,1));
            assertRealEquals(vectorMessage + "Element (3, 2)", expected(3,2), actual(3,2));
            assertRealEquals(vectorMessage + "Element (3, 3)", expected(3,3), actual(3,3));
        }

    void assertEquals(const String &message, const matriz_mxn &expected, const matriz_mxn &actual)
    {
        String text = StringFormatter::format("%s. Expected dimensions: [%dx%d]. Actual: [%dx%d]", message.c_str(), expected.getNroFilas(), expected.getNroColumnas(), actual.getNroFilas(), actual.getNroColumnas());
        assertTrue(text, expected.getNroFilas() == actual.getNroFilas() && expected.getNroColumnas() == expected.getNroColumnas());

        for(unsigned int i = 0; i < expected.getNroFilas(); i++) {
            for(unsigned int j = 0; j < expected.getNroColumnas(); j++) {
                text = StringFormatter::format("%s. Element (%d, %d) - Expected: [%.3f]. Actual: [%.3f]", message.c_str(), i, j, expected(i, j), actual(i, j));
                assertTrue(text, expected(i, j) == actual(i, j));
            }
        }
    }
private:
    String getFailedTestsString(std::vector<String> &stringList)
    {
        String result(stringList.empty() ? "" :  ": ");

        for(std::vector<String>::iterator iterator = stringList.begin(); iterator != stringList.end(); iterator++)
        {
            if(iterator != stringList.begin())
                result.append(", ");

            result.append(*iterator);
        }

        return result;
    }

};

class TestsManager {
	private:
		Logger *logger = LoggerFactory::getLogger("TestsManager");
		std::vector<UnitTest> unitTests;
		PlaygroundRunner *runner = null;
	public:
		TestsManager(PlaygroundRunner *runner)
		{
			logger->addAppender(LoggerFactory::getAppender("stdout"));
			this->runner = runner;
		}

		void addTest(UnitTest &unitTest)
		{
			unitTests.push_back(unitTest);
		}

		void doTests() {
		    std::vector<String> allFailedTests;

		    unsigned int totalTests = 0;
		    for(auto unitTest : unitTests) {
		        totalTests += unitTest.getNumberOfTests();
		    }

			logger->info("Running [%d] tests from [%d] test packs...", totalTests, unitTests.size());
			for(auto unitTest : unitTests) {
			    logger->info("Running tests pack [%s] ...", unitTest.toString().c_str());
			    std::vector<String> failedTests = unitTest.run(runner);

			    allFailedTests.insert(std::begin(allFailedTests), std::begin(failedTests), std::end(failedTests));

			    logger->info("\n");
			    //logger->info("Done running [%s] test pack...", unitTest.toString().c_str());
			}

			logger->info("Results:");

			if(!allFailedTests.empty()) {
			    logger->error("  Tests in error:");

			    for(auto failedTest : allFailedTests) {
			        logger->error("  - %s", failedTest.c_str());
			    }
			}

			logger->error("Tests run: [%d], Failures: [%d]", totalTests, allFailedTests.size());
		}
};


#endif /* TESTRUNNER_H_ */
