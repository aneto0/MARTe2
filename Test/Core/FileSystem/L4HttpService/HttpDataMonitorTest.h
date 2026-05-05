/**
 * @file HttpDataMonitorTest.h
 * @brief Header file for class HttpDataMonitorTest
 * @date 05/08/2025
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

 * @details This header file contains the declaration of the class HttpDataMonitorTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPDATAMONITORTEST_H_
#define TEST_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPDATAMONITORTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "HttpDataMonitor.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Tests the HttpDataMonitor public methods.
 */
class HttpDataMonitorTest {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API HttpDataMonitorTest)
public:

    /**
     * @brief Constructor
     */
    HttpDataMonitorTest();

    /**
     * @brief Destructor
     */
    virtual ~HttpDataMonitorTest();

    /**
     * @brief Test the HttpDataMonitor constructor
     */
    bool TestConstructor();

    /**
     * @brief Test the HttpDataMonitor destructor
     */
    bool TestDestructor();

    /**
     * @brief Test the Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Test if the Initialise method returns false if no Signals declared
     */
    bool TestInitialise_False_NoSignals();

    /**
     * @brief Test if the Initialise method returns false if no Components declared
     */
    bool TestInitialise_False_NoComponents();

    /**
     * @brief Test the GetAsStructuredData method
     */
    bool TestGetAsStructuredData();

    /**
     * @brief Test the GetAsStructuredData method when Signals points to a node
     */
    bool TestGetAsStructuredData_Dir();

    /**
     * @brief Test if the GetAsStructuredData method returns empty plots when the component is invalid
     */
    bool TestGetAsStructuredData_False_InvalidComponent();

    /**
     * @brief Test the GetAsText method
     */
    bool TestGetAsText();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPDATAMONITORTEST_H_ */

