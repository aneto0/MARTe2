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
#include "RealTimeDataSource.h"
#include "GAMTestHelper.h"
#include "RealTimeDataSourceDef.h"
#include "RealTimeGenericDataDef.h"
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

bool RealTimeApplicationTest::TestConfigureArchitecture() {
    ConfigurationDatabase cdb = GetCDB1();
    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    ReferenceT<RealTimeApplication> application = god->Find("$Application1");
    if (!application.IsValid()) {
        return false;
    }

    if (!application->ConfigureArchitecture()) {
        return false;
    }

    // check if the state 1 is consistent with definitions
    ReferenceT<RealTimeState> state1 = god->Find("$Application1.+States.+State1");
    if (!state1.IsValid()) {
        return false;
    }

    if (state1->GetNumberOfGAMGroups() != 1) {
        return false;
    }

    ReferenceT<PIDGAMGroup> gamGroup1 = god->Find("$Application1.+Functions.+PIDGroup1");
    if (state1->GetGAMGroups()[0] != gamGroup1) {
        return false;
    }

    // check the thread 1
    ReferenceT<RealTimeThread> thread1 = god->Find("$Application1.+States.+State1.+Threads.+Thread1");
    if (!thread1.IsValid()) {
        return false;
    }

    if (thread1->GetNumberOfGAMs() != 2) {
        printf("\n%d\n", thread1->GetNumberOfGAMs());
        return false;
    }

    ReferenceT<GAM> *gams1 = thread1->GetGAMs();
    ReferenceT<GAM> test1 = god->Find("$Application1.+Functions.+GAM1");
    ReferenceT<GAM> test2 = god->Find("$Application1.+Functions.+GAM2");

    if (gams1[0] != test1) {
        return false;
    }
    if (gams1[1] != test2) {
        return false;
    }

    // check the thread 2
    ReferenceT<RealTimeThread> thread2 = god->Find("$Application1.+States.+State1.+Threads.+Thread2");
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
    ReferenceT<GAM> test3 = god->Find("$Application1.+Functions.+PIDGroup1.+GAM3");
    ReferenceT<GAM> test4 = god->Find("$Application1.+Functions.+PIDGroup1.+GAM4");

    if (gams2[0] != test3) {
        return false;
    }
    if (gams2[1] != test4) {
        return false;
    }

    // check the state 2
    ReferenceT<RealTimeState> state2 = god->Find("$Application1.+States.+State2");
    if (!state2.IsValid()) {
        return false;
    }

    if (state2->GetNumberOfGAMGroups() != 1) {
        return false;
    }

    ReferenceT<PIDGAMGroup> gamGroup2 = god->Find("$Application1.+Functions.+PIDGroup2");
    if (state2->GetGAMGroups()[0] != gamGroup2) {
        return false;
    }

    // check the thread 3

    ReferenceT<RealTimeThread> thread3 = god->Find("$Application1.+States.+State2.+Threads.+Thread1");
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
    ReferenceT<GAM> test5 = god->Find("$Application1.+Functions.+GAMContainer.+GAM5");
    ReferenceT<GAM> test6 = god->Find("$Application1.+Functions.+GAMContainer.+GAM6");

    if (gams3[0] != test5) {
        return false;
    }
    if (gams3[1] != test6) {
        return false;
    }

    // check the thread 4
    ReferenceT<RealTimeThread> thread4 = god->Find("$Application1.+States.+State2.+Threads.+Thread2");

    if (thread4->GetNumberOfFunctions() != 2) {
        printf("\n%d\n", thread2->GetNumberOfFunctions());
        return false;
    }

    if (thread4->GetNumberOfGAMs() != 2) {
        printf("\n%d\n", thread2->GetNumberOfGAMs());
        return false;
    }

    ReferenceT<GAM> *gams4 = thread4->GetGAMs();
    ReferenceT<GAM> test7 = god->Find("$Application1.+Functions.+PIDGroup2.+GAM7");
    ReferenceT<GAM> test8 = god->Find("$Application1.+Functions.+PIDGroup2.+GAM8");

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

bool RealTimeApplicationTest::TestConfigureDataSource() {
    ConfigurationDatabase cdb1;

    // application
    cdb1.CreateAbsolute("$Application1");
    cdb1.Write("Class", "RealTimeApplication");
    //functions
    cdb1.CreateAbsolute("$Application1.+Functions");
    cdb1.Write("Class", "ReferenceContainer");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAM1");
    cdb1.Write("Class", "PlantGAM");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAM1.+Inputs");
    cdb1.Write("Class", "RealTimeDataDefContainer");
    cdb1.Write("IsInput", "true");
    cdb1.Write("IsFinal", "false");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAM1.+Inputs.+Control");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "TrackError");
    cdb1.Write("IsFinal", "true");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAM1.+Inputs.+Control.+Par1");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "uint32");
    cdb1.Write("Default", "2");
    cdb1.Write("Path", "+DDB2.PidControl1");
    cdb1.Write("IsFinal", "true");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAM1.+Inputs.+Control.+Par2");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "uint32");
    cdb1.Write("Default", "2");
    cdb1.Write("Path", "+DDB2.PidControl2");
    cdb1.Write("IsFinal", "true");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAM1.+Outputs");
    cdb1.Write("Class", "RealTimeDataDefContainer");
    cdb1.Write("IsOutput", "true");
    cdb1.Write("IsFinal", "true");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAM1.+Outputs.+Error");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "TrackError");
    cdb1.Write("IsFinal", "false");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAM1.+Outputs.+Error.+Par1");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "uint32");
    cdb1.Write("Default", "1");
    cdb1.Write("Path", "+DDB1.PidError1");
    cdb1.Write("IsFinal", "true");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAM1.+Outputs.+Error.+Par2");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "uint32");
    cdb1.Write("Default", "2");
    cdb1.Write("Path", "+DDB1.PidError2");
    cdb1.Write("IsFinal", "true");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAM2");
    cdb1.Write("Class", "PIDGAM");
    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup1");
    cdb1.Write("Class", "PIDGAMGroup");
    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup1.+GAM3");
    cdb1.Write("Class", "PIDGAM");
    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup1.+GAM4");
    cdb1.Write("Class", "PIDGAM");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer");
    cdb1.Write("Class", "ReferenceContainer");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5");
    cdb1.Write("Class", "PIDGAM");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Inputs");
    cdb1.Write("Class", "RealTimeDataDefContainer");
    cdb1.Write("IsInput", "true");
    cdb1.Write("IsFinal", "false");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Inputs.+Error");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "TrackError");
    cdb1.Write("IsFinal", "false");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Inputs.+Error.+Par2");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "uint32");
    cdb1.Write("Default", "2");
    cdb1.Write("Path", "+DDB1.PidError2");
    cdb1.Write("IsFinal", "true");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs");
    cdb1.Write("Class", "RealTimeDataDefContainer");
    cdb1.Write("IsOutput", "true");
    cdb1.Write("IsFinal", "false");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs.+Control");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "ControlIn");
    cdb1.Write("IsFinal", "false");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs.+Control.+Par2");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "uint32");
    cdb1.Write("Path", "+DDB2.PidControl2");
    cdb1.Write("Default", "1");
    cdb1.Write("IsFinal", "true");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs.+Noise");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "ControlNoise");
    cdb1.Write("IsFinal", "true");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs.+Noise.+noiseValue");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "float32");
    cdb1.Write("Default", "2");
    cdb1.Write("Path", "+DDB2.PidNoise");
    cdb1.Write("IsFinal", "true");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs");
    cdb1.Write("IsOutput", "true");
    cdb1.Write("IsFinal", "true");
    cdb1.Write("Class", "RealTimeDataDefContainer");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs.+Control");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "ControlIn");
    cdb1.Write("IsFinal", "true");
    cdb1.Write("Default", "1");
    cdb1.Write("Path", "+DDB1.u1");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM6");
    cdb1.Write("Class", "PIDGAM");

    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup2");
    cdb1.Write("Class", "PIDGAMGroup");
    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM7");
    cdb1.Write("Class", "PIDGAM");

    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM8");
    cdb1.Write("Class", "PIDGAM");

    // state 1
    cdb1.CreateAbsolute("$Application1.+States");
    cdb1.Write("Class", "ReferenceContainer");
    cdb1.CreateAbsolute("$Application1.+States.+State1");
    cdb1.Write("Class", "RealTimeState");
    cdb1.CreateAbsolute("$Application1.+States.+State1.+Threads");
    cdb1.Write("Class", "ReferenceContainer");

    // state 1 threads
    cdb1.CreateAbsolute("$Application1.+States.+State1.+Threads.+Thread1");
    cdb1.Write("Class", "RealTimeThread");
    const char8 *functionsT1[2] = { ":+Functions.+GAM1", ":+Functions.+GAM2" };
    cdb1.Write("Functions", functionsT1);
    cdb1.CreateAbsolute("$Application1.+States.+State1.+Threads.+Thread2");
    cdb1.Write("Class", "RealTimeThread");
    const char8 *functionsT2[1] = { ":+Functions.+PIDGroup1" };
    cdb1.Write("Functions", functionsT2);

    // state 2
    cdb1.CreateAbsolute("$Application1.+States.+State2");
    cdb1.Write("Class", "RealTimeState");
    cdb1.CreateAbsolute("$Application1.+States.+State2.+Threads");
    cdb1.Write("Class", "ReferenceContainer");

    // state 2 threads
    cdb1.CreateAbsolute("$Application1.+States.+State2.+Threads.+Thread1");
    cdb1.Write("Class", "RealTimeThread");
    const char8 *functionsT3[1] = { ":+Functions.+GAMContainer" };
    cdb1.Write("Functions", functionsT3);
    cdb1.CreateAbsolute("$Application1.+States.+State2.+Threads.+Thread2");
    cdb1.Write("Class", "RealTimeThread");
    const char8 *functionsT4[2] = { ":+Functions.+PIDGroup2.+GAM7", ":+Functions.+PIDGroup2.+GAM8" };
    cdb1.Write("Functions", functionsT4);

    // data
    cdb1.CreateAbsolute("$Application1.+Data");
    cdb1.Write("Class", "RealTimeDataSource");
    cdb1.Write("IsFinal", "false");
    cdb1.MoveToRoot();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb1);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (!app->ConfigureArchitecture()) {
        return false;
    }
    if (!app->ConfigureDataSource()) {
        return false;
    }

    // each definition must have one producer and one consumer
    ReferenceT<RealTimeDataSourceDef> def1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Data.+DDB1.PidError1");
    if (def1->GetNumberOfConsumers("+State1") != 0 || def1->GetNumberOfProducers("+State1") != 1) {
        printf("\n1 %d %d\n", def1->GetNumberOfConsumers("+State1"), def1->GetNumberOfProducers("+State1"));
        return false;
    }

    if (def1->GetNumberOfConsumers("+State2") != 1 || def1->GetNumberOfProducers("+State2") != 0) {
        return false;
    }

    // each definition must have one producer and one consumer
    ReferenceT<RealTimeDataSourceDef> def2 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Data.+DDB1.PidError2");
    if (def2->GetNumberOfConsumers("+State1") != 0 || def2->GetNumberOfProducers("+State1") != 1) {
        printf("\n2 %d %d\n", def2->GetNumberOfConsumers("+State1"), def2->GetNumberOfProducers("+State1"));
        return false;
    }

    if (def2->GetNumberOfConsumers("+State2") != 1 || def2->GetNumberOfProducers("+State2") != 0) {
        return false;
    }

    // each definition must have one producer and one consumer
    ReferenceT<RealTimeDataSourceDef> def3 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Data.+DDB2.PidControl1");
    if (def3->GetNumberOfConsumers("+State1") != 1 || def3->GetNumberOfProducers("+State1") != 0) {
        printf("\n3 %d %d\n", def3->GetNumberOfConsumers("+State1"), def3->GetNumberOfProducers("+State1"));
        return false;
    }

    if (def3->GetNumberOfConsumers("+State2") != 0 || def3->GetNumberOfProducers("+State2") != 1) {
        return false;
    }

    // each definition must have one producer and one consumer
    ReferenceT<RealTimeDataSourceDef> def4 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Data.+DDB2.PidControl2");
    if (def4->GetNumberOfConsumers("+State1") != 1 || def4->GetNumberOfProducers("+State1") != 0) {
        printf("\n4 %d %d\n", def4->GetNumberOfConsumers("+State1"), def4->GetNumberOfProducers("+State1"));
        return false;
    }

    if (def4->GetNumberOfConsumers("+State2") != 0 || def4->GetNumberOfProducers("+State2") != 1) {
        return false;
    }

    return true;
}

