/**
 * @file LexicalAnalyzer.cpp
 * @brief Source file for class LexicalAnalyzer
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

 * @details This source file contains the definition of all the methods for
 * the class LexicalAnalyzer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "LexicalAnalyzer.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/** indicates EOF */
static const uint32 EOF = 0u;

/** indicates an identifier or a string */
static const uint32 STRING = 1u;

/** indicates an integer */
static const uint32 SIGNED = 2u;

/** indicates an integer */
static const uint32 UNSIGNED = 3u;

/** indicates a float */
static const uint32 FLOAT = 4u;

/** indicates a wrongly constructed element */
static const uint32 ERROR = 5u;

/** indicates an element that is a token on its own */
static const uint32 TERMINAL = 6u;

/** indicates an element that separates parts of the phrase */
static const uint32 SEPARATOR = 7u;

//By default parse numbers
bool LexicalAnalyzer::parseNumbers = true;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

LexicalAnalyzer::LexicalAnalyzer() {
    Reset();
    terminal = '\0';
    isTerminal = false;
    tokenInfo[0].Set(EOF, "EOF", 0u);
    tokenInfo[1].Set(STRING, "STRING", sizeof(const char8*));
    tokenInfo[2].Set(SIGNED, "SIGNED", sizeof(int64));
    tokenInfo[3].Set(UNSIGNED, "UNSIGNED", sizeof(uint64));
    tokenInfo[4].Set(FLOAT, "FLOAT", sizeof(float64));
    tokenInfo[5].Set(ERROR, "ERROR", 0u);
    tokenInfo[5].Set(TERMINAL, "TERMINAL", 1u);
}

LexicalAnalyzer::~LexicalAnalyzer() {
    Reset();
}

static void ReadCommentOneLine(StreamI &stream) {

    char8 c = ' ';
    uint32 charSize = 1u;
    while (c != '\n') {
        stream.Read(&c, charSize);
    }

}

static void ReadCommentMultipleLines(StreamI &stream) {

    char8 c = ' ';
    uint32 charSize = 1u;
    bool go = true;
    while (go) {
        stream.Read(&c, charSize);
        if (c == '*') {
            stream.Read(&c, charSize);
            if (c == '/') {
                go = false;
            }
        }
    }
}

