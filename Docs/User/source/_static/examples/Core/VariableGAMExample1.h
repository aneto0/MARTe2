/**
 * @file VariableGAMExample1.h
 * @brief Header file for class VariableGAMExample1
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

 * @details This header file contains the declaration of the class VariableGAMExample1
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EXAMPLES_CORE_VARIABLEGAMEXAMPLE1_H_
#define EXAMPLES_CORE_VARIABLEGAMEXAMPLE1_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GAM.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief An example of a GAM which has fixed inputs and outputs.
 *
 * @details This GAM multiplies the input signal by a Gain.
 * The configuration syntax is (names and types are only given as an example):
 *
 * +GAMExample1 = {
 *     Class = VariableGAMExample1
 *     Gains = {5, 6, 7} //Compulsory
 *     InputSignals = {
 *         Signal1 = {
 *             DataSource = "DDB1"
 *             Type = uint32
 *         }
 *     }
 *     OutputSignals = {
 *         Signal1 = {
 *             DataSource = "DDB1"
 *             Type = uint32
 *         }
 *         Signal2 = {
 *             DataSource = "DDB1"
 *             Type = uint32
 *         }
 *         Signal3 = {
 *             DataSource = "DDB1"
 *             Type = uint32
 *         }
 *     }
 * }
 */
class VariableGAMExample1 : public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
    VariableGAMExample1();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~VariableGAMExample1();

    /**
     * @brief Reads the Gain from the configuration file.
     * @param[in] data see GAM::Initialise. The parameter Gains shall exist and will be read as an uint32 array (one value for each output signal).
     * @return true if the parameter Gain can be read.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

    /**
     * @brief Verifies correctness of the GAM configuration.
     * @details Checks that the number of input signals is equal to the number of output signals is greater or equal to one and that the same type is used.
     * @return true if the pre-conditions are met.
     * @pre
     *   SetConfiguredDatabase() &&
     *   GetNumberOfInputSignals() == 1 && GetNumberOfOutputSignals() >= 1 &&
     *   GetSignalType(InputSignals, 0) == GetSignalType(OutputSignals, *) == uint32 &&
     *   GetSignalNumberOfDimensions(InputSignals, 0) == GetSignalNumberOfDimensions(OutputSignals, *) == 0 &&
     *   GetSignalNumberOfSamples(InputSignals, 0) == GetSignalNumberOfSamples(OutputSignals, *) == 1 &&
     *   GetSignalNumberOfElements(InputSignals, 0) == GetSignalNumberOfElements(OutputSignals, *) == 1
     */
    virtual bool Setup();

    /**
     * @brief Multiplies the input signal by the Gain.
     * @return true.
     */
    virtual bool Execute();

private:

    /**
     * The input signal
     */
    MARTe::uint32 *inputSignal;

    /**
     * The output signals
     */
    MARTe::uint32 **outputSignals;

    /**
     * The number of output signals
     */
    MARTe::uint32 numberOfOutputSignals;

    /**
     * The configured gain.
     */
    MARTe::uint32 *gains;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EXAMPLES_CORE_VARIABLEGAMEXAMPLE1_H_ */

