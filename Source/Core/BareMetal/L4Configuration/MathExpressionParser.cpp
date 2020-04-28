/**
 * @file MathExpressionParser.cpp
 * @brief Source file for class MathExpressionParser
 * @date Apr 7, 2020
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
#include "AdvancedErrorManagement.h" // DEBUG remove this

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static uint32 Production[] = {0

,3,17,18,32 ,7,18,37,1,2,20,19,38 ,2,19,3 ,2,19,4 
,3,20,22,33 ,2,21,5 ,2,21,6 ,3,22,24,34 ,2,23,7 
,2,23,8 ,3,24,26,35 ,2,25,9 ,2,25,10 ,3,26,28,36 
,2,27,11 ,2,28,30 ,5,28,39,8,30,41 ,5,28,39,7,30,41 
,5,28,39,29,30,40 ,2,29,12 ,2,30,31 ,3,30,42,13 
,7,30,14,43,1,15,31,44 ,7,30,14,43,1,15,45,13 
,4,31,14,20,15 ,3,31,42,1 ,7,31,39,1,14,20,15,40 
,3,32,18,32 ,1,32 ,6,33,39,21,22,40,33 ,1,33 ,6,34,39,23,24,40,34 
,1,34 ,6,35,39,25,26,40,35 ,1,35 ,6,36,39,27,28,40,36 
,1,36 
,0};

static uint32 Production_row[] = {0

,1,5,13,16,19,23,26,29,33,36,39,43,46,49,53,56
,59,65,71,77,80,83,87,95,103,108,112,120,124,126,133,135
,142,144,151,153,160
,0};

static uint32 ParseArray[] = {

0,0,21,1,37,37,37,37,37,37,37,37,36,2,22,38,37,35,35
,35,35,35,35,34,34,3,4,6,7,35,33,33,33,33,32,32,15,5,9
,10,8,20,33,5,5,39,8,8,5,5,5,8,8,8,11,12,13,14,25
,0,11,11,0,14,14,11,11,11,14,14,14,16,28,31,31,30,30,18,17
,0,0,0,19,16,16,31,0,29
};

static uint32 Parse_row[] = {0

,2,12,22,36,22,39,31,53,46,56,25,70,29,1,44,71
,70,27,14,1
,0};

static uint32 Conflict[] = {

0,0,40,0,0,0,0,0,21,21,0,0,0,21,21,21,26,26,26
,26,26,26,26,26,26,0,0,27,26,21,21,21,21,21,21,21,0,0,21
,41,23,0,21,21,21,21,21,21,21,21,21,0,24,23,21
};

static uint32 Conflict_row[] = {0

,1,13,24,39
,0};

static const uint32 Constants[] = { 17, 16, 0, 38, 42, 37, 46, 4, 512 };

static const char8 * Terminal_name[] ={"0"

,"STRING"
,"="
,";"
,","
,">"
,"<"
,"+"
,"-"
,"*"
,"/"
,"^"
,"!"
,"NUMBER"
,"("
,")"
,"END_OF_SLK_INPUT"
};

static const char8 *GetTerminalName(const uint32 symbol) {
    return Terminal_name[symbol];
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MathExpressionParser::MathExpressionParser(StreamI &stream,
                                           StructuredDataI &databaseIn,
                                           BufferedStreamI * const err) :
    ParserI(stream, databaseIn, err, MathGrammar) {
		
		//tokenProducer.TokenizeInput();
        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Acquired terminals:: %s", &(MathGrammar.assignment));
        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Acquired string: %s", (dynamic_cast<StreamString&>(stream)).Buffer());
        
        Action[0] = static_cast<void (MathExpressionParser::*)(void)>(NULL);
    Action [ 1 ] = &MathExpressionParser::StoreAssignment;
    Action [ 2 ] = &MathExpressionParser::PopAssignment;
    Action [ 3 ] = &MathExpressionParser::PushOperator;
    Action [ 4 ] = &MathExpressionParser::PopOperator;
    Action [ 5 ] = &MathExpressionParser::PopOperatorAlternate;
    Action [ 6 ] = &MathExpressionParser::AddOperand;
    Action [ 7 ] = &MathExpressionParser::PushTypecast;
    Action [ 8 ] = &MathExpressionParser::PopTypecast;
    Action [ 9 ] = &MathExpressionParser::AddOperandTypecast;
}

MathExpressionParser::~MathExpressionParser() {
// Auto-generated destructor stub for MathExpressionParser
// TODO Verify if manual additions are needed
}

const char8* MathExpressionParser::OperatorLookupTable(const char8* operatorIn)
{
	const char8* ret = operatorIn;
	
	if (StringHelper::Compare(operatorIn, "+") == 0)
		ret = "SUM";
		
	else if (StringHelper::Compare(operatorIn, "*") == 0)
		ret = "MUL";
	
	else if (StringHelper::Compare(operatorIn, ">") == 0)
		ret = "COMP";
		
	else if (StringHelper::Compare(operatorIn, "/") == 0)
		ret = "DIV";
		
	else if (StringHelper::Compare(operatorIn, "^") == 0)
		ret = "POW";
		
	else if (StringHelper::Compare(operatorIn, "-") == 0)
		ret = "MIN";
	
	else if (StringHelper::Compare(operatorIn, "!") == 0)
		ret = "FACT";
	
	else
	{
		char8* uppercaseOp = StringHelper::StringDup(operatorIn);
		StringHelper::ToUpper(uppercaseOp);
		
		ret = uppercaseOp;
	}
		
	return ret;
	
} // OperatorLookupTable()

void MathExpressionParser::PopAssignment()
{
	REPORT_ERROR_STATIC(ErrorManagement::Debug, "WRITE %s", assignmentVarName.Buffer());
	REPORT_ERROR_STATIC(ErrorManagement::Information, "END");
	
	// Write in the stack machine expression
	stackMachineExpr += "WRITE ";
    stackMachineExpr += assignmentVarName.Buffer();
    stackMachineExpr += "\n";
}

void MathExpressionParser::PushOperator()
{
	StreamString* operat = new StreamString(currentToken->GetData());
	
	operatorStack.Add(operat);
}

void MathExpressionParser::PopOperator()
{
	uint32 top = operatorStack.GetSize() - 1;
	StreamString* operat;
	operatorStack.Extract(top, operat);
	
	REPORT_ERROR_STATIC(ErrorManagement::Debug, "Add Operator %s", operat->Buffer());
	
	// Write in the stack machine expression
	stackMachineExpr += OperatorLookupTable(operat->Buffer());
	stackMachineExpr += "\n";
}

void MathExpressionParser::PopOperatorAlternate()
{
	uint32 top = operatorStack.GetSize() - 1;
	StreamString* operat;
	operatorStack.Extract(top, operat);

	REPORT_ERROR_STATIC(ErrorManagement::Debug, "Add Operator %s", operat->Buffer());
	
	if (StringHelper::Compare(operat->Buffer(), "+") == 0)
	{
		// nothing
	}
	else if (StringHelper::Compare(operat->Buffer(), "-") == 0)
	{
		stackMachineExpr += "NEG";
		stackMachineExpr += "\n";
	}
	
}

void MathExpressionParser::PushTypecast()
{
	StreamString* operat = new StreamString(currentToken->GetData());
	
	typecastStack.Add(operat);
}

void MathExpressionParser::PopTypecast()
{
	uint32 top = typecastStack.GetSize() - 1;
	StreamString* operat;
	
	typecastStack.Extract(top, operat);
	
	// Write in the stack machine expression
	stackMachineExpr += "CAST ";
	stackMachineExpr += operat->Buffer();
	stackMachineExpr += "\n";
}

void MathExpressionParser::AddOperand()
{
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Add Operand  %s", currentToken->GetData());

    // Write in the stack machine expression
    if (StringHelper::Compare(currentToken->GetDescription(), "STRING") == 0)
		stackMachineExpr += "READ ";
		
	else if (StringHelper::Compare(currentToken->GetDescription(), "NUMBER") == 0)
		stackMachineExpr += "CONST ";
	
	stackMachineExpr += currentToken->GetData();
	stackMachineExpr += "\n";
}

void MathExpressionParser::AddOperandTypecast()
{
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

void MathExpressionParser::StoreAssignment()
{
	REPORT_ERROR_STATIC(ErrorManagement::Debug, "StoreAssignment %s", currentToken->GetData());
	
	assignmentVarName = currentToken->GetData();
}

void MathExpressionParser::Execute(const uint32 number) {
	(this->*Action[number])();
}

const char8 *MathExpressionParser::GetSymbolName(const uint32 symbol) const {
	const char8 *symbolName = static_cast<const char8 *>(NULL);

	if((symbol > 0u) && (symbol < Constants[ParserConstant::START_SYMBOL])) {
		symbolName=GetTerminalName(symbol);
	}
	else {
		symbolName="not a symbol";
	}
	return symbolName;
}

uint32 &MathExpressionParser::GetProduction(const uint32 index) const {
	return Production[index];
}

uint32 MathExpressionParser::GetProductionRow(const uint32 index) const {
	return Production_row[index];
}

uint32 MathExpressionParser::GetParse(const uint32 index) const {
	return ParseArray[index];
}

uint32 MathExpressionParser::GetParseRow(const uint32 index) const {
	return Parse_row[index];
}

uint32 MathExpressionParser::GetConflict(const uint32 index) const {
	return Conflict[index];
}

uint32 MathExpressionParser::GetConflictRow(const uint32 index) const {
	return Conflict_row[index];
}

uint32 MathExpressionParser::GetConstant(const uint32 index) const {
	return Constants[index];
}

StreamString MathExpressionParser::GetStackMachineExpression()
{
	return stackMachineExpr;
}

}
