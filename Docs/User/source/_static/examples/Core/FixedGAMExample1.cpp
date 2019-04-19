/**
 * @file FixedGAMExample1.cpp
 * @brief Source file for class FixedGAMExample1
 * @date 06/04/2018
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
 * the class FixedGAMExample1 (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "FixedGAMExample1.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {
FixedGAMExample1::FixedGAMExample1() {
    gain = 0u;
    inputSignal = NULL_PTR(MARTe::uint32 *);
    outputSignal = NULL_PTR(MARTe::uint32 *);
}

FixedGAMExample1::~FixedGAMExample1() {

}

bool FixedGAMExample1::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Could not Initialise the GAM");
    }
    if (ok) {
        ok = data.Read("Gain", gain);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The parameter Gain shall be set");
        }
    }
    if (ok) {
        REPORT_ERROR(ErrorManagement::Information, "Parameter Gain set to %d", gain);
    }
    return ok;
}

bool FixedGAMExample1::Setup() {
    using namespace MARTe;
    uint32 numberOfInputSignals = GetNumberOfInputSignals();
    uint32 numberOfOutputSignals = GetNumberOfOutputSignals();
    bool ok = (numberOfInputSignals == numberOfOutputSignals);
    if (ok) {
        ok = (numberOfOutputSignals == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "The number of input and output signals shall be equal to 1. numberOfInputSignals = %d numberOfOutputSignals = %d",
                         numberOfInputSignals, numberOfOutputSignals);
        }
    }
    if (ok) {
        TypeDescriptor inputSignalType = GetSignalType(InputSignals, 0u);
        TypeDescriptor outputSignalType = GetSignalType(OutputSignals, 0u);
        ok = (inputSignalType == outputSignalType);
        if (ok) {
            ok = (inputSignalType == UnsignedInteger32Bit);
        }
        if (!ok) {
            const char8 * const inputSignalTypeStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(inputSignalType);
            const char8 * const outputSignalTypeStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(outputSignalType);
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "The type of the input and output signal shall be uint32. inputSignalType = %s outputSignalType = %s", inputSignalTypeStr,
                         outputSignalTypeStr);
        }
    }
    if (ok) {
        uint32 numberOfInputSamples = 0u;
        uint32 numberOfOutputSamples = 0u;
        ok = GetSignalNumberOfSamples(InputSignals, 0u, numberOfInputSamples);
        if (ok) {
            ok = GetSignalNumberOfSamples(OutputSignals, 0u, numberOfOutputSamples);
        }
        if (ok) {
            ok = (numberOfInputSamples == numberOfOutputSamples);
        }
        if (ok) {
            ok = (numberOfInputSamples == 1u);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "The number of input and output signals samples shall be equal to 1. numberOfInputSamples = %d numberOfOutputSamples = %d",
                         numberOfInputSamples, numberOfOutputSamples);
        }
    }
    if (ok) {
        uint32 numberOfInputDimensions = 0u;
        uint32 numberOfOutputDimensions = 0u;
        ok = GetSignalNumberOfDimensions(InputSignals, 0u, numberOfInputDimensions);
        if (ok) {
            ok = GetSignalNumberOfDimensions(OutputSignals, 0u, numberOfOutputDimensions);
        }
        if (ok) {
            ok = (numberOfInputDimensions == numberOfOutputDimensions);
        }
        if (ok) {
            ok = (numberOfInputDimensions == 0u);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "The number of input and output signals dimensions shall be equal to 0. numberOfInputDimensions = %d numberOfOutputDimensions = %d",
                         numberOfInputDimensions, numberOfOutputDimensions);
        }
    }
    if (ok) {
        uint32 numberOfInputElements = 0u;
        uint32 numberOfOutputElements = 0u;
        ok = GetSignalNumberOfElements(InputSignals, 0u, numberOfInputElements);
        if (ok) {
            ok = GetSignalNumberOfElements(OutputSignals, 0u, numberOfOutputElements);
        }
        if (ok) {
            ok = (numberOfInputElements == numberOfOutputElements);
        }
        if (ok) {
            ok = (numberOfInputElements == 1u);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "The number of input and output signals elements shall be equal to 1. numberOfInputElements = %d numberOfOutputElements = %d",
                         numberOfInputElements, numberOfOutputElements);
        }
    }
    if (ok) {
        inputSignal = reinterpret_cast<uint32 *>(GetInputSignalMemory(0u));
        outputSignal = reinterpret_cast<uint32 *>(GetOutputSignalMemory(0u));
    }
    return ok;

}

bool FixedGAMExample1::Execute() {
    *outputSignal = gain * *inputSignal;
    return true;
}

CLASS_REGISTER(FixedGAMExample1, "")
}
