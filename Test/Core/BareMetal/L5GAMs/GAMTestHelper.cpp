/**
 * @file GAMTestHelper.cpp
 * @brief Source file for class GAMTestHelper
 * @date 01/mar/2016
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
 * the class GAMTestHelper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMTestHelper.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
DECLARE_CLASS_MEMBER(TrackError, Par1, uint32, "", "");

DECLARE_CLASS_MEMBER(TrackError, Par2, uint32, "", "");

static const IntrospectionEntry* TrackErrorEntries[] = { &TrackError_Par1_introspectionEntry, &TrackError_Par2_introspectionEntry, 0 };

DECLARE_CLASS_INTROSPECTION(TrackError, TrackErrorEntries);
INTROSPECTION_REGISTER(TrackError, "1.0", TrackError_introspection)

DECLARE_CLASS_MEMBER(ControlIn, Par1, uint32, "", "");

DECLARE_CLASS_MEMBER(ControlIn, Par2, uint32, "", "");

static const IntrospectionEntry* ControlInEntries[] = { &ControlIn_Par1_introspectionEntry, &ControlIn_Par2_introspectionEntry, 0 };

DECLARE_CLASS_INTROSPECTION(ControlIn, ControlInEntries);
INTROSPECTION_REGISTER(ControlIn, "1.0", ControlIn_introspection)

DECLARE_CLASS_MEMBER(ControlNoise, noiseValue, float32, "", "");
static const IntrospectionEntry* ControlNoiseEntries[] = { &ControlNoise_noiseValue_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(ControlNoise, ControlNoiseEntries);
INTROSPECTION_REGISTER(ControlNoise, "1.0", ControlNoise_introspection)

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

void PIDGAM::Execute(uint8 activeContextBuffer) {

}

PIDGAM::~PIDGAM() {
    if (localData != NULL) {
        delete localData;
    }
}

void PIDGAM::SetUp() {
    localData = new ConfigurationDatabase();
    localData->CreateAbsolute("+Inputs");
    localData->Write("Class", "RealTimeDataDefContainer");
    localData->Write("IsInput", "true");
    localData->Write("IsFinal", "false");
    localData->CreateAbsolute("+Inputs.+Error");
    localData->Write("Class", "RealTimeGenericDataDef");
    localData->Write("Type", "TrackError");
    localData->Write("IsFinal", "false");
    localData->CreateAbsolute("+Inputs.+Error.+Par1");
    localData->Write("Class", "RealTimeGenericDataDef");
    localData->Write("Type", "uint32");
    localData->Write("Default", "1");
    localData->Write("Path", "+DDB1.PidError1");
    localData->Write("IsFinal", "true");

    localData->CreateAbsolute("+Outputs");
    localData->Write("Class", "RealTimeDataDefContainer");
    localData->Write("IsOutput", "true");
    localData->Write("IsFinal", "false");
    localData->CreateAbsolute("+Outputs.+Control");
    localData->Write("Class", "RealTimeGenericDataDef");
    localData->Write("Type", "ControlIn");
    localData->Write("IsFinal", "false");
    localData->CreateAbsolute("+Outputs.+Control.+Par1");
    localData->Write("Class", "RealTimeGenericDataDef");
    localData->Write("Type", "uint32");
    localData->Write("Path", "+DDB2.PidControl1");
    localData->Write("Default", "1");
    localData->Write("IsFinal", "true");
    localData->MoveToRoot();
}
CLASS_REGISTER(PIDGAM, "1.0")

void PIDGAMGroup::SetUp() {
    context = 1u;
}

void PIDGAMGroup::PrepareNextState(const RealTimeStateInfo &status) {

}

uint32 PIDGAMGroup::GetContext() {
    return context;
}
CLASS_REGISTER(PIDGAMGroup, "1.0")

void PlantGAM::SetUp() {

}

void PlantGAM::Execute(uint8 activeContextBuffer) {

}

CLASS_REGISTER(PlantGAM, "1.0")
