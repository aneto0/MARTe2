/**
 * @file MathExpressionParserTest.cpp
 * @brief Source file for class MathExpressionParserTest
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

 * @details This source file contains the definition of all the methods for
 * the class MathExpressionParserTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <cstdio>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MathExpressionParserTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                      MathExpressionParserHelperTest                       */
/*---------------------------------------------------------------------------*/

MathExpressionParserHelperTest::MathExpressionParserHelperTest(StreamString inputString) :
        MathExpressionParser(inputString) {
    
}

bool MathExpressionParserHelperTest::OperatorFormattingHelperTest(char8* operatorIn, const char8* expectedOutput) {
    
    bool ok = (StringHelper::Compare(OperatorFormatting(operatorIn), expectedOutput) == 0u);
    
    return ok;
}

bool MathExpressionParserHelperTest::PushOperatorHelperTest() {
    
    bool ok = true;
    
    Token customToken(1u, "STRING", "TOKEN", 1u);
    currentToken = &customToken;
    PushOperator();
    
    StreamString* tokenData;
    ok &= operatorStack.Peek(0u, tokenData);
    
    ok &= (StringHelper::Compare(tokenData->Buffer(), "TOKEN") == 0u);
    
    return ok;
    
}

bool MathExpressionParserHelperTest::PopOperatorHelperTest() {
    
    bool ok = true;
        
    StreamString* tokenData = new StreamString("TOKEN");
    ok &= operatorStack.Add(tokenData);
    
    PopOperator();
    
    // Now the operator should be in stackMachineExpr
    ok &= (StringHelper::Compare(stackMachineExpr.Buffer(), "TOKEN\n") == 0u);
    
    // Now the stack is empty so it should fail
    PopOperator();
    ok &= (StringHelper::Compare(stackMachineExpr.Buffer(), "TOKEN\nERR\n") == 0u);
    
    return ok;
    
    
}

/*---------------------------------------------------------------------------*/
/*                        MathExpressionParserTest                           */
/*---------------------------------------------------------------------------*/

bool MathExpressionParserTest::TestConstructor() {

    StreamString configString = "";

    StreamString err;

    MathExpressionParser myParser(configString, &err);

    GrammarInfo myGrammar=myParser.GetGrammarInfo();

    bool ok=(StringHelper::Compare(myGrammar.separators, MathGrammar.separators)==0);

    ok &= (StringHelper::Compare(myGrammar.beginOneLineComment, MathGrammar.beginOneLineComment)==0);
    ok &= (StringHelper::Compare(myGrammar.beginMultipleLinesComment, MathGrammar.beginMultipleLinesComment)==0);
    ok &= (StringHelper::Compare(myGrammar.endMultipleLinesComment, MathGrammar.endMultipleLinesComment)==0);
    ok &= (StringHelper::Compare(&myGrammar.assignment, &MathGrammar.assignment)==0);
    return ok;
}

bool MathExpressionParserTest::TestDestructor() {

    StreamString configString = "C = A + (float64) B";
    
    configString.Seek(0);
    MathExpressionParser myParser(configString);
    
    bool ok = myParser.Parse();

    return !ok;
}

bool MathExpressionParserTest::TestGetGrammarInfo() {
    return TestConstructor();
}

bool MathExpressionParserTest::TestGetStackMachineExpression() {
    
    StreamString errors;
    StreamString exprString = "Z = (float)12; X = 5 + B*(float)C*!(X - Y) + sin(X + Y) + Z;";
    StreamString compString = "CONST float 12\n"
                              "WRITE Z\n"
                              "CONST float64 5\n"
                              "READ B\n"
                              "READ C\n"
                              "CAST float\n"
                              "MUL\n"
                              "READ X\n"
                              "READ Y\n"
                              "SUB\n"
                              "FACT\n"
                              "MUL\n"
                              "ADD\n"
                              "READ X\n"
                              "READ Y\n"
                              "ADD\n"
                              "SIN\n"
                              "ADD\n"
                              "READ Z\n"
                              "ADD\n"
                              "WRITE X\n";

    exprString.Seek(0);
    compString.Seek(0);
    MathExpressionParser myParser(exprString, &errors);
    
    if (!myParser.Parse()) {
        return false;
    }
    
    StreamString outputString = myParser.GetStackMachineExpression();
    
    if (StringHelper::Compare(compString.Buffer(), outputString.Buffer()) != 0) {
       return false;
    }
    
    return true;
}

bool MathExpressionParserTest::TestOperatorFormatting() {
    
    MathExpressionParserHelperTest helperParser("");
    
    bool ok = true;
    StreamString  operatorIn[14u] = {"&&", "||", "^", "==", "!=", ">", "<", ">=", "<=", "+", "-", "*", "/", "!"};
    StreamString operatorOut[14u] = {"AND", "OR", "XOR", "EQ", "NEQ", "GT", "LT", "GTE", "LTE", "ADD", "SUB", "MUL", "DIV", "FACT"};
    
    for (uint32 idx = 0u; idx < 14u; idx++) {
        ok &= helperParser.OperatorFormattingHelperTest(operatorIn[idx].BufferReference(), operatorOut[idx].Buffer());
    }
    
    // Now test an illegal case
    char8* nullChar = NULL_PTR(char8*);
    const char8* errString = "ERR";
    ok &= helperParser.OperatorFormattingHelperTest(nullChar, errString);

    return ok;
    
}

bool MathExpressionParserTest::TestPushOperator() {
    
    MathExpressionParserHelperTest helperParser("");
    bool ok = helperParser.PushOperatorHelperTest();
    
    return ok;
}

bool MathExpressionParserTest::TestPopOperator() {
    
    MathExpressionParserHelperTest helperParser("");
    bool ok = helperParser.PopOperatorHelperTest();
    
    return ok;
}

bool MathExpressionParserTest::TestExpression(const char8* expressionIn, const char8* expectedOutputString)
{
    StreamString errors;
    StreamString exprString = expressionIn;
    StreamString compString = expectedOutputString;

    exprString.Seek(0);
    compString.Seek(0);
    MathExpressionParser myParser(exprString, &errors);
    
    if (!myParser.Parse()) {
        return false;
    }
    
    StreamString outputString = myParser.GetStackMachineExpression();
    
    if (StringHelper::Compare(compString.Buffer(), outputString.Buffer()) != 0) {
       return false;
    }
    
    return true;
}

bool MathExpressionParserTest::TestExpressionError(const char8* expressionIn)
{
    StreamString errors;
    StreamString exprString = expressionIn;
    
    exprString.Seek(0);
    MathExpressionParser myParser(exprString, &errors);
    
    bool ret = myParser.Parse();
    
    return !ret;

}


