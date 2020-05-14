/**
 * @file MathExpressionParser.cpp
 * @brief Source file for class MathExpressionParser
 * @date Apr 7, 2020
 * @author Giuseppe Ferrò, RFX
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
 * the class MathExpressionParser (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MathExpressionParser.h"
#include "CompositeErrorManagement.h" // DEBUG remove this

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/




MathExpressionParser::MathExpressionParser() :
    ParserI(MathExpressionParserData::parserData) {
        

        //tokenProducer.TokenizeInput();
//    COMPOSITE_REPORT_ERROR(ErrorManagement::Debug, "Acquired terminals:: ", MathGrammar.terminals);
//    COMPOSITE_REPORT_ERROR(ErrorManagement::Debug, "Acquired string: ", (dynamic_cast<StreamString&>(stream)).Buffer());

    MapMethods();

}

MathExpressionParser::~MathExpressionParser() {
// Auto-generated destructor stub for MathExpressionParser
// TODO Verify if manual additions are needed
}

ErrorManagement::ErrorType  MathExpressionParser::PopAssignment(const Token *currentToken,BufferedStreamI *errorStream) {
    COMPOSITE_REPORT_ERROR(ErrorManagement::Debug, "PopAssignment ", currentToken->GetData());
    ErrorManagement::ErrorType  ret;
    
    // Write in the stack machine expression
    stackMachineExpr().Append("WRITE ").Append(assignmentVarName).Append('\n');
    return ret;
}

ErrorManagement::ErrorType  MathExpressionParser::PushOperator(const Token *currentToken,BufferedStreamI *errorStream){

    ErrorManagement::ErrorType ret;

    ret.parametersError = (currentToken == NULL_PTR(Token *));
    PARSER_ERROR_REPORT(errorStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

    if (ret){
        COMPOSITE_REPORT_ERROR(ErrorManagement::Debug, "PushOperator ", currentToken->GetData());

        ret = operatorStack.Push(currentToken->GetData());
        REPORT_ERROR(ret, "operatorStack.Push failed ");
    }
    return ret;
}

ErrorManagement::ErrorType  MathExpressionParser::PopOperator(const Token *currentToken,BufferedStreamI *errorStream){

    ErrorManagement::ErrorType ret;

    ret.parametersError = (currentToken == NULL_PTR(Token *));
    PARSER_ERROR_REPORT(errorStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

    if (ret){
        COMPOSITE_REPORT_ERROR(ErrorManagement::Debug, "PopOperator ", currentToken->GetData());
//        uint32 top = operatorStack.GetSize() - 1;
//        StreamString* currentOperator;
//        operatorStack.Extract(top, currentOperator);

        // Write in the stack machine expression
//        stackMachineExpr += OperatorFormatting(currentOperator->Buffer());
//        stackMachineExpr += "\n";

        DynamicCString currentOperator;
        ret = operatorStack.Pop(currentOperator);
        REPORT_ERROR(ret, "operatorStack.Pop failed");

		if (ret){
	        stackMachineExpr().Append(currentOperator).Append('\n');
		}
    }
    return ret;
}


ErrorManagement::ErrorType   MathExpressionParser::PopOperatorAlternate(const Token *currentToken,BufferedStreamI *errorStream) {
    ErrorManagement::ErrorType ret;

    ret.parametersError = (currentToken == NULL_PTR(Token *));
    PARSER_ERROR_REPORT(errorStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

    if (ret){
        COMPOSITE_REPORT_ERROR(ErrorManagement::Debug, "PopOperatorAlternate ", currentToken->GetData());
//        uint32 top = operatorStack.GetSize() - 1;
//        StreamString* currentOperator;
//        operatorStack.Extract(top, currentOperator);

        DynamicCString currentOperator;
        ret = operatorStack.Pop(currentOperator);
        REPORT_ERROR(ret, "operatorStack.Pop failed");

        if (currentOperator== "+") {
            // nothing
        }
        else if (currentOperator == "-") {
            stackMachineExpr().Append("NEG\n");
        }
        else {
            stackMachineExpr().Append("ERR\n");
            ret.syntaxError = true;
            PARSER_ERROR_REPORT(errorStream,ret,"PopOperatorAlternat(): operator %s has no alternative form.", currentOperator);
        }
    }

    return ret;
}

ErrorManagement::ErrorType  MathExpressionParser::PushTypecast(const Token *currentToken,BufferedStreamI *errorStream){

    ErrorManagement::ErrorType ret;

    ret.parametersError = (currentToken == NULL_PTR(Token *));
    PARSER_ERROR_REPORT(errorStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

    if (ret){
        COMPOSITE_REPORT_ERROR(ErrorManagement::Debug, "PushTypecast ", currentToken->GetData());
//        StreamString* currentOperator = new StreamString(currentToken->GetData());

        ret = typecastStack.Push(currentToken->GetData());
    }
    return ret;
}


ErrorManagement::ErrorType  MathExpressionParser::PopTypecast(const Token *currentToken,BufferedStreamI *errorStream){

    ErrorManagement::ErrorType ret;

    ret.parametersError = (currentToken == NULL_PTR(Token *));
    PARSER_ERROR_REPORT(errorStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

    if (ret){
//        COMPOSITE_REPORT_ERROR(ErrorManagement::Debug, "PopTypecast ", currentToken->GetData());
//        uint32 top = typecastStack.GetSize() - 1;
//        StreamString* currentOperator;
//        typecastStack.Extract(top, currentOperator);

        DynamicCString castType;
        ret = typecastStack.Pop(castType);
        REPORT_ERROR(ret, "operatorStack.Pop failed");

        if (ret){
        	stackMachineExpr().Append("CAST ").Append(castType).Append('\n');
        }
    }
    return ret;
}

ErrorManagement::ErrorType  MathExpressionParser::AddOperand(const Token *currentToken,BufferedStreamI *errorStream){

    ErrorManagement::ErrorType ret;

    ret.parametersError = (currentToken == NULL_PTR(Token *));
    PARSER_ERROR_REPORT(errorStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

    if (ret){
        COMPOSITE_REPORT_ERROR(ErrorManagement::Debug, "AddOperand ", currentToken->GetData());
        //    COMPOSITE_REPORT_ERROR(ErrorManagement::Debug, "Add Operand  ", currentToken->GetData());
        // Write in the stack machine expression
        if (currentToken->GetDescription() == "STRING"){
            stackMachineExpr().Append("READ ");
        }
        else if (currentToken->GetDescription() == "NUMBER"){
        	stackMachineExpr().Append("CONST ");
        } else {
        	stackMachineExpr().Append("ERR ");
        }
        
        stackMachineExpr().Append(currentToken->GetData()).Append('\n');
    }
    return ret;
}

ErrorManagement::ErrorType  MathExpressionParser::AddOperandTypecast(const Token *currentToken,BufferedStreamI *errorStream){

    ErrorManagement::ErrorType ret;

    ret.parametersError = (currentToken == NULL_PTR(Token *));
    PARSER_ERROR_REPORT(errorStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

    if (ret){
        COMPOSITE_REPORT_ERROR(ErrorManagement::Debug, "AddOperandTypecast ", currentToken->GetData());
//        uint32 top = typecastStack.GetSize() - 1;
//        StreamString* currentOperator;
//        typecastStack.Extract(top, currentOperator);
        DynamicCString castType;
        ret = typecastStack.Pop(castType);
        REPORT_ERROR(ret, "operatorStack.Pop failed");

        if (ret){
        	stackMachineExpr().Append("CONST ").Append(castType).Append(currentToken->GetData()).Append('\n');
        }
    }
    return ret;
}

ErrorManagement::ErrorType  MathExpressionParser::StoreAssignment(const Token *currentToken,BufferedStreamI *errorStream){

    ErrorManagement::ErrorType ret;

    ret.parametersError = (currentToken == NULL_PTR(Token *));
    PARSER_ERROR_REPORT(errorStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

    if (ret){
        COMPOSITE_REPORT_ERROR(ErrorManagement::Debug, "StoreAssignment ", currentToken->GetData());

        assignmentVarName = currentToken->GetData();
    }
    return ret;
}

ErrorManagement::ErrorType  MathExpressionParser::Execute(const uint32 number,const Token *currentToken,BufferedStreamI *errorStream) {
    return (this->*Action[number])(currentToken,errorStream);
}


CCString MathExpressionParser::GetStackMachineExpression() const {
    return stackMachineExpr;
}

}
