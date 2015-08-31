/**
 * @file MemoryCheckTest.cpp
 * @brief Source file for class MemoryCheckTest
 * @date 25/08/2015
 * @author Llorenç Capellà
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
 * the class MemoryCheckTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryCheckTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MemoryCheckTest::MemoryCheckTest() {
    // Auto-generated constructor stub for MemoryCheckTest
    // TODO Verify if manual additions are needed
}

MemoryCheckTest::~MemoryCheckTest() {
    // Auto-generated destructor stub for MemoryCheckTest
    // TODO Verify if manual additions are needed
}

bool MemoryCheckTest::TestCheck() {
    uint32 size = 100;
    //allocate a space of size integers
    int32* allocated = (int32*) HeapManager::Malloc(size * sizeof(int32));

    //checks if all the memory is allocated correctly
    if (!MemoryCheck::Check(allocated, (MemoryCheck::MemoryTestAccessMode) (MemoryCheck::Read | MemoryCheck::Write | MemoryCheck::Execute), size * sizeof(int32))) {
        return false;
    }

    //checks if a part the memory is allocated correctly
    if (!MemoryCheck::Check(allocated, (MemoryCheck::MemoryTestAccessMode) (MemoryCheck::Read | MemoryCheck::Write | MemoryCheck::Execute), (size / 2) * sizeof(int32))) {
        return false;
    }

    //0 as size
    uint32 testSize = 0;
    if (!MemoryCheck::Check(allocated, (MemoryCheck::MemoryTestAccessMode) (MemoryCheck::Read | MemoryCheck::Write | MemoryCheck::Execute), testSize)) {
        return false;
    }

    HeapManager::Free((void*&) allocated);

    //the check function on a null pointer should return false
    return !MemoryCheck::Check(NULL, (MemoryCheck::MemoryTestAccessMode) (MemoryCheck::Read | MemoryCheck::Write | MemoryCheck::Execute), size);

}

