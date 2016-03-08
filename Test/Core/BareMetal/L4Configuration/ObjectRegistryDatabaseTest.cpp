/**
 * @file ObjectRegistryDatabaseTest.cpp
 * @brief Source file for class ObjectRegistryDatabaseTest
 * @date 18/feb/2016
 * @author pc
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
#include "ConfigurationDatabase.h"
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

bool ObjectRegistryDatabaseTest::TestInitialise() {
ObjectRegistryDatabase *db=ObjectRegistryDatabase::Instance();

ConfigurationDatabase cdb;
cdb.CreateAbsolute("+PID1");
cdb.CreateAbsolute("+PID2");
cdb.MoveAbsolute("+PID1");
cdb.Write("Class", "PID");
cdb.Write("Kp", 1);
cdb.Write("Ki", 2);
cdb.Write("Kd", 3);
cdb.MoveAbsolute("+PID2");
cdb.Write("Class", "PID");
cdb.Write("Kp", 4);
cdb.Write("Ki", 5);
cdb.Write("Kd", 6);

cdb.MoveToRoot();
db->Initialise(cdb);

ReferenceT<PID> test=db->Find("+PID2");

printf("\n%d\n", test->Kp);
return true;
}




bool ObjectRegistryDatabaseTest::TestFind(){

    ObjectRegistryDatabase *db=ObjectRegistryDatabase::Instance();
    ConfigurationDatabase cdb;
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
    db->Initialise(cdb);


    ReferenceT<PID> test=db->Find("$A.$B.$C.+PID");

    ReferenceT<PID> test1=db->Find("+PID", test, 2);

    ReferenceT<PID> test2=db->Find("+PID", test, 3);

    printf("\n%d\n", test->Kp);
    printf("\n%d\n", test1->Kp);
    printf("\n%d\n", test2->Kp);
    return true;

}
