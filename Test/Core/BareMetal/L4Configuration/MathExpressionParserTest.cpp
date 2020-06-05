/**
 * @file MathExpressionParserTest.cpp
 * @brief Source file for class MathExpressionParserTest
 * @date 21/04/2020
 * @author nferron
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

    StreamString configString = "C = A + (float64) B"
                                ;
    
    configString.Seek(0);
    MathExpressionParser myParser(configString);
    
    bool ok = myParser.Parse();

    return !ok;
}

bool MathExpressionParserTest::TestGetGrammarInfo() {
    return TestConstructor();
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


