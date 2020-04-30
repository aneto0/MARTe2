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

,3,24,25,42 ,7,25,49,1,2,27,26,50 ,2,26,3 ,2,26,4 
,3,27,29,43 ,2,28,5 ,2,28,6 ,2,28,7 ,2,28,8 
,3,29,31,44 ,2,30,9 ,2,30,10 ,2,30,11 ,2,30,12 
,2,30,13 ,3,31,33,45 ,2,32,14 ,2,32,15 ,3,33,35,46 
,2,34,16 ,2,34,17 ,3,35,37,47 ,2,36,18 ,2,37,40 
,5,37,51,39,40,53 ,5,37,51,38,40,52 ,2,38,19 ,2,39,14 
,2,39,15 ,2,40,41 ,7,40,20,54,1,21,41,55 ,3,40,56,22 
,7,40,20,54,1,21,57,22 ,4,41,20,27,21 ,3,41,56,1 
,8,41,51,1,20,27,48,21,52 ,3,42,25,42 ,1,42 ,6,43,51,28,29,52,43 
,1,43 ,6,44,51,30,31,52,44 ,1,44 ,6,45,51,32,33,52,45 
,1,45 ,6,46,51,34,35,52,46 ,1,46 ,6,47,51,36,37,52,47 
,1,47 ,4,48,4,27,48 ,1,48 
,0};

static uint32 Production_row[] = {0

,1,5,13,16,19,23,26,29,32,35,39,42,45,48,51,54
,58,61,64,68,71,74,78,81,84,90,96,99,102,105,108,116
,120,128,133,137,146,150,152,159,161,168,170,177,179,186,188,195
,197,202
,0};

static uint32 ParseArray[] = {

0,0,1,2,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48
,47,3,4,48,46,46,46,46,46,46,46,46,46,46,46,46,46,45,45,17
,18,23,46,44,44,44,44,44,44,44,44,44,44,44,43,43,11,12,13,14
,15,44,42,42,42,42,42,42,41,41,41,41,41,40,40,39,39,39,39,5
,42,6,7,8,9,20,21,49,10,28,29,40,5,5,27,30,52,5,5,16
,5,10,10,37,50,0,10,10,19,10,0,0,16,16,51,34,32,16,16,22
,16,19,19,0,0,38,19,19,24,19,0,0,22,22,0,0,0,22,22,0
,22,25,25,0,0,0,26,24,0,24,0
};

static uint32 Parse_row[] = {0

,1,2,17,77,75,86,46,97,24,106,68,117,22,126,74,74
,93,94,101,69,58,39,20,1,82
,0};

static uint32 Conflict[] = {

0,0,53,0,0,0,0,0,0,0,0,0,0,0,0,30,30,0,0
,0,30,30,0,30,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35
,35,0,36,35,30,30,30,30,30,30,30,30,30,30,30,30,30,30,0,30
,54,31,0,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,0
,31,30,33
};

static uint32 Conflict_row[] = {0

,1,21,38,59
,0};

static const uint32 Constants[] = { 24, 23, 0, 51, 55, 49, 58, 4, 512 };

static const char8 * Terminal_name[] ={"0"

,"STRING"
,"="
,";"
,","
,"&"
,"|"
,"||"
,"&&"
,">"
,"<"
,">="
,"<="
,"=="
,"+"
,"-"
,"*"
,"/"
,"^"
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
		
	else if (StringHelper::Compare(operatorIn, "^") == 0)
		ret = "POW";
		
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
