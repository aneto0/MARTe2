/**
 * @file ConfigurationLoaderTCPTest.h
 * @brief Header file for class ConfigurationLoaderTCPTest
 * @date 16/02/2022
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

 * @details This header file contains the declaration of the class ConfigurationLoaderTCPTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L6APP_CONFIGURATIONLOADERTCPTEST_H_
#define L6APP_CONFIGURATIONLOADERTCPTEST_H_

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
 * @brief Tests the Loader public methods.
 */
class ConfigurationLoaderTCPTest {

public:

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the PostInit method.
     */
    bool TestPostInit();

    /**
     * @brief Tests that the PostInit method fails if the port is not specified.
     */
    bool TestPostInit_NoPort();

    /**
     * @brief Tests that the PostInit method fails if an invalid port is specified.
     */
    bool TestPostInit_InvalidPort();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute();

    /**
     * @brief Tests that the Execute method returns an error with an invalid configuration.
     */
    bool TestExecuteErr();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L6APP_CONFIGURATIONLOADERTCPTEST_H_ */
