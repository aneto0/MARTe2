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
#include "RealTimeDataSourceDefContainer.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeApplicationTest::RealTimeApplicationTest() {
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
    const char8 *supportedStates[] = { "State1" };
    cdb.Write("States", supportedStates);
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1.+GAM3");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1.+GAM4");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM5");
    cdb.Write("Class", "PIDGAM");
    // gam 5 definitions
    /*  cdb.CreateAbsolute("$Application1.+Functions.+GAM5.+Inputs");
     cdb.Write("Class", "RealTimeDataDefContainer");
     cdb.Write("IsInput", "true");
     cdb.Write("IsFinal", "true");
     cdb.CreateAbsolute("$Application1.+Functions.+GAM5.+Inputs.+Error");
     cdb.Write("Class", "RealTimeGenericDataDef");
     cdb.Write("IsFinal", "false");
     cdb.Write("Type", "TrackError");
     cdb.CreateAbsolute("$Application1.+Functions.+GAM5.+Inputs.+Error.+Par1");
     cdb.Write("Class", "RealTimeGenericDataDef");
     cdb.Write("Type", "uint32");
     cdb.Write("IsFinal", "true");
     cdb.Write("Default", "1");
     cdb.Write("Path", "DDB1.e1");

     cdb.CreateAbsolute("$Application1.+Functions.+GAM5.+Inputs.+Error.+Par2");
     cdb.Write("Class", "RealTimeGenericDataDef");
     cdb.Write("IsFinal", "false");
     cdb.Write("Type", "uint32");
     // no path specified... it should be the name then +Error.+Par2
     cdb.Write("Default", "1");
     */
    cdb.CreateAbsolute("$Application1.+Functions.+GAM5.+Inputs");
    cdb.Write("Class", "RealTimeDataDefContainer");
    cdb.Write("IsInput", "true");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM5.+Inputs.+Error");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "TrackError");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM5.+Inputs.+Error.+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Default", "2");
    cdb.Write("Path", "DDB1.PidError2");
    cdb.Write("IsFinal", "true");

    cdb.CreateAbsolute("$Application1.+Functions.+GAM5.+Outputs");
    cdb.Write("Class", "RealTimeDataDefContainer");
    cdb.Write("IsOutput", "true");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM5.+Outputs.+Control");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM5.+Outputs.+Control.+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "DDB2.PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");

    cdb.CreateAbsolute("$Application1.+Functions.+GAM5.+Outputs.+Noise");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlNoise");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM5.+Outputs.+Noise.+noiseValue");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "float32");
    cdb.Write("Default", "2");
    cdb.Write("Path", "DDB2.PidNoise");
    cdb.Write("IsFinal", "true");

    cdb.CreateAbsolute("$Application1.+Functions.+GAM5.+Outputs");
    cdb.Write("IsOutput", "true");
    cdb.Write("IsFinal", "true");
    cdb.Write("Class", "RealTimeDataDefContainer");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM5.+Outputs.+Control");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "true");
    cdb.Write("Default", "1");
    cdb.Write("Path", "DDB1.u1");

    cdb.CreateAbsolute("$Application1.+Functions.+GAM6");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM7");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM8");
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
    const char8 *functionsT1[2] = { ":+Functions.+GAM1", ":+Functions.+GAM2" };
    cdb.Write("Functions", functionsT1);
    cdb.CreateAbsolute("$Application1.+States.+State1.+Threads.+Thread2");
    cdb.Write("Class", "RealTimeThread");
    const char8 *functionsT2[] = { ":+Functions.+PIDGroup1" };
    cdb.Write("Functions", functionsT2);

    // state 2
    cdb.CreateAbsolute("$Application1.+States.+State2");
    cdb.Write("Class", "RealTimeState");
    cdb.CreateAbsolute("$Application1.+States.+State2.+Threads");
    cdb.Write("Class", "ReferenceContainer");

    // state 2 threads
    cdb.CreateAbsolute("$Application1.+States.+State2.+Threads.+Thread1");
    cdb.Write("Class", "RealTimeThread");
    const char8 *functionsT3[2] = { ":+Functions.+GAM5", ":+Functions.+GAM6" };
    cdb.Write("Functions", functionsT3);
    cdb.CreateAbsolute("$Application1.+States.+State2.+Threads.+Thread2");
    cdb.Write("Class", "RealTimeThread");
    const char8 *functionsT4[2] = { ":+Functions.+GAM7", ":+Functions.+GAM8" };
    cdb.Write("Functions", functionsT4);

    // data
    cdb.CreateAbsolute("$Application1.+Data");
    cdb.Write("Class", "RealTimeDataSourceDefContainer");
    cdb.Write("IsFinal", "false");
    cdb.MoveToRoot();
    bool x = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    printf("\nINITIALIZATION = %d\n", x);
}

