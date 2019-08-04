/**
 * @file StandardHeap_Generic.cpp
 * @brief Source file for class StandardHeap
 * @date 13/08/2015
 * @author Filippo Sartori
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "License")
 * You may not use this work except in compliance with the License.
 * You may obtain a copy of the License at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License permissions and limitations under the License.

 * @details This source file contains the definition of all the methods for
 * the class StandardHeap (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#ifndef LINT
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "StandardHeap_Generic.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

StandardHeap::StandardHeap() {
}

StandardHeap::~StandardHeap() {
}

/*lint -e{586} use of malloc function (deprecated) */
void *StandardHeap::Malloc(const uint32 size) {
    //void *pointer = malloc(size);
    //void *pointer = new char8[size];

    void* pointer = NULL_PTR(void*);

    if (size != 0u) {
        pointer = malloc(static_cast<osulong>(size));
    }

    ErrorManagement::ErrorType err;
    err.outOfMemory = (pointer == NULL);
    REPORT_ERROR(err, "StandardHeap: Failed Failed malloc()");
    return pointer;

}

/*lint -e{586} use of free function (deprecated) */
void StandardHeap::Free(void *&data) {
    if (data != NULL) {
        free(data);
    }
    data = NULL_PTR(void *);
}

/*lint -e{586} use of realloc function (deprecated) */
void *StandardHeap::Realloc(void *&data,
                            const uint32 newSize) {

    if (data == NULL) {
        data = StandardHeap::Malloc(newSize);
    }
    else {
        if (newSize == 0u) {
            StandardHeap::Free(data);
        }
        else {
            data = realloc(data, static_cast<osulong>(newSize));
            ErrorManagement::ErrorType err;
            err.outOfMemory = (data == NULL);
            REPORT_ERROR(err, "StandardHeap: Failed realloc()");
        }
    }
    return data;

}

namespace HeapManager{

/**
 * these pointers need to be defined and assigned somewhere if malloc or new are used during initialisation
 */
void *std_malloc(size_t size){
	return malloc(size);
}

/**
 * these pointers need to be defined and assigned somewhere if free or delete are used
 */
void std_free(void *p){
	free(p);
}

/**
 * these pointers need to be defined and assigned somewhere if realloc is used
 */
void *std_realloc(void *p, size_t size){
	return realloc(p,size);
}

}

}

