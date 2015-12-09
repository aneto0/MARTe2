/**
 * @file XMLParser.cpp
 * @brief Source file for class XMLParser
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
 * the class XMLParser (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

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

static uint16 Production[] = { 0

, 3, 9, 10, 22, 3, 10, 11, 18, 11, 11, 1, 23, 2, 3, 12, 1, 4, 2, 3, 24, 15, 11, 1, 23, 2, 3, 5, 2, 25, 6, 12, 1, 4, 2, 3, 24, 11, 11, 1, 26, 2, 3, 16, 1, 4, 2,
        3, 27, 2, 12, 13, 2, 12, 14, 2, 12, 15, 3, 13, 28, 17, 8, 14, 1, 3, 13, 19, 1, 4, 3, 9, 15, 1, 3, 14, 29, 20, 1, 4, 3, 3, 16, 11, 21, 2, 17, 2, 2, 17,
        7, 3, 18, 11, 18, 1, 18, 3, 19, 13, 19, 1, 19, 4, 20, 14, 29, 20, 1, 20, 3, 21, 11, 21, 1, 21, 0 };

static uint16 Production_row[] = { 0

, 1, 5, 9, 21, 37, 49, 52, 55, 58, 62, 71, 81, 85, 88, 91, 95, 97, 101, 103, 108, 110, 114, 0 };

static uint16 ParseArray[] = {

0, 0, 24, 6, 18, 17, 9, 13, 6, 15, 17, 9, 14, 1, 2, 23, 16, 10, 11, 12, 25, 26, 0, 0, 0, 0, 0, 0, 0 };

static uint16 Parse_row[] = { 0

, 12, 13, 14, 1, 4, 16, 17, 18, 5, 8, 3, 19, 20, 0 };

static uint16 Conflict[] = {

0, 0, 0, 27, 28, 19, 20, 21, 29, 22, 8, 7, 0, 30, 3, 0, 7, 4, 0, 3, 5, 3 };

static uint16 Conflict_row[] = { 0

, 1, 1, 2, 5, 5, 9, 12, 18, 0 };

static const uint16 Constants[] = { 9u, 8u, 0u, 23u, 31u, 22u, 30u, 8u };

XMLParser::XMLParser(StreamI &stream,
                     ConfigurationDatabase &databaseIn,
                     BufferedStreamI * const err) :
        ParserI(stream, databaseIn, err, XMLGrammar) {

}

XMLParser::~XMLParser() {

}

uint16 &XMLParser::GetProduction(uint32 index) {
    return Production[index];
}

uint16 XMLParser::GetProductionRow(uint32 index) {
    return Production_row[index];
}

uint16 XMLParser::GetParse(uint32 index) {
    return ParseArray[index];
}

uint16 XMLParser::GetParseRow(uint32 index) {
    return Parse_row[index];
}

uint16 XMLParser::GetConflict(uint32 index) {
    return Conflict[index];
}

uint16 XMLParser::GetConflictRow(uint32 index) {
    return Conflict_row[index];
}

uint16 XMLParser::GetConstant(uint32 index) {
    return Constants[index];
}

uint16 XMLParser::GetConditionalProduction(uint16 symbol) {
    return 0u;
}

uint16 XMLParser::GetPredictedEntry(SlkToken tokenProducer,
                                    uint16 productionNumber,
                                    uint16 tokenId,
                                    uint16 level,
                                    uint16 x) {
    return 0u;
}

}
