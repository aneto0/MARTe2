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
#include "HeapManager.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

uint32 ThreadsDatabase::nOfEntries = 0u;
uint32 ThreadsDatabase::maxNOfEntries = 0u;
ThreadInformation **ThreadsDatabase::entries = static_cast<ThreadInformation **>(NULL);
FastPollingMutexSem ThreadsDatabase::internalMutex;


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ThreadsDatabase::NewEntry(ThreadInformation * const threadInformation) {
    bool ok = ThreadsDatabase::AllocMore();
    if (ok) {
        ok = (ThreadsDatabase::nOfEntries < ThreadsDatabase::maxNOfEntries);
        // no space
        if (ok) {
            ok = false;
            // search for empty space staring from guess
            uint32 index = ThreadsDatabase::nOfEntries;
            while (index != (ThreadsDatabase::nOfEntries - 1u)) {
                if (ThreadsDatabase::entries[index] == NULL) {
                    ThreadsDatabase::entries[index] = threadInformation;
                    ThreadsDatabase::nOfEntries++;
                    ok = true;
                    break;
                }
                index++;
                // roll-over
                if (index >= ThreadsDatabase::maxNOfEntries) {
                    index -= ThreadsDatabase::maxNOfEntries;
                }
            }
        }
        //CStaticAssertErrorCondition(FatalError,"TDB:TDB_NewEntry could not find empty slot!!");
    }
    return ok;
}

ThreadInformation *ThreadsDatabase::RemoveEntry(const ThreadIdentifier &threadId) {
    ThreadInformation *threadInfo = static_cast<ThreadInformation *>(NULL);
    // search for empty space staring from guess
    uint32 index = 0u;
    while (index < ThreadsDatabase::maxNOfEntries) {
        ThreadInformation *threadInfoIdx = ThreadsDatabase::entries[index];
        if (threadInfoIdx != NULL) {
            if (threadInfoIdx->GetThreadIdentifier() == threadId) {
                ThreadsDatabase::entries[index] = static_cast<ThreadInformation *>(NULL);
                ThreadsDatabase::nOfEntries--;

                // free at the end
                if (ThreadsDatabase::nOfEntries == 0u) {
                    HeapManager::Free(reinterpret_cast<void *&>(ThreadsDatabase::entries));
                    //For AllocMore to reallocate again!
                    ThreadsDatabase::maxNOfEntries = 0u;
                }
                threadInfo = threadInfoIdx;
                break;
            }
        }
        index++;
    }

    //CStaticAssertErrorCondition(FatalError,"TDB:TDB_RemoveEntry could not find/remove entry ThreadIdentifier=%08x ",threadId);
    return threadInfo;

}

ThreadInformation *ThreadsDatabase::GetThreadInformation(const ThreadIdentifier &threadId) {
    ThreadInformation *threadInfo = static_cast<ThreadInformation *>(NULL);
    // search for empty space staring from guess
    uint32 index = 0u;
    while (index < ThreadsDatabase::maxNOfEntries) {
        ThreadInformation *threadInfoIdx = ThreadsDatabase::entries[index];
        if (threadInfoIdx != NULL) {
            if (threadInfoIdx->GetThreadIdentifier() == threadId) {
                threadInfo = threadInfoIdx;
                break;
            }
        }
        index++;
    }

    //CStaticAssertErrorCondition(FatalError,"TDB:TDB_GetTII could not find entry ThreadIdentifier=%08x ",threadId);
    return threadInfo;
}

bool ThreadsDatabase::Lock() {
    ErrorType err = ThreadsDatabase::internalMutex.FastLock();
    return (err == NoError);
}

void ThreadsDatabase::UnLock() {
    ThreadsDatabase::internalMutex.FastUnLock();
}

uint32 ThreadsDatabase::NumberOfThreads() {
    return ThreadsDatabase::nOfEntries;
}

ThreadIdentifier ThreadsDatabase::GetThreadID(const uint32 &n) {
    ThreadIdentifier tid = 0u;
    if (n < ThreadsDatabase::maxNOfEntries) {
        if (ThreadsDatabase::entries[n] != NULL) {
            tid = ThreadsDatabase::entries[n]->GetThreadIdentifier();
        }
    }

    //CStaticAssertErrorCondition(FatalError,"TDB:TDB_GetThreadID(%i) mismatch between actual entries and TDB_NOfEntries");
    return tid;
}

bool ThreadsDatabase::GetInfoIndex(ThreadInformation &threadInfoCopy,
                                   const uint32 &n) {
    ThreadIdentifier threadId = GetThreadID(n);
    ThreadInformation *threadInfo = GetThreadInformation(threadId);
    if (threadInfo != NULL) {
        threadInfoCopy.Copy(*threadInfo);
    }
    return (threadInfo != NULL);
}

bool ThreadsDatabase::GetInfo(ThreadInformation &threadInfoCopy,
                              const ThreadIdentifier &threadId) {
    ThreadInformation *threadInfo = GetThreadInformation(threadId);
    if (threadInfo != NULL) {
        threadInfoCopy.Copy(*threadInfo);
    }
    return (threadInfo != NULL);
}

ThreadIdentifier ThreadsDatabase::Find(const char8 * const name) {
    ThreadIdentifier tid = 0u;
    // search for empty space staring from guess
    uint32 index = 0u;
    while (index < ThreadsDatabase::maxNOfEntries) {
        if (ThreadsDatabase::entries[index] != NULL) {
            if (StringHelper::Compare(ThreadsDatabase::entries[index]->ThreadName(), name) == 0) {
                tid = ThreadsDatabase::entries[index]->GetThreadIdentifier();
                break;
            }
        }
        index++;
    }

    return tid;
}

bool ThreadsDatabase::AllocMore() {
    bool ok = true;
    // no need
    if (maxNOfEntries <= nOfEntries) {
        // first time?
        if (entries == NULL) {
            uint32 size = static_cast<uint32>(sizeof(ThreadInformation *)) * THREADS_DATABASE_GRANULARITY;
            entries = static_cast<ThreadInformation **>(HeapManager::Malloc(size));
            if (entries != NULL) {
                maxNOfEntries = THREADS_DATABASE_GRANULARITY;
                nOfEntries = 0u;
            }
            else {
                //CStaticAssertErrorCondition(FatalError,"TDB:TDB_AllocMore failed allocating %i entries",TDB_THREADS_DATABASE_GRANULARITY);
                ok = false;
            }
        }
        else {
            uint32 newSize = static_cast<uint32>(sizeof(ThreadInformation *)) * (THREADS_DATABASE_GRANULARITY + maxNOfEntries);
            entries = static_cast<ThreadInformation **>(HeapManager::Realloc(reinterpret_cast<void *&>(entries), newSize));
            if (entries != NULL) {
                maxNOfEntries += THREADS_DATABASE_GRANULARITY;
            }
            else {
                //CStaticAssertErrorCondition(FatalError,"TDB:TDB_AllocMore failed re-allocating to %i entries",TDB_THREADS_DATABASE_GRANULARITY+TDB_MaxNOfEntries);
                ok = false;
            }
        }

        // clean new memory
        uint32 i;
        if (ok) {
            for (i = (maxNOfEntries - THREADS_DATABASE_GRANULARITY); i < maxNOfEntries; i++) {
                entries[i] = static_cast<ThreadInformation *>(NULL);
            }
        }
    }
    return ok;
}
