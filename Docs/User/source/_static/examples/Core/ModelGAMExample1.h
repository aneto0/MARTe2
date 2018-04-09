/**
 * @file ModelGAMExample1.h
 * @brief Header file for class ModelGAMExample1
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

 * @details This header file contains the declaration of the class ModelGAMExample1
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CORE_MODELGAMEXAMPLE1_H_
#define CORE_MODELGAMEXAMPLE1_H_

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
 * Define the structured signal produced by this GAM
 */
struct ModelGAMExampleStruct1 {
    MARTe::float32 f1;
    MARTe::float32 f2;
    MARTe::float32 f3[6];
};
struct ModelGAMExampleStructSignal {
    MARTe::uint32 u1;
    ModelGAMExampleStruct1 s1;
    ModelGAMExampleStruct1 s2;
};

/**
 * @brief An example of a GAM which has produces a structured output signal.
 *
 * +GAMExample1 = {
 *     Class = ModelGAMExample1
 *     OutputSignals = {
 *         Signal1 = {
 *             DataSource = "DDB1"
 *             Type = ModelGAMExampleStructSignal
 *         }
 *     }
 * }
 */
class ModelGAMExample1 : public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
    ModelGAMExample1();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~ModelGAMExample1();

    /**
     * @brief Verifies correctness of the GAM configuration.
     * @details Checks the number of output signals is equal to one and that the type of the signal is ModelGAMExample1.
     * @return true if the pre-conditions are met.
     * @pre
     *   SetConfiguredDatabase() &&
     *   GetNumberOfInputSignals() == 0 && GetNumberOfOutputSignals() == 1 &&
     *   GetSignalType(OutputSignals, 0) == ModelGAMExampleStructSignal &&
     *   GetSignalNumberOfDimensions(OutputSignals, 0) == 0 &&
     *   GetSignalNumberOfSamples(OutputSignals, 0) == 1 &&
     *   GetSignalNumberOfElements(OutputSignals, 0) == 1
     */
    virtual bool Setup();

    /**
     * @brief Multiplies the input signal by the Gain.
     * @return true.
     */
    virtual bool Execute();

private:

    /**
     * The output signal
     */
    MARTe::uint32 *outputSignal;

    /**
     * The configured gain.
     */
    MARTe::uint32 gain;
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CORE_MODELGAMEXAMPLE1_H_ */
