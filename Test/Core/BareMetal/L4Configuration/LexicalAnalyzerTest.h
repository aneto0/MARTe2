/**
 * @file LexicalAnalyzerTest.h
 * @brief Header file for class LexicalAnalyzerTest
 * @date 26/11/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class LexicalAnalyzerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LEXICALANALYZERTEST_H_
#define LEXICALANALYZERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "LexicalAnalyzer.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
/**
 * @brief Tests all the LexicalAnalyzer functions
 */
class LexicalAnalyzerTest {

public:

    /**
     * @brief Tests that the constructor correctly sets the tokens.
     */
    bool TestConstructor();

    /**
     * @brief Tests the GetToken function by tokenising a multi-line string containing all the known terminals.
     */
    bool TestGetToken();

    /**
     * @brief Tests the PeekToken function by tokenising a multi-line string containing all the known terminals.
     */
    bool TestPeekToken();

    /**
     * @brief Tests that the GetToken correctly handles the escape characters.
     */
    bool TestEscape();

    /**
     * @brief Tests that the lexical analyzer skips correctly comments in the stream.
     */
    bool TestComments();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LEXICALANALYZERTEST_H_ */

