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
#include "AdvancedErrorManagement.h" // DEBUG remove this

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static uint32 Production[] = {0

,3,17,18,32 ,7,18,37,1,2,20,19,38 ,2,19,3 ,2,19,4 
,3,20,22,33 ,2,21,5 ,2,21,6 ,3,22,24,34 ,2,23,7 
,2,23,8 ,3,24,26,35 ,2,25,9 ,2,25,10 ,3,26,28,36 
,2,27,11 ,2,28,30 ,5,28,39,29,30,40 ,2,29,12 ,2,30,31 
,3,30,41,13 ,7,30,14,42,1,15,31,43 ,7,30,14,42,1,15,44,13 
,4,31,14,20,15 ,3,31,41,1 ,7,31,39,1,14,20,15,40 
,3,32,18,32 ,1,32 ,6,33,39,21,22,40,33 ,1,33 ,6,34,39,23,24,40,34 
,1,34 ,6,35,39,25,26,40,35 ,1,35 ,6,36,39,27,28,40,36 
,1,36 
,0};

static uint32 Production_row[] = {0

,1,5,13,16,19,23,26,29,33,36,39,43,46,49,53,56
,59,65,68,71,75,83,91,96,100,108,112,114,121,123,130,132
,139,141,148
,0};

static uint32 ParseArray[] = {

0,0,5,1,35,35,35,35,35,35,35,35,34,5,5,5,35,33,33
,33,33,33,33,32,32,29,29,28,28,33,31,31,31,31,30,30,8,29,2
,11,3,4,31,14,6,7,16,8,8,8,11,11,11,19,14,14,14,17,16
,16,9,10,12,13,37,20,36,26,15,18,0,0,0,0,0,0,0,23,0
,0,0,0,27
};

static uint32 Parse_row[] = {0

,2,37,37,1,39,35,53,38,53,42,57,45,57,52,63,66
,22,27,14,1
,0};

static uint32 Conflict[] = {

0,0,38,0,0,0,0,0,0,0,0,0,0,19,19,19,24,24,24
,24,24,24,24,24,24,0,0,25,24,19,19,19,19,19,19,19,0,0,19
,39,21,0,19,19,19,19,19,19,19,19,19,0,22,21,19
};

static uint32 Conflict_row[] = {0

,1,13,24,39
,0};

static const uint32 Constants[] = { 17, 16, 0, 36, 40, 37, 45, 4, 512 };

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
    Action [ 5 ] = &MathExpressionParser::AddOperand;
    Action [ 6 ] = &MathExpressionParser::PushTypecast;
    Action [ 7 ] = &MathExpressionParser::PopTypecast;
    Action [ 8 ] = &MathExpressionParser::AddOperandTypecast;
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
		
	if (StringHelper::Compare(operatorIn, "*") == 0)
		ret = "MUL";
	
	if (StringHelper::Compare(operatorIn, ">") == 0)
		ret = "COMP";
		
	if (StringHelper::Compare(operatorIn, "/") == 0)
		ret = "DIV";
		
	if (StringHelper::Compare(operatorIn, "^") == 0)
		ret = "POW";
		
	if (StringHelper::Compare(operatorIn, "-") == 0)
		ret = "MIN";
	
	if (StringHelper::Compare(operatorIn, "sin") == 0)
		ret = "SIN";
	
	if (StringHelper::Compare(operatorIn, "cos") == 0)
		ret = "COS";
	
	if (StringHelper::Compare(operatorIn, "tan") == 0)
		ret = "TAN";
	
	if (StringHelper::Compare(operatorIn, "log") == 0)
		ret = "LOG";
		
	if (StringHelper::Compare(operatorIn, "!") == 0)
		ret = "FACT";
		
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
