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

/*lint -e{9117} data generated from slk*/
static uint32 Production[] = {0

,3,9,10,24 ,3,10,11,19 ,6,11,25,1,2,12,26 ,5,11,27,1,2,16 
,2,11,17 ,2,12,13 ,2,12,14 ,2,12,15 ,3,13,28,18 
,5,14,3,13,20,4 ,5,15,3,14,21,4 ,5,16,5,11,22,6 
,5,17,5,11,23,6 ,2,18,1 ,2,18,7 ,3,19,11,19 
,1,19 ,3,20,13,20 ,2,20,29 ,3,21,14,21 ,2,21,30 
,3,22,11,22 ,2,22,31 ,3,23,11,23 ,1,23 
,0};

/*lint -e{9117} data generated from slk*/
static uint32 Production_row[] = {0

,1,5,9,16,22,25,28,31,34,38,44,50,56,62,65,68
,72,74,78,81,85,88,92,95,99
,0};

/*lint -e{9117} data generated from slk*/
static uint32 ParseArray[] = {

0,0,6,16,27,10,18,16,6,19,17,22,18,24,11,22,23,24,25
,1,2,26,9,1,2,5,14,12,9,20,21,13,15,0,0
};

/*lint -e{9117} data generated from slk*/
static uint32 Parse_row[] = {0

,18,19,20,1,21,2,11,22,26,25,2,5,26,10,12
,0};

/*lint -e{9117} data generated from slk*/
static uint32 Conflict[] = {

0,0,7,28,8,3,0,3,7,4,0,3
};

/*lint -e{9117} data generated from slk*/
static uint32 Conflict_row[] = {0

,1,1,4
,0};


/*
 #define START_SYMBOL 9
 #define END_OF_SLK_INPUT_ 8
 #define START_STATE 0
 #define START_CONFLICT 26
 #define END_CONFLICT 29
 #define START_ACTION 24
 #define END_ACTION 32
 #define TOTAL_CONFLICTS 3
 */

static const uint32 Constants[] = { 9u, 8u, 0u, 26u, 29u, 24u, 32u, 3u };


static const char8 * Terminal_name[] = { "0"

, "STRING", ":", "[", "]", "{", "}", "NUMBER", "END_OF_SLK_INPUT" };


static const char8 *GetTerminalName(const uint32 symbol) {
    return Terminal_name[symbol];
}

JsonParser::JsonParser(StreamI &stream,
                       StructuredDataI &databaseIn,
                       BufferedStreamI * const err) :
        ConfigurationParserI(stream, databaseIn, err, JsonGrammar) {
    Action[0] = static_cast<void (JsonParser::*)(void)>(NULL);
    Action [ 1 ] = &JsonParser::End;
    Action [ 2 ] = &JsonParser::GetNodeName;
    Action [ 3 ] = &JsonParser::AddLeaf;
    Action [ 4 ] = &JsonParser::CreateNode;
    Action [ 5 ] = &JsonParser::AddScalar;
    Action [ 6 ] = &JsonParser::EndVector;
    Action [ 7 ] = &JsonParser::EndMatrix;
    Action [ 8 ] = &JsonParser::BlockEnd;

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
