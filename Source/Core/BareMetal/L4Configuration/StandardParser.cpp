/**
 * @file StandardParser.cpp
 * @brief Source file for class StandardParser
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
 * the class StandardParser (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

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

static uint32 Production[] = { 0u

, 3u, 9u, 10u, 22u, 3u, 10u, 11u, 18u, 6u, 11u, 23u, 1u, 2u, 12u, 24u, 10u, 11u, 23u, 1u, 2u, 3u, 25u, 1u, 4u, 12u, 24u, 5u, 11u, 26u, 1u, 2u, 16u, 2u, 12u, 13u, 2u, 12u, 14u, 2u, 12u, 15u, 3u, 13u, 27u,
        17u, 5u, 14u, 5u, 13u, 19u, 6u, 5u, 15u, 5u, 14u, 20u, 6u, 5u, 16u, 5u, 11u, 21u, 6u, 2u, 17u, 1u, 2u, 17u, 7u, 3u, 18u, 11u, 18u, 1u, 18u, 3u, 19u, 13u, 19u, 2u, 19u, 28u, 3u, 20u, 14u, 20u, 2u,
        20u, 29u, 3u, 21u, 11u, 21u, 2u, 21u, 30u
        , 0u };

static uint32 Production_row[] = { 0u

, 1u, 5u, 9u, 16u, 27u, 33u, 36u, 39u, 42u, 46u, 52u, 58u, 64u, 67u, 70u, 74u, 76u, 80u, 83u, 87u, 90u, 94u, 0u };

static uint32 ParseArray[] = {

0u, 0u, 6u, 1u, 17u, 9u, 24u, 13u, 6u, 18u, 17u, 9u, 15u, 14u, 19u, 20u, 21u, 2u, 23u, 16u, 10u, 22u, 11u, 12u, 0u, 0u, 0u };

static uint32 Parse_row[] = { 0u

, 2u, 16u, 17u, 1u, 4u, 15u, 17u, 18u, 6u, 11u, 3u, 9u, 15u, 0u };

static uint32 Conflict[] = {

0u, 0u, 7u, 25u, 0u, 3u, 8u, 4u, 7u, 26u, 27u, 3u, 3u, 5u, 3u, 0u, 3u, 3u, 3u };

static uint32 Conflict_row[] = { 0u

, 1u, 1u, 4u, 9u, 11u, 0u };


static const uint32 Constants[] = { 9u, 8u, 0u, 23u, 28u, 22u, 31u, 5u };


static const char8 * Terminal_name[] = { "0"

, "STRING", "=", "(", ")", "{", "}", "NUMBER", "END_OF_SLK_INPUT" };


static const char8 *GetTerminalName(const uint32 symbol) {
    return Terminal_name[symbol];
}

StandardParser::StandardParser(StreamI &stream,
                               StructuredDataI &databaseIn,
                               BufferedStreamI * const err) :
        ConfigurationParserI(stream, databaseIn, err, StandardGrammar) {
    
    Action[0] = static_cast<void (StandardParser::*)(void)>(NULL);
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


void StandardParser::Execute(const uint32 number) {
    (this->*Action[number])();
}

const char8 *StandardParser::GetSymbolName(const uint32 symbol)const  {
    const char8 *symbolName = static_cast<const char8 *>(NULL);

    if((symbol > 0u) && (symbol < Constants[ParserConstant::START_SYMBOL])) {
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


}
