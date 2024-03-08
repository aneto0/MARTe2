/**
 * @file RealTimeState.h
 * @brief Header file for class RealTimeState
 * @date 19/02/2016
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

 * @details This header file contains the declaration of the class RealTimeState
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMESTATE_H_
#define REALTIMESTATE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ObjectRegistryDatabase.h"
#include "GAMGroup.h"
#include "RealTimeApplication.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A container of one (and only one) ReferenceContainer of RealTimeThread elements.
 * @details The RealTimeState lists the threads which are
 * allowed in any given state of the application. Switching state will enable
 * different threads to be executed.
 *
 * The syntax in the configuration stream shall be:
 * <pre>
 * State_name = {
 *     Class = RealTimeState
 *     +Threads = {
 *         Class = ReferenceContainer
 *         RealTimeThread_name = {
 *             Class = RealTimeThread
 *             ...
 *         }
 *         ...
 *      }
 * }
 * </pre>
 */
class RealTimeState: public ReferenceContainer, public StatefulI {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     * @post
     *   GetNumberOfStatefuls() == 0u
     */
RealTimeState    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~RealTimeState();

    /**
     * @brief Registers all the StatefulI Functions (GAM and GAMGroup components that implement StatefulI)  that are executed by all the RealTimeThreads that are owned by this RealTimeState.
     * @param[in] statefulsIn StatefulI Functions that are executed by all the RealTimeThreads that are owned by this RealTimeState.
     * @return true if all the elements in \a statefulsIn implement the StatefulI interface.
     */
    bool AddStatefuls(ReferenceContainer &statefulsIn);

    /**
     * @brief Calls PrepareNextState on all the StatefulI Functions (GAM and GAMGroup components that implement StatefulI) that are executed by all the RealTimeThreads that are owned by this RealTimeState.
     * @param[in] currentStateName the name of the current state being executed.
     * @param[in] nextStateName the name of the next state to be executed.
     * @return true if the state change is accepted by all the StatefulI Functions.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Gets the number of StatefulI Functions (GAM and GAMGroup components that implement StatefulI) that are that are executed by all the RealTimeThreads that are owned by this RealTimeState.
     * @return  the number of StatefulI Functions that are that are executed by all the RealTimeThreads that are owned by this RealTimeState.
     */
    uint32 GetNumberOfStatefuls();

    /**
     * @see ReferenceContainer::Purge()
     */
    virtual void Purge(ReferenceContainer &purgeList);

private:

    /**
     * Container of StatefulI Functions that are that are executed by all the RealTimeThreads that are owned by this RealTimeState.
     */
    ReferenceContainer statefuls;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMESTATE_H_ */

