/**
 * @file ClassRegistryDatabaseTest.cpp
 * @brief Source file for class ClassRegistryDatabaseTest
 * @date 06/08/2015
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
 * the class ClassRegistryDatabaseTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ClassRegistryDatabaseTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include "Reference.h"
#include "TestObjectHelper1.h"
bool ClassRegistryDatabaseTest::TestConstructor() {
    ClassRegistryDatabase db = ClassRegistryDatabase::Instance();
    printf("ClassRegistryDatabase size = %d\n", db.ListSize());
    uint32 i = 0u;
    for (i = 0; i < db.ListSize(); i++) {
        ClassRegistryItem *item = (ClassRegistryItem *) db.ListPeek(i);
        printf("[%d] %s %d\n", i, item->GetClassProperties()->GetName(), item->GetNumberOfInstances());
    }
    Heap h;
    TestObjectHelper1 *obj1 = dynamic_cast<TestObjectHelper1 *>(ClassRegistryDatabase::Instance().CreateByName("TestObjectHelper1", h));
    Reference ref1 = obj1;
    printf("ref1 = %d\n", ref1.NumberOfReferences());
    if (obj1 != NULL) {
        Reference ref2 = obj1;
        printf("ref2 = %d\n", ref2.NumberOfReferences());
    }
    printf("ref1 = %d\n", ref1.NumberOfReferences());
    for (i = 0; i < db.ListSize(); i++) {
        ClassRegistryItem *item = (ClassRegistryItem *) db.ListPeek(i);
        printf("[%d] %s %d\n", i, item->GetClassProperties()->GetName(), item->GetNumberOfInstances());
    }

    return true;
}

