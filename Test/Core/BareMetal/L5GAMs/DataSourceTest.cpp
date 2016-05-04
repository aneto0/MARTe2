/**
 * @file DataSourceTest.cpp
 * @brief Source file for class DataSourceTest
 * @date 15/apr/2016
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
 * the class DataSourceTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DataSourceTest.h"
#include "ConfigurationDatabase.h"
#include "GAMTestHelper.h"
#include "DataSourceContainer.h"
#include "DataSourceSignal.h"
#include "GAMGenericSignal.h"
#include "MemoryMapInputReader.h"

#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool DataSourceTest::TestConstructor() {
    return true;
}

bool DataSourceTest::TestInitialise() {
    ConfigurationDatabase cdb;

    cdb.Write("Class", "DataSource");
    cdb.CreateAbsolute("+DDB1");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateAbsolute("+DDB2");
    cdb.Write("Class", "ReferenceContainer");

    cdb.MoveToRoot();

    DataSource sourceDefs;

    if (!sourceDefs.Initialise(cdb)) {
        return false;
    }

    return sourceDefs.Size() == 2;
}

bool DataSourceTest::TestAllocate() {

    StreamString confString = ""
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
            "            Default = 1\n"
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
            "            Default = 1\n"
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
            "            Default = 1\n"
            "            Path = DDB2.PidNoise\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n";

    confString.Seek(0);
    ConfigurationDatabase cdb;

    StandardParser parser(confString, cdb);
    if (!parser.Parse()) {
        return false;
    }

    ReferenceT<PIDGAM> gam1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam1->Initialise(cdb)) {
        return false;
    }
    gam1->SetName("PID1");
    gam1->AddState("state1", "thread1");
    gam1->AddState("state2", "thread1");

    StreamString plantConf = ""
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
            "            Default = 1\n"
            "            Path = DDB2.PidControl1\n"
            "            IsFinal = true\n"
            "        }\n"
            "        +Par2 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Default = 1\n"
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
            "            Default = 1\n"
            "            Path = DDB1.PidError2\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n";

    plantConf.Seek(0);
    ConfigurationDatabase cdbPlant;
    StandardParser plantParser(plantConf, cdbPlant);
    if (!plantParser.Parse()) {
        return false;
    }

    ReferenceT<PlantGAM> gam2 = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!gam2->Initialise(cdbPlant)) {
        printf("\nfailed gam2 init\n");
        return false;
    }
    gam2->SetName("Plant");
    gam2->AddState("state1", "thread1");
    gam2->AddState("state2", "thread1");

    ConfigurationDatabase cdbDS;

    cdbDS.Write("Class", "DataSourceContainer");
    cdbDS.Write("IsFinal", "true");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "DataSource");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "DataSource");
    cdbDS.MoveToRoot();

    ReferenceT<DataSourceContainer> sourceDefs = ReferenceT<DataSourceContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sourceDefs->SetName("Data");

    RealTimeApplication app;
    app.Insert(sourceDefs);

    gam1->SetApplication(app);
    gam2->SetApplication(app);

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam1)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam2)) {
        return false;
    }

    if (!sourceDefs->Verify()) {
        return false;
    }

    if (!sourceDefs->Allocate()) {
        return false;
    }


    // test if it is possible read a value
    ReferenceT<MemoryMapOutputWriter> writer = ReferenceT<MemoryMapOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<GAMGenericSignal> defOut = gam2->Find("Outputs.Error");

    writer->SetApplication(app);
    if (!writer->AddSignal(defOut)) {
        return false;
    }
    if (!writer->Finalise()) {
        return false;
    }

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<GAMGenericSignal> defIn = gam1->Find("Inputs.Error");
    reader->SetApplication(app);
    if (!reader->AddSignal(defIn)) {
        return false;
    }
    if (!reader->Finalise()) {
        return false;
    }

    TrackError *var = (TrackError *) writer->GetSignal(0);
    var->Par1 = 1;
    var->Par2 = 2;
    writer->Write(0);

    reader->Read(0);
    TrackError *ret = (TrackError *) reader->GetSignal(0);
    if (ret->Par1 != 1) {
        return false;
    }
    if (ret->Par2 != 2) {
        return false;
    }
    return true;
}

bool DataSourceTest::TestAllocate_Structure() {

    StreamString confString = ""
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
            "            Default = 1\n"
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
            "            Default = 1\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "    +Noise = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = ControlNoise\n"
            "        IsFinal = true\n"
            "        Default = \"noiseValue=2\"\n"
            "        Path = DDB2.PidNoise\n"
            "    }\n"
            "}\n";

    confString.Seek(0);
    ConfigurationDatabase cdb;

    StandardParser parser(confString, cdb);
    if (!parser.Parse()) {
        return false;
    }

    ReferenceT<PIDGAM> gam1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam1->Initialise(cdb)) {
        return false;
    }
    gam1->SetName("PID1");
    gam1->AddState("state1", "thread1");
    gam1->AddState("state2", "thread1");

    StreamString plantConf = ""
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
            "            Default = 1\n"
            "            Path = DDB2.PidControl1\n"
            "            IsFinal = true\n"
            "        }\n"
            "        +Par2 = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Default = 1\n"
            "            Path = DDB2.PidControl2\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "    +Noise = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = ControlNoise\n"
            "        IsFinal = true\n"
            "        Default = \"noiseValue=2\"\n"
            "        Path = DDB2.PidNoise\n"
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
            "            Default = 1\n"
            "            Path = DDB1.PidError2\n"
            "            IsFinal = true\n"
            "        }\n"
            "    }\n"
            "}\n";

    plantConf.Seek(0);
    ConfigurationDatabase cdbPlant;
    StandardParser plantParser(plantConf, cdbPlant);
    if (!plantParser.Parse()) {
        return false;
    }

    ReferenceT<PlantGAM> gam2 = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!gam2->Initialise(cdbPlant)) {
        printf("\nfailed gam2 init\n");
        return false;
    }
    gam2->SetName("Plant");
    gam2->AddState("state1", "thread1");
    gam2->AddState("state2", "thread1");

    ConfigurationDatabase cdbDS;

    cdbDS.Write("Class", "DataSourceContainer");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "DataSource");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "DataSource");
    cdbDS.MoveToRoot();

    ReferenceT<DataSourceContainer> sourceDefs = ReferenceT<DataSourceContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sourceDefs->SetName("Data");

    RealTimeApplication app;
    app.Insert(sourceDefs);

    gam1->SetApplication(app);
    gam2->SetApplication(app);

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam1)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam2)) {
        return false;
    }

    if (!sourceDefs->Verify()) {
        return false;
    }

    if (!sourceDefs->Allocate()) {
        return false;
    }


    // test if it is possible read a value
    ReferenceT<MemoryMapOutputWriter> writer = ReferenceT<MemoryMapOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<GAMGenericSignal> defOut = gam2->Find("Inputs.Noise");

    writer->SetApplication(app);
    writer->AddSignal(defOut);
    writer->Finalise();

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<GAMGenericSignal> defIn = gam1->Find("Outputs.Noise");
    reader->SetApplication(app);
    reader->AddSignal(defIn);
    reader->Finalise();

    ControlNoise *var = (ControlNoise *) writer->GetSignal(0);
    var->noiseValue = 3.5;
    writer->Write(0);

    reader->Read(0);
    ControlNoise *ret = (ControlNoise *) reader->GetSignal(0);
    if (ret->noiseValue != 3.5) {
        return false;
    }

    return true;
}

