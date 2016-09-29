/**
 * @file MessageFilterPool.h
 * @brief Header file for class MessageFilterPool
 * @date 22/08/2016
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class MessageFilterPool
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MESSAGEFILTERPOOL_H_
#define MESSAGEFILTERPOOL_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainer.h"
#include "MessageFilter.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Default message handling mechanism.
 * @details A container of MessageFilter objects which are tested when a new message is received. As soon as one of the filters
 *  declares that the message was consumed (i.e. that MessageFilter::ConsumeMessage returns ErrorManagement::NoError)
 *  the message will not be tested on any subsequent filters.
 *  Filters can be added to the container using the ReferenceContainer::Insert methods.
 */
class MessageFilterPool: public ReferenceContainer {

public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor. NOOP.
     */
    MessageFilterPool();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~MessageFilterPool();

    /**
     * @brief Tests the \a message on all the filters that belong to this container.
     * @details As soon as one of the filters declares that the message was consumed (i.e. that MessageFilter::ConsumeMessage
     * returns ErrorManagement::NoError) the message will not be tested on any subsequent filters.
     * If the filter is not permanent (!MessageFilter::IsPermanentFilter), it will be removed from the container after handling the message.
     * @param[in,out] message is the message to be tested on the installed filters.
     * @return
     *   ErrorManagement::NoError if at least one filter can consume the message (i.e. that MessageFilter::ConsumeMessage
     * returns ErrorManagement::NoError).
     *   ErrorManagement::UnsupportedFeature if no filter can consume the message.
     */
    ErrorManagement::ErrorType ReceiveMessage(ReferenceT<Message> &message);

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MESSAGEFILTERPOOL_H_ */
	
