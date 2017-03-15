/**
 * @file AdvancedErrorManagementTest.h
 * @brief Header file for class AdvancedErrorManagementTest
 * @date 12/03/2017
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

 * @details This header file contains the declaration of the class AdvancedErrorManagementTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ADVANCEDERRORMANAGEMENTTEST_H_
#define ADVANCEDERRORMANAGEMENTTEST_H_

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
 * @brief Tests the AdvancedErrorManagement macros.
 */
class AdvancedErrorManagementTest {
public:
    /**
     * @brief Tests the REPORT_ERROR_STATIC macro with no parameters
     */
    bool TestREPORT_ERROR_STATIC_NoParameters();

    /**
     * @brief Tests the REPORT_ERROR_STATIC macro with parameters
     */
    bool TestREPORT_ERROR_STATIC_Parameters();

    /**
     * @brief Tests the REPORT_ERROR macro with no parameters
     */
    bool TestREPORT_ERROR_NoParameters();

    /**
     * @brief Tests the REPORT_ERROR macro with parameters
     */
    bool TestREPORT_ERROR_Parameters();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_BAREMETAL_L3STREAMS_ADVANCEDERRORMANAGEMENTTEST_H_ */

