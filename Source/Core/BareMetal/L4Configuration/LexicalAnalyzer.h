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
#include "ParserGrammar.h"
#include "StaticList.h"
#include "StreamI.h"
#include "Token.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Implementation of a lexical analyzer which will be used by the Parser.
 */
/*lint -e1712 . Justification: This class must be as per the only defined constructor. No need for a default constructor.*/
class DLL_API LexicalAnalyzer {

public:

    /**
     * @brief Default constructor.
     * @param[in] stream the stream to be tokenized.
     * @param[in] terminalsIn C-string containing the terminal characters.
     * @param[in] separatorsIn C-string containing the separator characters.
     * @details
     *   - If the token represents a number (also in hexadecimal, octal, or binary format) its type will be NUMBER_TOKEN.
     *   - If the token represents a string (beginning with a ' " ' or a 'non-number' character at the beginning) its type will be STRING_TOKEN.
     *   - If the token is invalid (i.e 1234a56) its type will be ERROR_TOKEN.
     *   - If the token is a terminal its type will be TERMINAL_TOKEN.
     *   - If the read operation from the stream fails, a token with the EOF_TOKEN type will be returned.
     * @post
     *   separators == separatorsIn &&
     *   terminals == terminalsIn &&
     *   inputStream == &stream &&
     *   token == NULL &&
     *   tokenInfo[0].Set(EOF_TOKEN, "EOF") &&
     *   tokenInfo[1].Set(STRING_TOKEN, "STRING") &&
     *   tokenInfo[2].Set(NUMBER_TOKEN, "NUMBER") &&
     *   tokenInfo[3].Set(ERROR_TOKEN, "ERROR") &&
     *   tokenInfo[4].Set(TERMINAL_TOKEN, "TERMINAL")
     */
    LexicalAnalyzer(StreamI &stream,
                    const char8 * const terminalsIn,
                    const char8 * const separatorsIn);

    /**
     * @brief Destructor.
     * @post
     *   The token queue will be deallocated.
     */
    ~LexicalAnalyzer();

    /**
     * @brief Extracts the next token from the internal queue.
     * @return a pointer to the next token found in the queue.
     */
    Token *GetToken();

    /**
     * @brief Returns the token at the specified position in the queue without consuming it.
     * @param[in] position is the position in the queue of the desired token (0 means the next).
     * @return the token in the specified position found in the internal queue.
     */
    Token *PeekToken(const uint32 position);

private:

    /**
     * @brief Tokenizes the stream in input adding tokens in the internal queue.
     * @param[in] level is the number of tokens to add to the queue.
     */
    void TokenizeInput(const uint32 level = 1u);

    /**
     * Internal token queue
     */
    StaticList<Token *> tokenQueue;

    /**
     * Separator characters
     */
    StreamString separators;

    /**
     * Terminal characters
     */
    StreamString terminals;

    /**
     * Default token types
     */
    TokenInfo tokenInfo[6];

    /**
     * The last token returned
     */
    Token *token;

    /**
     * Pointer to the stream to be tokenized
     */
    StreamI *inputStream;

    /**
     * Line number counter
     */
    uint32 lineNumber;
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LEXICALANALYZER_H_ */

