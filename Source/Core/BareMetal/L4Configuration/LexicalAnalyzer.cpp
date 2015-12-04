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
#include "TypeConversion.h"
#include "ErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static bool GetC(StreamI &stream,
                 char8 &c) {
    uint32 charSize = 1u;
    bool ret = stream.Read(&c, charSize);
    return (ret) && (charSize == 1u);
}

static void ReadCommentOneLine(StreamI &stream) {

    char8 c = ' ';
    while (c != '\n') {
        if (!GetC(stream, c)) {
            break;
        }
    }

}

static void ReadCommentMultipleLines(StreamI &stream,
                                     uint32 &lineNumber) {

    char8 c = ' ';
    bool go = true;
    while (go) {
        if (GetC(stream, c)) {
            if (c == '*') {
                if (GetC(stream, c)) {
                    if (c == '/') {
                        go = false;
                    }
                }
                else {
                    go = false;
                }
            }
            if (c == '\n') {
                lineNumber++;
            }
        }
        else {
            go = false;
        }
    }
}

static bool SkipComment(StreamI &stream,
                        StreamString& tokenString,
                        char8 &nextChar,
                        uint32 &lineNumber,
                        const char8 * const separators,
                        const char8 * const terminals,
                        char8 &separator) {

    char8 c = ' ';

    bool isEOF = false;
    // if it is a new token needs to skip separators at the beginning
    bool isNewToken = (tokenString.Size() == 0u);
    bool isComment = true;
    bool skip = true;

    // skip separators before
    while (skip) {
        if (GetC(stream, c)) {
            // exit, found a terminal!
            if (StringHelper::SearchChar(terminals, c) != NULL) {
                skip = false;
                isComment = false;
            }
            else {
                //stop loop, not a separator
                if ((StringHelper::SearchChar(separators, c) == NULL) || (!isNewToken)) {
                    skip = false;
                }
            }
        }
        // EOF!
        else {
            isEOF = true;
            isComment = false;
            skip = false;
        }
        if ((skip) && (c == '\n')) {
            lineNumber++;
        }
    }

    // could be a comment ?
    if (isComment) {
        isComment = (c == '/');
        if (isComment) {
            if (GetC(stream, c)) {
                // found double /, comment on one line
                if (c == '/') {
                    ReadCommentOneLine(stream);
                    separator = '\n';
                }
                // found /* , comment on multiple line
                else if (c == '*') {
                    ReadCommentMultipleLines(stream, lineNumber);
                    separator = '\0';
                }
                else {
                    tokenString += '/';
                    isComment = false;
                }
            }
            else {
                isComment = false;
                isEOF = true;
            }
        }
    }

    // allows to understand if a comment is found
    if (isComment) {
        nextChar = '\0';
    }
    else {
        nextChar = c;
    }

    return !isEOF;
}

