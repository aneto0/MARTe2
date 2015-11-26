/**
 * @file Parser.h
 * @brief Header file for class Parser
 * @date 25/11/2015
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

 * @details This header file contains the declaration of the class Parser
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4CONFIGURATION_PARSER_H_
#define L4CONFIGURATION_PARSER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "StreamI.h"
#include "StaticListHolder.h"
#include "LexicalAnalyzer.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/** a base class for parsers. Use slk and CreateParserData to create the actual parser data from a ll file*/
class Parser: public Object {

    typedef int32 TokenDataType;

    CLASS_REGISTER_DECLARATION()


public:
    /** */
    Parser();
    /** */
    virtual ~Parser();

    /** @param errorLevel
     0 means do not display
     1 means show actions
     2 means show productions
     0xFFFFFFFF means show all      */
    bool Parse(StreamI &stream,
               StreamI *err = NULL,
               int32 errorLevel = 0);

protected:

    /** automatically created and contained in ParserData.cpp */
    virtual void InitTokens() = 0;

    /** the action to be performed */
    virtual void Action(int32 actionNumber,
                        Token *latd) = 0;

    virtual TokenDataType GetParse(int32 index) = 0;

    virtual int32 GetParseRow(int32 index) = 0;

    virtual TokenDataType GetConflict(int32 index) = 0;

    virtual int32 GetConflictRow(int32 index) = 0;

    virtual TokenDataType GetProduction(int32 index) = 0;

    virtual int32 GetProductionRow(int32 index) = 0;

    virtual TokenDataType GetTerminal2Index(int32 index) = 0;

    virtual const char8 * GetProductionName(int32 index) = 0;

    virtual const char8 * GetNonTerminalName(int32 index) = 0;

    virtual const char8 * GetTerminalName(int32 index) = 0;

    virtual const char8 * GetActionName(int32 index) = 0;

    virtual TokenDataType GetStartSymbol() = 0;


    /** a lexicala analyzer */
    LexicalAnalyzer lexicalAnalyzer;

    /** */
    StaticListHolder stack;


private:
    /** */
    const char8 *GetSymbolName(int32 symbol);

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PARSER_H_ */

