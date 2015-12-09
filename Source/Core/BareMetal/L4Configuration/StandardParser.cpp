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

static uint16 Production[] = { 0

, 3, 9, 10, 22, 3, 10, 11, 18, 6, 11, 23, 1, 2, 12, 24, 10, 11, 23, 1, 2, 3, 25, 1, 4, 12, 24, 5, 11, 26, 1, 2, 16, 2, 12, 13, 2, 12, 14, 2, 12, 15, 3, 13, 27,
        17, 5, 14, 5, 13, 19, 6, 5, 15, 5, 14, 20, 6, 5, 16, 5, 11, 21, 6, 2, 17, 1, 2, 17, 7, 3, 18, 11, 18, 1, 18, 3, 19, 13, 19, 2, 19, 28, 3, 20, 14, 20, 2,
        20, 29, 3, 21, 11, 21, 2, 21, 30

        , 0 };

static uint16 Production_row[] = { 0

, 1, 5, 9, 16, 27, 33, 36, 39, 42, 46, 52, 58, 64, 67, 70, 74, 76, 80, 83, 87, 90, 94, 0 };

static uint16 ParseArray[] = {

0, 0, 6, 1, 17, 9, 24, 13, 6, 18, 17, 9, 15, 14, 19, 20, 21, 2, 23, 16, 10, 22, 11, 12, 0, 0, 0 };

static uint16 Parse_row[] = { 0

, 2, 16, 17, 1, 4, 15, 17, 18, 6, 11, 3, 9, 15, 0 };

static uint16 Conflict[] = {

0, 0, 7, 25, 0, 3, 8, 4, 7, 26, 27, 3, 3, 5, 3, 0, 3, 3, 3 };

static uint16 Conflict_row[] = { 0

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
static const uint16 Constants[] = { 9u, 8u, 0u, 23u, 28u, 22u, 31u, 5u };

StandardParser::StandardParser(StreamI &stream,
                               ConfigurationDatabase &databaseIn,
                               BufferedStreamI * const err) :
        ParserI(stream, databaseIn, err, StandardGrammar) {

}

StandardParser::~StandardParser() {

}

uint16 &StandardParser::GetProduction(uint32 index) {
    return Production[index];
}

uint16 StandardParser::GetProductionRow(uint32 index) {
    return Production_row[index];
}

uint16 StandardParser::GetParse(uint32 index) {
    return ParseArray[index];
}

uint16 StandardParser::GetParseRow(uint32 index) {
    return Parse_row[index];
}

uint16 StandardParser::GetConflict(uint32 index) {
    return Conflict[index];
}

uint16 StandardParser::GetConflictRow(uint32 index) {
    return Conflict_row[index];
}

uint16 StandardParser::GetConstant(uint32 index) {
    return Constants[index];
}

uint16 StandardParser::GetConditionalProduction(uint16 symbol) {
    return 0u;
}

uint16 StandardParser::GetPredictedEntry(SlkToken tokenProducer,
                                          uint16 productionNumber,
                                          uint16 tokenId,
                                          uint16 level,
                                          uint16 x) {
    return 0u;
}

}
