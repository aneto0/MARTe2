/**
 * @file RealTimeDataSourceBrokerTest.cpp
 * @brief Source file for class RealTimeDataSourceBrokerTest
 * @date 11/03/2016
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
 * the class RealTimeDataSourceBrokerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSourceBrokerTest.h"
#include "GAMTestHelper.h"
#include "RealTimeGenericDataDef.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeDataSourceBrokerTest::RealTimeDataSourceBrokerTest() {

    cdb.CreateAbsolute("+Inputs");
    cdb.Write("Class", "RealTimeDataDefContainer");
    cdb.Write("IsInput", "true");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Inputs.+Error");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "TrackError");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Inputs.+Error.+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Default", "2");
    cdb.Write("Path", "+DDB1.PidError2");
    cdb.Write("IsFinal", "true");

    cdb.CreateAbsolute("+Outputs");
    cdb.Write("Class", "RealTimeDataDefContainer");
    cdb.Write("IsOutput", "true");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Outputs.+Control");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Outputs.+Control.+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "+DDB2.PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");

    cdb.CreateAbsolute("+Outputs.+Noise");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlNoise");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Outputs.+Noise.+noiseValue");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "float32");
    cdb.Write("Default", "2");
    cdb.Write("Path", "+DDB2.PidNoise");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();
}

bool RealTimeDataSourceBrokerTest::TestConstructor() {
    RealTimeDataSourceBroker test;
    return true;
}

bool RealTimeDataSourceBrokerTest::TestSetApplication() {
    RealTimeDataSourceBroker test;
    ReferenceT<RealTimeApplication> rtApp;
    test.SetApplication(*rtApp.operator ->());
    return true;
}

bool RealTimeDataSourceBrokerTest::TestAddVariable_Allocation() {

    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1","thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Inputs.+Error");

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    if (broker->GetData(0) != NULL) {
        return false;
    }

    if (!broker->AddVariable(def)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetData(0) == NULL) {
        return false;
    }

    return true;
}

bool RealTimeDataSourceBrokerTest::TestAddVariable_Static() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1","thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Inputs.+Error");

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    TrackError output;

    if (broker->GetData(0) != NULL) {
        return false;
    }

    if (!broker->AddVariable(def, &output)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetData(0) != &output) {
        return false;
    }

    return true;
}

bool RealTimeDataSourceBrokerTest::TestAddVariable_Allocation_BasicType() {

    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    cdb.CreateAbsolute("+Inputs.+Basic");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Default", "2");
    cdb.Write("Path", "+DDB1.Basic");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1","thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Inputs.+Basic");

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());
    if (!broker->AddVariable(def)) {
        return false;
    }
    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetData(0) == NULL) {
        return false;
    }
    return true;

}

bool RealTimeDataSourceBrokerTest::TestAddVariable_Static_BasicType() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    cdb.CreateAbsolute("+Inputs.+Basic");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Default", "2");
    cdb.Write("Path", "+DDB1.Basic");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1","thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Inputs.+Basic");

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    uint32 x;

    broker->SetApplication(*rtapp.operator ->());
    if (!broker->AddVariable(def, &x)) {
        return false;
    }
    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetData(0) != &x) {
        return false;
    }
    return true;

}

bool RealTimeDataSourceBrokerTest::TestAddVariableFalse_NoApplicationSet() {

    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1","thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Inputs.+Error");

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    TrackError output;

    if (broker->GetData(0) != NULL) {
        return false;
    }

    return (!broker->AddVariable(def, &output));
}

bool RealTimeDataSourceBrokerTest::TestAddVariableFalse_NoDataInApplication() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Datas");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Datas.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Datas.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        printf("\nfail here\n");
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1","thread1");

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Inputs.+Error");

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    TrackError output;

    return (!broker->AddVariable(def, &output));
}

bool RealTimeDataSourceBrokerTest::TestAddVariableFalse_InvalidDef() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }
    ReferenceT<RealTimeGenericDataDef> def;

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    TrackError output;

    return (!broker->AddVariable(def, &output));
}

bool RealTimeDataSourceBrokerTest::TestAddVariableFalse_UnintrospectableType() {
    ConfigurationDatabase cdbLoc;

    cdbLoc.Write("Class", "RealTimeGenericDataDef");
    cdbLoc.Write("Type", "Object");
    cdbLoc.Write("IsFinal", "false");
    cdbLoc.CreateAbsolute("+Par1");
    cdbLoc.Write("Class", "RealTimeGenericDataDef");
    cdbLoc.Write("Type", "uint32");
    cdbLoc.Write("Path", "DDB2.PidControl1");
    cdbLoc.Write("Default", "1");
    cdbLoc.Write("IsFinal", "true");
    cdbLoc.CreateAbsolute("+Par2");
    cdbLoc.Write("Class", "RealTimeGenericDataDef");
    cdbLoc.Write("Type", "uint32");
    cdbLoc.Write("Path", "PidControl2");
    cdbLoc.Write("Default", "1");
    cdbLoc.Write("IsFinal", "true");

    cdbLoc.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> def = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!def->Initialise(cdbLoc)) {
        return false;
    }

    ConfigurationDatabase appCDB;

    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> app = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!app->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*app.operator ->());

    return !broker->AddVariable(def);
}

bool RealTimeDataSourceBrokerTest::TestAddVariableFalse_NotInDataSource() {
    ConfigurationDatabase cdbLoc;

    cdbLoc.Write("Class", "RealTimeGenericDataDef");
    cdbLoc.Write("Type", "ControlIn");
    cdbLoc.Write("IsFinal", "false");
    cdbLoc.CreateAbsolute("+Par1");
    cdbLoc.Write("Class", "RealTimeGenericDataDef");
    cdbLoc.Write("Type", "uint32");
    cdbLoc.Write("Path", "DDB2.PidControl1");
    cdbLoc.Write("Default", "1");
    cdbLoc.Write("IsFinal", "true");
    cdbLoc.CreateAbsolute("+Par2");
    cdbLoc.Write("Class", "RealTimeGenericDataDef");
    cdbLoc.Write("Type", "uint32");
    cdbLoc.Write("Path", "PidControl2");
    cdbLoc.Write("Default", "1");
    cdbLoc.Write("IsFinal", "true");

    cdbLoc.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> def = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!def->Initialise(cdbLoc)) {
        return false;
    }

    ConfigurationDatabase appCDB;

    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> app = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!app->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*app.operator ->());

    return !broker->AddVariable(def);
}

bool RealTimeDataSourceBrokerTest::TestAddVariable_MultiDimensional_Static_Vector() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ConfigurationDatabase cdbMulti;

    cdbMulti.CreateAbsolute("+Inputs");
    cdbMulti.Write("Class", "RealTimeDataDefContainer");
    cdbMulti.Write("IsInput", "true");
    cdbMulti.Write("IsFinal", "false");
    cdbMulti.CreateAbsolute("+Inputs.+Error");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "TrackErrorArray");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.CreateAbsolute("+Inputs.+Error.+Pars");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "uint32");
    cdbMulti.Write("Modifiers", "[2]");
    cdbMulti.Write("Default", "{1,2}");
    cdbMulti.Write("Path", "+DDB1.PidErrorArray");
    cdbMulti.Write("IsFinal", "true");

    cdbMulti.CreateAbsolute("+Outputs");
    cdbMulti.Write("Class", "RealTimeDataDefContainer");
    cdbMulti.Write("IsOutput", "true");
    cdbMulti.Write("IsFinal", "false");
    cdbMulti.CreateAbsolute("+Outputs.+Control");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "ControlInArray");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.CreateAbsolute("+Outputs.+Control.+Pars");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "uint32");
    cdbMulti.Write("Modifiers", "[2]");
    cdbMulti.Write("Path", "+DDB2.PidControlArray");
    cdbMulti.Write("Default", "{3,4}");
    cdbMulti.Write("IsFinal", "true");

    cdbMulti.MoveToRoot();

    ReferenceT<PIDGAM2> gam = ReferenceT<PIDGAM2>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdbMulti)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1","thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Inputs.+Error");

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    TrackErrorArray output;

    output.Pars[0] = 1;
    output.Pars[1] = 2;

    if (broker->GetData(0) != NULL) {
        return false;
    }

    if (!broker->AddVariable(def, &output)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetData(0) != &output) {
        return false;
    }

    TrackErrorArray *ret = (TrackErrorArray *) broker->GetData(0);
    if (ret->Pars[0] != 1) {
        return false;
    }

    if (ret->Pars[1] != 2) {
        return false;
    }

    return true;
}

bool RealTimeDataSourceBrokerTest::TestAddVariable_MultiDimensional_Allocation_Vector() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ConfigurationDatabase cdbMulti;

    cdbMulti.CreateAbsolute("+Inputs");
    cdbMulti.Write("Class", "RealTimeDataDefContainer");
    cdbMulti.Write("IsInput", "true");
    cdbMulti.Write("IsFinal", "false");
    cdbMulti.CreateAbsolute("+Inputs.+Error");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "TrackErrorArray");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.CreateAbsolute("+Inputs.+Error.+Pars");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "uint32");
    cdbMulti.Write("Default", "{1,2}");
    cdbMulti.Write("Modifiers", "[2]");
    cdbMulti.Write("Path", "+DDB1.PidErrorArray");
    cdbMulti.Write("IsFinal", "true");

    cdbMulti.CreateAbsolute("+Outputs");
    cdbMulti.Write("Class", "RealTimeDataDefContainer");
    cdbMulti.Write("IsOutput", "true");
    cdbMulti.Write("IsFinal", "false");
    cdbMulti.CreateAbsolute("+Outputs.+Control");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "ControlInArray");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.CreateAbsolute("+Outputs.+Control.+Pars");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "uint32");
    cdbMulti.Write("Modifiers", "[2]");
    cdbMulti.Write("Path", "+DDB2.PidControlArray");
    cdbMulti.Write("Default", "{3,4}");
    cdbMulti.Write("IsFinal", "true");

    cdbMulti.MoveToRoot();

    ReferenceT<PIDGAM2> gam = ReferenceT<PIDGAM2>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdbMulti)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1","thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Inputs.+Error");

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    if (broker->GetData(0) != NULL) {
        return false;
    }

    if (!broker->AddVariable(def)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    TrackErrorArray *ret = (TrackErrorArray *) broker->GetData(0);

    return ret != NULL;
}

bool RealTimeDataSourceBrokerTest::TestAddVariable_MultiDimensional_Static_Matrix() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ConfigurationDatabase cdbMulti;

    cdbMulti.CreateAbsolute("+Inputs");
    cdbMulti.Write("Class", "RealTimeDataDefContainer");
    cdbMulti.Write("IsInput", "true");
    cdbMulti.Write("IsFinal", "false");
    cdbMulti.CreateAbsolute("+Inputs.+Error");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "TrackErrorMatrix");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.CreateAbsolute("+Inputs.+Error.+Pars");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "uint32");
    cdbMulti.Write("Modifiers", "[3][2]");
    cdbMulti.Write("Default", "{{1,2},{3,4},{5,6}}");
    cdbMulti.Write("Path", "+DDB1.PidErrorArray");
    cdbMulti.Write("IsFinal", "true");

    cdbMulti.CreateAbsolute("+Outputs");
    cdbMulti.Write("Class", "RealTimeDataDefContainer");
    cdbMulti.Write("IsOutput", "true");
    cdbMulti.Write("IsFinal", "false");
    cdbMulti.CreateAbsolute("+Outputs.+Control");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "ControlInMatrix");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.CreateAbsolute("+Outputs.+Control.+Pars");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "uint32");
    cdbMulti.Write("Modifiers", "[3][2]");
    cdbMulti.Write("Path", "+DDB2.PidControlArray");
    cdbMulti.Write("Default", "{{7,8},{9,10},{11,12}}");
    cdbMulti.Write("IsFinal", "true");

    cdbMulti.MoveToRoot();

    ReferenceT<PIDGAM3> gam = ReferenceT<PIDGAM3>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdbMulti)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1","thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Inputs.+Error");

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    TrackErrorMatrix output;

    output.Pars[0][0] = 1;
    output.Pars[0][1] = 2;
    output.Pars[1][0] = 3;
    output.Pars[1][1] = 4;
    output.Pars[2][0] = 5;
    output.Pars[2][1] = 6;

    if (broker->GetData(0) != NULL) {
        return false;
    }

    if (!broker->AddVariable(def, &output)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetData(0) != &output) {
        return false;
    }

    TrackErrorMatrix *ret = (TrackErrorMatrix *) broker->GetData(0);
    if (ret->Pars[0][0] != 1) {
        return false;
    }

    if (ret->Pars[0][1] != 2) {
        return false;
    }
    if (ret->Pars[1][0] != 3) {
        return false;
    }

    if (ret->Pars[1][1] != 4) {
        return false;
    }

    if (ret->Pars[2][0] != 5) {
        return false;
    }

    if (ret->Pars[2][1] != 6) {
        return false;
    }

    return true;
}

bool RealTimeDataSourceBrokerTest::TestAddVariable_MultiDimensional_Allocation_Matrix() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ConfigurationDatabase cdbMulti;

    cdbMulti.CreateAbsolute("+Inputs");
    cdbMulti.Write("Class", "RealTimeDataDefContainer");
    cdbMulti.Write("IsInput", "true");
    cdbMulti.Write("IsFinal", "false");
    cdbMulti.CreateAbsolute("+Inputs.+Error");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "TrackErrorMatrix");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.CreateAbsolute("+Inputs.+Error.+Pars");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "uint32");
    cdbMulti.Write("Modifiers", "[3][2]");
    cdbMulti.Write("Default", "{{1,2},{3,4},{5,6}}");
    cdbMulti.Write("Path", "+DDB1.PidErrorArray");
    cdbMulti.Write("IsFinal", "true");

    cdbMulti.CreateAbsolute("+Outputs");
    cdbMulti.Write("Class", "RealTimeDataDefContainer");
    cdbMulti.Write("IsOutput", "true");
    cdbMulti.Write("IsFinal", "false");
    cdbMulti.CreateAbsolute("+Outputs.+Control");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "ControlInMatrix");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.CreateAbsolute("+Outputs.+Control.+Pars");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "uint32");
    cdbMulti.Write("Modifiers", "[3][2]");
    cdbMulti.Write("Path", "+DDB2.PidControlArray");
    cdbMulti.Write("Default", "{{7,8},{9,10},{11,12}}");
    cdbMulti.Write("IsFinal", "true");

    cdbMulti.MoveToRoot();

    ReferenceT<PIDGAM3> gam = ReferenceT<PIDGAM3>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdbMulti)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1","thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Inputs.+Error");

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    if (broker->GetData(0) != NULL) {
        return false;
    }

    if (!broker->AddVariable(def)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    TrackErrorMatrix *ret = (TrackErrorMatrix *) broker->GetData(0);

    return (ret != NULL);
}

bool RealTimeDataSourceBrokerTest::TestAddVariableFalse_UnsupportedMultidimStructured() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ConfigurationDatabase cdbMulti;

    cdbMulti.CreateAbsolute("+Inputs");
    cdbMulti.Write("Class", "RealTimeDataDefContainer");
    cdbMulti.Write("IsInput", "true");
    cdbMulti.Write("IsFinal", "false");
    cdbMulti.CreateAbsolute("+Inputs.+Error");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "TrackErrorMatrix");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.Write("Modifiers", "[3][2]");
    cdbMulti.Write("Path", "+DDB1.PidErrorStructured");
    cdbMulti.Write("IsFinal", "true");

    cdbMulti.CreateAbsolute("+Outputs");
    cdbMulti.Write("Class", "RealTimeDataDefContainer");
    cdbMulti.Write("IsOutput", "true");
    cdbMulti.Write("IsFinal", "false");
    cdbMulti.CreateAbsolute("+Outputs.+Control");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "ControlInMatrix");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.CreateAbsolute("+Outputs.+Control.+Pars");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "uint32");
    cdbMulti.Write("Modifiers", "[3][2]");
    cdbMulti.Write("Path", "+DDB2.PidControlArray");
    cdbMulti.Write("Default", "{{7,8},{9,10},{11,12}}");
    cdbMulti.Write("IsFinal", "true");

    cdbMulti.MoveToRoot();

    ReferenceT<PIDGAM3> gam = ReferenceT<PIDGAM3>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdbMulti)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1","thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Inputs.+Error");

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    return (!broker->AddVariable(def));
}

bool RealTimeDataSourceBrokerTest::TestAddVariableFalse_UnregisteredType() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ConfigurationDatabase cdbDef;
    cdbDef.Write("Class", "RealTimeGenericDataDef");
    cdbDef.Write("Type", "Invalid");
    cdbDef.Write("IsFinal", "true");
    cdbDef.Write("Path", "+DDB1.PidErrorStructured");
    cdbDef.Write("IsFinal", "true");
    cdbDef.MoveToRoot();


    ReferenceT<RealTimeGenericDataDef> def = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if(!def->Initialise(cdbDef)){
        return false;
    }
    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());
    return (!broker->AddVariable(def));
}

bool RealTimeDataSourceBrokerTest::TestAddVariableFalse_UnfoundMember() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ConfigurationDatabase cdbDef;
    cdbDef.Write("Class", "RealTimeGenericDataDef");
    cdbDef.Write("Type", "TrackError");
    cdbDef.Write("IsFinal", "true");
    cdbDef.CreateAbsolute("+Par3");
    cdbDef.Write("Class", "RealTimeGenericDataDef");
    cdbDef.Write("Type", "uint32");
    cdbDef.Write("Default", "1");
    cdbDef.Write("Path", "+DDB1.PidError1");
    cdbDef.Write("IsFinal", "true");
    cdbDef.CreateAbsolute("+Par4");
    cdbDef.Write("Class", "RealTimeGenericDataDef");
    cdbDef.Write("Type", "uint32");
    cdbDef.Write("Default", "2");
    cdbDef.Write("Path", "+DDB1.PidError2");
    cdbDef.Write("IsFinal", "true");
    cdbDef.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> def = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!def->Initialise(cdbDef)) {
        return false;
    }

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());
    return (!broker->AddVariable(def));

}

bool RealTimeDataSourceBrokerTest::TestGetData_Allocation() {

    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1","thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Outputs.+Control");
    ReferenceT<RealTimeGenericDataDef> def2 = gam->Find("+Outputs.+Noise");

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    if (broker->GetData(0) != NULL) {
        return false;
    }

    if (broker->GetData(1) != NULL) {
        return false;
    }

    if (!broker->AddVariable(def)) {
        return false;
    }

    if (!broker->AddVariable(def2)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetData(0) == NULL) {
        return false;
    }

    if (broker->GetData(1) == NULL) {
        return false;
    }
    return !broker->GetData(2);
}

bool RealTimeDataSourceBrokerTest::TestGetData_Static() {

    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1","thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ControlIn control;
    ControlNoise noise;

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Outputs.+Control");
    ReferenceT<RealTimeGenericDataDef> def2 = gam->Find("+Outputs.+Noise");

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    if (broker->GetData(0) != NULL) {
        return false;
    }

    if (broker->GetData(1) != NULL) {
        return false;
    }

    if (!broker->AddVariable(def, &control)) {
        return false;
    }

    if (!broker->AddVariable(def2, &noise)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetData(0) != &control) {
        return false;
    }

    if (broker->GetData(1) != &noise) {
        return false;
    }
    return !broker->GetData(2);

}

bool RealTimeDataSourceBrokerTest::TestGetMemoryPointer_Allocation() {

    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1","thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Outputs.+Control");
    ReferenceT<RealTimeGenericDataDef> def2 = gam->Find("+Outputs.+Noise");

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    if (!broker->AddVariable(def)) {
        return false;
    }

    if (!broker->AddVariable(def2)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetData(0) != broker->GetMemoryPointer(0)) {
        return false;
    }

    if (broker->GetData(1) != broker->GetMemoryPointer(2)) {
        return false;
    }
    return !broker->GetData(3);

}

bool RealTimeDataSourceBrokerTest::TestGetMemoryPointer_Static() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1","thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ControlIn control;
    ControlNoise noise;

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Outputs.+Control");
    ReferenceT<RealTimeGenericDataDef> def2 = gam->Find("+Outputs.+Noise");

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    if (!broker->AddVariable(def, &control)) {
        return false;
    }

    if (!broker->AddVariable(def2, &noise)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (&control.Par1 != broker->GetMemoryPointer(0)) {
        return false;
    }

    if (&control.Par2 != broker->GetMemoryPointer(1)) {
        return false;
    }

    if (&noise.noiseValue != broker->GetMemoryPointer(2)) {
        return false;
    }
    return !broker->GetData(3);

}

bool RealTimeDataSourceBrokerTest::TestFinalise() {

    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1","thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Outputs.+Control");
    ReferenceT<RealTimeGenericDataDef> def2 = gam->Find("+Outputs.+Noise");

    ReferenceT<RealTimeDataSourceBroker> broker = ReferenceT<RealTimeDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    if (!broker->AddVariable(def)) {
        return false;
    }

    if (!broker->AddVariable(def2)) {
        return false;
    }

    if (broker->GetData(0) != NULL) {
        return false;
    }

    if (broker->GetMemoryPointer(0) != NULL) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetData(0) == NULL) {
        return false;
    }

    if (broker->GetMemoryPointer(0) == NULL) {
        return false;
    }

    return true;
}