bool EscapeChar(char8 &c) {
    bool ret = true;
    switch (c) {
    case ('n'): {
        c = '\n';
    }
        break;
    case ('t'): {
        c = '\t';
    }
        break;
    case ('r'): {
        c = '\r';
    }
        break;
    case ('"'): {
        c = '"';
    }
        break;
    default: {
        ret = false;
    }
    }
    return ret;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/*lint -e{429} . Justification: the allocated memory is freed by the class destructor. */
Token *LexicalAnalyzer::GetToken() {
    // delete the previous token
    if (token != NULL) {
        delete token;
    }

    if (tokenQueue.GetSize() == 0u) {
        TokenizeInput();
    }
    if (!tokenQueue.Extract(0u, token)) {
        REPORT_ERROR(ErrorManagement::FatalError, "GetToken: Failed Extract() of the token from the token stack");
    }
    return token;
}

/*lint -e{429} . Justification: the allocated memory is freed by the class destructor. */
Token *LexicalAnalyzer::PeekToken(const uint32 position) {

    uint32 queueSize = tokenQueue.GetSize();
    if (position >= queueSize) {
        TokenizeInput((position - queueSize) + 1u);
    }
    Token *peekToken = static_cast<Token*>(NULL);
    if (!tokenQueue.Peek(position, peekToken)) {
        REPORT_ERROR(ErrorManagement::FatalError, "PeekToken: Failed Peek() of the token from the token stack");
    }
    return peekToken;

}

LexicalAnalyzer::LexicalAnalyzer(StreamI &stream,
                                 const char8 * const terminalsIn,
                                 const char8 * const separatorsIn) {
    token = static_cast<Token *>(NULL);
    inputStream = &stream;
    lineNumber = 1u;
    terminals = terminalsIn;
    separators = separatorsIn;
    tokenInfo[0].Set(EOF_TOKEN, "EOF");
    tokenInfo[1].Set(STRING_TOKEN, "STRING");
    tokenInfo[2].Set(NUMBER_TOKEN, "NUMBER");
    tokenInfo[3].Set(ERROR_TOKEN, "ERROR");
    tokenInfo[4].Set(TERMINAL_TOKEN, "TERMINAL");
}

/*lint -e{1551} Justification: Memory has to be freed in the destructor.
 * No exceptions should be thrown given that the memory is managed exclusively managed by this class.". */
LexicalAnalyzer::~LexicalAnalyzer() {
    for (uint32 i = 0u; i < tokenQueue.GetSize(); i++) {
        Token *toDelete;
        if (tokenQueue.Extract((tokenQueue.GetSize() - i) - 1u, toDelete)) {
            delete toDelete;
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "~LexicalAnalyzer: Failed Extract() of the token from the token stack");
        }
    }
    if (token != NULL) {
        delete token;
    }
    inputStream = static_cast<StreamI*>(NULL);
}

