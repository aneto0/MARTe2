/**
 * @file MARTe2UTestMacros.h
 * @brief Header file for module MARTe2UTestMacros
 * @date 12/06/2021
 * @author Giuseppe Avon
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the module MARTe2UTestMacros
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MARTE2_UNIT_TEST_MACROS_H_
#define MARTE2_UNIT_TEST_MACROS_H_

/* Approximately define the number of tests to correctly size the output array and avoid continuous realloc  */
#define TEST_ESTIMATED_NUMBER		 	5000u

/* Estimate the failure rate in order to set the size of the fail holder array */
#define TEST_ESTIMATED_FAILURE_RATE     0.1

#define TEST_SIZE_GRANULARITY		TEST_ESTIMATED_NUMBER
#define TEST_FAIL_SIZE_GRANULARITY	static_cast<MARTe::uint32>(TEST_ESTIMATED_NUMBER * TEST_ESTIMATED_FAILURE_RATE)

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CompilerTypes.h"
#include "StaticList.h"

/*---------------------------------------------------------------------------*/
/*                           Module declaration                              */
/*---------------------------------------------------------------------------*/

/**
 * @brief 	Interface to tag every test and enable access to the Test() method
 * 			which calls the test unit. Keeps the suite (group) and test name alongside
 * 			with the test result.
 */
class TestMarkerInterface {
    public:
        char* suiteName;
        char* testName;
        bool testResult;
        
		/**
		 * @brief Construct a new Test Marker Interface object
		 * 
		 * @param suiteName Name of the test suite or group
		 * @param testName Name of the test unit
		 */
        TestMarkerInterface(const char* suiteName, const char* testName);

		/**
		 * @brief Runs the test
		 */
        virtual void Test() = 0;
};

/**
 * @brief 	Offers facilities to Add() the tests from the macros which automatically
 * 			populate the internal StaticLists and offers to the caller the Run() and
 * 			Clean() method to execute the tests and clean-up internals.
 * 			Keeps track of the executed/passed/failed tests to generate a report.
 */
class TestFramework {
	public:
		/**
		 * @brief Adds a test which has to be executed from the framework
		 * @param miInstance Pointer to the marker interface which wraps the Test() function
		 */
		void Add(TestMarkerInterface* miInstance);

		/**
		 * @brief 	Run, by iteratively calling Test() on all the previously added
		 * 			tests. Keeps track of the execution result, storing test group and unit
		 * 			name internally.
		 */
		void Run();

		/**
		 * @brief 	Empties all the internal structures used to hold tests, also passed
		 * 			and failed units.
		 */
		void Clean();

		/**
		 * @brief 	Prints a resume of executed/passed/failed highlighting names on the failed units.
		 */
		void PrintResults();

		/**
		 * @brief Singleton pattern instance getter method
		 * @return TestFramework& The one and only instance of the TestFramework
		 */
        static TestFramework& Instance();

    private:
		/**
		 * @brief Number of total tests, counted when tests are added
		 */
		int statsTotalTests;

		/**
		 * @brief Number of passed tests, valid only after Run()
		 */
		int statsPassedTests;

		/**
		 * @brief Number of failed tests, valid only after Run()
		 * 
		 */
		int statsFailedTests;

		/**
		 * @brief Holds all the tests which can be run
		 */
		MARTe::StaticList<TestMarkerInterface*, TEST_SIZE_GRANULARITY> testHolderDataStructure;

		/**
		 * @brief Holds the tests which reported run failure
		 */
		MARTe::StaticList<TestMarkerInterface*, TEST_FAIL_SIZE_GRANULARITY> failedHolderDataStructure;

		/**
		 * @brief Holds the tests which reported succesful run
		 */
		MARTe::StaticList<TestMarkerInterface*, TEST_SIZE_GRANULARITY> passedHolderDataStructure;

		/**
		 * @brief Construct a new Test Framework object
		 */
		TestFramework();
};

/**
 * @brief Class used by the macro. Every test unit creates an instance of this class
 * 		  to bind itself to the auto-construction sequence
 */
class Bootstrapper {
	public:
		/**
		 * @brief Construct a new Bootstrapper object
		 * @param inFun The interface where the test method resides
		 */
		Bootstrapper(TestMarkerInterface* inFun);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#define ASSERT_TRUE(x) this->testResult = x; printf("%s", (this->testResult)?" PASS\n":" FAIL!\n")
#define ASSERT_FALSE(x) this->testResult = !x; printf("%s", (this->testResult)?" PASS\n":" FAIL!\n")

#define TEST(x,y) 	\
class Tester_##x##_##y : public TestMarkerInterface {\
	public:  \
		Tester_##x##_##y() : TestMarkerInterface(#x, #y) {} \
		virtual void Test(); \
}; \
static Tester_##x##_##y Test_##x##_##y; \
Bootstrapper Bootstrapper_##x##_##y(&Test_##x##_##y); \
void Tester_##x##_##y::Test()

#endif /* MARTE2_UNIT_TEST_MACROS_H_*/
