/**
 * @file RealTimeStateTest.cpp
 * @brief Source file for class RealTimeStateTest
 * @date 07/mar/2016
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
 * the class RealTimeStateTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeStateTest.h"
#include "GAMTestHelper.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeStateTest::RealTimeStateTest() {
    /*ConfigurationDatabase cdb = GetCDB1();
     if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
     printf("\n!!!\n");
     }*/

}

RealTimeStateTest::~RealTimeStateTest() {
    ObjectRegistryDatabase::Instance()->CleanUp();
}

bool RealTimeStateTest::TestConstructor() {

    RealTimeState test;
    if (test.GetNumberOfGAMGroups() != 0) {
        return false;
    }

    if (test.GetGAMGroups() != NULL) {
        return false;
    }

    return true;
}

bool RealTimeStateTest::TestConfigureArchitecture() {

    ConfigurationDatabase cdb;
    StreamString conf = cdbStr1;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!state1->ConfigureArchitecture(*app.operator->())) {
        return false;
    }

    ReferenceT<PIDGAMGroup> group1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.PIDGroup1");

    if (state1->GetNumberOfGAMGroups() != 1) {
        return false;
    }
    if (state1->GetGAMGroups()[0] != group1) {
        return false;
    }
    return true;

}

bool RealTimeStateTest::TestConfigureArchitecture_SingleInGAMGroup() {
    ConfigurationDatabase cdb;
    StreamString conf = cdbStr1;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }

    ReferenceT<RealTimeState> state2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!state2->ConfigureArchitecture(*app.operator->())) {
        return false;
    }

    ReferenceT<PIDGAMGroup> group2 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.PIDGroup2");

    if (state2->GetNumberOfGAMGroups() != 1) {
        return false;
    }
    if (state2->GetGAMGroups()[0] != group2) {
        return false;
    }
    return true;

}

bool RealTimeStateTest::TestConfigureArchitectureFalse_NoThreads() {

    ConfigurationDatabase cdb;
    StreamString conf = ""
            "$Application1 = {\n" "\n"
            "    Class = RealTimeApplication\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threadss = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {:Functions.GAM1 :Functions.GAM2}\n"
            "                }\n"
            "                +Thread2 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {:Functions.PIDGroup1}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "}\n";
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1");
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    return (!state1->ConfigureArchitecture(*app.operator->()));
}

bool RealTimeStateTest::TestInsertFunction() {
    RealTimeState state;

    if (state.Size() != 0) {
        return false;
    }

    ReferenceT<PIDGAM> function1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    function1->SetName("function1");

    ReferenceT<PIDGAM> function2 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    function1->SetName("function2");

    if (!state.InsertFunction(function1)) {
        return false;
    }
    if (!state.InsertFunction(function2)) {
        return false;
    }

    if (state.Size() != 1) {
        return false;
    }

    ReferenceT<ReferenceContainer> functionsContainer = state.Get(0);
    if (StringHelper::Compare(functionsContainer->GetName(), "Functions") != 0) {
        return false;
    }

    return functionsContainer->Size() == 2;

}

bool RealTimeStateTest::TestAddGAMGroup() {
    RealTimeState state;

    const uint32 size = 32;
    ReferenceT<PIDGAMGroup> gamGroup[size];
    for (uint32 i = 0; i < size; i++) {
        gamGroup[i] = ReferenceT<PIDGAMGroup>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        state.AddGAMGroup(gamGroup[i]);
        // add it two times
        state.AddGAMGroup(gamGroup[i]);
    }

    if (state.GetNumberOfGAMGroups() != size) {
        return false;
    }

    ReferenceT<GAMGroup> *ret = state.GetGAMGroups();

    for (uint32 i = 0; i < size; i++) {
        if (ret[i] != gamGroup[i]) {
            return false;
        }
    }
    return true;
}

bool RealTimeStateTest::TestGetGAMGroups() {
    RealTimeState state;

    const uint32 size = 32;
    ReferenceT<PIDGAMGroup> gamGroup[size];
    for (uint32 i = 0; i < size; i++) {
        gamGroup[i] = ReferenceT<PIDGAMGroup>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        state.AddGAMGroup(gamGroup[i]);
    }

    ReferenceT<GAMGroup> *ret = state.GetGAMGroups();

    for (uint32 i = 0; i < size; i++) {
        if (ret[i] != gamGroup[i]) {
            return false;
        }
    }
    return true;
}

