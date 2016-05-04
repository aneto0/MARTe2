/**
 * @file RealTimeApplicationTest.cpp
 * @brief Source file for class RealTimeApplicationTest
 * @date 26/feb/2016
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
 * the class RealTimeApplicationTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeApplicationTest.h"
#include "RealTimeState.h"
#include "RealTimeThread.h"
#include "ObjectRegistryDatabase.h"
#include "DataSource.h"
#include "GAMTestHelper.h"
#include "DataSourceSignal.h"
#include "GAMGenericSignal.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeApplicationTest::RealTimeApplicationTest() {

}

RealTimeApplicationTest::~RealTimeApplicationTest() {
    ObjectRegistryDatabase::Instance()->CleanUp();
}

bool RealTimeApplicationTest::TestConstructor() {
    return true;
}

bool RealTimeApplicationTest::TestInitialise() {
    ConfigurationDatabase cdb;
    StreamString conf = cdbStr1;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    cdb.MoveAbsolute("$Application1");
    RealTimeApplication test;
    return test.Initialise(cdb);
}

bool RealTimeApplicationTest::TestConfigureArchitecture() {
    ConfigurationDatabase cdb;
    StreamString conf = cdbStr1;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }

    if (!application->ConfigureArchitecture()) {
        return false;
    }

    // check if the state 1 is consistent with definitions
    ReferenceT<RealTimeState> state1 = god->Find("Application1.States.State1");
    if (!state1.IsValid()) {
        return false;
    }

    if (state1->GetNumberOfGAMGroups() != 1) {
        return false;
    }

    ReferenceT<PIDGAMGroup> gamGroup1 = god->Find("Application1.Functions.PIDGroup1");
    if (state1->GetGAMGroups()[0] != gamGroup1) {
        return false;
    }

    // check the thread 1
    ReferenceT<RealTimeThread> thread1 = god->Find("Application1.States.State1.Threads.Thread1");
    if (!thread1.IsValid()) {
        return false;
    }

    if (thread1->GetNumberOfGAMs() != 2) {
        printf("\n%d\n", thread1->GetNumberOfGAMs());
        return false;
    }

    ReferenceT<GAM> *gams1 = thread1->GetGAMs();
    ReferenceT<GAM> test1 = god->Find("Application1.Functions.GAM1");
    ReferenceT<GAM> test2 = god->Find("Application1.Functions.GAM2");

    if (gams1[0] != test1) {
        return false;
    }
    if (gams1[1] != test2) {
        return false;
    }

    // check the thread 2
    ReferenceT<RealTimeThread> thread2 = god->Find("Application1.States.State1.Threads.Thread2");
    if (!thread2.IsValid()) {
        return false;
    }

    if (thread2->GetNumberOfFunctions() != 1) {
        printf("\n%d\n", thread2->GetNumberOfFunctions());
        return false;
    }

    if (thread2->GetNumberOfGAMs() != 2) {
        printf("\n%d\n", thread2->GetNumberOfGAMs());
        return false;
    }

    ReferenceT<GAM> *gams2 = thread2->GetGAMs();
    ReferenceT<GAM> test3 = god->Find("Application1.Functions.PIDGroup1.GAM3");
    ReferenceT<GAM> test4 = god->Find("Application1.Functions.PIDGroup1.GAM4");

    if (gams2[0] != test3) {
        return false;
    }
    if (gams2[1] != test4) {
        return false;
    }

    // check the state 2
    ReferenceT<RealTimeState> state2 = god->Find("Application1.States.State2");
    if (!state2.IsValid()) {
        return false;
    }

    if (state2->GetNumberOfGAMGroups() != 1) {
        return false;
    }

    ReferenceT<PIDGAMGroup> gamGroup2 = god->Find("Application1.Functions.PIDGroup2");
    if (state2->GetGAMGroups()[0] != gamGroup2) {
        return false;
    }

    // check the thread 3

    ReferenceT<RealTimeThread> thread3 = god->Find("Application1.States.State2.Threads.Thread1");
    if (!thread3.IsValid()) {
        return false;
    }

    if (thread3->GetNumberOfFunctions() != 1) {
        printf("\n%d\n", thread2->GetNumberOfFunctions());
        return false;
    }

    if (thread3->GetNumberOfGAMs() != 2) {
        printf("\n%d\n", thread2->GetNumberOfGAMs());
        return false;
    }

    ReferenceT<GAM> *gams3 = thread3->GetGAMs();
    ReferenceT<GAM> test5 = god->Find("Application1.Functions.GAMContainer.GAM5");
    ReferenceT<GAM> test6 = god->Find("Application1.Functions.GAMContainer.GAM6");

    if (gams3[0] != test5) {
        return false;
    }
    if (gams3[1] != test6) {
        return false;
    }

    // check the thread 4
    ReferenceT<RealTimeThread> thread4 = god->Find("Application1.States.State2.Threads.Thread2");

    if (thread4->GetNumberOfFunctions() != 2) {
        printf("\n%d\n", thread2->GetNumberOfFunctions());
        return false;
    }

    if (thread4->GetNumberOfGAMs() != 2) {
        printf("\n%d\n", thread2->GetNumberOfGAMs());
        return false;
    }

    ReferenceT<GAM> *gams4 = thread4->GetGAMs();
    ReferenceT<GAM> test7 = god->Find("Application1.Functions.PIDGroup2.GAM7");
    ReferenceT<GAM> test8 = god->Find("Application1.Functions.PIDGroup2.GAM8");

    if (gams4[0] != test7) {
        return false;
    }
    if (gams4[1] != test8) {
        return false;
    }

    StreamString output;
    ConfigurationDatabase outputCDB;
    application->ToStructuredData(outputCDB);
    output.Printf("%!", outputCDB);
    printf("\n%s\n", output.Buffer());

    return true;
}

bool RealTimeApplicationTest::TestConfigureArchitectureFalse_NoStates() {
    ConfigurationDatabase cdb;

    StreamString conf = "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = PIDGAM"
            "        }"
            "        +GAM2 = {"
            "            Class = PIDGAM"
            "        }"
            "        +PIDGroup1 = {"
            "            Class = PIDGAMGroup"
            "            +GAM3 = {"
            "                Class = PIDGAM"
            "            }"
            "            +GAM4 = {"
            "                Class = PIDGAM"
            "            }"
            "        }"
            "    }"
            "    +Statess = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.GAM1 :Functions.GAM2 }"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.PIDGroup1 }"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = DataSourceContainer"
            "        +DDB1 = {"
            "            Class = DataSource"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = DummyScheduler"
            "    }"
            "}";

    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (!application.IsValid()) {
        return false;
    }

    return (!application->ConfigureArchitecture());

}

bool RealTimeApplicationTest::TestConfigureDataSource() {
    ConfigurationDatabase cdb;
    StreamString conf = "\n"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = PIDGAM"
            "        }"
            "        +GAM2 = {"
            "            Class = PIDGAM"
            "        }"
            "        +PIDGroup1 = {"
            "            Class = PIDGAMGroup"
            "            +GAM3 = {"
            "                Class = PIDGAM"
            "            }"
            "            +GAM4 = {"
            "                Class = PIDGAM"
            "            }"
            "        }"
            "        +GAMContainer = {"
            "            Class = ReferenceContainer"
            "            +GAM5 = {"
            "                Class = PIDGAM"
            "                +Inputs = {"
            "                    Class = GAMSignalsContainer"
            "                    IsInput = true"
            "                    IsFinal = false"
            "                    +Error = {"
            "                        Class = GAMGenericSignal"
            "                        Type = TrackError"
            "                        IsFinal = false"
            "                        +Par2 = {"
            "                            Class = GAMGenericSignal"
            "                                Type = uint32"
            "                                Default = 2"
            "                                Path = DDB1.PidError2"
            "                                IsFinal = true"
            "                            }"
            "                        }"
            "                }"
            "                +Outputs = {"
            "                    Class = GAMSignalsContainer"
            "                    IsOutput = true"
            "                    IsFinal = false"
            "                    +Control = {"
            "                        Class = GAMGenericSignal"
            "                        Type = ControlIn"
            "                        IsFinal = false"
            "                        +Par2 = {"
            "                            Class = GAMGenericSignal"
            "                            Type = uint32"
            "                            Path = DDB2.PidControl2"
            "                            Default = 2"
            "                            IsFinal = true"
            "                        }"
            "                    }"
            "                    +Noise = {"
            "                        Class = GAMGenericSignal"
            "                        Type = ControlNoise"
            "                        IsFinal = true"
            "                        +noiseValue = {"
            "                            Class = GAMGenericSignal"
            "                            Type = float32"
            "                            Default = 2"
            "                            Path = DDB2.PidNoise"
            "                            IsFinal = true"
            "                        }"
            "                    }"
            "                }"
            "            }"
            "            +GAM6 = {"
            "                Class = PIDGAM"
            "            }"
            "        }"
            "        +PIDGroup2 = {"
            "            Class = PIDGAMGroup"
            "            +GAM7 = {"
            "                Class = PIDGAM"
            "            }"
            "            +GAM8 = {"
            "                Class = PlantGAM"
            "                +Inputs = {"
            "                    Class = GAMSignalsContainer"
            "                    IsInput = true"
            "                    IsFinal = false"
            "                    +Control = {"
            "                        Class = GAMGenericSignal"
            "                        Type = TrackError"
            "                        IsFinal = true"
            "                        +Par1 = {"
            "                            Class = GAMGenericSignal"
            "                            Type = uint32"
            "                            Default = 1"
            "                            Path = DDB2.PidControl1"
            "                            IsFinal = true"
            "                        }"
            "                        +Par2 = {"
            "                            Class = GAMGenericSignal"
            "                            Type = uint32"
            "                            Default = 2"
            "                            Path = DDB2.PidControl2"
            "                            IsFinal = true"
            "                        }"
            "                    }"
            "                }"
            "                +Outputs = {"
            "                    Class = GAMSignalsContainer"
            "                    IsOutput = true"
            "                    IsFinal = true"
            "                    +Error = {"
            "                        Class = GAMGenericSignal"
            "                        Type = TrackError"
            "                        IsFinal = false"
            "                        +Par1 = {"
            "                            Class = GAMGenericSignal"
            "                            Type = uint32"
            "                            Default = 1"
            "                            Path = DDB1.PidError1"
            "                            IsFinal = true"
            "                        }"
            "                        +Par2 = {"
            "                            Class = GAMGenericSignal"
            "                            Type = uint32"
            "                            Default = 2"
            "                            Path = DDB1.PidError2"
            "                            IsFinal = true"
            "                        }"
            "                    }"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.GAM1 :Functions.PIDGroup2.GAM8 }"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.PIDGroup1  }"
            "                }"
            "            }"
            "        }"
            "        +State2 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.GAMContainer }"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.PIDGroup2.GAM7 :Functions.GAM2 }"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = DataSourceContainer"
            "        +DDB1 = {"
            "            Class = DataSource"
            "        }"
            "        +DDB2 = {"
            "            Class = DataSource"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = DummyScheduler"
            "    }"
            "}";
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!app->ConfigureArchitecture()) {
        return false;
    }
    if (!app->ConfigureDataSource()) {
        return false;
    }

    // each definition must have one producer and one consumer
    ReferenceT<DataSourceSignal> def1 = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1.PidError1");
    if (def1->GetNumberOfConsumers("State1") != 0 || def1->GetNumberOfProducers("State1") != 1) {
        printf("\n1 %d %d\n", def1->GetNumberOfConsumers("State1"), def1->GetNumberOfProducers("State1"));
        return false;
    }

    if (def1->GetNumberOfConsumers("State2") != 1 || def1->GetNumberOfProducers("State2") != 0) {
        return false;
    }

    // each definition must have one producer and one consumer
    ReferenceT<DataSourceSignal> def2 = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1.PidError2");
    if (def2->GetNumberOfConsumers("State1") != 0 || def2->GetNumberOfProducers("State1") != 1) {
        printf("\n2 %d %d\n", def2->GetNumberOfConsumers("State1"), def2->GetNumberOfProducers("State1"));
        return false;
    }

    if (def2->GetNumberOfConsumers("State2") != 1 || def2->GetNumberOfProducers("State2") != 0) {
        return false;
    }

    // each definition must have one producer and one consumer
    ReferenceT<DataSourceSignal> def3 = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB2.PidControl1");
    if (def3->GetNumberOfConsumers("State1") != 1 || def3->GetNumberOfProducers("State1") != 0) {
        printf("\n3 %d %d\n", def3->GetNumberOfConsumers("State1"), def3->GetNumberOfProducers("State1"));
        return false;
    }

    if (def3->GetNumberOfConsumers("State2") != 0 || def3->GetNumberOfProducers("State2") != 1) {
        return false;
    }

    // each definition must have one producer and one consumer
    ReferenceT<DataSourceSignal> def4 = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB2.PidControl2");
    if (def4->GetNumberOfConsumers("State1") != 1 || def4->GetNumberOfProducers("State1") != 0) {
        printf("\n4 %d %d\n", def4->GetNumberOfConsumers("State1"), def4->GetNumberOfProducers("State1"));
        return false;
    }

    if (def4->GetNumberOfConsumers("State2") != 0 || def4->GetNumberOfProducers("State2") != 1) {
        return false;
    }

    return true;
}

bool RealTimeApplicationTest::TestConfigureDataSourceFalse_NoFunctions() {
    ConfigurationDatabase cdb;

    StreamString conf = "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functionss = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = PIDGAM"
            "        }"
            "        +GAM2 = {"
            "            Class = PIDGAM"
            "        }"
            "        +PIDGroup1 = {"
            "            Class = PIDGAMGroup"
            "            +GAM3 = {"
            "                Class = PIDGAM"
            "            }"
            "            +GAM4 = {"
            "                Class = PIDGAM"
            "            }"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functionss.GAM1 :Functionss.GAM2 }"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functionss.PIDGroup1 }"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = DataSourceContainer"
            "        +DDB1 = {"
            "            Class = DataSource"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = DummyScheduler"
            "    }"
            "}";

    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!app->ConfigureArchitecture()) {
        return false;
    }
    return (!app->ConfigureDataSource());
}

bool RealTimeApplicationTest::TestConfigureDataSourceFalse_InvalidFunction() {
    ConfigurationDatabase cdb1;
    // application
    cdb1.CreateAbsolute("$Application1");
    cdb1.Write("Class", "RealTimeApplication");

    cdb1.CreateAbsolute("$Application1.+Functions");
    cdb1.Write("Class", "ReferenceContainer");
    cdb1.CreateAbsolute("$Application1.+Functions.+Function1");
    cdb1.Write("Class", "Object");
    cdb1.MoveToRoot();

    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb1)) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    return (!app->ConfigureDataSource());

}

bool RealTimeApplicationTest::TestValidateDataSource() {
    ConfigurationDatabase cdb;
    StreamString conf = cdbStr2;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!app->ConfigureArchitecture()) {
        return false;
    }
    if (!app->ConfigureDataSource()) {
        return false;
    }

    return app->ValidateDataSource();
}

bool RealTimeApplicationTest::TestValidateDataSourceFalse_MoreThanOneProducer() {
    ConfigurationDatabase cdb1;

    ConfigurationDatabase cdb;
    StreamString conf = "\n"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = PIDGAM"
            "        }"
            "        +GAM2 = {"
            "            Class = PIDGAM"
            "        }"
            "        +PIDGroup1 = {"
            "            Class = PIDGAMGroup"
            "            +GAM3 = {"
            "                Class = PIDGAM"
            "            }"
            "            +GAM4 = {"
            "                Class = PIDGAM"
            "            }"
            "        }"
            "        +GAMContainer = {"
            "            Class = ReferenceContainer"
            "            +GAM5 = {"
            "                Class = PIDGAM"
            "                +Inputs = {"
            "                    Class = GAMSignalsContainer"
            "                    IsInput = true"
            "                    IsFinal = false"
            "                    +Error = {"
            "                        Class = GAMGenericSignal"
            "                        Type = TrackError"
            "                        IsFinal = false"
            "                        +Par2 = {"
            "                            Class = GAMGenericSignal"
            "                                Type = uint32"
            "                                Default = 2"
            "                                Path = DDB1.PidError2"
            "                                IsFinal = true"
            "                            }"
            "                        }"
            "                }"
            "                +Outputs = {"
            "                    Class = GAMSignalsContainer"
            "                    IsOutput = true"
            "                    IsFinal = false"
            "                    +Control = {"
            "                        Class = GAMGenericSignal"
            "                        Type = ControlIn"
            "                        IsFinal = false"
            "                        +Par2 = {"
            "                            Class = GAMGenericSignal"
            "                            Type = uint32"
            "                            Path = DDB2.PidControl2"
            "                            Default = 2"
            "                            IsFinal = true"
            "                        }"
            "                    }"
            "                    +Noise = {"
            "                        Class = GAMGenericSignal"
            "                        Type = ControlNoise"
            "                        IsFinal = true"
            "                        +noiseValue = {"
            "                            Class = GAMGenericSignal"
            "                            Type = float32"
            "                            Default = 2"
            "                            Path = DDB2.PidNoise"
            "                            IsFinal = true"
            "                        }"
            "                    }"
            "                }"
            "            }"
            "            +GAM6 = {"
            "                Class = PIDGAM"
            "            }"
            "        }"
            "        +PIDGroup2 = {"
            "            Class = PIDGAMGroup"
            "            +GAM7 = {"
            "                Class = PIDGAM"
            "            }"
            "            +GAM8 = {"
            "                Class = PlantGAM"
            "                +Inputs = {" /////////////////////////////CHANGED HERE!!
            "                    Class = GAMSignalsContainer"
            "                    IsOutput = true"
            "                    IsFinal = false"
            "                    +Control = {"
            "                        Class = GAMGenericSignal"
            "                        Type = TrackError"
            "                        IsFinal = true"
            "                        +Par1 = {"
            "                            Class = GAMGenericSignal"
            "                            Type = uint32"
            "                            Default = 1"
            "                            Path = DDB2.PidControl1"
            "                            IsFinal = true"
            "                        }"
            "                        +Par2 = {"
            "                            Class = GAMGenericSignal"
            "                            Type = uint32"
            "                            Default = 2"
            "                            Path = DDB2.PidControl2"
            "                            IsFinal = true"
            "                        }"
            "                    }"
            "                }"
            "                +Outputs = {"
            "                    Class = GAMSignalsContainer"
            "                    IsOutput = true"
            "                    IsFinal = true"
            "                    +Error = {"
            "                        Class = GAMGenericSignal"
            "                        Type = TrackError"
            "                        IsFinal = false"
            "                        +Par1 = {"
            "                            Class = GAMGenericSignal"
            "                            Type = uint32"
            "                            Default = 1"
            "                            Path = DDB1.PidError1"
            "                            IsFinal = true"
            "                        }"
            "                        +Par2 = {"
            "                            Class = GAMGenericSignal"
            "                            Type = uint32"
            "                            Default = 2"
            "                            Path = DDB1.PidError2"
            "                            IsFinal = true"
            "                        }"
            "                    }"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.GAM1 :Functions.GAM2}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.PIDGroup1 }"
            "                }"
            "            }"
            "        }"
            "        +State2 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.GAMContainer }"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.PIDGroup2.GAM7  :Functions.PIDGroup2.GAM8 }"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = DataSourceContainer"
            "        +DDB1 = {"
            "            Class = DataSource"
            "        }"
            "        +DDB2 = {"
            "            Class = DataSource"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = DummyScheduler"
            "    }"
            "}";
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!app->ConfigureArchitecture()) {

        return false;
    }
    if (!app->ConfigureDataSource()) {
        return false;
    }

    return !app->ValidateDataSource();
}

bool RealTimeApplicationTest::TestValidateDataSourceFalse_NoData() {
    ConfigurationDatabase cdb1;
    // application
    cdb1.CreateAbsolute("$Application1");
    cdb1.Write("Class", "RealTimeApplication");

    // data
    cdb1.CreateAbsolute("$Application1.+Datas");
    cdb1.Write("Class", "DataSourceContainer");
    cdb1.CreateAbsolute("$Application1.+Datas.+DDB1");
    cdb1.Write("Class", "ReferenceContainer");
    cdb1.CreateAbsolute("$Application1.+Datas.+DDB2");
    cdb1.Write("Class", "ReferenceContainer");
    cdb1.MoveToRoot();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb1);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    return !app->ValidateDataSource();

}

bool RealTimeApplicationTest::TestAllocateDataSource() {
    ConfigurationDatabase cdb;
    StreamString conf = cdbStr2;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!app.IsValid()) {
        return false;
    }

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

    // test if it is possible read a value
    ReferenceT<MemoryMapOutputWriter> writer = ReferenceT<MemoryMapOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<GAMGenericSignal> defOut = app->Find("Functions.PIDGroup2.GAM8.Outputs.Error");
    writer->SetApplication(*app.operator ->());
    writer->AddSignal(defOut);
    writer->Finalise();

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<GAMGenericSignal> defIn = app->Find("Functions.GAMContainer.GAM5.Inputs.Error");
    reader->SetApplication(*app.operator ->());
    reader->AddSignal(defIn);
    reader->Finalise();

    TrackError *var = (TrackError *) writer->GetSignal(0);
    var->Par1 = 1;
    var->Par2 = 2;
    writer->Write(0);

    reader->Read(0);
    TrackError *ret = (TrackError *) writer->GetSignal(0);
    if (ret->Par1 != 1) {
        return false;
    }
    if (ret->Par2 != 2) {
        return false;
    }

    return true;
}

bool RealTimeApplicationTest::TestAllocateDataSourceFalse_NoData() {
    ConfigurationDatabase cdb1;
    // application
    cdb1.CreateAbsolute("$Application1");
    cdb1.Write("Class", "RealTimeApplication");

    // data
    cdb1.CreateAbsolute("$Application1.+Datas");
    cdb1.Write("Class", "DataSourceContainer");
    cdb1.CreateAbsolute("$Application1.+Datas.+DDB1");
    cdb1.Write("Class", "DataSource");
    cdb1.CreateAbsolute("$Application1.+Datas.+DDB2");
    cdb1.Write("Class", "DataSource");
    cdb1.MoveToRoot();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb1);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    return !app->AllocateDataSource();
}

bool RealTimeApplicationTest::TestConfigureDataSourceLinks() {
    ConfigurationDatabase cdb;
    StreamString conf = cdbStr2;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!app.IsValid()) {
        return false;
    }

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

    ReferenceT<PIDGAM> gam5 = app->Find("Functions.GAMContainer.GAM5");
    ReferenceT<PlantGAM> gam8 = app->Find("Functions.PIDGroup2.GAM8");

    // tests if the links are configured

    ReferenceT<BrokerContainer> gam5Reader = gam5->GetInputReader();
    ReferenceT<BrokerContainer> gam5Writer = gam5->GetOutputWriter();

    ReferenceT<BrokerContainer> gam8Reader = gam8->GetInputReader();
    ReferenceT<BrokerContainer> gam8Writer = gam8->GetOutputWriter();

    ControlIn* gam5Out = (ControlIn*) gam5Writer->GetSignal(0);
    gam5Out->Par1 = 2;
    gam5Out->Par2 = 3;

    gam5Writer->Write(0);

    TrackError* gam8Out = (TrackError*) gam8Writer->GetSignal(0);
    gam8Out->Par1 = 4;
    gam8Out->Par2 = 5;

    gam8Writer->Write(0);

    gam5Reader->Read(0);
    TrackError *gam5In = (TrackError *) gam5Reader->GetSignal(0);

    gam8Reader->Read(0);
    ControlIn *gam8In = (ControlIn *) gam8Reader->GetSignal(0);

    if (gam5In->Par1 != 4) {
        return false;
    }
    if (gam5In->Par2 != 5) {
        return false;
    }

    if (gam8In->Par1 != 2) {
        return false;
    }
    if (gam8In->Par2 != 3) {
        return false;
    }

    return true;
}

bool RealTimeApplicationTest::TestConfigureDataSourceLinksFalse_NoFunctions() {
    ConfigurationDatabase cdb1;
    // application
    cdb1.CreateAbsolute("$Application1");
    cdb1.Write("Class", "RealTimeApplication");

    // data
    cdb1.CreateAbsolute("$Application1.+Functionss");
    cdb1.Write("Class", "ReferenceContainer");
    cdb1.MoveToRoot();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb1);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    return !app->ConfigureDataSourceLinks();

}

bool RealTimeApplicationTest::TestConfigureDataSourceLinksFalse_InvalidFunction() {
    ConfigurationDatabase cdb1;
    // application
    cdb1.CreateAbsolute("$Application1");
    cdb1.Write("Class", "RealTimeApplication");

    // data
    cdb1.CreateAbsolute("$Application1.+Functions");
    cdb1.Write("Class", "ReferenceContainer");
    cdb1.CreateAbsolute("$Application1.+Functions.+Function1");
    cdb1.Write("Class", "Object");
    cdb1.MoveToRoot();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb1);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    return !app->ConfigureDataSourceLinks();
}

bool RealTimeApplicationTest::TestPrepareNextState() {

    ConfigurationDatabase cdb;
    StreamString conf = cdbStr2;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!app.IsValid()) {
        return false;
    }

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

    const char8 *nextState = "State2";

    if (!app->PrepareNextState(nextState)) {
        return false;
    }

    ReferenceT<PIDGAM> gam5 = app->Find("Functions.GAMContainer.GAM5");
    ReferenceT<PlantGAM> gam8 = app->Find("Functions.PIDGroup2.GAM8");

    // tests if the links are configured

    ReferenceT<BrokerContainer> gam5Reader = gam5->GetInputReader();

    ReferenceT<BrokerContainer> gam8Reader = gam8->GetInputReader();

    gam5Reader->Read(0);
    TrackError *gam5In = (TrackError *) gam5Reader->GetSignal(0);

    gam8Reader->Read(0);
    ControlIn *gam8In = (ControlIn *) gam8Reader->GetSignal(0);

    if (gam5In->Par1 != 1) {
        return false;
    }
    if (gam5In->Par2 != 2) {
        return false;
    }

    if (gam8In->Par1 != 1) {
        return false;
    }

    if (gam8In->Par2 != 2) {
        return false;
    }
    return true;
}

bool RealTimeApplicationTest::TestPrepareNextStateFalse_NoData() {
    ConfigurationDatabase cdb1;
    // application
    cdb1.CreateAbsolute("$Application1");
    cdb1.Write("Class", "RealTimeApplication");

    cdb1.CreateAbsolute("$Application1.+States");
    cdb1.Write("Class", "ReferenceContainer");
    cdb1.CreateAbsolute("$Application1.+States.+State2");
    cdb1.Write("Class", "RealTimeState");

    // data
    // data
    cdb1.CreateAbsolute("$Application1.+Datas");
    cdb1.Write("Class", "DataSourceContainer");
    cdb1.CreateAbsolute("$Application1.+Datas.+DDB1");
    cdb1.Write("Class", "DataSource");
    cdb1.CreateAbsolute("$Application1.+Datas.+DDB2");
    cdb1.Write("Class", "DataSource");
    //scheduler
    cdb1.CreateAbsolute("$Application1.+Scheduler");
    cdb1.Write("Class", "DummyScheduler");

    cdb1.MoveToRoot();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb1);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    const char8 *nextState = "State2";
    return !app->PrepareNextState(nextState);
}

bool RealTimeApplicationTest::TestGetActiveBuffer() {
    RealTimeApplication app;
    return app.GetActiveBuffer() == 1;
}

bool RealTimeApplicationTest::TestValidateDataSourceLinks() {
    ConfigurationDatabase cdb;

    static const char8 *cdbStr2 = "\n"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = PIDGAM"
            "        }"
            "        +GAM2 = {"
            "            Class = PIDGAM"
            "        }"
            "        +PIDGroup1 = {"
            "            Class = PIDGAMGroup"
            "            +GAM3 = {"
            "                Class = PIDGAM"
            "            }"
            "            +GAM4 = {"
            "                Class = PIDGAM"
            "            }"
            "        }"
            "        +GAMContainer = {"
            "            Class = ReferenceContainer"
            "            +GAM5 = {"
            "                Class = PIDGAM"
            "                +Outputs = {"
            "                    Class = GAMSignalsContainer"
            "                    IsOutput = true"
            "                    IsFinal = false"
            "                    +Control = {"
            "                        Class = GAMGenericSignal"
            "                        Type = ControlIn"
            "                        IsFinal = false"
            "                        +Par2 = {"
            "                            Class = GAMGenericSignal"
            "                            Type = uint32"
            "                            Path = DDB.Counter1"
            "                            Default = 1"
            "                            IsFinal = true"
            "                            Cycles = 1"
            "                        }"
            "                    }"
            "                }"
            "            }"
            "            +GAM6 = {"
            "                Class = PIDGAM"
            "                +Inputs = {"
            "                    Class = GAMGenericSignal"
            "                    Type = uint32"
            "                    Path = DDB.Counter1"
            "                    Default = 1"
            "                    IsFinal = true"
            "                }"
            "            }"
            "        }"
            "        +PIDGroup2 = {"
            "            Class = PIDGAMGroup"
            "            +GAM7 = {"
            "                Class = PIDGAM"
            "            }"
            "            +GAM8 = {"
            "                Class = PlantGAM"
            "                +Inputs = {"
            "                    Class = GAMSignalsContainer"
            "                    IsInput = true"
            "                    IsFinal = false"
            "                    +Control = {"
            "                        Class = GAMGenericSignal"
            "                        Type = TrackError"
            "                        IsFinal = true"
            "                        +Par1 = {"
            "                            Class = GAMGenericSignal"
            "                            Type = uint32"
            "                            Default = 1"
            "                            Path = DDB.Counter1"
            "                            IsFinal = true"
            "                        }"
            "                        +Par2 = {"
            "                            Class = GAMGenericSignal"
            "                            Type = uint32"
            "                            Default = 2"
            "                            Path = DDB.Counter2"
            "                            IsFinal = true"
            "                            Cycles = 1"
            "                        }"
            "                    }"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.GAM1 :Functions.GAM2}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.PIDGroup1 }"
            "                }"
            "            }"
            "        }"
            "        +State2 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.GAMContainer }"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.PIDGroup2.GAM7  :Functions.PIDGroup2.GAM8 }"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = DataSourceContainer"
            "        +DDB = {"
            "            Class = DataSource"
            "            +Counter1 = {"
            "                Class = SyncDataSourceSignal"
            "                Type = uint32"
            "                Default = 1"
            "            }"
            "            +Counter2 = {"
            "                Class = SyncDataSourceSignal"
            "                Type = uint32"
            "                Default = 1"
            "            }"
            "        }"
            "        +DDB2 = {"
            "            Class = DataSource"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = DummyScheduler"
            "    }"
            "}";

    StreamString conf = cdbStr2;
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

    bool ret = app->ValidateDataSourceLinks();
    ObjectRegistryDatabase::Instance()->CleanUp();
    return ret;
}

bool RealTimeApplicationTest::TestValidateDataSourceLinksFalse_MoreSync() {
    ConfigurationDatabase cdb;

    static const char8 *cdbStr2 = "\n"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = PIDGAM"
            "        }"
            "        +GAM2 = {"
            "            Class = PIDGAM"
            "        }"
            "        +PIDGroup1 = {"
            "            Class = PIDGAMGroup"
            "            +GAM3 = {"
            "                Class = PIDGAM"
            "            }"
            "            +GAM4 = {"
            "                Class = PIDGAM"
            "            }"
            "        }"
            "        +GAMContainer = {"
            "            Class = ReferenceContainer"
            "            +GAM5 = {"
            "                Class = PIDGAM"
            "                +Outputs = {"
            "                    Class = GAMSignalsContainer"
            "                    IsOutput = true"
            "                    IsFinal = false"
            "                    +Control = {"
            "                        Class = GAMGenericSignal"
            "                        Type = ControlIn"
            "                        IsFinal = false"
            "                        +Par2 = {"
            "                            Class = GAMGenericSignal"
            "                            Type = uint32"
            "                            Path = DDB.Counter1"
            "                            Default = 1"
            "                            IsFinal = true"
            "                            Cycles = 1"
            "                        }"
            "                    }"
            "                }"
            "            }"
            "            +GAM6 = {"
            "                Class = PIDGAM"
            "                +VarInput = {"
            "                    Class = GAMSignalsContainer"
            "                    IsInput = true"
            "                    IsFinal = true"
            "                    +Counter = {"
            "                        Class = GAMGenericSignal"
            "                        Type = uint32"
            "                        Path = DDB.Counter1"
            "                        Default = 1"
            "                        IsFinal = true"
            "                        Cycles = 2"
            "                    }"
            "                }"
            "            }"
            "        }"
            "        +PIDGroup2 = {"
            "            Class = PIDGAMGroup"
            "            +GAM7 = {"
            "                Class = PIDGAM"
            "            }"
            "            +GAM8 = {"
            "                Class = PlantGAM"
            "                +Inputs = {"
            "                    Class = GAMSignalsContainer"
            "                    IsInput = true"
            "                    IsFinal = false"
            "                    +Control = {"
            "                        Class = GAMGenericSignal"
            "                        Type = TrackError"
            "                        IsFinal = true"
            "                        +Par1 = {"
            "                            Class = GAMGenericSignal"
            "                            Type = uint32"
            "                            Default = 1"
            "                            Path = DDB.Counter1"
            "                            IsFinal = true"
            "                        }"
            "                        +Par2 = {"
            "                            Class = GAMGenericSignal"
            "                            Type = uint32"
            "                            Default = 2"
            "                            Path = DDB.Counter2"
            "                            IsFinal = true"
            "                            Cycles = 1"
            "                        }"
            "                    }"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.GAM1 :Functions.GAM2}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.PIDGroup1 }"
            "                }"
            "            }"
            "        }"
            "        +State2 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.GAMContainer }"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { :Functions.PIDGroup2.GAM7  :Functions.PIDGroup2.GAM8 }"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = DataSourceContainer"
            "        +DDB = {"
            "            Class = DataSource"
            "            +Counter1 = {"
            "                Class = SyncDataSourceSignal"
            "                Type = uint32"
            "                Default = 1"
            "            }"
            "            +Counter2 = {"
            "                Class = SyncDataSourceSignal"
            "                Type = uint32"
            "                Default = 1"
            "            }"
            "        }"
            "        +DDB1 = {"
            "            Class = DataSource"
            "        }"
            "        +DDB2 = {"
            "            Class = DataSource"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = DummyScheduler"
            "    }"
            "}";

    StreamString conf = cdbStr2;
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

    bool ret = !app->ValidateDataSourceLinks();
    ObjectRegistryDatabase::Instance()->CleanUp();
    return ret;
}
/*
bool RealTimeApplicationTest::TestStopExecution() {
    ConfigurationDatabase dummyCDB;

    dummyCDB.Write("Class", "RealTimeApplication");
    dummyCDB.CreateAbsolute("+Scheduler");
    dummyCDB.Write("Class", "DummyScheduler");
    dummyCDB.MoveToRoot();
    RealTimeApplication app;
    if (!app.Initialise(dummyCDB)) {
        return false;
    }

    if (!app.StopExecution()) {
        return false;
    }

    ReferenceT<DummyScheduler> sched = app.Find("+Scheduler");
    return sched->numberOfExecutions == 1;
}*/
