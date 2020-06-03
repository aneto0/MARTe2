/**
 * @file HttpDefinitionTest.h
 * @brief Header file for class HttpDefinitionTest
 * @date 17/09/2018
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class HttpDefinitionTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPDEFINITIONTEST_H_
#define HTTPDEFINITIONTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HttpDefinition.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
/**
 * @brief Tests the HttpDefinition methods
 */
class HttpDefinitionTest {
public:

    /**
     * @brief Constructor
     */
    HttpDefinitionTest();

    /**
     * @brief Destructor
     */
    virtual ~HttpDefinitionTest();

    /**
     * @brief Tests the HttpEncode method.
     */
    bool TestHttpEncode();

    /**
     * @brief Tests the HexDecode method.
     */
    bool TestHexDecode();

    /**
     * @brief Tests the HttpDecode method.
     */
    bool TestHttpDecode();

    /**
     * @brief Tests the IsReplyCode method.
     */
    bool TestIsReplyCode();

    /**
     * @brief Tests the GenerateReplyCode method.
     */
    bool TestGenerateReplyCode();

    /**
     * @brief Tests the GetErrorCodeString method.
     */
    bool TestGetErrorCodeString();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HTTPDEFINITIONTEST_H_ */