bool RealTimeApplicationTest::TestValidateDataSource() {
    ConfigurationDatabase cdb1 = GetCDB2();
    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb1);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (!app->ConfigureArchitecture()) {
        return false;
    }
    if (!app->ConfigureDataSource()) {
        return false;
    }

    return app->ValidateDataSource();
}

bool RealTimeApplicationTest::TestValidateDataSourceFalse_MoreThanOneConsumer() {
    ConfigurationDatabase cdb1;

    // application
    cdb1.CreateAbsolute("$Application1");
    cdb1.Write("Class", "RealTimeApplication");
    //functions
    cdb1.CreateAbsolute("$Application1.+Functions");
    cdb1.Write("Class", "ReferenceContainer");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAM1");
    cdb1.Write("Class", "PIDGAM");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAM2");
    cdb1.Write("Class", "PIDGAM");
    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup1");
    cdb1.Write("Class", "PIDGAMGroup");
    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup1.+GAM3");
    cdb1.Write("Class", "PIDGAM");
    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup1.+GAM4");
    cdb1.Write("Class", "PIDGAM");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer");
    cdb1.Write("Class", "ReferenceContainer");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5");
    cdb1.Write("Class", "PIDGAM");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Inputs");
    cdb1.Write("Class", "RealTimeDataDefContainer");
    cdb1.Write("IsInput", "true");
    cdb1.Write("IsFinal", "false");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Inputs.+Error");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "TrackError");
    cdb1.Write("IsFinal", "false");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Inputs.+Error.+Par2");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "uint32");
    cdb1.Write("Default", "2");
    cdb1.Write("Path", "+DDB1.PidError2");
    cdb1.Write("IsFinal", "true");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs");
    cdb1.Write("Class", "RealTimeDataDefContainer");
    cdb1.Write("IsOutput", "true");
    cdb1.Write("IsFinal", "false");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs.+Control");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "ControlIn");
    cdb1.Write("IsFinal", "false");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs.+Control.+Par2");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "uint32");
    cdb1.Write("Path", "+DDB2.PidControl2");
    cdb1.Write("Default", "1");
    cdb1.Write("IsFinal", "true");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs.+Noise");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "ControlNoise");
    cdb1.Write("IsFinal", "true");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs.+Noise.+noiseValue");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "float32");
    cdb1.Write("Default", "2");
    cdb1.Write("Path", "+DDB2.PidNoise");
    cdb1.Write("IsFinal", "true");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs");
    cdb1.Write("IsOutput", "true");
    cdb1.Write("IsFinal", "true");
    cdb1.Write("Class", "RealTimeDataDefContainer");
    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs.+Control");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "ControlIn");
    cdb1.Write("IsFinal", "true");
    cdb1.Write("Default", "1");
    cdb1.Write("Path", "+DDB1.u1");

    cdb1.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM6");
    cdb1.Write("Class", "PIDGAM");

    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup2");
    cdb1.Write("Class", "PIDGAMGroup");
    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM7");
    cdb1.Write("Class", "PIDGAM");

    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM8");
    cdb1.Write("Class", "PIDGAM");

    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM8.+Inputs");
    cdb1.Write("Class", "RealTimeDataDefContainer");
    cdb1.Write("IsInput", "true");
    cdb1.Write("IsFinal", "false");
    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM8.+Inputs.+Error");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "TrackError");
    cdb1.Write("IsFinal", "false");
    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM8.+Inputs.+Error.+Par2");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "uint32");
    cdb1.Write("Default", "2");
    cdb1.Write("Path", "+DDB1.PidError2");
    cdb1.Write("IsFinal", "true");

    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM8.+Outputs");
    cdb1.Write("Class", "RealTimeDataDefContainer");
    cdb1.Write("IsOutput", "true");
    cdb1.Write("IsFinal", "false");
    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM8.+Outputs.+Control");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "ControlIn");
    cdb1.Write("IsFinal", "false");
    cdb1.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM8.+Outputs.+Control.+Par2");
    cdb1.Write("Class", "RealTimeGenericDataDef");
    cdb1.Write("Type", "uint32");
    cdb1.Write("Path", "+DDB2.PidControl2");
    cdb1.Write("Default", "1");
    cdb1.Write("IsFinal", "true");

    // state 1
    cdb1.CreateAbsolute("$Application1.+States");
    cdb1.Write("Class", "ReferenceContainer");
    cdb1.CreateAbsolute("$Application1.+States.+State1");
    cdb1.Write("Class", "RealTimeState");
    cdb1.CreateAbsolute("$Application1.+States.+State1.+Threads");
    cdb1.Write("Class", "ReferenceContainer");

    // state 1 threads
    cdb1.CreateAbsolute("$Application1.+States.+State1.+Threads.+Thread1");
    cdb1.Write("Class", "RealTimeThread");
    const char8 *functionsT1[2] = { ":+Functions.+GAM1", ":+Functions.+GAM2" };
    cdb1.Write("Functions", functionsT1);
    cdb1.CreateAbsolute("$Application1.+States.+State1.+Threads.+Thread2");
    cdb1.Write("Class", "RealTimeThread");
    const char8 *functionsT2[1] = { ":+Functions.+PIDGroup1" };
    cdb1.Write("Functions", functionsT2);

    // state 2
    cdb1.CreateAbsolute("$Application1.+States.+State2");
    cdb1.Write("Class", "RealTimeState");
    cdb1.CreateAbsolute("$Application1.+States.+State2.+Threads");
    cdb1.Write("Class", "ReferenceContainer");

    // state 2 threads
    cdb1.CreateAbsolute("$Application1.+States.+State2.+Threads.+Thread1");
    cdb1.Write("Class", "RealTimeThread");
    const char8 *functionsT3[1] = { ":+Functions.+GAMContainer" };
    cdb1.Write("Functions", functionsT3);
    cdb1.CreateAbsolute("$Application1.+States.+State2.+Threads.+Thread2");
    cdb1.Write("Class", "RealTimeThread");
    const char8 *functionsT4[2] = { ":+Functions.+PIDGroup2.+GAM7", ":+Functions.+PIDGroup2.+GAM8" };
    cdb1.Write("Functions", functionsT4);

    // data
    cdb1.CreateAbsolute("$Application1.+Data");
    cdb1.Write("Class", "RealTimeDataSource");
    cdb1.Write("IsFinal", "false");
    cdb1.MoveToRoot();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb1);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (!app->ConfigureArchitecture()) {

        return false;
    }
    if (!app->ConfigureDataSource()) {
        return false;
    }

    return !app->ValidateDataSource();
}

