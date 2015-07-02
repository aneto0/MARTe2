/**
 * @file ThreadsDatabase.cpp
 * @brief Source file for class ThreadsDatabase
 * @date 17/06/2015
 * @author Giuseppe Ferr�
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
 * the class ThreadsDatabase (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include "ThreadsDatabase.h"
#include "ThreadInformation.h"
#include "Memory.h"
#include "Threads.h"
#include "StringHelper.h"



/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

uint32 ThreadsDatabase::nOfEntries = 0;
uint32 ThreadsDatabase::maxNOfEntries = 0;
ThreadInformation **ThreadsDatabase::entries = NULL;
FastPollingMutexSem ThreadsDatabase::internalMutex;


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
    uint32 index = ThreadsDatabase::nOfEntries;
    if (index >= ThreadsDatabase::maxNOfEntries)
        index -= ThreadsDatabase::maxNOfEntries;
    while (index != ThreadsDatabase::nOfEntries - 1) {
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

ThreadInformation *ThreadsDatabaseRemoveEntry(TID threadId) {
    // search for empty space staring from guess
    uint32 index = 0;
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

ThreadInformation *ThreadsDatabaseGetThreadInformation(TID threadId) {
    // search for empty space staring from guess
    uint32 index = 0;
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

bool ThreadsDatabaseLock(TimeoutType tt) {
    return ThreadsDatabase::internalMutex.FastLock(tt);
}

void ThreadsDatabaseUnLock() {
    ThreadsDatabase::internalMutex.FastUnLock();
}

uint32 ThreadsDatabaseNumberOfThreads() {
    return ThreadsDatabase::nOfEntries;
}

TID ThreadsDatabaseGetThreadID(uint32 n) {

    if (n >= ThreadsDatabase::nOfEntries) {
        //CStaticAssertErrorCondition(FatalError,"TDB:TDB_GetThreadID(%i) index out of range",n);
        return (TID) 0;
    }

    // search for empty space staring from guess
    uint32 index = 0;
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
    return (TID) 0;
}

bool ThreadsDatabaseGetInfo(ThreadInformation &threadInfoCopy,
                            int32 n,
                            TID threadId) {
    if (n >= 0) {
        TID threadId = ThreadsDatabaseGetThreadID(n);
        ThreadInformation *threadInfo = ThreadsDatabaseGetThreadInformation(threadId);
        if (threadInfo == NULL) {
            return False;
        }
        threadInfoCopy = *threadInfo;
        return True;
    }
    else {
        ThreadInformation *threadInfo = ThreadsDatabaseGetThreadInformation(threadId);
        if (threadInfo == NULL) {
            return False;
        }
        threadInfoCopy = *threadInfo;
        return True;
    }

}

TID ThreadsDatabaseFind(const char *name) {

    if (name == NULL) {
        return (TID) 0;
    }

    // search for empty space staring from guess
    uint32 index = 0;
    while (index < ThreadsDatabase::maxNOfEntries) {
        if (ThreadsDatabase::entries[index] != NULL) {
            if (StringHelper::Compare(ThreadsDatabase::entries[index]->ThreadName(), name) == 0) {
                return ThreadsDatabase::entries[index]->threadId;
            }
        }
        index++;
    }

    return (TID) 0;

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ThreadsDatabase::AllocMore() {
    // no need
    if (maxNOfEntries > nOfEntries) {
        return True;
    }

    // first time?
    if (entries == NULL) {
        entries = (ThreadInformation **) MemoryMalloc(sizeof(ThreadInformation *) * THREADS_DATABASE_GRANULARITY);
        if (entries != NULL) {
            maxNOfEntries = THREADS_DATABASE_GRANULARITY;
            nOfEntries = 0;
        }
        else {
            //CStaticAssertErrorCondition(FatalError,"TDB:TDB_AllocMore failed allocating %i entries",TDB_THREADS_DATABASE_GRANULARITY);
            return False;
        }
    }
    else {
        entries = (ThreadInformation **) MemoryRealloc((void *&) entries, sizeof(ThreadInformation *) * (THREADS_DATABASE_GRANULARITY + maxNOfEntries));
        if (entries != NULL) {
            maxNOfEntries += THREADS_DATABASE_GRANULARITY;
        }
        else {
            //CStaticAssertErrorCondition(FatalError,"TDB:TDB_AllocMore failed re-allocating to %i entries",TDB_THREADS_DATABASE_GRANULARITY+TDB_MaxNOfEntries);
            return False;
        }
    }

    // clean new memory
    uint32 i;
    for (i = (maxNOfEntries - THREADS_DATABASE_GRANULARITY); i < maxNOfEntries; i++) {
        entries[i] = NULL;
    }
    return True;
}

bool ThreadsDatabase::NewEntry(ThreadInformation *ti) {
    return ThreadsDatabaseNewEntry(ti);
}

ThreadInformation *ThreadsDatabase::RemoveEntry(TID tid) {
    return ThreadsDatabaseRemoveEntry(tid);
}

ThreadInformation *ThreadsDatabase::GetThreadInformation(TID tid) {
    return ThreadsDatabaseGetThreadInformation(tid);
}

bool ThreadsDatabase::Lock(TimeoutType tt) {
    return ThreadsDatabaseLock(tt);
}

void ThreadsDatabase::UnLock() {
    ThreadsDatabaseUnLock();
}

uint32 ThreadsDatabase::NumberOfThreads() {
    return ThreadsDatabaseNumberOfThreads();
}

TID ThreadsDatabase::GetThreadID(uint32 n) {
    return ThreadsDatabaseGetThreadID(n);
}

bool ThreadsDatabase::GetInfo(ThreadInformation &tiCopy,
                              int32 n,
                              TID tid) {
    return ThreadsDatabaseGetInfo(tiCopy, n, tid);
}

TID ThreadsDatabase::Find(const char *name) {
    return ThreadsDatabaseFind(name);
}

