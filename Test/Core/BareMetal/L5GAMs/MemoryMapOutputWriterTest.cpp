/**
 * @file MemoryMapOutputWriterTest.cpp
 * @brief Source file for class MemoryMapOutputWriterTest
 * @date 20/apr/2016
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
 * the class MemoryMapOutputWriterTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryMapOutputWriterTest.h"
#include "GAMTestHelper.h"
#include "GAMGenericSignal.h"
#include "StandardParser.h"
#include "MemoryMapInputReader.h"
#include "Threads.h"
#include "GAMSampledSignal.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MemoryMapOutputWriterTest::MemoryMapOutputWriterTest() {
    StreamString gamCFG = ""
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
    gamCFG.Seek(0);

    StandardParser parser(gamCFG, gamCDB);
    parser.Parse();

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

    StandardParser parserPlant(plantCFG, plantCDB);
    parserPlant.Parse();

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

    StandardParser parserApp(appCFG, appCDB);
    parserApp.Parse();

}

bool MemoryMapOutputWriterTest::TestConstructor() {
    MemoryMapOutputWriter test;
    return true;
}

bool MemoryMapOutputWriterTest::TestWrite_Allocation() {

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> pid = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(gamCDB)) {
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

bool MemoryMapOutputWriterTest::TestWrite_Static() {

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> pid = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(gamCDB)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("+state1", "thread1");
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

bool MemoryMapOutputWriterTest::TestWrite_MoreThanOneVariable() {

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PIDGAM> pid = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(gamCDB)) {
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

    ConfigurationDatabase plantCDB;

    plantCFG.Seek(0);
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

bool MemoryMapOutputWriterTest::TestWrite_MultiDim_Vector() {

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    StreamString cfgMulti = ""
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
    cfgMulti.Seek(0);
    ConfigurationDatabase cdbMulti;

    StandardParser parser(cfgMulti, cdbMulti);
    if (!parser.Parse()) {
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

bool MemoryMapOutputWriterTest::TestWrite_MultiDim_Matrix() {
    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    StreamString cfgMulti = ""
            "+Inputs = {\n"
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

    cfgMulti.Seek(0);
    ConfigurationDatabase cdbMulti;

    StreamString error;
    StandardParser parser(cfgMulti, cdbMulti, &error);

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

bool MemoryMapOutputWriterTest::TestWrite_Blocks() {

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
            "            Operation = \"{{0,1},{2,3}, {6,7}}\"" // write the first, the second and the last
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

    uint32 test[4][2];

    if (!reader->AddSignal(defIn, &test)) {
        return false;
    }
    if (!reader->Finalise()) {
        return false;
    }

    ControlInMatrix *controlPlant = (ControlInMatrix *) writer->GetSignal(0);

    // test if the deafult value are read
    controlPlant->Pars[0][0] = 0;

    controlPlant->Pars[0][1] = 0;

    controlPlant->Pars[1][0] = 0;

    controlPlant->Pars[1][1] = 0;

    controlPlant->Pars[2][0] = 0;

    controlPlant->Pars[2][1] = 0;

    if (!writer->Write(0)) {
        return false;
    }

    if (!reader->Read(0)) {
        return false;
    }

    // test if the deafult value are read
    if (test[0][0] != 0) {
        return false;
    }

    if (test[0][1] != 0) {
        return false;
    }

    if (test[1][0] != 0) {
        return false;
    }

    if (test[1][1] != 0) {
        return false;
    }

    if (test[2][0] != 5) {
        return false;
    }

    if (test[2][1] != 6) {
        return false;
    }

    if (test[3][0] != 0) {
        return false;
    }

    if (test[3][1] != 0) {
        return false;
    }

    return true;
}

static void WriteData(WriteParam &arg) {
    for (uint32 i = 0u; i < (arg.numberOfWrites); i++) {
        uint32 *control = (uint32 *) arg.writer->GetSignal(0);
        control[0] = i;
        control[1] = i;
        control[2] = i;
        // will block on the write until read is performed
        arg.writer->Write(0);
    }
    Threads::EndThread();
}

static void ReadData(ReadParam &arg) {
    while (*arg.spinlock == 0) {
        Sleep::MSec(1);
    }
    // unblocks the write
    arg.reader->Read(0);
    Sleep::MSec(200);

    for (uint32 i = 0u; i < arg.numberOfReads; i++) {
        arg.reader->Read(0);
        uint32 *error = (uint32 *) arg.reader->GetSignal(0);
        arg.retVal &= (error[0] == i);
        arg.retVal &= (error[1] == i);
        arg.retVal &= (error[2] == i);
        arg.retVal &= (error[3] == 0);
        Sleep::MSec(200);

        printf("\nnum= %d val=%d\n", i, error[0]);
    }
    Threads::EndThread();
}

bool MemoryMapOutputWriterTest::TestSynchronisedWrite() {

    StreamString appCFG = ""
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +Driver = {\n"
            "        Class = DataSource\n"
            "        +DriverSignal = {\n"
            "            Class = WriteSyncDataSourceSignal\n"
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
            "        Cycles = 1\n"
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

bool MemoryMapOutputWriterTest::TestSynchronisedWrite_LastSample() {
    StreamString appCFG = ""
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +Driver = {\n"
            "        Class = DataSource\n"
            "        +DriverSignal = {\n"
            "            Class = WriteSyncDataSourceSignal\n"
            "            Type = uint32\n"
            "            Dimensions = \"[8]\"\n"
            "            Samples=2\n"
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
            "        Cycles = 1\n"
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

static void ReadDataSample(ReadParam &arg) {
    while (*arg.spinlock == 0) {
        Sleep::MSec(1);
    }
    // unblocks the write 2
    arg.reader->Read(0);
    while (*arg.spinlock < 2) {
        Sleep::MSec(10);
    }
    // allow to block the write 1-3
    Sleep::MSec(100);
    // unblocks the write 1-3
    arg.reader->Read(0);
    while (*arg.spinlock > 1) {
        Sleep::MSec(10);
    }
    Sleep::MSec(100);

    for (uint32 i = 0u; i < (arg.numberOfReads - 1); i++) {

        //read
        arg.reader->Read(0);

        printf("\nchecking...\n");
        uint32 *control = (uint32 *) arg.reader->GetSignal(0);
        arg.retVal &= (control[0] == i);
        arg.retVal &= (control[1] == i);
        arg.retVal &= (control[2] == 2 * i);
        arg.retVal &= (control[3] == 2 * i);
        arg.retVal &= (control[4] == 3 * i);
        arg.retVal &= (control[5] == 3 * i);
        printf("\ntest=%d val=%d", i, control[0]);
        printf("\ntest=%d val=%d", i, control[1]);
        printf("\ntest=%d val=%d", 2 * i, control[2]);
        printf("\ntest=%d val=%d", 2 * i, control[3]);
        printf("\ntest=%d val=%d", 3 * i, control[4]);
        printf("\ntest=%d val=%d", 3 * i, control[5]);

        arg.reader->Read(0);
        printf("\nread the 2\n");
        while (*arg.spinlock < 2) {
            Sleep::MSec(10);
        }
        Sleep::MSec(100);
        printf("\nread the 1-3\n");
        arg.reader->Read(0);
        while (*arg.spinlock > 1) {
            Sleep::MSec(10);
        }
        Sleep::MSec(100);
    }
    Threads::EndThread();
}

static void WriteDataSample2(WriteParam &arg) {

    for (uint32 i = 0u; i < arg.numberOfWrites; i++) {

        uint32 *error = (uint32 *) arg.writer->GetSignal(0);
        error[0] = 2 * i;
        error[1] = 2 * i;
        printf("\nt2 write\n");
        arg.writer->Write(0);
        printf("\nt2 end write %d\n", i);
        Atomic::Increment(arg.spinlock);
        while (*arg.spinlock > 1) {
            Sleep::MSec(5);
        }
    }
    Threads::EndThread();
}

static void WriteDataSample1_3(WriteParam &arg) {

    for (uint32 i = 0u; i < arg.numberOfWrites; i++) {
        // wait the sample 2 write
        while (*arg.spinlock < 2) {
            Sleep::MSec(5);
        }
        uint32 *error = (uint32 *) arg.writer->GetSignal(0);
        error[0] = i;
        error[1] = i;
        error[2] = 3 * i;
        error[3] = 3 * i;
        printf("\nt13 write\n");
        arg.writer->Write(0);
        Atomic::Decrement(arg.spinlock);

        printf("\nt13 end write %d\n", i);

    }
    Threads::EndThread();
}

bool MemoryMapOutputWriterTest::TestSynchronisedWrite_SampleBlocks() {

    StreamString appCFG = ""
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +Driver = {\n"
            "        Class = DataSource\n"
            "        +DriverSignal = {\n"
            "            Class = WriteSyncDataSourceSignal\n"
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
            "+Output1 = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsOutput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMSampledSignal\n"
            "        Type = uint32\n"
            "        Dimensions = \"[2]\"\n"
            "        Path = \"Driver.DriverSignal\"\n"
            "        IsFinal = true\n"
            "        Samples = \"{{1,1,2}}\"" // the middle one
            "        Cycles = 1\n"
            "     }\n"
            "}\n"
            "+Output2 = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsOutput = false\n"
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
            "+Input = {\n"
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

    ReferenceT<GAMSampledSignal> defOut1 = pid->Find("Output1.Error");

    ReferenceT<MemoryMapOutputWriter> writer1 = ReferenceT<MemoryMapOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer1->SetName("writer1");

    writer1->SetApplication(*rtapp.operator ->());

    if (!writer1->AddSignal(defOut1)) {
        return false;
    }

    if (!writer1->Finalise()) {
        return false;
    }

    ReferenceT<GAMSampledSignal> defOut2 = pid->Find("Output2.Error");

    ReferenceT<MemoryMapOutputWriter> writer2 = ReferenceT<MemoryMapOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer2->SetName("writer2");

    writer2->SetApplication(*rtapp.operator ->());

    if (!writer2->AddSignal(defOut2)) {
        return false;
    }
    if (!writer2->Finalise()) {
        return false;
    }

    ReferenceT<GAMSampledSignal> defIn2 = pid->Find("Input.Control");

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddSignal(defIn2)) {
        return false;
    }
    if (!reader->Finalise()) {
        return false;
    }

    volatile int32 spinLock = 0;

    uint32 nOp = 5;
    //volatile int32 spin = 0;
    WriteParam paramWrite1 = { writer1, nOp, &spinLock, true };
    WriteParam paramWrite2 = { writer2, nOp, &spinLock, true };
    ReadParam paramRead = { reader, nOp, &spinLock, true };

    Threads::BeginThread((ThreadFunctionType) (WriteDataSample2), &paramWrite1);
    Threads::BeginThread((ThreadFunctionType) (WriteDataSample1_3), &paramWrite2);
    Threads::BeginThread((ThreadFunctionType) (ReadDataSample), &paramRead);

    Sleep::MSec(10);
    Atomic::Increment(&spinLock);
    while (Threads::NumberOfThreads() > 0) {
        Sleep::MSec(10);
    }
    return (paramRead.retVal);
}

static void WriteDataSample(WriteParam &arg) {

    for (uint32 i = 0u; i < arg.numberOfWrites; i += 3) {

        uint32 *error = (uint32 *) arg.writer->GetSignal(0);
        // 3 cycles
        error[0] = 2 * i;
        error[1] = 2 * i;
        error[2] = 2 * (i + 1);
        error[3] = 2 * (i + 1);
        error[4] = 2 * (i + 2);
        error[5] = 2 * (i + 2);
        arg.writer->Write(0);
        printf("\nEndWrite %d\n", i);
    }
    Threads::EndThread();
}

static void ReadDataSampleNoWait(ReadParam &arg) {
    while (*arg.spinlock == 0) {
        Sleep::MSec(1);
    }
    //unblocks the write
    arg.reader->Read(0);
    Sleep::MSec(200);

    for (uint32 i = 0u; i < (arg.numberOfReads); i++) {
        arg.reader->Read(0);

        uint32 *control = (uint32 *) arg.reader->GetSignal(0);
        arg.retVal &= (control[0] == 0);
        arg.retVal &= (control[1] == 0);
        arg.retVal &= (control[2] == 2 * i);
        arg.retVal &= (control[3] == 2 * i);
        arg.retVal &= (control[4] == 0 * i);
        arg.retVal &= (control[5] == 0 * i);
        printf("\nt2 num= %d val=%d\n", i, control[2]);

        Sleep::MSec(200);
    }
    Threads::EndThread();
}

bool MemoryMapOutputWriterTest::TestSynchronisedWrite_MoreCycles() {

    StreamString appCFG = ""
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +Driver = {\n"
            "        Class = DataSource\n"
            "        +DriverSignal = {\n"
            "            Class = WriteSyncDataSourceSignal\n"
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
            "        Samples = \"{{1,1,2}}\"" // the middle one
            "        Cycles = 3\n"
            "     }\n"
            "}\n"
            "+Input = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
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

    ReferenceT<GAMSampledSignal> defIn1 = pid->Find("Input.Error");

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddSignal(defIn1)) {
        return false;
    }
    if (!reader->Finalise()) {
        return false;
    }

    volatile int32 spinLock = 0;

    uint32 nOp = 9;
    //volatile int32 spin = 0;
    WriteParam paramWrite = { writer, nOp, &spinLock, true };
    ReadParam paramRead = { reader, nOp, &spinLock, true };

    Threads::BeginThread((ThreadFunctionType) (ReadDataSampleNoWait), &paramRead);
    Threads::BeginThread((ThreadFunctionType) (WriteDataSample), &paramWrite);

    Sleep::MSec(10);
    Atomic::Increment(&spinLock);
    while (Threads::NumberOfThreads() > 0) {
        Sleep::MSec(10);
    }
    return (paramRead.retVal);
}

static void WriteDataSampleAndBlock(WriteParam &arg) {

    for (uint32 i = 0u; i < arg.numberOfWrites; i += 2) {
        uint32 *error = (uint32 *) arg.writer->GetSignal(0);
        // 2 cycles

        // first cycle first sample
        error[0] = i;
        error[1] = 2 * i;
        error[2] = 2 * i;
        error[3] = 4 * i;

        // first cycle last sample
        error[4] = 9 * i;
        error[5] = 10 * i;
        error[6] = 10 * i;
        error[7] = 12 * i;

        // second cycle first sample
        error[8] = (i + 1);
        error[9] = 2 * (i + 1);
        error[10] = 2 * (i + 1);
        error[11] = 4 * (i + 1);

        // second cycle last sample
        error[12] = 9 * (i + 1);
        error[13] = 10 * (i + 1);
        error[14] = 10 * (i + 1);
        error[15] = 12 * (i + 1);

        arg.writer->Write(0);

    }
    Threads::EndThread();
}

static void ReadDataSampleAndBlock(ReadParam &arg) {
    while (*arg.spinlock == 0) {
        Sleep::MSec(1);
    }

    arg.reader->Read(0);
    Sleep::MSec(200);

    for (uint32 i = 0u; i < (arg.numberOfReads); i++) {
        arg.reader->Read(0);

        uint32 *error = (uint32 *) arg.reader->GetSignal(0);

        // first sample
        arg.retVal &= (error[0] == 0);
        arg.retVal &= (error[1] == i);
        arg.retVal &= (error[2] == 2 * i);
        arg.retVal &= (error[3] == 2 * i);
        arg.retVal &= (error[4] == 0);
        arg.retVal &= (error[5] == 0);
        arg.retVal &= (error[6] == 4 * i);
        arg.retVal &= (error[7] == 0);

        // second sample
        arg.retVal &= (error[8] == 0);
        arg.retVal &= (error[9] == 0);
        arg.retVal &= (error[10] == 0);
        arg.retVal &= (error[11] == 0);
        arg.retVal &= (error[12] == 0);
        arg.retVal &= (error[13] == 0);
        arg.retVal &= (error[14] == 0);
        arg.retVal &= (error[15] == 0);

        //third sample
        arg.retVal &= (error[16] == 0);
        arg.retVal &= (error[17] == 9 * i);
        arg.retVal &= (error[18] == 10 * i);
        arg.retVal &= (error[19] == 10 * i);
        arg.retVal &= (error[20] == 0);
        arg.retVal &= (error[21] == 0);
        arg.retVal &= (error[22] == 12 * i);
        arg.retVal &= (error[23] == 0);
        printf("\nt2 num= %d val=%d ret=%d\n",  i, error[0], arg.retVal);

        Sleep::MSec(200);
    }
    Threads::EndThread();
}

bool MemoryMapOutputWriterTest::TestSynchronisedWrite_SampleAndElementBlocks() {
    StreamString appCFG = ""
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +Driver = {\n"
            "        Class = DataSource\n"
            "        +DriverSignal = {\n"
            "            Class = WriteSyncDataSourceSignal\n"
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
            "+Output = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Control = {\n"
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
            "+Input = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
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

    ReferenceT<GAMSampledSignal> defIn1 = pid->Find("Input.Error");

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddSignal(defIn1)) {
        return false;
    }
    if (!reader->Finalise()) {
        return false;
    }

    volatile int32 spinLock = 0;

    uint32 nOp = 6;
    //volatile int32 spin = 0;
    WriteParam paramWrite = { writer, nOp, &spinLock, true };
    ReadParam paramRead1 = { reader, nOp, &spinLock, true };

    Threads::BeginThread((ThreadFunctionType) (WriteDataSampleAndBlock), &paramWrite);
    Threads::BeginThread((ThreadFunctionType) (ReadDataSampleAndBlock), &paramRead1);

    Sleep::MSec(10);
    Atomic::Increment(&spinLock);
    while (Threads::NumberOfThreads() > 0) {
        Sleep::MSec(10);
    }
    return (paramRead1.retVal);
}

