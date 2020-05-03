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

CCString MathExpressionParser::OperatorLookupTable(CCString operatorIn)
{
	const char8* ret = operatorIn;
	
	if (operatorIn == "+")
		ret = "SUM";
		
	if (operatorIn == "*")
		ret = "MUL";
	
	if (operatorIn == ">")
		ret = "COMP";
		
	if (operatorIn == "/")
		ret = "DIV";
		
	if (operatorIn == "^")
		ret = "POW";
		
	if (operatorIn == "-")
		ret = "MIN";
	
	if (operatorIn == "sin")
		ret = "SIN";
	
	if (operatorIn == "cos")
		ret = "COS";
	
	if (operatorIn == "tan")
		ret = "TAN";
	
	if (operatorIn == "log")
		ret = "LOG";
		
	return ret;
	
} // OperatorLookupTable()

ErrorManagement::ErrorType  MathExpressionParser::End(const Token *currentToken,BufferedStreamI *errorStream){

	ErrorManagement::ErrorType ret;

	ret.parametersError = (currentToken == NULL_PTR(Token *));
	PARSER_ERROR_REPORT(errorStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

	if (ret){
		// Write in the stack machine expression
		stackMachineExpr += "WRITE ";
	    stackMachineExpr += assignmentVarName.Buffer();
	    stackMachineExpr += "\n";
	}
	return ret;
}

ErrorManagement::ErrorType  MathExpressionParser::PushOperator(const Token *currentToken,BufferedStreamI *errorStream){

	ErrorManagement::ErrorType ret;

	ret.parametersError = (currentToken == NULL_PTR(Token *));
	PARSER_ERROR_REPORT(errorStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

	if (ret){

		StreamString* operat = new StreamString(currentToken->GetData());

		operatorStack.Add(operat);
	}
	return ret;
}

ErrorManagement::ErrorType  MathExpressionParser::PopOperator(const Token *currentToken,BufferedStreamI *errorStream){


	ErrorManagement::ErrorType ret;

	ret.parametersError = (currentToken == NULL_PTR(Token *));
	PARSER_ERROR_REPORT(errorStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

	if (ret){
		uint32 top = operatorStack.GetSize() - 1;
		StreamString* operat;
		operatorStack.Extract(top, operat);

//		COMPOSITE_REPORT_ERROR(ErrorManagement::Debug, "Add Operator ", operat->Buffer());

		// Write in the stack machine expression
		stackMachineExpr += OperatorLookupTable(operat->Buffer());
		stackMachineExpr += "\n";
	}
	return ret;
}

ErrorManagement::ErrorType  MathExpressionParser::PushTypecast(const Token *currentToken,BufferedStreamI *errorStream){

	ErrorManagement::ErrorType ret;

	ret.parametersError = (currentToken == NULL_PTR(Token *));
	PARSER_ERROR_REPORT(errorStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

	if (ret){
		StreamString* operat = new StreamString(currentToken->GetData());

		typecastStack.Add(operat);
	}
	return ret;
}

ErrorManagement::ErrorType  MathExpressionParser::PopTypecast(const Token *currentToken,BufferedStreamI *errorStream){

	ErrorManagement::ErrorType ret;

	ret.parametersError = (currentToken == NULL_PTR(Token *));
	PARSER_ERROR_REPORT(errorStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

	if (ret){
		uint32 top = typecastStack.GetSize() - 1;
		StreamString* operat;

		typecastStack.Extract(top, operat);

		// Write in the stack machine expression
		stackMachineExpr += "CAST ";
		stackMachineExpr += operat->Buffer();
		stackMachineExpr += "\n";
	}
	return ret;
}

ErrorManagement::ErrorType  MathExpressionParser::AddOperand(const Token *currentToken,BufferedStreamI *errorStream){

	ErrorManagement::ErrorType ret;

	ret.parametersError = (currentToken == NULL_PTR(Token *));
	PARSER_ERROR_REPORT(errorStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

	if (ret){
		//	COMPOSITE_REPORT_ERROR(ErrorManagement::Debug, "Add Operand  ", currentToken->GetData());
	    // Write in the stack machine expression
	    if (currentToken->GetDescription() == "STRING")
			stackMachineExpr += "READ ";

		else if (currentToken->GetDescription() == "NUMBER")
			stackMachineExpr += "CONST ";
		
		stackMachineExpr += currentToken->GetData();
		stackMachineExpr += "\n";
	}
	return ret;
}

ErrorManagement::ErrorType  MathExpressionParser::AddOperandTypecast(const Token *currentToken,BufferedStreamI *errorStream){

	ErrorManagement::ErrorType ret;

	ret.parametersError = (currentToken == NULL_PTR(Token *));
	PARSER_ERROR_REPORT(errorStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

	if (ret){
		uint32 top = typecastStack.GetSize() - 1;
		StreamString* operat;
		typecastStack.Extract(top, operat);

	    // Write in the stack machine expression
	    stackMachineExpr += "CONST ";
		stackMachineExpr += operat->Buffer();
		stackMachineExpr += " ";
		stackMachineExpr += currentToken->GetData();
		stackMachineExpr += "\n";
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


CCString MathExpressionParser::GetStackMachineExpression()
{
	return stackMachineExpr.Buffer();
}

}
