/**
 * @file MemoryStatisticsDatabase.h
 * @brief Header file for class MemoryStatisticsDatabase
 * @date 02/07/2015
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

 * @details This header file contains the declaration of the class MemoryStatisticsDatabase
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYSTATISTICSDATABASE_H_
#define MEMORYSTATISTICSDATABASE_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Threads.h"
#include "FastPollingMutexSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief The basic element of the memory database.
 */
class ThreadAllocationStatistics {

public:
    /** The thread identifier */
    TID threadId;

    /** The memory amount allocated by the thread */
    int32 totalMemorySize;

    /** How many allocations the thread did */
    int32 nOfMemoryChunks;

    /**
     * @brief Checks if this element is zero.
     * @return true if all attributes are equal to zero.
     */
    inline bool IsEmpty() const;

    /**
     * @brief Sets this element to zero.
     * @details Sets all the attributes to zero.
     */
    inline void SetEmpty();

};

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A database on the stack which contains the memory informations for a fixed number of threads.
 * @details This database is used if the symbol MEMORY_STATISTICS is defined.\n
 * Since the database allocates space on the stack, the maximum size MAX_NO_OF_MEMORY_MONITORS is defined in GeneralDefinitionsOS.h
 */
class MemoryStatisticsDatabase {
public:

    /**
     * @brief Finds an element in the database by the thread identifier.
     * @param[in] tid is the thread identifier.
     * @return a pointer to the element in the database.
     */
    static inline ThreadAllocationStatistics* Find(TID tid);

    /**
     * @brief Signs a new memory chunk of memory for the specified element.
     * @details if the element is not in the database it is added.
     * @param[in] tid is the thread identifier used to find the element in the database.
     * @param[in] memSize is the size of the memory chunk
     * @return true if the element is not in the database and this is already full.
     */
    static inline bool AddMemoryChunk(TID tid,
                                      uint32 memSize);

    /**
     * @brief Deletes a memory chunk of memory from the specified element informations.
     * @details If the remained memory is zero, the element is removed from the database.
     * @param[in] tid is the thread identifier used to find the element.
     * @param[in] memSize is the size of the memory chunk freed.
     * @return true if the element is already in the database, false otherwise.
     */
    static inline bool FreeMemoryChunk(TID tid,
                                       uint32 memSize);

    /**
     * @brief Empties the database.
     * @details Sets all the elements to zero.
     */
    static inline void Clear();

    /**
     * @brief Returns the sum of the current allocated heap memory.
     * @return the amount of the current allocated heap memory.
     */
    static inline int32 GetTotalUsedHeap();

    /**
     * @brief Returns the number of elements currently in the database.
     * @return the number of elements in the database.
     */
    static inline uint32 GetNOfElements();
private:

    /** The memory database array. */
    static ThreadAllocationStatistics stackDatabase[MAX_NO_OF_MEMORY_MONITORS];

    /** The current number of elements in the database. */
    static uint32 nOfElements;

    /** Internal mutex */
    static FastPollingMutexSem internalMutex;

    /**
     * @brief Adds a new element in the database.
     * @param[in] tid is the identifier of the thread which has to be monitored.
     * @return false if the database is already full, true otherwise.
     */
    static inline bool Add(TID tid);

    /**
     * @brief Removes an element from the database.
     * @param[in] tid is the thread identifier of the element which must be removed.
     * @return true if the requested element is in the database, false otherwise.
     */
    static inline bool Remove(TID tid);

