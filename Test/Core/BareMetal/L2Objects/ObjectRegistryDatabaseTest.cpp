/**
 * @file ObjectRegistryDatabaseTest.cpp
 * @brief Source file for class ObjectRegistryDatabaseTest
 * @date 18/02/2016
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
 * the class ObjectRegistryDatabaseTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassRegistryItemT.h"
#include "ObjectRegistryDatabaseTest.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ObjectRegistryDatabaseTest::ObjectRegistryDatabaseTest() {
    cdb.CreateAbsolute("$A");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateRelative("+PID");
    cdb.Write("Class", "PID");
    cdb.Write("Kp", 7);
    cdb.Write("Ki", 8);
    cdb.Write("Kd", 9);
    cdb.CreateAbsolute("$A.$B");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateRelative("+PID");
    cdb.Write("Class", "PID");
    cdb.Write("Kp", 4);
    cdb.Write("Ki", 5);
    cdb.Write("Kd", 6);
    cdb.CreateAbsolute("$A.$B.$C");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateRelative("+PID");
    cdb.Write("Class", "PID");
    cdb.Write("Kp", 1);
    cdb.Write("Ki", 2);
    cdb.Write("Kd", 3);
    cdb.MoveToRoot();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);

}

ObjectRegistryDatabaseTest::~ObjectRegistryDatabaseTest() {
    ObjectRegistryDatabase::Instance()->CleanUp();
}

bool ObjectRegistryDatabaseTest::TestInstance() {
    return ObjectRegistryDatabase::Instance() != NULL;
}

bool ObjectRegistryDatabaseTest::TestGetClassName() {
    return StringHelper::Compare(ObjectRegistryDatabase::Instance()->GetClassName(), "ObjectRegistryDatabase") == 0;
}

