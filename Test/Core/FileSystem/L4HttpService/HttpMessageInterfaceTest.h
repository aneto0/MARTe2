/**
 * @file HttpMessageInterfaceTest.h
 * @brief Header file for class HttpMessageInterfaceTest
 * @date 18/03/2019
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class HttpMessageInterfaceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPMESSAGEINTERFACETEST_H_
#define TEST_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPMESSAGEINTERFACETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Tests the HttpMessageInterface public methods.
 */
class HttpMessageInterfaceTest {
public:
    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests that the Initialise method fails if there are childs which are not messages.
     */
    bool TestInitialise_False_Not_Only_Messages();

    /**
     * @brief Tests the GetAsStructuredData.
     */
    bool TestGetAsStructuredData();

    /**
     * @brief Tests the GetAsText.
     */
    bool TestGetAsText();

    /**
     * @brief Tests the GetAsStructuredData with a Message that does not exist.
     */
    bool TestGetAsStructuredData_Message_Does_Not_Exist();

    /**
     * @brief Tests the GetAsText with a Message that does not exist.
     */
    bool TestGetAsText_Message_Does_Not_Exist();

    /**
     * @brief Tests the GetAsStructuredData with a Message that cannot be sent.
     */
    bool TestGetAsStructuredData_Message_Cannot_Be_Sent();

    /**
     * @brief Tests the GetAsText with a Message that cannot be sent.
     */
    bool TestGetAsText_Message_Cannot_Be_Sent();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPMESSAGEINTERFACETEST_H_ */
