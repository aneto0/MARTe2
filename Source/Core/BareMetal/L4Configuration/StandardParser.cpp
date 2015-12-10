/**
 * @file StandardParser.cpp
 * @brief Source file for class StandardParser
 * @date 09/dic/2015
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
 * the class StandardParser (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static uint32 Production[] = { 0

, 3, 9, 10, 22, 3, 10, 11, 18, 6, 11, 23, 1, 2, 12, 24, 10, 11, 23, 1, 2, 3, 25, 1, 4, 12, 24, 5, 11, 26, 1, 2, 16, 2, 12, 13, 2, 12, 14, 2, 12, 15, 3, 13, 27,
        17, 5, 14, 5, 13, 19, 6, 5, 15, 5, 14, 20, 6, 5, 16, 5, 11, 21, 6, 2, 17, 1, 2, 17, 7, 3, 18, 11, 18, 1, 18, 3, 19, 13, 19, 2, 19, 28, 3, 20, 14, 20, 2,
        20, 29, 3, 21, 11, 21, 2, 21, 30

        , 0 };

static uint32 Production_row[] = { 0

, 1, 5, 9, 16, 27, 33, 36, 39, 42, 46, 52, 58, 64, 67, 70, 74, 76, 80, 83, 87, 90, 94, 0 };

static uint32 ParseArray[] = {

0, 0, 6, 1, 17, 9, 24, 13, 6, 18, 17, 9, 15, 14, 19, 20, 21, 2, 23, 16, 10, 22, 11, 12, 0, 0, 0 };

static uint32 Parse_row[] = { 0

, 2, 16, 17, 1, 4, 15, 17, 18, 6, 11, 3, 9, 15, 0 };

static uint32 Conflict[] = {

0, 0, 7, 25, 0, 3, 8, 4, 7, 26, 27, 3, 3, 5, 3, 0, 3, 3, 3 };

static uint32 Conflict_row[] = { 0

, 1, 1, 4, 9, 11, 0 };

/*
 #define START_SYMBOL 9
 #define END_OF_SLK_INPUT_ 8
 #define START_STATE 0
 #define START_CONFLICT 23
 #define END_CONFLICT 28
 #define START_ACTION 22
 #define END_ACTION 31
 #define TOTAL_CONFLICTS 5
 */

static const uint32 Constants[] = { 9u, 8u, 0u, 23u, 28u, 22u, 31u, 5u };

static const char8 * Nonterminal_name[] = { "0"

, "expression", "cdbFile", "expressions", "variables", "scalar", "vector", "matrix", "block", "token", "expressions_*", "scalar_*", "vector_*",
        "expressions_2_*" };

static const char8 * Terminal_name[] = { "0"

, "STRING", "=", "(", ")", "{", "}", "NUMBER", "END_OF_SLK_INPUT" };

static const char8 * Action_name[] = { "0"

, "__End", "__GetNodeName", "__AddLeaf", "__GetTypeCast", "__CreateNode", "__AddScalar", "__EndVector", "__EndMatrix", "__BlockEnd" };

static const char8 *GetActionName(const uint32 symbol) {
    return Action_name[symbol - (Constants[ParserConstant::START_ACTION] - 1u)];
}

static const char8 *GetNonTerminalName(const uint32 symbol) {
    return Nonterminal_name[symbol - (Constants[ParserConstant::START_SYMBOL] - 1u)];
}

static const char8 *GetTerminalName(const uint32 symbol) {
    return Terminal_name[symbol];
}

StandardParser::StandardParser(StreamI &stream,
                               ConfigurationDatabase &databaseIn,
                               BufferedStreamI * const err) :
        ParserI(stream, databaseIn, err, StandardGrammar) {
    Action[0] = 0;
    Action[1] = &StandardParser::End;
    Action[2] = &StandardParser::GetNodeName;
    Action[3] = &StandardParser::AddLeaf;
    Action[4] = &StandardParser::GetTypeCast;
    Action[5] = &StandardParser::CreateNode;
    Action[6] = &StandardParser::AddScalar;
    Action[7] = &StandardParser::EndVector;
    Action[8] = &StandardParser::EndMatrix;
    Action[9] = &StandardParser::BlockEnd;
}

StandardParser::~StandardParser() {

}


void StandardParser::Execute(uint32 number) {
    (this->*Action[number])();
}

const char8 *StandardParser::GetSymbolName(const uint32 symbol)const  {
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

uint32 &StandardParser::GetProduction(const uint32 index)const  {
    return Production[index];
}

uint32 StandardParser::GetProductionRow(const uint32 index)const  {
    return Production_row[index];
}

uint32 StandardParser::GetParse(const uint32 index)const  {
    return ParseArray[index];
}

uint32 StandardParser::GetParseRow(const uint32 index)const  {
    return Parse_row[index];
}

uint32 StandardParser::GetConflict(const uint32 index)const  {
    return Conflict[index];
}

uint32 StandardParser::GetConflictRow(const uint32 index)const  {
    return Conflict_row[index];
}

uint32 StandardParser::GetConstant(const uint32 index)const  {
    return Constants[index];
}

uint32 StandardParser::GetConditionalProduction(const uint32 symbol)const  {
    return 0u;
}

uint32 StandardParser::GetPredictedEntry(const uint32 productionNumber,
                                         const uint32 tokenId,
                                         const uint32 level,
                                         const uint32 x)const  {
    return 0u;
}

}
