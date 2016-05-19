/**
 * @file DataSourceContainerTest.cpp
 * @brief Source file for class DataSourceContainerTest
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
 * the class DataSourceContainerTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DataSourceContainerTest.h"

#include <DataSourceI.h>
#include <GAMDataSource.h>

#include "DataSourceTest.h"
#include "ConfigurationDatabase.h"
#include "GAMTestHelper.h"
#include "GAMGenericSignal.h"
#include "MemoryMapInputReader.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool DataSourceContainerTest::TestConstructor() {
    return true;
}

bool DataSourceContainerTest::TestInitialise() {
    ConfigurationDatabase cdb;

    cdb.Write("Class", "DataSource");
    cdb.Write("IsFinal", "true");
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

bool DataSourceContainerTest::TestAddDataDefinition_Final() {
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
            "            Default = 2\n"
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

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam->Initialise(cdb)) {
        return false;
    }
    gam->SetName("PID1");

    ConfigurationDatabase cdbDS;

    cdbDS.Write("Class", "DataSourceContainer");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "DataSource");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "DataSource");
    cdbDS.MoveToRoot();

    ReferenceT<DataSourceContainer> sourceDefs = ReferenceT<DataSourceContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }

    RealTimeApplication app;
    app.Insert(sourceDefs);
    gam->SetApplication(app);

    if (!sourceDefs->AddDataDefinition(gam)) {
        return false;
    }

    // consider the time data source
    return sourceDefs->Size() == 3;
}

bool DataSourceContainerTest::TestAddDataDefinitionFalse_InvalidPath() {
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
            "            Default = 2\n"
            "            Path = DDB3.PidNoise\n" // invalid path here
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

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam->Initialise(cdb)) {
        return false;
    }
    gam->SetName("PID1");

    ConfigurationDatabase cdbDS;

    cdbDS.Write("Class", "DataSourceContainer");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "DataSource");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "DataSource");
    cdbDS.MoveToRoot();

    ReferenceT<DataSourceContainer> sourceDefs = ReferenceT<DataSourceContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }
    RealTimeApplication app;
    app.Insert(sourceDefs);
    gam->SetApplication(app);

    return (!sourceDefs->AddDataDefinition(gam));
}

bool DataSourceContainerTest::TestAddDataDefinition_AlreadyExistentLeaf() {

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
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "DataSource");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "DataSource");
    cdbDS.MoveToRoot();

    ReferenceT<DataSourceContainer> sourceDefs = ReferenceT<DataSourceContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }
    RealTimeApplication app;
    app.Insert(sourceDefs);
    gam1->SetApplication(app);
    gam2->SetApplication(app);

    if (!sourceDefs->AddDataDefinition(gam1)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam2)) {
        return false;
    }

    // each definition must have one producer and one consumer
    ReferenceT<GAMDataSource> def1 = sourceDefs->Find("DDB1.PidError1");
    if (def1->GetNumberOfConsumers("state1") != 1 || def1->GetNumberOfProducers("state1") != 1) {
        printf("\n%d %d\n", def1->GetNumberOfConsumers("state1"), def1->GetNumberOfProducers("state1"));
        return false;
    }
    if (def1->GetNumberOfConsumers("state2") != 1 || def1->GetNumberOfProducers("state2") != 1) {
        printf("\n%d %d\n", def1->GetNumberOfConsumers("state2"), def1->GetNumberOfProducers("state2"));

        return false;
    }

    // each definition must have one producer and one consumer
    ReferenceT<GAMDataSource> def2 = sourceDefs->Find("DDB1.PidError2");
    if (def2->GetNumberOfConsumers("state1") != 1 || def2->GetNumberOfProducers("state1") != 1) {
        return false;
    }
    if (def2->GetNumberOfConsumers("state2") != 1 || def2->GetNumberOfProducers("state2") != 1) {
        return false;
    }

    // each definition must have one producer and one consumer
    ReferenceT<GAMDataSource> def3 = sourceDefs->Find("DDB2.PidControl1");
    if (def3->GetNumberOfConsumers("state1") != 1 || def3->GetNumberOfProducers("state1") != 1) {
        return false;
    }
    if (def3->GetNumberOfConsumers("state2") != 1 || def3->GetNumberOfProducers("state2") != 1) {
        return false;
    }

    // each definition must have one producer and one consumer
    ReferenceT<GAMDataSource> def4 = sourceDefs->Find("DDB2.PidControl2");
    if (def4->GetNumberOfConsumers("state1") != 1 || def4->GetNumberOfProducers("state1") != 1) {
        return false;
    }
    if (def4->GetNumberOfConsumers("state2") != 1 || def4->GetNumberOfProducers("state2") != 1) {
        return false;
    }

    return true;
}

bool DataSourceContainerTest::TestAddDataDefinitionFalse_InvalidGAM() {

    ReferenceT<PlantGAM> gam;
    ConfigurationDatabase cdbDS;

    cdbDS.Write("Class", "DataSourceContainer");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "DataSource");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "DataSource");
    cdbDS.MoveToRoot();

    ReferenceT<DataSourceContainer> sourceDefs = ReferenceT<DataSourceContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }

    return (!sourceDefs->AddDataDefinition(gam));
}

bool DataSourceContainerTest::TestVerify() {
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
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "DataSource");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "DataSource");
    cdbDS.MoveToRoot();

    ReferenceT<DataSourceContainer> sourceDefs = ReferenceT<DataSourceContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }

    RealTimeApplication app;
    app.Insert(sourceDefs);
    gam1->SetApplication(app);
    gam2->SetApplication(app);

    if (!sourceDefs->AddDataDefinition(gam1)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam2)) {
        return false;
    }
    return sourceDefs->Verify();
}

bool DataSourceContainerTest::TestVerify_TwoProducers() {
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
            "    IsOutput = true\n"   // wrong here
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
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "DataSource");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "DataSource");
    cdbDS.MoveToRoot();

    ReferenceT<DataSourceContainer> sourceDefs = ReferenceT<DataSourceContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }

    RealTimeApplication app;
    app.Insert(sourceDefs);
    gam1->SetApplication(app);
    gam2->SetApplication(app);

    if (!sourceDefs->AddDataDefinition(gam1)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam2)) {
        return false;
    }
    return !sourceDefs->Verify();
}

bool DataSourceContainerTest::TestAllocate() {
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
    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }
    RealTimeApplication app;
    app.Insert(sourceDefs);
    gam1->SetApplication(app);
    gam2->SetApplication(app);
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
    writer->AddSignal(defOut);
    writer->Finalise();

    ReferenceT<MemoryMapInputReader> reader = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<GAMGenericSignal> defIn = gam1->Find("Inputs.Error");
    reader->SetApplication(app);
    reader->AddSignal(defIn);
    reader->Finalise();

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

bool DataSourceContainerTest::TestAllocate_Structure() {

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
            "        Default = \"noiseValue=1\"\n"
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
            "        Default = \"noiseValue=1\"\n"
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
    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }
    RealTimeApplication app;
    app.Insert(sourceDefs);
    gam1->SetApplication(app);
    gam2->SetApplication(app);
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

bool DataSourceContainerTest::TestPrepareNextState() {

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
            "        Default = \"noiseValue=1\"\n"
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
            "}\n"
            "+Outputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsOutput = true\n"
            "    IsFinal = true\n"
            "    +Error = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = TrackError\n"
            "        Path = DDB1.PlantOutput\n"
            "        Default = \"Par1=3 Par2=4\"\n"   // structured init
            "        IsFinal = true\n"
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
    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }
    RealTimeApplication app;
    app.Insert(sourceDefs);
    gam1->SetApplication(app);
    gam2->SetApplication(app);
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

    RealTimeStateInfo info;
    info.currentState = "";
    info.nextState = "state1";
    info.activeBuffer = 1;

    if (!sourceDefs->PrepareNextState(info)) {
        return false;
    }

    // test if it is possible read a value

    ReferenceT<MemoryMapInputReader> reader1 = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<GAMGenericSignal> def1 = gam1->Find("Inputs.Error");
    reader1->SetApplication(app);
    reader1->AddSignal(def1);
    reader1->Finalise();

    reader1->Read(0);
    TrackError *ret1 = (TrackError *) reader1->GetSignal(0);
    if (ret1->Par1 != 1) {
        return false;
    }
    if (ret1->Par2 != 1) {
        return false;
    }

    ReferenceT<MemoryMapInputReader> reader2 = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<GAMGenericSignal> def2 = gam2->Find("Outputs.Error");
    reader2->SetApplication(app);
    reader2->AddSignal(def2);
    reader2->Finalise();

    reader2->Read(0);
    TrackError *ret2 = (TrackError *) reader2->GetSignal(0);
    if (ret2->Par1 != 3) {
        return false;
    }
    if (ret2->Par2 != 4) {
        return false;
    }

    return true;

}

bool DataSourceContainerTest::TestPrepareNextState_MultiDim_Vector() {
    StreamString confString = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = TrackErrorArray\n"
            "        IsFinal = false\n"
            "        +Pars = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Default = \"{1, 2}\"\n"
            "            Dimensions = \"[2]\"\n"
            "            Path = DDB1.PidError\n"
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
            "        IsFinal = false\n"
            "        +Pars = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Dimensions = \"[2]\"\n"
            "            Path = DDB2.PidControl\n"
            "            Default = \"{3,4}\"\n"
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

    ReferenceT<PIDGAM2> gam1 = ReferenceT<PIDGAM2>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam1->Initialise(cdb)) {
        return false;
    }
    gam1->SetName("PID1");
    gam1->AddState("state1", "thread1");
    gam1->AddState("state2", "thread1");

    ConfigurationDatabase cdbDS;

    cdbDS.Write("Class", "DataSourceContainer");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "DataSource");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "DataSource");
    cdbDS.MoveToRoot();

    ReferenceT<DataSourceContainer> sourceDefs = ReferenceT<DataSourceContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sourceDefs->SetName("Data");
    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }
    RealTimeApplication app;
    app.Insert(sourceDefs);
    gam1->SetApplication(app);
    if (!sourceDefs->AddDataDefinition(gam1)) {
        return false;
    }

    if (!sourceDefs->Verify()) {
        return false;
    }

    if (!sourceDefs->Allocate()) {
        return false;
    }

    RealTimeStateInfo info;
    info.currentState = "";
    info.nextState = "state1";
    info.activeBuffer = 1;

    if (!sourceDefs->PrepareNextState(info)) {
        return false;
    }
    // test if it is possible read a value

    ReferenceT<MemoryMapInputReader> reader1 = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<GAMGenericSignal> def1 = gam1->Find("Inputs.Error");
    reader1->SetApplication(app);
    reader1->AddSignal(def1);
    reader1->Finalise();

    reader1->Read(0);
    TrackErrorArray *ret1 = (TrackErrorArray *) reader1->GetSignal(0);
    if (ret1->Pars[0] != 1) {
        return false;
    }
    if (ret1->Pars[1] != 2) {
        return false;
    }

    ReferenceT<DataSource> ddb2 = sourceDefs->Get(1);

    ReferenceT<MemoryMapInputReader> reader2 = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<GAMGenericSignal> def2 = gam1->Find("Outputs.Control");
    reader2->SetApplication(app);
    reader2->AddSignal(def2);
    reader2->Finalise();

    reader2->Read(0);
    ControlInArray *ret2 = (ControlInArray *) reader2->GetSignal(0);
    if (ret2->Pars[0] != 3) {
        return false;
    }
    if (ret2->Pars[1] != 4) {
        return false;
    }
    return true;
}

bool DataSourceContainerTest::TestPrepareNextState_MultiDim_Matrix() {

    StreamString confString = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = false\n"
            "    +Error = {\n"
            "        Class = GAMGenericSignal\n"
            "        Type = TrackErrorMatrix\n"
            "        IsFinal = false\n"
            "        +Pars = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Default = \"{{1, 2}, {3,4}, {5,6}}\"\n"
            "            Dimensions = \"[3][2]\"\n"
            "            Path = DDB1.PidError\n"
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
            "        IsFinal = false\n"
            "        +Pars = {\n"
            "            Class = GAMGenericSignal\n"
            "            Type = uint32\n"
            "            Dimensions = \"[3][2]\"\n"
            "            Path = DDB2.PidControl\n"
            "            Default = \"{{7,8}, {9,10}, {11,12}}\"\n"
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

    ReferenceT<PIDGAM3> gam1 = ReferenceT<PIDGAM3>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam1->Initialise(cdb)) {
        return false;
    }
    gam1->SetName("PID1");
    gam1->AddState("state1", "thread1");
    gam1->AddState("state2", "thread1");

    ConfigurationDatabase cdbDS;

    cdbDS.Write("Class", "DataSourceContainer");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "DataSource");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "DataSource");
    cdbDS.MoveToRoot();

    ReferenceT<DataSourceContainer> sourceDefs = ReferenceT<DataSourceContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sourceDefs->SetName("Data");
    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }
    RealTimeApplication app;
    app.Insert(sourceDefs);
    gam1->SetApplication(app);
    if (!sourceDefs->AddDataDefinition(gam1)) {
        return false;
    }

    if (!sourceDefs->Verify()) {
        return false;
    }

    if (!sourceDefs->Allocate()) {
        return false;
    }

    RealTimeStateInfo info;
    info.currentState = "";
    info.nextState = "state1";
    info.activeBuffer = 1;

    if (!sourceDefs->PrepareNextState(info)) {
        return false;
    }

    // test if it is possible read a value

    ReferenceT<MemoryMapInputReader> reader1 = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<GAMGenericSignal> def1 = gam1->Find("Inputs.Error");
    reader1->SetApplication(app);
    reader1->AddSignal(def1);
    reader1->Finalise();

    reader1->Read(0);
    TrackErrorMatrix *ret1 = (TrackErrorMatrix *) reader1->GetSignal(0);
    if (ret1->Pars[0][0] != 1) {
        return false;
    }
    if (ret1->Pars[0][1] != 2) {
        return false;
    }
    if (ret1->Pars[1][0] != 3) {
        return false;
    }
    if (ret1->Pars[1][1] != 4) {
        return false;
    }
    if (ret1->Pars[2][0] != 5) {
        return false;
    }
    if (ret1->Pars[2][1] != 6) {
        return false;
    }

    ReferenceT<DataSource> ddb2 = sourceDefs->Get(1);

    ReferenceT<MemoryMapInputReader> reader2 = ReferenceT<MemoryMapInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<GAMGenericSignal> def2 = gam1->Find("Outputs.Control");
    reader2->SetApplication(app);
    reader2->AddSignal(def2);
    reader2->Finalise();

    reader2->Read(0);
    ControlInMatrix *ret2 = (ControlInMatrix *) reader2->GetSignal(0);
    if (ret2->Pars[0][0] != 7) {
        return false;
    }
    if (ret2->Pars[0][1] != 8) {
        return false;
    }
    if (ret2->Pars[1][0] != 9) {
        return false;
    }
    if (ret2->Pars[1][1] != 10) {
        return false;
    }
    if (ret2->Pars[2][0] != 11) {
        return false;
    }
    if (ret2->Pars[2][1] != 12) {
        return false;
    }
    return true;
}