bool RealTimeApplicationTest::TestAllocateDataSource() {
    ConfigurationDatabase cdb1 = GetCDB2();
    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb1);

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

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
    ReferenceT<RealTimeDataSourceOutputWriter> writer = ReferenceT<RealTimeDataSourceOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<RealTimeGenericDataDef> defOut = app->Find("+Functions.+PIDGroup2.+GAM8.+Outputs.+Error");
    writer->SetApplication(app);
    writer->AddVariable(defOut);
    writer->Finalise();

    ReferenceT<RealTimeDataSourceInputReader> reader = ReferenceT<RealTimeDataSourceInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<RealTimeGenericDataDef> defIn = app->Find("+Functions.+GAMContainer.+GAM5.+Inputs.+Error");
    reader->SetApplication(app);
    reader->AddVariable(defIn);
    reader->Finalise();

    TrackError *var = (TrackError *) writer->GetData(0);
    var->Par1 = 1;
    var->Par2 = 2;
    writer->Write(0);

    reader->Read(0);
    TrackError *ret = (TrackError *) writer->GetData(0);
    if (ret->Par1 != 1) {
        return false;
    }
    if (ret->Par2 != 2) {
        return false;
    }

    return true;
}

bool RealTimeApplicationTest::TestConfigureDataSourceLinks() {
    ConfigurationDatabase cdb1 = GetCDB2();
    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb1);

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

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

    ReferenceT<PIDGAM> gam5 = app->Find("+Functions.+GAMContainer.+GAM5");
    ReferenceT<PlantGAM> gam8 = app->Find("+Functions.+PIDGroup2.+GAM8");

    // tests if the links are configured

    ReferenceT<RealTimeDataSourceInputReader> gam5Reader = gam5->GetInputReader();
    ReferenceT<RealTimeDataSourceOutputWriter> gam5Writer = gam5->GetOutputWriter();

    ReferenceT<RealTimeDataSourceInputReader> gam8Reader = gam8->GetInputReader();
    ReferenceT<RealTimeDataSourceOutputWriter> gam8Writer = gam8->GetOutputWriter();

    ControlIn* gam5Out = (ControlIn*) gam5Writer->GetData(0);
    gam5Out->Par1 = 2;
    gam5Out->Par2 = 3;

    gam5Writer->Write(0);

    TrackError* gam8Out = (TrackError*) gam8Writer->GetData(0);
    gam8Out->Par1 = 4;
    gam8Out->Par2 = 5;

    gam8Writer->Write(0);

    gam5Reader->Read(0);
    TrackError *gam5In = (TrackError *) gam5Reader->GetData(0);

    gam8Reader->Read(0);
    ControlIn *gam8In = (ControlIn *) gam8Reader->GetData(0);

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

