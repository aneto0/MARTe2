/**
 * @file GlobalObjectsDatabaseTest.cpp
 * @brief Source file for class GlobalObjectsDatabaseTest
 * @date 26/09/2015
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
 * the class GlobalObjectsDatabaseTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "GlobalObjectsDatabaseTest.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
bool GlobalObjectsDatabaseTest::TestInstance() {
    return (GlobalObjectsDatabase::Instance() != NULL_PTR(GlobalObjectsDatabase *));
}

bool GlobalObjectsDatabaseTest::TestPeek() {
    bool result = false;
    uint32 i;
    GlobalObjectsDatabase *database = GlobalObjectsDatabase::Instance();
    for (i = 0; i < NUMBER_OF_GLOBAL_OBJECTS; i++) {
        const GlobalObjectI *globalObjectI = database->Peek(i);
        if(globalObjectI != NULL_PTR(GlobalObjectI *)){
            const char8 *const className = globalObjectI->GetClassName();
            if(StringHelper::Compare(className, "ClassRegistryDatabase") == 0){
                result = true;
                break;
            }
        }
    }
    return result;
}

bool GlobalObjectsDatabaseTest::TestGetStandardHeap() {
    HeapI *heap = GlobalObjectsDatabase::Instance()->GetStandardHeap();
    StandardHeap *stdHeap = dynamic_cast<StandardHeap *>(heap);
    return (stdHeap != NULL_PTR(StandardHeap *));
}
}
