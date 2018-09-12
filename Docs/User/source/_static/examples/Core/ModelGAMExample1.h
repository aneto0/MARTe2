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
#include "Introspection.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Define the structured signal produced by this GAM
 */
struct ModelGAMExampleStructInner1 {
    MARTe::float32 f1;
    MARTe::float32 f2;
    MARTe::float32 f3[6];
};
struct ModelGAMExampleStructSignal {
    MARTe::uint32 u1;
    ModelGAMExampleStructInner1 s1;
    ModelGAMExampleStructInner1 s2;
};

/**
 * @brief An example of a GAM which has a structured output signal (the previous state is read as an input signal).
 *
 * +GAMExample1 = {
 *     Class = ModelGAMExample1
 *     InputSignals = {
 *         Signal1 = {
 *             DataSource = "DDB1"
 *             Type = ModelGAMExampleStructSignal
 *         }
 *     }
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
     * @return true if the Input and Output signal characteristics match with the ModelGAMExampleStructSignal.
     */
    virtual bool Setup();

    /**
     * @brief Generates some random number based on the previous state values.
     * @return true.
     */
    virtual bool Execute();

private:

    /**
     * @brief Checks if the signal is configured as expected.
     * @param[in] signalDirection the signal direction to check.
     * @param[in] introEntry the introspectable member to be queried.
     * @param[in] signalIdx the index of the signal mapping to the member.
     * @return If the member and the signal properties match.
     */
    bool CheckSignal(MARTe::SignalDirection signalDirection, MARTe::IntrospectionEntry introEntry, MARTe::uint32 signalIdx);

    /**
     * The input signal
     */
    ModelGAMExampleStructSignal *inputSignal;

    /**
     * The output signal
     */
    ModelGAMExampleStructSignal *outputSignal;
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CORE_MODELGAMEXAMPLE1_H_ */