Token *LexicalAnalyzer::GetToken(StreamI &stream) {
    char8 c = ' ';
    uint32 charSize = 1u;

    Reset();
    if (isTerminal) {
        token = new Token(tokenInfo[TERMINAL], static_cast<void *>(&terminal));
        isTerminal = false;
    }

    // the token
    StreamString tokenString = "";

    bool ok = true;

    // could be a comment ?
    if (stream.Read(&c, charSize)) {
        if (c == '/') {
            if (stream.Read(&c, charSize)) {
                // found double /, comment on one line
                if (c == '/') {
                    ReadCommentOneLine(stream);
                }
                // found /* , comment on single line
                else if (c == '*') {
                    ReadCommentMultipleLines(stream);
                }
                else {
                    tokenString += '/';
                }
            }
        }
    }

    bool isEOF = false;
    // take the tokenString
    while (ok) {
        if (StringHelper::SearchChar(separators.Buffer(), c)) {
            // if something is written, exit
            ok = (tokenString.Size() == 0u);
        }
        else if (StringHelper::SearchChar(terminals.Buffer(), c)) {
            terminal = c;
            isTerminal = true;
            ok = false;
        }
        else {
            tokenString += c;
        }
        if (!stream.Read(&c, charSize)) {
            isEOF = (tokenString.Size() == 0u);
            ok = false;
        }
    }

    const char8* tokenBuffer = tokenString.Buffer();
    bool converted = false;

    if (tokenString.Size() > 0u) {
        // a string for sure!
        if (tokenBuffer[0] != '"') {
            if (!converted) {
                // if negative could be a signed integer
                if (tokenBuffer[0] == '-') {
                    int64 possibleSignedInteger = 0;
                    if (TypeConvert(possibleSignedInteger, tokenBuffer)) {
                        token = new Token(tokenInfo[SIGNED], reinterpret_cast<void*>(&possibleSignedInteger));
                        converted = true;
                    }
                }
                // if positive could be an unsigned integer
                else {
                    uint64 possibleUnSignedInteger = 0u;
                    if (TypeConvert(possibleUnSignedInteger, tokenBuffer)) {
                        token = new Token(tokenInfo[UNSIGNED], reinterpret_cast<void*>(&possibleUnSignedInteger));
                        converted = true;
                    }
                }
            }

            // not an integer! Try a float
            if (!converted) {
                float64 possibleFloat = 0.0;
                if (TypeConvert(possibleFloat, tokenBuffer)) {
                    token = new Token(tokenInfo[FLOAT], reinterpret_cast<void*>(&possibleFloat));
                    converted = true;
                }
            }
        }
        // not an integer neither a float! Cast it to string
        if (!converted) {
            uint32 begin = 0u;
            uint32 end = StringHelper::Length(tokenBuffer) - 1u;
            if (tokenBuffer[begin] == '"') {
                begin++;
            }
            if (tokenBuffer[end] == '"') {
                tokenBuffer[end] = '\0';
                end--;
            }
            uint32 size = (end - begin) + 2u;
            char8 *possibleString = HeapManager::Malloc(size);

            if (StringHelper::Copy(possibleString, &tokenBuffer[begin])) {
                token = new Token(tokenInfo[STRING], reinterpret_cast<void*>(&possibleString));
                converted = true;
            }
        }
    }
    else {
        if (isTerminal) {
            token = new Token(tokenInfo[TERMINAL], static_cast<void *>(&terminal));
            isTerminal = false;
        }
        if (isEOF) {
            token = new Token(tokenInfo[EOF], NULL);
        }
    }

    return token;
}
/*
 bool LexicalAnalyzer::TokenizeInput(StreamI &stream) {
 int32 status = statusNormal;
 bool escape = false;
 int32 c = 0;
 char8 cc = ' ';
 StreamString tokenValue;
 uint32 charSize = 1u;

 // read the charcter
 while (stream.Read(&cc, charSize)) {
 if (cc == '\n') {
 lineCounter++;
 }
 // need to rescan this character
 bool repeat = true;

 while (repeat) {
 repeat = false;
 if (escape) {
 switch (cc) {
 case '\\':
 c = '\\';
 break;
 case 'n':
 c = '\n';
 break;
 case 't':
 c = '\t';
 break;
 case 'r':
 c = '\r';
 break;
 default: {
 REPORT_ERROR(ErrorManagement::FatalError, "LexicalAnalyzer::TokenizeInput:character sequence not known");
 return false;
 }
 }
 //            cc = (char8)-1;  // Fabio...
 if (c > 0) {
 tokenValue += c;
 escape = false;
 }
 }
 // escape
 else if (cc == '\\') {
 escape = true;
 }
 else
 // normal!
 {
 c = cc;
 // detect terminals
 if (StringHelper::SearchChar(terminals.Buffer(), cc)) {
 c = LATV_Terminal;
 }
 else
 // detect separator
 if (StringHelper::SearchChar(separators.Buffer(), cc)) {
 c = LATV_Separator;
 }
 switch (status) {

 // normal part of the phrase
 case statusNormal: {
 switch (c) {
 // maybe a comment!
 case '/': {
 status = statusComment0;
 }
 break;
 // a string!
 case '"': {
 if (tokenValue.Size() > 0u) {
 AddToken(new Token(LATV_Ident, tokenValue.Buffer(), lineCounter));
 AddToken(new Token(LATV_Error, "ident cannot include \"", lineCounter));
 return true;
 }
 status = statusString;
 }
 break;
 case LATV_Separator: {
 // if there is a separator and the token is not empty, add it to the queue.
 if (tokenValue.Size() > 0u) {
 AddToken(new Token(LATV_Ident, tokenValue.Buffer(), lineCounter));
 return true;
 }
 }
 break;
 case LATV_Terminal: {
 // if there is a terminal and the token is not empty, add it to the queue.
 if (tokenValue.Size() > 0u) {
 AddToken(new Token(LATV_Ident, tokenValue.Buffer(), lineCounter));
 }
 AddToken(new Token(cc, lineCounter));
 return true;
 }
 break;
 default: {
 if (tokenValue.Size() == 0u) {
 // a hexadecimal!
 if (c == '0') {
 status = statusHex;
 }
 // if a '-' or '0-9' and we parse numbers the status is number
 else if ((IsNumberStart(c)) && (parseNumbers)) {
 status = statusNumber;
 }
 }
 tokenValue += c;
 }
 }

 }
 break;
 // terminates through the number case
 case statusHex: {
 if (tokenValue.Size() == 1) {
 if ((c != 'X') && (c != 'x')) {
 status = statusNumber;
 }
 else {
 tokenValue += c;
 break;
 }
 }
 else {
 if (cc == '.') {
 AddToken(new Token(LATV_Error, "HEX number does not have '.'", lineCounter));
 return true;
 }
 else if (IsHex(c)) {
 if (tokenValue.Size() < 19) {
 tokenValue += c;
 break;
 }
 else {
 AddToken(new Token(LATV_Error, "too many figures in HEX number ", lineCounter));
 return true;
 }
 }
 }
 }
 case statusNumber: {
 // a float!
 if (cc == '.') {  // HEX manages this separately
 status = statusFloat;
 }
 else if (toupper(c) == 'E') { // HEX manages this separately
 status = statusExp;
 }
 else if (c == LATV_Terminal) {
 AddToken(new Token(LATV_Number, tokenValue.Buffer(), lineCounter));
 AddToken(new Token(cc, lineCounter));
 return true;
 }
 else if (c == LATV_Separator) {
 AddToken(new Token(LATV_Number, tokenValue.Buffer(), lineCounter));
 return true;
 }
 else if (!IsNumber(c)) { // HEX manages this separately
 AddToken(new Token(LATV_Number, tokenValue.Buffer(), lineCounter));
 AddToken(new Token(LATV_Error, "number terminated by not a terminator or terminal", lineCounter));
 return true;
 }

 tokenValue += cc;
 }
 break;
 case statusFloat: {
 if (toupper(c) == 'E') {
 status = statusExp;
 }
 else if (c == LATV_Terminal) {
 AddToken(new Token(LATV_Float, tokenValue.Buffer(), lineCounter));
 AddToken(new Token(cc, lineCounter));
 return true;
 }
 else if (c == LATV_Separator) {
 AddToken(new Token(LATV_Float, tokenValue.Buffer(), lineCounter));
 return true;
 }
 else if (!IsNumber(c)) {
 AddToken(new Token(LATV_Float, tokenValue.Buffer(), lineCounter));
 AddToken(new Token(LATV_Error, "number terminated by not a terminator or terminal", lineCounter));
 return true;
 }
 tokenValue += cc;
 }
 break;
 case statusExp: {
 if ((cc != '-') && (cc != '+') && (!IsNumber(cc))) {
 AddToken(new Token(LATV_Error, "exponent of number terminated prematurely", lineCounter));
 return true;
 }
 status = statusExp2;
 tokenValue += cc;
 }
 break;
 case statusExp2: {
 if (c == LATV_Terminal) {
 AddToken(new Token(LATV_Float, tokenValue.Buffer(), lineCounter));
 AddToken(new Token(cc, lineCounter));
 return true;
 }
 else if (c == LATV_Separator) {
 AddToken(new Token(LATV_Float, tokenValue.Buffer(), lineCounter));
 return true;
 }
 else if (!IsNumber(c)) {
 AddToken(new Token(LATV_Float, tokenValue.Buffer(), lineCounter));
 AddToken(new Token(LATV_Error, "number terminated by not a terminator or terminal", lineCounter));
 return true;
 }
 tokenValue += cc;
 }
 break;
 case statusString: {
 switch (cc) {
 case '"': {
 AddToken(new Token(LATV_Ident, tokenValue.Buffer(), lineCounter));
 return true;
 }
 break;
 default:
 tokenValue += cc;
 }
 }
 break;
 case statusComment0: {
 switch (cc) {
 case '/': {
 status = statusLineComment;
 }
 break;
 case '*': {
 status = statusComment;
 }
 break;
 default: {
 tokenValue += '/';
 //tokenValue += cc;
 status = statusNormal;
 repeat = true;
 }
 }
 }
 break;
 case statusLineComment: {
 switch (cc) {
 case '\n':
 case '\r': {
 status = statusNormal;
 }
 break;
 }
 }
 break;
 case statusComment: {
 switch (cc) {
 case '*': {
 status = statusComment3;
 }
 break;
 }
 }
 break;
 case statusComment3: {
 switch (cc) {
 case '/': {
 status = statusNormal;
 }
 break;
 default: {
 status = statusComment;
 }
 break;
 }
 }
 break;
 default: {
 }
 break;
 }
 }
 }
 }

 if (tokenValue.Size() > 0u) {
 switch (status) {
 case statusNormal:
 case statusString: {
 AddToken(new Token(LATV_Ident, tokenValue.Buffer(), lineCounter));
 }
 break;
 case statusNumber: {
 }
 break;
 }
 }

 AddToken(new Token(LATV_EOF, "EOF", lineCounter));
 return true;
 }*/