    /**
     * @brief Finds an element in the database by the thread identifier.
     * @param[in] tid is the thread identifier.
     * @return a pointer to the element in the database.
     */
    static inline ThreadAllocationStatistics* PrivateFind(TID tid);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
bool ThreadAllocationStatistics::IsEmpty() const {
    return threadId == 0 && totalMemorySize == 0 && nOfMemoryChunks == 0;
}

void ThreadAllocationStatistics::SetEmpty() {
    threadId = 0;
    totalMemorySize = 0;
    nOfMemoryChunks = 0;
}

bool MemoryStatisticsDatabase::Add(TID tid) {
    for (uint32 i = 0; i < MAX_NO_OF_MEMORY_MONITORS; i++) {

        //found an empty element
        if (stackDatabase[i].IsEmpty()) {
            stackDatabase[i].threadId = tid;
            nOfElements++;
            return true;
        }
    }

    //the database is full
    return false;
}

bool MemoryStatisticsDatabase::Remove(TID tid) {

    ThreadAllocationStatistics* ret = PrivateFind(tid);

    //not found
    if (ret == NULL) {
        return false;
    }

    //set the element as empty
    ret->SetEmpty();
    nOfElements--;
    return true;
}

ThreadAllocationStatistics* MemoryStatisticsDatabase::PrivateFind(TID tid) {

    for (uint32 i = 0, counter = 0; counter < nOfElements && i < MAX_NO_OF_MEMORY_MONITORS; i++) {
        //skip in case of empty element
        if (stackDatabase[i].IsEmpty()) {
            continue;
        }

        //if found return
        if (tid == stackDatabase[i].threadId) {
            return stackDatabase + i;
        }
        counter++;
    }

    //not found
    return NULL;
}

ThreadAllocationStatistics* MemoryStatisticsDatabase::Find(TID tid) {
    internalMutex.FastLock();
    ThreadAllocationStatistics *ret = PrivateFind(tid);
    internalMutex.FastUnLock();
    return ret;
}

bool MemoryStatisticsDatabase::AddMemoryChunk(TID tid,
                                              uint32 memSize) {
    internalMutex.FastLock();

    //if it is not in the db try to add it
    ThreadAllocationStatistics *ret = PrivateFind(tid);

    if (ret == NULL) {
        if (!Add(tid)) {
            internalMutex.FastUnLock();
            return false;
        }

        ret = PrivateFind(tid);
    }

    //increment the number of chunks and add the memory size
    ret->nOfMemoryChunks++;
    ret->totalMemorySize += memSize;

    //remove the element if the thread has not allocated memory
    if (ret->totalMemorySize == 0) {
        Remove(tid);
    }
    internalMutex.FastUnLock();
    return true;
}

bool MemoryStatisticsDatabase::FreeMemoryChunk(TID tid,
                                               uint32 memSize) {
    internalMutex.FastLock();

    ThreadAllocationStatistics *ret = PrivateFind(tid);
    if (ret == NULL) {
        if (!Add(tid)) {
            internalMutex.FastUnLock();
            return false;
        }

        ret = PrivateFind(tid);
    }
    //decrement the number of chuncks and remove the memory size
    ret->nOfMemoryChunks--;
    ret->totalMemorySize -= memSize;

    //remove the element if the thread has not allocated memory
    if (ret->totalMemorySize == 0) {
        Remove(tid);
    }
    internalMutex.FastUnLock();

    return true;
}

void MemoryStatisticsDatabase::Clear() {
    internalMutex.FastLock();
    for (uint32 i = 0, p = 0; i < MAX_NO_OF_MEMORY_MONITORS && p < nOfElements; i++) {
        if (stackDatabase[i].IsEmpty()) {
            continue;
        }
        stackDatabase[i].SetEmpty();
        p++;
    }
    nOfElements = 0;
    internalMutex.FastUnLock();
}

int32 MemoryStatisticsDatabase::GetTotalUsedHeap() {
    internalMutex.FastLock();
    int32 sum = 0;
    for (uint32 i = 0, p = 0; i < MAX_NO_OF_MEMORY_MONITORS && p < nOfElements; i++) {
        if (stackDatabase[i].IsEmpty()) {
            continue;
        }
        sum += stackDatabase[i].totalMemorySize;
        p++;
    }
    internalMutex.FastUnLock();
    return sum;
}

uint32 MemoryStatisticsDatabase::GetNOfElements() {
    internalMutex.FastLock();
    uint32 ret = nOfElements;
    internalMutex.FastUnLock();
    return ret;
}

#endif /* MEMORYSTATISTICSDATABASE_H_ */

