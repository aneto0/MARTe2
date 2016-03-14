/**
 * @file RealTimeDataSourceInputReaderTest.cpp
 * @brief Source file for class RealTimeDataSourceInputReaderTest
 * @date 14/03/2016
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
 * the class RealTimeDataSourceInputReaderTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSourceInputReaderTest.h"
#include "GAMTestHelper.h"
#include "RealTimeGenericDataDef.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeDataSourceInputReaderTest::RealTimeDataSourceInputReaderTest() {
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

bool RealTimeDataSourceInputReaderTest::TestConstructor() {
    RealTimeDataSourceInputReader test;
    return true;
}

bool RealTimeDataSourceInputReaderTest::TestRead_DefaultValues_Static() {

    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "ReferenceContainer");
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

    gam->SetApplication(rtapp);
    gam->AddState("+state1");
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

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Inputs.+Error");

    ReferenceT<RealTimeDataSourceInputReader> reader = ReferenceT<RealTimeDataSourceInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("broker");

    reader->SetApplication(rtapp);

    if (!reader->AddVariable(def, &error)) {
        return false;
    }

    if (!reader->Finalise()) {
        return false;
    }

    RealTimeStateInfo info;
    info.currentState = "";
    info.nextState = "+state1";
    info.activeBuffer = 1;
    if (!rtapp->PrepareNextState(info)) {
        return false;
    }

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

bool RealTimeDataSourceInputReaderTest::TestRead_DefaultValues_Allocation() {

    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "ReferenceContainer");
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

    gam->SetApplication(rtapp);
    gam->AddState("+state1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def = gam->Find("+Inputs.+Error");

    ReferenceT<RealTimeDataSourceInputReader> reader = ReferenceT<RealTimeDataSourceInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("broker");

    reader->SetApplication(rtapp);

    if (!reader->AddVariable(def)) {
        return false;
    }

    if (!reader->Finalise()) {
        return false;
    }

    RealTimeStateInfo info;
    info.currentState = "";
    info.nextState = "+state1";
    info.activeBuffer = 1;
    if (!rtapp->PrepareNextState(info)) {
        return false;
    }

    if (!reader->Read(0)) {
        return false;
    }

    TrackError *error = (TrackError *) reader->GetData(0);

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

bool RealTimeDataSourceInputReaderTest::TestRead_Static() {

    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "ReferenceContainer");
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

    pid->SetApplication(rtapp);
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

    plant->SetApplication(rtapp);
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

    ReferenceT<RealTimeDataSourceInputReader> reader = ReferenceT<RealTimeDataSourceInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(rtapp);

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

    ReferenceT<RealTimeDataSourceOutputWriter> writer = ReferenceT<RealTimeDataSourceOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(rtapp);

    if (!writer->AddVariable(def2, &errorPlant)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    RealTimeStateInfo info;
    info.currentState = "";
    info.nextState = "+state1";
    info.activeBuffer = 1;
    if (!rtapp->PrepareNextState(info)) {
        return false;
    }

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

bool RealTimeDataSourceInputReaderTest::TestRead_Allocation() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "ReferenceContainer");
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

    pid->SetApplication(rtapp);
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

    plant->SetApplication(rtapp);
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

    ReferenceT<RealTimeDataSourceInputReader> reader = ReferenceT<RealTimeDataSourceInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(rtapp);

    if (!reader->AddVariable(def)) {
        return false;
    }

    if (!reader->Finalise()) {
        return false;
    }

    ReferenceT<RealTimeGenericDataDef> def2 = plant->Find("+Outputs.+Error");

    ReferenceT<RealTimeDataSourceOutputWriter> writer = ReferenceT<RealTimeDataSourceOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(rtapp);

    if (!writer->AddVariable(def2)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    TrackError *errorPlant = (TrackError *) writer->GetData(0);
    errorPlant->Par1 = 10;
    errorPlant->Par2 = 20;

    RealTimeStateInfo info;
    info.currentState = "";
    info.nextState = "+state1";
    info.activeBuffer = 1;
    if (!rtapp->PrepareNextState(info)) {
        return false;
    }

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

bool RealTimeDataSourceInputReaderTest::TestRead_MoreThanOneVariable() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "ReferenceContainer");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "ReferenceContainer");
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

    pid->SetApplication(rtapp);
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

    plant->SetApplication(rtapp);
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

    ReferenceT<RealTimeDataSourceOutputWriter> writer = ReferenceT<RealTimeDataSourceOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    writer->SetName("writer");

    writer->SetApplication(rtapp);

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

    ReferenceT<RealTimeDataSourceInputReader> reader = ReferenceT<RealTimeDataSourceInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader->SetName("reader");

    reader->SetApplication(rtapp);

    if (!reader->AddVariable(def3)) {
        return false;
    }

    if (!reader->AddVariable(def4)) {
        return false;
    }

    if (!reader->Finalise()) {
        return false;
    }

    RealTimeStateInfo info;
    info.currentState = "";
    info.nextState = "+state1";
    info.activeBuffer = 1;
    if (!rtapp->PrepareNextState(info)) {
        return false;
    }

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

