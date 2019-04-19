/**
 * @file ThreadsDatabase_Generic.cpp
 * @brief Source file for module ThreadsDatabase
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
 * the module ThreadsDatabase (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ThreadsDatabase.h"
#include "GeneralDefinitions.h"
#include "HeapManager.h"
#include "StringHelper.h"
#include "ErrorManagement.h"
#include "FastPollingMutexSem.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

namespace ThreadsDatabase {

/**
 * Fast semaphore for exclusive access to the database.
 */
static FastPollingMutexSem internalMutex;

/**
 * Actual number of entries stored in the database.
 */
static uint32 nOfEntries = 0u;

/**
 * Maximum number of entries that can be stored in the database.
 */
static uint32 maxNOfEntries = 0u;

/**
 * Vector of ThreadInformation pointers.
 */
static ThreadInformation **entries = static_cast<ThreadInformation **>(NULL);

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool NewEntry(ThreadInformation * const threadInformation) {
    bool ok = AllocMore();
    if (ok) {
        ok = (nOfEntries < maxNOfEntries);
        // no space
        if (ok) {
            ok = false;
            // search for empty space staring from guess
            uint32 index = nOfEntries;
            while (index != (nOfEntries - 1u)) {
                if (entries[index] == NULL) {
                    entries[index] = threadInformation;
                    nOfEntries++;
                    ok = true;
                    break;
                }
                index++;
                // roll-over
                if (index >= maxNOfEntries) {
                    index -= maxNOfEntries;
                }
            }
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Error: cannot find an empty slot");
        }
    }
    return ok;
}

ThreadInformation *RemoveEntry(const ThreadIdentifier &threadId) {
    ThreadInformation *threadInfo = static_cast<ThreadInformation *>(NULL);
    // search for empty space staring from guess
    uint32 index = 0u;
    while (index < maxNOfEntries) {
        ThreadInformation *threadInfoIdx = entries[index];
        if (threadInfoIdx != NULL) {
            if (threadInfoIdx->GetThreadIdentifier() == threadId) {
                entries[index] = static_cast<ThreadInformation *>(NULL);
                nOfEntries--;

                // free at the end
                if (nOfEntries == 0u) {
                    /*lint -e{9025} We are passing a double-pointer to a reference */
                    /*lint -e{929} Type handled inside HeapManager::Free through a HeapI interface */
                    bool ok = HeapManager::Free(reinterpret_cast<void *&>(entries));
                    if (!ok) {
                        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Error: database memory cleanup failed");
                    }
                    //For AllocMore to reallocate again!
                    maxNOfEntries = 0u;
                }
                threadInfo = threadInfoIdx;
                break;
            }
        }
        index++;
    }

    return threadInfo;

}

ThreadInformation *GetThreadInformation(const ThreadIdentifier &threadId) {
    ThreadInformation *threadInfo = static_cast<ThreadInformation *>(NULL);
    // search for empty space staring from guess
    uint32 index = 0u;
    while (index < maxNOfEntries) {
        ThreadInformation *threadInfoIdx = entries[index];
        if (threadInfoIdx != NULL) {
            if (threadInfoIdx->GetThreadIdentifier() == threadId) {
                threadInfo = threadInfoIdx;
                break;
            }
        }
        index++;
    }
    return threadInfo;
}

bool Lock() {
    ErrorManagement::ErrorType err = internalMutex.FastLock();
    return (err == ErrorManagement::NoError);
}

void UnLock() {
    internalMutex.FastUnLock();
}

uint32 NumberOfThreads() {
    return nOfEntries;
}

ThreadIdentifier GetThreadID(const uint32 &n) {
    ThreadIdentifier tid = 0u;
    uint32 notEmptyN = (n + 1u);
    uint32 index = 0u;
    while ((notEmptyN != 0u) && (index < maxNOfEntries)) {
        if (entries[index] != NULL) {
            notEmptyN--;
        }
        index++;
    }
    index--;
    if (notEmptyN != 0u) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Error: the index in input is greater than the number of database records");
    }
    else {
        if (entries[index] != NULL) {
            tid = entries[index]->GetThreadIdentifier();
        }
    }

    return tid;
}

bool GetInfoIndex(ThreadInformation &threadInfoCopy, const uint32 &n) {
    ThreadIdentifier threadId = GetThreadID(n);
    ThreadInformation *threadInfo = GetThreadInformation(threadId);
    if (threadInfo != NULL) {
        threadInfoCopy.Copy(*threadInfo);
    }
    return (threadInfo != NULL);
}

bool GetInfo(ThreadInformation &threadInfoCopy, const ThreadIdentifier &threadId) {
    ThreadInformation *threadInfo = GetThreadInformation(threadId);
    if (threadInfo != NULL) {
        threadInfoCopy.Copy(*threadInfo);
    }
    return (threadInfo != NULL);
}

ThreadIdentifier Find(const char8 * const name) {
    ThreadIdentifier tid = 0u;
// search for empty space starting from guess
    uint32 index = 0u;
    while (index < maxNOfEntries) {
        if (entries[index] != NULL) {
            if (StringHelper::Compare(entries[index]->ThreadName(), name) == 0) {
                tid = entries[index]->GetThreadIdentifier();
                break;
            }
        }
        index++;
    }

    return tid;
}

bool AllocMore() {
    bool ok = true;
// no need
    if (maxNOfEntries <= nOfEntries) {
        // first time?
        if (entries == NULL) {
            uint32 size = static_cast<uint32>(sizeof(ThreadInformation *)) * THREADS_DATABASE_GRANULARITY;
            /*lint -e{925} HeapManager::Malloc returns ThreadInformation ** */
            entries = static_cast<ThreadInformation **>(HeapManager::Malloc(size));
            if (entries != NULL) {
                maxNOfEntries = THREADS_DATABASE_GRANULARITY;
                nOfEntries = 0u;
            }
            else {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Error: memory allocation for the database failed");
                ok = false;
            }
        }
        else {
            uint32 newSize = static_cast<uint32>(sizeof(ThreadInformation *)) * (THREADS_DATABASE_GRANULARITY + maxNOfEntries);
            /*lint -e{9025} We are passing a double-pointer to a reference */
            /*lint -e{929} Type handled inside HeapManager::Realloc through a HeapI interface */
            /*lint -e{925} HeapManager::Realloc returns ThreadInformation ** */
            entries = static_cast<ThreadInformation **>(HeapManager::Realloc(reinterpret_cast<void *&>(entries), newSize));
            if (entries != NULL) {
                maxNOfEntries += THREADS_DATABASE_GRANULARITY;
            }
            else {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Error: memory reallocation for the database failed");
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

}
}
