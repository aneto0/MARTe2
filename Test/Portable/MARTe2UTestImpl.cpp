/**
 * @file MARTe2UTestImpl.cpp
 * @brief Source file for class MARTe2UTestImpl
 * @date 14/06/2021
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

 * @details This source file contains the definition of all the methods for
 * the class MARTe2UTestImpl (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "CompilerTypes.h"
#include "MARTe2UTestMacros.h"

using namespace MARTe;

TestMarkerInterface::TestMarkerInterface(const char* suiteName, const char* testName) {
	this->testName = (char*)testName;
	this->suiteName = (char*)suiteName;
}

TestFramework::TestFramework() {
	statsPassedTests = 0;
	statsFailedTests = 0;
	statsTotalTests = 0;
}

void TestFramework::Add(TestMarkerInterface* miInstance) {
	testHolderDataStructure.Add(miInstance);
	statsTotalTests++;
}

void TestFramework::Run() {
	TestMarkerInterface* currentTest = NULL;
	uint32 dsSize = testHolderDataStructure.GetSize();
	printf("---TESTRESULTBEGIN---\r\n");
	for(uint32 i = 0; i < dsSize; i++) {
		currentTest = testHolderDataStructure[i];
		printf("[%d/%d] |%s|.|%s|", (i+1), statsTotalTests, currentTest->suiteName, currentTest->testName);
		currentTest->Test();
		if(currentTest->testResult) {
			passedHolderDataStructure.Add(currentTest);
			statsPassedTests++;
		}
		else {
			failedHolderDataStructure.Add(currentTest);
			statsFailedTests++;
		}
	}
	printf("---STATSDELIMITER---\r\n");
    PrintResults();
	printf("---TESTRESULTEND---\r\n");
}

void TestFramework::Clean() {
	failedHolderDataStructure.Clean();
	passedHolderDataStructure.Clean();
	testHolderDataStructure.Clean();

    statsPassedTests = 0;
	statsFailedTests = 0;
	statsTotalTests = 0;
}

void TestFramework::PrintResults() {
	MARTe::uint32 failedSize = failedHolderDataStructure.GetSize();

	for(MARTe::uint32 i = 0; i < failedSize; i++) {
		printf("Failed |%s|.|%s|\n", failedHolderDataStructure[i]->suiteName, failedHolderDataStructure[i]->testName);
	}
	printf("Stats:\r\n----------------------\n");
	printf("Total/Passed/Failed = %d/%d/%d\n", statsTotalTests, statsPassedTests, statsFailedTests);
}

TestFramework& TestFramework::Instance() {
	static TestFramework instance;
	return instance;
}

Bootstrapper::Bootstrapper(TestMarkerInterface* inFun) {
	TestFramework::Instance().Add(inFun);
}
