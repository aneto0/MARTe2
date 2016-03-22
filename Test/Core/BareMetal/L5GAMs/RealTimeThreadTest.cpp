/**
 * @file RealTimeThreadTest.cpp
 * @brief Source file for class RealTimeThreadTest
 * @date 07/03/2016
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
 * the class RealTimeThreadTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeThreadTest.h"
#include "ConfigurationDatabase.h"
#include "GAMTestHelper.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeThreadTest::RealTimeThreadTest() {
    cdb = GetCDB1();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        printf("\n!!!\n");
    }
}

RealTimeThreadTest::~RealTimeThreadTest() {
    ObjectRegistryDatabase::Instance()->CleanUp();
}

bool RealTimeThreadTest::TestConstructor() {

    RealTimeThread test;

    if (test.GetNumberOfFunctions() != 0) {
        return false;
    }
    if (test.GetFunctions() != NULL) {
        return false;
    }
    return test.Size() == 0;
}

bool RealTimeThreadTest::TestInitialise() {

    ConfigurationDatabase cdb;

    const char8 * functions[3] = { "a", "b", "c" };
    cdb.Write("Functions", functions);

    RealTimeThread test;
    if (!test.Initialise(cdb)) {
        return false;
    }

    StreamString *functionRet = test.GetFunctions();
    if (functionRet[0] != "a") {
        return false;
    }
    if (functionRet[1] != "b") {
        return false;
    }

    if (functionRet[2] != "c") {
        return false;
    }

    return test.GetNumberOfFunctions() == 3;

}

bool RealTimeThreadTest::TestInitialiseFalse_NoFunctions() {
    ConfigurationDatabase cdb;

    RealTimeThread test;
    return (!test.Initialise(cdb));
}

bool RealTimeThreadTest::TestConfigureArchitecture() {

    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1.+Threads.+Thread1");
    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (!thread1->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }

    ReferenceT<PIDGAM> gam1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+GAM1");
    ReferenceT<PIDGAM> gam2 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+GAM2");

    if (gam1->GetNumberOfSupportedStates() != 1) {
        return false;
    }

    if (gam1->GetSupportedStates()[0] != "+State1") {
        return false;
    }
    if (gam2->GetNumberOfSupportedStates() != 1) {
        return false;
    }

    if (gam2->GetSupportedStates()[0] != "+State1") {
        return false;
    }

    return true;
}

bool RealTimeThreadTest::TestConfigureArchitecture_MoreGAMs() {
    ConfigurationDatabase cdb;

    // application
    cdb.CreateAbsolute("$Application1");
    cdb.Write("Class", "RealTimeApplication");
    //functions
    cdb.CreateAbsolute("$Application1.+Functions");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM1");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM2");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1");
    cdb.Write("Class", "PIDGAMGroup");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1.+GAM3");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1.+GAM4");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer");
    cdb.Write("Class", "ReferenceContainer");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM6");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2");
    cdb.Write("Class", "PIDGAMGroup");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM7");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM8");
    cdb.Write("Class", "PIDGAM");

    // state 1
    cdb.CreateAbsolute("$Application1.+States");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateAbsolute("$Application1.+States.+State1");
    cdb.Write("Class", "RealTimeState");
    cdb.CreateAbsolute("$Application1.+States.+State1.+Threads");
    cdb.Write("Class", "ReferenceContainer");

    // state 1 threads
    cdb.CreateAbsolute("$Application1.+States.+State1.+Threads.+Thread1");
    cdb.Write("Class", "RealTimeThread");
    const char8 *functionsT1[6] = { ":+Functions.+GAM1", ":+Functions.+GAM2", ":+Functions.+PIDGroup1", ":+Functions.+GAMContainer",
            ":+Functions.+PIDGroup2.+GAM7", ":+Functions.+PIDGroup2.+GAM8" };
    cdb.Write("Functions", functionsT1);

    // data
    cdb.CreateAbsolute("$Application1.+Data");
    cdb.Write("Class", "RealTimeDataSourceContainer");

    //scheduler
    cdb.CreateAbsolute("$Application1.+Scheduler");
    cdb.Write("Class", "BasicGAMScheduler");
    cdb.MoveToRoot();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1.+Threads.+Thread1");

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (!thread1->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }

    if (thread1->GetNumberOfGAMs() != 8) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    return true;
}

bool RealTimeThreadTest::TestConfigureArchitecture_GAMGroup() {
    ReferenceT<RealTimeThread> thread2 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1.+Threads.+Thread2");

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (!thread2->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }

    ReferenceT<PIDGAM> gam3 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+PIDGroup1.+GAM3");
    ReferenceT<PIDGAM> gam4 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+PIDGroup1.+GAM4");

    if (gam3->GetNumberOfSupportedStates() != 1) {
        return false;
    }

    if (gam3->GetSupportedStates()[0] != "+State1") {
        return false;
    }
    if (gam4->GetNumberOfSupportedStates() != 1) {
        return false;
    }

    if (gam4->GetSupportedStates()[0] != "+State1") {
        return false;
    }

    return true;
}

bool RealTimeThreadTest::TestConfigureArchitecture_ReferenceContainer() {
    ReferenceT<RealTimeThread> thread3 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State2.+Threads.+Thread1");

    ReferenceT<RealTimeState> state2 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State2");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (!thread3->ConfigureArchitecture(*app.operator->(), *state2.operator->())) {
        return false;
    }

    ReferenceT<PIDGAM> gam5 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+GAMContainer.+GAM5");
    ReferenceT<PIDGAM> gam6 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+GAMContainer.+GAM6");

    if (gam5->GetNumberOfSupportedStates() != 1) {
        return false;
    }

    if (gam5->GetSupportedStates()[0] != "+State2") {
        return false;
    }
    if (gam6->GetNumberOfSupportedStates() != 1) {
        return false;
    }

    if (gam6->GetSupportedStates()[0] != "+State2") {
        return false;
    }

    return true;
}

bool RealTimeThreadTest::TestConfigureArchitecture_SingleInGAMGroup() {
    ReferenceT<RealTimeThread> thread4 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State2.+Threads.+Thread2");

    ReferenceT<RealTimeState> state2 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State2");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (!thread4->ConfigureArchitecture(*app.operator->(), *state2.operator->())) {
        return false;
    }

    ReferenceT<PIDGAM> gam7 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+PIDGroup2.+GAM7");
    ReferenceT<PIDGAM> gam8 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+PIDGroup2.+GAM8");

    if (gam7->GetNumberOfSupportedStates() != 1) {
        return false;
    }

    if (gam7->GetSupportedStates()[0] != "+State2") {
        return false;
    }
    if (gam8->GetNumberOfSupportedStates() != 1) {
        return false;
    }

    if (gam8->GetSupportedStates()[0] != "+State2") {
        return false;
    }

    return true;
}

bool RealTimeThreadTest::TestConfigureArchitectureFalse_OrphanGAM() {
    ConfigurationDatabase cdb;

    // application

    cdb.CreateAbsolute("+GAM1");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1");
    cdb.Write("Class", "RealTimeApplication");
    //functions
    cdb.CreateAbsolute("$Application1.+Functions");
    cdb.Write("Class", "ReferenceContainer");

    cdb.CreateAbsolute("$Application1.+Functions.+GAM2");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1");
    cdb.Write("Class", "PIDGAMGroup");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1.+GAM3");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1.+GAM4");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer");
    cdb.Write("Class", "ReferenceContainer");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM6");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2");
    cdb.Write("Class", "PIDGAMGroup");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM7");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM8");
    cdb.Write("Class", "PIDGAM");

    // state 1
    cdb.CreateAbsolute("$Application1.+States");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateAbsolute("$Application1.+States.+State1");
    cdb.Write("Class", "RealTimeState");
    cdb.CreateAbsolute("$Application1.+States.+State1.+Threads");
    cdb.Write("Class", "ReferenceContainer");

    // state 1 threads
    cdb.CreateAbsolute("$Application1.+States.+State1.+Threads.+Thread1");
    cdb.Write("Class", "RealTimeThread");
    const char8 *functionsT1[6] = { "::+GAM1", ":+Functions.+GAM2", ":+Functions.+PIDGroup1", ":+Functions.+GAMContainer", ":+Functions.+PIDGroup2.+GAM7",
            ":+Functions.+PIDGroup2.+GAM8" };
    cdb.Write("Functions", functionsT1);

    // data
    cdb.CreateAbsolute("$Application1.+Data");
    cdb.Write("Class", "RealTimeDataSourceContainer");

    //scheduler
    cdb.CreateAbsolute("$Application1.+Scheduler");
    cdb.Write("Class", "BasicGAMScheduler");

    cdb.MoveToRoot();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);

    printf("\nsize ordb= %d\n", ObjectRegistryDatabase::Instance()->Size());
    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1.+Threads.+Thread1");

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (thread1->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    return true;
}

bool RealTimeThreadTest::TestConfigureArchitectureFalse_InvalidGAMType() {
    ConfigurationDatabase cdb;

    // application
    cdb.CreateAbsolute("$Application1");
    cdb.Write("Class", "RealTimeApplication");
    //functions
    cdb.CreateAbsolute("$Application1.+Functions");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM1");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM2");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1");
    cdb.Write("Class", "PIDGAMGroup");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1.+GAM3");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1.+GAM4");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer");
    cdb.Write("Class", "ReferenceContainer");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5");
    cdb.Write("Class", "PIDGAM");

    //invalid here
    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM6");
    cdb.Write("Class", "Object");

    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2");
    cdb.Write("Class", "PIDGAMGroup");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM7");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM8");
    cdb.Write("Class", "PIDGAM");

    // state 1
    cdb.CreateAbsolute("$Application1.+States");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateAbsolute("$Application1.+States.+State1");
    cdb.Write("Class", "RealTimeState");
    cdb.CreateAbsolute("$Application1.+States.+State1.+Threads");
    cdb.Write("Class", "ReferenceContainer");

    // state 1 threads
    cdb.CreateAbsolute("$Application1.+States.+State1.+Threads.+Thread1");
    cdb.Write("Class", "RealTimeThread");
    const char8 *functionsT1[6] = { ":+Functions.+GAM1", ":+Functions.+GAM2", ":+Functions.+PIDGroup1", ":+Functions.+GAMContainer",
            ":+Functions.+PIDGroup2.+GAM7", ":+Functions.+PIDGroup2.+GAM8" };
    cdb.Write("Functions", functionsT1);

    // data
    cdb.CreateAbsolute("$Application1.+Data");
    cdb.Write("Class", "RealTimeDataSourceContainer");

    //scheduler
    cdb.CreateAbsolute("$Application1.+Scheduler");
    cdb.Write("Class", "BasicGAMScheduler");

    cdb.MoveToRoot();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1.+Threads.+Thread1");

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (thread1->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    return true;

}

bool RealTimeThreadTest::TestConfigureArchitectureFalse_InvalidGAMPath() {
    ConfigurationDatabase cdb;

    // application
    cdb.CreateAbsolute("$Application1");
    cdb.Write("Class", "RealTimeApplication");
    //functions
    cdb.CreateAbsolute("$Application1.+Functions");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM1");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM2");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1");
    cdb.Write("Class", "PIDGAMGroup");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1.+GAM3");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1.+GAM4");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer");
    cdb.Write("Class", "ReferenceContainer");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM6");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2");
    cdb.Write("Class", "PIDGAMGroup");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM7");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM8");
    cdb.Write("Class", "PIDGAM");

    // state 1
    cdb.CreateAbsolute("$Application1.+States");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateAbsolute("$Application1.+States.+State1");
    cdb.Write("Class", "RealTimeState");
    cdb.CreateAbsolute("$Application1.+States.+State1.+Threads");
    cdb.Write("Class", "ReferenceContainer");

    // state 1 threads
    cdb.CreateAbsolute("$Application1.+States.+State1.+Threads.+Thread1");
    cdb.Write("Class", "RealTimeThread");
    const char8 *functionsT1[6] = { ":+Functions.boh.+GAM1", ":+Functions.+GAM2", ":+Functions.+PIDGroup1", ":+Functions.+GAMContainer",
            ":+Functions.+PIDGroup2.+GAM7", ":+Functions.+PIDGroup2.+GAM8" };
    cdb.Write("Functions", functionsT1);

    // data
    cdb.CreateAbsolute("$Application1.+Data");
    cdb.Write("Class", "RealTimeDataSourceContainer");

    //scheduler
    cdb.CreateAbsolute("$Application1.+Scheduler");
    cdb.Write("Class", "BasicGAMScheduler");
    cdb.MoveToRoot();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1.+Threads.+Thread1");

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (thread1->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    return true;
}

bool RealTimeThreadTest::TestGetNumberOfFunctions() {
    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1.+Threads.+Thread1");
    ReferenceT<RealTimeThread> thread2 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1.+Threads.+Thread2");
    ReferenceT<RealTimeThread> thread3 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State2.+Threads.+Thread1");
    ReferenceT<RealTimeThread> thread4 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State2.+Threads.+Thread2");

    if (thread1->GetNumberOfFunctions() != 2) {
        return false;
    }
    if (thread2->GetNumberOfFunctions() != 1) {
        return false;
    }
    if (thread3->GetNumberOfFunctions() != 1) {
        return false;
    }
    if (thread4->GetNumberOfFunctions() != 2) {
        return false;
    }

    return true;
}

bool RealTimeThreadTest::TestGetFunctions() {
    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1.+Threads.+Thread1");
    ReferenceT<RealTimeThread> thread2 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1.+Threads.+Thread2");
    ReferenceT<RealTimeThread> thread3 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State2.+Threads.+Thread1");
    ReferenceT<RealTimeThread> thread4 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State2.+Threads.+Thread2");

    if (thread1->GetFunctions()[0] != ":+Functions.+GAM1") {
        return false;
    }
    if (thread1->GetFunctions()[1] != ":+Functions.+GAM2") {
        return false;
    }
    if (thread2->GetFunctions()[0] != ":+Functions.+PIDGroup1") {
        return false;
    }
    if (thread3->GetFunctions()[0] != ":+Functions.+GAMContainer") {
        return false;
    }
    if (thread4->GetFunctions()[0] != ":+Functions.+PIDGroup2.+GAM7") {
        return false;
    }
    if (thread4->GetFunctions()[1] != ":+Functions.+PIDGroup2.+GAM8") {
        return false;
    }

    return true;
}

bool RealTimeThreadTest::TestGetNumberOfGAMs() {
    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1.+Threads.+Thread1");
    ReferenceT<RealTimeThread> thread2 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1.+Threads.+Thread2");
    ReferenceT<RealTimeThread> thread3 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State2.+Threads.+Thread1");
    ReferenceT<RealTimeThread> thread4 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State2.+Threads.+Thread2");

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1");
    ReferenceT<RealTimeState> state2 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State2");
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (!thread1->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }
    if (!thread2->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }

    if (!thread3->ConfigureArchitecture(*app.operator->(), *state2.operator->())) {
        return false;
    }

    if (!thread4->ConfigureArchitecture(*app.operator->(), *state2.operator->())) {
        return false;
    }

    if (thread1->GetNumberOfGAMs() != 2) {
        return false;
    }
    if (thread2->GetNumberOfGAMs() != 2) {
        return false;
    }
    if (thread3->GetNumberOfGAMs() != 2) {
        return false;
    }
    if (thread4->GetNumberOfGAMs() != 2) {
        return false;
    }

    return true;
}

bool RealTimeThreadTest::TestGetGAMs() {
    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1.+Threads.+Thread1");
    ReferenceT<RealTimeThread> thread2 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1.+Threads.+Thread2");
    ReferenceT<RealTimeThread> thread3 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State2.+Threads.+Thread1");
    ReferenceT<RealTimeThread> thread4 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State2.+Threads.+Thread2");

    ReferenceT<PIDGAM> gam1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+GAM1");
    ReferenceT<PIDGAM> gam2 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+GAM2");

    ReferenceT<PIDGAM> gam3 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+PIDGroup1.+GAM3");
    ReferenceT<PIDGAM> gam4 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+PIDGroup1.+GAM4");

    ReferenceT<PIDGAM> gam5 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+GAMContainer.+GAM5");
    ReferenceT<PIDGAM> gam6 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+GAMContainer.+GAM6");

    ReferenceT<PIDGAM> gam7 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+PIDGroup2.+GAM7");
    ReferenceT<PIDGAM> gam8 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+PIDGroup2.+GAM8");

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1");
    ReferenceT<RealTimeState> state2 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State2");
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (!thread1->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }
    if (!thread2->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }

    if (!thread3->ConfigureArchitecture(*app.operator->(), *state2.operator->())) {
        return false;
    }

    if (!thread4->ConfigureArchitecture(*app.operator->(), *state2.operator->())) {
        return false;
    }

    if (thread1->GetGAMs()[0] != gam1) {
        return false;
    }
    if (thread1->GetGAMs()[1] != gam2) {
        return false;
    }
    if (thread2->GetGAMs()[0] != gam3) {
        return false;
    }
    if (thread2->GetGAMs()[1] != gam4) {
        return false;
    }
    if (thread3->GetGAMs()[0] != gam5) {
        return false;
    }
    if (thread3->GetGAMs()[1] != gam6) {
        return false;
    }
    if (thread4->GetGAMs()[0] != gam7) {
        return false;
    }
    if (thread4->GetGAMs()[1] != gam8) {
        return false;
    }
    return true;
}

bool RealTimeThreadTest::TestToStructuredData() {
    ReferenceT<RealTimeThread> thread2 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1.+Threads.+Thread2");

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1");
    ReferenceT<RealTimeState> state2 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State2");
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");
    if (!thread2->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }

    ConfigurationDatabase outCDB;

    thread2->ToStructuredData(outCDB);
    StreamString display;
    display.Printf("%!", outCDB);

    printf("\n%s\n", display.Buffer());

    StreamString test = "+Thread2 = {\n"
            "Class = RealTimeThread\n"
            "Functions = { :+Functions.+PIDGroup1 } \n"
            "+PIDGroup1 = {\n"
            "Class = PIDGAMGroup\n"
            "+GAM3 = {\n"
            "Class = PIDGAM\n"
            "}\n"
            "+GAM4 = {\n"
            "Class = PIDGAM\n"
            "}\n"
            "}\n"
            "}\n";

    return test == display;
}

