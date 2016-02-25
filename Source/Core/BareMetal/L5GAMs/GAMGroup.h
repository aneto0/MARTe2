/**
 * @file GAMGroup.h
 * @brief Header file for class GAMGroup
 * @date 24/02/2016
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class GAMGroup
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMGROUP_H_
#define GAMGROUP_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "RealTimeStateInfo.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief A group of GAMs sharing the same context.
 * @details In order to allow custom implementations of the shared context, some of the methods
 * of this class are pure virtual. The derived class can generally
 * add a context (see ContextT) and implements the specific procedures in order
 * to prepare the context for the next state (PrepareNextState(*) function) and
 * to switch to the new context (ChangeState(*) function).
 *
 * @details The definition of the GAMGroup in the configuration stream is inside the
 * block RealTimeApplicatio.Functions
 * GAMGroup_name = {\n
 *    Class = GAMGroup\n
 *    States = {State1_name, State2_name, ... }\n
 *    ... // context definition, ecc\n
 *    GAM1_name = {\n
 *        Class = GAM\n
 *        ...\n
 *    }\n
 *    ...\n
 * }
 */
class GAMGroup: public ReferenceContainer {
public:

    /**
     * @brief Constructor
     * @post
     *   GetSupportedStates() == NULL &&
     *   GetNumberOfSupportedStates() == 0;
     */
    GAMGroup();

    /**
     * @brief Destructor. Frees the array of the supported state names.
     */
    virtual ~GAMGroup();

    /**
     * @brief Setup all the GAMs in the container. This function can be customly
     * implemented to initialise the context, make accelerators, ecc.
     */
    virtual void SetUp();

    /**
     * @brief Does all the necessary operations to prepare the context used for the
     * next state.
     * @param[in] status specifies the current and the next state.
     */
    virtual void PrepareNextState(const RealTimeStateInfo &status)=0;

    /**
     * @brief Initialises the object from cdb.
     * @param[in] data is the StructuredData containing the initialisation data.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Returns the array with the names of the states in which this GAMGroup is involved.
     * @return the names of the supported states.
     */
    StreamString *GetSupportedStates() const;

    /**
     * @brief Returns the number of the supported states.
     * @return the number of the supported states.
     */
    uint32 GetNumberOfSupportedStates() const;

private:

    /**
     * The names of the supported states
     */
    StreamString *supportedStates;

    /**
     * How many supported states
     */
    uint32 numberOfSupportedStates;

    //? Possible specific GAMContexts
    //? Possible uint8 activeContextBuffer

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMGROUP_H_ */

