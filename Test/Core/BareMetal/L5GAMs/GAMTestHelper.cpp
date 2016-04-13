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
#include "stdio.h"
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

DECLARE_CLASS_MEMBER(TrackErrorArray, Pars, uint32, "[2]", "");
static const IntrospectionEntry* TrackErrorArrayEntries[] = { &TrackErrorArray_Pars_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(TrackErrorArray, TrackErrorArrayEntries);
INTROSPECTION_REGISTER(TrackErrorArray, "1.0", TrackErrorArray_introspection)

DECLARE_CLASS_MEMBER(ControlInArray, Pars, uint32, "[2]", "");
static const IntrospectionEntry* ControlInArrayEntries[] = { &ControlInArray_Pars_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(ControlInArray, ControlInArrayEntries);
INTROSPECTION_REGISTER(ControlInArray, "1.0", ControlInArray_introspection)

DECLARE_CLASS_MEMBER(TrackErrorMatrix, Pars, uint32, "[3][2]", "");
static const IntrospectionEntry* TrackErrorMatrixEntries[] = { &TrackErrorMatrix_Pars_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(TrackErrorMatrix, TrackErrorMatrixEntries);
INTROSPECTION_REGISTER(TrackErrorMatrix, "1.0", TrackErrorMatrix_introspection)

DECLARE_CLASS_MEMBER(ControlInMatrix, Pars, uint32, "[3][2]", "");
static const IntrospectionEntry* ControlInMatrixEntries[] = { &ControlInMatrix_Pars_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(ControlInMatrix, ControlInMatrixEntries);
INTROSPECTION_REGISTER(ControlInMatrix, "1.0", ControlInMatrix_introspection)

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

PIDGAM::PIDGAM() :
        BasicGAM() {

}

void PIDGAM::Execute(uint8 activeContextBuffer) {

    inputReader->Read(activeContextBuffer);

    TrackError *error = (TrackError*) inputReader->GetData(0);

    if (error != NULL) {
        printf("\nExecuting: error.par1= %d, error.par2= %d\n", error->Par1, error->Par2);

        ControlIn *control = (ControlIn*) outputWriter->GetData(0);
        ControlNoise *noise = (ControlNoise*) outputWriter->GetData(1);
        uint32 Kp = 10;

        control->Par1 = Kp * error->Par1;
        control->Par2 = Kp * error->Par2;

        if (noise->noiseValue > 0) {
            noise->noiseValue = -noise->noiseValue;
        }

    }

    outputWriter->Write(activeContextBuffer);
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

PIDGAMGroup::PIDGAMGroup() {
    context = 1u;

}

void PIDGAMGroup::SetUp() {
    context = 1u;
}

void PIDGAMGroup::PrepareNextState(const RealTimeStateInfo &status) {
    if (StringHelper::Compare(status.currentState, "state1") == 0) {
        if (StringHelper::Compare(status.nextState, "state2") == 0) {
            context++;
        }
    }
    if (StringHelper::Compare(status.currentState, "state2") == 0) {
        if (StringHelper::Compare(status.nextState, "state1") == 0) {
            context--;
        }
    }
}

uint32 PIDGAMGroup::GetContext() {
    return context;
}
CLASS_REGISTER(PIDGAMGroup, "1.0")

PlantGAM::PlantGAM() {

}

void PlantGAM::SetUp() {

}

void PlantGAM::Execute(uint8 activeContextBuffer) {

}

CLASS_REGISTER(PlantGAM, "1.0")

PIDGAM2::PIDGAM2() {

}

void PIDGAM2::Execute(uint8 activeContextBuffer) {

    inputReader->Read(activeContextBuffer);
    TrackErrorArray *error = (TrackErrorArray*) inputReader->GetData(0);

    printf("\nExecuting: error.par1= %d, error.par2= %d\n", error->Pars[0], error->Pars[1]);

    ControlInArray *control = (ControlInArray*) outputWriter->GetData(0);
    uint32 Kp = 10;

    control->Pars[0] = Kp * error->Pars[0];
    control->Pars[1] = Kp * error->Pars[1];

    outputWriter->Write(activeContextBuffer);
}

PIDGAM2::~PIDGAM2() {

}

void PIDGAM2::SetUp() {

}
CLASS_REGISTER(PIDGAM2, "1.0")

PIDGAM3::PIDGAM3() {

}

void PIDGAM3::Execute(uint8 activeContextBuffer) {

    inputReader->Read(activeContextBuffer);
    TrackErrorMatrix *error = (TrackErrorMatrix*) inputReader->GetData(0);

    ControlInMatrix*control = (ControlInMatrix*) outputWriter->GetData(0);
    uint32 Kp = 10;

    control->Pars[0][0] = Kp * error->Pars[0][0];
    control->Pars[0][1] = Kp * error->Pars[0][1];
    control->Pars[1][0] = Kp * error->Pars[1][0];
    control->Pars[1][1] = Kp * error->Pars[1][1];
    control->Pars[2][0] = Kp * error->Pars[2][0];
    control->Pars[2][1] = Kp * error->Pars[2][1];

    outputWriter->Write(activeContextBuffer);
}

PIDGAM3::~PIDGAM3() {

}

void PIDGAM3::SetUp() {

}

CLASS_REGISTER(PIDGAM3, "1.0")

SharedDataSource::SharedDataSource() {
    spinLockSem = new FastPollingEventSem;
    spinLockSem->Create();
    printf("\nCreated SharedDataSource\n");
}

SharedDataSource::~SharedDataSource() {
    delete spinLockSem;
}

void SharedDataSource::WriteEnd() {
    spinLockSem->FastPost();
}

CLASS_REGISTER(SharedDataSource, "1.0")

DummyGAM::DummyGAM() {

}

void DummyGAM::Execute(uint8 activeContextBuffer) {
    inputReader->Read(activeContextBuffer);
    uint32 *counterIn = (uint32*) inputReader->GetData(0);
    uint32 *counterOut = (uint32*) outputWriter->GetData(0);
    *counterOut = (*counterIn)+1;
    printf("\nExecute: Input=%d, Output=%d\n", *counterIn, *counterOut);

    outputWriter->Write(activeContextBuffer);
}


void DummyGAM::SetUp() {

}
CLASS_REGISTER(DummyGAM, "1.0")



DummyScheduler::DummyScheduler() {
    numberOfExecutions = 0;
}
void DummyScheduler::StartExecution(const uint32 activeBuffer) {
    numberOfExecutions++;
}
void DummyScheduler::StopExecution() {
    numberOfExecutions++;
}

CLASS_REGISTER(DummyScheduler, "1.0")


