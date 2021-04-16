/**
 * @file StandardHeap_GenericTest.cpp
 * @brief Source file for class StandardHeap_GenericTest
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
 * the class StandardHeap_GenericTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StandardHeap_GenericTest.h"
#include INCLUDE_FILE_ENVIRONMENT(BareMetal,L1Portability,ENVIRONMENT,StandardHeap.h)

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;


StandardHeap_GenericTest::StandardHeap_GenericTest() {
    retVal = false;
    ptr = NULL;
    intPtr = NULL;
    // Auto-generated constructor stub for StandardHeap_GenericTest
    // TODO Verify if manual additions are needed
}

StandardHeap_GenericTest::~StandardHeap_GenericTest() {
    // Auto-generated destructor stub for StandardHeap_GenericTest
    // TODO Verify if manual additions are needed
}

bool StandardHeap_GenericTest::TestConstructor(){
    StandardHeap sh;
    retVal = (sh.FirstAddress() == 0u) && (sh.LastAddress() == 0u);
    return retVal;
}

bool StandardHeap_GenericTest::TestMalloc(){
    StandardHeap sh;
    uint32 size = 1;
    ptr = sh.Malloc(size);
    retVal = (ptr != NULL);
    sh.Free((void*&)ptr);
    return (retVal);
}

bool StandardHeap_GenericTest::TestMallocFirstAdress(){
    StandardHeap sh;
    uint32 size = 2;
    ptr = sh.Malloc(size);
    retVal = reinterpret_cast<uintp>(ptr) == sh.FirstAddress();
    sh.Free(ptr);
    return (retVal);
}

bool StandardHeap_GenericTest::TestMallocLastAdress(){
    StandardHeap sh;
    uint32 size = 3;
    ptr = sh.Malloc(size);
    retVal = reinterpret_cast<uintp>(ptr) + size == sh.LastAddress();
    sh.Free(ptr);
    return (retVal);
}

bool StandardHeap_GenericTest::TestMallocReadWrite(){
    StandardHeap sh;
    uint32 size = 3;
    ptr = sh.Malloc(size*4);
    intPtr = static_cast<int*>(ptr);
    *intPtr = 0;
    *(intPtr + 1) = 1;
    *(intPtr + 2) = 2;
    retVal = (*intPtr == 0);
    retVal &= (*(intPtr + 1) == 1);
    retVal &= (*(intPtr + 2) == 2);
    sh.Free(ptr);
    return (retVal);
}

bool StandardHeap_GenericTest::TestFree(){
    StandardHeap sh;
    uint32 size = 3;
    ptr = sh.Malloc(size);
    retVal = true;
    if(ptr == NULL){
        retVal = false;
    }
    sh.Free(ptr);
    retVal &= (ptr == NULL);
    return retVal;
}
/*
bool StandardHeap_GenericTest::TestRealloc(){
    StandardHeap sh;
    uint32 size = 3;
    void *ptr1 = NULL;
    ptr = sh.Malloc(size);
    ptr1 = sh.Realloc(ptr, size);
    retVal = true;
    if(ptr == NULL){
        retVal = false;
    }
    if(ptr1 == NULL){
        retVal = false;
    }
    retVal &=
    sh.Free(ptr);
    retVal &= (ptr == NULL);
    return retVal;
}
*/



