/**
 * @file ParserI.cpp
 * @brief Source file for class ParserI
 * @date 09/12/2015
 * @author Giuseppe Ferrò
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
 * the class ParserI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ParserI.h"
//#include "TypeConversion.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static void PrintErrorOnStream(CCString const format,
                               const uint32 lineNumber,
                               BufferedStreamI * const err) {
    if (err != NULL) {
        if (!err->Printf(format, lineNumber)) {
            REPORT_ERROR(ErrorManagement::FatalError, "PrintErrorOnStream: Failed Printf() on parseError stream");
        }

//TODO FIX THIS
//        REPORT_ERROR(ErrorManagement::FatalError, format, lineNumber);
    }
}

static CCString GetCurrentTokenData(Token * const token) {

    return (token != NULL)?(token->GetData()):(emptyString);
}

static uint32 GetCurrentTokenLineNumber(const Token * const token) {
    return (token != NULL)?(token->GetLineNumber()):0u;
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ParserI::ParserI(StreamI &stream,StructuredDataI &databaseIn,BufferedStreamI * const err,const GrammarInfo &grammarIn) :
        tokenProducer(stream, &grammarIn.assignment, grammarIn.separators, grammarIn.beginOneLineComment,
        		grammarIn.beginMultipleLinesComment,grammarIn.endMultipleLinesComment),memory(1024*1024) {

	parseStatus.Init();

    database = &databaseIn;
    errorStream = err;
    grammar = grammarIn;
    currentToken = static_cast<Token*>(NULL);
}

ParserI::~ParserI() {
    currentToken = static_cast<Token*>(NULL);
    errorStream=static_cast<BufferedStreamI*>(NULL);
    database=static_cast<StructuredDataI*>(NULL);
}

uint32 ParserI::GetNextTokenType() {
    uint32 ret = 0u;

    currentToken = tokenProducer.GetToken();

    uint32 endTokendId = GetConstant(ParserConstant::START_SYMBOL); //StringHelper::Length(terminals)+2u;
    CCString toCompare;

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
        if (toCompare == GetSymbolName(i)) {
            ret = i;
        }
    }
    return ret;
}

uint32 ParserI::PeekNextTokenType(const uint32 position) {
    uint32 ret = 0u;

    Token* tok = tokenProducer.PeekToken(position);
    uint32 endTokendId = GetConstant(ParserConstant::START_SYMBOL);
    CCString toCompare = static_cast<const char8 *>(NULL);

    if (tok->GetId() == TERMINAL_TOKEN) {
        toCompare = tok->GetData();
    }
    else {
        toCompare = tok->GetDescription();
    }
    for (uint32 i = 0u; i < endTokendId; i++) {
        if (toCompare == GetSymbolName(i)) {
            ret = i;
        }
    }

    return ret;
}

GrammarInfo ParserI::GetGrammarInfo() const {
    return grammar;
}

void ParserI::GetTypeCast() {
	if ((currentToken != NULL_PTR(Token *)) && (parseStatus.ok)){
		parseStatus.td = TypeDescriptor(currentToken->GetData());
	}
}

void ParserI::BlockEnd() {
    if (!database->MoveToAncestor(1u)) {
        PrintErrorOnStream("\nFailed StructuredDataI::MoveToAncestor(1)! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
		parseStatus.ok.fatalError = true;
    }
}

void ParserI::CreateNode() {
    if (!database->CreateRelative(GetCurrentTokenData(currentToken))) {
        PrintErrorOnStream("\nFailed StructuredDataI::CreateRelative(*)! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
		parseStatus.ok.fatalError = true;
    }
}

void ParserI::AddLeaf() {
	Reference ref;

	if (parseStatus.ok){
		parseStatus.ok = memory.End();
		if (!parseStatus.ok){
	        PrintErrorOnStream("\n Failed ending construction of object[%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
		}
	}

	if (parseStatus.ok){
		parseStatus.ok = memory.GetReference(ref);
		if (!ref.IsValid()){
			parseStatus.ok.fatalError = true;
		}
		if (!parseStatus.ok){
	        PrintErrorOnStream("\n Failed creating object[%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
		}
	}

	if (parseStatus.ok){
		ref->SetName(parseStatus.nodeName);

		// no errors if ref is valid. which must be since we got here
        database->Write(ref);
	}

	parseStatus.Init();

}

void ParserI::GetNodeName() {
	if ((currentToken != NULL_PTR(Token *)) && (parseStatus.ok)){
		parseStatus.nodeName = currentToken->GetData().GetList();
	}
}

void ParserI::AddScalar() {
	if ((currentToken != NULL_PTR(Token *)) && (parseStatus.ok)){

		// restart parsing
		if (parseStatus.parseElStatus == ParseStatus::parseElFinished){
			memory.Start(parseStatus.td);
		}

		parseStatus.ok = memory.AddElement(currentToken->GetData());
		if (!parseStatus.ok){
	        PrintErrorOnStream("\n Failed adding element[%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
		}
	}
}

void ParserI::EndVector() {
	if (parseStatus.ok){
		parseStatus.ok = memory.EndVector();
		if (!parseStatus.ok){
	        PrintErrorOnStream("\n Failed ending vector [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
		}
	}
}

void ParserI::EndMatrix() {
}

void ParserI::End() {
    if (!database->MoveToRoot()) {
        PrintErrorOnStream("\nFailed StructuredDataI::MoveToRoot() at the end! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        parseStatus.ok.fatalError = true;
    }
}

void ParserI::ParseStatus::Init(){
	nodeName.Truncate(0);
	ok = true;
	parseElStatus = ParseStatus::parseElFinished;
	td = ConstCharString(sizeof(CCString));
}

ErrorManagement::ErrorType ParserI::Parse() {
	parseStatus.Init();

    bool isEOF = false;

    while ((parseStatus.ok) && (!isEOF)) {

        uint32 stackArray[ParserConstant::PARSE_STACK_SIZE];
        uint32 *stack = &stackArray[0];

        uint32 *top = &stackArray[ParserConstant::PARSE_STACK_SIZE - 1u];
        *top = 0u;
        uint32 start_symbol = GetConstant(ParserConstant::START_SYMBOL);

        StackPush(start_symbol, stack, top);
        uint32 token = GetNextTokenType();
        uint32 new_token = token;

        for (uint32 symbol = StackPop(top); (symbol > 0u) && (parseStatus.ok);) {
            if (symbol >= GetConstant(ParserConstant::START_ACTION)) {
                Execute(symbol - (GetConstant(ParserConstant::START_ACTION) - 1u));

            }
            else if (symbol >= GetConstant(ParserConstant::START_SYMBOL)) {
                uint32 level = 0u; // before was 1

                uint32 index = GetParseRow(symbol - (GetConstant(ParserConstant::START_SYMBOL) - 1u));
                index += token;
                uint32 entry = GetParse(index);
                while (entry >= GetConstant(ParserConstant::START_CONFLICT)) {
                    index = GetConflictRow(entry - (GetConstant(ParserConstant::START_CONFLICT) - 1u));
                    index += PeekNextTokenType(level);
                    entry = GetConflict(index);
                    ++level;
                }
                if (entry > 0u) {
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
                        (token == 0u) ? (isEOF = true) : (parseStatus.ok.invalidOperation = true);
                        if (!parseStatus.ok) {
                            PrintErrorOnStream("\nInvalid Token! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
                        }
                        new_token = GetConstant(ParserConstant::END_OF_SLK_INPUT);
                    }
                }
                else {
                    (token == 0u) ? (isEOF = true) : (parseStatus.ok.invalidOperation = true);
                    if (!parseStatus.ok) {
                        PrintErrorOnStream("\nInvalid Token! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
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
                    	parseStatus.ok.internalSetupError = true;
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
            parseStatus.ok.internalSetupError = true;
        }
    }

    return parseStatus.ok;
}

}
