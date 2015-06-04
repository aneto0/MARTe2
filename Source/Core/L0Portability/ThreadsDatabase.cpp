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
 * $Id: $
 *
 **/
#include "GeneralDefinitions.h"
#include "ThreadsDatabase.h"
#include "ThreadInformation.h"
#include "Atomic.h"
#include "Memory.h"
#include "Threads.h"
#include "Sleep.h"

int32 ThreadsDatabase::atomicSem = 0;
int32 ThreadsDatabase::nOfEntries = 0;
int32 ThreadsDatabase::maxNOfEntries = 0;
ThreadInformation **ThreadsDatabase::entries = NULL;

bool ThreadsDatabase::AllocMore() {
    // no need
    if (maxNOfEntries > nOfEntries) {
        return True;
    }

    // first time?
    if (entries == NULL) {
        entries = (ThreadInformation **) MemoryMalloc(
                sizeof(ThreadInformation *) * GRANULARITY);
        if (entries != NULL) {
            maxNOfEntries = GRANULARITY;
            nOfEntries = 0;
        }
        else {
            //CStaticAssertErrorCondition(FatalError,"TDB:TDB_AllocMore failed allocating %i entries",TDB_Granularity);
            return False;
        }
    }
    else {
        entries = (ThreadInformation **) MemoryRealloc(
                (void *&) entries,
                sizeof(ThreadInformation *) * (GRANULARITY + maxNOfEntries));
        if (entries != NULL) {
            maxNOfEntries += GRANULARITY;
        }
        else {
            //CStaticAssertErrorCondition(FatalError,"TDB:TDB_AllocMore failed re-allocating to %i entries",TDB_Granularity+TDB_MaxNOfEntries);
            return False;
        }
    }

    // clean new memory
    int32 i;
    for (i = (maxNOfEntries - GRANULARITY); i < maxNOfEntries; i++) {
        entries[i] = NULL;
    }
    return True;
}

/** create new TDB entry associated to the threadInfo 
 threadId = 0 --> current TID */
bool ThreadsDatabaseNewEntry(ThreadInformation *threadInfo) {
    if (threadInfo == NULL) {
        return False;
    }

    if (!ThreadsDatabase::AllocMore()) {
        //CStaticAssertErrorCondition(FatalError,"TDB:TDB_NewEntry failed (re-)allocating memory");
        return False;
    }

    // no space
    if (ThreadsDatabase::maxNOfEntries <= ThreadsDatabase::nOfEntries) {
        //CStaticAssertErrorCondition(FatalError,"TDB:TDB_NewEntry no space for new entry");
        return False;
    }

    // search for empty space staring from guess
    int32 index = ThreadsDatabase::nOfEntries + 1;
    if (index >= ThreadsDatabase::maxNOfEntries)
        index -= ThreadsDatabase::maxNOfEntries;
    while (index != ThreadsDatabase::nOfEntries) {
        if (ThreadsDatabase::entries[index] == NULL) {
            ThreadsDatabase::entries[index] = threadInfo;
            ThreadsDatabase::nOfEntries++;
            return True;
        }
        index++;
        // roll-over
        if (index >= ThreadsDatabase::maxNOfEntries) {
            index -= ThreadsDatabase::maxNOfEntries;
        }
    }

    //CStaticAssertErrorCondition(FatalError,"TDB:TDB_NewEntry could not find empty slot!!");
    return False;

}

/** destroy TDB entry  */
ThreadInformation *ThreadsDatabaseRemoveEntry(TID threadId) {
    // search for empty space staring from guess
    int index = 0;
    while (index < ThreadsDatabase::maxNOfEntries) {
        ThreadInformation *threadInfo = ThreadsDatabase::entries[index];
        if (threadInfo != NULL) {
            if (threadInfo->threadId == threadId) {
                ThreadsDatabase::entries[index] = NULL;
                ThreadsDatabase::nOfEntries--;

                // free at the end
                if (ThreadsDatabase::nOfEntries == 0) {
                    MemoryFree((void *&) ThreadsDatabase::entries);
                    //For AllocMore to reallocate again!
                    ThreadsDatabase::maxNOfEntries = 0;
                }
                return threadInfo;
            }
        }
        index++;
    }

    //CStaticAssertErrorCondition(FatalError,"TDB:TDB_RemoveEntry could not find/remove entry TID=%08x ",threadId);
    return NULL;

}

