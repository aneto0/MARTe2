/**
 * @file ObjectRegistryDatabaseMessageFilter.h
 * @brief Header file for class ObjectRegistryDatabaseMessageFilter
 * @date 19/06/2018
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class ObjectRegistryDatabaseMessageFilter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4MESSAGES_OBJECTREGISTRYDATABASEMESSAGEFILTER_H_
#define L4MESSAGES_OBJECTREGISTRYDATABASEMESSAGEFILTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A message filter that allows to purge and load the ObjectRegistryDatabase.
 * @details This filter will accepts messages with two type of functions, named "purge" and "load".
 *
 * If the message contains a valid StructuredDataI (messageToTest->Get(0u)), a parameter named Root allows to act on the specified
 *  subtree of the ObjectRegistryDatabase.
 *
 * The "purge" function will Purge() the ObjectRegistryDatabase (or a specified SubTree). The "load" function will Initialise the ObjectRegistryDatabase (or a specified SubTree)
 *  with the configuration available in messageToTest->Get(0u).
 *
 */
class ObjectRegistryDatabaseMessageFilter: public MessageFilter, public Object {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Sets the MessageFilter as permanent.
     */
    ObjectRegistryDatabaseMessageFilter();

    /**
     * @brief NOOP.
     */
    virtual ~ObjectRegistryDatabaseMessageFilter();

    /**
     * @brief Implements the specification detailed in the class description.
     * @param[in] messageToTest the message.
     * @return ErrorManagement::NoError if the message is valid (i.e. contains one of the two known functions) and the function is successfully completed.
     */
    virtual ErrorManagement::ErrorType ConsumeMessage(ReferenceT<Message> &messageToTest);

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L4MESSAGES_OBJECTREGISTRYDATABASEMESSAGEFILTER_H_ */

