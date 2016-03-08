/**
 * @file MessageBroker.h
 * @brief Header file for class MessageBroker
 * @date 26/02/2016
 * @author Ivan Herrero
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

 * @details This header file contains the declaration of the class MessageBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MESSAGEBROKER_H_
#define 		MESSAGEBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Envelope.h"
#include "MessageI.h"
#include "CompilerTypes.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief TODO Write into the brief the main concept that the class represents 
 * explained into a single line paragraph.
 * @details TODO Write into the details a detailed description of the class 
 * that can span multiple lines on one paragraph. If more paragraphs are 
 * needed, then put subsequent "@details" tags. This description shall explain 
 * the  concept which the class represents alongside its responsibilities or 
 * services it provides. Example: "This class represents a stack of doubles,
 * that is, a data structure which ..."
 */
class DLL_API MessageBroker : public GlobalObjectI {
public:

    /**
     * @brief Singleton access to the MessageBroker.
     * @return a pointer to the MessageBroker.
     */
    static MessageBroker *Instance();

    void SendMessage(const Envelope &envelope, MessageI &reply);

    virtual const char8 * const GetClassName() const;
private:
    MessageBroker();
    ~MessageBroker();
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

inline const char8 * const MessageBroker::GetClassName() const {
    return "MessageBroker";
}

}

#endif /* MESSAGEBROKER_H_ */

