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

,6,14,32,1,2,15,33 ,3,15,17,28 ,2,16,3 ,2,16,4 
,3,17,19,29 ,2,18,5 ,2,18,6 ,3,19,21,30 ,2,20,7 
,2,20,8 ,3,21,23,31 ,2,22,9 ,2,23,24 ,5,23,34,1,24,35 
,3,24,10,25 ,2,24,27 ,5,25,36,1,11,26 ,3,25,15,11 
,5,26,10,15,11,37 ,4,26,38,1,37 ,3,26,39,12 ,3,27,38,1 
,3,27,38,12 ,6,28,34,16,17,35,28 ,1,28 ,6,29,34,18,19,35,29 
,1,29 ,6,30,34,20,21,35,30 ,1,30 ,6,31,34,22,23,35,31 
,1,31 
,0};

static uint32 Production_row[] = {0

,1,8,12,15,18,22,25,28,32,35,38,42,45,48,54,58
,61,67,71,77,82,86,90,94,101,103,110,112,119,121,128
,0};

static uint32 ParseArray[] = {

0,0,2,1,31,31,31,31,31,31,30,2,31,2,31,29,29,29,29
,28,28,3,4,29,5,29,27,27,26,26,24,24,8,5,27,5,27,11,25
,12,25,8,32,8,22,16,11,0,11,33,20,13,0,13,15,23,16,0,18
,19,18,21,6,7,9,10,0,0,0,0,0
};

static uint32 Parse_row[] = {0

,2,1,18,23,57,31,57,36,30,41,44,48,49,43,27,23
,12,1
,0};

static uint32 Conflict[] = {

0,0,14,0,13,13,13,13,13,13,13,14,13,14,18,0,18,18,18
,18,18,18,18,18,34,18,17,0,18,18,18,18,18,18,18,17,18,17
};

static uint32 Conflict_row[] = {0

,1,13,25
,0};

static const uint32 Constants[] = { 14, 13, 0, 32, 35, 32, 40, 3 };

static const char8 * Terminal_name[] ={"0"

,"STRING"
,"="
,">"
,"<"
,"+"
,"-"
,"*"
,"/"
,"^"
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
    Action [ 2 ] = &MathExpressionParser::End;
    Action [ 3 ] = &MathExpressionParser::PushOperator;
    Action [ 4 ] = &MathExpressionParser::PopOperator;
    Action [ 5 ] = &MathExpressionParser::PushTypecast;
    Action [ 6 ] = &MathExpressionParser::PopTypecast;
    Action [ 7 ] = &MathExpressionParser::AddOperand;
    Action [ 8 ] = &MathExpressionParser::AddOperandTypecast;
    
    //for (uint32 i = 0; ((tokenProducer.PeekToken(i))->GetId()) != 0; i++)
	//{
		//REPORT_ERROR_STATIC(ErrorManagement::Debug, "op: %-10s %-5s %-2u", (tokenProducer.PeekToken(i))->GetDescription(), (tokenProducer.PeekToken(i))->GetData(), (tokenProducer.PeekToken(i))->GetId());
	//}
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
		
	if (StringHelper::Compare(operatorIn, "(") == 0)
		ret = "CAST";
		
	return ret;
	
} // OperatorLookupTable()

void MathExpressionParser::End()
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
    stackMachineExpr += "CONST<";
	stackMachineExpr += operat->Buffer();
	stackMachineExpr += ">";
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