void LexicalAnalyzer::SetParseNumbers(bool parseNumbersIn) {
    parseNumbers = parseNumbersIn;
}

void LexicalAnalyzer::Reset() {

    if(token->GetId()==STRING){
        if(!HeapManager::Free(reinterpret_cast<void* &>(token->GetData()))){
            //TODO
        }
    }

/*Token *tokenData;
 for (uint32 lastElementIndex = tokenQueue.GetSize(); lastElementIndex > 0u; lastElementIndex--) {
 if (tokenQueue.Extract((void *) &tokenData), lastElementIndex - 1u) {
 if (tokenData != NULL) {
 delete tokenData;
 }
 }
 }*/
}

void LexicalAnalyzer::AddSeparators(const char8 *s) {
separators += s;
}

void LexicalAnalyzer::AddTerminals(const char8 *s) {
terminals += s;
}

bool LexicalAnalyzer::ChangeTokenCode(const char8 *tokenName,
                                      int32 token) {

bool ret = false;
if (tokenName != NULL) {
    if (tokenName[0] != '\0') {
        for (uint32 index = 0u; (index < 8u) && (tokenInfo[index].GetDescription() != NULL); index++) {
            if (StringHelper::Compare(tokenInfo[index].GetDescription(), tokenName) == 0) {
                tokenInfo[index].Set(token, tokenInfo[index].GetDescription());
            }
        }
        ret = true;
    }
}
return ret;
}

int32 LexicalAnalyzer::GetTokenValue(const char8 *tokenName) {

int32 ret = -1;
if (tokenName != NULL) {
    if (tokenName[0] != '\0') {

        for (uint32 index = 0u; (index < 8u) && (tokenInfo[index].GetDescription() != NULL); index++) {
            if (StringHelper::Compare(tokenInfo[index].GetDescription(), tokenName) == 0) {
                ret = tokenInfo[index].GetTokenId();
            }
        }
    }
}
return ret;
}

}
