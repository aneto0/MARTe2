/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
   will be approved by the European Commission - subsequent
   versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the 
   Licence.
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
   writing, software distributed under the Licence is
   distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
   express or implied.
 * See the Licence for the specific language governing 
   permissions and limitations under the Licence.
 *
 * $Id: Parser.h 3 2012-01-15 16:26:07Z aneto $
 *
**/

/** 
 * @file
 * Parser implementation
 */
#include "System.h"
#include "Object.h"
#include "FString.h"
#include "Streamable.h"
#include "StaticStackHolder.h"
#include "StaticQueueHolder.h"
#include "LexicalAnalyzer.h"

#if !defined(PARSER_H)
#define PARSER_H

typedef int32 TokenDataType;

class Parser;

extern "C"{
    /** */
    bool ParserParse(Parser &parser,Streamable &stream,Streamable *err=NULL,int errorLevel=0);
}

OBJECT_DLL(Parser)

/** a base class for parsers. Use slk and CreateParserData to create the actual parser data from a ll file*/
class Parser: public Object{
OBJECT_DLL_STUFF(Parser)
protected:
    /** a lexicala analyzer */
    LexicalAnalyzer         la;

    /** */
    StaticStackHolder       stack;

private:
    /** */
    const char *GetSymbolName ( int symbol ){
        if ( symbol >= GetStartSymbol() ) {
            return  GetNonTerminalName(symbol - (GetStartSymbol() - 1));
        } else if ( symbol > 0 ) {
            return GetTerminalName (GetTerminal2Index (symbol));
        } else {
            return GetActionName (-symbol);
        }
    }

    /** */
    friend bool ParserParse(Parser &parser,Streamable &stream,Streamable *err,int errorLevel);

protected:

    /** automatically created and contained in ParserData.cpp */
    virtual void             InitTokens()                       = 0;

    /** the action to be performed */
    virtual void             Action(int actionNumber,
                                   LA_TokenData *latd)          = 0;

    virtual TokenDataType    GetParse(int index)                = 0;

    virtual int              GetParseRow(int index)             = 0;

    virtual TokenDataType    GetConflict(int index)             = 0;

    virtual int              GetConflictRow(int index)          = 0;

    virtual TokenDataType    GetProduction(int index)           = 0;

    virtual int              GetProductionRow(int index)        = 0;

    virtual TokenDataType    GetTerminal2Index(int index)       = 0;

    virtual const char *     GetProductionName(int index)       = 0;

    virtual const char *     GetNonTerminalName(int index)      = 0;

    virtual const char *     GetTerminalName(int index)         = 0;

    virtual const char *     GetActionName(int index)           = 0;

    virtual TokenDataType    GetStartSymbol()                   = 0;

public:
    /** */
    Parser():stack(sizeof(TokenDataType)/sizeof(int)){
        la.AddSeparators("\n\r\t ");
        la.AddTerminals("{}()=*");
    }

    /** */
    virtual ~Parser(){
    }

    /** @param errorLevel
        0 means do not display
        1 means show actions
        2 means show productions
        0xFFFFFFFF means show all      */
    bool Parse(Streamable &stream,Streamable *err=NULL,int errorLevel=0){
        return ParserParse(*this,stream,err,errorLevel);
    }


};

#endif
