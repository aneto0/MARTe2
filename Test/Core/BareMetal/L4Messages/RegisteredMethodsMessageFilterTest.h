/**
 * @file RegisteredMethodsMessageFilterTest.h
 * @brief Header file for class RegisteredMethodsMessageFilterTest
 * @date 09/09/2016
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

 * @details This header file contains the declaration of the class RegisteredMethodsMessageFilterTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REGISTEREDMETHODSMESSAGEFILTERTEST_H_
#define REGISTEREDMETHODSMESSAGEFILTERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Tests the RegisteredMethodsMessageFilterTest public methods.
 */
class RegisteredMethodsMessageFilterTest {
public:

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the SetDestinationObject method.
     */
    bool TestSetDestinationObject();

    /**
     * @brief Tests the ConsumeMessage method.
     */
    bool TestConsumeMessage();

    /**
     * @brief Tests the ConsumeMessage method with a message that is a reply (should not be handled).
     */
    bool TestConsumeMessage_Reply();

    /**
     * @brief Tests the ConsumeMessage method with a message that requests an indirect reply .
     */
    bool TestConsumeMessage_IndirectReply();

    /**
     * @brief Tests the IsPermanentFilter method (should return true).
     */
    bool TestIsPermanentFilter();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REGISTEREDMETHODSMESSAGEFILTERTEST_H_ */