bool RealTimeApplicationTest::TestPrepareNextState() {
    ConfigurationDatabase cdb1 = GetCDB2();
    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb1);

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

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

    RealTimeStateInfo info;

    info.activeBuffer = 1;
    info.currentState = "";
    info.nextState = "+State2";

    if (!app->PrepareNextState(info)) {
        return false;
    }

    ReferenceT<PIDGAM> gam5 = app->Find("+Functions.+GAMContainer.+GAM5");
    ReferenceT<PlantGAM> gam8 = app->Find("+Functions.+PIDGroup2.+GAM8");

    // tests if the links are configured

    ReferenceT<RealTimeDataSourceInputReader> gam5Reader = gam5->GetInputReader();

    ReferenceT<RealTimeDataSourceInputReader> gam8Reader = gam8->GetInputReader();

    gam5Reader->Read(0);
    TrackError *gam5In = (TrackError *) gam5Reader->GetData(0);

    gam8Reader->Read(0);
    ControlIn *gam8In = (ControlIn *) gam8Reader->GetData(0);

    if (gam5In->Par1 != 1) {
        return false;
    }
    if (gam5In->Par2 != 2) {
        return false;
    }

    if (gam8In->Par1 != 2) {
        return false;
    }
    if (gam8In->Par2 != 2) {
        return false;
    }

    return true;
}

bool RealTimeApplicationTest::TestChangeState() {
    RealTimeApplication app;
    if (app.GetActiveBuffer() != 1) {
        return false;
    }
    if (app.ChangeState() != 0) {
        return false;
    }
    return app.ChangeState() == 1;
}

bool RealTimeApplicationTest::TestGetActiveBuffer() {
    RealTimeApplication app;
    if (app.GetActiveBuffer() != 1) {
        return false;
    }
    if (app.ChangeState() != 0) {
        return false;
    }
    if (app.GetActiveBuffer() != 0) {
        return false;
    }

    if (app.ChangeState() != 1) {
        return false;
    }
    return app.GetActiveBuffer() == 1;
}

