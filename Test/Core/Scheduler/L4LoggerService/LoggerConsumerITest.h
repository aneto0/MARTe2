/**
 * @file LoggerConsumerITest.h
 * @brief Header file for class LoggerConsumerITest
 * @date 13/03/2017
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

 * @details This header file contains the declaration of the class LoggerConsumerITest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LOGGERCONSUMERITEST_H_
#define LOGGERCONSUMERITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "LoggerConsumerI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the LoggerConsumerI public methods.
 */
class LoggerConsumerITest {
public:
    /**
     * @brief Tests the LoadPrintPreferences method.
     */
    bool TestLoadPrintPreferences();

    /**
     * @brief Tests the LoadPrintPreferences method with only some keys.
     */
    bool TestLoadPrintPreferences_SomeKeys();

    /**
     * @brief Tests the LoadPrintPreferences method with a Format that is too large.
     */
    bool TestLoadPrintPreferences_False_TooLarge();

    /**
     * @brief Tests the LoadPrintPreferences method with an invalid key.
     */
    bool TestLoadPrintPreferences_False_InvalidKey();

    /**
     * @brief Tests the PrintToStream method.
     */
    bool TestPrintToStream();

    /**
     * @brief Tests the PrintToStream method with keys enabled.
     */
    bool TestPrintToStream_WithKeys();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LOGGERCONSUMERITEST_H_ */

