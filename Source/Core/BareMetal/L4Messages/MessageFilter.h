/**
 * @file MessageFilter.h
 * @brief Header file for class MessageFilter
 * @date 17/08/2016
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

 * @details This header file contains the declaration of the class MessageFilter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MESSAGEFILTER_H_
#define MESSAGEFILTER_H_

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


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Class to implement a filter on Message objects.
 */
class MessageFilter: public Object{
public:

    /**
     * @brief Constructor.
     * @post
     *   IsPermanentFilter() == isPermanentFilter &&
     *   IsMessageConsumed() == true.
     */
    MessageFilter(const bool isPermanentFilter);

    /**
     * @brief Destructor.
     */
    virtual ~MessageFilter();

    /**
     * @brief Returns true if the filter is to remain active after a successful match.
     * @return true if the filter is to remain active after a successful match.
     */
    bool IsPermanentFilter() const;

    /**
     * @brief Checks if the \a messageToTest is compatible with the Filter.
     * @param[in] messageToTest the message to be tested by this filter.
     * @return ErrorManagement::NoError if the message was consumed by this filter.
     */
    virtual ErrorManagement::ErrorType ConsumeMessage(ReferenceT<Message> &messageToTest) = 0;

private:

    /**
     * True if it remains active after a successful match.
     */
    bool permanentFilter;

    /*lint -e{1712} This class does not have a default constructor because
     * the decision to make a filter permanent must be defined on construction and remains constant
     * during object's lifetime*/
};

} // namespace

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/





#endif /* MESSAGEFILTER_H_ */
	
