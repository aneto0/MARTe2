/**
 * @file Envelope.h
 * @brief Header file for class Envelope
 * @date 26/2/2016 TODO Verify the value and format of the date
 * @author IHerrero TODO Verify the name and format of the author
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

 * @details This header file contains the declaration of the class Envelope
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ENVELOPE_H_
#define 		ENVELOPE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MessageI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {


typedef char* Address; //TODO: Temporary forward reference until the actual class to be used is fixed.

//TODO: A copy constructor / assignment operator is needed on MessageI;


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
class DLL_API Envelope {
public:
    /**
     * @brief Constructor ...
     * @param[in] address
     * @param[in] message
     */
    Envelope(Address address, MessageI message);
    virtual ~Envelope();
    Address GetAddress() const;
    MessageI GetMessage() const;
private:
    Address address_;
    MessageI message_;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

inline Address Envelope::GetAddress() const {
    return address_;
}

inline MessageI Envelope::GetMessage() const {
    return message_;
}

}

#endif /* ENVELOPE_H_ */

