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

#include "ObjectRegistryDatabaseTest.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
bool PID::Initialise(StructuredDataI & data) {
    bool ok = data.Read("Kp", Kp);
    ok &= data.Read("Ki", Ki);
    ok &= data.Read("Kd", Kd);
    return ok;
}
CLASS_REGISTER(PID, "1.0")

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

bool ObjectRegistryDatabaseTest::TestFind() {
    ReferenceT<PID> test = ObjectRegistryDatabase::Instance()->Find("$A.$B.$C.+PID");

    if (test->Kp != 1) {
        return false;
    }
    if (test->Ki != 2) {
        return false;
    }
    if (test->Kd != 3) {
        return false;
    }

    return true;
}

bool ObjectRegistryDatabaseTest::TestFind_Relative() {
    ReferenceT<PID> test = ObjectRegistryDatabase::Instance()->Find("$A.$B.$C.+PID");

    ReferenceT<PID> test2 = ObjectRegistryDatabase::Instance()->Find(":::+PID", test);
    if (test2->Kp != 7) {
        return false;
    }
    if (test2->Ki != 8) {
        return false;
    }
    if (test2->Kd != 9) {
        return false;
    }

    ReferenceT<ReferenceContainer> start = ObjectRegistryDatabase::Instance()->Find("$A.$B");
    // relative search
    ReferenceT<PID> test4 = ObjectRegistryDatabase::Instance()->Find(":$C.+PID", start);
    if (test4->Kp != 1) {
        return false;
    }
    if (test4->Ki != 2) {
        return false;
    }
    if (test4->Kd != 3) {
        return false;
    }
    return true;
}

bool ObjectRegistryDatabaseTest::TestFind_Absolute() {
    ReferenceT<ReferenceContainer> start = ObjectRegistryDatabase::Instance()->Find("$A.$B");

// absolute search
    ReferenceT<PID> test5 = ObjectRegistryDatabase::Instance()->Find("$A.$B.$C.+PID", start);
    if (test5->Kp != 1) {
        return false;
    }
    if (test5->Ki != 2) {
        return false;
    }
    if (test5->Kd != 3) {
        return false;
    }

    return true;

}

bool ObjectRegistryDatabaseTest::TestFindTooManyBackSteps() {
    ReferenceT<PID> start = ObjectRegistryDatabase::Instance()->Find("$A.$B.$C.+PID");
    if (!start.IsValid()) {
        return false;
    }

// searches from the beginning
    ReferenceT<PID> test2 = ObjectRegistryDatabase::Instance()->Find("::::+PID", start);
    if (test2->Kp != 7) {
        return false;
    }
    if (test2->Ki != 8) {
        return false;
    }
    if (test2->Kd != 9) {
        return false;
    }

    return true;
}

bool ObjectRegistryDatabaseTest::TestGetClassName() {
    return StringHelper::Compare(ObjectRegistryDatabase::Instance()->GetClassName(), "ObjectRegistryDatabase") == 0;
}

