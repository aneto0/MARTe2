/**
 * @file GlobalObjectsDatabase.cpp
 * @brief Source file for class GlobalObjectsDatabase
 * @date 25/09/2015
 * @author Andre' Neto
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
 * the class GlobalObjectsDatabase (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GlobalObjectsDatabase.h"
#include "HardwareI.h"
#include "HeapManager.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

GlobalObjectsDatabase *GlobalObjectsDatabase::Instance() {
    static GlobalObjectsDatabase instance;
    return &instance;
}

GlobalObjectsDatabase::GlobalObjectsDatabase() {
    HardwareI::InitArchitecture();
    HardwareI::InitEnvironment();
    uint32 i;
    for (i = 0u; i < NUMBER_OF_GLOBAL_OBJECTS; i++) {
        databaseList[i] = NULL_PTR(GlobalObjectI *);
        destructionListOrder[i] = NUMBER_OF_GLOBAL_OBJECTS;
    }
}

/*lint -e{1551} the delete could indeed throw an exception if there is an issue
 * destroying one of the global objects registered in the database. Nevertheless
 * this would only happen at the exiting of the program (given that by design
 * GlobalObjectsDatabase::~GlobalObjectsDatabase is the last object to be destroyed
 * before ending the program).
 */
GlobalObjectsDatabase::~GlobalObjectsDatabase() {
    uint32 i;
    uint32 smallest = 0u;
    uint32 deleteIdx = 0u;
    while (smallest != NUMBER_OF_GLOBAL_OBJECTS) {
        smallest = NUMBER_OF_GLOBAL_OBJECTS;
        for (i = 0u; i < NUMBER_OF_GLOBAL_OBJECTS; i++) {
            if (smallest > destructionListOrder[i]) {
                smallest = destructionListOrder[i];
                deleteIdx = i;
            }
        }
        if (smallest != NUMBER_OF_GLOBAL_OBJECTS) {
            destructionListOrder[deleteIdx] = NUMBER_OF_GLOBAL_OBJECTS;
            delete databaseList[deleteIdx];
        }
    }
}

void GlobalObjectsDatabase::Add(const GlobalObjectI * const globalObject,
                                const uint32 destructionOrder) {
    uint32 i;
    for (i = 0u; i < NUMBER_OF_GLOBAL_OBJECTS; i++) {
        if (databaseList[i] == NULL_PTR(GlobalObjectI *)) {
            databaseList[i] = globalObject;
            destructionListOrder[i] = destructionOrder;
            break;
        }
    }
}

const GlobalObjectI *GlobalObjectsDatabase::Peek(const uint32 idx) const {
    return databaseList[idx];
}

/*lint -e{1536} There is only and only one StandardHeap implementation. The GlobalObjectsDatabase
 * singleton provides the only access point to this heap.
 */
HeapI *GlobalObjectsDatabase::GetStandardHeap() {
    return &standardHeap;
}

}
