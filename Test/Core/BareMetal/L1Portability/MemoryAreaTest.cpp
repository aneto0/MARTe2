/**
 * @file MemoryAreaTest.cpp
 * @brief Source file for class MemoryAreaTest
 * @date 10/03/2016
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

 * @details This source file contains the definition of all the methods for
 * the class MemoryAreaTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryAreaTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool MemoryAreaTest::TestConstructor() {
    MemoryArea memory;
    if (memory.GetMemoryStart() != NULL) {
        return false;
    }
    return memory.GetMemorySize() == 0u;
}

bool MemoryAreaTest::TestAdd_OnlySize() {
    MemoryArea memory;
    const uint32 size = 32;
    uint32 offset[size];
    for (uint32 i = 0; i < size; i++) {
        if (!memory.Add(sizeof(uint32), offset[i])) {
            return false;
        }
        *(uint32*) memory.GetPointer(offset[i]) = i;
    }
    for (uint32 i = 0; i < size; i++) {
        if (*(uint32*) memory.GetPointer(offset[i]) != i) {
            return false;
        }

    }

    return true;
}

bool MemoryAreaTest::TestAdd_Element() {
    MemoryArea memory;
    const uint32 size = 32;
    uint32 element[size];
    uint32 offset[size];
    for (uint32 i = 0; i < size; i++) {
        element[i] = i;
        if (!memory.Add(&element[i], sizeof(uint32), offset[i])) {
            return false;
        }
    }
    for (uint32 i = 0; i < size; i++) {
        if (*((uint32*) memory.GetPointer(offset[i])) != i) {
            return false;
        }
    }

    return true;

}

bool MemoryAreaTest::TestFree() {
    MemoryArea memory;
    uint32 offset = 0u;
    memory.Add(32, offset);
    if (memory.GetMemorySize() != 32) {
        return false;
    }
    if (memory.GetMemoryStart() == NULL) {
        return false;
    }
    memory.Free();
    if (memory.GetMemoryStart() != NULL) {
        return false;
    }
    return memory.GetMemorySize() == 0;
}

bool MemoryAreaTest::TestGetMemoryStart() {
    MemoryArea memory;
    if (memory.GetMemoryStart() != NULL) {
        return false;
    }
    uint32 offset = 0u;
    memory.Add(32, offset);
    return memory.GetMemoryStart() != NULL;
}

bool MemoryAreaTest::TestGetMemorySize(uint32 size) {
    MemoryArea memory;
    if (memory.GetMemorySize() != 0) {
        return false;
    }
    uint32 offset = 0u;
    memory.Add(size, offset);
    return memory.GetMemorySize() == size;
}


bool MemoryAreaTest::TestGetPointer(){
    return TestAdd_OnlySize();
}


