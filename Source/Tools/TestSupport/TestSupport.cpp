/**
 * @file TestSupport.cpp
 * @brief Header file for class AnyType
 * @date 2 Jun 2019
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#include "TestSupport.h"
#include <stdio.h>

class TestInstance;
TestInstance *TestRoot = NULL;
bool TestSupportShowResults = true;


TestInstance::TestInstance(){
	nextTest = TestRoot;
	TestRoot = this;
	testName = "";
}
TestInstance::~TestInstance(){
}

FILE *testResultsFile = NULL;
FILE *testListFile = NULL;
FILE *testDetailsFile = NULL;

FILE *getTestResultsFile(){
	if (testResultsFile == NULL){
		testResultsFile = fopen("testResults.lst","w");
	}
	return testResultsFile;
}
FILE *getTestListFile(){
	if (testListFile == NULL){
		testListFile = fopen("testList.lst","w");
	}
	return testListFile;
}
FILE *getTestDetailsFile(){
	if (testDetailsFile == NULL){
		testDetailsFile = fopen("testDetails.lst","w");
	}
	return testDetailsFile;
}

void RunAllTests(){
	FILE *testResultsFile 	= getTestResultsFile();
	FILE *testListFile 	= getTestListFile();
	FILE *testDetailsFile 	= getTestDetailsFile();

	TestInstance *test = TestRoot;
	int counter = 0;
	int failedCounter = 0;
	while (test != NULL){
		bool res = true;
		printf("Starting %s ... ",test->testName);
		test->Execute(res);
		if (res){
			printf(": OK  done\n");
			fprintf(testResultsFile," OK  %s\n",test->testName);
			fprintf(testDetailsFile," OK  %s\n",test->testName);
		} else {
			printf(": NO failed  \n");
			fprintf(testResultsFile,"*NO* %s\n",test->testName);
			fprintf(testDetailsFile,"*NO* %s\n",test->testName);
			failedCounter++;
		}
		fflush(testResultsFile);
		test = test->nextTest;
		counter ++;
	}
	fprintf(testResultsFile,"executed %i tests - %i failed \n",counter,failedCounter);
	fclose(testResultsFile);
	fclose(testListFile);
	fclose(testDetailsFile);
}
