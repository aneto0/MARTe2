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

,3,19,20,37 ,7,20,44,1,2,22,21,45 ,2,21,3 ,2,21,4 
,3,22,24,38 ,2,23,5 ,2,23,6 ,3,24,26,39 ,2,25,7 
,2,25,8 ,3,26,28,40 ,2,27,9 ,2,27,10 ,3,28,30,41 
,2,29,11 ,2,29,12 ,3,30,32,42 ,2,31,13 ,2,32,35 
,5,32,46,34,35,48 ,5,32,46,33,35,47 ,2,33,14 ,2,34,9 
,2,34,10 ,2,35,36 ,7,35,15,49,1,16,36,50 ,3,35,51,17 
,7,35,15,49,1,16,52,17 ,4,36,15,22,16 ,3,36,51,1 
,8,36,46,1,15,22,43,16,47 ,3,37,20,37 ,1,37 ,6,38,46,23,24,47,38 
,1,38 ,6,39,46,25,26,47,39 ,1,39 ,6,40,46,27,28,47,40 
,1,40 ,6,41,46,29,30,47,41 ,1,41 ,6,42,46,31,32,47,42 
,1,42 ,4,43,4,22,43 ,1,43 
,0};

static uint32 Production_row[] = {0

,1,5,13,16,19,23,26,29,33,36,39,43,46,49,53,56
,59,63,66,69,75,81,84,87,90,93,101,105,113,118,122,131
,135,137,144,146,153,155,162,164,171,173,180,182,187
,0};

static uint32 ParseArray[] = {

0,0,1,2,43,43,43,43,43,43,43,43,43,43,42,3,4,43,41
,41,41,41,41,41,41,41,40,40,6,7,18,41,39,39,39,39,39,39,38
,38,9,10,12,13,5,39,37,37,37,37,36,36,5,5,8,25,22,5,5
,37,5,0,8,8,11,15,16,8,8,46,8,27,11,11,14,23,24,11,11
,32,11,47,14,14,17,0,0,14,14,44,14,0,17,17,19,29,33,17,17
,0,17,45,20,20,0,0,0,21,19,0,19,35,35,34,34,0,0,0,0
,0,0,0,0,0,35,0,0
};

static uint32 Parse_row[] = {0

,1,2,12,43,23,53,33,63,33,73,54,83,17,93,42,66
,54,80,78,108,43,29,15,1,85
,0};

static uint32 Conflict[] = {

0,0,48,0,0,0,0,0,0,0,25,25,0,0,0,25,25,0,25
,30,30,30,30,30,30,30,30,30,30,30,0,31,30,25,25,25,25,25,25
,25,25,25,0,25,49,26,0,25,25,25,25,25,25,25,25,25,25,25,0
,26,25,28
};

static uint32 Conflict_row[] = {0

,1,16,28,44
,0};

static const uint32 Constants[] = { 19, 18, 0, 46, 50, 44, 53, 4, 512 };

static const char8 * Terminal_name[] ={"0"

,"STRING"
,"="
,";"
,","
,"&"
,"|"
,">"
,"<"
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
		ret = "AND";
		
	else if (StringHelper::Compare(operatorIn, "|") == 0)
		ret = "OR";
		
	else if (StringHelper::Compare(operatorIn, "<") == 0)
		ret = "LT";
		
	else if (StringHelper::Compare(operatorIn, ">") == 0)
		ret = "GT";	
		
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
