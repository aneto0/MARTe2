/**
 * @file MutexTest.cpp
 * @brief Source file for class MutexTest
 * @date 25/06/2015
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
 * the class MutexTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MutexTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MutexTest::MutexTest() {
    mutexSem.Create();
}

MutexTest::~MutexTest() {
    //mutexSem.Close();
}

bool MutexTest::TestLock(TimeoutType timeout) {

    //make a copy of the semaphore
    MutexSem newSem(mutexSem);


    Error myErrorReturn = Debug;
    //lock with infinite timeout.
    if (!mutexSem.Lock()) {
        return False;
    }

    if (!mutexSem.IsRecursive()) {
        if (mutexSem.FastTryLock()) {
            return False;
        }

        //it should return false after the timeout.
        if (mutexSem.Lock(timeout, myErrorReturn)) {
            return False;
        }

        if (myErrorReturn != Timeout) {
            return False;
        }
    }

    if (!newSem.UnLock()) {
        return False;
    }

    return True;

}

bool MutexTest::TestUnLock() {

    Error myErrorReturn;

    if (!mutexSem.Lock(TTInfiniteWait, myErrorReturn)) {

        return False;
    }

    if (myErrorReturn != Debug) {
        return False;
    }

    //the unlock should allow a new lock without errors
    if (!mutexSem.UnLock()) {

        return False;
    }

    if (!mutexSem.Lock(TTInfiniteWait, myErrorReturn)) {

        return False;
    }

    if (myErrorReturn != Debug) {

        return False;
    }

    if (!mutexSem.FastUnLock()) {

        return False;
    }

    if (!mutexSem.FastLock(TTInfiniteWait, myErrorReturn)) {

        return False;
    }

    if (myErrorReturn != Debug) {

        return False;
    }

    if (!mutexSem.FastUnLock()) {
        return False;
    }

    return True;
}