bool RealTimeStateTest::TestGetNumberOfGAMGroups() {
    RealTimeState state;

    const uint32 size = 32;
    ReferenceT<PIDGAMGroup> gamGroup[size];
    for (uint32 i = 0; i < size; i++) {
        gamGroup[i] = ReferenceT<PIDGAMGroup>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        state.AddGAMGroup(gamGroup[i]);
    }

    return (state.GetNumberOfGAMGroups() == size);
}

bool RealTimeStateTest::TestPrepareState() {
    RealTimeState state;

    ConfigurationDatabase empty;
    const uint32 size = 32;
    ReferenceT<PIDGAMGroup> gamGroup[size];
    for (uint32 i = 0; i < size; i++) {
        gamGroup[i] = ReferenceT<PIDGAMGroup>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        state.AddGAMGroup(gamGroup[i]);
        gamGroup[i]->Initialise(empty);
    }

    RealTimeStateInfo info;
    info.activeBuffer = 0;
    info.currentState = "state1";
    info.nextState = "state2";

    state.PrepareState(info);

    for (uint32 i = 0; i < size; i++) {
        if (gamGroup[i]->GetContext() != 2) {
            return false;
        }
    }

    info.activeBuffer = 1;
    info.currentState = "state2";
    info.nextState = "state1";

    // change again
    state.PrepareState(info);

    for (uint32 i = 0; i < size; i++) {
        if (gamGroup[i]->GetContext() != 1) {
            return false;
        }
    }
    return true;
}

