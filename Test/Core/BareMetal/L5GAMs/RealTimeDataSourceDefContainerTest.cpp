/**
 * @file RealTimeDataSourceDefContainerTest.cpp
 * @brief Source file for class RealTimeDataSourceDefContainerTest
 * @date 07/mar/2016
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
 * the class RealTimeDataSourceDefContainerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSourceDefContainerTest.h"
#include "ConfigurationDatabase.h"
#include "GAMTestHelper.h"
#include "RealTimeDataSourceDef.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool RealTimeDataSourceDefContainerTest::TestConstructor() {
    return true;
}

bool RealTimeDataSourceDefContainerTest::TestInitialise() {
    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeDataSourceDefContainer");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+DDB1");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateAbsolute("+DDB2");
    cdb.Write("Class", "ReferenceContainer");

    cdb.MoveToRoot();

    RealTimeDataSourceDefContainer sourceDefs;

    if (!sourceDefs.Initialise(cdb)) {
        return false;
    }

    return sourceDefs.Size() == 2;
}

bool RealTimeDataSourceDefContainerTest::TestAddDataDefinition_Final() {
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
    if (!gam->Initialise(cdb)) {
        return false;
    }
    gam->SetName("PID1");

    ConfigurationDatabase cdbDS;

    cdbDS.Write("Class", "RealTimeDataSourceDefContainer");
    cdbDS.Write("IsFinal", "true");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.MoveToRoot();

    ReferenceT<RealTimeDataSourceDefContainer> sourceDefs = ReferenceT<RealTimeDataSourceDefContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam)) {
        return false;
    }

    return sourceDefs->Size() == 2;
}

bool RealTimeDataSourceDefContainerTest::TestAddDataDefinitionFalse_Final() {
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
    cdb.Write("Path", "+DDB3.PidNoise");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam->Initialise(cdb)) {
        return false;
    }
    gam->SetName("PID1");

    ConfigurationDatabase cdbDS;

    cdbDS.Write("Class", "RealTimeDataSourceDefContainer");
    cdbDS.Write("IsFinal", "true");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.MoveToRoot();

    ReferenceT<RealTimeDataSourceDefContainer> sourceDefs = ReferenceT<RealTimeDataSourceDefContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }

    return (!sourceDefs->AddDataDefinition(gam));
}

bool RealTimeDataSourceDefContainerTest::TestAddDataDefinition_AlreadyExistentLeaf() {
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

    ReferenceT<PIDGAM> gam1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam1->Initialise(cdb)) {
        return false;
    }
    gam1->SetName("PID1");
    gam1->AddState("state1");
    gam1->AddState("state2");

    ConfigurationDatabase cdbPlant;

    cdbPlant.CreateAbsolute("+Inputs");
    cdbPlant.Write("Class", "RealTimeDataDefContainer");
    cdbPlant.Write("IsInput", "true");
    cdbPlant.Write("IsFinal", "false");
    cdbPlant.CreateAbsolute("+Inputs.+Control");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "TrackError");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.CreateAbsolute("+Inputs.+Control.+Par1");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "uint32");
    cdbPlant.Write("Default", "2");
    cdbPlant.Write("Path", "+DDB2.PidControl1");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.CreateAbsolute("+Inputs.+Control.+Par2");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "uint32");
    cdbPlant.Write("Default", "2");
    cdbPlant.Write("Path", "+DDB2.PidControl2");
    cdbPlant.Write("IsFinal", "true");

    cdbPlant.CreateAbsolute("+Outputs");
    cdbPlant.Write("Class", "RealTimeDataDefContainer");
    cdbPlant.Write("IsOutput", "true");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.CreateAbsolute("+Outputs.+Error");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "TrackError");
    cdbPlant.Write("IsFinal", "false");
    cdbPlant.CreateAbsolute("+Outputs.+Error.+Par1");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "uint32");
    cdbPlant.Write("Default", "1");
    cdbPlant.Write("Path", "+DDB1.PidError1");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.CreateAbsolute("+Outputs.+Error.+Par2");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "uint32");
    cdbPlant.Write("Default", "2");
    cdbPlant.Write("Path", "+DDB1.PidError2");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.MoveToRoot();

    ReferenceT<PlantGAM> gam2 = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!gam2->Initialise(cdbPlant)) {
        printf("\nfailed gam2 init\n");
        return false;
    }
    gam2->SetName("Plant");
    gam2->AddState("state1");
    gam2->AddState("state2");

    ConfigurationDatabase cdbDS;

    cdbDS.Write("Class", "RealTimeDataSourceDefContainer");
    cdbDS.Write("IsFinal", "true");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.MoveToRoot();

    ReferenceT<RealTimeDataSourceDefContainer> sourceDefs = ReferenceT<RealTimeDataSourceDefContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam1)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam2)) {
        return false;
    }

    // each definition must have one producer and one consumer
    ReferenceT<RealTimeDataSourceDef> def1 = sourceDefs->Find("+DDB1.PidError1");
    if (def1->GetNumberOfConsumers("state1") != 1 || def1->GetNumberOfProducers("state1") != 1) {
        printf("\n%d %d\n", def1->GetNumberOfConsumers("state1"), def1->GetNumberOfProducers("state1"));
        return false;
    }
    if (def1->GetNumberOfConsumers("state2") != 1 || def1->GetNumberOfProducers("state2") != 1) {
        printf("\n%d %d\n", def1->GetNumberOfConsumers("state2"), def1->GetNumberOfProducers("state2"));

        return false;
    }

    // each definition must have one producer and one consumer
    ReferenceT<RealTimeDataSourceDef> def2 = sourceDefs->Find("+DDB1.PidError2");
    if (def2->GetNumberOfConsumers("state1") != 1 || def2->GetNumberOfProducers("state1") != 1) {
        return false;
    }
    if (def2->GetNumberOfConsumers("state2") != 1 || def2->GetNumberOfProducers("state2") != 1) {
        return false;
    }

    // each definition must have one producer and one consumer
    ReferenceT<RealTimeDataSourceDef> def3 = sourceDefs->Find("+DDB2.PidControl1");
    if (def3->GetNumberOfConsumers("state1") != 1 || def3->GetNumberOfProducers("state1") != 1) {
        return false;
    }
    if (def3->GetNumberOfConsumers("state2") != 1 || def3->GetNumberOfProducers("state2") != 1) {
        return false;
    }

    // each definition must have one producer and one consumer
    ReferenceT<RealTimeDataSourceDef> def4 = sourceDefs->Find("+DDB2.PidControl2");
    if (def4->GetNumberOfConsumers("state1") != 1 || def4->GetNumberOfProducers("state1") != 1) {
        return false;
    }
    if (def4->GetNumberOfConsumers("state2") != 1 || def4->GetNumberOfProducers("state2") != 1) {
        return false;
    }

    return true;
}

bool RealTimeDataSourceDefContainerTest::TestAddDataDefinition_NotFinal() {

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
    cdb.Write("Path", "+DDB3.PidNoise");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam->Initialise(cdb)) {
        return false;
    }
    gam->SetName("PID1");

    ConfigurationDatabase cdbDS;

    cdbDS.Write("Class", "RealTimeDataSourceDefContainer");
    cdbDS.Write("IsFinal", "false");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.MoveToRoot();

    ReferenceT<RealTimeDataSourceDefContainer> sourceDefs = ReferenceT<RealTimeDataSourceDefContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam)) {
        return false;
    }

    return sourceDefs->Size() == 3;

}

bool RealTimeDataSourceDefContainerTest::TestVerify() {
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

    ReferenceT<PIDGAM> gam1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam1->Initialise(cdb)) {
        return false;
    }
    gam1->SetName("PID1");
    gam1->AddState("state1");
    gam1->AddState("state2");

    ConfigurationDatabase cdbPlant;

    cdbPlant.CreateAbsolute("+Inputs");
    cdbPlant.Write("Class", "RealTimeDataDefContainer");
    cdbPlant.Write("IsInput", "true");
    cdbPlant.Write("IsFinal", "false");
    cdbPlant.CreateAbsolute("+Inputs.+Control");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "TrackError");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.CreateAbsolute("+Inputs.+Control.+Par1");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "uint32");
    cdbPlant.Write("Default", "2");
    cdbPlant.Write("Path", "+DDB2.PidControl1");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.CreateAbsolute("+Inputs.+Control.+Par2");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "uint32");
    cdbPlant.Write("Default", "2");
    cdbPlant.Write("Path", "+DDB2.PidControl2");
    cdbPlant.Write("IsFinal", "true");

    cdbPlant.CreateAbsolute("+Outputs");
    cdbPlant.Write("Class", "RealTimeDataDefContainer");
    cdbPlant.Write("IsOutput", "true");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.CreateAbsolute("+Outputs.+Error");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "TrackError");
    cdbPlant.Write("IsFinal", "false");
    cdbPlant.CreateAbsolute("+Outputs.+Error.+Par1");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "uint32");
    cdbPlant.Write("Default", "1");
    cdbPlant.Write("Path", "+DDB1.PidError1");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.CreateAbsolute("+Outputs.+Error.+Par2");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "uint32");
    cdbPlant.Write("Default", "2");
    cdbPlant.Write("Path", "+DDB1.PidError2");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.MoveToRoot();

    ReferenceT<PlantGAM> gam2 = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!gam2->Initialise(cdbPlant)) {
        printf("\nfailed gam2 init\n");
        return false;
    }
    gam2->SetName("Plant");
    gam2->AddState("state1");
    gam2->AddState("state2");

    ConfigurationDatabase cdbDS;

    cdbDS.Write("Class", "RealTimeDataSourceDefContainer");
    cdbDS.Write("IsFinal", "true");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.MoveToRoot();

    ReferenceT<RealTimeDataSourceDefContainer> sourceDefs = ReferenceT<RealTimeDataSourceDefContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam1)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam2)) {
        return false;
    }
    return sourceDefs->Verify();
}

bool RealTimeDataSourceDefContainerTest::TestVerify_TwoProducers() {
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

    ReferenceT<PIDGAM> gam1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!gam1->Initialise(cdb)) {
        return false;
    }
    gam1->SetName("PID1");
    gam1->AddState("state1");
    gam1->AddState("state2");

    ConfigurationDatabase cdbPlant;

    cdbPlant.CreateAbsolute("+Inputs");
    cdbPlant.Write("Class", "RealTimeDataDefContainer");
    // set it as an output (wrong)
    cdbPlant.Write("IsOutput", "true");
    cdbPlant.Write("IsFinal", "false");
    cdbPlant.CreateAbsolute("+Inputs.+Control");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "TrackError");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.CreateAbsolute("+Inputs.+Control.+Par1");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "uint32");
    cdbPlant.Write("Default", "2");
    cdbPlant.Write("Path", "+DDB2.PidControl1");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.CreateAbsolute("+Inputs.+Control.+Par2");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "uint32");
    cdbPlant.Write("Default", "2");
    cdbPlant.Write("Path", "+DDB2.PidControl2");
    cdbPlant.Write("IsFinal", "true");

    cdbPlant.CreateAbsolute("+Outputs");
    cdbPlant.Write("Class", "RealTimeDataDefContainer");
    cdbPlant.Write("IsOutput", "true");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.CreateAbsolute("+Outputs.+Error");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "TrackError");
    cdbPlant.Write("IsFinal", "false");
    cdbPlant.CreateAbsolute("+Outputs.+Error.+Par1");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "uint32");
    cdbPlant.Write("Default", "1");
    cdbPlant.Write("Path", "+DDB1.PidError1");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.CreateAbsolute("+Outputs.+Error.+Par2");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "uint32");
    cdbPlant.Write("Default", "2");
    cdbPlant.Write("Path", "+DDB1.PidError2");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.MoveToRoot();

    ReferenceT<PlantGAM> gam2 = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!gam2->Initialise(cdbPlant)) {
        printf("\nfailed gam2 init\n");
        return false;
    }
    gam2->SetName("Plant");
    gam2->AddState("state1");
    gam2->AddState("state2");

    ConfigurationDatabase cdbDS;

    cdbDS.Write("Class", "RealTimeDataSourceDefContainer");
    cdbDS.Write("IsFinal", "true");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.MoveToRoot();

    ReferenceT<RealTimeDataSourceDefContainer> sourceDefs = ReferenceT<RealTimeDataSourceDefContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam1)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam2)) {
        return false;
    }
    return !sourceDefs->Verify();
}
