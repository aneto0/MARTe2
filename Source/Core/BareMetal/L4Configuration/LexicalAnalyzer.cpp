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

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "LexicalAnalyzer.h"
#include "StreamString.h"
#include "TypeConversion.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Gets the next character from the stream.
 * @param[in] stream is the stream of characters to be read.
 * @param[out] c is the read character.
 * @return false if EOF, true otherwise.
 */
static bool GetC(StreamI &stream,
                 char8 &c) {
    uint32 charSize = 1u;
    bool ret = stream.Read(&c, charSize);
    return (ret) && (charSize == 1u);
}

/**
 * @brief Reads the comment on single lines.
 * @param[in] stream is the stream of characters to be read.
 */
static void ReadCommentOneLine(StreamI &stream) {

    char8 c = ' ';
    while (c != '\n') {
        if (!GetC(stream, c)) {
            break;
        }
    }
}

/**
 * @brief Reads the comment on multiple lines.
 * @param[in] stream is the stream of characters to be read.
 * @param[out] lineNumber is the token line number.
 * @param[in] multipleLineEnd C-string containing the pattern
 * that it is used for marking the end of a multiple line comment.
 */
static void ReadCommentMultipleLines(StreamI &stream,
                                     uint32 &lineNumber,
                                     const char8 * const multipleLineEnd) {

    char8 c = ' ';
    uint32 size = StringHelper::Length(multipleLineEnd);
    char8 buffer[16];

    // read the next characters to match the end of comment
    for (uint32 i = 0u; i < size; i++) {
        if (!GetC(stream, c)) {
            break;
        }
        if (c == '\n') {
            lineNumber++;
        }
        buffer[i] = c;
    }
    buffer[size] = '\0';

    while (StringHelper::Compare(&buffer[0], multipleLineEnd) != 0) {

        if (!GetC(stream, c)) {
            break;
        }
        for (uint32 i = 1u; i < size; i++) {
            buffer[i - 1u] = buffer[i];
        }
        if (c == '\n') {
            lineNumber++;
        }
        buffer[size - 1u] = c;
        buffer[size] = '\0';

    }
}

/**
 * @brief Skips the comments in the stream.
 * @param[in] stream is the stream of characters to be read.
 * @param[out] buffer contains the data read from the stream.
 * @param[out] bufferSize the actual size of the data in buffer
 * @param[out] lineNumber is the token line number.
 * @param[in] separators is the separator characters list.
 * @param[in] oneLineBegin C-string containing the pattern that
 * it is used for marking the beginning of a single line comment.
 * @param[in] multipleLineBegin C-string containing the pattern that
 * it is used for marking the beginning of a multiple line comment.
 * @param[in] multipleLineEnd C-string containing the pattern that
 * it is used for marking the end of a multiple line comment.
 * @param[out] separator returns the separator char found at the end of the comment.
 * @param[in] isNewToken specifies if the separators at the beginning must be skipped or not.
 * @return false if EOF, true otherwise.
 */
