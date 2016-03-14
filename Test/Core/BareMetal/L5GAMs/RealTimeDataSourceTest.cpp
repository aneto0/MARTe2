/**
 * @file RealTimeDataSourceTest.cpp
 * @brief Source file for class RealTimeDataSourceTest
 * @date 09/mar/2016
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
 * the class RealTimeDataSourceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSourceTest.h"
#include "ConfigurationDatabase.h"
#include "GAMTestHelper.h"
#include "RealTimeDataSourceDef.h"
#include "RealTimeGenericDataDef.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool RealTimeDataSourceTest::TestConstructor() {
    return true;
}

bool RealTimeDataSourceTest::TestInitialise() {
    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeDataSource");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+DDB1");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateAbsolute("+DDB2");
    cdb.Write("Class", "ReferenceContainer");

    cdb.MoveToRoot();

    RealTimeDataSource sourceDefs;

    if (!sourceDefs.Initialise(cdb)) {
        return false;
    }

    return sourceDefs.Size() == 2;
}

bool RealTimeDataSourceTest::TestAddDataDefinition_Final() {
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

    cdbDS.Write("Class", "RealTimeDataSource");
    cdbDS.Write("IsFinal", "true");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.MoveToRoot();

    ReferenceT<RealTimeDataSource> sourceDefs = ReferenceT<RealTimeDataSource>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam)) {
        return false;
    }

    return sourceDefs->Size() == 2;
}

bool RealTimeDataSourceTest::TestAddDataDefinitionFalse_Final() {
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

    cdbDS.Write("Class", "RealTimeDataSource");
    cdbDS.Write("IsFinal", "true");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.MoveToRoot();

    ReferenceT<RealTimeDataSource> sourceDefs = ReferenceT<RealTimeDataSource>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }

    return (!sourceDefs->AddDataDefinition(gam));
}

bool RealTimeDataSourceTest::TestAddDataDefinition_AlreadyExistentLeaf() {
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
    cdbPlant.Write("Type", "ControlIn");
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

    cdbDS.Write("Class", "RealTimeDataSource");
    cdbDS.Write("IsFinal", "true");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.MoveToRoot();

    ReferenceT<RealTimeDataSource> sourceDefs = ReferenceT<RealTimeDataSource>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

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

bool RealTimeDataSourceTest::TestAddDataDefinition_NotFinal() {

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

    cdbDS.Write("Class", "RealTimeDataSource");
    cdbDS.Write("IsFinal", "false");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.MoveToRoot();

    ReferenceT<RealTimeDataSource> sourceDefs = ReferenceT<RealTimeDataSource>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    if (!sourceDefs->Initialise(cdbDS)) {
        return false;
    }

    if (!sourceDefs->AddDataDefinition(gam)) {
        return false;
    }

    return sourceDefs->Size() == 3;

}

bool RealTimeDataSourceTest::TestVerify() {
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
    cdbPlant.Write("Type", "ControlIn");
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

    cdbDS.Write("Class", "RealTimeDataSource");
    cdbDS.Write("IsFinal", "true");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.MoveToRoot();

    ReferenceT<RealTimeDataSource> sourceDefs = ReferenceT<RealTimeDataSource>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

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

bool RealTimeDataSourceTest::TestVerify_TwoProducers() {
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
    cdbPlant.Write("Type", "ControlIn");
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

    cdbDS.Write("Class", "RealTimeDataSource");
    cdbDS.Write("IsFinal", "true");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.MoveToRoot();

    ReferenceT<RealTimeDataSource> sourceDefs = ReferenceT<RealTimeDataSource>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

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

bool RealTimeDataSourceTest::TestAllocate() {

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
    cdbPlant.Write("Type", "ControlIn");
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

    cdbDS.Write("Class", "RealTimeDataSource");
    cdbDS.Write("IsFinal", "true");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.MoveToRoot();

    ReferenceT<RealTimeDataSource> sourceDefs = ReferenceT<RealTimeDataSource>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sourceDefs->SetName("+Data");
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
    ReferenceT<RealTimeDataSourceOutputWriter> writer = ReferenceT<RealTimeDataSourceOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<RealTimeGenericDataDef> defOut = gam2->Find("+Outputs.+Error");

    ReferenceT<RealTimeApplication> app = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    app->Insert(sourceDefs);

    writer->SetApplication(app);
    writer->AddVariable(defOut);
    writer->Finalise();

    ReferenceT<RealTimeDataSourceInputReader> reader = ReferenceT<RealTimeDataSourceInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<RealTimeGenericDataDef> defIn = gam1->Find("+Inputs.+Error");
    reader->SetApplication(app);
    reader->AddVariable(defIn);
    reader->Finalise();

    TrackError *var = (TrackError *) writer->GetData(0);
    var->Par1 = 1;
    var->Par2 = 2;
    writer->Write(0);

    reader->Read(0);
    TrackError *ret = (TrackError *) reader->GetData(0);
    if (ret->Par1 != 1) {
        return false;
    }
    if (ret->Par2 != 2) {
        return false;
    }

    return true;

}

bool RealTimeDataSourceTest::TestAllocate_Structure() {

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
    cdb.Write("Default", "noiseValue=0.5");
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
    cdbPlant.Write("Type", "ControlIn");
    cdbPlant.Write("Default", "Par1=1 \n Par2=2");
    cdbPlant.Write("Path", "+DDB2.PlantControl");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.CreateAbsolute("+Inputs.+Noise");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "ControlNoise");
    cdbPlant.Write("Default", "noiseValue=0.5");
    cdbPlant.Write("Path", "+DDB2.PidNoise");
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

    cdbDS.Write("Class", "RealTimeDataSource");
    cdbDS.Write("IsFinal", "true");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.MoveToRoot();

    ReferenceT<RealTimeDataSource> sourceDefs = ReferenceT<RealTimeDataSource>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sourceDefs->SetName("+Data");
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
    ReferenceT<RealTimeDataSourceOutputWriter> writer = ReferenceT<RealTimeDataSourceOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<RealTimeGenericDataDef> defOut = gam2->Find("+Inputs.+Noise");

    ReferenceT<RealTimeApplication> app = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    app->Insert(sourceDefs);

    writer->SetApplication(app);
    writer->AddVariable(defOut);
    writer->Finalise();

    ReferenceT<RealTimeDataSourceInputReader> reader = ReferenceT<RealTimeDataSourceInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<RealTimeGenericDataDef> defIn = gam1->Find("+Outputs.+Noise");
    reader->SetApplication(app);
    reader->AddVariable(defIn);
    reader->Finalise();

    ControlNoise *var = (ControlNoise *) writer->GetData(0);
    var->noiseValue = 3.5;
    writer->Write(0);

    reader->Read(0);
    ControlNoise *ret = (ControlNoise *) reader->GetData(0);
    if (ret->noiseValue != 3.5) {
        return false;
    }

    return true;
}

bool RealTimeDataSourceTest::TestPrepareNextState() {

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
    cdbPlant.Write("Type", "ControlIn");
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.CreateAbsolute("+Inputs.+Control.+Par1");
    cdbPlant.Write("Class", "RealTimeGenericDataDef");
    cdbPlant.Write("Type", "uint32");
    cdbPlant.Write("Default", "1");
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
    cdbPlant.Write("IsFinal", "true");
    cdbPlant.Write("Path", "+DDB1.PlantOutput");
    cdbPlant.Write("Default", "Par1=3 \n Par2=4");
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

    cdbDS.Write("Class", "RealTimeDataSource");
    cdbDS.Write("IsFinal", "true");
    cdbDS.CreateAbsolute("+DDB1");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.CreateAbsolute("+DDB2");
    cdbDS.Write("Class", "ReferenceContainer");
    cdbDS.MoveToRoot();

    ReferenceT<RealTimeDataSource> sourceDefs = ReferenceT<RealTimeDataSource>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sourceDefs->SetName("+Data");
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

    RealTimeStateInfo info;
    info.currentState = "";
    info.nextState = "state1";
    info.activeBuffer = 1;

    if (!sourceDefs->PrepareNextState(info)) {
        return false;
    }

    // test if it is possible read a value
    ReferenceT<RealTimeApplication> app = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    app->Insert(sourceDefs);

    ReferenceT<RealTimeDataSourceInputReader> reader1 = ReferenceT<RealTimeDataSourceInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<RealTimeGenericDataDef> def1 = gam1->Find("+Inputs.+Error");
    reader1->SetApplication(app);
    reader1->AddVariable(def1);
    reader1->Finalise();

    reader1->Read(0);
    TrackError *ret1 = (TrackError *) reader1->GetData(0);
    if (ret1->Par1 != 1) {
        return false;
    }
    if (ret1->Par2 != 2) {
        return false;
    }

    ReferenceT<RealTimeDataSourceInputReader> reader2 = ReferenceT<RealTimeDataSourceInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<RealTimeGenericDataDef> def2 = gam2->Find("+Outputs.+Error");
    reader2->SetApplication(app);
    reader2->AddVariable(def2);
    reader2->Finalise();

    reader2->Read(0);
    TrackError *ret2 = (TrackError *) reader2->GetData(0);
    if (ret2->Par1 != 3) {
        return false;
    }
    if (ret2->Par2 != 4) {
        return false;
    }

    return true;

}

