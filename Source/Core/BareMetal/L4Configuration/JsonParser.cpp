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
,3,11,12,29 ,3,12,13,23 ,6,13,30,1,2,14,31 ,10,13,30,1,2,3,32,1,4,14,31 
,5,13,33,1,2,19 ,5,13,34,1,2,16 ,2,13,21 ,2,14,15 
,2,14,17 ,2,14,18 ,3,15,35,22 ,5,16,5,20,24,6 
,5,17,5,15,25,6 ,5,18,5,17,26,6 ,5,19,7,13,27,8 
,3,20,40,19 ,5,21,7,13,28,8 ,2,22,1 ,2,22,9 
,3,23,13,23 ,1,23 ,3,24,20,24 ,2,24,36 ,3,25,15,25 
,2,25,37 ,3,26,17,26 ,2,26,38 ,3,27,13,27 ,2,27,39 
,3,28,13,28 ,1,28 
,0};

/*lint -e{9117} data generated from slk*/
static uint32 Production_row[] = {0
,1,5,9,16,27,33,39,42,45,48,51,55,61,67,73,79
,83,89,92,95,99,101,105,108,112,115,119,122,126,129,133
,0};

/*lint -e{9117} data generated from slk*/
static uint32 ParseArray[] = {
0,0,8,20,0,0,33,11,24,20,8,28,21,25,30,11,24,28,29
,1,30,31,2,32,18,1,23,22,2,7,26,27,19,12,13,14,15,16,17
,0,0,0};

/*lint -e{9117} data generated from slk*/
static uint32 Parse_row[] = {0
,18,21,22,1,6,28,29,30,29,30,31,23,2,20,7,25
,10,13
,0};

/*lint -e{9117} data generated from slk*/
static uint32 Conflict[] = {
0,0,9,34,0,3,10,4,3,35,9,5,3,3,6,0,3
};

/*lint -e{9117} data generated from slk*/
static uint32 Conflict_row[] = {0
,1,1,4,7
,0
};

/*
 #define START_SYMBOL 11
 #define END_OF_SLK_INPUT_ 10
 #define START_STATE 0
 #define START_CONFLICT 32
 #define END_CONFLICT 36
 #define START_ACTION 29
 #define END_ACTION 41
 #define TOTAL_CONFLICTS 4
*/

static const uint32 Constants[] = { 11u, 10u, 0u, 32u, 36u, 29u, 41u, 4u };

static const char8 * Terminal_name[] ={"0"
,"STRING"
,":"
,"("
,")"
,"["
,"]"
,"{"
,"}"
,"NUMBER"
,"END_OF_SLK_INPUT"
};

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
    Action [ 4 ] = &JsonParser::GetTypeCast;
    Action [ 5 ] = &JsonParser::CreateNode;
    Action [ 6 ] = &JsonParser::CreateNodeV;
    Action [ 7 ] = &JsonParser::AddScalar;
    Action [ 8 ] = &JsonParser::EndVectorB;
    Action [ 9 ] = &JsonParser::EndVector;
    Action [ 10 ] = &JsonParser::EndMatrix;
    Action [ 11 ] = &JsonParser::BlockEnd;
    Action [ 12 ] = &JsonParser::CreateNodeVE;
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
