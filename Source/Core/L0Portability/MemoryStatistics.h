/**
 * @file MemoryStatistics.h
 * @brief Header file for class MemoryStatistics
 * @date 18/06/2015
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

 * @details This header file contains the declaration of the class MemoryStatistics
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYSTATISTICS_H_
#define 		MEMORYSTATISTICS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Threads.h"

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
    uint32 totalMemorySize;

    /** How many allocations the thread did */
    uint32 nOfMemoryChunks;

    /**
     * @brief Checks if this element is zero.
     * @return true if all attributes are equal to zero.
     */
    inline bool isEmpty() const {
        return threadId == 0 && totalMemorySize == 0 && nOfMemoryChunks == 0;
    }

    /**
     * @brief Sets this element to zero.
     * @details Sets all the attributes to zero.
     */
    inline void setEmpty() {
        threadId = 0;
        totalMemorySize = 0;
        nOfMemoryChunks = 0;
    }

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
private:

    /** The memory database array. */
    static ThreadAllocationStatistics stackDatabase[MAX_NO_OF_MEMORY_MONITORS];

    /** The current number of elements in the database. */
    static uint32 nOfElements;

    /**
     * @brief Adds a new element in the database.
     * @param[in] tid is the identifier of the thread which has to be monitored.
     * @return False if the database is already full, true otherwise.
     */
    static inline bool Add(TID tid);

    /**
     * @brief Removes an element from the database.
     * @param[in] tid is the thread identifier of the element which must be removed.
     * @return true if the requested element is in the database, false otherwise.
     */
    static inline bool Remove(TID tid);

public:

    /**
     * @brief Finds an element in the database by the thread identifier.
     * @param[in] tid is the thread identifier.
     * @return a pointer to the element in the database.
     */
    static inline ThreadAllocationStatistics* Find(TID tid) const;

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

};

ThreadAllocationStatistics MemoryStatisticsDatabase::stackDatabase[MAX_NO_OF_MEMORY_MONITORS] = { { 0, 0, 0 } };
uint32 MemoryStatisticsDatabase::nOfElements = 0;

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
bool ThreadAllocationStatistics::isEmpty() const {
    return threadId == 0 && totalMemorySize == 0 && nOfMemoryChunks == 0;
}

void ThreadAllocationStatistics::setEmpty() {
    threadId = 0;
    totalMemorySize = 0;
    nOfMemoryChunks = 0;
}

bool MemoryStatisticsDatabase::Add(TID tid) {
    for (uint32 i = 0; i < MAX_NO_OF_MEMORY_MONITORS; i++) {
        //found an empty element
        if (stackDatabase[i].isEmpty()) {
            stackDatabase[i].threadId = tid;
            nOfElements++;
            return True;
        }
    }

    //the database is full
    return False;
}

bool MemoryStatisticsDatabase::Remove(TID tid) {
    ThreadAllocationStatistics* ret = Find(tid);

    //not found
    if (ret == NULL)
        return False;

    //set the element as empty
    ret->setEmpty();
    nOfElements--;
    return True;
}

ThreadAllocationStatistics* MemoryStatisticsDatabase::Find(TID tid) const {

    for (uint32 i = 0, counter = 0; counter < nOfElements && i < MAX_NO_OF_MEMORY_MONITORS; i++) {
        //skip in case of empty element
        if (stackDatabase[i].isEmpty())
            continue;

        //if found return
        if (tid == stackDatabase[i].threadId) {
            return stackDatabase + i;
        }
        counter++;
    }

    //not found
    return NULL;
}

bool MemoryStatisticsDatabase::AddMemoryChunk(TID tid,
                                              uint32 memSize) {

    //if it is not in the db try to add it
    ThreadAllocationStatistics *ret = Find(tid);
    if (ret == NULL) {
        if (!Add(tid)) {
            return False;
        }
    }

    ret = Find(tid);

    //This should never happen
    if (ret == NULL) {
        return False;
    }

    //increment the number of chunks and add the memory size
    ret->nOfMemoryChunks++;
    ret->totalMemorySize += memSize;
    return True;
}

bool MemoryStatisticsDatabase::FreeMemoryChunk(TID tid,
                                               uint32 memSize) {
    ThreadAllocationStatistics *ret = Find(tid);
    if (ret == NULL) {
        return False;
    }
    //decrement the number of chuncks and remove the memory size
    ret->nOfMemoryChunks--;
    ret->totalMemorySize -= memSize;

    //remove the element if the thread has not allocated memory
    if (ret->nOfMemoryChunks <= 0) {
        Remove(tid);
    }
    return True;
}

#endif /* MEMORYSTATISTICS_H_ */

