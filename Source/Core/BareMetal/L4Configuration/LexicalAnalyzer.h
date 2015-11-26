/**
 * @file LexicalAnalyzer.h
 * @brief Header file for class LexicalAnalyzer
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

 * @details This header file contains the declaration of the class LexicalAnalyzer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LEXICALANALYZER_H_
#define LEXICALANALYZER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Token.h"
#include "StreamI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/** A slightly programmable lexical analyzer.
 It recognizes Identifiers, Numbers and Floats.
 It allows to browse ahead without consuming the token,
 Tokenisation is performed on demand  */
class LexicalAnalyzer {

public:
    // initialisation functions

    /** constructor */
    LexicalAnalyzer();

    /** constructor */
    ~LexicalAnalyzer();

    /** reset status */
    void Reset();

    /** set these characters as separators */
    void AddSeparators(const char8 *s);

    /** set these characters/LA_TokenValue as separators */
    void AddTerminals(const char8 *s);

    /** change the token code associated with a given complex terminal.
     valid tokenNames are "EOF","IDENT","NUMBER","FLOAT","ERROR" */
    bool ChangeTokenCode(const char8 *tokenName,
                         int32 token);

    /** retrieve the toaken value associated with a certain tokenName */
    int32 GetTokenValue(const char8 *tokenName);

    /** takes one token from the stack or processes the input for a new one
     moves the token into lasToken. The class allocates the data but does
     not provide to the deallocation once the structure has been extracted */
    Token *GetToken(StreamI &stream);

    /** reads in the stack at position lookAhead or increases the stack to allow for it
     it returns the token but it still keeps hold of it */
    Token *PeekToken(uint32 lookAhead,
                     StreamI &stream);

    static void SetParseNumbers(bool parseNumbersIn);


private:

    /** a string made of separators, plus the  */
    StreamString separators;

    /** a string composed of the terminal characters in the order */
    StreamString terminals;

    TokenInfo tokenInfo[8];

    Token *token;

    bool isTerminal;

    char8 terminal;

    /**
     * Switches the parsing of numbers. If off the Lexical Analyser will not try
     * to check if the string is a number
     */
    static bool parseNumbers;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LEXICALANALYZER_H_ */

