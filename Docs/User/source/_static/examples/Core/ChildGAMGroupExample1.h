/**
 * @file ChildGAMGroupExample1.h
 * @brief Header file for class ChildGAMGroupExample1
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

 * @details This header file contains the declaration of the class ChildGAMGroupExample1
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EXAMPLES_CORE_CHILDGAMGROUPEXAMPLE1_H_
#define EXAMPLES_CORE_CHILDGAMGROUPEXAMPLE1_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GAM.h"
#include "GAMGroupSharedInfoExample1.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {
/**
 * @brief An example of a GAM which requires information that is made available by a parent GAMGroup.
 *
 * @details This GAM multiplies the input signal the matrix shared by the GAMGroup (see SetContext).
 *
 * +GAMExample1 = {
 *     Class = ChildGAMGroupExample1
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
 *     }
 * }
 */
class ChildGAMGroupExample1 : public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
    ChildGAMGroupExample1();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~ChildGAMGroupExample1();

    /**
     * @brief See GAM::SetContext
     * @return true if the parent MARTe::ReferenceT<const GAMGroupSharedInfoExample1> is Valid and if the matrix dimensions are compatible with the input/output signal properties.
     */
    virtual bool SetContext(ConstReference context);

    /**
     * @brief Verifies correctness of the GAM configuration.
     * @details Checks that the number of input signals is equal to the number of output signals is equal to one and that the same type is used.
     * @return true if the pre-conditions are met.
     * @pre
     *   SetConfiguredDatabase() &&
     *   GetNumberOfInputSignals() == GetNumberOfOutputSignals() == 1 &&
     *   GetSignalType(InputSignals, 0) == GetSignalType(OutputSignals, 0) == uint32 &&
     *   GetSignalNumberOfDimensions(InputSignals, 0) == GetSignalNumberOfDimensions(OutputSignals, 0) == 0 &&
     *   GetSignalNumberOfSamples(InputSignals, 0) == GetSignalNumberOfSamples(OutputSignals, 0) == 1 &&
     *   GetSignalNumberOfElements(InputSignals, 0) == GetSignalNumberOfElements(OutputSignals, 0) == 1
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
    MARTe::Matrix<MARTe::uint32> *inputSignal;

    /**
     * The output signal
     */
    MARTe::Matrix<MARTe::uint32> *outputSignal;

    /**
     * True if the context has been set
     */
    bool contextSet;

    /**
     * The context containing the Matrix to be shared.
     */
    MARTe::ReferenceT<const GAMGroupSharedInfoExample1> matrixModelContext;
};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EXAMPLES_CORE_CHILDGAMGROUPEXAMPLE1_H_ */