/** access private thread information
 on timeout returns NULL
 threadId = 0 --> current TID */
ThreadInformation *ThreadsDatabaseGetThreadInformation(TID threadId) {
    // search for empty space staring from guess
    int32 index = 0;
    while (index < ThreadsDatabase::maxNOfEntries) {
        ThreadInformation *threadInfo = ThreadsDatabase::entries[index];
        if (threadInfo != NULL) {
            if (threadInfo->threadId == threadId) {
                return threadInfo;
            }
        }
        index++;
    }

    //CStaticAssertErrorCondition(FatalError,"TDB:TDB_GetTII could not find entry TID=%08x ",threadId);
    return NULL;
}

// global functions
/** must be locked before accessing TDB information */
bool ThreadsDatabaseLock(TimeoutType tt) {
    int64 ticksStop = tt.HighResolutionTimerTicks();
    ticksStop += HighResolutionTimer::Counter();
    while (!Atomic::TestAndSet(&ThreadsDatabase::atomicSem)) {
        if (tt != TTInfiniteWait) {
            int64 ticks = HighResolutionTimer::Counter();
            if (ticks > ticksStop) {
                return False;
            }
        }
        // yield CPU
        SleepMSec(1);
    }
    return True;
}

/** must be unlocked after accessing TDB information */
bool ThreadsDatabaseUnLock() {
    ThreadsDatabase::atomicSem = 0;
    return True;
}

/** how many threads are registered
 value meaningful only between Lock/UnLock*/
int32 ThreadsDatabaseNumberOfThreads() {
    return ThreadsDatabase::nOfEntries;
}

/** the TID of thread #n
 value meaningful only between Lock/UnLock*/
TID ThreadsDatabaseGetThreadID(int32 n) {

    if ((n < 0) || (n >= ThreadsDatabase::nOfEntries)) {
        //CStaticAssertErrorCondition(FatalError,"TDB:TDB_GetThreadID(%i) index out of range",n);
        return 0;
    }

    // search for empty space staring from guess
    int32 index = 0;
    while (index < ThreadsDatabase::maxNOfEntries) {
        if (ThreadsDatabase::entries[index] != NULL) {
            if (n == 0) {
                return ThreadsDatabase::entries[index]->threadId;
            }
            n--;
        }
        index++;
    }

    //CStaticAssertErrorCondition(FatalError,"TDB:TDB_GetThreadID(%i) mismatch between actual entries and TDB_NOfEntries");
    return 0;
}

/** retrieves information about a thread identified either by name or TID or index
 to be called between Lock/UnLock*/
bool ThreadsDatabaseGetInfo(ThreadInformation &threadInfoCopy, int32 n,
                            TID threadId) {
    if (n >= 0) {
        TID threadId = ThreadsDatabaseGetThreadID(n);
        ThreadInformation *threadInfo = ThreadsDatabaseGetThreadInformation(
                threadId);
        if (threadInfo == NULL) {
            return False;
        }
        threadInfoCopy = *threadInfo;
        return True;
    }
    else if (threadId != (TID) - 1) {
        ThreadInformation *threadInfo = ThreadsDatabaseGetThreadInformation(
                threadId);
        if (threadInfo == NULL) {
            return False;
        }
        threadInfoCopy = *threadInfo;
        return True;
    }
    else {
        //CStaticAssertErrorCondition(FatalError,"TDB:TDB_GetThreadID(%i) mismatch between actual entries and TDB_NOfEntries ");
    }
    return False;
}

