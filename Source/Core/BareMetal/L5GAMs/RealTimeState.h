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
class DLL_API RealTimeState: public ReferenceContainer {

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

    /**
     * @brief Propagates the configuration setup request to the declared RealTimeThreads.
     * @details Calls RealTimeThread.ConfigureArchitecture(rtApp) in each of the declared RealTimeThread elements.
     * @param[in] rtApp is the RealTimeApplication where this state is declared into.
     * @return true if one and only one child element named "Threads" exits AND is of type ReferenceContainer AND
     * if RealTimeThread.ConfigureArchitecture is true for all the "Threads" elements.
     */
    bool ConfigureArchitecture(RealTimeApplication & rtApp);

    /**
     * @see RealTimeApplication::ValidateDataSourceLinks()
     */
    bool ValidateDataSourceLinks();

    /**
     * @brief Inserts a function.
     * @details If the container called "+Functions" it is not present, it will be created and \a functionReference
     * will be inserted into.
     * @param[in] functionReference is the reference to be inserted.
     */
    bool InsertFunction(Reference functionReference);

    /**
     * @brief Stores a stateful GAMGroup into the internal array.
     * @param[in] element is the new GAMGroup to be added.
     * @return true if the memory allocation succeeds, false otherwise.
     */
    void AddGAMGroup(ReferenceT<GAMGroup> element);

    /**
     * @brief Prepare the context for the state in each registered GAMGroup.
     * @param[in] status contains informations about the current and the next state.
     */
    void PrepareState(const RealTimeStateInfo &status);

    /**
     * @brief Returns the stateful GAMGroups array.
     * @return the stateful GAMGroups array.
     */
    ReferenceT<GAMGroup> * GetGAMGroups();

    /**
     * @brief Returns the number of GAMGroups currently registered.
     * @return the number of GAMGroups currently registered.
     */
    uint32 GetNumberOfGAMGroups() const;

private:

    /**
     * The stateful GAMGroups array.
     */
    ReferenceT<GAMGroup> * statefulGAMGroups;

    /**
     * The number of stateful GAMGroups registered.
     */
    uint32 numberOfGAMGroups;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMESTATE_H_ */

