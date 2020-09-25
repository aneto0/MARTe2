/**
 * @file MathExpressionParserTest.h
 * @brief Header file for class MathExpressionParserTest
 * @date 21/04/2020
 * @author Nicolo Ferron
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

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "MathExpressionParser.h"
#include "Token.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Class for testing MathExpressionParser protected methods.
 */
class MathExpressionParserHelperTest : public MathExpressionParser {
   
public:
    
    /**
     * @brief Constructor.
     */
    MathExpressionParserHelperTest(StreamString inputString);
    
    /**
     * @brief Gives access to MathExpressionParser::OperatorFormatting() method for testing.
     */
    bool OperatorFormattingHelperTest(char8* operatorIn, const char8* expectedOutput);
    
    /**
     * @brief Gives access to MathExpressionParser::PushOperator() method for testing.
     */
    bool PushOperatorHelperTest();
    
    /**
     * @brief Gives access to MathExpressionParser::PopOperator() method for testing.
     */
    bool PopOperatorHelperTest();
    
    /**
     * @brief Gives access to MathExpressionParser::PopOperatorAlternate() method for testing.
     */
    bool PopOperatorAlternateHelperTest();
    
    /**
     * @brief Gives access to MathExpressionParser::PushTypecast() method for testing.
     */
    bool PushTypecastHelperTest();
    
    /**
     * @brief Gives access to MathExpressionParser::PopTypecast() method for testing.
     */
    bool PopTypecastHelperTest();
    
    /**
     * @brief Gives access to MathExpressionParser::AddOperand() method for testing.
     */
    bool AddOperandHelperTest();
    
    /**
     * @brief Gives access to MathExpressionParser::AddOperandTypecast() method for testing.
     */
    bool AddOperandTypecastHelperTest();
    
    /**
     * @brief Gives access to MathExpressionParser::StoreAssignment() method for testing.
     */
    bool StoreAssignmentHelperTest();
    
    /**
     * @brief Gives access to MathExpressionParser::PopAssignment() method for testing.
     */
    bool PopAssignmentHelperTest();
};

class MathExpressionParserTest {

public:
    
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();
    
    /**
     * @brief   Tests the destructor.
     * @details The test tries to feed the parser with an expression not ended
     *          by `;`, resulting in a premature abort and typecastStack
     *          and operatorStack not popped. The test thus checks the
     *          destructor ability to empty those stacks when required. 
     */
    bool TestDestructor();

    /**
     * @brief Tests if the function returns the math grammar.
     */
    bool TestGetGrammarInfo();
    
    /**
     * @brief Tests GetStackMachineExpression() method.
     */
    bool TestGetStackMachineExpression();
    
    /**
     * @brief Test OperatorFormatting method.
     */
    bool TestOperatorFormatting();
    
    /**
     * @brief Test PushOperator method.
     */
    bool TestPushOperator();
    
    /**
     * @brief Test PopOperator method.
     */
    bool TestPopOperator();
    
    /**
     * @brief Test PopOperatorAlternate method.
     */
    bool TestPopOperatorAlternate();
    
    /**
     * @brief Test PushOperator method.
     */
    bool TestPushTypecast();
    
    /**
     * @brief Test PopOperator method.
     */
    bool TestPopTypecast();
    
    /**
     * @brief Test AddOperand method.
     */
    bool TestAddOperand();
    
    /**
     * @brief Test AddOperand method.
     */
    bool TestAddOperandTypecast();
    
    /**
     * @brief Test StoreAssignment method.
     */
    bool TestStoreAssignment();
    
    /**
     * @brief Test PopAssignment method.
     */
    bool TestPopAssignment();
    
    /**
     * @brief Tests the parsing of an expression by comparing the result with the expected output.
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

