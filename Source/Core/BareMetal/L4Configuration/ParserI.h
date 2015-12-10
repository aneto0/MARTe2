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
#include "Token.h"
#include "StaticListHolder.h"
#include "ConfigurationDatabase.h"
#include "StreamString.h"
#include "AnyTypeCreator.h"
#include "LexicalAnalyzer.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class ParserI {

public:

    ParserI(StreamI &stream,
            ConfigurationDatabase &databaseIn,
            BufferedStreamI * const err,
            const ParserGrammar &grammarIn);

    virtual ~ParserI();

    bool Parse();

    ParserGrammar GetGrammar() const;

protected:

    virtual void End();
    virtual void GetNodeName();
    virtual void AddLeaf();
    virtual void GetTypeCast();
    virtual void CreateNode();
    virtual void AddScalar();
    virtual void EndVector();
    virtual void EndMatrix();
    virtual void BlockEnd();
    virtual void CreateClassLeaf();
    virtual void Predict(const uint32 entry);

    virtual uint32 &GetProduction(const uint32 index)const =0;

    virtual uint32 GetProductionRow(const uint32 index)const =0;

    virtual uint32 GetParse(const uint32 index)const =0;

    virtual uint32 GetParseRow(const uint32 index)const =0;

    virtual uint32 GetConflict(const uint32 index)const =0;

    virtual uint32 GetConflictRow(const uint32 index)const =0;

    virtual uint32 GetConditionalProduction(const uint32 symbol)const =0;

    virtual uint32 GetPredictedEntry(const uint32 productionNumber,
                                     const uint32 tokenId,
                                     const uint32 level,
                                     const uint32 x)const =0;

    virtual uint32 GetConstant(const uint32 index)const =0;

    virtual const char8 *GetSymbolName(const uint32 symbol)const =0;

    virtual uint32 GetNextTokenType();

    virtual uint32 PeekNextTokenType(const uint32 position);

    inline void StackPush(const uint32 symbol,
                          const uint32 * const stack,
                          uint32* &top) const;

    inline uint32 StackPop(uint32* &top) const;

    //void CreateClassLeaf();

    virtual void Execute(uint32 number)=0;

    Token *currentToken;

private:
    StreamString typeName;

    StreamString nodeName;

    ConfigurationDatabase *database;

    bool isError;

    LexicalAnalyzer tokenProducer;


    uint32 numberOfColumns;

    uint32 firstNumberOfColumns;

    uint32 numberOfRows;

    AnyTypeCreator memory;

    BufferedStreamI *errorStream;

    uint32 tokenType;

    uint8 numberOfDimensions;

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

void ParserI::StackPush(const uint32 symbol,
                        const uint32 * const stack,
                        uint32 *&top) const {
    if (top > stack) {
        top--;
        *top = symbol;
    }
}

uint32 ParserI::StackPop(uint32 * &top) const {
    uint32 ret = 0u;
    if (*top != 0u) {
        ret = *top;
        top++;
    }
    return ret;
}

}
#endif /* PARSERI_H_ */

