/**
 * @file BasicGAMTest.cpp
 * @brief Source file for class BasicGAMTest
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
 * the class BasicGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicGAMTest.h"
#include "GAMTestHelper.h"
#include "RealTimeDataDefContainer.h"
#include "BasicRealTimeDataSourceDef.h"
#include "RealTimeGenericDataDef.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

BasicGAMTest::BasicGAMTest() {

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

BasicGAMTest::~BasicGAMTest() {
    ObjectRegistryDatabase::Instance()->CleanUp();
}

bool BasicGAMTest::TestConstructor() {
    PIDGAM gamTest;
    if (gamTest.GetNumberOfSupportedStates() != 0) {
        return false;
    }
    if (gamTest.GetSupportedStates() != NULL) {
        return false;
    }

    return (gamTest.GetSupportedThreads() == NULL);
}

bool BasicGAMTest::TestConfigureDataSourceLinks() {
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

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1", "thread1");

    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "+state1";
    rtapp->PrepareNextState(nextState);

    if (!gam->ConfigureDataSourceLinks()) {
        return false;
    }

    return true;
}

bool BasicGAMTest::TestIsSync() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "RealTimeDataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
    appCDB.Write("Class", "RealTimeDataSource");
    appCDB.CreateAbsolute("+Data.+DDB1.+Driver");
    appCDB.Write("Class", "SharedDataSource");
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

    ConfigurationDatabase cdb;
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

    cdb.CreateAbsolute("+Inputs.+Sync");
    cdb.Write("Class", "RealTimeGenericDataDef");

    cdb.Write("Type", "uint32");
    cdb.Write("IsFinal", "true");
    cdb.Write("Default", "2");
    cdb.Write("Path", "+DDB1.+Driver");

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

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");

    if (gam->IsSync()) {
        return false;
    }

    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1", "thread1");

    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "+state1";
    rtapp->PrepareNextState(nextState);

    if (!gam->ConfigureDataSourceLinks()) {
        return false;
    }

    return gam->IsSync();
}

bool BasicGAMTest::TestGetInputReader() {
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

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");

    if (gam->GetInputReader().IsValid()) {
        return false;
    }

    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1", "thread1");

    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "+state1";
    rtapp->PrepareNextState(nextState);

    if (!gam->ConfigureDataSourceLinks()) {
        return false;
    }

    ReferenceT<BasicRealTimeDataSourceInputReader> reader = gam->GetInputReader();
    if (!reader.IsValid()) {
        return false;
    }
    reader->Read(0);
    TrackError *var1 = (TrackError *) reader->GetData(0);
    printf("\nread %d\n", var1->Par2 );

    return (var1->Par2 == 2);
}

bool BasicGAMTest::TestGetOutputWriter() {
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

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");

    if (gam->GetOutputWriter().IsValid()) {
        return false;
    }

    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1", "thread1");

    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "+state1";
    rtapp->PrepareNextState(nextState);

    if (!gam->ConfigureDataSourceLinks()) {
        return false;
    }

    ReferenceT<BasicRealTimeDataSourceOutputWriter> writer = gam->GetOutputWriter();
    if (!writer.IsValid()) {
        return false;
    }

    ControlIn *var1 = (ControlIn *) writer->GetData(0);
    var1->Par2 = 10;
    writer->Write(0);

    ConfigurationDatabase defCDB;
    defCDB.Write("Class", "RealTimeGenericDataDef");
    defCDB.Write("Type", "uint32");
    defCDB.Write("Path", "+DDB2.PidControl2");
    defCDB.Write("Default", "1");
    defCDB.Write("IsFinal", "true");
    defCDB.MoveToRoot();

    RealTimeGenericDataDef def;
    if (!def.Initialise(defCDB)) {
        return false;
    }

    BasicRealTimeDataSourceInputReader reader;
    reader.SetApplication(*rtapp.operator ->());
    if (!reader.AddVariable(ReferenceT<RealTimeDataDefI>(&def))) {
        return false;
    }
    if (!reader.Finalise()) {
        return false;
    }
    reader.Read(0);
    uint32 *readVar = (uint32 *) reader.GetData(0);

    printf("\nread %d\n", *readVar);
    return (*readVar== 10);
}

bool BasicGAMTest::TestExecute() {
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

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdb)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("+state1", "thread1");

    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    const char8 *nextState = "+state1";
    rtapp->PrepareNextState(nextState);

    if (!gam->ConfigureDataSourceLinks()) {
        return false;
    }

    gam->Execute(0);

    return true;
}
