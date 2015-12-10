/**
 * @file JsonParser.cpp
 * @brief Source file for class JsonParser
 * @date 10/dic/2015
 * @author pc
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
 * the class JsonParser (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "JsonParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static uint32 Production[] = { 0

, 3, 9, 10, 22, 3, 10, 11, 18, 6, 11, 23, 1, 2, 12, 24, 5, 11, 25, 1, 2, 16, 2, 12, 13, 2, 12, 14, 2, 12, 15, 3, 13, 26, 17, 5, 14, 3, 13, 19, 4, 5, 15, 3, 14,
        20, 4, 5, 16, 5, 11, 21, 6, 2, 17, 1, 2, 17, 7, 3, 18, 11, 18, 1, 18, 3, 19, 13, 19, 2, 19, 27, 3, 20, 14, 20, 2, 20, 28, 3, 21, 11, 21, 2, 21, 29, 0 };

static uint32 Production_row[] = { 0

, 1, 5, 9, 16, 22, 25, 28, 31, 35, 41, 47, 53, 56, 59, 63, 65, 69, 72, 76, 79, 83, 0 };

static uint32 ParseArray[] = {

0, 0, 5, 16, 23, 8, 17, 12, 5, 16, 14, 8, 1, 13, 18, 19, 20, 15, 2, 22, 9, 21, 10, 11, 0, 0, 0, 0 };

static uint32 Parse_row[] = { 0

, 11, 17, 18, 1, 4, 17, 19, 18, 6, 9, 2, 11, 15, 0 };

static uint32 Conflict[] = {

0, 0, 6, 24, 7, 3, 0, 3, 6, 4, 0, 3 };

static uint32 Conflict_row[] = { 0

, 1, 1, 4, 0 };
/*
 #define START_SYMBOL 9
 #define END_OF_SLK_INPUT_ 8
 #define START_STATE 0
 #define START_CONFLICT 22
 #define END_CONFLICT 25
 #define START_ACTION 22
 #define END_ACTION 30
 #define TOTAL_CONFLICTS 3
 */

static const uint32 Constants[] = { 9u, 8u, 0u, 22u, 25u, 22u, 30u, 3u };

static const char8 * Nonterminal_name[] = { "0"

, "expression", "cdbFile", "expressions", "variables", "scalar", "vector", "matrix", "block", "token", "expressions_*", "scalar_*", "vector_*",
        "expressions_2_*" };

static const char8 * Terminal_name[] = { "0"

, "STRING", ":", "[", "]", "{", "}", "NUMBER", "END_OF_SLK_INPUT" };

static const char8 * Action_name[] = { "0"

, "__End", "__GetNodeName", "__AddLeaf", "__CreateNode", "__AddScalar", "__EndVector", "__EndMatrix", "__BlockEnd" };



static const char8 *GetActionName(const uint32 symbol) {
    return Action_name[symbol - (Constants[ParserConstant::START_ACTION] - 1u)];
}

static const char8 *GetNonTerminalName(const uint32 symbol) {
    return Nonterminal_name[symbol - (Constants[ParserConstant::START_SYMBOL] - 1u)];
}

static const char8 *GetTerminalName(const uint32 symbol) {
    return Terminal_name[symbol];
}

JsonParser::JsonParser(StreamI &stream,
                       ConfigurationDatabase &databaseIn,
                       BufferedStreamI * const err) :
        ParserI(stream, databaseIn, err, JsonGrammar) {
    Action[0] = 0;
    Action[1] = &JsonParser::End;
    Action[2] = &JsonParser::GetNodeName;
    Action[3] = &JsonParser::AddLeaf;
    Action[4] = &JsonParser::CreateNode;
    Action[5] = &JsonParser::AddScalar;
    Action[6] = &JsonParser::EndVector;
    Action[7] = &JsonParser::EndMatrix;
    Action[8] = &JsonParser::BlockEnd;
}

JsonParser::~JsonParser() {

}

void JsonParser::Execute(uint32 number) {
    (this->*Action[number])();
}

const char8 *JsonParser::GetSymbolName(const uint32 symbol)const  {
    const char8 *symbolName = static_cast<const char8 *>(NULL);
    if((symbol >= Constants[ParserConstant::START_ACTION]) && (symbol < Constants[ParserConstant::END_ACTION])) {
        symbolName=GetActionName(symbol);
    }
    else if(symbol >= Constants[ParserConstant::START_SYMBOL]) {
        symbolName=GetNonTerminalName(symbol);
    }
    else if(symbol > 0u) {
        symbolName=GetTerminalName(symbol);
    }
    else {
        symbolName="not a symbol";
    }
    return symbolName;
}

uint32 &JsonParser::GetProduction(const uint32 index)const  {
    return Production[index];
}

uint32 JsonParser::GetProductionRow(const uint32 index)const  {
    return Production_row[index];
}

uint32 JsonParser::GetParse(const uint32 index)const  {
    return ParseArray[index];
}

uint32 JsonParser::GetParseRow(const uint32 index)const  {
    return Parse_row[index];
}

uint32 JsonParser::GetConflict(const uint32 index)const  {
    return Conflict[index];
}

uint32 JsonParser::GetConflictRow(const uint32 index)const  {
    return Conflict_row[index];
}

uint32 JsonParser::GetConstant(const uint32 index)const  {
    return Constants[index];
}

uint32 JsonParser::GetConditionalProduction(const uint32 symbol)const  {
    return 0u;
}

uint32 JsonParser::GetPredictedEntry(const uint32 productionNumber,
                                     const uint32 tokenId,
                                     const uint32 level,
                                     const uint32 x)const  {
    return 0u;
}

}
