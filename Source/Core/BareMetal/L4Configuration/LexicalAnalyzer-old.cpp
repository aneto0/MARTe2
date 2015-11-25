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
 * $Id: LexicalAnalyzer.cpp 3 2012-01-15 16:26:07Z aneto $
 *
**/

#include "LexicalAnalyzer.h"

//By default parse numbers
bool LexicalAnalyzer::parseNumbers   = True;

static const int32 statusNormal      = 0x0001;
static const int32 statusEsc         = 0x1000;
static const int32 statusString      = 0x0002;
static const int32 statusComment0    = 0x0003;
static const int32 statusComment     = 0x0004;
static const int32 statusLineComment = 0x0005;
static const int32 statusComment3    = 0x0006;
static const int32 statusNumber      = 0x0007;
static const int32 statusFloat       = 0x0008;
static const int32 statusExp         = 0x0009;
static const int32 statusExp2        = 0x000A;
static const int32 statusHex         = 0x000B;

static inline bool IsNumber(char c){
    return ((c>='0') && (c<='9'));
}
static inline bool IsHex(char c){
    if ((c>='A') && (c<='F')) return True;
    if ((c>='a') && (c<='f')) return True;
    return IsNumber(c);
}
static inline bool IsNumberStart(char c){
    return (c=='-') || IsNumber(c);
}

