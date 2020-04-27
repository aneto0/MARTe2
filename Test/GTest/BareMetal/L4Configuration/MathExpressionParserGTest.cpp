/**
 * @file MathExpressionParserGTest.cpp
 * @brief Source file for class MathExpressionParserGTest
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

 * @details This source file contains the definition of all the methods for
 * the class MathExpressionParserGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


#include "MathExpressionParserTest.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestConstructor) {
    MathExpressionParserTest parserTest;
    ASSERT_TRUE(parserTest.TestConstructor());
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestGetGrammarInfo) {
    MathExpressionParserTest parserTest;
    ASSERT_TRUE(parserTest.TestGetGrammarInfo());
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpression_Sum)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = A + b;";
    const char8* expectedOutput = "READ A\n"
                                  "READ b\n"
                                  "SUM\n"
                                  "WRITE ret\n";
            
    ASSERT_TRUE(parserTest.TestExpression(expression, expectedOutput));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpression_Product)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = A * b;";
    const char8* expectedOutput = "READ A\n"
                                  "READ b\n"
                                  "MUL\n"
                                  "WRITE ret\n";
            
    ASSERT_TRUE(parserTest.TestExpression(expression, expectedOutput));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpression_Power)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = A^b;";
    const char8* expectedOutput = "READ A\n"
                                  "READ b\n"
                                  "POW\n"
                                  "WRITE ret\n";
            
    ASSERT_TRUE(parserTest.TestExpression(expression, expectedOutput));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpression_Parentheses_Sum)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = (A + b);";
    const char8* expectedOutput = "READ A\n"
                                  "READ b\n"
                                  "SUM\n"
                                  "WRITE ret\n";
            
    ASSERT_TRUE(parserTest.TestExpression(expression, expectedOutput));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpression_Unary)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = sin(A);";
    const char8* expectedOutput = "READ A\n"
                                  "SIN\n"
                                  "WRITE ret\n";
            
    ASSERT_TRUE(parserTest.TestExpression(expression, expectedOutput));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpression_TypecastConstant)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = A + (float32) 0.99;";
    const char8* expectedOutput = "READ A\n"
                                  "CONST float32 0.99\n"
                                  "SUM\n"
                                  "WRITE ret\n";
            
    ASSERT_TRUE(parserTest.TestExpression(expression, expectedOutput));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpression_Constant)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = A + 0.99;";
    const char8* expectedOutput = "READ A\n"
                                  "CONST 0.99\n"
                                  "SUM\n"
                                  "WRITE ret\n";
            
    ASSERT_TRUE(parserTest.TestExpression(expression, expectedOutput));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpression_VariableTypecast)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = (bool) A;";
    const char8* expectedOutput = "READ A\n"
                                  "CAST bool\n"
                                  "WRITE ret\n";
            
    ASSERT_TRUE(parserTest.TestExpression(expression, expectedOutput));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpression_TypecastAsOperand)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = (bool)A*B;";
    const char8* expectedOutput = "READ A\n"
                                  "CAST bool\n"
                                  "READ B\n"
                                  "MUL\n"
                                  "WRITE ret\n";
            
    ASSERT_TRUE(parserTest.TestExpression(expression, expectedOutput));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpression_ExpressionWithFunctions)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = (sin(A) + cos(B + foo))^tan(log(D));";
    const char8* expectedOutput = "READ A\n"
                                  "SIN\n"
                                  "READ B\n"
                                  "READ foo\n"
                                  "SUM\n"
                                  "COS\n"
                                  "SUM\n"
                                  "READ D\n"
                                  "LOG\n"
                                  "TAN\n"
                                  "POW\n"
                                  "WRITE ret\n";

            
    ASSERT_TRUE(parserTest.TestExpression(expression, expectedOutput));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpression_CompleteExpression_1)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = sin(A +B) > ((type)(C+D) * tan((bool)E + (float)15));";
    const char8* expectedOutput = "READ A\n"
                                  "READ B\n"
                                  "SUM\n"
                                  "SIN\n"
                                  "READ C\n"
                                  "READ D\n"
                                  "SUM\n"
                                  "CAST type\n"
                                  "READ E\n"
                                  "CAST bool\n"
                                  "CONST float 15\n"
                                  "SUM\n"
                                  "TAN\n"
                                  "MUL\n"
                                  "COMP\n"
                                  "WRITE ret\n";
            
    ASSERT_TRUE(parserTest.TestExpression(expression, expectedOutput));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpression_CompleteExpression_2)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = A+ (B*C-(D/E^F)*G)*H;";
    const char8* expectedOutput = "READ A\n"
                                  "READ B\n"
                                  "READ C\n"
                                  "MUL\n"
                                  "READ D\n"
                                  "READ E\n"
                                  "READ F\n"
                                  "POW\n"
                                  "DIV\n"
                                  "READ G\n"
                                  "MUL\n"
                                  "MIN\n"
                                  "READ H\n"
                                  "MUL\n"
                                  "SUM\n"
                                  "WRITE ret\n";
            
    ASSERT_TRUE(parserTest.TestExpression(expression, expectedOutput));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpression_CompleteExpression_3)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = 5 + B*(float)C*!(X - Y) + sin(X + Y) + Z;";
    const char8* expectedOutput = "CONST 5\n"
                                   "READ B\n"
                                   "READ C\n"
                                   "CAST float\n"
                                   "MUL\n"
                                   "READ X\n"
                                   "READ Y\n"
                                   "MIN\n"
                                   "FACT\n"
                                   "MUL\n"
                                   "SUM\n"
                                   "READ X\n"
                                   "READ Y\n"
                                   "SUM\n"
                                   "SIN\n"
                                   "SUM\n"
                                   "READ Z\n"
                                   "SUM\n"
                                   "WRITE ret\n";
            
    ASSERT_TRUE(parserTest.TestExpression(expression, expectedOutput));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpressionError_Assignment_NoStatement)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "(A + b)";
            
    ASSERT_TRUE(parserTest.TestExpressionError(expression));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpressionError_Assignment_MultipleVariables)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret + b = A";
            
    ASSERT_TRUE(parserTest.TestExpressionError(expression));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpressionError_UnbalancedParenthesis_1)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = (A + b";
            
    ASSERT_TRUE(parserTest.TestExpressionError(expression));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpressionError_UnbalancedParenthesis_2)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = A + b)";
            
    ASSERT_TRUE(parserTest.TestExpressionError(expression));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpressionError_UndefinedOperator)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = (A b)";
            
    ASSERT_TRUE(parserTest.TestExpressionError(expression));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpressionError_EmptyParentheses)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = ()";
            
    ASSERT_TRUE(parserTest.TestExpressionError(expression));
}

TEST(BareMetal_L4Configuration_MathExpressionParserGTest,TestExpressionError_InvalidVariableName)
{
    MathExpressionParserTest parserTest;
    
    const char8* expression     = "ret = 12A";
            
    ASSERT_TRUE(parserTest.TestExpressionError(expression));
}

