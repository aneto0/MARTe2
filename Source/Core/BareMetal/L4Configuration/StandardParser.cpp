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

static uint32 Production[] = {0u
,3u,10u,11u,23u,3u,11u,12u,19u,6u,12u,24u,1u,2u,13u,25u,10u,12u,24u,1u,2u,3u,26u,1u,4u,13u,25u
,11u,12u,24u,1u,2u,3u,27u,1u,5u,14u,4u,25u,5u,12u,28u,1u,2u,17u
,2u,13u,14u,2u,13u,15u,2u,13u,16u,3u,14u,29u,18u,5u,15u,6u,14u,20u,7u
,5u,16u,6u,15u,21u,7u,5u,17u,6u,12u,22u,7u,2u,18u,1u,2u,18u,8u
,3u,19u,12u,19u,1u,19u,3u,20u,14u,20u,2u,20u,30u,3u,21u,15u,21u
,2u,21u,31u,3u,22u,12u,22u,2u,22u,32u
,0u};

static uint32 Production_row[] = {0u
,1u,5u,9u,16u,27u,39u,45u,48u,51u,54u,58u,64u,70u,76u,79u,82u
,86u,88u,92u,95u,99u,102u,106u
,0u};

static uint32 ParseArray[] = {
0u,0u,7u,1u,18u,10u,14u,25u,16u,7u,19u,18u,10u,15u,20u,21u,17u,22u,2u
,24u,11u,12u,13u,23u,0u,0u,0u,0u
};

static uint32 Parse_row[] = {0u
,2u,17u,18u,1u,4u,14u,15u,16u,5u,7u,3u,8u,16u
,0u};

static uint32 Conflict[] = {
0u,0u,8u,26u,29u,0u,3u,9u,27u,8u,0u,28u,0u,3u,30u,4u,5u,0u,0u
,3u,0u,3u,3u,6u,0u,0u,0u,0u,3u,3u
};

static uint32 Conflict_row[] = {0u
,1u,1u,5u,3u,13u,11u,21u
,0u};

/*
#define START_SYMBOL 10
#define END_OF_SLK_INPUT_ 9
#define START_STATE 0
#define START_CONFLICT 24
#define END_CONFLICT 31
#define START_ACTION 23
#define END_ACTION 33
#define TOTAL_CONFLICTS 7
*/
static const uint32 Constants[] = { 10u, 9u, 0u, 24u, 31u, 23u, 33u, 7u };

static const char8 * Terminal_name[] ={"0"
,"STRING"
,"="
,"("
,")"
,"|"
,"{"
,"}"
,"NUMBER"
,"END_OF_SLK_INPUT"
};

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
    Action[5] = &StandardParser::GetExprCast;
    Action[6] = &StandardParser::CreateNode;
    Action[7] = &StandardParser::AddScalar;
    Action[8] = &StandardParser::EndVector;
    Action[9] = &StandardParser::EndMatrix;
    Action[10] = &StandardParser::BlockEnd;
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