RealTimeApplicationTest::~RealTimeApplicationTest() {
    ObjectRegistryDatabase::Instance()->CleanUp();
}

bool RealTimeApplicationTest::TestConstructor() {
    return true;
}

bool RealTimeApplicationTest::TestConfigureArchitecture() {

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    ReferenceT<RealTimeApplication> application = god->Find("$Application1");
    if (!application.IsValid()) {
        return false;
    }

    application->ConfigureArchitecture();

    ReferenceT<RealTimeState> state1 = god->Find("$Application1.+States.+State1");
    if (!state1.IsValid()) {
        return false;
    }

    if (state1->GetNumberOfElements() != 1) {
        return false;
    }

    ReferenceT<RealTimeThread> thread1 = god->Find("$Application1.+States.+State1.+Threads.+Thread1");
    if (!thread1.IsValid()) {
        return false;
    }

    if (thread1->GetNumberOfGAMs() != 2) {
        printf("\n%d\n", thread1->GetNumberOfGAMs());
        return false;
    }

    ReferenceT<GAM> *gams = thread1->GetGAMs();
    ReferenceT<GAM> test1 = god->Find("$Application1.+Functions.+GAM1");
    ReferenceT<GAM> test2 = god->Find("$Application1.+Functions.+GAM2");

    if ((!test1.IsValid()) || (!test2.IsValid())) {
        return false;
    }
    if (gams[0] != test1) {
        return false;
    }
    if (gams[1] != test2) {
        return false;
    }

    StreamString output;
    ConfigurationDatabase outputCDB;
    god->ToStructuredData(outputCDB);
    output.Printf("%!", outputCDB);
    printf("\n%s\n", output.Buffer());

    return true;
}

bool RealTimeApplicationTest::TestConfigureArchitectureGAMGroup() {

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("$Application1");
    if (!application.IsValid()) {
        return false;
    }

    application->ConfigureArchitecture();

    ReferenceT<RealTimeState> state1 = god->Find("$Application1.+States.+State1");
    if (!state1.IsValid()) {
        return false;
    }

    if (state1->GetNumberOfElements() != 1) {
        return false;
    }

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

    ReferenceT<GAM> *gams = thread2->GetGAMs();
    ReferenceT<GAM> test1 = god->Find("$Application1.+Functions.+PIDGroup1.+GAM3");
    ReferenceT<GAM> test2 = god->Find("$Application1.+Functions.+PIDGroup1.+GAM4");

    if ((!test1.IsValid()) || (!test2.IsValid())) {
        return false;
    }
    if (gams[0] != test1) {
        return false;
    }
    if (gams[1] != test2) {
        return false;
    }

    StreamString output;
    ConfigurationDatabase outputCDB;
    god->ToStructuredData(outputCDB);
    output.Printf("%!", outputCDB);
    printf("\n%s\n", output.Buffer());

    return true;
}

bool RealTimeApplicationTest::TestConfigureDataSource() {

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("$Application1");
    if (!application.IsValid()) {
        return false;
    }

    if (!application->ConfigureArchitecture()) {
        return false;
    }

    if (!application->ConfigureDataSource()) {
        return false;
    }

    ReferenceT<RealTimeDataSourceDefContainer> data = god->Find("$Application1.+Data");
    if (!data.IsValid()) {
        return false;
    }

    StreamString output;
    ConfigurationDatabase outputCDB;
    god->ToStructuredData(outputCDB);
    output.Printf("%!", outputCDB);
    printf("\n%s\n", output.Buffer());

    printf("\n%d\n", data->Size());
    return data->Size() == 2;

    /*    ReferenceT<RealTimeState> state1 = god->Find("$Application1.+States.+State1");
     if (!state1.IsValid()) {
     return false;
     }

     if (state1->GetNumberOfElements() != 1) {
     return false;
     }

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

     ReferenceT<GAM> *gams = thread2->GetGAMs();
     ReferenceT<GAM> test1 = god->Find("$Application1.+Functions.+PIDGroup1.+GAM3");
     ReferenceT<GAM> test2 = god->Find("$Application1.+Functions.+PIDGroup1.+GAM4");

     if ((!test1.IsValid()) || (!test2.IsValid())) {
     return false;
     }
     if (gams[0] != test1) {
     return false;
     }
     if (gams[1] != test2) {
     return false;
     }
     */
    return true;

}

