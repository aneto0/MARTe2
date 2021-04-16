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
#include "HighResolutionTimer.h"
#include "MutexSem.h"
#include "Sleep.h"
#include "Threads.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

CountingSemTest::CountingSemTest() {
}

CountingSemTest::~CountingSemTest() {
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

struct ThreadData {
  CountingSem *semaphore;
  MutexSem    *mutex;
  TimeoutType timeoutTime;
  uint32      sharedData;
};

void TestWaitForAllCallback(ThreadData & payload) {
    //Wait before proceeding...
    payload.semaphore->WaitForAll(payload.timeoutTime);
    payload.mutex->Lock();
    payload.sharedData ++;
    payload.mutex->UnLock();
    Threads::EndThread();
}

void killThreads() {
  Sleep::MSec(100);
  //Check if all the threads have terminated
  for (uint32 i = 0; i < Threads::NumberOfThreads(); i++) {
    ThreadIdentifier tid = Threads::FindByIndex(i);
    if (Threads::IsAlive(tid)) {
      Threads::Kill(tid);
    }
  }
}

bool CountingSemTest::TestWaitForAll(uint32 numberOfActors, TimeoutType timeoutTime) {

  if (numberOfActors == 0) return true; // Nothing todo in this case...
  
  // Initialize thread payload
  ThreadData payload = {
    new CountingSem(), 
    new MutexSem(), // mutex to protect access to sharedData sharedVariable
    timeoutTime, // max wait time..
    0xABCD // shared data
  };
  uint32 i = 0; // counter variable

  bool test = payload.semaphore->Create(numberOfActors); // initialize semaphore
  test &= payload.mutex->Create(); // Initialize mutex
  if (test) { 
    // prepare first N-1 treads
    for (i = 0; i < numberOfActors - 1; i++) {
      Threads::BeginThread((ThreadFunctionType) TestWaitForAllCallback, &payload);
      Sleep::MSec(10); // ensure that threads are waiting...
      if (payload.sharedData != 0xABCD) { // if value changed something has gone wrong... 
        killThreads();
        
        payload.semaphore->Close();
        payload.mutex->Close();
        delete payload.mutex;
        delete payload.semaphore;
        return false;
      }
    }
  } else {
    delete payload.mutex;
    delete payload.semaphore;
    return false;
  }
  // reinitialize data to 0
  payload.sharedData = 0;
  i++;
  // initialize last thread
  Threads::BeginThread((ThreadFunctionType) TestWaitForAllCallback, &payload);
  test &= !payload.semaphore->Reset();
  Sleep::MSec(100);
  test &= payload.semaphore->Reset();
  // after the synchronization the data should be equal to the num of threads.
  while (payload.sharedData != numberOfActors) { 
    Sleep::MSec(25);   
    i --;
    if (i <= 0) { 
      // if after N waiting period the value is not yet at the expected value 
      // something has gone wrong
      test = false;
    }
  }
   
  killThreads();
  // clean status
  payload.semaphore->Close();
  payload.mutex->Close();
  test &= payload.semaphore->IsClosed();
  delete payload.mutex;
  delete payload.semaphore;
  return test;
}


bool CountingSemTest::TestForcePass(uint32 numberOfActors, TimeoutType timeout) {
   
  if (numberOfActors == 0) return true; // Nothing todo in this case...
  
  // Initialize thread payload
  ThreadData payload = {
    new CountingSem(), 
    new MutexSem(), // mutex to protect access to sharedData sharedVariable
    timeout, // max wait time..
    0xABCD // shared data
  };
  uint32 i = 0; // counter variable

  // The number of actors setted in the semaphore is bigger then the real number
  bool test = payload.semaphore->Create(numberOfActors + 1); // initialize semaphore
  test &= payload.mutex->Create(); // Initialize mutex
  if (test) { 
    // prepare first N-1 treads
    for (i = 0; i < numberOfActors; i++) {
      Threads::BeginThread((ThreadFunctionType) TestWaitForAllCallback, &payload);
      Sleep::MSec(10); // ensure that threads are waiting...
      if (payload.sharedData != 0xABCD) { // if value changed something has gone wrong... 
        killThreads();
        
        payload.semaphore->Close();
        payload.mutex->Close();
        delete payload.mutex;
        delete payload.semaphore;
        return false;
      }
    }
  } else {
    delete payload.mutex;
    delete payload.semaphore;
    return false;
  }
  
  payload.sharedData = 0;
  test &= payload.semaphore->ForcePass();
  Sleep::MSec(500);
  test &= payload.sharedData == numberOfActors; 
  killThreads();
  // clean status
  payload.semaphore->Close();
  payload.mutex->Close();
  test &= payload.semaphore->IsClosed();
  delete payload.mutex;
  delete payload.semaphore;
  return test;
}



bool CountingSemTest::TestForceReset(uint32 numberOfActors, TimeoutType timeoutTime) {

  if (numberOfActors == 0) return true; // Nothing todo in this case...
  
  // Initialize thread payload
  ThreadData payload = {
    new CountingSem(), 
    new MutexSem(), // mutex to protect access to sharedData sharedVariable
    timeoutTime, // max wait time..
    0xABCD // shared data
  };
  uint32 i = 0; // counter variable

  bool test = payload.semaphore->Create(numberOfActors); // initialize semaphore
  test &= payload.mutex->Create(); // Initialize mutex
  if (test) { 
    // prepare first N-1 treads
    for (i = 0; i < numberOfActors - 1; i++) {
      Threads::BeginThread((ThreadFunctionType) TestWaitForAllCallback, &payload);
      Sleep::MSec(10); // ensure that threads are waiting...
      if (payload.sharedData != 0xABCD) { // if value changed something has gone wrong... 
        killThreads();
        
        payload.semaphore->Close();
        payload.mutex->Close();
        delete payload.mutex;
        delete payload.semaphore;
        return false;
      }
    }
  } else {
    delete payload.mutex;
    delete payload.semaphore;
    return false;
  }
  // reinitialize data to 0
  payload.sharedData = 0;
  i++;
  // initialize last thread
  Threads::BeginThread((ThreadFunctionType) TestWaitForAllCallback, &payload);
  test &= payload.semaphore->ForceReset();
  // after the synchronization the data should be equal to the num of threads.
  while (payload.sharedData != numberOfActors) { 
    Sleep::MSec(25);   
    i --;
    if (i <= 0) { 
      // if after N waiting period the value is not yet at the expected value 
      // something has gone wrong
      test = false;
    }
  }
   
  killThreads();
  // clean status
  payload.semaphore->Close();
  payload.mutex->Close();
  test &= payload.semaphore->IsClosed();
  delete payload.mutex;
  delete payload.semaphore;
  return test;
}
