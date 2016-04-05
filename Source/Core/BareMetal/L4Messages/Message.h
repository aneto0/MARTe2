/**
 * @file Message.h
 * @brief Header file for class Message
 * @date Apr 5, 2016
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

 * @details This header file contains the declaration of the class Message
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainer.h"
#include "CString.h"
#include "StreamString.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 *
 * */
class DLL_API Message: public ReferenceContainer{

    /**
     * who is the originator of the message
     * null means anonymous
     * */
    StreamString sender;

    /**
     * who is the destination of the message
     * addressed from GODB forward
     * */
    StreamString destination;

    /**
     * what is the function of this message
     * */
    StreamString function;

public:

    CLASS_REGISTER_DECLARATION()

    /**
     * TODO
     * */
    Message(CCString newSender, CCString newDestination, CCString newFunction){
       sender      = newSender;
       destination = newDestination;
       function    = newFunction;
    }

    /**
     * TODO
     * */
    bool Initialise(StructuredDataI &data){
        // TODO find sender field and initialise sender
        // TODO find destination field and initialise destination
        // TODO find function field and initialise function
        return ReferenceContainer::Initialise(data);
    };

    /**
     * TODO
     * */
    virtual ~Message(){

    }

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MESSAGE_H_ */
	