static bool SkipComment(StreamI &stream,
                        char8 * const buffer,
                        uint32 &bufferSize,
                        uint32 &lineNumber,
                        const char8 * const separators,
                        const char8 * const oneLineBegin,
                        const char8 * const multipleLineBegin,
                        const char8 * const multipleLineEnd,
                        char8 &separator,
                        const bool isNewToken) {

    char8 c = '\0';
    bufferSize = 0u;
    buffer[0] = '\0';
    separator = '\0';
    bool isEOF = false;
    // if it is a new token needs to skip separators at the beginning
    bool isComment = true;
    bool skip = true;

    // skip separators before
    while (skip) {
        if (GetC(stream, c)) {
            //stop loop, not a separator
            if (StringHelper::SearchChar(separators, c) == NULL) {
                skip = false;
            }
            else {
                // if it is a separator at the end exit without doing nothing else
                if (!isNewToken) {
                    isComment=false;
                    separator=c;
                }
            }
            // in any case do not skip if it is not a new token
            if(!isNewToken) {
                skip=false;
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
        uint32 i = 0u;
        // check if it is a one line comment
        while (isComment) {
            if (oneLineBegin[i] != '\0') {
                isComment = (c == oneLineBegin[i]);
                buffer[i] = c;
                if (isComment) {
                    if (!GetC(stream, c)) {
                        isComment = false;
                        isEOF = true;
                        c = '\0';
                    }
                }
                i++;
            }
            else {
                buffer[i] = c;
                isComment = (i != 0u);
                i++;
                break;
            }
        }
        // add the terminal
        buffer[i] = '\0';
        if (isComment) {
            // comment on one line
            ReadCommentOneLine(stream);
            buffer[0] = '\0';
            separator = '\n';
        }
        else {
            isComment = true;
            uint32 bufferSize1 = i;
            i = 0u;
            // check if it is a multiple line comment
            while (isComment) {
                if (multipleLineBegin[i] != '\0') {
                    if (i < bufferSize1) {
                        c = buffer[i];
                    }
                    else {
                        if (!GetC(stream, c)) {
                            isComment = false;
                            isEOF = true;
                            c = '\0';
                        }
                    }
                    if (isComment) {
                        isComment = (c == multipleLineBegin[i]);
                        buffer[i] = c;
                        i++;
                    }
                }
                else {
                    isComment = (i != 0u);
                    break;
                }
            }

            if (isComment) {
                // comment on multiple line
                ReadCommentMultipleLines(stream, lineNumber, multipleLineEnd);
                buffer[0] = '\0';
            }
            else {
                if (i >= bufferSize1) {
                    buffer[i] = '\0';
                    bufferSize = i;
                }
                else {
                    buffer[bufferSize1] = '\0';
                    bufferSize = bufferSize1;
                }
            }
        }
    }

    return !isEOF;
}

/**
 * @brief Builds the escape character in case when '\' is read.
 * @param[out] c is the character in output.
 * @return true if the character in input matches a known escape sequence, false otherwise.
 */
static bool EscapeChar(char8 &c) {
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
    case ('\\'): {
        c = '\\';
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
Token * LexicalAnalyzer::GetToken() {
    // delete the previous token
    if (token != NULL) {
        delete token;
    }
    TokenizeInput();
    if (!tokenQueue.Extract(0u, token)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "StaticList<Token *>: Failed Extract() of the token from the token stack");
    }
    return token;
}

/*lint -e{429} . Justification: the allocated memory is freed by the class destructor. */
Token *LexicalAnalyzer::PeekToken(const uint32 position) {

    TokenizeInput(position);
    Token *peekToken = static_cast<Token*>(NULL);
    if (!tokenQueue.Peek(position, peekToken)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "StaticList<Token *>: Failed Peek() of the token from the token stack");
    }
    return peekToken;

}

LexicalAnalyzer::LexicalAnalyzer(StreamI &stream,
                                 const char8 * const terminalsIn,
                                 const char8 * const separatorsIn,
                                 const char8 * const oneLineCommentBeginIn,
                                 const char8 * const multipleLineCommentBeginIn,
                                 const char8 * const multipleLineCommentEndIn) {
    token = static_cast<Token *>(NULL);
    inputStream = &stream;
    lineNumber = 1u;
    terminals = terminalsIn;
    separators = separatorsIn;
    oneLineCommentBegin=oneLineCommentBeginIn;
    multipleLineCommentBegin=multipleLineCommentBeginIn;
    multipleLineCommentEnd= multipleLineCommentEndIn;
    keywords = "";
    tokenInfo[0].Set(EOF_TOKEN, "EOF");
    tokenInfo[1].Set(STRING_TOKEN, "STRING");
    tokenInfo[2].Set(NUMBER_TOKEN, "NUMBER");
    tokenInfo[3].Set(ERROR_TOKEN, "ERROR");
    tokenInfo[4].Set(TERMINAL_TOKEN, "TERMINAL");
}

LexicalAnalyzer::LexicalAnalyzer(StreamI &stream,
                                 const char8 * const terminalsIn,
                                 const char8 * const separatorsIn,
                                 const char8 * const oneLineCommentBeginIn,
                                 const char8 * const multipleLineCommentBeginIn,
                                 const char8 * const multipleLineCommentEndIn,
                                 const char8 * const keywordsIn) {
    token = static_cast<Token *>(NULL);
    inputStream = &stream;
    lineNumber = 1u;
    terminals = terminalsIn;
    separators = separatorsIn;
    oneLineCommentBegin=oneLineCommentBeginIn;
    multipleLineCommentBegin=multipleLineCommentBeginIn;
    multipleLineCommentEnd= multipleLineCommentEndIn;
    keywords = keywordsIn;
    tokenInfo[0].Set(EOF_TOKEN, "EOF");
    tokenInfo[1].Set(STRING_TOKEN, "STRING");
    tokenInfo[2].Set(NUMBER_TOKEN, "NUMBER");
    tokenInfo[3].Set(ERROR_TOKEN, "ERROR");
    tokenInfo[4].Set(TERMINAL_TOKEN, "TERMINAL");
}

/*lint -e{1551} Justification: Memory has to be freed in the destructor.
 * No exceptions should be thrown given that the memory is managed exclusively managed by this class.". */
LexicalAnalyzer::~LexicalAnalyzer() {
    uint32 queueSize=tokenQueue.GetSize();
    for (uint32 i = 0u; i < queueSize; i++) {
        Token *toDelete;
        if (tokenQueue.Extract((queueSize - i) - 1u, toDelete)) {
            delete toDelete;
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "StaticList<Token *>: Failed Extract() of the token from the token stack");
        }
    }
    if (token != NULL) {
        delete token;
    }
    inputStream = static_cast<StreamI*>(NULL);
}

/*lint -e{429} . Justification: the allocated memory is freed by the class destructor. */
void LexicalAnalyzer::AddToken(char8 * const tokenBuffer,
                               const bool isString) {


    if (StringHelper::Length(tokenBuffer) > 0u) {

        // a string for sure!
        uint32 firstDigit = 0u;
        if ((tokenBuffer[0] == '+') || (tokenBuffer[0] == '-')) {
            firstDigit = 1u;
        }
        int8 zero = static_cast<int8>('0');
        bool isString2 = (((static_cast<int8>(tokenBuffer[firstDigit]) - zero) > 9) || ((static_cast<int8>(tokenBuffer[firstDigit]) - zero) < 0));

        bool converted = false;

        // a string for sure!
        if ((isString) || (isString2)) {
            uint32 begin = 0u;
            uint32 end = StringHelper::Length(tokenBuffer) - 1u;
            if (tokenBuffer[begin] == '"') {
                begin++;
            }
            if (isString) {
                if (tokenBuffer[end] == '"') {
                    tokenBuffer[end] = '\0';
                }
            }
            /*lint -e{423} .Justification: The pointer is added to a stack and the memory is freed by the class destructor */
            Token *toAdd = new Token(tokenInfo[STRING_TOKEN], &tokenBuffer[begin], lineNumber);
            if (!tokenQueue.Add(toAdd)) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "StaticList<Token *>: Failed Add() of the token to the token stack");
            }
            converted = true;
        }

        if (!converted) {
            // not an integer! Try a float (number)
            float64 possibleFloat = 0.0;
            if (TypeConvert(possibleFloat, tokenBuffer)) {
                /*lint -e{423} .Justification: The pointer is added to a stack and the memory is freed by the class destructor */
                Token *toAdd = new Token(tokenInfo[NUMBER_TOKEN], &tokenBuffer[0], lineNumber);
                if (!tokenQueue.Add(toAdd)) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "StaticList<Token *>: Failed Add() of the token to the token stack");
                }
                converted = true;
            }
        }

        // error!
        if (!converted) {
            /*lint -e{423} .Justification: The pointer is added to a stack and the memory is freed by the class destructor */
            Token *toAdd = new Token(tokenInfo[ERROR_TOKEN], "", lineNumber);
            if (!tokenQueue.Add(toAdd)) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "StaticList<Token *>: Failed Add() of the token to the token stack");
            }
        }
    }

}

