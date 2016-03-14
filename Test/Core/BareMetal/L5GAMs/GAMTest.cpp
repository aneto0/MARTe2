/**
 * @file GAMTest.cpp
 * @brief Source file for class GAMTest
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
 * the class GAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMTest.h"
#include "GAMTestHelper.h"
#include "RealTimeDataDefContainer.h"
#include "RealTimeDataSourceDef.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMTest::GAMTest() {

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

GAMTest::~GAMTest() {
    ObjectRegistryDatabase::Instance()->CleanUp();
}

bool GAMTest::TestConstructor() {
    PIDGAM gamTest;
    if (gamTest.GetNumberOfSupportedStates() != 0) {
        return false;
    }
    if (gamTest.GetSupportedStates() != NULL) {
        return false;
    }
    return true;
}

bool GAMTest::TestInitialise() {
    PIDGAM gamTest;
    if (!gamTest.Initialise(cdb)) {
        return false;
    }

    // there must be two containers
    if (gamTest.Size() != 2) {
        return false;
    }

    // check the input
    ReferenceT<RealTimeDataDefContainer> inputs = gamTest.Get(0);
    if (inputs->Size() != 1) {
        return false;
    }

    // check the output
    ReferenceT<RealTimeDataDefContainer> outputs = gamTest.Get(1);
    if (outputs->Size() != 2) {
        return false;
    }

    return true;
}

bool GAMTest::TestSetApplication() {
    ReferenceT<RealTimeApplication> rtApp;
    PIDGAM gam;
    gam.SetApplication(rtApp);
    return true;
}

bool GAMTest::TestSetGAMGroup() {
    ReferenceT<PIDGAMGroup> rtgg;
    PIDGAM gam;
    gam.SetGAMGroup(rtgg);
    return true;
}

bool GAMTest::TestAddState() {

    PIDGAM gam;
    const uint32 size = 4;
    StreamString states[4] = { "state1", "state2", "state3", "state4" };
    for (uint32 i = 0u; i < size; i++) {
        gam.AddState(states[i].Buffer());
    }

    if (gam.GetNumberOfSupportedStates() != size) {
        return false;
    }

    StreamString *retStates = gam.GetSupportedStates();
    for (uint32 i = 0u; i < size; i++) {
        if (retStates[i] != states[i]) {
            return false;
        }
    }
    return true;
}

bool GAMTest::TestGetNumberOfSupportedStates() {
    return TestAddState();
}

bool GAMTest::TestGetSupportedStates() {
    return TestAddState();
}

bool GAMTest::TestGetNumberOfSupportedStates_GAMGroup() {

    ReferenceT<PIDGAMGroup> gamGroup = ReferenceT<PIDGAMGroup>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    const uint32 size = 4;
    StreamString states[4] = { "state1", "state2", "state3", "state4" };
    for (uint32 i = 0u; i < size; i++) {
        gamGroup->AddState(states[i].Buffer());
    }

    gam->SetGAMGroup(gamGroup);

    StreamString *retStates = gam->GetSupportedStates();
    for (uint32 i = 0u; i < size; i++) {
        if (retStates[i] != states[i]) {
            return false;
        }
    }
    return true;

}

bool GAMTest::TestGetSupportedStates_GAMGroup() {
    ReferenceT<PIDGAMGroup> gamGroup = ReferenceT<PIDGAMGroup>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    const uint32 size = 4;
    StreamString states[4] = { "state1", "state2", "state3", "state4" };
    for (uint32 i = 0u; i < size; i++) {
        gamGroup->AddState(states[i].Buffer());
    }

    gam->SetGAMGroup(gamGroup);

    return (gam->GetNumberOfSupportedStates() == size);
}

bool GAMTest::TestConfigureDataSource() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(rtapp);
    gam->AddState("state1");
    gam->AddState("state2");

    if (!gam->ConfigureDataSource()) {
        return false;
    }
    ReferenceT<RealTimeDataSourceDef> def1 = rtapp->Find("+Data.+DDB1.PidError1");
    if (def1->GetNumberOfConsumers("state1") != 1 || def1->GetNumberOfConsumers("state2") != 1) {

    }

    ReferenceT<RealTimeDataSourceDef> def2 = rtapp->Find("+Data.+DDB1.PidError2");
    if (def2->GetNumberOfConsumers("state1") != 1 || def2->GetNumberOfConsumers("state2") != 1) {

    }

    ReferenceT<RealTimeDataSourceDef> def3 = rtapp->Find("+Data.+DDB2.PidControl1");
    if (def3->GetNumberOfProducers("state1") != 1 || def3->GetNumberOfProducers("state2") != 1) {

    }
    ReferenceT<RealTimeDataSourceDef> def4 = rtapp->Find("+Data.+DDB2.PidControl2");
    if (def4->GetNumberOfProducers("state1") != 1 || def4->GetNumberOfProducers("state2") != 1) {

    }

    return true;
}

bool GAMTest::TestConfigureDataSource_NotFinal() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.Write("IsFinal", "false");

    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(rtapp);
    gam->AddState("state1");
    gam->AddState("state2");

    if (!gam->ConfigureDataSource()) {
        return false;
    }
    ReferenceT<RealTimeDataSourceDef> def1 = rtapp->Find("+Data.+DDB1.PidError1");
    if (def1->GetNumberOfConsumers("state1") != 1 || def1->GetNumberOfConsumers("state2") != 1) {

    }

    ReferenceT<RealTimeDataSourceDef> def2 = rtapp->Find("+Data.+DDB1.PidError2");
    if (def2->GetNumberOfConsumers("state1") != 1 || def2->GetNumberOfConsumers("state2") != 1) {

    }

    ReferenceT<RealTimeDataSourceDef> def3 = rtapp->Find("+Data.+DDB2.PidControl1");
    if (def3->GetNumberOfProducers("state1") != 1 || def3->GetNumberOfProducers("state2") != 1) {

    }
    ReferenceT<RealTimeDataSourceDef> def4 = rtapp->Find("+Data.+DDB2.PidControl2");
    if (def4->GetNumberOfProducers("state1") != 1 || def4->GetNumberOfProducers("state2") != 1) {

    }

    return true;
}

bool GAMTest::TestConfigureDataSourceFalse_Final() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+Data.+DDB3");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(rtapp);
    gam->AddState("state1");
    gam->AddState("state2");

    return (!gam->ConfigureDataSource());
}

bool GAMTest::TestConfigureDataSourceLinks() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "ReferenceContainer");
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

    gam->SetApplication(rtapp);
    gam->AddState("+state1");

    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    RealTimeStateInfo info;
    info.currentState = "";
    info.nextState = "+state1";
    info.activeBuffer = 1;
    if (!rtapp->PrepareNextState(info)) {
        return false;
    }

    if (!gam->ConfigureDataSourceLinks()) {
        return false;
    }

    return true;
}

bool GAMTest::TestExecute() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "ReferenceContainer");
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

    gam->SetApplication(rtapp);
    gam->AddState("+state1");

    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    RealTimeStateInfo info;
    info.currentState = "";
    info.nextState = "+state1";
    info.activeBuffer = 1;
    if (!rtapp->PrepareNextState(info)) {
        return false;
    }

    if (!gam->ConfigureDataSourceLinks()) {
        return false;
    }

    gam->Execute(0);

    return true;
}

