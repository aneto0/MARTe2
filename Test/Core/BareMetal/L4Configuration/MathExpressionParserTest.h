/**
 * @file MathExpressionParserTest.h
 * @brief Header file for class MathExpressionParserTest
 * @date 21/04/2020
 * @author RFX
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

 * @details This header file contains the declaration of the class MathExpressionParserTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MATHEXPRESSIONPARSERTEST_H_
#define MATHEXPRESSIONPARSERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include "MathExpressionParser.h"
#include "ConfigurationDatabase.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class MathExpressionParserTest {

public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests if the function returns the math grammar.
     */
    bool TestGetGrammarInfo();
    
    /**
     * @brief Tests the parsing of expression by comparing the result with the expected output.
     */
    bool TestExpression(const char8* expressionIn, const char8* expectedOutputString);
    
    /**
     * @brief Tests if the MathExpressionParser::Parse() function returns false in case of errors in the input expression.
     */
    bool TestExpressionError(const char8* expressionIn);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MATHEXPRESSIONPARSERTEST_H_ */
