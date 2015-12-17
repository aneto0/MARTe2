/**
 * @file JsonParser.cpp
 * @brief Source file for class JsonParser
 * @date 10/12/2015
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
 * the class JsonParser (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

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

static uint32 Production[] = { 0u

, 3u, 9u, 10u, 22u, 3u, 10u, 11u, 18u, 6u, 11u, 23u, 1u, 2u, 12u, 24u, 5u, 11u, 25u, 1u, 2u, 16u, 2u, 12u, 13u, 2u, 12u, 14u, 2u, 12u, 15u, 3u, 13u, 26u, 17u, 5u, 14u, 3u, 13u, 19u, 4u, 5u, 15u, 3u, 14u,
        20u, 4u, 5u, 16u, 5u, 11u, 21u, 6u, 2u, 17u, 1u, 2u, 17u, 7u, 3u, 18u, 11u, 18u, 1u, 18u, 3u, 19u, 13u, 19u, 2u, 19u, 27u, 3u, 20u, 14u, 20u, 2u, 20u, 28u, 3u, 21u, 11u, 21u, 2u, 21u, 29u, 0u };

static uint32 Production_row[] = { 0u

, 1u, 5u, 9u, 16u, 22u, 25u, 28u, 31u, 35u, 41u, 47u, 53u, 56u, 59u, 63u, 65u, 69u, 72u, 76u, 79u, 83u, 0u};

static uint32 ParseArray[] = {

0u, 0u, 5u, 16u, 23u, 8u, 17u, 12u, 5u, 16u, 14u, 8u, 1u, 13u, 18u, 19u, 20u, 15u, 2u, 22u, 9u, 21u, 10u, 11u, 0u, 0u, 0u, 0u };

static uint32 Parse_row[] = { 0u

, 11u, 17u, 18u, 1u, 4u, 17u, 19u, 18u, 6u, 9u, 2u, 11u, 15u, 0u };

static uint32 Conflict[] = {

0u, 0u, 6u, 24u, 7u, 3u, 0u, 3u, 6u, 4u, 0u, 3u };

static uint32 Conflict_row[] = { 0u
, 1u, 1u, 4u, 0u };
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


static const char8 * Terminal_name[] = { "0"

, "STRING", ":", "[", "]", "{", "}", "NUMBER", "END_OF_SLK_INPUT" };


static const char8 *GetTerminalName(const uint32 symbol) {
    return Terminal_name[symbol];
}

JsonParser::JsonParser(StreamI &stream,
                       StructuredDataI &databaseIn,
                       BufferedStreamI * const err) :
        ParserI(stream, databaseIn, err, JsonGrammar) {
    Action[0] = static_cast<void (JsonParser::*)(void)>(NULL);
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

void JsonParser::Execute(const uint32 number) {
    (this->*Action[number])();
}

const char8 *JsonParser::GetSymbolName(const uint32 symbol)const  {
    const char8 *symbolName = static_cast<const char8 *>(NULL);

    if((symbol > 0u) && (symbol < Constants[ParserConstant::START_SYMBOL])) {
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

}
