/**
 * @file VariableGAMExample1.cpp
 * @brief Source file for class VariableGAMExample1
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
 * the class VariableGAMExample1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "VariableGAMExample1.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {
VariableGAMExample1::VariableGAMExample1() {
    numberOfOutputSignals = 0u;
    gains = NULL_PTR(MARTe::uint32 *);
    inputSignal = NULL_PTR(MARTe::uint32 *);
    outputSignals = NULL_PTR(MARTe::uint32 **);
}

VariableGAMExample1::~VariableGAMExample1() {
    if (gains != NULL_PTR(MARTe::uint32 *)) {
        delete[] gains;
    }
    if (outputSignals != NULL_PTR(MARTe::uint32 **)) {
        delete[] outputSignals;
    }
}

bool VariableGAMExample1::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Could not Initialise the GAM");
    }
    if (ok) {
        AnyType arrayDescription = data.GetType("Gains");
        ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
        uint32 numberOfElements = 0u;
        if (ok) {
            numberOfElements = arrayDescription.GetNumberOfElements(0u);
            ok = (numberOfElements > 0u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "No elements defined in the array");
            }
        }
        if (ok) {
            //Reconfiguration...
            if (gains != NULL) {
                delete [] gains;
            }
            gains = new uint32[numberOfElements];
            Vector<uint32> readVector(gains, numberOfElements);
            ok = data.Read("Gains", readVector);
            if (ok) {
                REPORT_ERROR(ErrorManagement::Information, "Gains set to %d", readVector);
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not read the Gains");
            }
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The parameter Gains shall be set as an array");
        }
    }
    return ok;
}

bool VariableGAMExample1::Setup() {
    using namespace MARTe;
    uint32 numberOfInputSignals = GetNumberOfInputSignals();
    numberOfOutputSignals = GetNumberOfOutputSignals();
    bool ok = (numberOfInputSignals == 1u);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "The number of input signals shall be equal to 1. numberOfInputSignals = %d ", numberOfInputSignals);
    }
    if (ok) {
        ok = (numberOfOutputSignals > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The number of output signals shall be greater than 1. numberOfOutputSignals = %d ",
                         numberOfOutputSignals);
        }
    }
    if (ok) {
        outputSignals = new uint32*[numberOfOutputSignals];
    }
    if (ok) {
        TypeDescriptor inputSignalType = GetSignalType(InputSignals, 0u);
        uint32 n;
        for (n = 0u; (n < numberOfOutputSignals) && (ok); n++) {
            StreamString outputSignalName;
            ok = GetSignalName(OutputSignals, n, outputSignalName);
            TypeDescriptor outputSignalType = GetSignalType(OutputSignals, n);
            ok = (inputSignalType == outputSignalType);
            if (ok) {
                ok = (inputSignalType == UnsignedInteger32Bit);
            }
            if (!ok) {
                const char8 * const inputSignalTypeStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(inputSignalType);
                const char8 * const outputSignalTypeStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(outputSignalType);
                REPORT_ERROR(ErrorManagement::ParametersError,
                             "The type of the input and output signals shall be uint32. inputSignalType = %s outputSignalType (%s) = %s", inputSignalTypeStr,
                             outputSignalName.Buffer(), outputSignalTypeStr);
            }

            uint32 numberOfInputSamples = 0u;
            uint32 numberOfOutputSamples = 0u;
            if (ok) {
                ok = GetSignalNumberOfSamples(InputSignals, 0u, numberOfInputSamples);
            }
            if (ok) {
                ok = GetSignalNumberOfSamples(OutputSignals, n, numberOfOutputSamples);
            }
            if (ok) {
                ok = (numberOfInputSamples == numberOfOutputSamples);
            }
            if (ok) {
                ok = (numberOfInputSamples == 1u);
                if (!ok) {
                    REPORT_ERROR(
                            ErrorManagement::ParametersError,
                            "The number of input and output signals samples shall be equal to 1. numberOfInputSamples = %d numberOfOutputSamples (%s) = %d",
                            numberOfInputSamples, outputSignalName.Buffer(), numberOfOutputSamples);
                }
            }
            uint32 numberOfInputDimensions = 0u;
            uint32 numberOfOutputDimensions = 0u;
            if (ok) {
                ok = GetSignalNumberOfDimensions(InputSignals, 0u, numberOfInputDimensions);
            }
            if (ok) {
                ok = GetSignalNumberOfDimensions(OutputSignals, n, numberOfOutputDimensions);
            }
            if (ok) {
                ok = (numberOfInputDimensions == numberOfOutputDimensions);
            }
            if (ok) {
                ok = (numberOfInputDimensions == 0u);
                if (!ok) {
                    REPORT_ERROR(
                            ErrorManagement::ParametersError,
                            "The number of input and output signals dimensions shall be equal to 0. numberOfInputDimensions = %d numberOfOutputDimensions (%s) = %d",
                            numberOfInputDimensions, outputSignalName.Buffer(), numberOfOutputDimensions);
                }
            }
            uint32 numberOfInputElements = 0u;
            uint32 numberOfOutputElements = 0u;
            if (ok) {
                ok = GetSignalNumberOfElements(InputSignals, 0u, numberOfInputElements);
            }
            if (ok) {
                ok = GetSignalNumberOfElements(OutputSignals, n, numberOfOutputElements);
            }
            if (ok) {
                ok = (numberOfInputElements == numberOfOutputElements);
            }
            if (ok) {
                ok = (numberOfInputElements == 1u);
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError,
                             "The number of input and output signals elements shall be equal to 1. numberOfInputElements = %d numberOfOutputElements (%s) = %d",
                             numberOfInputElements, outputSignalName.Buffer(), numberOfOutputElements);
            }
            if (ok) {
                outputSignals[n] = reinterpret_cast<uint32 *>(GetOutputSignalMemory(n));
            }
        }
        if (ok) {
            inputSignal = reinterpret_cast<uint32 *>(GetInputSignalMemory(0u));
        }
    }
    return ok;
}

bool VariableGAMExample1::Execute() {
    MARTe::uint32 n;
    for (n = 0u; (n < numberOfOutputSignals); n++) {
        *(outputSignals[n]) = gains[n] * *inputSignal;
    }
    return true;
}

CLASS_REGISTER(VariableGAMExample1, "")
}