bool LATokenizeInput(LexicalAnalyzer &la,StreamInterface &stream){
    int     status  =   statusNormal;
    bool    escape  =   False;
    int     c = 0;
    char    cc = ' ';
    FString tokenValue;

    while (stream.GetC(cc)){
        if (cc=='\n') la.lineCounter++;
        // need to rescan this character
        bool repeat;
        repeat = True;
        
        while(repeat){
	        repeat = False;
	        if (escape){
	            switch(cc){
	                case '\\': c = '\\'; break;
	                case 'n': c = '\n'; break;
	                case 't': c = '\t'; break;
	                case 'r': c = '\r'; break;
	                default:{
	                    CStaticAssertErrorCondition(FatalError,"LexicalAnalyzer::TokenizeInput:character sequence \\%c is not known",cc);
	                    return False;
	                }
	            }
	//            cc = (char)-1;  // Fabio...
	            if (c > 0){
	                tokenValue += c;
	                escape = False;
	            }
	        } else
	        // not escape
	        if (cc == '\\'){
	            escape = True;
	        } else
	        // normal!
	        {
	            c = cc;
	            // detect terminals
	            if (strchr(la.terminals.Buffer(),c)){
	                c = LATV_Terminal;
	            } else
	            // detect separator
	            if (strchr(la.separators.Buffer(),c)){
	                c = LATV_Separator;
	            }
	            switch(status){
	
	                // normal part of the phrase
	                case statusNormal:{
	                    switch(c){
	                        case '/':{
	                            status = statusComment0;
	                        } break;
	                        case '"':{
	                            if (tokenValue.Size()>0) {
	                                la.AddToken(
	                                    new LA_TokenData(la.tokenInfo[LATV_Ident - 0x100],
	                                                     tokenValue.Buffer(),la.lineCounter));
	                                la.AddToken(
	                                    new LA_TokenData(la.tokenInfo[LATV_Error - 0x100],
	                                                     "ident cannot include \"",la.lineCounter));
	                                return True;
	                            }
	                            status = statusString;
	                        } break;
	                        case LATV_Separator:{
	                            if (tokenValue.Size()>0) {
	                                la.AddToken(
	                                    new LA_TokenData(la.tokenInfo[LATV_Ident - 0x100],
	                                                     tokenValue.Buffer(),la.lineCounter));
	                                return True;
	                            }
	                        } break;
	                        case LATV_Terminal:{
	                            if (tokenValue.Size()>0) {
	                                la.AddToken(
	                                    new LA_TokenData(la.tokenInfo[LATV_Ident - 0x100],
	                                                     tokenValue.Buffer(),la.lineCounter));
	                            }
	                            la.AddToken(
	                                new LA_TokenData(cc,la.lineCounter));
	                            return True;
	                        } break;
	                        default:{
	                            if (tokenValue.Size() == 0){
	                                if (c == '0'){
	                                    status = statusHex;
	                                } else
	                                if (IsNumberStart(c) && la.parseNumbers){
	                                    status = statusNumber;
	                                }
	                            }
	                            tokenValue += c;
	                        }
	                    }
	
	                } break;
	                // terminates through the number case
	                case statusHex:{
	                    if (tokenValue.Size() == 1){
	                       if ((c != 'X') && (c != 'x')){
	                            status = statusNumber;
	                        } else {
	                            tokenValue += c;
	                            break;
	                        }
	                    } else {
	                        if (cc == '.'){
	                            la.AddToken(
	                                new LA_TokenData(la.tokenInfo[LATV_Error - 0x100],
	                                             "HEX number does not have '.'",la.lineCounter));
	                            return True;
	                        } else
	                        if (IsHex(c)){
	                            if (tokenValue.Size() < 19){
	                                tokenValue += c;
	                                break;
	                            } else {
	                                la.AddToken(
	                                    new LA_TokenData(la.tokenInfo[LATV_Error - 0x100],
	                                                 "too many figures in HEX number ",la.lineCounter));
	                                return True;
	                            }
	                        }
	                    }
	                }
	                case statusNumber:{
	                    if (cc == '.'){  // HEX manages this separately
	                        status = statusFloat;
	                    } else
	                    if (toupper(c) == 'E'){ // HEX manages this separately
	                        status = statusExp;
	                    } else
	                    if (c == LATV_Terminal){
	                        la.AddToken(
	                            new LA_TokenData(la.tokenInfo[LATV_Number - 0x100],
	                                             tokenValue.Buffer(),la.lineCounter));
	                        la.AddToken(
	                            new LA_TokenData(cc,la.lineCounter));
	                        return True;
	                    } else
	                    if (c == LATV_Separator){
	                        la.AddToken(
	                            new LA_TokenData(la.tokenInfo[LATV_Number - 0x100],
	                                             tokenValue.Buffer(),la.lineCounter));
	                        return True;
	                    } else
	                    if (!IsNumber(c)){ // HEX manages this separately
	                        la.AddToken(
	                            new LA_TokenData(la.tokenInfo[LATV_Number - 0x100],
	                                             tokenValue.Buffer(),la.lineCounter));
	                        la.AddToken(
	                            new LA_TokenData(la.tokenInfo[LATV_Error - 0x100],
	                                             "number terminated by not a terminator or terminal",la.lineCounter));
	                        return True;
	                    }
	
	                    tokenValue += cc;
	                } break;
	                case statusFloat:{
	                    if (toupper(c) == 'E'){
	                        status = statusExp;
	                    } else
	                    if (c == LATV_Terminal){
	                        la.AddToken(
	                            new LA_TokenData(la.tokenInfo[LATV_Float - 0x100],
	                                             tokenValue.Buffer(),la.lineCounter));
	                        la.AddToken(
	                            new LA_TokenData(cc,la.lineCounter));
	                        return True;
	                    } else
	                    if (c == LATV_Separator){
	                        la.AddToken(
	                            new LA_TokenData(la.tokenInfo[LATV_Float - 0x100],
	                                             tokenValue.Buffer(),la.lineCounter));
	                        return True;
	                    } else
	                    if (!IsNumber(c)){
	                        la.AddToken(
	                            new LA_TokenData(la.tokenInfo[LATV_Float - 0x100],
	                                             tokenValue.Buffer(),la.lineCounter));
	                        la.AddToken(
	                            new LA_TokenData(la.tokenInfo[LATV_Error - 0x100],
	                                             "number terminated by not a terminator or terminal",la.lineCounter));
	                        return True;
	                    }
	                    tokenValue += cc;
	                } break;
	                case statusExp:{
	                    if ((cc != '-') && ( cc != '+') && (!IsNumber(cc))){
	                        la.AddToken(
	                            new LA_TokenData(la.tokenInfo[LATV_Error - 0x100],
	                                             "exponent of number terminated prematurely",la.lineCounter));
	                        return True;
	                    }
	                    status = statusExp2;
	                    tokenValue += cc;
	                } break;
	                case statusExp2:{
	                    if (c == LATV_Terminal){
	                        la.AddToken(
	                            new LA_TokenData(la.tokenInfo[LATV_Float - 0x100],
	                                             tokenValue.Buffer(),la.lineCounter));
	                        la.AddToken(
	                            new LA_TokenData(cc,la.lineCounter));
	                        return True;
	                    } else
	                    if (c == LATV_Separator){
	                        la.AddToken(
	                            new LA_TokenData(la.tokenInfo[LATV_Float - 0x100],
	                                             tokenValue.Buffer(),la.lineCounter));
	                        return True;
	                    } else
	                    if (!IsNumber(c)){
	                        la.AddToken(
	                            new LA_TokenData(la.tokenInfo[LATV_Float - 0x100],
	                                             tokenValue.Buffer(),la.lineCounter));
	                        la.AddToken(
	                            new LA_TokenData(la.tokenInfo[LATV_Error - 0x100],
	                                             "number terminated by not a terminator or terminal",la.lineCounter));
	                        return True;
	                    }
	                    tokenValue += cc;
	                } break;
	                case statusString:{
	                    switch(cc){
	                        case '"':{
	                            la.AddToken(
	                                new LA_TokenData(la.tokenInfo[LATV_Ident - 0x100],
	                                                 tokenValue.Buffer(),la.lineCounter));
	                            return True;
	                        } break;
	                        default:
	                            tokenValue += cc;
	                    }
	                } break;
	                case statusComment0:{
	                    switch(cc){
	                        case '/':{
	                            status = statusLineComment;
	                        } break;
	                        case '*':{
	                            status = statusComment;
	                        } break;
	                        default:{                            
	                            tokenValue += '/';
	                            //tokenValue += cc;
	                            status = statusNormal;
	                            repeat = True;
	                        }
	                    }
	                } break;
	                case statusLineComment:{
	                    switch(cc){
	                        case '\n':
	                        case '\r':{
	                            status = statusNormal;
	                        } break;
	                    }
	                } break;
	                case statusComment:{
	                    switch(cc){
	                        case '*':{
	                            status = statusComment3;
	                        } break;
	                    }
	                } break;
	                case statusComment3:{
	                    switch(cc){
	                        case '/':{
	                            status = statusNormal;
	                        } break;
	                        default:{
	                            status = statusComment;
	                        } break;
	                    }
	                } break;
	                default:{
	                } break;
	            }
	        }
        }
    }

    if (tokenValue.Size()>0){
        switch(status){
            case statusNormal:
            case statusString:{
                la.AddToken(
                    new LA_TokenData(la.tokenInfo[LATV_Ident - 0x100],
                                     tokenValue.Buffer(),la.lineCounter));
            } break;
            case statusNumber:{
            } break;
        }
    }

    la.AddToken(
        new LA_TokenData(la.tokenInfo[LATV_EOF - 0x100],
                         "EOF",la.lineCounter));
    return True;
}

