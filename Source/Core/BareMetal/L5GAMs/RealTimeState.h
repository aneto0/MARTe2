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
#include "RealTimeStateInfo.h"
#include "GAMGroup.h"
#include "RealTimeApplication.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A container of one (and only one) ReferenceContainer of RealTimeThread elements.
 * @details The RealTimeState contains one RealTimeThread container which
 * shall be named Threads. The RealTimeState lists the threads which are
 * allowed in any given state of the application. Switching state will enable
 * different threads to be executed.
 *
 * The syntax in the configuration stream shall be:
 * State_name = {\n
 *     Class = RealTimeState\n
 *     +Threads = {
 *         Class = ReferenceContainer
 *         RealTimeThread_name = {\n
 *             Class = RealTimeThread\n
 *             ...\n
 *         }\n
 *         ...\n
 *      }\n
 * }\n
 */
class DLL_API RealTimeState: public ReferenceContainer, public StatefulI {

public:
    CLASS_REGISTER_DECLARATION()
    ;

    /**
     * @brief Constructor
     * @post
     *   GetStatefulGAMGroups() == NULL &&
     *   GetNumberOfElements() == 0 &&
     *   GetContextActiveBuffer() == 0;
     */
    RealTimeState();

    /**
     * @brief Destructor.
     * @post
     *   free(GetStatefulGAMGroups()) &&
     *   GetStatefulGAMGroups() = NULL
     */
    virtual ~RealTimeState();


    bool AddStatefuls(ReferenceContainer &statefulsIn);


    virtual bool PrepareNextState(const char8 * currentStateName,
                                  const char8 * nextStateName);

private:


    ReferenceContainer statefuls;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMESTATE_H_ */

