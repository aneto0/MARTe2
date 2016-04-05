/**
 * @file BasicGAMTest.cpp
 * @brief Source file for class BasicGAMTest
 * @date 22/mar/2016
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
 * the class BasicGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicGAMTest.h"
#include "GAMTestHelper.h"
#include "RealTimeDataDefContainer.h"
#include "BasicRealTimeDataSourceDef.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

BasicGAMTest::BasicGAMTest() {

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

BasicGAMTest::~BasicGAMTest() {
    ObjectRegistryDatabase::Instance()->CleanUp();
}

bool BasicGAMTest::TestConstructor() {
    PIDGAM gamTest;
    if (gamTest.GetNumberOfSupportedStates() != 0) {
        return false;
    }
    if (gamTest.GetSupportedStates() != NULL) {
        return false;
    }
    return true;
}

bool BasicGAMTest::TestInitialise() {
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

bool BasicGAMTest::TestSetApplication() {
    RealTimeApplication rtApp;
    PIDGAM gam;
    gam.SetApplication(rtApp);
    return true;
}

bool BasicGAMTest::TestSetGAMGroup() {
    ReferenceT<PIDGAMGroup> rtgg;
    PIDGAM gam;
    gam.SetGAMGroup(rtgg);
    return true;
}

bool BasicGAMTest::TestAddState() {

    PIDGAM gam;
    const uint32 size = 5;
    StreamString states[size] = { "state1", "state2", "state3", "state4", "state5" };
    StreamString threads[size] = { "thread1", "thread1", "thread1", "thread1", "thread1" };
    for (uint32 i = 0u; i < size; i++) {
        if (!gam.AddState(states[i].Buffer(), threads[i].Buffer())) {
            return false;
        }
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

bool BasicGAMTest::TestAddStateFalse_MoreThreadsPerGAM() {
    PIDGAM gam;
    if (!gam.AddState("state1", "thread1")) {
        return false;
    }
    if (!gam.AddState("state1", "thread1")) {
        return false;
    }
    return (!gam.AddState("state1", "thread2"));
}

bool BasicGAMTest::TestGetNumberOfSupportedStates() {
    return TestAddState();
}

bool BasicGAMTest::TestGetSupportedStates() {
    return TestAddState();
}

bool BasicGAMTest::TestGetSupportedThreads() {
    PIDGAM gam;
    const uint32 size = 5;
    StreamString states[size] = { "state1", "state2", "state3", "state4", "state5" };
    StreamString threads[size] = { "thread1", "thread2", "thread3", "thread4", "thread5" };
    for (uint32 i = 0u; i < size; i++) {
        if (!gam.AddState(states[i].Buffer(), threads[i].Buffer())) {
            return false;
        }
    }

    if (gam.GetNumberOfSupportedStates() != size) {
        return false;
    }

    StreamString *retThreads = gam.GetSupportedThreads();
    for (uint32 i = 0u; i < size; i++) {
        if (retThreads[i] != threads[i]) {
            return false;
        }
    }
    return true;
}

bool BasicGAMTest::TestGetSupportedThreads_GAMGroup() {

    ReferenceT<PIDGAMGroup> gamGroup = ReferenceT<PIDGAMGroup>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    const uint32 size = 4;
    StreamString states[size] = { "state1", "state2", "state3", "state4" };
    StreamString threads[size] = { "thread1", "thread2", "thread3", "thread4" };

    for (uint32 i = 0u; i < size; i++) {
        gamGroup->AddState(states[i].Buffer(), threads[i].Buffer());
    }

    gam->SetGAMGroup(gamGroup);

    StreamString *retThreads = gam->GetSupportedThreads();
    for (uint32 i = 0u; i < size; i++) {
        if (retThreads[i] != threads[i]) {
            return false;
        }
    }
    return true;

}

bool BasicGAMTest::TestGetNumberOfSupportedStates_GAMGroup() {

    ReferenceT<PIDGAMGroup> gamGroup = ReferenceT<PIDGAMGroup>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    const uint32 size = 4;
    StreamString states[size] = { "state1", "state2", "state3", "state4" };
    for (uint32 i = 0u; i < size; i++) {
        gamGroup->AddState(states[i].Buffer(), "thread1");
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

bool BasicGAMTest::TestGetSupportedStates_GAMGroup() {
    ReferenceT<PIDGAMGroup> gamGroup = ReferenceT<PIDGAMGroup>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    const uint32 size = 4;
    StreamString states[4] = { "state1", "state2", "state3", "state4" };
    for (uint32 i = 0u; i < size; i++) {
        gamGroup->AddState(states[i].Buffer(), "thread1");
    }

    gam->SetGAMGroup(gamGroup);

    return (gam->GetNumberOfSupportedStates() == size);
}

bool BasicGAMTest::TestConfigureDataSource() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("PIDD");
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    gam->AddState("state2", "thread1");

    if (!gam->ConfigureDataSource()) {
        return false;
    }
    ReferenceT<BasicRealTimeDataSourceDef> def1 = rtapp->Find("+Data.+DDB1.PidError1");
    if (def1->GetNumberOfConsumers("state1") != 1 || def1->GetNumberOfConsumers("state2") != 1) {

    }

    ReferenceT<BasicRealTimeDataSourceDef> def2 = rtapp->Find("+Data.+DDB1.PidError2");
    if (def2->GetNumberOfConsumers("state1") != 1 || def2->GetNumberOfConsumers("state2") != 1) {

    }

    ReferenceT<BasicRealTimeDataSourceDef> def3 = rtapp->Find("+Data.+DDB2.PidControl1");
    if (def3->GetNumberOfProducers("state1") != 1 || def3->GetNumberOfProducers("state2") != 1) {

    }
    ReferenceT<BasicRealTimeDataSourceDef> def4 = rtapp->Find("+Data.+DDB2.PidControl2");
    if (def4->GetNumberOfProducers("state1") != 1 || def4->GetNumberOfProducers("state2") != 1) {

    }

    return true;
}

bool BasicGAMTest::TestConfigureDataSourceFalse_NoData() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Datas"); // wrong
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Datas.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Datas.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    gam->AddState("state2", "thread1");

    return (!gam->ConfigureDataSource());
}

bool BasicGAMTest::TestConfigureDataSourceFalse_NoApplicationSet() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam->Initialise(cdb)) {
        return false;
    }

    //gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    gam->AddState("state2", "thread1");

    return (!gam->ConfigureDataSource());
}

bool BasicGAMTest::TestConfigureDataSource_NoStates() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());

    return (gam->ConfigureDataSource());
}

bool BasicGAMTest::TestConfigureDataSourceFalse_Final() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB3");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    gam->AddState("state2", "thread1");

    return (!gam->ConfigureDataSource());
}

bool BasicGAMTest::TestConfigureDataSourceLinks() {
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

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1", "thread1");

    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "+state1";
    rtapp->PrepareNextState(nextState);

    if (!gam->ConfigureDataSourceLinks()) {
        return false;
    }

    return true;
}

bool BasicGAMTest::TestIsSync() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB1.+Driver");
    appCDB.Write("Class", "SharedDataSource");
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

    ConfigurationDatabase cdb;
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

    cdb.CreateAbsolute("+Inputs.+Sync");
    cdb.Write("Class", "RealTimeGenericDataDef");

    cdb.Write("Type", "uint32");
    cdb.Write("IsFinal", "true");
    cdb.Write("Default", "2");
    cdb.Write("Path", "+DDB1.+Driver");

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

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");

    if (gam->IsSync()) {
        return false;
    }

    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1", "thread1");

    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "+state1";
    rtapp->PrepareNextState(nextState);

    if (!gam->ConfigureDataSourceLinks()) {
        return false;
    }

    return gam->IsSync();
}

bool BasicGAMTest::TestExecute() {
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
    gam->AddState("+state1", "thread1");

    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "+state1";
    rtapp->PrepareNextState(nextState);

    if (!gam->ConfigureDataSourceLinks()) {
        return false;
    }

    gam->Execute(0);

    return true;
}
