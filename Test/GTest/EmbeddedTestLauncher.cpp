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

#include "stdio.h"
#include "stdlib.h"
#include "string.h"


#define TEST_DBGRANULARITY 64

typedef bool (*TestFunctionType)(void);

struct TestInfo {
    const char *className;
    const char *functionName;
    TestFunctionType function;
};

TestInfo *TestFunctions;
int numberOfTests = 0;

class AddTest {
public:
    AddTest(TestFunctionType x,
            const char *className,
            const char *functionName) {
        Allocate();
        TestFunctions[numberOfTests].function = x;
        TestFunctions[numberOfTests].className = className;
        TestFunctions[numberOfTests].functionName = functionName;
        numberOfTests++;
    }

private:
    void Allocate() {
        TestInfo* oldData = TestFunctions;
        int size=numberOfTests*sizeof(TestInfo);
        if ((numberOfTests % TEST_DBGRANULARITY) == 0) {
            TestFunctions = (TestInfo*) (/*pvPortMalloc*/malloc((numberOfTests + TEST_DBGRANULARITY) * sizeof(TestInfo)));

        }
        if (size != 0) {
            memcpy(TestFunctions, oldData, size);
            /*vPortFree*/free((void*) oldData);
        }
    }
};

#define TEST(className, functionName) \
    bool functionName (); \
    static AddTest ADD##functionName(functionName, #className, #functionName); \
    bool functionName ()

#define ASSERT_TRUE(boolean) return boolean

#define ASSERT_FALSE(boolean) return !boolean

#include "BasicTypeGTest.cpp"

int main() {
    for (int i = 0; i < numberOfTests; i++) {
        if (TestFunctions[i].function()) {
            // here print su uart
            printf("\n%s::%s Passed !!\n", TestFunctions[i].className, TestFunctions[i].functionName);
        }
        else {
            printf("\n%s::%s Failed!!\n", TestFunctions[i].className, TestFunctions[i].functionName);
        }
    }
    return 0;
}
