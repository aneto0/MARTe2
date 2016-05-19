/**
 * @file GAMTest.cpp
 * @brief Source file for class GAMTest
 * @date 25/04/2016
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

#include <GAMDataSource.h>

#include "GAMTestHelper.h"
#include "GAMSignalsContainer.h"
#include "GAMGenericSignal.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMTest::GAMTest() {
    StreamString conf = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = TrackError\n"
            "        IsFinal = false\n"
            "        +Par2 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Default = 2\n"
            "            Path = DDB1.PidError2\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+Outputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsOutput = true\n"
            "    IsFinal = false\n"
            "    +Control = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = ControlIn\n"
            "        IsFinal = false\n"
            "        +Par2 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Path = DDB2.PidControl2\n"
            "            Default = 2\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "    +Noise = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = ControlNoise\n"
            "        IsFinal = true\n"
            "        +noiseValue = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = float32\n"
            "            Default = 2\n"
            "            Path = DDB2.PidNoise\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n";

    conf.Seek(0);
    StandardParser parser(conf, cdb);
    parser.Parse();
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

    return (gamTest.GetSupportedThreads() == NULL);
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
    ReferenceT<GAMSignalsContainer> inputs = gamTest.Get(0);
    if (inputs->Size() != 1) {
        return false;
    }

    // check the output
    ReferenceT<GAMSignalsContainer> outputs = gamTest.Get(1);
    if (outputs->Size() != 2) {
        return false;
    }

    return true;
}

bool GAMTest::TestSetApplication() {
    RealTimeApplication rtApp;
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

bool GAMTest::TestAddStateFalse_MoreThreadsPerGAM() {
    PIDGAM gam;
    if (!gam.AddState("state1", "thread1")) {
        return false;
    }
    if (!gam.AddState("state1", "thread1")) {
        return false;
    }
    return (!gam.AddState("state1", "thread2"));
}

bool GAMTest::TestGetNumberOfSupportedStates() {
    return TestAddState();
}

bool GAMTest::TestGetSupportedStates() {
    return TestAddState();
}

bool GAMTest::TestGetSupportedThreads() {
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

bool GAMTest::TestConfigureDataSource() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "DataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "DataSource");
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
    ReferenceT<GAMDataSource> def1 = rtapp->Find("Data.DDB1.PidError1");
    if (def1->GetNumberOfConsumers("state1") != 1 || def1->GetNumberOfConsumers("state2") != 1) {

    }

    ReferenceT<GAMDataSource> def2 = rtapp->Find("Data.DDB1.PidError2");
    if (def2->GetNumberOfConsumers("state1") != 1 || def2->GetNumberOfConsumers("state2") != 1) {

    }

    ReferenceT<GAMDataSource> def3 = rtapp->Find("Data.DDB2.PidControl1");
    if (def3->GetNumberOfProducers("state1") != 1 || def3->GetNumberOfProducers("state2") != 1) {

    }
    ReferenceT<GAMDataSource> def4 = rtapp->Find("Data.DDB2.PidControl2");
    if (def4->GetNumberOfProducers("state1") != 1 || def4->GetNumberOfProducers("state2") != 1) {

    }

    return true;
}

bool GAMTest::TestConfigureDataSourceFalse_NoDataSource() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "DataSource");
    appCDB.CreateAbsolute("+Data.+DDB3");
    appCDB.Write("Class", "DataSource");
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

bool GAMTest::TestConfigureDataSourceFalse_NoData() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Datas"); // wrong
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Datas.+DDB1");
    appCDB.Write("Class", "DataSource");
    appCDB.CreateAbsolute("+Datas.+DDB2");
    appCDB.Write("Class", "DataSource");
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

bool GAMTest::TestConfigureDataSourceFalse_NoApplicationSet() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "DataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "DataSource");
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

bool GAMTest::TestConfigureDataSource_NoStates() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "DataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "DataSource");
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

bool GAMTest::TestConfigureDataSourceLinks() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "DataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "DataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    StreamString conf = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = TrackError\n"
            "        IsFinal = false\n"
            "        +Par2 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Default = 2\n"
            "            Path = DDB1.PidError2\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+Outputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsOutput = true\n"
            "    IsFinal = false\n"
            "    +Control = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = ControlIn\n"
            "        IsFinal = false\n"
            "        +Par2 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Path = DDB2.PidControl2\n"
            "            Default = 2\n"
            "            IsFinal = true\n"
            "            Cycles = 1\n" // the only one sync
            "        }\n"
            "    }\n"
            "    +Noise = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = ControlNoise\n"
            "        IsFinal = true\n"
            "        +noiseValue = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = float32\n"
            "            Default = 2\n"
            "            Path = DDB2.PidNoise\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n";
    conf.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }

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
    gam->AddState("state1", "thread1");

    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "state1";
    rtapp->PrepareNextState(nextState);

    if (!gam->ConfigureDataSourceLinks()) {
        return false;
    }

    return true;
}

bool GAMTest::TestConfigureDataSourceLinksFalse_MoreThanOneSyncSignal() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "DataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "DataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    StreamString conf = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = TrackError\n"
            "        IsFinal = false\n"
            "        +Par2 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Default = 2\n"
            "            Path = DDB1.PidError2\n"
            "            IsFinal = true\n"
            "            Cycles = 1\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+Outputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsOutput = true\n"
            "    IsFinal = false\n"
            "    +Control = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = ControlIn\n"
            "        IsFinal = false\n"
            "        +Par2 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Path = DDB2.PidControl2\n"
            "            Default = 2\n"
            "            IsFinal = true\n"
            "            Cycles = 1\n"
            "        }\n"
            "    }\n"
            "    +Noise = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = ControlNoise\n"
            "        IsFinal = true\n"
            "        +noiseValue = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = float32\n"
            "            Default = 2\n"
            "            Path = DDB2.PidNoise\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n";
    conf.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }

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
    gam->AddState("state1", "thread1");

    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "state1";
    rtapp->PrepareNextState(nextState);

    return (!gam->ConfigureDataSourceLinks());
}

bool GAMTest::TestIsSync() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "DataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "DataSource");
    appCDB.CreateAbsolute("+States");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+States.+state1");
    appCDB.Write("Class", "RealTimeState");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }
    StreamString conf = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = TrackError\n"
            "        IsFinal = false\n"
            "        +Par2 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Default = 2\n"
            "            Path = DDB1.PidError2\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+Outputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsOutput = true\n"
            "    IsFinal = false\n"
            "    +Control = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = ControlIn\n"
            "        IsFinal = false\n"
            "        +Par2 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Path = DDB2.PidControl2\n"
            "            Default = 2\n"
            "            IsFinal = true\n"
            "            Cycles = 1"  // declared as sync here!
            "        }\n"
            "    }\n"
            "    +Noise = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = ControlNoise\n"
            "        IsFinal = true\n"
            "        +noiseValue = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = float32\n"
            "            Default = 2\n"
            "            Path = DDB2.PidNoise\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n";
    ConfigurationDatabase cdb;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    parser.Parse();

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");

    if (gam->IsSync()) {
        return false;
    }

    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");

    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "state1";
    rtapp->PrepareNextState(nextState);

    if (!gam->ConfigureDataSourceLinks()) {
        return false;
    }

    return gam->IsSync();
}

bool GAMTest::TestGetInputReader() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "DataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "DataSource");
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
    gam->AddState("state1", "thread1");

    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "state1";
    rtapp->PrepareNextState(nextState);

    if (!gam->ConfigureDataSourceLinks()) {
        return false;
    }

    ReferenceT<BrokerContainer> reader = gam->GetInputReader();
    if (!reader.IsValid()) {
        return false;
    }
    reader->Read(0);
    TrackError *var1 = (TrackError *) reader->GetSignal(0);
    printf("\nread %d\n", var1->Par2);

    return (var1->Par2 == 2);
}

bool GAMTest::TestGetOutputWriter() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "DataSource");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "DataSource");
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
    gam->AddState("state1", "thread1");

    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "state1";
    rtapp->PrepareNextState(nextState);

    if (!gam->ConfigureDataSourceLinks()) {
        return false;
    }

    ReferenceT<BrokerContainer> writer = gam->GetOutputWriter();
    if (!writer.IsValid()) {
        return false;
    }

    ControlIn *var1 = (ControlIn *) writer->GetSignal(0);
    var1->Par2 = 10;
    writer->Write(0);

    ConfigurationDatabase defCDB;
    defCDB.Write("Class", "GAMGenericSignal");
    defCDB.Write("Type", "uint32");
    defCDB.Write("Path", "DDB2.PidControl2");
    defCDB.Write("Default", "1");
    defCDB.Write("IsFinal", "true");
    defCDB.MoveToRoot();

    ReferenceT<GAMGenericSignal> def = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!def->Initialise(defCDB)) {
        return false;
    }

    MemoryMapInputReader reader;
    reader.SetApplication(*rtapp.operator ->());
    if (!reader.AddSignal(def)) {
        return false;
    }
    if (!reader.Finalise()) {
        return false;
    }
    reader.Read(0);
    uint32 *readVar = (uint32 *) reader.GetSignal(0);

    printf("\nread %d\n", *readVar);
    return (*readVar == 10);
}
