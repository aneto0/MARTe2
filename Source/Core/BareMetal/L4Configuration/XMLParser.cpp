/**
 * @file XMLParser.cpp
 * @brief Source file for class XMLParser
 * @date 09/12/2015
 * @author Giuseppe Ferro
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
 * the class XMLParser (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "XMLParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static uint32 Production[] = { 0u

, 3u, 11u, 12u, 24u, 3u, 12u, 13u, 20u, 11u, 13u, 1u, 25u, 2u, 3u, 14u, 1u, 4u, 2u, 3u, 26u, 15u, 13u, 1u, 25u, 2u, 3u, 5u, 27u, 2u, 6u, 14u, 1u, 4u, 2u, 3u, 26u, 10u, 13u, 1u, 28u, 2u, 3u, 18u, 1u, 4u, 2u,
        3u, 2u, 14u, 15u, 2u, 14u, 16u, 2u, 14u, 17u, 3u, 15u, 29u, 19u, 5u, 16u, 7u, 15u, 21u, 8u, 5u, 17u, 7u, 16u, 22u, 8u, 3u, 18u, 13u, 23u, 2u, 19u, 2u, 2u, 19u, 9u, 3u, 20u, 13u, 20u, 1u, 20u, 3u,
        21u, 15u, 21u, 2u, 21u, 30u, 3u, 22u, 16u, 22u, 2u, 22u, 31u, 3u, 23u, 13u, 23u, 2u, 23u, 32u, 0u };

static uint32 Production_row[] = { 0u

, 1u, 5u, 9u, 21u, 37u, 48u, 51u, 54u, 57u, 61u, 67u, 73u, 77u, 80u, 83u, 87u, 89u, 93u, 96u, 100u, 103u, 107u, 0u };

static uint32 ParseArray[] = {

0u, 0u, 1u, 6u, 2u, 17u, 9u, 13u, 24u, 15u, 6u, 18u, 17u, 9u, 14u, 19u, 20u, 23u, 16u, 10u, 11u, 12u, 25u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u };

static uint32 Parse_row[] = { 0u

, 1u, 3u, 16u, 1u, 4u, 12u, 13u, 20u, 5u, 8u, 3u, 8u, 21u, 0u };

static uint32 Conflict[] = {

0u, 0u, 0u, 26u, 7u, 21u, 27u, 22u, 0u, 8u, 0u, 7u, 5u, 3u, 0u, 0u, 4u, 0u, 3u, 0u, 3u };

static uint32 Conflict_row[] = { 0u

, 1u, 2u, 3u, 3u, 11u, 0u };

/*
 #define START_SYMBOL 11
 #define END_OF_SLK_INPUT_ 10
 #define START_STATE 0
 #define START_CONFLICT 23
 #define END_CONFLICT 28
 #define START_ACTION 24
 #define END_ACTION 33
 #define TOTAL_CONFLICTS 5
 #define PARSE_STACK_SIZE 512
 */
static const uint32 Constants[] = { 11u, 10u, 0u, 23u, 28u, 24u, 33u, 5u };


static const char8 * Terminal_name[] = { "0"

, "<", "STRING", ">", "/", "(", ")", "{", "}", "NUMBER", "END_OF_SLK_INPUT" };


/*
 #define START_SYMBOL 11
 #define START_ACTION 24
 #define END_ACTION 33
 #define GET_NONTERMINAL_NAME(symbol) (Nonterminal_name [symbol - 10])
 #define GET_TERMINAL_NAME(symbol) (Terminal_name [symbol])
 #define GET_ACTION_NAME(symbol) (Action_name [symbol-(START_ACTION-1)])
 #define GET_PRODUCTION_NAME(number) (Production_name [number])

 */

static const char8 *GetTerminalName(const uint32 symbol) {
    return Terminal_name[symbol];
}

XMLParser::XMLParser(StreamI &stream,
                     StructuredDataI &databaseIn,
                     BufferedStreamI * const err) :
        ConfigurationParserI(stream, databaseIn, err, XMLGrammar) {
    Action[0] = static_cast<void (XMLParser::*)(void)>(NULL);
    Action[1] = &XMLParser::End;
    Action[2] = &XMLParser::GetNodeName;
    Action[3] = &XMLParser::AddLeaf;
    Action[4] = &XMLParser::GetTypeCast;
    Action[5] = &XMLParser::CreateNode;
    Action[6] = &XMLParser::AddScalar;
    Action[7] = &XMLParser::EndVector;
    Action[8] = &XMLParser::EndMatrix;
    Action[9] = &XMLParser::BlockEnd;
}

XMLParser::~XMLParser() {

}

void XMLParser::Execute(const uint32 number) {
    (this->*Action[number])();
}

const char8 *XMLParser::GetSymbolName(const uint32 symbol)const  {
    const char8 *symbolName = static_cast<const char8 *>(NULL);

    if((symbol > 0u) && (symbol < Constants[ParserConstant::START_SYMBOL])) {
        symbolName=GetTerminalName(symbol);
    }
    else {
        symbolName="not a symbol";
    }
    return symbolName;
}

uint32 &XMLParser::GetProduction(const uint32 index)const  {
    return Production[index];
}

uint32 XMLParser::GetProductionRow(const uint32 index)const  {
    return Production_row[index];
}

uint32 XMLParser::GetParse(const uint32 index)const  {
    return ParseArray[index];
}

uint32 XMLParser::GetParseRow(const uint32 index)const  {
    return Parse_row[index];
}

uint32 XMLParser::GetConflict(const uint32 index)const  {
    return Conflict[index];
}

uint32 XMLParser::GetConflictRow(const uint32 index)const  {
    return Conflict_row[index];
}

uint32 XMLParser::GetConstant(const uint32 index)const  {
    return Constants[index];
}


}
