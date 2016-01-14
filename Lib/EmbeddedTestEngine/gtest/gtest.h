/**
 * @file EmbeddedTestLauncher.cpp
 * @brief Source file for class EmbeddedTestLauncher
 * @date 16/12/2015
 * @author Giuseppe Ferrò
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
 * the class EmbeddedTestLauncher (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
#ifndef GTEST_H
#define GTEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"

#define TEST_DBGRANULARITY 64
#define MAX_NUMBER_TESTS 4096
#define MAX_NUMBER_FAIL_TESTS 64

typedef void (*TestFunctionType)(void);

struct TestInfo {
    const char *className;
    const char *functionName;
    TestFunctionType function;
};

//extern TestInfo TestFunctions[MAX_NUMBER_TESTS];
extern TestInfo *TestFunctions;

extern int FailedTestFunctions[MAX_NUMBER_FAIL_TESTS];
extern volatile int numberOfTests;
extern bool TestResult;

class AddTest {
public:
    AddTest(TestFunctionType x,
            const char *className,
            const char *functionName) {
        Allocate();
        if (numberOfTests < MAX_NUMBER_TESTS) {
            TestFunctions[numberOfTests].function = x;
            TestFunctions[numberOfTests].className = className;
            TestFunctions[numberOfTests].functionName = functionName;
            numberOfTests++;
        }
    }

    ~AddTest() {
        if (TestFunctions) {
            free(TestFunctions);
        }
    }

private:
    void Allocate() {
        if ((numberOfTests % TEST_DBGRANULARITY) == 0) {
            TestFunctions = (TestInfo*) (realloc(TestFunctions, (numberOfTests + TEST_DBGRANULARITY) * sizeof(TestInfo)));
        }
    }

};

#define TEST(className, functionName) \
    void className##_##functionName (); \
    static AddTest ADD_##functionName(className##_##functionName, #className, #functionName);\
    void className##_##functionName ()

#define ASSERT_TRUE(boolean) TestResult&=boolean;

#define ASSERT_FALSE(boolean) TestResult&=!boolean;

#endif
