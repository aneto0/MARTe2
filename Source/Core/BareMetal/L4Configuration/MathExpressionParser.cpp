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

#include <iostream>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MathExpressionParser.h"
#include "TypeConversion.h"
#include "AdvancedErrorManagement.h"

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

static void PrintErrorOnStream(const char8 * const format,
                               const uint32 lineNumber,
                               BufferedStreamI * const err) {
    if (err != NULL) {
        if (!err->Printf(format, lineNumber)) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "PrintErrorOnStream: Failed Printf() on parseError stream");
        }
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, format, lineNumber);
    }
}

static const char8* GetCurrentTokenData(Token * const token) {

    return (token != NULL)?(token->GetData()):(static_cast<const char8*>(NULL));
}

//static uint32 GetCurrentTokenId(const Token * const token) {
    //return (token != NULL)?(token->GetId()):(ERROR_TOKEN);
//}

static uint32 GetCurrentTokenLineNumber(const Token * const token) {
    return (token != NULL)?(token->GetLineNumber()):0u;
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

//const MARTe::char8* allOperators = "+-*/^=";

namespace MARTe {

MathExpressionParser::MathExpressionParser(StreamI &stream,
                                           StructuredDataI &databaseIn,
                                           BufferedStreamI * const err) :
    tokenProducer(stream, &(MathGrammar.assignment), MathGrammar.separators, MathGrammar.beginOneLineComment, MathGrammar.beginMultipleLinesComment, MathGrammar.endMultipleLinesComment),
    memory(1u) {
        numberOfColumns = 0u;
        firstNumberOfColumns = 0u;
        numberOfRows = 0u;
        database = &databaseIn;
        errorStream = err;
        tokenType = 0u;
        numberOfDimensions = 0u;
        grammar = MathGrammar;
        currentToken = static_cast<Token*>(NULL);
        isError = false;
		
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
}

MathExpressionParser::~MathExpressionParser() {
// Auto-generated destructor stub for MathExpressionParser
// TODO Verify if manual additions are needed
}

uint32 MathExpressionParser::GetNextTokenType()
{
	uint32 ret = 0u;

	currentToken = tokenProducer.GetToken();

	uint32 endTokendId = GetConstant(ParserConstant::START_SYMBOL); //StringHelper::Length(terminals)+2u;
	const char8* toCompare = static_cast<const char8 *>(NULL);

	// if it is a terminal use the data
	if (currentToken->GetId() == TERMINAL_TOKEN) {
		toCompare = currentToken->GetData();
	}
	// otherwise use the description
	else {
		toCompare = currentToken->GetDescription();
	}
	// return the slk token number
	for (uint32 i = 0u; i < endTokendId; i++) {
		if (StringHelper::Compare(toCompare, GetSymbolName(i)) == 0) {
			ret = i;
		}
	}
	return ret;
}

uint32 MathExpressionParser::PeekNextTokenType(const uint32 position)
{
	uint32 ret = 0u;

	Token* tok = tokenProducer.PeekToken(position);
	uint32 endTokendId = GetConstant(ParserConstant::START_SYMBOL);
	const char8* toCompare = static_cast<const char8 *>(NULL);

	if (tok->GetId() == TERMINAL_TOKEN) {
		toCompare = tok->GetData();
	}
	else {
		toCompare = tok->GetDescription();
	}
	for (uint32 i = 0u; i < endTokendId; i++) {
		if (StringHelper::Compare(toCompare, GetSymbolName(i)) == 0) {
			ret = i;
		}
	}

	return ret;
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
	StackMachineExpr += "WRITE ";
    StackMachineExpr += assignmentVarName.Buffer();
    StackMachineExpr += "\n";
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
	StackMachineExpr += OperatorLookupTable(operat->Buffer());
	StackMachineExpr += "\n";
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
	StackMachineExpr += "CAST ";
	StackMachineExpr += operat->Buffer();
	StackMachineExpr += "\n";
}

void MathExpressionParser::AddOperand()
{
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Add Operand  %s", currentToken->GetData());

    // Write in the stack machine expression
    if (StringHelper::Compare(currentToken->GetDescription(), "STRING") == 0)
		StackMachineExpr += "READ ";
		
	else if (StringHelper::Compare(currentToken->GetDescription(), "NUMBER") == 0)
		StackMachineExpr += "CONST ";
	
	StackMachineExpr += currentToken->GetData();
	StackMachineExpr += "\n";
}

void MathExpressionParser::AddOperandTypecast()
{
	uint32 top = typecastStack.GetSize() - 1;
	StreamString* operat;
	typecastStack.Extract(top, operat);
	
    // Write in the stack machine expression
    StackMachineExpr += "CONST<";
	StackMachineExpr += operat->Buffer();
	StackMachineExpr += ">";
	StackMachineExpr += currentToken->GetData();
	StackMachineExpr += "\n";
}

void MathExpressionParser::StoreAssignment()
{
	REPORT_ERROR_STATIC(ErrorManagement::Debug, "StoreAssignment");
	
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
	return StackMachineExpr;
}

bool MathExpressionParser::Parse() {
typeName = defaultTypeName;

for (uint32 i = 0; ((tokenProducer.PeekToken(i))->GetId()) != 0; i++)
	{
		REPORT_ERROR_STATIC(ErrorManagement::Debug, "op: %-10s %-5s %-2u", (tokenProducer.PeekToken(i))->GetDescription(), (tokenProducer.PeekToken(i))->GetData(), (tokenProducer.PeekToken(i))->GetId());
	}

bool isEOF = false;

while ((!isError) && (!isEOF)) {
    
    uint32 stackArray[ParserConstant::PARSE_STACK_SIZE];
    uint32 *stack = &stackArray[0];

    uint32 *top = &stackArray[ParserConstant::PARSE_STACK_SIZE - 1u];
    *top = 0u;
    uint32 start_symbol = GetConstant(ParserConstant::START_SYMBOL);

    StackPush(start_symbol, stack, top);
    uint32 token = GetNextTokenType();
    uint32 new_token = token;

    for (uint32 symbol = StackPop(top); (symbol > 0u) && (!isError);)
    {
		// It's an action
        if (symbol >= GetConstant(ParserConstant::START_ACTION))
        {
            Execute(symbol - (GetConstant(ParserConstant::START_ACTION) - 1u));

        }
        
        // It's a symbol
        else if (symbol >= GetConstant(ParserConstant::START_SYMBOL))
        {
            uint32 level = 0u; // before was 1
            
            // Look for it among those defined in the grammar
            uint32 index = GetParseRow(symbol - (GetConstant(ParserConstant::START_SYMBOL) - 1u));
            index += token;
            uint32 entry = GetParse(index);
            while (entry >= GetConstant(ParserConstant::START_CONFLICT))
            {
                index = GetConflictRow(entry - (GetConstant(ParserConstant::START_CONFLICT) - 1u));
                index += PeekNextTokenType(level);
                entry = GetConflict(index);
                ++level;
            }
            
            if (entry > 0u)
            {
                uint32 *production = &GetProduction(GetProductionRow(entry));
                uint32 production_length = *production - 1u;
                production = &production[1];
                /*lint -e{415} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                if (*production == symbol) {
                    /*lint -e{661} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                    for (; production_length > 0u; production_length--) {
                        /*lint -e{662} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                        uint32 toPush = production[production_length];
                        StackPush(toPush, stack, top);
                    }
                }
                else {
                    (token == 0u) ? (isEOF = true) : (isError = true);
                    if (isError) {
                        PrintErrorOnStream("\nInvalid Token! [%d], %s", GetCurrentTokenLineNumber(currentToken), errorStream);
                        std::cout << GetCurrentTokenData(currentToken) << " err1 (syntax error: valid token but invalid positioning (?)) \n";
					}
                    new_token = GetConstant(ParserConstant::END_OF_SLK_INPUT);
                }
            }
            
            // This particular token was not found among those defined in the grammar, so it's invalid
            else
            {
                (token == 0u) ? (isEOF = true) : (isError = true);
                if (isError) {
                    PrintErrorOnStream("\nInvalid Token! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
                    std::cout << GetCurrentTokenData(currentToken) << " " << (tokenProducer.PeekToken(0))->GetData() << " err2 (token was not found among those defined in the grammar(?))\n";
                }
                new_token = GetConstant(ParserConstant::END_OF_SLK_INPUT);
            }
        }
        else {
            if (symbol > 0u) {
                if (symbol == token) {
                    token = GetNextTokenType();
                    new_token = token;
                }
                else {
                    isError = true;
                    PrintErrorOnStream("\nInvalid Expression! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
                    new_token = GetConstant(ParserConstant::END_OF_SLK_INPUT);
                }
            }
        }
        if (token != new_token) {
            if (new_token > 0u) {
                token = new_token;
            }
            if (token != GetConstant(ParserConstant::END_OF_SLK_INPUT)) {
                continue;
            }
        }
        symbol = StackPop(top);
    }
    if (token != GetConstant(ParserConstant::END_OF_SLK_INPUT)) {
        PrintErrorOnStream("\nEOF found with tokens on internal parser stack! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
}

REPORT_ERROR_STATIC(ErrorManagement::Debug, "PARSED!");

return !isError;
}

}
