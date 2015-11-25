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
 * $Id: Parser.cpp 3 2012-01-15 16:26:07Z aneto $
 *
**/

#include "Parser.h"
#include "LexicalAnalyzer.h"

OBJECTREGISTER(Parser,"$Id: Parser.cpp 3 2012-01-15 16:26:07Z aneto $")

bool ParserParse(Parser &parser,Streamable &stream,Streamable *err,int errorLevel){
    parser.InitTokens();

    TokenDataType   entry;
    TokenDataType   symbol;
    TokenDataType   token;
    TokenDataType   new_token;

    TokenDataType   eofToken = parser.la.GetTokenValue("EOF");

    LA_TokenData *  latd = NULL;
    TokenDataType   startSymbol = parser.GetStartSymbol();

    parser.stack.StackPush((const intptr *)&startSymbol);
    latd = parser.la.GetToken(stream);
    token = latd->Token();
    new_token = token;

    if ( errorLevel & 0x1000) {
        if (!err) parser.AssertErrorCondition(Information, "[%s] %s",latd->Description(),latd->Data() );
        else      err->Printf("[%s] %s\n",latd->Description(),latd->Data() );
    }

    while ( parser.stack.StackPop((intptr *)&symbol)) {

        if ( symbol >= startSymbol ) {                  // nonterminal symbol

            entry = 0;
            int level = 1;
            int index = parser.GetParseRow (symbol - startSymbol + 1);
            index += parser.GetTerminal2Index (token);
            entry = parser.GetParse (index);
            while ( entry < 0 ) {
                int index =  parser.GetConflictRow(-entry);
                LA_TokenData *latd = parser.la.PeekToken(level,stream);
                if ( errorLevel  & 0x1000) {
                    if (!err) parser.AssertErrorCondition(Information, ">>[%s] (%i)%s",latd->Description(),level,latd->Data() );
                    else err->Printf(">>[%s] (%i)%s\n",latd->Description(),level,latd->Data() );
                }
                index += parser.GetTerminal2Index (latd->Token());
                entry =  parser.GetConflict(index);
                ++level;
            }
            if ( entry ) {
                int productionIndex = parser.GetProductionRow(entry);
                productionIndex++;
                if (parser.GetProduction(productionIndex) == symbol) {
                    while (parser.GetProduction(++productionIndex) != 0) {
                        TokenDataType productionEl = parser.GetProduction(productionIndex);
                        parser.stack.StackPush((const intptr *)&productionEl);
                    }
                    if ( errorLevel  & 0x2 ) {
                        if (!err) parser.AssertErrorCondition(Information, "trace production (%i) = %s ",entry,parser.GetProductionName(entry));
                        else      err->Printf("trace production (%i) = %s \n",entry,parser.GetProductionName(entry));
                    }
                } else {                                   // lhs does not match
                    if (err) err->Printf("no entry for nonterminal/token:  %s  %s \n", parser.GetSymbolName ( symbol ), parser.GetSymbolName ( token ) );
                    parser.AssertErrorCondition(ParametersError, "no entry for nonterminal/token:  %s  %s \n", parser.GetSymbolName ( symbol ), parser.GetSymbolName ( token ) );

                    if (latd != NULL) delete latd;
                    latd = parser.la.GetToken(stream);
                    if ( errorLevel  & 0x1000) {
                        if (!err) parser.AssertErrorCondition(Information, "[%s] %s",latd->Description(),latd->Data() );
                        else      err->Printf("[%s] %s\n",latd->Description(),latd->Data() );
                    }
                    new_token = latd->Token();
                }
            } else {                                       // no table entry
                if (err) err->Printf("no entry for nonterminal/token:  %s  %s \n", parser.GetSymbolName ( symbol ), parser.GetSymbolName ( token ) );
                parser.AssertErrorCondition(ParametersError, "no entry for nonterminal/token:  %s  %s \n", parser.GetSymbolName ( symbol ), parser.GetSymbolName ( token ) );
                if (latd != NULL) delete latd;
                latd = parser.la.GetToken(stream);
                if ( errorLevel  & 0x1000) {
                    if (!err) parser.AssertErrorCondition(Information, "[%s] %s",latd->Description(),latd->Data() );
                    else      err->Printf("[%s] %s\n",latd->Description(),latd->Data() );
                }
                new_token = latd->Token();
            }

        } else
        if ( symbol > 0 ) {                         // terminal symbol
            if ( symbol == token ) {
                if (latd != NULL) delete latd;
                latd = parser.la.GetToken(stream);
                if ( errorLevel  & 0x1000 ) {
                    if (!err) parser.AssertErrorCondition(Information, "[%s] %s",latd->Description(),latd->Data() );
                    else err->Printf("[%s] %s\n",latd->Description(),latd->Data() );
                }
                token = latd->Token();
                new_token = token;
            } else {                                       // token mismatch
                parser.AssertErrorCondition(ParametersError, "mismatch %s %s",parser.GetSymbolName(symbol),parser.GetSymbolName(token));
                new_token = token;
            }

        } else {                                           // action symbol
            if ( errorLevel & 0x1 ) {
                if (!err) parser.AssertErrorCondition(Information, "trace action %s(%s,%s)",parser.GetSymbolName(symbol),latd->Description(),latd->Data());
                else      err->Printf("trace action %s(%s,%s)\n",parser.GetSymbolName(symbol),latd->Description(),latd->Data());
            }
            parser.Action( -symbol ,latd);
        }

        if ( token != new_token ) {
            if ( new_token ) {
                token = new_token;
            }
            if ( token != eofToken ) {
                continue;                                  // try this token
            }
        }

    }

    if ( token != eofToken ) {                    // input left over
        parser.AssertErrorCondition(ParametersError, "input left");
        if (err) err->Printf("input left\n");
    }

    if (latd != NULL) delete latd;

    return False;

}


