/**
 * @file CountingSemTest.cpp
 * @brief Source file for class CountingSemTest
 * @date 12/04/2021
 * @author Martino Ferrari
 *
 * @copyright Copyright 2021 F4E | European Joint Undertaking for ITER and
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
 * the class CountingSemTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "CountingSemTest.h"
#include "ErrorType.h"
#include "Threads.h"
#include "Sleep.h"
#include "HighResolutionTimer.h"
#include "MutexSem.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

CountingSemTest::CountingSemTest() {
    sharedVariable = 0;
    sharedVariable1 = 0;
    timeout = TTInfiniteWait;
}

CountingSemTest::~CountingSemTest() {
    countingSem.Close();
}

bool CountingSemTest::TestConstructor() {
    CountingSem testSem;
    return testSem.IsClosed();
}

bool CountingSemTest::TestDestructor() {
    CountingSem testSem;
    testSem.Create(1);
    bool test = !testSem.IsClosed();
    testSem.~CountingSem();
    test &= testSem.IsClosed();
    return test;
}

bool CountingSemTest::TestCreate(uint32 numberOfActors) {
    CountingSem testSem;
    bool test = testSem.Create(numberOfActors);
    if (test) {
        test &= (testSem.GetProperties() != NULL);
    }
    testSem.Close();
    return test;
}

bool CountingSemTest::TestWaitForAll(uint32 numberOfActors, TimeoutType timeoutTime) {
  return true;
}



bool CountingSemTest::TestReset() {
  return true;
}

bool CountingSemTest::TestForceReset() {
  return true;
}

bool CountingSemTest::TestForcePass() {
  return true;
}


