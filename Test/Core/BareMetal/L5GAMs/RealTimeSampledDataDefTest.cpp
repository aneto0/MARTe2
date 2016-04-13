/**
 * @file RealTimeSampledDataDefTest.cpp
 * @brief Source file for class RealTimeSampledDataDefTest
 * @date 04/mar/2016
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
 * the class RealTimeSampledDataDefTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeSampledDataDefTest.h"
#include "ConfigurationDatabase.h"
#include "ReferenceT.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool RealTimeSampledDataDefTest::TestConstructor() {
    RealTimeSampledDataDef def;
    if (def.GetSamples() != 0) {
        return false;
    }
    return def.GetSamplesPerCycle() == 0;
}

bool RealTimeSampledDataDefTest::TestInitialise(StructuredDataI & data) {
    RealTimeSampledDataDef def;
    return def.Initialise(data);
}

bool RealTimeSampledDataDefTest::TestMergeWithLocal_AddSamples() {

    ConfigurationDatabase localData;
    int32 localSamples = 2;
    localData.Write("Samples", localSamples);

    ConfigurationDatabase cdb;
    int32 samplesPerCycle = 3;
    cdb.Write("SamplesPerCycle", samplesPerCycle);
    // not final by default

    ReferenceT<RealTimeSampledDataDef> def = ReferenceT<RealTimeSampledDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetSamples() != localSamples) {
        return false;
    }

    return def->GetSamplesPerCycle() == samplesPerCycle;
}

bool RealTimeSampledDataDefTest::TestMergeWithLocal_AddSamplesPerCycle() {
    ConfigurationDatabase localData;
    int32 localSamplesPerCycle = 2;
    localData.Write("SamplesPerCycle", localSamplesPerCycle);

    ConfigurationDatabase cdb;
    int32 samples = 3;
    cdb.Write("Samples", samples);
    // not final by default

    ReferenceT<RealTimeSampledDataDef> def = ReferenceT<RealTimeSampledDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetSamplesPerCycle() != localSamplesPerCycle) {
        return false;
    }

    return def->GetSamples() == samples;

}

bool RealTimeSampledDataDefTest::TestMergeWithLocal_AddPath() {
    ConfigurationDatabase localData;
    int32 localSamplesPerCycle = 2;
    localData.Write("SamplesPerCycle", localSamplesPerCycle);
    localData.Write("Path", "DDB.adcIn");

    ConfigurationDatabase cdb;
    int32 samples = 3;
    cdb.Write("Samples", samples);
    // not final by default

    ReferenceT<RealTimeSampledDataDef> def = ReferenceT<RealTimeSampledDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetSamplesPerCycle() != localSamplesPerCycle) {
        return false;
    }

    if (def->GetSamples() != samples) {
        return false;
    }
    return StringHelper::Compare("DDB.adcIn", def->GetPath()) == 0;

}

bool RealTimeSampledDataDefTest::TestMergeWithLocal_AddType() {
    ConfigurationDatabase localData;
    int32 localSamplesPerCycle = 2;
    localData.Write("SamplesPerCycle", localSamplesPerCycle);
    localData.Write("Type", "ControlIn");

    ConfigurationDatabase cdb;
    int32 samples = 3;
    cdb.Write("Samples", samples);
    // not final by default

    ReferenceT<RealTimeSampledDataDef> def = ReferenceT<RealTimeSampledDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetSamplesPerCycle() != localSamplesPerCycle) {
        return false;
    }

    if (def->GetSamples() != samples) {
        return false;
    }
    return StringHelper::Compare("ControlIn", def->GetType()) == 0;

}

bool RealTimeSampledDataDefTest::TestMergeWithLocal_AddModifiers() {
    ConfigurationDatabase localData;
    int32 localSamplesPerCycle = 2;
    localData.Write("SamplesPerCycle", localSamplesPerCycle);
    localData.Write("Modifiers", "[2][2]");

    ConfigurationDatabase cdb;
    int32 samples = 3;
    cdb.Write("Samples", samples);
    cdb.Write("Type", "ControlIn");
    // not final by default

    ReferenceT<RealTimeSampledDataDef> def = ReferenceT<RealTimeSampledDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetSamplesPerCycle() != localSamplesPerCycle) {
        return false;
    }

    if (def->GetSamples() != samples) {
        return false;
    }

    return (def->GetNumberOfDimensions() == 2);

}

bool RealTimeSampledDataDefTest::TestMergeWithLocalFalse_FinalGlobal() {

    ConfigurationDatabase localData;
    int32 localSamples = 2;
    localData.Write("Samples", localSamples);

    ConfigurationDatabase cdb;
    int32 samplesPerCycle = 3;
    cdb.Write("SamplesPerCycle", samplesPerCycle);
    cdb.Write("IsFinal", "true");
    // not final by default

    ReferenceT<RealTimeSampledDataDef> def = ReferenceT<RealTimeSampledDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetSamples() != 0) {
        return false;
    }

    return def->GetSamplesPerCycle() == samplesPerCycle;

}

bool RealTimeSampledDataDefTest::TestMergeWithLocal_DifferentSamples() {
    ConfigurationDatabase localData;
    int32 localSamplesPerCycle = 2;
    int32 localSamples = 1;
    localData.Write("SamplesPerCycle", localSamplesPerCycle);
    localData.Write("Samples", localSamples);

    ConfigurationDatabase cdb;
    int32 samples = 3;
    cdb.Write("Samples", samples);
    // not final by default

    ReferenceT<RealTimeSampledDataDef> def = ReferenceT<RealTimeSampledDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetSamplesPerCycle() != localSamplesPerCycle) {
        return false;
    }

    return def->GetSamples() == samples;
}

bool RealTimeSampledDataDefTest::TestMergeWithLocal_DifferentSamplesPerCycle() {
    ConfigurationDatabase localData;
    int32 localSamplesPerCycle = 2;
    int32 localSamples = 1;
    localData.Write("SamplesPerCycle", localSamplesPerCycle);
    localData.Write("Samples", localSamples);

    ConfigurationDatabase cdb;
    int32 samplesPerCycle = 3;
    cdb.Write("SamplesPerCycle", samplesPerCycle);
    // not final by default

    ReferenceT<RealTimeSampledDataDef> def = ReferenceT<RealTimeSampledDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetSamplesPerCycle() != samplesPerCycle) {
        return false;
    }

    return def->GetSamples() == localSamples;
}

bool RealTimeSampledDataDefTest::TestMergeWithLocal_DifferentType() {
    ConfigurationDatabase localData;
    int32 localSamplesPerCycle = 2;
    localData.Write("SamplesPerCycle", localSamplesPerCycle);
    localData.Write("Type", "ControlOut");

    ConfigurationDatabase cdb;
    int32 samples = 3;
    cdb.Write("Samples", samples);
    cdb.Write("Type", "ControlIn");
    // not final by default

    ReferenceT<RealTimeSampledDataDef> def = ReferenceT<RealTimeSampledDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetSamplesPerCycle() != localSamplesPerCycle) {
        return false;
    }

    if (def->GetSamples() != samples) {
        return false;
    }
    return StringHelper::Compare("ControlIn", def->GetType()) == 0;
}

bool RealTimeSampledDataDefTest::TestMergeWithLocal_DifferentPath() {
    ConfigurationDatabase localData;
    int32 localSamplesPerCycle = 2;
    localData.Write("SamplesPerCycle", localSamplesPerCycle);
    localData.Write("Path", "DDB2.adcIn");

    ConfigurationDatabase cdb;
    int32 samples = 3;
    cdb.Write("Samples", samples);
    cdb.Write("Path", "DDB.adcIn");
    // not final by default

    ReferenceT<RealTimeSampledDataDef> def = ReferenceT<RealTimeSampledDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetSamplesPerCycle() != localSamplesPerCycle) {
        return false;
    }

    if (def->GetSamples() != samples) {
        return false;
    }
    return StringHelper::Compare("DDB.adcIn", def->GetPath()) == 0;

}


bool RealTimeSampledDataDefTest::TestMergeWithLocal_DifferentModifiers(){
    ConfigurationDatabase localData;
    int32 localSamplesPerCycle = 2;
    localData.Write("SamplesPerCycle", localSamplesPerCycle);
    localData.Write("Modifiers", "[3]");

    ConfigurationDatabase cdb;
    int32 samples = 3;
    cdb.Write("Samples", samples);
    localData.Write("Modifiers", "[2][2]");

    // not final by default

    ReferenceT<RealTimeSampledDataDef> def = ReferenceT<RealTimeSampledDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->MergeWithLocal(localData)) {
        return false;
    }

    if (def->GetSamplesPerCycle() != localSamplesPerCycle) {
        return false;
    }

    if (def->GetSamples() != samples) {
        return false;
    }
    return def->GetNumberOfDimensions() == 2;
}


bool RealTimeSampledDataDefTest::TestVerify() {

    ConfigurationDatabase cdb;
    cdb.Write("Class", "RealTimeSampledDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "true");
    int32 samples = 10;
    int32 samplesPerCycle = 3;
    cdb.Write("Samples", samples);
    cdb.Write("SamplesPerCycle", samplesPerCycle);
    ReferenceT<RealTimeSampledDataDef> def = ReferenceT<RealTimeSampledDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    if (!def->Verify()) {
        return false;
    }

    if (def->GetSamplesPerCycle() != samplesPerCycle) {
        return false;
    }

    return def->GetSamples() == samples;

}

bool RealTimeSampledDataDefTest::TestVerifyFalse_UnregisteredType() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "RealTimeSampledDataDef");
    cdb.Write("Type", "invalid");
    cdb.Write("IsFinal", "true");
    int32 samples = 10;
    int32 samplesPerCycle = 3;
    cdb.Write("Samples", samples);
    cdb.Write("SamplesPerCycle", samplesPerCycle);
    ReferenceT<RealTimeSampledDataDef> def = ReferenceT<RealTimeSampledDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }

    return !def->Verify();

}

bool RealTimeSampledDataDefTest::TestVerifyFalse_UnintrospectableType() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "RealTimeSampledDataDef");
    cdb.Write("Type", "Object");
    cdb.Write("IsFinal", "true");
    int32 samples = 10;
    int32 samplesPerCycle = 3;
    cdb.Write("Samples", samples);
    cdb.Write("SamplesPerCycle", samplesPerCycle);
    ReferenceT<RealTimeSampledDataDef> def = ReferenceT<RealTimeSampledDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }
    return !def->Verify();
}

bool RealTimeSampledDataDefTest::TestVerifyFalse_EmptyType() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "RealTimeSampledDataDef");
    cdb.Write("IsFinal", "true");
    int32 samples = 10;
    int32 samplesPerCycle = 3;
    cdb.Write("Samples", samples);
    cdb.Write("SamplesPerCycle", samplesPerCycle);
    ReferenceT<RealTimeSampledDataDef> def = ReferenceT<RealTimeSampledDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }
    return !def->Verify();

}

bool RealTimeSampledDataDefTest::TestVerifyFalse_UnsupportedMultiDim(){
    ConfigurationDatabase cdb;
    cdb.Write("Class", "RealTimeSampledDataDef");
    cdb.Write("IsFinal", "true");
    cdb.Write("Type", "ControlIn");
    cdb.Write("Modifiers", "[2]");
    int32 samples = 10;
    int32 samplesPerCycle = 3;
    cdb.Write("Samples", samples);
    cdb.Write("SamplesPerCycle", samplesPerCycle);
    ReferenceT<RealTimeSampledDataDef> def = ReferenceT<RealTimeSampledDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def->SetName("ADC");
    if (!def->Initialise(cdb)) {
        return false;
    }
    return !def->Verify();
}



bool RealTimeSampledDataDefTest::TestGetSamples() {

    RealTimeSampledDataDef def;

    if (def.GetSamples() != 0) {
        return false;
    }

    ConfigurationDatabase cdb;
    int32 samples = 5;
    cdb.Write("Samples", samples);

    def.Initialise(cdb);
    return def.GetSamples() == 5;
}

bool RealTimeSampledDataDefTest::TestGetSamplesPerCycle() {
    RealTimeSampledDataDef def;

    if (def.GetSamplesPerCycle() != 0) {
        return false;
    }

    ConfigurationDatabase cdb;
    int32 samplesPerCycle = 5;
    cdb.Write("SamplesPerCycle", samplesPerCycle);

    def.Initialise(cdb);
    return def.GetSamplesPerCycle() == 5;
}

bool RealTimeSampledDataDefTest::TestToStructuredData() {
    RealTimeSampledDataDef def;
    def.SetName("Control");
    ConfigurationDatabase cdb;

    cdb.Write("Type", "ControlIn");
    cdb.Write("Path", "DDB.control");
    cdb.Write("Modifiers", "[2][3]");
    int32 samplesPerCycle = 1;
    cdb.Write("SamplesPerCycle", samplesPerCycle);
    int32 samples = 2;
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

    StreamString testOut = "Control = {\n"
            "Class = RealTimeSampledDataDef\n"
            "Type = ControlIn\n"
            "Path = DDB.control\n"
            "NumberOfDimensions = 2\n"
            "NumberOfElements = { 3 2 1 } \n"
            "Samples = 2\n"
            "SamplesPerCycle = 1\n"
            "}\n";

    printf("\n%s\n", display.Buffer());
    return testOut == display;
}
