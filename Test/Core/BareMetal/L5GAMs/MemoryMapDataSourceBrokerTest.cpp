/**
 * @file MemoryMapDataSourceBrokerTest.cpp
 * @brief Source file for class MemoryMapDataSourceBrokerTest
 * @date 18/04/2016
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
 * the class MemoryMapDataSourceBrokerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryMapDataSourceBrokerTest.h"
#include "GAMTestHelper.h"
#include "GAMGenericSignal.h"
#include "DataSource.h"
#include "GAMSampledSignal.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MemoryMapDataSourceBrokerTest::MemoryMapDataSourceBrokerTest() {
    StreamString cdbConf = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Control = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = ControlIn\n"
            "        IsFinal = true\n"
            "        +Par1 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Default = 2\n"
            "            Path = DDB1.PidControl1\n"
            "            IsFinal = true\n"
            "        }\n"
            "        +Par2 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Default = 2\n"
            "            Path = DDB1.PidControl2\n"
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
            "    +Noise = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = ControlNoise\n"
            "        IsFinal = true\n"
            "        +noiseValue = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = float32\n"
            "            Default = 0.0\n"
            "            Path = DDB1.PidNoise1\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n";

    cdbConf.Seek(0);
    StandardParser parser(cdbConf, cdb);
    parser.Parse();

    StreamString vectorConf = "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = TrackErrorArray\n"
            "        IsFinal = true\n"
            "        +Pars = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Modifiers = \"[2]\"\n"
            "            Default = \"{1,2}\"\n"
            "            Path = DDB1.PidErrorArray\n"
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
            "            Modifiers = \"[2]\"\n"
            "            Path = DDB1.PidControlArray\n"
            "            Default = \"{3,4}\"\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n";

    vectorConf.Seek(0);

    StandardParser parserVector(vectorConf, cdbVector);

    parserVector.Parse();

    StreamString matrixConf = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = TrackErrorMatrix\n"
            "        IsFinal = true\n"
            "        +Pars = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Modifiers = \"[3][2]\"\n"
            "            Default = \"{{1,2},{3,4},{5,6}}\"\n"
            "            Path = DDB1.PidErrorArray\n"
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
            "            Modifiers = \"[3][2]\"\n"
            "            Path = DDB1.PidControlArray\n"
            "            Default = \"{{7,8},{9,10},{11,12}}\"\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n";
    matrixConf.Seek(0);

    StandardParser parserMatrix(matrixConf, cdbMatrix);

    parserMatrix.Parse();

}

bool MemoryMapDataSourceBrokerTest::TestConstructor() {
    MemoryMapDataSourceBroker test;
    return true;
}

bool MemoryMapDataSourceBrokerTest::TestSetApplication() {
    MemoryMapDataSourceBroker test;
    RealTimeApplication app;
    test.SetApplication(app);
    return true;
}

bool MemoryMapDataSourceBrokerTest::TestAddSignal_Allocation() {

    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
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

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
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

    ReferenceT<GAMGenericSignal> def = gam->Find("Inputs.Control");

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    if (broker->GetSignal(0) != NULL) {
        return false;
    }

    if (!broker->AddSignal(def)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetSignal(0) == NULL) {
        return false;
    }

    return true;
}

bool MemoryMapDataSourceBrokerTest::TestAddSignal_Static() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
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

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
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

    ReferenceT<GAMGenericSignal> def = gam->Find("Inputs.Control");

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    ControlIn output;

    if (broker->GetSignal(0) != NULL) {
        return false;
    }

    if (!broker->AddSignal(def, &output)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetSignal(0) != &output) {
        return false;
    }

    return true;
}

bool MemoryMapDataSourceBrokerTest::TestAddSignal_Allocation_BasicType() {

    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
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

    ConfigurationDatabase xcdb;
    xcdb.CreateAbsolute("+Inputs");
    xcdb.Write("Class", "GAMSignalsContainer");
    xcdb.Write("IsInput", "true");
    xcdb.Write("IsFinal", "true");
    xcdb.CreateAbsolute("+Inputs.+Basic");
    xcdb.Write("Class", "GAMGenericSignal");
    xcdb.Write("Type", "uint32");
    xcdb.Write("Default", "2");
    xcdb.Write("Path", "DDB1.Basic");
    xcdb.Write("IsFinal", "true");
    xcdb.MoveToRoot();

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(xcdb)) {
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

    ReferenceT<GAMGenericSignal> def = gam->Find("Inputs.Basic");

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());
    if (!broker->AddSignal(def)) {
        return false;
    }
    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetSignal(0) == NULL) {
        return false;
    }
    return true;

}

bool MemoryMapDataSourceBrokerTest::TestAddSignal_Static_BasicType() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
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

    ConfigurationDatabase xcdb;
    xcdb.CreateAbsolute("+Inputs");
    xcdb.Write("Class", "GAMSignalsContainer");
    xcdb.Write("IsInput", "true");
    xcdb.Write("IsFinal", "true");
    xcdb.CreateAbsolute("+Inputs.+Basic");
    xcdb.Write("Class", "GAMGenericSignal");
    xcdb.Write("Type", "uint32");
    xcdb.Write("Default", "2");
    xcdb.Write("Path", "DDB1.Basic");
    xcdb.Write("IsFinal", "true");
    xcdb.MoveToRoot();

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(xcdb)) {
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

    ReferenceT<GAMGenericSignal> def = gam->Find("Inputs.Basic");

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    uint32 x;

    broker->SetApplication(*rtapp.operator ->());
    if (!broker->AddSignal(def, &x)) {
        return false;
    }
    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetSignal(0) != &x) {
        return false;
    }
    return true;

}

bool MemoryMapDataSourceBrokerTest::TestAddSignalFalse_InvalidDef() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
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
    ReferenceT<GAMGenericSignal> def;

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    TrackError output;

    return (!broker->AddSignal(def, &output));
}

bool MemoryMapDataSourceBrokerTest::TestAddSignalFalse_NoDataSourceSet() {

    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
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

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
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

    ReferenceT<GAMGenericSignal> def = gam->Find("Inputs.Control");

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    ControlIn output;

    if (broker->GetSignal(0) != NULL) {
        return false;
    }

    return (!broker->AddSignal(def, &output));
}

bool MemoryMapDataSourceBrokerTest::TestAddSignalFalse_UnintrospectableType() {
    ConfigurationDatabase cdbLoc;

    cdbLoc.Write("Class", "GAMGenericSignal");
    cdbLoc.Write("Type", "Object");
    cdbLoc.Write("IsFinal", "true");
    cdbLoc.Write("Path", "DDB2.PidControl");
    cdbLoc.Write("Default", "1");
    cdbLoc.Write("IsFinal", "true");

    cdbLoc.MoveToRoot();

    ReferenceT<GAMGenericSignal> def = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!def->Initialise(cdbLoc)) {
        return false;
    }

    ConfigurationDatabase appCDB;

    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "DataSource");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> app = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!app->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*app.operator ->());

    return !broker->AddSignal(def);
}

bool MemoryMapDataSourceBrokerTest::TestAddSignalFalse_UnregisteredType() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
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

    ConfigurationDatabase cdbDef;
    cdbDef.Write("Class", "GAMGenericSignal");
    cdbDef.Write("Type", "Invalid");
    cdbDef.Write("IsFinal", "true");
    cdbDef.Write("Path", "DDB1.PidErrorStructured");
    cdbDef.Write("IsFinal", "true");
    cdbDef.MoveToRoot();

    ReferenceT<GAMGenericSignal> def = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!def->Initialise(cdbDef)) {
        return false;
    }

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());
    return (!broker->AddSignal(def));
}

bool MemoryMapDataSourceBrokerTest::TestAddSignalFalse_NotInDataSource() {
    ConfigurationDatabase cdbLoc;

    cdbLoc.Write("Class", "GAMGenericSignal");
    cdbLoc.Write("Type", "ControlIn");
    cdbLoc.Write("IsFinal", "false");
    cdbLoc.CreateAbsolute("+Par1");
    cdbLoc.Write("Class", "GAMGenericSignal");
    cdbLoc.Write("Type", "uint32");
    cdbLoc.Write("Path", "DDB2.PidControl1");
    cdbLoc.Write("Default", "1");
    cdbLoc.Write("IsFinal", "true");
    cdbLoc.CreateAbsolute("+Par2");
    cdbLoc.Write("Class", "GAMGenericSignal");
    cdbLoc.Write("Type", "uint32");
    // the data source will not be configured, this def is not found
    cdbLoc.Write("Path", "DDB2.PidControl2");
    cdbLoc.Write("Default", "1");
    cdbLoc.Write("IsFinal", "true");

    cdbLoc.MoveToRoot();

    ReferenceT<GAMGenericSignal> def = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!def->Initialise(cdbLoc)) {
        return false;
    }

    ConfigurationDatabase appCDB;

    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB2");
    appCDB.Write("Class", "DataSource");
    appCDB.CreateAbsolute("+Data.+DDB2.PidControl1");
    appCDB.Write("Class", "DataSourceSignal");
    appCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> app = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!app->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*app.operator ->());

    return !broker->AddSignal(def);
}

bool MemoryMapDataSourceBrokerTest::TestAddSignal_MultiDimensional_Static_Vector() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
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

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(cdbVector)) {
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

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    TrackErrorArray output;

    output.Pars[0] = 1;
    output.Pars[1] = 2;

    if (broker->GetSignal(0) != NULL) {
        return false;
    }

    if (!broker->AddSignal(def, &output)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetSignal(0) != &output) {
        return false;
    }

    TrackErrorArray *ret = (TrackErrorArray *) broker->GetSignal(0);
    if (ret->Pars[0] != 1) {
        return false;
    }

    if (ret->Pars[1] != 2) {
        return false;
    }
    return true;
}

bool MemoryMapDataSourceBrokerTest::TestAddSignal_MultiDimensional_Allocation_Vector() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.Write("IsFinal", "true");
    appCDB.CreateAbsolute("+Data.+DDB1");
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

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(cdbVector)) {
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

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    if (broker->GetSignal(0) != NULL) {
        return false;
    }

    if (!broker->AddSignal(def)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    TrackErrorArray *ret = (TrackErrorArray *) broker->GetSignal(0);

    return ret != NULL;
}

bool MemoryMapDataSourceBrokerTest::TestAddSignal_MultiDimensional_Static_Matrix() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
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

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(cdbMatrix)) {
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

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    TrackErrorMatrix output;

    output.Pars[0][0] = 1;
    output.Pars[0][1] = 2;
    output.Pars[1][0] = 3;
    output.Pars[1][1] = 4;
    output.Pars[2][0] = 5;
    output.Pars[2][1] = 6;

    if (broker->GetSignal(0) != NULL) {
        return false;
    }

    if (!broker->AddSignal(def, &output)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetSignal(0) != &output) {
        return false;
    }

    TrackErrorMatrix *ret = (TrackErrorMatrix *) broker->GetSignal(0);
    if (ret->Pars[0][0] != 1) {
        return false;
    }

    if (ret->Pars[0][1] != 2) {
        return false;
    }
    if (ret->Pars[1][0] != 3) {
        return false;
    }

    if (ret->Pars[1][1] != 4) {
        return false;
    }

    if (ret->Pars[2][0] != 5) {
        return false;
    }

    if (ret->Pars[2][1] != 6) {
        return false;
    }
    return true;
}

bool MemoryMapDataSourceBrokerTest::TestAddSignal_MultiDimensional_Allocation_Matrix() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
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

    ReferenceT<PIDGAM3> gam = ReferenceT<PIDGAM3>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(cdbMatrix)) {
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

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    if (broker->GetSignal(0) != NULL) {
        return false;
    }

    if (!broker->AddSignal(def)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    TrackErrorMatrix *ret = (TrackErrorMatrix *) broker->GetSignal(0);

    return (ret != NULL);
}

bool MemoryMapDataSourceBrokerTest::TestAddSignalFalse_UnsupportedMultidimStructured() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
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

    ConfigurationDatabase cdbMulti;

    cdbMulti.CreateAbsolute("+Inputs");
    cdbMulti.Write("Class", "GAMSignalsContainer");
    cdbMulti.Write("IsInput", "true");
    cdbMulti.Write("IsFinal", "false");
    cdbMulti.CreateAbsolute("+Inputs.+Error");
    cdbMulti.Write("Class", "GAMGenericSignal");
    cdbMulti.Write("Type", "TrackErrorMatrix");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.Write("Modifiers", "[3][2]");
    cdbMulti.Write("Path", "DDB1.PidErrorStructured");
    cdbMulti.Write("IsFinal", "true");

    cdbMulti.CreateAbsolute("+Outputs");
    cdbMulti.Write("Class", "GAMSignalsContainer");
    cdbMulti.Write("IsOutput", "true");
    cdbMulti.Write("IsFinal", "false");
    cdbMulti.CreateAbsolute("+Outputs.+Control");
    cdbMulti.Write("Class", "GAMGenericSignal");
    cdbMulti.Write("Type", "ControlInMatrix");
    cdbMulti.Write("IsFinal", "true");
    cdbMulti.CreateAbsolute("+Outputs.+Control.+Pars");
    cdbMulti.Write("Class", "GAMGenericSignal");
    cdbMulti.Write("Type", "uint32");
    cdbMulti.Write("Modifiers", "[3][2]");
    cdbMulti.Write("Path", "DDB1.PidControlArray");
    cdbMulti.Write("Default", "{{7,8},{9,10},{11,12}}");
    cdbMulti.Write("IsFinal", "true");

    cdbMulti.MoveToRoot();

    ReferenceT<PIDGAM3> gam = ReferenceT<PIDGAM3>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Pid1");
    if (!gam->Initialise(cdbMulti)) {
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

    ReferenceT<GAMGenericSignal> def = gam->Find("Inputs.Error");

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    return (!broker->AddSignal(def));
}

bool MemoryMapDataSourceBrokerTest::TestAddSignalFalse_UnfoundMember() {
    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
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

    ConfigurationDatabase cdbDef;
    cdbDef.Write("Class", "GAMGenericSignal");
    cdbDef.Write("Type", "TrackError");
    cdbDef.Write("IsFinal", "true");
    cdbDef.CreateAbsolute("+Par3");
    cdbDef.Write("Class", "GAMGenericSignal");
    cdbDef.Write("Type", "uint32");
    cdbDef.Write("Default", "1");
    cdbDef.Write("Path", "DDB1.PidError1");
    cdbDef.Write("IsFinal", "true");
    cdbDef.CreateAbsolute("+Par4");
    cdbDef.Write("Class", "GAMGenericSignal");
    cdbDef.Write("Type", "uint32");
    cdbDef.Write("Default", "2");
    cdbDef.Write("Path", "DDB1.PidError2");
    cdbDef.Write("IsFinal", "true");
    cdbDef.MoveToRoot();

    ReferenceT<GAMGenericSignal> def = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!def->Initialise(cdbDef)) {
        return false;
    }

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());
    return (!broker->AddSignal(def));

}

bool MemoryMapDataSourceBrokerTest::TestGetSignal_Allocation() {

    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
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

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
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

    ReferenceT<GAMGenericSignal> def = gam->Find("Outputs.Error");
    ReferenceT<GAMGenericSignal> def2 = gam->Find("Outputs.Noise");

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    if (broker->GetSignal(0) != NULL) {
        return false;
    }

    if (broker->GetSignal(1) != NULL) {
        return false;
    }

    if (!broker->AddSignal(def)) {
        return false;
    }

    if (!broker->AddSignal(def2)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetSignal(0) == NULL) {
        return false;
    }

    if (broker->GetSignal(1) == NULL) {
        return false;
    }
    return !broker->GetSignal(2);
}

bool MemoryMapDataSourceBrokerTest::TestGetSignal_Static() {

    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
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

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
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

    TrackError error;
    ControlNoise noise;

    ReferenceT<GAMGenericSignal> def = gam->Find("Outputs.Error");
    ReferenceT<GAMGenericSignal> def2 = gam->Find("Outputs.Noise");

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    if (broker->GetSignal(0) != NULL) {
        return false;
    }

    if (broker->GetSignal(1) != NULL) {
        return false;
    }

    if (!broker->AddSignal(def, &error)) {
        return false;
    }

    if (!broker->AddSignal(def2, &noise)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetSignal(0) != &error) {
        return false;
    }

    if (broker->GetSignal(1) != &noise) {
        return false;
    }
    return !broker->GetSignal(2);

}

bool MemoryMapDataSourceBrokerTest::TestFinalise() {

    ConfigurationDatabase appCDB;
    appCDB.CreateAbsolute("+Data");
    appCDB.Write("Class", "DataSourceContainer");
    appCDB.CreateAbsolute("+Data.+DDB1");
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

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
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

    ReferenceT<GAMGenericSignal> def = gam->Find("Outputs.Error");
    ReferenceT<GAMGenericSignal> def2 = gam->Find("Outputs.Noise");

    ReferenceT<MemoryMapDataSourceBroker> broker = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    if (!broker->AddSignal(def)) {
        return false;
    }

    if (!broker->AddSignal(def2)) {
        return false;
    }

    if (broker->GetSignal(0) != NULL) {
        return false;
    }


    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetSignal(0) == NULL) {
        return false;
    }


    return true;
}

bool MemoryMapDataSourceBrokerTest::TestFailureOfSampleDefinitionInGeneric() {
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
            "        Class = GAMGenericSignal\n"
            "        Type = ControlInMatrix\n"
            "        IsFinal = true\n"
            "        +Pars = {\n"
            "            Class = GAMSampledSignal\n"
            "            Type = uint32\n"
            "            Dimensions = \"[3][2]\"\n"
            "            Operation = \"{{0,3}{6,6}}\"\n"
            "            Path = Driver.DriverSignal\n"
            "            IsFinal = true\n"
            "            Samples = \"{{1,1,1},{0,0,1}}\"" // the first and the last
            "            Cycles=2\n"
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

    ReferenceT<GAMGenericSignal> defIn1 = pid->Find("Input1.Error");

    ReferenceT<MemoryMapDataSourceBroker> reader1 = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    reader1->SetName("reader1");

    reader1->SetApplication(*rtapp.operator ->());

    //impossible define a sampled signal into a generic one.
    return (!reader1->AddSignal(defIn1));
}
