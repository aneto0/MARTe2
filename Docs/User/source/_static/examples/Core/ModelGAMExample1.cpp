/**
 * @file ModelGAMExample1.cpp
 * @brief Source file for class ModelGAMExample1
 * @date 09/04/2018
 * @author Andre Neto
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
 * the class ModelGAMExample1 (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "IntrospectionT.h"
#include "ModelGAMExample1.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
//Register the structures
DECLARE_CLASS_MEMBER(ModelGAMExampleStructInner1, f1, float32, "", "");
DECLARE_CLASS_MEMBER(ModelGAMExampleStructInner1, f2, float32, "", "");
DECLARE_CLASS_MEMBER(ModelGAMExampleStructInner1, f3, float32, "[6]", "");
//The array members must follow the naming convention CLASSNAME_MEMBERNAME_introspectionEntry
static const MARTe::IntrospectionEntry* ModelGAMExampleStructInner1Entries[] = { &ModelGAMExampleStructInner1_f1_introspectionEntry,
        &ModelGAMExampleStructInner1_f2_introspectionEntry, &ModelGAMExampleStructInner1_f3_introspectionEntry, 0 };
//Finally declare the class as introspectable
DECLARE_STRUCT_INTROSPECTION(ModelGAMExampleStructInner1, ModelGAMExampleStructInner1Entries)

DECLARE_CLASS_MEMBER(ModelGAMExampleStructSignal, u1, uint32, "", "");
DECLARE_CLASS_MEMBER(ModelGAMExampleStructSignal, s1, ModelGAMExampleStructInner1, "", "");
DECLARE_CLASS_MEMBER(ModelGAMExampleStructSignal, s2, ModelGAMExampleStructInner1, "", "");
//The array members must follow the naming convention CLASSNAME_MEMBERNAME_introspectionEntry
static const MARTe::IntrospectionEntry* ModelGAMExampleStructSignalEntries[] = { &ModelGAMExampleStructSignal_u1_introspectionEntry,
        &ModelGAMExampleStructSignal_s1_introspectionEntry, &ModelGAMExampleStructSignal_s2_introspectionEntry, 0 };
//Finally declare the class as introspectable
DECLARE_STRUCT_INTROSPECTION(ModelGAMExampleStructSignal, ModelGAMExampleStructSignalEntries)

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ModelGAMExample1::ModelGAMExample1() {
    inputSignal = NULL_PTR(ModelGAMExampleStructSignal *);
    outputSignal = NULL_PTR(ModelGAMExampleStructSignal *);
}

ModelGAMExample1::~ModelGAMExample1() {
}

bool ModelGAMExample1::CheckSignal(MARTe::SignalDirection signalDirection, MARTe::IntrospectionEntry introEntry, MARTe::uint32 signalIdx) {
    using namespace MARTe;
    TypeDescriptor signalMemberType = GetSignalType(signalDirection, signalIdx);
    TypeDescriptor introMemberType = introEntry.GetMemberTypeDescriptor();
    StreamString signalName;
    bool ok = GetSignalName(signalDirection, signalIdx, signalName);
    if (ok) {
        ok = (signalMemberType == introMemberType);
        if (!ok) {
            const char8 * const memberTypeTypeStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(signalMemberType);
            const char8 * const introMemberTypeStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(introMemberType);
            REPORT_ERROR(ErrorManagement::ParametersError, "Output type signal mismatch = %s != %s for signal %s", memberTypeTypeStr, introMemberTypeStr,
                         signalName.Buffer());
        }
    }
    if (ok) {
        uint32 numberOfOutputSamples = 0u;
        ok = GetSignalNumberOfSamples(signalDirection, signalIdx, numberOfOutputSamples);
        if (ok) {
            ok = (numberOfOutputSamples == 1u);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The number of output signals samples shall be equal to 1. numberOfOutputSamples = %d for signal %s",
                         signalIdx, numberOfOutputSamples, signalName.Buffer());
        }
    }
    uint32 introNumberOfElements = 1u;
    if (ok) {
        uint32 signalNumberOfDimensions = 0u;
        uint8 introMemberNumberOfDimensions = introEntry.GetNumberOfDimensions();
        ok = GetSignalNumberOfDimensions(signalDirection, signalIdx, signalNumberOfDimensions);
        if (ok) {
            ok = (signalNumberOfDimensions == introMemberNumberOfDimensions);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Number of dimensions mismatch = %d != %d for signal %s", signalNumberOfDimensions,
                         introMemberNumberOfDimensions, signalName.Buffer());
            ok = true;
        }
        if (ok) {
            uint32 k;
            for (k = 0; k < introMemberNumberOfDimensions; k++) {
                introNumberOfElements *= introEntry.GetNumberOfElements(k);
            }
        }
    }
    if (ok) {
        uint32 signalNumberOfElements = 0u;
        ok = GetSignalNumberOfElements(signalDirection, signalIdx, signalNumberOfElements);
        if (ok) {
            ok = (signalNumberOfElements == introNumberOfElements);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Number of elements mismatch = %d != %d for signal %s", signalNumberOfElements,
                         introNumberOfElements, signalName.Buffer());
        }
    }
    return ok;
}

bool ModelGAMExample1::Setup() {
    using namespace MARTe;
    uint32 numberOfInputSignals = GetNumberOfInputSignals();
    uint32 numberOfOutputSignals = GetNumberOfOutputSignals();
    bool ok = (numberOfInputSignals == numberOfOutputSignals);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "The number of input signals shall be equal to the number of output signals. numberOfInputSignals = %d != numberOfOutputSignals ", numberOfInputSignals, numberOfOutputSignals);
    }

    ClassRegistryItem *cri = NULL_PTR(ClassRegistryItem *);
    if (ok) {
        cri = ClassRegistryDatabase::Instance()->Find("ModelGAMExampleStructSignal");
        ok = (cri != NULL_PTR(ClassRegistryItem *));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "ModelGAMExampleStructSignal is not registered!");
        }
    }
    const Introspection *introspectionMainStruct = NULL_PTR(Introspection *);
    if (ok) {
        introspectionMainStruct = cri->GetIntrospection();
    }
    if (ok) {
        cri = ClassRegistryDatabase::Instance()->Find("ModelGAMExampleStructInner1");
    }
    const Introspection *introspectionInnerStruct = NULL_PTR(Introspection *);
    if (ok) {
        introspectionInnerStruct = cri->GetIntrospection();
    }

    uint32 introspectionMainStructNumberOfMembers = 0u;
    if (ok) {
        //where +1 is the //u1
        introspectionMainStructNumberOfMembers = introspectionInnerStruct->GetNumberOfMembers() * 2 + 1;
    }
    if (ok) {
        //The structure has 7 signals
        ok = (numberOfOutputSignals == introspectionMainStructNumberOfMembers);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The number of output signals shall be equal to %d. numberOfOutputSignals = %d",
                         introspectionMainStructNumberOfMembers, numberOfOutputSignals);
        }
    }
    //Check that all the properties match
    if (ok) {
        ok = CheckSignal(InputSignals, introspectionMainStruct->operator [](0), 0);
    }
    if (ok) {
        ok = CheckSignal(InputSignals, introspectionInnerStruct->operator [](0), 1);
    }
    if (ok) {
        ok = CheckSignal(InputSignals, introspectionInnerStruct->operator [](1), 2);
    }
    if (ok) {
        ok = CheckSignal(InputSignals, introspectionInnerStruct->operator [](2), 3);
    }
    if (ok) {
        ok = CheckSignal(InputSignals, introspectionInnerStruct->operator [](0), 4);
    }
    if (ok) {
        ok = CheckSignal(InputSignals, introspectionInnerStruct->operator [](1), 5);
    }
    if (ok) {
        ok = CheckSignal(InputSignals, introspectionInnerStruct->operator [](2), 6);
    }
    if (ok) {
        ok = CheckSignal(OutputSignals, introspectionMainStruct->operator [](0), 0);
    }
    if (ok) {
        ok = CheckSignal(OutputSignals, introspectionInnerStruct->operator [](0), 1);
    }
    if (ok) {
        ok = CheckSignal(OutputSignals, introspectionInnerStruct->operator [](1), 2);
    }
    if (ok) {
        ok = CheckSignal(OutputSignals, introspectionInnerStruct->operator [](2), 3);
    }
    if (ok) {
        ok = CheckSignal(OutputSignals, introspectionInnerStruct->operator [](0), 4);
    }
    if (ok) {
        ok = CheckSignal(OutputSignals, introspectionInnerStruct->operator [](1), 5);
    }
    if (ok) {
        ok = CheckSignal(OutputSignals, introspectionInnerStruct->operator [](2), 6);
    }
    if (ok) {
        inputSignal = reinterpret_cast<ModelGAMExampleStructSignal *>(GetInputSignalMemory(0u));
    }
    if (ok) {
        outputSignal = reinterpret_cast<ModelGAMExampleStructSignal *>(GetOutputSignalMemory(0u));
    }
    return ok;
}

bool ModelGAMExample1::Execute() {
    MARTe::float32 gain1 = 1.02;
    MARTe::float32 gain2 = 1.03;
    (outputSignal->u1)++;
    outputSignal->s1.f1 = gain1 * inputSignal->s1.f1;
    outputSignal->s1.f2 = gain2 * inputSignal->s1.f2;
    outputSignal->s1.f3[0] = inputSignal->s1.f1;
    outputSignal->s1.f3[1] = -inputSignal->s1.f1;
    outputSignal->s1.f3[2] = inputSignal->s1.f2;
    outputSignal->s1.f3[3] = -inputSignal->s1.f2;
    outputSignal->s1.f3[4] = inputSignal->s1.f1 + inputSignal->s1.f2;
    outputSignal->s1.f3[5] = inputSignal->s1.f1 - inputSignal->s1.f2;
    outputSignal->s2.f1 = gain1 * inputSignal->s2.f1;
    outputSignal->s2.f2 = gain2 * inputSignal->s2.f2;
    outputSignal->s2.f3[0] = inputSignal->s2.f1;
    outputSignal->s2.f3[1] = -inputSignal->s2.f1;
    outputSignal->s2.f3[2] = inputSignal->s2.f2;
    outputSignal->s2.f3[3] = -inputSignal->s2.f2;
    outputSignal->s2.f3[4] = inputSignal->s2.f1 - inputSignal->s2.f2;
    outputSignal->s2.f3[5] = inputSignal->s2.f1 + inputSignal->s2.f2;
    return true;
}

CLASS_REGISTER(ModelGAMExample1, "")
