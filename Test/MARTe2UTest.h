/**
 * @file MARTe2UTest.h
 * @brief Header file for module MARTe2UTest
 * @date 12/06/2021
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

 * @details This header file contains the declaration of the module MARTe2UTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MARTE2_UNIT_TEST_H_
#define MARTE2_UNIT_TEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include QUOTE(MARTe2_TEST_ENVIRONMENT/MARTe2UTestMacros.h)

/*---------------------------------------------------------------------------*/
/*                           Module declaration                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    namespace UnitTest {
        /**
         * @brief Prepare the test environment. It will be called before the Run().
         * @param[in] argc as provided by the call to the main()
         * @param[in] argv as provided by the call to the main()
         * @return true if the environment was successfully prepared to allow execution of the tests.
         * @post
         *  Run
         */
        bool PrepareTestEnvironment(int argc, char **argv);

        /**
         * @brief Executes all the unit tests.
         * @return true if the tests were successfully executed.
         * @pre
         *  PrepareTestEnvironment
         * @post
         *  CleanTestEnvironment
         */
        bool Run();

        /**
         * @brief Cleans the test environment. It will always be called.
         * @pre
         *  PrepareTestEnvironment
         *  Run
         */
        void CleanTestEnvironment();
    }

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif 
