/**
 * @file MemoryMapInputReaderTest.cpp
 * @brief Source file for class MemoryMapInputReaderTest
 * @date 19/apr/2016
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
 * the class MemoryMapInputReaderTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryMapInputReaderTest.h"
#include "GAMTestHelper.h"
#include "GAMGenericSignal.h"
#include "Threads.h"
#include "GAMSampledSignal.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MemoryMapInputReaderTest::MemoryMapInputReaderTest() {

    StreamString pidCFG = ""
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

    pidCFG.Seek(0);
    StandardParser parser1(pidCFG, pidCDB);
    parser1.Parse();

    StreamString plantCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Control = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = TrackError\n"
            "        IsFinal = true\n"
            "        +Par1 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Default = 1\n"
            "            Path = DDB2.PidControl1\n"
            "            IsFinal = true\n"
            "        }\n"
            "        +Par2 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Default = 2\n"
            "            Path = DDB2.PidControl2\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+Outputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsOutput = true\n"
            "    IsFinal = true\n"
            "    +Error = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = TrackError\n"
            "        IsFinal = false\n"
            "        +Par1 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Default = 1\n"
            "            Path = DDB1.PidError1\n"
            "            IsFinal = true\n"
            "        }\n"
            "        +Par2 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Default = 2\n"
            "            Path = DDB1.PidError2\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n";
    plantCFG.Seek(0);
    StandardParser parser2(plantCFG, plantCDB);
    parser2.Parse();

    StreamString appCFG = ""
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +DDB1 = {\n"
            "        Class = DataSource\n"
            "    }\n"
            "    +DDB2 = {\n"
            "        Class = DataSource\n"
            "    }\n"
            "}\n"
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n";

    appCFG.Seek(0);

    StandardParser parser3(appCFG, appCDB);
    parser3.Parse();

}

bool MemoryMapInputReaderTest::TestConstructor() {
    MemoryMapInputReader test;
    return true;
}

bool MemoryMapInputReaderTest::TestRead_DefaultValues_Static() {

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(pidCDB)) {
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

    TrackError error;

    ReferenceT<GAMGenericSignal> def = gam->Find("Inputs.Error");

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("broker");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddSignal(def, &error)) {
        return false;
    }

    if (!reader->Finalise()) {
        return false;
    }

    const char8 *nextState = "state1";

    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);

    if (!reader->Read(0)) {
        return false;
    }

    // test if the deafult value are read
    if (error.Par1 != 1) {
        return false;
    }

    if (error.Par2 != 2) {
        return false;
    }

    printf("\nerror = %d %d\n", error.Par1, error.Par2);

    return true;

}

bool MemoryMapInputReaderTest::TestRead_DefaultValues_Allocation() {

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(pidCDB)) {
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

    ReferenceT<GAMGenericSignal> def = gam->Find("Inputs.Error");

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("broker");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddSignal(def)) {
        return false;
    }

    if (!reader->Finalise()) {
        return false;
    }

    const char8 *nextState = "state1";
    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);

    if (!reader->Read(0)) {
        return false;
    }

    TrackError *error = (TrackError *) reader->GetSignal(0);

    // test if the deafult value are read
    if (error->Par1 != 1) {
        return false;
    }

    if (error->Par2 != 2) {
        return false;
    }

    printf("\nerror = %d %d\n", error->Par1, error->Par2);

    return true;
}

bool MemoryMapInputReaderTest::TestRead_Static() {

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> pid = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(pidCDB)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("state1", "thread1");
    if (!pid->ConfigureDataSource()) {
        return false;
    }

    ReferenceT<PlantGAM> plant = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    plant->SetName("plant");
    if (!plant->Initialise(plantCDB)) {
        return false;
    }

    plant->SetApplication(*rtapp.operator ->());
    plant->AddState("state1", "thread1");
    if (!plant->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    TrackError errorPID;

    ReferenceT<GAMGenericSignal> def = pid->Find("Inputs.Error");

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddSignal(def, &errorPID)) {
        return false;
    }

    if (!reader->Finalise()) {
        return false;
    }

    TrackError errorPlant;
    errorPlant.Par1 = 10;
    errorPlant.Par2 = 20;

    ReferenceT<GAMGenericSignal> def2 = plant->Find("Outputs.Error");

    ReferenceT<MemoryMapOutputWriter> writer = ReferenceT<MemoryMapOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(*rtapp.operator ->());

    if (!writer->AddSignal(def2, &errorPlant)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    const char8 *nextState = "state1";
    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);

    if (!writer->Write(0)) {
        return false;
    }

    if (!reader->Read(0)) {
        return false;
    }
    printf("\nerror = %d %d\n", errorPID.Par1, errorPID.Par2);

    // test if the deafult value are read
    if (errorPID.Par1 != 10) {
        return false;
    }

    if (errorPID.Par2 != 20) {
        return false;
    }

    return true;
}

bool MemoryMapInputReaderTest::TestRead_Allocation() {

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> pid = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(pidCDB)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("state1", "thread1");
    if (!pid->ConfigureDataSource()) {
        return false;
    }

    ReferenceT<PlantGAM> plant = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    plant->SetName("plant");
    if (!plant->Initialise(plantCDB)) {
        return false;
    }

    plant->SetApplication(*rtapp.operator ->());
    plant->AddState("state1", "thread1");
    if (!plant->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def = pid->Find("Inputs.Error");

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddSignal(def)) {
        return false;
    }

    if (!reader->Finalise()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def2 = plant->Find("Outputs.Error");

    ReferenceT<MemoryMapOutputWriter> writer = ReferenceT<MemoryMapOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(*rtapp.operator ->());

    if (!writer->AddSignal(def2)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    TrackError *errorPlant = (TrackError *) writer->GetSignal(0);
    errorPlant->Par1 = 10;
    errorPlant->Par2 = 20;

    const char8 *nextState = "state1";
    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);

    if (!writer->Write(0)) {
        return false;
    }

    if (!reader->Read(0)) {
        return false;
    }

    TrackError *errorPID;
    errorPID = (TrackError *) reader->GetSignal(0);

    printf("\nerror = %d %d\n", errorPID->Par1, errorPID->Par2);

    // test if the deafult value are read
    if (errorPID->Par1 != 10) {
        return false;
    }

    if (errorPID->Par2 != 20) {
        return false;
    }

    return true;
}

bool MemoryMapInputReaderTest::TestRead_MoreThanOneVariable() {

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> pid = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(pidCDB)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("state1", "thread1");
    if (!pid->ConfigureDataSource()) {
        return false;
    }

    StreamString plantCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Control = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = TrackError\n"
            "        IsFinal = true\n"
            "        +Par1 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Path = DDB2.PidControl1\n"
            "            IsFinal = true\n"
            "        }\n"
            "        +Par2 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Path = DDB2.PidControl2\n"
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
            "            Path = DDB2.PidNoise\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+Outputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsOutput = true\n"
            "    IsFinal = true\n"
            "    +Error = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = TrackError\n"
            "        IsFinal = false\n"
            "        +Par1 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Path = DDB1.PidError1\n"
            "            IsFinal = true\n"
            "        }\n"
            "        +Par2 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Path = DDB1.PidError2\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n";

    plantCFG.Seek(0);
    ConfigurationDatabase plantCDB;

    StandardParser parser(plantCFG, plantCDB);
    if (!parser.Parse()) {
        return false;
    }
    ReferenceT<PlantGAM> plant = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    plant->SetName("plant");
    if (!plant->Initialise(plantCDB)) {
        return false;
    }

    plant->SetApplication(*rtapp.operator ->());
    plant->AddState("state1", "thread1");
    if (!plant->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def1 = pid->Find("Outputs.Control");
    ReferenceT<GAMGenericSignal> def2 = pid->Find("Outputs.Noise");

    ReferenceT<MemoryMapOutputWriter> writer = ReferenceT<MemoryMapOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(*rtapp.operator ->());

    if (!writer->AddSignal(def1)) {
        return false;
    }
    if (!writer->AddSignal(def2)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    ControlIn *pidControl = (ControlIn *) writer->GetSignal(0);
    ControlNoise *pidNoise = (ControlNoise *) writer->GetSignal(1);

    pidControl->Par1 = 10;
    pidControl->Par2 = 20;
    pidNoise->noiseValue = 0.5;

    ReferenceT<GAMGenericSignal> def3 = plant->Find("Inputs.Control");
    ReferenceT<GAMGenericSignal> def4 = plant->Find("Inputs.Noise");

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddSignal(def3)) {
        return false;
    }

    if (!reader->AddSignal(def4)) {
        return false;
    }

    if (!reader->Finalise()) {
        return false;
    }

    const char8 *nextState = "state1";
    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);
    if (!writer->Write(0)) {
        return false;
    }

    if (!reader->Read(0)) {
        return false;
    }

    ControlIn *controlPlant = (ControlIn *) reader->GetSignal(0);
    ControlNoise *controlNoise = (ControlNoise *) reader->GetSignal(1);

    printf("\nerror = %d %d %f\n", controlPlant->Par1, controlPlant->Par2, controlNoise->noiseValue);

    // test if the deafult value are read
    if (controlPlant->Par1 != 10) {
        return false;
    }

    if (controlPlant->Par2 != 20) {
        return false;
    }

    if (controlNoise->noiseValue != 0.5) {
        return false;
    }

    return true;
}

bool MemoryMapInputReaderTest::TestRead_MultiDim_Vector() {

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    printf("\n??\n");
    StreamString multiCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = ControlInArray\n"
            "        IsFinal = true\n"
            "        +Pars = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Dimensions = \"[2]\"\n"
            "            Default = \"{1,2}\"\n"
            "            Path = DDB1.PidControlArray\n"
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
            "        Type = ControlInArray\n"
            "        IsFinal = true\n"
            "        +Pars = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Dimensions = \"[2]\"\n"
            "            Path = DDB1.PidControlArray\n"
            "            Default = \"{1,2}\"\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n";

    multiCFG.Seek(0);
    ConfigurationDatabase cdbMulti;

    StreamString error;
    StandardParser parser(multiCFG, cdbMulti, &error);
    if (!parser.Parse()) {
        printf("\n%s\n", error.Buffer());
        return false;
    }

    ReferenceT<PIDGAM2> pid = ReferenceT<PIDGAM2>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(cdbMulti)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("state1", "thread1");
    if (!pid->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> defOut = pid->Find("Outputs.Control");

    ReferenceT<MemoryMapOutputWriter> writer = ReferenceT<MemoryMapOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(*rtapp.operator ->());

    if (!writer->AddSignal(defOut)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    ControlInArray *pidControl = (ControlInArray *) writer->GetSignal(0);

    pidControl->Pars[0] = 10;
    pidControl->Pars[1] = 20;

    ReferenceT<GAMGenericSignal> defIn = pid->Find("Inputs.Error");

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddSignal(defIn)) {
        return false;
    }
    if (!reader->Finalise()) {
        return false;
    }

    const char8 *nextState = "state1";
    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);

    if (!writer->Write(0)) {
        return false;
    }

    if (!reader->Read(0)) {
        return false;
    }

    ControlInArray *controlPlant = (ControlInArray *) reader->GetSignal(0);

    printf("\nerror = %d %d \n", controlPlant->Pars[0], controlPlant->Pars[1]);

    // test if the deafult value are read
    if (controlPlant->Pars[0] != 10) {
        return false;
    }

    if (controlPlant->Pars[1] != 20) {
        return false;
    }
    return true;
}

bool MemoryMapInputReaderTest::TestRead_MultiDim_Matrix() {

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    StreamString multiCFG = "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = ControlInMatrix\n"
            "        IsFinal = true\n"
            "        +Pars = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Dimensions = \"[3][2]\"\n"
            "            Default = \"{ {1,2}, {3,4}, {5,6}}\"\n"
            "            Path = DDB1.PidControlArray\n"
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
            "        Type = ControlInMatrix\n"
            "        IsFinal = true\n"
            "        +Pars = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Dimensions = \"[3][2]\"\n"
            "            Path = DDB1.PidControlArray\n"
            "            Default = \"{ {1,2}, {3,4}, {5,6}}\"\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n";

    multiCFG.Seek(0);
    ConfigurationDatabase cdbMulti;

    StreamString error;
    StandardParser parser(multiCFG, cdbMulti, &error);
    if (!parser.Parse()) {
        printf("\n%s\n", error.Buffer());
        return false;
    }

    ReferenceT<PIDGAM3> pid = ReferenceT<PIDGAM3>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(cdbMulti)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("state1", "thread1");
    if (!pid->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> defOut = pid->Find("Outputs.Control");

    ReferenceT<MemoryMapOutputWriter> writer = ReferenceT<MemoryMapOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(*rtapp.operator ->());

    if (!writer->AddSignal(defOut)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    ControlInMatrix *pidControl = (ControlInMatrix *) writer->GetSignal(0);

    pidControl->Pars[0][0] = 10;
    pidControl->Pars[0][1] = 20;
    pidControl->Pars[1][0] = 30;
    pidControl->Pars[1][1] = 40;
    pidControl->Pars[2][0] = 50;
    pidControl->Pars[2][1] = 60;

    ReferenceT<GAMGenericSignal> defIn = pid->Find("Inputs.Error");

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddSignal(defIn)) {
        return false;
    }
    if (!reader->Finalise()) {
        return false;
    }

    const char8 *nextState = "state1";
    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);

    if (!writer->Write(0)) {
        return false;
    }

    if (!reader->Read(0)) {
        return false;
    }

    ControlInMatrix *controlPlant = (ControlInMatrix *) reader->GetSignal(0);

    // test if the deafult value are read
    if (controlPlant->Pars[0][0] != 10) {
        return false;
    }

    if (controlPlant->Pars[0][1] != 20) {
        return false;
    }

    if (controlPlant->Pars[1][0] != 30) {
        return false;
    }

    if (controlPlant->Pars[1][1] != 40) {
        return false;
    }

    if (controlPlant->Pars[2][0] != 50) {
        return false;
    }

    if (controlPlant->Pars[2][1] != 60) {
        return false;
    }

    return true;

}

bool MemoryMapInputReaderTest::TestRead_Blocks() {

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    StreamString multiCFG = "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = uint32\n"
            "        IsFinal = true\n"
            "        Dimensions = \"[4][2]\"\n"
            "        Default = \"{ {1,2}, {3,4}, {5,6}, {7,8}}\"\n"
            "        Path = DDB1.PidControlArray\n"
            "    }\n"
            "}\n"
            "+Outputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsOutput = true\n"
            "    IsFinal = false\n"
            "    +Control = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = ControlInMatrix\n"
            "        IsFinal = true\n"
            "        +Pars = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Dimensions = \"[3][2]\"\n"
            "            Operation = \"{{0,1},{6,7}, {6,7}}\"" // read the first and the last 2 times
            "            Path = DDB1.PidControlArray\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n";

    multiCFG.Seek(0);
    ConfigurationDatabase cdbMulti;

    StreamString error;
    StandardParser parser(multiCFG, cdbMulti, &error);
    if (!parser.Parse()) {
        printf("\n%s\n", error.Buffer());
        return false;
    }

    ReferenceT<PIDGAM3> pid = ReferenceT<PIDGAM3>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(cdbMulti)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("state1", "thread1");
    if (!pid->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "state1";
    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);

    ReferenceT<GAMGenericSignal> defIn = pid->Find("Outputs.Control");

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddSignal(defIn)) {
        return false;
    }
    if (!reader->Finalise()) {
        return false;
    }

    if (!reader->Read(0)) {
        return false;
    }

    ControlInMatrix *controlPlant = (ControlInMatrix *) reader->GetSignal(0);

    // test if the deafult value are read
    if (controlPlant->Pars[0][0] != 1) {
        return false;
    }

    if (controlPlant->Pars[0][1] != 2) {
        return false;
    }

    if (controlPlant->Pars[1][0] != 7) {
        return false;
    }

    if (controlPlant->Pars[1][1] != 8) {
        return false;
    }

    if (controlPlant->Pars[2][0] != 7) {
        return false;
    }

    if (controlPlant->Pars[2][1] != 8) {
        return false;
    }

    return true;
}


static void WriteData(WriteParam &arg) {
    while (*arg.spinlock == 0) {
        Sleep::MSec(1);
    }
    for (uint32 i = 0u; i < (arg.numberOfWrites); i++) {
        uint32 *control = (uint32 *) arg.writer->GetSignal(0);
        control[0] = i;
        control[1] = i;
        control[2] = i;
        arg.writer->Write(0);
        Sleep::MSec(200);
    }
    Threads::EndThread();
}

static void ReadData(ReadParam &arg) {

    for (uint32 i = 0u; i < arg.numberOfReads; i++) {
        arg.reader->Read(0);
        uint32 *error = (uint32 *) arg.reader->GetSignal(0);
        arg.retVal &= (error[0] == i);
        arg.retVal &= (error[1] == i);
        arg.retVal &= (error[2] == i);
        arg.retVal &= (error[3] == 0);
        printf("\nnum= %d val=%d\n", i, error[0]);
    }
    Threads::EndThread();
}

bool MemoryMapInputReaderTest::TestSynchronisedRead() {

    StreamString appCFG = ""
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +Driver = {\n"
            "        Class = DataSource\n"
            "        +DriverSignal = {\n"
            "            Class = SyncDataSourceSignal\n"
            "            Type = uint32\n"
            "            Dimensions = \"[8]\"\n"
            "            Samples=1\n"
            "            Default= \"{0,0,0,0,0,0,0,0}\""
            "         }\n"
            "    }\n"
            "}\n"
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }
    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    StreamString multiCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = uint32\n"
            "        Dimensions = \"[2][2]\"\n"
            "        Path = \"Driver.DriverSignal\"\n"
            "        IsFinal = true\n"
            "        Cycles = 1\n"
            "     }\n"
            "}\n"
            "+Outputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsOutput = true\n"
            "    IsFinal = false\n"
            "    +Control = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = uint32\n"
            "        Dimensions = \"[3]\"\n"
            "        Path = \"Driver.DriverSignal\"\n"
            "        IsFinal = true\n"
            "    }\n"
            "}\n";

    multiCFG.Seek(0);
    ConfigurationDatabase cdbMulti;

    StreamString error;
    StandardParser parser(multiCFG, cdbMulti, &error);
    if (!parser.Parse()) {
        printf("\n%s\n", error.Buffer());
        return false;
    }

    ReferenceT<PIDGAM3> pid = ReferenceT<PIDGAM3>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(cdbMulti)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("state1", "thread1");
    if (!pid->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "state1";
    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);

    ReferenceT<GAMGenericSignal> defOut = pid->Find("Outputs.Control");

    ReferenceT<MemoryMapOutputWriter> writer = ReferenceT<MemoryMapOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(*rtapp.operator ->());

    if (!writer->AddSignal(defOut)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> defIn = pid->Find("Inputs.Error");

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddSignal(defIn)) {
        return false;
    }
    if (!reader->Finalise()) {
        return false;
    }

    volatile int32 spinLock = 0;

    uint32 nOp = 5;
    //volatile int32 spin = 0;
    WriteParam paramWrite = { writer, nOp, &spinLock, true };
    ReadParam paramRead = { reader, nOp, &spinLock, true };

    Threads::BeginThread((ThreadFunctionType) (WriteData), &paramWrite);
    Threads::BeginThread((ThreadFunctionType) (ReadData), &paramRead);

    Sleep::MSec(10);
    Atomic::Increment(&spinLock);
    while (Threads::NumberOfThreads() > 0) {
        Sleep::MSec(10);
    }
    return paramRead.retVal;
}

bool MemoryMapInputReaderTest::TestSynchronisedRead_LastSample() {

    StreamString appCFG = ""
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +Driver = {\n"
            "        Class = DataSource\n"
            "        +DriverSignal = {\n"
            "            Class = SyncDataSourceSignal\n"
            "            Type = uint32\n"
            "            Dimensions = \"[8]\"\n"
            "            Samples=2\n"
            "            Default= \"{0,0,0,0,0,0,0,0}\"" // the same initialisation for each sample
            "         }\n"
            "    }\n"
            "}\n"
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }
    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    StreamString multiCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = uint32\n"
            "        Dimensions = \"[2][2]\"\n"
            "        Path = \"Driver.DriverSignal\"\n"
            "        IsFinal = true\n"
            "        Cycles = 1\n"
            "     }\n"
            "}\n"
            "+Outputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsOutput = true\n"
            "    IsFinal = false\n"
            "    +Control = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = uint32\n"
            "        Dimensions = \"[3]\"\n"
            "        Path = \"Driver.DriverSignal\"\n"
            "        IsFinal = true\n"
            "    }\n"
            "}\n";

    multiCFG.Seek(0);
    ConfigurationDatabase cdbMulti;

    StreamString error;
    StandardParser parser(multiCFG, cdbMulti, &error);
    if (!parser.Parse()) {
        printf("\n%s\n", error.Buffer());
        return false;
    }

    ReferenceT<PIDGAM3> pid = ReferenceT<PIDGAM3>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(cdbMulti)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("state1", "thread1");
    if (!pid->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "state1";
    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);

    ReferenceT<GAMGenericSignal> defOut = pid->Find("Outputs.Control");

    ReferenceT<MemoryMapOutputWriter> writer = ReferenceT<MemoryMapOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(*rtapp.operator ->());

    if (!writer->AddSignal(defOut)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> defIn = pid->Find("Inputs.Error");

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddSignal(defIn)) {
        return false;
    }
    if (!reader->Finalise()) {
        return false;
    }

    volatile int32 spinLock = 0;

    uint32 nOp = 5;
    //volatile int32 spin = 0;
    WriteParam paramWrite = { writer, nOp, &spinLock, true };
    ReadParam paramRead = { reader, nOp, &spinLock, true };

    Threads::BeginThread((ThreadFunctionType) (WriteData), &paramWrite);
    Threads::BeginThread((ThreadFunctionType) (ReadData), &paramRead);

    Sleep::MSec(10);
    Atomic::Increment(&spinLock);
    while (Threads::NumberOfThreads() > 0) {
        Sleep::MSec(10);
    }
    return paramRead.retVal;
}

static void WriteDataSample(WriteParam &arg) {
    while (*arg.spinlock == 0) {
        Sleep::MSec(1);
    }

    for (uint32 i = 0u; i < (arg.numberOfWrites); i++) {
        uint32 *control = (uint32 *) arg.writer->GetSignal(0);
        control[0] = i;
        control[1] = i;
        control[2] = 2 * i;
        control[3] = 2 * i;
        control[4] = 3 * i;
        control[5] = 3 * i;
        arg.writer->Write(0);
        while (*arg.spinlock < 2) {
            Sleep::MSec(10);
        }
        arg.writer->Write(0);
        while (*arg.spinlock > 1) {
            Sleep::MSec(10);
        }
        Sleep::MSec(200);
    }
    Threads::EndThread();
}

static void ReadDataSample2(ReadParam &arg) {

    for (uint32 i = 0u; i < arg.numberOfReads; i++) {
        while (*arg.spinlock > 1) {
            Sleep::MSec(5);
        }
        printf("\nt2 read\n");
        arg.reader->Read(0);
        printf("\nt2 end read\n");
        Atomic::Increment(arg.spinlock);
        uint32 *error = (uint32 *) arg.reader->GetSignal(0);
        arg.retVal &= (error[0] == 2 * i);
        arg.retVal &= (error[1] == 2 * i);
        printf("\nt2 num= %d val=%d\n", i, error[0]);
    }
    Threads::EndThread();
}

static void ReadDataSample1_3(ReadParam &arg) {

    for (uint32 i = 0u; i < arg.numberOfReads; i++) {

        while (*arg.spinlock < 2) {
            Sleep::MSec(5);
        }
        printf("\nt13 read\n");
        arg.reader->Read(0);
        printf("\nt13 end read\n");
        Atomic::Decrement(arg.spinlock);
        uint32 *error = (uint32 *) arg.reader->GetSignal(0);
        arg.retVal &= (error[0] == i);
        arg.retVal &= (error[1] == i);
        arg.retVal &= (error[2] == 3 * i);
        arg.retVal &= (error[3] == 3 * i);
        printf("\nt13 num= %d val=%d\n", i, error[0]);
    }
    Threads::EndThread();
}

bool MemoryMapInputReaderTest::TestSynchronisedRead_SampleBlocks() {

    StreamString appCFG = ""
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +Driver = {\n"
            "        Class = DataSource\n"
            "        +DriverSignal = {\n"
            "            Class = SyncDataSourceSignal\n"
            "            Type = uint32\n"
            "            Dimensions = \"[2]\"\n"
            "            Samples=3\n"
            "            Default= \"{0,0}\"" // the same initialisation for each sample
            "         }\n"
            "    }\n"
            "}\n"
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }
    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    printf("\n??\n");
    StreamString multiCFG = ""
            "+Input1 = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMSampledSignal\n"
            "        Type = uint32\n"
            "        Dimensions = \"[2]\"\n"
            "        Path = \"Driver.DriverSignal\"\n"
            "        IsFinal = true\n"
            "        Samples = \"{{2,2,3}}\"" // the middle one
            "        Cycles = 1\n"
            "     }\n"
            "}\n"
            "+Input2 = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsOutput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMSampledSignal\n"
            "        Type = uint32\n"
            "        Dimensions = \"[2]\"\n"
            "        Path = \"Driver.DriverSignal\"\n"
            "        IsFinal = true\n"
            "        Samples = \"{{1,1,1},{0,0,1}}\""// the first one and the last one
            "        Cycles = 1\n"
            "    }\n"
            "}\n"
            "+Output = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Control = {\n"
            "        Class = GAMSampledSignal\n"
            "        Type = uint32\n"
            "        Dimensions = \"[2]\"\n"
            "        Path = \"Driver.DriverSignal\"\n"
            "        IsFinal = true\n"
            "        Samples = \"{{1,0,1}}\""// all of them
            "     }\n"
            "}\n";

    multiCFG.Seek(0);
    ConfigurationDatabase cdbMulti;

    StreamString error;
    StandardParser parser(multiCFG, cdbMulti, &error);
    if (!parser.Parse()) {
        printf("\n%s\n", error.Buffer());
        return false;
    }

    ReferenceT<PIDGAM3> pid = ReferenceT<PIDGAM3>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(cdbMulti)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("state1", "thread1");
    if (!pid->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "state1";
    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);

    ReferenceT<GAMSampledSignal> defOut = pid->Find("Output.Control");

    ReferenceT<MemoryMapOutputWriter> writer = ReferenceT<MemoryMapOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(*rtapp.operator ->());

    if (!writer->AddSignal(defOut)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    ReferenceT<GAMSampledSignal> defIn1 = pid->Find("Input1.Error");

    ReferenceT<MemoryMapInputReader> reader1 = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader1->SetName("reader1");

    reader1->SetApplication(*rtapp.operator ->());

    if (!reader1->AddSignal(defIn1)) {
        return false;
    }
    if (!reader1->Finalise()) {
        return false;
    }

    ReferenceT<GAMSampledSignal> defIn2 = pid->Find("Input2.Error");

    ReferenceT<MemoryMapInputReader> reader2 = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader2->SetName("reader2");

    reader2->SetApplication(*rtapp.operator ->());

    if (!reader2->AddSignal(defIn2)) {
        return false;
    }
    if (!reader2->Finalise()) {
        return false;
    }

    volatile int32 spinLock = 0;

    uint32 nOp = 5;
    //volatile int32 spin = 0;
    WriteParam paramWrite = { writer, nOp, &spinLock, true };
    ReadParam paramRead1 = { reader1, nOp, &spinLock, true };
    ReadParam paramRead2 = { reader2, nOp, &spinLock, true };

    Threads::BeginThread((ThreadFunctionType) (WriteDataSample), &paramWrite);
    Threads::BeginThread((ThreadFunctionType) (ReadDataSample2), &paramRead1);
    Threads::BeginThread((ThreadFunctionType) (ReadDataSample1_3), &paramRead2);

    Sleep::MSec(10);
    Atomic::Increment(&spinLock);
    while (Threads::NumberOfThreads() > 0) {
        Sleep::MSec(10);
    }
    return (paramRead1.retVal) && (paramRead2.retVal);
}

static void ReadDataSample(ReadParam &arg) {

    for (uint32 i = 0u; i < arg.numberOfReads; i += 3) {
        arg.reader->Read(0);
        uint32 *error = (uint32 *) arg.reader->GetSignal(0);
        // 3 cycles
        arg.retVal &= (error[0] == 2 * i);
        arg.retVal &= (error[1] == 2 * i);
        arg.retVal &= (error[2] == 2 * (i + 1));
        arg.retVal &= (error[3] == 2 * (i + 1));
        arg.retVal &= (error[4] == 2 * (i + 2));
        arg.retVal &= (error[5] == 2 * (i + 2));
        printf("\nt2 num= %d val=%d\n", i, error[0]);
    }
    Threads::EndThread();
}

static void WriteDataSampleNoWait(WriteParam &arg) {
    while (*arg.spinlock == 0) {
        Sleep::MSec(1);
    }

    for (uint32 i = 0u; i < (arg.numberOfWrites); i++) {
        uint32 *control = (uint32 *) arg.writer->GetSignal(0);
        control[0] = i;
        control[1] = i;
        control[2] = 2 * i;
        control[3] = 2 * i;
        control[4] = 3 * i;
        control[5] = 3 * i;
        arg.writer->Write(0);
        Sleep::MSec(200);
    }
    Threads::EndThread();
}

bool MemoryMapInputReaderTest::TestSynchronisedRead_MoreCycles() {

    StreamString appCFG = ""
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +Driver = {\n"
            "        Class = DataSource\n"
            "        +DriverSignal = {\n"
            "            Class = SyncDataSourceSignal\n"
            "            Type = uint32\n"
            "            Dimensions = \"[2]\"\n"
            "            Samples=3\n"
            "            Default= \"{0,0}\"" // the same initialisation for each sample
            "         }\n"
            "    }\n"
            "}\n"
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }
    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    printf("\n??\n");
    StreamString multiCFG = ""
            "+Input1 = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMSampledSignal\n"
            "        Type = uint32\n"
            "        Dimensions = \"[2]\"\n"
            "        Path = \"Driver.DriverSignal\"\n"
            "        IsFinal = true\n"
            "        Samples = \"{{2,2,3}}\"" // the middle one
            "        Cycles = 3\n"
            "     }\n"
            "}\n"
            "+Output = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Control = {\n"
            "        Class = GAMSampledSignal\n"
            "        Type = uint32\n"
            "        Dimensions = \"[2]\"\n"
            "        Path = \"Driver.DriverSignal\"\n"
            "        IsFinal = true\n"
            "        Samples = \"{{1,0,1}}\""// all of them
            "     }\n"
            "}\n";

    multiCFG.Seek(0);
    ConfigurationDatabase cdbMulti;

    StreamString error;
    StandardParser parser(multiCFG, cdbMulti, &error);
    if (!parser.Parse()) {
        printf("\n%s\n", error.Buffer());
        return false;
    }

    ReferenceT<PIDGAM3> pid = ReferenceT<PIDGAM3>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(cdbMulti)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("state1", "thread1");
    if (!pid->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "state1";
    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);

    ReferenceT<GAMSampledSignal> defOut = pid->Find("Output.Control");

    ReferenceT<MemoryMapOutputWriter> writer = ReferenceT<MemoryMapOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(*rtapp.operator ->());

    if (!writer->AddSignal(defOut)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    ReferenceT<GAMSampledSignal> defIn1 = pid->Find("Input1.Error");

    ReferenceT<MemoryMapInputReader> reader1 = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader1->SetName("reader1");

    reader1->SetApplication(*rtapp.operator ->());

    if (!reader1->AddSignal(defIn1)) {
        return false;
    }
    if (!reader1->Finalise()) {
        return false;
    }

    volatile int32 spinLock = 0;

    uint32 nOp = 9;
    //volatile int32 spin = 0;
    WriteParam paramWrite = { writer, nOp, &spinLock, true };
    ReadParam paramRead1 = { reader1, nOp, &spinLock, true };

    Threads::BeginThread((ThreadFunctionType) (WriteDataSampleNoWait), &paramWrite);
    Threads::BeginThread((ThreadFunctionType) (ReadDataSample), &paramRead1);

    Sleep::MSec(10);
    Atomic::Increment(&spinLock);
    while (Threads::NumberOfThreads() > 0) {
        Sleep::MSec(10);
    }
    return (paramRead1.retVal);
}

static void ReadDataSampleAndBlock(ReadParam &arg) {

    for (uint32 i = 0u; i < arg.numberOfReads; i += 2) {
        arg.reader->Read(0);
        uint32 *error = (uint32 *) arg.reader->GetSignal(0);
        // 2 cycles

        // first cycle first sample
        arg.retVal &= (error[0] == i);
        arg.retVal &= (error[1] == 2 * i);
        arg.retVal &= (error[2] == 2 * i);
        arg.retVal &= (error[3] == 4 * i);

        // first cycle last sample
        arg.retVal &= (error[4] == 9 * i);
        arg.retVal &= (error[5] == 10 * i);
        arg.retVal &= (error[6] == 10 * i);
        arg.retVal &= (error[7] == 12 * i);

        // second cycle first sample
        arg.retVal &= (error[8] == (i + 1));
        arg.retVal &= (error[9] == 2 * (i + 1));
        arg.retVal &= (error[10] == 2 * (i + 1));
        arg.retVal &= (error[11] == 4 * (i + 1));

        // second cycle last sample
        arg.retVal &= (error[12] == 9 * (i + 1));
        arg.retVal &= (error[13] == 10 * (i + 1));
        arg.retVal &= (error[14] == 10 * (i + 1));
        arg.retVal &= (error[15] == 12 * (i + 1));
        printf("\nt2 num= %d val=%d ret=%d\n", 8 * i, error[4], arg.retVal);
    }
    Threads::EndThread();
}

static void WriteDataSampleAndBlock(WriteParam &arg) {
    while (*arg.spinlock == 0) {
        Sleep::MSec(1);
    }

    for (uint32 i = 0u; i < (arg.numberOfWrites); i++) {
        uint32 *control = (uint32 *) arg.writer->GetSignal(0);
        uint32 n = 1u;
        for (uint32 j = 0u; j < 24; j += 2) {
            control[j] = (n * i);
            control[j + 1] = (n * i);
            n++;
        }
        arg.writer->Write(0);
        Sleep::MSec(200);
    }
    Threads::EndThread();
}

bool MemoryMapInputReaderTest::TestSynchronisedRead_SampleAndElementBlocks() {
    StreamString appCFG = ""
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +Driver = {\n"
            "        Class = DataSource\n"
            "        +DriverSignal = {\n"
            "            Class = SyncDataSourceSignal\n"
            "            Type = uint32\n"
            "            Dimensions = \"[8]\"\n"
            "            Samples=3\n"
            "            Default= \"{0,0,0,0,0,0,0,0}\"" // the same initialisation for each sample
            "         }\n"
            "    }\n"
            "}\n"
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }
    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    printf("\n??\n");
    StreamString multiCFG = ""
            "+Input1 = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMSampledSignal\n"
            "        Type = uint32\n"
            "        Dimensions = \"[6]\"\n"
            "        Path = \"Driver.DriverSignal\"\n"
            "        IsFinal = true\n"
            "        Samples = \"{{1,1,1},{0,0,1}}\"\n" // the first and the last
            "        Operation = \"{{1,3}{6,6}}\"\n"
            "        Cycles = 2\n"
            "     }\n"
            "}\n"
            "+Output = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Control = {\n"
            "        Class = GAMSampledSignal\n"
            "        Type = uint32\n"
            "        Dimensions = \"[8]\"\n"
            "        Path = \"Driver.DriverSignal\"\n"
            "        IsFinal = true\n"
            "        Samples = \"{{1,0,1}}\""// all of them
            "     }\n"
            "}\n";

    multiCFG.Seek(0);
    ConfigurationDatabase cdbMulti;

    StreamString error;
    StandardParser parser(multiCFG, cdbMulti, &error);
    if (!parser.Parse()) {
        printf("\n%s\n", error.Buffer());
        return false;
    }

    ReferenceT<PIDGAM3> pid = ReferenceT<PIDGAM3>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(cdbMulti)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("state1", "thread1");
    if (!pid->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "state1";
    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);

    ReferenceT<GAMSampledSignal> defOut = pid->Find("Output.Control");

    ReferenceT<MemoryMapOutputWriter> writer = ReferenceT<MemoryMapOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(*rtapp.operator ->());

    if (!writer->AddSignal(defOut)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    ReferenceT<GAMSampledSignal> defIn1 = pid->Find("Input1.Error");

    ReferenceT<MemoryMapInputReader> reader1 = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader1->SetName("reader1");

    reader1->SetApplication(*rtapp.operator ->());

    if (!reader1->AddSignal(defIn1)) {
        return false;
    }
    if (!reader1->Finalise()) {
        return false;
    }

    volatile int32 spinLock = 0;

    uint32 nOp = 6;
    //volatile int32 spin = 0;
    WriteParam paramWrite = { writer, nOp, &spinLock, true };
    ReadParam paramRead1 = { reader1, nOp, &spinLock, true };

    Threads::BeginThread((ThreadFunctionType) (WriteDataSampleAndBlock), &paramWrite);
    Threads::BeginThread((ThreadFunctionType) (ReadDataSampleAndBlock), &paramRead1);

    Sleep::MSec(10);
    Atomic::Increment(&spinLock);
    while (Threads::NumberOfThreads() > 0) {
        Sleep::MSec(10);
    }
    return (paramRead1.retVal);
}
