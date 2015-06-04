/*
 * Copyright 2015 F4E | European Joint Undertaking for 
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent  
 versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
 Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is 
 distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied. 
 * See the Licence  
 permissions and limitations under the Licence. 
 *
 * $Id: Endianity.h 3 2012-01-15 16:26:07Z aneto $
 *
 **/
/**
 *  @file 
 *  @brief Linux implementation of the mutex semaphore.
 */

#ifndef MUTEX_SEM_OS_H
#define MUTEX_SEM_OS_H

#include "../../GeneralDefinitions.h"
#include "cmsis_os.h"

/** @brief Private mutex semaphore class used for Solaris and Linux when using pThread. */
class PrivateMutexSemStruct {
    /**  Mutex Handle */
    osMutexId mutexHandle;
    /** Mutex Attributes */
    pthread_mutexattr_t mutexAttributes;
public:
    /** @brief Constructor. */
    PrivateMutexSemStruct() {
    }
    /** @brief Destructor. */
    ~PrivateMutexSemStruct() {
    }

    /** @brief Initialize the semaphore with the right attributes.
      * @return false if something wrong with pthread_mutex initialization. */
    bool Init() {

	const osMutexDef_t mutexDefinition = {0};	

       	return mutexHandle=osMutexCreate(mutexDefinition) != NULL; 
    }

    /** @brief Destroy the semaphore.
      * @return false if something wrong in pthread_mutex destruction. */
    bool Close() {
	return osMutexDelete(mutexHandle)==0;
    }

    /** @brief Lock the semaphore until an unlock or the timeout expire.
      * @param msecTimeout is the desired timeout.
      * @return false if lock fails also because the expire of the timeout, true otherwise.
      * The thread that locks a semaphore cannot be killed. */
    bool Lock(TimeoutType msecTimeout = TTInfiniteWait) {
        if (msecTimeout == TTInfiniteWait) {
    		return osMutexWait(mutexHandle, osWaitForever)==0; 
        }
        else {
           	return osMutexWait(mutexHandle, msecTimeout.msecTimeout)==0; 
        }
        return True;
    }

    /** @brief Unlock the semaphore.
      * @return true if the unlock has success.
      * Enable the possibility to kill the thread after the unlock. */
    bool UnLock() {
	return osMutexRelease(mutexHandle)==0;
    }

    /** @brief Fast return in case of locked semaphore.
      * @return true if the semaphore was unlocked and locks it, otherwise return false.*/
    bool TryLock() {
        return osMutexWait(mutexHandle,0)==0;
    }

};

/** @see MutexSem::Create.
  * @brief Create a mutex semafore with a given initial state 
  * @param semH is a pointer to the new PrivateMutexSemStruct object.
  * @param locked defines the initial state (true = locked, false = unlocked).
  * @return false if the new or Init (or Lock if locked = true) fails, true otherwise */
static bool MutexSemOSCreate(HANDLE &semH, bool locked) {
    if (semH != (HANDLE) NULL) {
        delete (PrivateMutexSemStruct *) semH;
    }
    // Create the Structure
    semH = (HANDLE) new PrivateMutexSemStruct();
    if (semH == (HANDLE) NULL) {
        return False;
    }
    if(!(((PrivateMutexSemStruct*) semH)->Init()) 
	return False;


    if(locked)
	if(!(((PrivateMutexSemStruct*)semH)->Lock())) return False;

        return True;  

}

/** @see MutexSem::Close
  * @brief Close the semafore handle.
  * @param semH is a pointer to the mutex semaphore.
  * @return true. */
static inline bool MutexSemOSClose(HANDLE &semH) {
    if (semH == (HANDLE) NULL) {
        return True;
    }
    semH = (HANDLE) NULL;
    return True;
}

/** @see MutexSem::Lock
  * @brief Lock the semafore.
  * @param semH is a pointer to the mutex semaphore.
  * @param msecTimeout is the desired timeout.
  * @return the result of PrivateMutexSemStruct::Lock */
static inline bool MutexSemOSLock(HANDLE &semH, TimeoutType msecTimeout) {
    if (semH == (HANDLE) NULL) {
        return False;
    }
    return ((PrivateMutexSemStruct *) semH)->Lock(msecTimeout);
}

/** @see MutexSem::UnLock
  * @brief Unlock the semaphore.
  * @param semH is a pointer to the mutex semaphore.
  * @return the return of PrivateMutexSemStruct::UnLock. */
static inline bool MutexSemOSUnLock(HANDLE &semH) {
    if (semH == (HANDLE) NULL) {
        return False;
    }
    return ((PrivateMutexSemStruct *) semH)->UnLock();
}

/** @see MutexSem::Lock MutexSemOSLock. */
static inline bool MutexSemOSFastLock(HANDLE &semH, TimeoutType msecTimeout) {
    if (semH == (HANDLE) NULL) {
        return False;
    }
    return ((PrivateMutexSemStruct *) semH)->Lock(msecTimeout);
}

/** @see MutexSem::UnLock MutexSemOSUnLock. */
static inline bool MutexSemOSFastUnLock(HANDLE &semH) {
    if (semH == (HANDLE) NULL) {
        return False;
    }
    return ((PrivateMutexSemStruct *) semH)->UnLock();
}

/** @see MutexSem::FastTryLock. 
  * @brief Fast return in case of locked semaphore.
  * @param semH is a pointer to the mutex semaphore.
  * @param msecTimeout is the desired timeout.
  * @return the result of PrivateMutexSemStruct::FastLock. */
static inline bool MutexSemOSFastTryLock(HANDLE &semH) {
    if (semH == (HANDLE) NULL) {
        return False;
    }
    return ((PrivateMutexSemStruct *) semH)->TryLock();
}

#endif

