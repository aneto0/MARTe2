/**
 * @file BasicRealTimeDataSourceOutputWriterTest.cpp
 * @brief Source file for class BasicRealTimeDataSourceOutputWriterTest
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
 * the class BasicRealTimeDataSourceOutputWriterTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicRealTimeDataSourceOutputWriterTest.h"
#include "GAMTestHelper.h"
#include "RealTimeGenericDataDef.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

BasicRealTimeDataSourceOutputWriterTest::BasicRealTimeDataSourceOutputWriterTest() {
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

bool BasicRealTimeDataSourceOutputWriterTest::TestConstructor() {
    BasicRealTimeDataSourceOutputWriter test;
    return true;
}

bool BasicRealTimeDataSourceOutputWriterTest::TestWrite_Allocation() {

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

    ReferenceT<PIDGAM> pid = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(cdb)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("+state1");
    if (!pid->ConfigureDataSource()) {
        return false;
    }

    ConfigurationDatabase plantCDB;

    plantCDB.CreateAbsolute("+Inputs");
    plantCDB.Write("Class", "RealTimeDataDefContainer");
    plantCDB.Write("IsInput", "true");
    plantCDB.Write("IsFinal", "false");
    plantCDB.CreateAbsolute("+Inputs.+Control");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "TrackError");
    plantCDB.Write("IsFinal", "true");
    plantCDB.CreateAbsolute("+Inputs.+Control.+Par1");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "uint32");
    plantCDB.Write("Default", "2");
    plantCDB.Write("Path", "+DDB2.PidControl1");
    plantCDB.Write("IsFinal", "true");
    plantCDB.CreateAbsolute("+Inputs.+Control.+Par2");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "uint32");
    plantCDB.Write("Default", "2");
    plantCDB.Write("Path", "+DDB2.PidControl2");
    plantCDB.Write("IsFinal", "true");

    plantCDB.CreateAbsolute("+Outputs");
    plantCDB.Write("Class", "RealTimeDataDefContainer");
    plantCDB.Write("IsOutput", "true");
    plantCDB.Write("IsFinal", "true");
    plantCDB.CreateAbsolute("+Outputs.+Error");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "TrackError");
    plantCDB.Write("IsFinal", "false");
    plantCDB.CreateAbsolute("+Outputs.+Error.+Par1");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "uint32");
    plantCDB.Write("Default", "1");
    plantCDB.Write("Path", "+DDB1.PidError1");
    plantCDB.Write("IsFinal", "true");
    plantCDB.CreateAbsolute("+Outputs.+Error.+Par2");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "uint32");
    plantCDB.Write("Default", "2");
    plantCDB.Write("Path", "+DDB1.PidError2");
    plantCDB.Write("IsFinal", "true");
    plantCDB.MoveToRoot();

    ReferenceT<PlantGAM> plant = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    plant->SetName("plant");
    if (!plant->Initialise(plantCDB)) {
        return false;
    }

    plant->SetApplication(*rtapp.operator ->());
    plant->AddState("+state1");
    if (!plant->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def = pid->Find("+Inputs.+Error");

    ReferenceT<BasicRealTimeDataSourceInputReader> reader = ReferenceT<BasicRealTimeDataSourceInputReader>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddVariable(def)) {
        return false;
    }

    if (!reader->Finalise()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def2 = plant->Find("+Outputs.+Error");

    ReferenceT<BasicRealTimeDataSourceOutputWriter> writer = ReferenceT<BasicRealTimeDataSourceOutputWriter>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(*rtapp.operator ->());

    if (!writer->AddVariable(def2)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    TrackError *errorPlant = (TrackError *) writer->GetData(0);
    errorPlant->Par1 = 10;
    errorPlant->Par2 = 20;

    const char8 *nextState = "+state1";
    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);

    if (!writer->Write(0)) {
        return false;
    }

    if (!reader->Read(0)) {
        return false;
    }

    TrackError *errorPID;
    errorPID = (TrackError *) reader->GetData(0);

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

bool BasicRealTimeDataSourceOutputWriterTest::TestWrite_Static() {

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

    ReferenceT<PIDGAM> pid = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(cdb)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("+state1");
    if (!pid->ConfigureDataSource()) {
        return false;
    }

    ConfigurationDatabase plantCDB;

    plantCDB.CreateAbsolute("+Inputs");
    plantCDB.Write("Class", "RealTimeDataDefContainer");
    plantCDB.Write("IsInput", "true");
    plantCDB.Write("IsFinal", "false");
    plantCDB.CreateAbsolute("+Inputs.+Control");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "TrackError");
    plantCDB.Write("IsFinal", "true");
    plantCDB.CreateAbsolute("+Inputs.+Control.+Par1");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "uint32");
    plantCDB.Write("Default", "2");
    plantCDB.Write("Path", "+DDB2.PidControl1");
    plantCDB.Write("IsFinal", "true");
    plantCDB.CreateAbsolute("+Inputs.+Control.+Par2");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "uint32");
    plantCDB.Write("Default", "2");
    plantCDB.Write("Path", "+DDB2.PidControl2");
    plantCDB.Write("IsFinal", "true");

    plantCDB.CreateAbsolute("+Outputs");
    plantCDB.Write("Class", "RealTimeDataDefContainer");
    plantCDB.Write("IsOutput", "true");
    plantCDB.Write("IsFinal", "true");
    plantCDB.CreateAbsolute("+Outputs.+Error");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "TrackError");
    plantCDB.Write("IsFinal", "false");
    plantCDB.CreateAbsolute("+Outputs.+Error.+Par1");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "uint32");
    plantCDB.Write("Default", "1");
    plantCDB.Write("Path", "+DDB1.PidError1");
    plantCDB.Write("IsFinal", "true");
    plantCDB.CreateAbsolute("+Outputs.+Error.+Par2");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "uint32");
    plantCDB.Write("Default", "2");
    plantCDB.Write("Path", "+DDB1.PidError2");
    plantCDB.Write("IsFinal", "true");
    plantCDB.MoveToRoot();

    ReferenceT<PlantGAM> plant = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    plant->SetName("plant");
    if (!plant->Initialise(plantCDB)) {
        return false;
    }

    plant->SetApplication(*rtapp.operator ->());
    plant->AddState("+state1");
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

    ReferenceT<RealTimeGenericDataDef> def = pid->Find("+Inputs.+Error");

    ReferenceT<BasicRealTimeDataSourceInputReader> reader = ReferenceT<BasicRealTimeDataSourceInputReader>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddVariable(def, &errorPID)) {
        return false;
    }

    if (!reader->Finalise()) {
        return false;
    }

    TrackError errorPlant;
    errorPlant.Par1 = 10;
    errorPlant.Par2 = 20;

    ReferenceT<RealTimeGenericDataDef> def2 = plant->Find("+Outputs.+Error");

    ReferenceT<BasicRealTimeDataSourceOutputWriter> writer = ReferenceT<BasicRealTimeDataSourceOutputWriter>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(*rtapp.operator ->());

    if (!writer->AddVariable(def2, &errorPlant)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    const char8 *nextState = "+state1";
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

bool BasicRealTimeDataSourceOutputWriterTest::TestWrite_MoreThanOneVariable() {
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

    ReferenceT<PIDGAM> pid = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(cdb)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("+state1");
    if (!pid->ConfigureDataSource()) {
        return false;
    }

    ConfigurationDatabase plantCDB;

    plantCDB.CreateAbsolute("+Inputs");
    plantCDB.Write("Class", "RealTimeDataDefContainer");
    plantCDB.Write("IsInput", "true");
    plantCDB.Write("IsFinal", "false");
    plantCDB.CreateAbsolute("+Inputs.+Control");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "TrackError");
    plantCDB.Write("IsFinal", "true");
    plantCDB.CreateAbsolute("+Inputs.+Control.+Par1");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "uint32");
    plantCDB.Write("Default", "2");
    plantCDB.Write("Path", "+DDB2.PidControl1");
    plantCDB.Write("IsFinal", "true");
    plantCDB.CreateAbsolute("+Inputs.+Control.+Par2");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "uint32");
    plantCDB.Write("Default", "2");
    plantCDB.Write("Path", "+DDB2.PidControl2");
    plantCDB.Write("IsFinal", "true");
    plantCDB.CreateAbsolute("+Inputs.+Noise");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "ControlNoise");
    plantCDB.Write("IsFinal", "true");
    plantCDB.CreateAbsolute("+Inputs.+Noise.+noiseValue");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "float32");
    plantCDB.Write("Default", "2");
    plantCDB.Write("Path", "+DDB2.PidNoise");
    plantCDB.Write("IsFinal", "true");

    plantCDB.CreateAbsolute("+Outputs");
    plantCDB.Write("Class", "RealTimeDataDefContainer");
    plantCDB.Write("IsOutput", "true");
    plantCDB.Write("IsFinal", "true");
    plantCDB.CreateAbsolute("+Outputs.+Error");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "TrackError");
    plantCDB.Write("IsFinal", "false");
    plantCDB.CreateAbsolute("+Outputs.+Error.+Par1");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "uint32");
    plantCDB.Write("Default", "1");
    plantCDB.Write("Path", "+DDB1.PidError1");
    plantCDB.Write("IsFinal", "true");
    plantCDB.CreateAbsolute("+Outputs.+Error.+Par2");
    plantCDB.Write("Class", "RealTimeGenericDataDef");
    plantCDB.Write("Type", "uint32");
    plantCDB.Write("Default", "2");
    plantCDB.Write("Path", "+DDB1.PidError2");
    plantCDB.Write("IsFinal", "true");
    plantCDB.MoveToRoot();

    ReferenceT<PlantGAM> plant = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    plant->SetName("plant");
    if (!plant->Initialise(plantCDB)) {
        return false;
    }

    plant->SetApplication(*rtapp.operator ->());
    plant->AddState("+state1");
    if (!plant->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def1 = pid->Find("+Outputs.+Control");
    ReferenceT<RealTimeGenericDataDef> def2 = pid->Find("+Outputs.+Noise");

    ReferenceT<BasicRealTimeDataSourceOutputWriter> writer = ReferenceT<BasicRealTimeDataSourceOutputWriter>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(*rtapp.operator ->());

    if (!writer->AddVariable(def1)) {
        return false;
    }
    if (!writer->AddVariable(def2)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    ControlIn *pidControl = (ControlIn *) writer->GetData(0);
    ControlNoise *pidNoise = (ControlNoise *) writer->GetData(1);

    pidControl->Par1 = 10;
    pidControl->Par2 = 20;
    pidNoise->noiseValue = 0.5;

    ReferenceT<RealTimeGenericDataDef> def3 = plant->Find("+Inputs.+Control");
    ReferenceT<RealTimeGenericDataDef> def4 = plant->Find("+Inputs.+Noise");

    ReferenceT<BasicRealTimeDataSourceInputReader> reader = ReferenceT<BasicRealTimeDataSourceInputReader>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddVariable(def3)) {
        return false;
    }

    if (!reader->AddVariable(def4)) {
        return false;
    }

    if (!reader->Finalise()) {
        return false;
    }

    const char8 *nextState = "+state1";
    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);

    if (!writer->Write(0)) {
        return false;
    }

    if (!reader->Read(0)) {
        return false;
    }

    ControlIn *controlPlant = (ControlIn *) reader->GetData(0);
    ControlNoise *controlNoise = (ControlNoise *) reader->GetData(1);

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

bool BasicRealTimeDataSourceOutputWriterTest::TestWrite_MultiDim_Vector() {

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
    ConfigurationDatabase cdbMulti;

    // read and write on the same memory
    cdbMulti.CreateAbsolute("+Inputs");
    cdbMulti.Write("Class", "RealTimeDataDefContainer");
    cdbMulti.Write("IsInput", "true");
    cdbMulti.Write("IsFinal", "false");
    cdbMulti.CreateAbsolute("+Inputs.+Error");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "ControlInArray");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.CreateAbsolute("+Inputs.+Error.+Pars");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "uint32");
    cdbMulti.Write("Modifiers", "[2]");
    cdbMulti.Write("Default", "{1,2}");
    cdbMulti.Write("Path", "+DDB1.PidControlArray");
    cdbMulti.Write("IsFinal", "true");

    cdbMulti.CreateAbsolute("+Outputs");
    cdbMulti.Write("Class", "RealTimeDataDefContainer");
    cdbMulti.Write("IsOutput", "true");
    cdbMulti.Write("IsFinal", "false");
    cdbMulti.CreateAbsolute("+Outputs.+Control");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "ControlInArray");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.CreateAbsolute("+Outputs.+Control.+Pars");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "uint32");
    cdbMulti.Write("Modifiers", "[2]");
    cdbMulti.Write("Path", "+DDB1.PidControlArray");
    cdbMulti.Write("Default", "{1,2}");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.MoveToRoot();

    ReferenceT<PIDGAM2> pid = ReferenceT<PIDGAM2>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(cdbMulti)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("+state1");
    if (!pid->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> defOut = pid->Find("+Outputs.+Control");

    ReferenceT<BasicRealTimeDataSourceOutputWriter> writer = ReferenceT<BasicRealTimeDataSourceOutputWriter>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(*rtapp.operator ->());

    if (!writer->AddVariable(defOut)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    ControlInArray *pidControl = (ControlInArray *) writer->GetData(0);

    pidControl->Pars[0] = 10;
    pidControl->Pars[1] = 20;

    ReferenceT<RealTimeGenericDataDef> defIn = pid->Find("+Inputs.+Error");

    ReferenceT<BasicRealTimeDataSourceInputReader> reader = ReferenceT<BasicRealTimeDataSourceInputReader>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddVariable(defIn)) {
        return false;
    }
    if (!reader->Finalise()) {
        return false;
    }

    const char8 *nextState = "+state1";
    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);

    if (!writer->Write(0)) {
        return false;
    }

    if (!reader->Read(0)) {
        return false;
    }

    ControlInArray *controlPlant = (ControlInArray *) reader->GetData(0);

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

bool BasicRealTimeDataSourceOutputWriterTest::TestWrite_MultiDim_Matrix() {

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
    ConfigurationDatabase cdbMulti;

    // read and write on the same memory
    cdbMulti.CreateAbsolute("+Inputs");
    cdbMulti.Write("Class", "RealTimeDataDefContainer");
    cdbMulti.Write("IsInput", "true");
    cdbMulti.Write("IsFinal", "false");
    cdbMulti.CreateAbsolute("+Inputs.+Error");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "ControlInArray");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.CreateAbsolute("+Inputs.+Error.+Pars");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "uint32");
    cdbMulti.Write("Modifiers", "[3][2]");
    cdbMulti.Write("Default", "{{1,2},{3,4},{5,6}}");
    cdbMulti.Write("Path", "+DDB1.PidControlArray");
    cdbMulti.Write("IsFinal", "true");

    cdbMulti.CreateAbsolute("+Outputs");
    cdbMulti.Write("Class", "RealTimeDataDefContainer");
    cdbMulti.Write("IsOutput", "true");
    cdbMulti.Write("IsFinal", "false");
    cdbMulti.CreateAbsolute("+Outputs.+Control");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "ControlInArray");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.CreateAbsolute("+Outputs.+Control.+Pars");
    cdbMulti.Write("Class", "RealTimeGenericDataDef");
    cdbMulti.Write("Type", "uint32");
    cdbMulti.Write("Modifiers", "[3][2]");
    cdbMulti.Write("Path", "+DDB1.PidControlArray");
    cdbMulti.Write("Default", "{{1,2},{3,4},{5,6}}");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.MoveToRoot();

    ReferenceT<PIDGAM3> pid = ReferenceT<PIDGAM3>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    pid->SetName("Pid1");
    if (!pid->Initialise(cdbMulti)) {
        return false;
    }

    pid->SetApplication(*rtapp.operator ->());
    pid->AddState("+state1");
    if (!pid->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> defOut = pid->Find("+Outputs.+Control");

    ReferenceT<BasicRealTimeDataSourceOutputWriter> writer = ReferenceT<BasicRealTimeDataSourceOutputWriter>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(*rtapp.operator ->());

    if (!writer->AddVariable(defOut)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    ControlInMatrix *pidControl = (ControlInMatrix *) writer->GetData(0);

    pidControl->Pars[0][0] = 10;
    pidControl->Pars[0][1] = 20;
    pidControl->Pars[1][0] = 30;
    pidControl->Pars[1][1] = 40;
    pidControl->Pars[2][0] = 50;
    pidControl->Pars[2][1] = 60;

    ReferenceT<RealTimeGenericDataDef> defIn = pid->Find("+Inputs.+Error");

    ReferenceT<BasicRealTimeDataSourceInputReader> reader = ReferenceT<BasicRealTimeDataSourceInputReader>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(*rtapp.operator ->());

    if (!reader->AddVariable(defIn)) {
        return false;
    }
    if (!reader->Finalise()) {
        return false;
    }

    const char8 *nextState = "+state1";
    // the function will fail because of the scheduler not initialised
    rtapp->PrepareNextState(nextState);

    if (!writer->Write(0)) {
        return false;
    }

    if (!reader->Read(0)) {
        return false;
    }

    ControlInMatrix *controlPlant = (ControlInMatrix *) reader->GetData(0);

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