bool RealTimeStateTest::TestValidateDataSourceLinks() {
    ConfigurationDatabase cdb;
    StreamString conf = ""
            "$Application1 = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAM1 = {\n"
            "            Class = DummyGAM\n"
            "            +Input = {\n"
            "                Class = GAMSignalsContainer\n"
            "                IsInput = true\n"
            "                IsFinal = true\n"
            "                +Counter = {\n"
            "                    Class = GAMGenericSignal\n"
            "                    Type = uint32\n"
            "                    Default = 0\n"
            "                    Path = DDB.Counter1\n"
            "                    IsFinal = true\n"
            "                }\n"
            "            }\n"
            "            +Output = {\n"
            "                Class = GAMSignalsContainer\n"
            "                IsOutput = true\n"
            "                IsFinal = true\n"
            "                +Counter = {\n"
            "                    Class = GAMGenericSignal\n"
            "                    Type = uint32\n"
            "                    Default = 0\n"
            "                    Path = DDB.Counter2\n"
            "                    IsFinal = true\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAM2 = {\n"
            "            Class = DummyGAM\n"
            "            +Input = {\n"
            "                Class = GAMSignalsContainer\n"
            "                IsInput = true\n"
            "                IsFinal = true\n"
            "                +Counter = {\n"
            "                    Class = GAMGenericSignal\n"
            "                    Type = uint32\n"
            "                    Default = 0\n"
            "                    Path = DDB.Counter2\n"
            "                    IsFinal = true\n"
            "                }\n"
            "            }\n"
            "            +Output = {\n"
            "                Class = GAMSignalsContainer\n"
            "                IsOutput = true\n"
            "                IsFinal = true\n"
            "                +Counter = {\n"
            "                    Class = GAMGenericSignal\n"
            "                    Type = uint32\n"
            "                    Default = 0\n"
            "                    Path = DDB.Counter1\n"
            "                    IsFinal = true\n"
            "                    Cycles = 1" // sync in thread 1
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAM3 = {\n"
            "            Class = DummyGAM\n"
            "            +Input = {\n"
            "                Class = GAMSignalsContainer\n"
            "                IsInput = true\n"
            "                IsFinal = true\n"
            "                +Counter = {\n"
            "                    Class = GAMGenericSignal\n"
            "                    Type = uint32\n"
            "                    Default = 0\n"
            "                    Path = DDB.Counter2\n"
            "                    IsFinal = true\n"
            "                    Cycles = 2" // sync in thread 2
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {:Functions.GAM1 :Functions.GAM2}\n"
            "                }\n"
            "                +Thread2 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {:Functions.GAM3 }\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = DataSourceContainer\n"
            "        +DDB = {\n"
            "            Class = DataSource\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = DummyScheduler\n"
            "    }\n"
            "}\n";
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    ReferenceT<RealTimeState> state = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1");

    if (!app->ConfigureArchitecture()) {
        return false;
    }

    if (!app->ConfigureDataSource()) {
        return false;
    }

    if (!app->ValidateDataSource()) {
        return false;
    }

    if (!app->AllocateDataSource()) {
        return false;
    }

    if (!app->ConfigureDataSourceLinks()) {
        return false;
    }

    bool ret = state->ValidateDataSourceLinks();
    ObjectRegistryDatabase::Instance()->CleanUp();
    return ret;
}



bool RealTimeStateTest::TestValidateDataSourceLinksFalse_MoreThanOneSyncPerThread(){
    ConfigurationDatabase cdb;
    StreamString conf = ""
            "$Application1 = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAM1 = {\n"
            "            Class = DummyGAM\n"
            "            +Input = {\n"
            "                Class = GAMSignalsContainer\n"
            "                IsInput = true\n"
            "                IsFinal = true\n"
            "                +Counter = {\n"
            "                    Class = GAMGenericSignal\n"
            "                    Type = uint32\n"
            "                    Default = 0\n"
            "                    Path = DDB.Counter1\n"
            "                    IsFinal = true\n"
            "                    Cycles = 2" // sync in thread 1
            "                }\n"
            "            }\n"
            "            +Output = {\n"
            "                Class = GAMSignalsContainer\n"
            "                IsOutput = true\n"
            "                IsFinal = true\n"
            "                +Counter = {\n"
            "                    Class = GAMGenericSignal\n"
            "                    Type = uint32\n"
            "                    Default = 0\n"
            "                    Path = DDB.Counter2\n"
            "                    IsFinal = true\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAM2 = {\n"
            "            Class = DummyGAM\n"
            "            +Input = {\n"
            "                Class = GAMSignalsContainer\n"
            "                IsInput = true\n"
            "                IsFinal = true\n"
            "                +Counter = {\n"
            "                    Class = GAMGenericSignal\n"
            "                    Type = uint32\n"
            "                    Default = 0\n"
            "                    Path = DDB.Counter2\n"
            "                    IsFinal = true\n"
            "                }\n"
            "            }\n"
            "            +Output = {\n"
            "                Class = GAMSignalsContainer\n"
            "                IsOutput = true\n"
            "                IsFinal = true\n"
            "                +Counter = {\n"
            "                    Class = GAMGenericSignal\n"
            "                    Type = uint32\n"
            "                    Default = 0\n"
            "                    Path = DDB.Counter1\n"
            "                    IsFinal = true\n"
            "                    Cycles = 1" // sync in thread 1
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAM3 = {\n"
            "            Class = DummyGAM\n"
            "            +Input = {\n"
            "                Class = GAMSignalsContainer\n"
            "                IsInput = true\n"
            "                IsFinal = true\n"
            "                +Counter = {\n"
            "                    Class = GAMGenericSignal\n"
            "                    Type = uint32\n"
            "                    Default = 0\n"
            "                    Path = DDB.Counter2\n"
            "                    IsFinal = true\n"
            "                    Cycles = 2" // sync in thread 2
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {:Functions.GAM1 :Functions.GAM2}\n"
            "                }\n"
            "                +Thread2 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {:Functions.GAM3 }\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = DataSourceContainer\n"
            "        +DDB = {\n"
            "            Class = DataSource\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = DummyScheduler\n"
            "    }\n"
            "}\n";
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    ReferenceT<RealTimeState> state = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1");

    if (!app->ConfigureArchitecture()) {
        return false;
    }

    if (!app->ConfigureDataSource()) {
        return false;
    }

    if (!app->ValidateDataSource()) {
        return false;
    }

    if (!app->AllocateDataSource()) {
        return false;
    }

    if (!app->ConfigureDataSourceLinks()) {
        return false;
    }

    bool ret = !state->ValidateDataSourceLinks();
    ObjectRegistryDatabase::Instance()->CleanUp();
    return ret;
}
