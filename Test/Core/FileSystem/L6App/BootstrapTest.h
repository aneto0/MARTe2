/**
 * @file BootstrapTest.h
 * @brief Header file for class BootstrapTest
 * @date 05/04/2018
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

 * @details This header file contains the declaration of the class BootstrapTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L6APP_BOOTSTRAPTEST_H_
#define L6APP_BOOTSTRAPTEST_H_

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
 * @brief Tests the Bootstrap public methods.
 */
class BootstrapTest {

public:

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the ReadParameters function.
     */
    bool TestReadParameters();

    /**
     * @brief Tests the ReadParameters function without specifying a loader.
     */
    bool TestReadParameters_FalseNoLoader();

    /**
     * @brief Tests the ReadParameters function without specifying a filename.
     */
    bool TestReadParameters_FalseNoFilename();

    /**
     * @brief Tests the GetConfigurationStream method.
     */
    bool TestGetConfigurationStream();

    /**
     * @brief Tests the GetConfigurationStream method with a wrong file.
     */
    bool TestGetConfigurationStream_False();

    /**
     * @brief Tests the CloseConfigurationStream method.
     */
    bool TestCloseConfigurationStream();

    /**
     * @brief Tests the CloseConfigurationStream method with a wrong file.
     */
    bool TestCloseConfigurationStream_False();

    /**
     * @brief The Run is not tested as this would block the tests.
     */
    //bool TestRun();

    /**
     * @brief Tests the Printf.
     */
    bool TestPrintf();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L6APP_BOOTSTRAPTEST_H_ */

