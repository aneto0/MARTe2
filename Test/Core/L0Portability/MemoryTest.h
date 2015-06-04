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
 * $Id:$
 *
 **/
/**
 * @file MemoryTest.h
 * @brief Tests the Memory class and associated functions.
 *
 * The test consists in allocate and reallocate parts of memory observing if it's possible de-reference it later.
 * For the shared memory, tests it's behavior sharing it between different threads.
 */

#ifndef MEMORY_TEST_H
#define MEMORY_TEST_H

#include "Memory.h"
#include "Sleep.h"
#include "Threads.h"
#include "EventSem.h"

/** @brief Class for testing of Memory functions. */
class MemoryTest {

private:

public:
    /** a semaphore used for the shared memory test. */
    EventSem eventSem;

    /** @brief Constructor. */
    MemoryTest() {
        eventSem.Create();
    }

    /**
     * @brief Tests the correct behavior of the malloc function.
     * @param size is the number of integers to allocate.
     * @return true if the pointers to the allocated memory are valid. **/
    bool TestMallocAndFree(int32 size);

    /** 
     * @brief Tests the correct behavior of the realloc function.
     * @param size1 is the number of integers to allocate with initial malloc.
     * @param size2 is the additional memory which must be allocated.
     * @return true if the pointers are valid and the realloc does not corrupt the initial memory. **/
    bool TestRealloc(int32 size1, int32 size2);

    /** 
     * @brief Tests the correct behavior of the string duplicate.
     * @param s is the string to duplicate.
     * @return true if the string result of the function is equal to s.**/
    bool TestMemoryStringDup(const char *s);

    /** 
     * @brief Tests the shared memory between two different threads and the main process.
     * @return true if the shared int and the shared bool are consistent with the operations done by threads. **/
    bool TestSharedMemory();

    /** 
     * @brief Tests the copy and the move functions.
     * @return true if the memory is copied correctly. */
    bool TestCopyAndMove();

    /**
     * @brief Tests the set and the search functions.
     * @return true if the functions work correctly. */
    bool TestSetAndSearch();
};

#endif