/*lint -e{429} . Justification: the allocated memory is freed by the class destructor. */
void LexicalAnalyzer::TokenizeInput(const uint32 level) {
    bool ok = true;
    bool isEOF = false;

    Token* toAdd = static_cast<Token*>(NULL);

    StreamString separatorsUsed = separators.Buffer();
    StreamString terminalsUsed = terminals.Buffer();

    uint32 nTokens = 0u;

    while (nTokens < level) {
        char8 c = '\0';
        char8 terminal = '\0';
        char8 separator = '\0';
        // the token
        StreamString tokenString = "";
        // skips one or consecutive comments
        bool isTerminal = false;
        // skips one or consecutive comments and controls EOF
        while ((ok) && (c == '\0')) {
            ok = SkipComment(*inputStream, tokenString, c, lineNumber, separators.Buffer(), terminals.Buffer(), separator);
            if (separator == '\n') {
                lineNumber++;
            }
        }
        separator = '\0';
        isEOF = !ok;
        // if a '\' is found take the next to respect the special string characters
        bool escape = false;
        // it begins with "?
        bool isString1 = false;
        // take the tokenString
        while (ok) {
            if ((StringHelper::SearchChar(separatorsUsed.Buffer(), c) != NULL) && (!escape)) {
                // this means that a string is found! Read everything until another " is found
                if (isString1) {
                    tokenString += c;
                    separatorsUsed = separators;
                    terminalsUsed = terminals;
                    ok = false;
                }
                else {
                    // if something is written, exit
                    ok = (tokenString.Size() == 0u);
                }
                separator = c;
            }
            else if ((StringHelper::SearchChar(terminalsUsed.Buffer(), c) != NULL) && (!escape)) {
                terminal = c;
                isTerminal = true;
                ok = false;
            }
            else {
                if (escape) {
                    if (!EscapeChar(c)) {
                        tokenString += '\\';
                    }
                    escape = false;
                }
                else {
                    // found a string! read everything until another "
                    if (c == '"') {
                        if (tokenString.Size() == 0u) {
                            separatorsUsed = "\"";
                            terminalsUsed = "\"";
                            isString1 = true;
                        }
                    }
                    if (c == '\\') {
                        escape = true;
                    }
                }
                // if escape is true rescan!
                if (!escape) {
                    tokenString += c;
                }
            }
            if (ok) {
                if (c == '\n') {
                    lineNumber++;
                }
                if (isString1) {
                    ok = GetC(*inputStream, c);
                }
                else {
                    ok = SkipComment(*inputStream, tokenString, c, lineNumber, separators.Buffer(), terminals.Buffer(), separator);
                    if (ok) {
                        // stop if a comment is found at the end!
                        ok = (c != '\0');
                    }
                }
            }
        }
        if (tokenString.Size() > 0u) {
            char8* tokenBuffer = tokenString.BufferReference();

            // a string for sure!
            uint32 firstDigit = 0u;
            if ((tokenBuffer[0] == '+') || (tokenBuffer[0] == '-')) {
                firstDigit = 1u;
            }
            int8 zero = static_cast<int8>('0');
            bool isString2 = (((static_cast<int8>(tokenBuffer[firstDigit]) - zero) > 9) || ((static_cast<int8>(tokenBuffer[firstDigit]) - zero) < 0));

            bool converted = false;

            // a string for sure!
            if ((isString1) || (isString2)) {
                uint32 begin = 0u;
                uint32 end = StringHelper::Length(tokenBuffer) - 1u;
                if (tokenBuffer[begin] == '"') {
                    begin++;
                }
                if (isString1) {
                    if (tokenBuffer[end] == '"') {
                        tokenBuffer[end] = '\0';
                    }
                }
                /*lint -e{423} .Justification: The pointer is added to a stack and the memory is freed by the class destructor */
                toAdd = new Token(tokenInfo[STRING_TOKEN], &tokenBuffer[begin], lineNumber);
                if (!tokenQueue.Add(toAdd)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "TokenizeInput: Failed Add() of the token to the token stack");
                }
                converted = true;
            }

            if (!converted) {
                // not an integer! Try a float (number)
                float64 possibleFloat = 0.0;
                if (TypeConvert(possibleFloat, tokenBuffer)) {
                    /*lint -e{423} .Justification: The pointer is added to a stack and the memory is freed by the class destructor */
                    toAdd = new Token(tokenInfo[NUMBER_TOKEN], &tokenBuffer[0], lineNumber);
                    if (!tokenQueue.Add(toAdd)) {
                        REPORT_ERROR(ErrorManagement::FatalError, "TokenizeInput: Failed Add() of the token to the token stack");
                    }
                    converted = true;
                }
            }

            // error!
            if (!converted) {
                /*lint -e{423} .Justification: The pointer is added to a stack and the memory is freed by the class destructor */
                toAdd = new Token(tokenInfo[ERROR_TOKEN], "", lineNumber);
                if (!tokenQueue.Add(toAdd)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "TokenizeInput: Failed Add() of the token to the token stack");
                }
            }
        }

        if (isTerminal) {
            char8 terminalBuffer[2] = { terminal, '\0' };
            /*lint -e{423} .Justification: The pointer is added to a stack and the memory is freed by the class destructor */
            toAdd = new Token(tokenInfo[TERMINAL_TOKEN], &terminalBuffer[0], lineNumber);
            if (!tokenQueue.Add(toAdd)) {
                REPORT_ERROR(ErrorManagement::FatalError, "TokenizeInput: Failed Add() of the token to the token stack");
            }
        }
        if (isEOF) {
            /*lint -e{423} .Justification: The pointer is added to a stack and the memory is freed by the class destructor */
            toAdd = new Token(tokenInfo[EOF_TOKEN], "", lineNumber);
            if (!tokenQueue.Add(toAdd)) {
                REPORT_ERROR(ErrorManagement::FatalError, "TokenizeInput: Failed Add() of the token to the token stack");
            }
        }

        // if a newline is the separator add it for the next token
        if (separator == '\n') {
            lineNumber++;
        }
        nTokens++;
    }
}

}