/*lint -e{429} . Justification: the allocated memory is freed by the class destructor. */
void LexicalAnalyzer::AddTerminal(const char8 terminal) {

    char8 terminalBuffer[2] = {terminal, '\0'};
    /*lint -e{423} .Justification: The pointer is added to a stack and the memory is freed by the class destructor */
    Token *toAdd = new Token(tokenInfo[TERMINAL_TOKEN], &terminalBuffer[0], lineNumber);
    if (!tokenQueue.Add(toAdd)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "StaticList<Token *>: Failed Add() of the token to the token stack");
    }
}

/*lint -e{429} . Justification: the allocated memory is freed by the class destructor. */
void LexicalAnalyzer::AddTerminal(const char8* const terminalBuffer) {

    /*lint -e{423} .Justification: The pointer is added to a stack and the memory is freed by the class destructor */
    Token *toAdd = new Token(tokenInfo[TERMINAL_TOKEN], terminalBuffer, lineNumber);
    if (!tokenQueue.Add(toAdd)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "StaticList<Token *>: Failed Add() of the token to the token stack");
    }
}

/*lint -e{429} . Justification: the allocated memory is freed by the class destructor. */
/*lint --e{9007} StringHelper::SearchString() and StringHelper::Compare() have no side effects*/
void LexicalAnalyzer::TokenizeInput(const uint32 level) {

    bool ok = true;
    bool isEOF = false;

    StreamString separatorsUsed = separators.Buffer();
    StreamString terminalsUsed = terminals.Buffer();

    while (tokenQueue.GetSize() < (level + 1u)) {
        char8 c = '\0';
        char8 terminal = '\0';
        char8 separator = '\0';

        char8 buffer[16] = { '\0' };
        uint32 bufferSize = 0u;
        // skips one or consecutive comments and controls EOF
        while ((ok) && (c == '\0')) {
            ok = SkipComment(*inputStream, &buffer[0], bufferSize, lineNumber, separators.Buffer(), oneLineCommentBegin.Buffer(),
                             multipleLineCommentBegin.Buffer(), multipleLineCommentEnd.Buffer(), separator, true);
            c = buffer[0];
            // need to do this for one line comments at the end of the tokens
            if (separator == '\n') {
                lineNumber++;
            }
        }

        // the token
        StreamString tokenString = "";
        uint32 bufferIndex = 1u;
        bufferSize--;

        separator = '\0';
        isEOF = !ok;
        // if a '\' is found take the next to respect the special string characters
        bool escape = false;
        // it begins with "?
        bool isString1 = false;
        
        // Lexer starts building the next token and will stop when a single-character terminal is found
        while (ok) {
            
            //std::cout << tokenString.Buffer() << "\n";
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
                if(bufferSize>0u) {
                    AddToken(tokenString.BufferReference(), isString1);
                    AddTerminal(terminal);
                    tokenString="";
                    terminal='\0';
                }
                else {
                    ok = false;
                }
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
                if(bufferSize>0u) {
                    c=buffer[bufferIndex];
                    bufferIndex++;
                    bufferSize--;
                }
                else {
                    if (isString1) {
                        ok = GetC(*inputStream, c);
                    }
                    else {
                        ok = SkipComment(*inputStream, &buffer[0], bufferSize, lineNumber, separators.Buffer(), oneLineCommentBegin.Buffer(),
                        multipleLineCommentBegin.Buffer(), multipleLineCommentEnd.Buffer(), separator, false);
                        if (ok) {
                            // not a comment with a terminal as the next char!
                            bufferIndex=1u;
                            bufferSize--;
                            c=buffer[0];
                            // stop if a comment or separator is found at the end!
                            if(c == '\0') {
                                ok=false;
                            }
                        }
                    }
                }
            }
        }
        
        // The lexer stopped because it found a sigle-character terminal
        
        // So now we have a trail of characters (tokenSring) ended by a terminal (trail + terminal)
        StreamString multiCharToken = tokenString.Buffer();
        multiCharToken += terminal;
        
        // If the trail of characters + terminal is a keyword
        if ( (tokenString.Size() != 0u) && (StringHelper::SearchString(keywords.Buffer(), multiCharToken.Buffer()) != NULL) ) {
            AddTerminal(multiCharToken.Buffer());
            tokenString="";
        }
        
        // If trail + terminal is not a keyword, we handle them separately
        else {
            
            // Trail:
                // if trail alone is a keyword
                if ( (StringHelper::SearchString(keywords.Buffer(), tokenString.Buffer()) != NULL) && (StringHelper::Compare("", tokenString.Buffer()) != 0)) {
                    AddTerminal(tokenString.BufferReference());
                }
                // if trail alone is not a keyword
                else {
                    AddToken(tokenString.BufferReference(), isString1);
                }
            
            // Terminal:
                if (terminal != '\0') {
                    
                    // terminal may be followed by another terminal and their combination may be a keyword, so:
                    char8 nextChar = '\0';
                    uint32 charSize = 1u;
                    
                    uint64  pos = inputStream->Position();
                    bool readOk = inputStream->Read(&nextChar, charSize);
                    if (!readOk) {
                        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed Read() while looking ahead in input stream.");
                    }
                    
                    multiCharToken = "";
                    multiCharToken += terminal;
                    if (nextChar != ' ') {              // since space is used as separator in MathGrammar.keywords
                        multiCharToken += nextChar;
                    }
                    
                    if ( (StringHelper::SearchString(keywords.Buffer(), multiCharToken.Buffer()) != NULL) && (StringHelper::Compare("", multiCharToken.Buffer()) != 0) ) {
                        AddTerminal(multiCharToken.Buffer());
                    }
                    // if the terminal is alone, just add it (and reset the stream pointer)
                    else {
                        AddTerminal(terminal);
                        bool seekOk = inputStream->Seek(pos);
                        if (!seekOk) {
                            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed Seek() while restoring pointer to current position.");
                        }
                    }
                }
        }
        
        if (isEOF) {
            /*lint -e{423} .Justification: The pointer is added to a stack and the memory is freed by the class destructor */
            Token *toAdd = new Token(tokenInfo[EOF_TOKEN], "", lineNumber);
            if (!tokenQueue.Add(toAdd)) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "StaticList<Token *>: Failed Add() of the token to the token stack");
            }
        }

        // if a newline is the separator add it for the next token
        if (separator == '\n') {
            lineNumber++;
        }

    }
}

}
