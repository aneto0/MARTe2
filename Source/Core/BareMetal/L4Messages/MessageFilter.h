/**
 * @file MessageFilter.h
 * @brief Header file for class MessageFilter
 * @date Aug 17, 2016
 * @author fsartori
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

 * @details This header file contains the declaration of the class MessageFilter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4MESSAGES_MESSAGEFILTER_H_
#define L4MESSAGES_MESSAGEFILTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "CString.h"
#include "Message.h"
#include "StringHelper.h"
#include "ReferenceT.h"

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief class to implement a filter on Messages.
 */
class MessageFilter: public Object{
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * TODO
     * Initialises basic search filter
     *
     */
    inline MessageFilter(bool isPermanentFilter);

    /**
     * TODO
     * Initialises basic search filter
     *
     */
    virtual ~MessageFilter();

    /**
     * TODO
     */
    inline bool IsPermanentFilter(){
        return permanentFilter;
    }

    /**
     * TODO
     * Single test of a message.
     * Also try consuming (uses and does not delete it) the message if matched
    */
    virtual ErrorManagement::ErrorType ConsumeMessage(ReferenceT<Message> &messageToTest)= 0;

    /**
     * TODO
     * Was the message consumed?
    */
    inline bool MessageConsumed(ErrorManagement::ErrorType ret);

private:

    /**
     * True if it remains active after a successful match
     */
    bool permanentFilter;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


inline bool MessageFilter::MessageConsumed(ErrorManagement::ErrorType ret){
    return !ret.unsupportedFeature && !ret.parametersError;
}

MessageFilter::MessageFilter(bool isPermanentFilter){
    permanentFilter = isPermanentFilter;
}


} // namespace


#endif /* L4MESSAGES_MESSAGEFILTER_H_ */
	
