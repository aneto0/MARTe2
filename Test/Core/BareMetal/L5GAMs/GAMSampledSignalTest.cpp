/**
 * @file GAMSampledSignalTest.cpp
 * @brief Source file for class GAMSampledSignalTest
 * @date 12/apr/2016
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
 * the class GAMSampledSignalTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMSampledSignalTest.h"
#include "ConfigurationDatabase.h"
#include "ReferenceT.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool GAMSampledSignalTest::TestConstructor() {
    GAMSampledSignal def;
    if (StringHelper::Compare(def.GetSamples(), "") != 0) {
        return false;
    }
    return def.GetCycles() == 0;
}

bool GAMSampledSignalTest::TestInitialise(StructuredDataI & data) {
    GAMSampledSignal def;
    return def.Initialise(data);
}

bool GAMSampledSignalTest::TestMergeWithLocal_AddSamples() {

    ConfigurationDatabase localData;
    StreamString localSamples = "{{1,2,100}}";
    localData.Write("Samples", localSamples);

    ConfigurationDatabase cdb;
    uint32 cycles = 3;
    cdb.Write("Cycles", cycles);
    // not final by default

    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (localSamples != def->GetSamples()) {
        return false;
    }

    return def->GetCycles() == cycles;
}

bool GAMSampledSignalTest::TestMergeWithLocal_AddCycles() {
    ConfigurationDatabase localData;
    uint32 localCycles = 2;
    localData.Write("Cycles", localCycles);

    ConfigurationDatabase cdb;
    StreamString samples = "{{1,2,100}}";
    cdb.Write("Samples", samples);
    // not final by default

    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetCycles() != localCycles) {
        return false;
    }

    return samples == def->GetSamples();

}

bool GAMSampledSignalTest::TestMergeWithLocal_AddPath() {
    ConfigurationDatabase localData;
    uint32 localCycles = 2;
    localData.Write("Cycles", localCycles);
    localData.Write("Path", "DDB.adcIn");

    ConfigurationDatabase cdb;
    StreamString samples = "{{1,2,100}}";
    cdb.Write("Samples", samples);
    // not final by default

    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetCycles() != localCycles) {
        return false;
    }

    if (samples != def->GetSamples()) {
        return false;
    }
    return StringHelper::Compare("DDB.adcIn", def->GetPath()) == 0;

}

bool GAMSampledSignalTest::TestMergeWithLocal_AddType() {
    ConfigurationDatabase localData;
    uint32 localCycles = 2;
    localData.Write("Cycles", localCycles);
    localData.Write("Type", "ControlIn");

    ConfigurationDatabase cdb;
    StreamString samples = "{{1,2,100}}";
    cdb.Write("Samples", samples);
    // not final by default

    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetCycles() != localCycles) {
        return false;
    }

    if (samples != def->GetSamples()) {
        return false;
    }
    return StringHelper::Compare("ControlIn", def->GetType()) == 0;

}

bool GAMSampledSignalTest::TestMergeWithLocal_AddDimensions() {
    ConfigurationDatabase localData;
    uint32 localCycles = 2;
    localData.Write("Cycles", localCycles);
    localData.Write("Dimensions", "[2][2]");

    ConfigurationDatabase cdb;
    StreamString samples = "{{1,2,100}}";
    cdb.Write("Samples", samples);
    cdb.Write("Type", "ControlIn");
    // not final by default

    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetCycles() != localCycles) {
        return false;
    }

    if (samples != def->GetSamples()) {
        return false;
    }

    return (def->GetNumberOfDimensions() == 2);

}

bool GAMSampledSignalTest::TestMergeWithLocal_AddOperation() {
    ConfigurationDatabase localData;
    uint32 localCycles = 2;
    localData.Write("Cycles", localCycles);
    localData.Write("Operation", "{{1,1},{10,10}}");

    ConfigurationDatabase cdb;
    StreamString samples = "{{1,2,100}}";
    cdb.Write("Samples", samples);
    cdb.Write("Type", "ControlIn");
    // not final by default

    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetCycles() != localCycles) {
        return false;
    }

    if (samples != def->GetSamples()) {
        return false;
    }

    return StringHelper::Compare(def->GetOperation(), "{{1,1},{10,10}}") == 0;

}

bool GAMSampledSignalTest::TestMergeWithLocalFalse_FinalGlobal() {

    ConfigurationDatabase localData;
    StreamString localSamples = "{{1,2,100}}";
    localData.Write("Samples", localSamples);

    ConfigurationDatabase cdb;
    uint32 cycles = 3;
    cdb.Write("Cycles", cycles);
    cdb.Write("IsFinal", "true");
    // not final by default

    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (def->MergeWithLocal(localData)) {
        return false;
    }

    if (localSamples == def->GetSamples()) {
        return false;
    }

    return def->GetCycles() == cycles;

}

bool GAMSampledSignalTest::TestMergeWithLocal_DifferentSamples() {
    ConfigurationDatabase localData;
    uint32 localCycles = 2;
    StreamString localSamples = "{{1,2,100}}";
    localData.Write("Cycles", localCycles);
    localData.Write("Samples", localSamples);

    ConfigurationDatabase cdb;
    StreamString samples = "{{1,3,100}}";
    cdb.Write("Samples", samples);
    // not final by default

    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetCycles() != localCycles) {
        return false;
    }

    return samples == def->GetSamples();
}

bool GAMSampledSignalTest::TestMergeWithLocal_DifferentCycles() {
    ConfigurationDatabase localData;
    uint32 localCycles = 2;
    StreamString localSamples = "{{1,2,100}}";
    localData.Write("Cycles", localCycles);
    localData.Write("Samples", localSamples);

    ConfigurationDatabase cdb;
    uint32 cycles = 3;
    cdb.Write("Cycles", cycles);
    // not final by default

    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetCycles() != cycles) {
        return false;
    }

    return localSamples == def->GetSamples();
}

bool GAMSampledSignalTest::TestMergeWithLocal_DifferentType() {
    ConfigurationDatabase localData;
    uint32 localCycles = 2;
    localData.Write("Cycles", localCycles);
    localData.Write("Type", "ControlOut");

    ConfigurationDatabase cdb;
    StreamString samples = "{{1,2,100}}";
    cdb.Write("Samples", samples);
    cdb.Write("Type", "ControlIn");
    // not final by default

    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetCycles() != localCycles) {
        return false;
    }

    if (samples != def->GetSamples()) {
        return false;
    }
    return StringHelper::Compare("ControlIn", def->GetType()) == 0;
}

bool GAMSampledSignalTest::TestMergeWithLocal_DifferentPath() {
    ConfigurationDatabase localData;
    uint32 localCycles = 2;
    localData.Write("Cycles", localCycles);
    localData.Write("Path", "DDB2.adcIn");

    ConfigurationDatabase cdb;
    StreamString samples = "{{1,2,100}}";
    cdb.Write("Samples", samples);
    cdb.Write("Path", "DDB.adcIn");
    // not final by default

    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetCycles() != localCycles) {
        return false;
    }

    if (samples != def->GetSamples()) {
        return false;
    }
    return StringHelper::Compare("DDB.adcIn", def->GetPath()) == 0;

}

bool GAMSampledSignalTest::TestMergeWithLocal_DifferentOperation() {
    ConfigurationDatabase localData;
    uint32 localCycles = 2;
    localData.Write("Cycles", localCycles);
    localData.Write("Operation", "{{1,1}},{{3,4}}");

    ConfigurationDatabase cdb;
    StreamString samples = "{{1,2,100}}";
    cdb.Write("Samples", samples);
    cdb.Write("Operation", "{{1,1}}");

    // not final by default

    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetCycles() != localCycles) {
        return false;
    }

    if (samples != def->GetSamples()) {
        return false;
    }
    return def->GetNumberOfDimensions() == 0;
}

bool GAMSampledSignalTest::TestMergeWithLocal_DifferentDimensions() {
    ConfigurationDatabase localData;
    uint32 localCycles = 2;
    localData.Write("Cycles", localCycles);
    localData.Write("Dimensions", "[3]");

    ConfigurationDatabase cdb;
    StreamString samples = "{{1,2,100}}";
    cdb.Write("Samples", samples);
    cdb.Write("Dimensions", "[2][2]");

    // not final by default

    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetCycles() != localCycles) {
        return false;
    }

    if (samples != def->GetSamples()) {
        return false;
    }
    return def->GetNumberOfDimensions() == 2;
}

bool GAMSampledSignalTest::TestVerify() {

    ConfigurationDatabase cdb;
    cdb.Write("Class", "GAMSampledSignal");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "true");
    StreamString samples = "{{1,2,100}}";
    uint32 cycles = 3;
    cdb.Write("Samples", samples);
    cdb.Write("Cycles", cycles);
    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->Verify()) {
        return false;
    }

    if (def->GetCycles() != cycles) {
        return false;
    }

    return samples == def->GetSamples();

}

bool GAMSampledSignalTest::TestVerifyFalse_UnregisteredType() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "GAMSampledSignal");
    cdb.Write("Type", "invalid");
    cdb.Write("IsFinal", "true");
    StreamString samples = "{{1,2,100}}";
    uint32 cycles = 3;
    cdb.Write("Samples", samples);
    cdb.Write("Cycles", cycles);
    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    return !def->Verify();

}

bool GAMSampledSignalTest::TestVerifyFalse_UnintrospectableType() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "GAMSampledSignal");
    cdb.Write("Type", "Object");
    cdb.Write("IsFinal", "true");
    StreamString samples = "{{1,2,100}}";
    uint32 cycles = 3;
    cdb.Write("Samples", samples);
    cdb.Write("Cycles", cycles);
    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }
    return !def->Verify();
}

bool GAMSampledSignalTest::TestVerifyFalse_EmptyType() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "GAMSampledSignal");
    cdb.Write("IsFinal", "true");
    StreamString samples = "{{1,2,100}}";
    uint32 cycles = 3;
    cdb.Write("Samples", samples);
    cdb.Write("Cycles", cycles);
    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }
    return !def->Verify();

}

bool GAMSampledSignalTest::TestVerifyFalse_UnsupportedMultiDim() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "GAMSampledSignal");
    cdb.Write("IsFinal", "true");
    cdb.Write("Type", "ControlIn");
    cdb.Write("Dimensions", "[2]");
    StreamString samples = "{{1,2,100}}";
    uint32 cycles = 3;
    cdb.Write("Samples", samples);
    cdb.Write("Cycles", cycles);
    ReferenceT<GAMSampledSignal> def = ReferenceT<GAMSampledSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }
    return !def->Verify();
}

bool GAMSampledSignalTest::TestGetSamples() {

    GAMSampledSignal def;

    if (StringHelper::Compare(def.GetSamples(), "") != 0) {
        return false;
    }

    ConfigurationDatabase cdb;
    StreamString samples = "{{1,2,100}}";
    cdb.Write("Samples", samples);

    def.Initialise(cdb);
    return samples == def.GetSamples();
}

bool GAMSampledSignalTest::TestGetCycles() {
    GAMSampledSignal def;

    if (def.GetCycles() != 0) {
        return false;
    }

    ConfigurationDatabase cdb;
    uint32 cycles = 5;
    cdb.Write("Cycles", cycles);

    def.Initialise(cdb);
    return def.GetCycles() == cycles;
}

bool GAMSampledSignalTest::TestToStructuredData() {
    GAMSampledSignal def;
    def.SetName("Control");
    ConfigurationDatabase cdb;

    cdb.Write("Type", "ControlIn");
    cdb.Write("Path", "DDB.control");
    cdb.Write("Dimensions", "[2][3]");
    cdb.Write("Operation", "{{0,5}}");
    uint32 cycles = 1;
    cdb.Write("Cycles", cycles);
    StreamString samples = "{{1,2,100}}";
    cdb.Write("Samples", samples);

    if (!def.Initialise(cdb)) {
        return false;
    }

    ConfigurationDatabase output;
    if (!def.ToStructuredData(output)) {
        return false;
    }

    StreamString display;
    display.Printf("%!", output);

    StreamString testOut = "+Control = {\n"
            "Class = \"GAMSampledSignal\"\n"
            "Type = \"ControlIn\"\n"
            "Path = \"DDB.control\"\n"
            "Dimensions = \"[2][3]\"\n"
            "Operation = \"{{0,5}}\"\n"
            "Samples = \"{{1,2,100}}\"\n"
            "Cycles = +1\n"
            "}\n";

    printf("\n%s\n", display.Buffer());
    return testOut == display;
}
