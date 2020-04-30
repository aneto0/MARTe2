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

,3,24,25,40 ,7,25,46,1,2,27,26,47 ,2,26,3 ,2,26,4 
,3,27,29,41 ,2,28,5 ,2,28,6 ,2,28,7 ,2,28,8 
,2,28,9 ,3,29,31,42 ,2,30,10 ,2,30,11 ,2,30,12 
,2,30,13 ,2,30,14 ,3,31,33,43 ,2,32,15 ,2,32,16 
,3,33,35,44 ,2,34,17 ,2,34,18 ,2,35,38 ,5,35,48,37,38,50 
,5,35,48,36,38,49 ,2,36,19 ,2,37,15 ,2,37,16 ,2,38,39 
,7,38,20,51,1,21,39,52 ,3,38,53,22 ,7,38,20,51,1,21,54,22 
,4,39,20,27,21 ,3,39,53,1 ,8,39,48,1,20,27,45,21,49 
,3,40,25,40 ,1,40 ,6,41,48,28,29,49,41 ,1,41 ,6,42,48,30,31,49,42 
,1,42 ,6,43,48,32,33,49,43 ,1,43 ,6,44,48,34,35,49,44 
,1,44 ,4,45,4,27,45 ,1,45 
,0};

static uint32 Production_row[] = {0

,1,5,13,16,19,23,26,29,32,35,38,42,45,48,51,54
,57,61,64,67,71,74,77,80,86,92,95,98,101,104,112,116
,124,129,133,142,146,148,155,157,164,166,173,175,182,184,189
,0};

static uint32 ParseArray[] = {

0,0,1,2,45,45,45,45,45,45,45,45,45,45,45,45,45,45,44
,44,3,4,45,43,43,43,43,43,43,43,43,43,43,43,43,42,42,18,19
,21,22,43,41,41,41,41,41,41,41,40,40,40,40,40,5,6,7,8,9
,10,41,39,39,38,38,38,38,38,5,5,11,49,5,5,26,5,27,28,17
,39,0,36,0,0,11,11,20,0,11,11,33,11,17,17,23,0,17,17,0
,17,20,20,29,37,20,20,0,20,24,24,46,0,25,23,0,23,12,13,14
,15,16,48,0,31,0,0,0,47,0,0
};

static uint32 Parse_row[] = {0

,1,2,17,53,50,69,106,77,22,85,22,93,55,61,101,70
,80,58,39,20,1,106
,0};

static uint32 Conflict[] = {

0,0,50,0,0,0,0,0,0,0,0,0,0,0,0,0,29,29,0
,0,29,29,0,29,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34
,34,0,35,34,29,29,29,29,29,29,29,29,29,29,29,29,29,29,0,29
,51,30,0,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,0
,30,29,32
};

static uint32 Conflict_row[] = {0

,1,21,38,59
,0};

static const uint32 Constants[] = { 24, 23, 0, 48, 52, 46, 55, 4, 512 };

static const char8 * Terminal_name[] ={"0"

,"STRING"
,"="
,";"
,","
,"&"
,"|"
,"||"
,"&&"
,"^"
,">"
,"<"
,">="
,"<="
,"=="
,"+"
,"-"
,"*"
,"/"
,"!"
,"("
,")"
,"NUMBER"
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
    Action [ 6 ] = &MathExpressionParser::PushTypecast;
    Action [ 7 ] = &MathExpressionParser::PopTypecast;
    Action [ 8 ] = &MathExpressionParser::AddOperand;
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
		
	else if (StringHelper::Compare(operatorIn, "/") == 0)
		ret = "DIV";
		
	else if (StringHelper::Compare(operatorIn, "-") == 0)
		ret = "MIN";
	
	else if (StringHelper::Compare(operatorIn, "!") == 0)
		ret = "FACT";
		
	else if (StringHelper::Compare(operatorIn, "&") == 0)
		ret = "BAND";
		
	else if (StringHelper::Compare(operatorIn, "|") == 0)
		ret = "BOR";
	
	else if (StringHelper::Compare(operatorIn, "&&") == 0)
		ret = "AND";
		
	else if (StringHelper::Compare(operatorIn, "||") == 0)
		ret = "OR";
		
    else if (StringHelper::Compare(operatorIn, "^") == 0)
		ret = "XOR";
		
	else if (StringHelper::Compare(operatorIn, "<") == 0)
		ret = "LT";
		
	else if (StringHelper::Compare(operatorIn, ">") == 0)
		ret = "GT";	
		
	else if (StringHelper::Compare(operatorIn, "<=") == 0)
		ret = "LE";
		
	else if (StringHelper::Compare(operatorIn, ">=") == 0)
		ret = "GE";	
		
	else if (StringHelper::Compare(operatorIn, "==") == 0)
		ret = "EQ";	
		
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
