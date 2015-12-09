/**
 * @file ParserI.h
 * @brief Header file for class ParserI
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

 * @details This header file contains the declaration of the class ParserI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PARSERI_H_
#define PARSERI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "SlkError.h"
#include "SlkToken.h"
#include "StaticListHolder.h"
#include "ConfigurationDatabase.h"
#include "StreamString.h"
#include "AnyTypeCreator.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class ParserI {

public:

    ParserI(StreamI &stream,
            ConfigurationDatabase &databaseIn,
            BufferedStreamI * const err,
            ParserGrammar grammarIn);

    virtual ~ParserI();

    bool Parse();

    ParserGrammar GetGrammar() const;

private:

    virtual void End();
    virtual void GetNodeName();
    virtual void AddLeaf();
    virtual void GetTypeCast();
    virtual void CreateNode();
    virtual void AddScalar();
    virtual void EndVector();
    virtual void EndMatrix();
    virtual void BlockEnd();
    virtual void Predict(uint16 entry);

    virtual uint16 &GetProduction(uint32 index)=0;

    virtual uint16 GetProductionRow(uint32 index)=0;

    virtual uint16 GetParse(uint32 index)=0;

    virtual uint16 GetParseRow(uint32 index)=0;

    virtual uint16 GetConflict(uint32 index)=0;

    virtual uint16 GetConflictRow(uint32 index)=0;

    virtual uint16 GetConditionalProduction(uint16 symbol)=0;

    virtual uint16 GetPredictedEntry(SlkToken tokenProducer,
                                     uint16 productionNumber,
                                     uint16 tokenId,
                                     uint16 level,
                                     uint16 x)=0;

    virtual uint16 GetConstant(uint32 index)=0;

    inline void StackPush(uint16 symbol,
                          uint16* &stack,
                          uint16* &top);

    inline uint16 StackPop(uint16* &top);

    //void CreateClassLeaf();

    void InitializeTable();
    void Execute(int32 number);

    // functions void f()
    void (ParserI::*Action[10])(void);

    StreamString typeName;

    StreamString nodeName;

    ConfigurationDatabase *database;

    SlkError parseError;

    SlkToken tokenProducer;

    uint32 numberOfColumns;

    uint32 firstNumberOfColumns;

    uint32 numberOfRows;

    AnyTypeCreator memory;

    BufferedStreamI *errorStream;

    uint32 tokenType;

    uint32 numberOfDimensions;

    ParserGrammar grammar;

};

namespace ParserConstant {

static const uint32 START_SYMBOL = 0u;
static const uint32 END_OF_SLK_INPUT = 1u;
static const uint32 START_STATE = 2u;
static const uint32 START_CONFLICT = 3u;
static const uint32 END_CONFLICT = 4u;
static const uint32 START_ACTION = 5u;
static const uint32 END_ACTION = 6u;
static const uint32 TOTAL_CONFLICTS = 7u;
static const uint32 PARSE_STACK_SIZE = 512u;

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
//#define StackPush(symbol,stack,top) if ( top > stack ) *--top = symbol
//#define StackPop(top)   (*top ? *top++ : 0)

void ParserI::StackPush(uint16 symbol,
                        uint16 *&stack,
                        uint16 *&top) {
    if (top > stack) {
        top--;
        *top = symbol;
    }
}

uint16 ParserI::StackPop(uint16 * &top) {
    uint16 ret = 0u;
    if (*top != 0u) {
        ret = *top;
        top++;
    }
    return ret;
}

}
#endif /* PARSERI_H_ */

