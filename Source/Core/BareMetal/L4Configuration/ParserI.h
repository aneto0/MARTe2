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
#include "StructuredDataI.h"
#include "StreamString.h"
#include "AnyTypeCreator.h"
#include "LexicalAnalyzer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Abstract parser which allows to transform a stream of characters
 * into a data store, applying lexical rules set at instance level
 * and parsing rules implemented into subclasses.
 *
 * @details This class is a partial abstract class providing the generic
 * functionality of a parser while expecting subclasses to provide the specific
 * rules for each concrete language (each subclass must implement the pure
 * abstract functions).
 *
 * Each instance of the parser is bound when it is constructed, with all the
 * objects involved in the parsing analysis, as follows:
 * - An input stream of characters encoded into a given language (XML, JSON,
 * math expressions, etc).
 * - An output stream of characters where the parser will write all the errors
 * found on the input stream of characters.
 *
 * Storage of the output is up to subclasses. 
 * 
 * At construction time the parser is initialized with the lexical
 * elements that the language used in the input stream of characters needs,
 * while the grammar of that language is expected to be implemented in
 * subclasses.
 *
 * After being properly created, each instance is ready to parse the input
 * stream of characters, whenever the user calls the method Parse().
 *
 */
class DLL_API ParserI {

public:

    /**
     * @brief Constructor which initializes the instance with all the items
     * involved into the parsing (input, output, and grammar configuration).
     * @param[in] stream is the stream to be read and parsed.
     * @param[out] err is a stream where parse error messages are written into.
     * @param[in] grammarIn contains the comments patterns, the separator and
     * terminal characters.
     * @post
     *   GetGrammar() == grammarIn
     */
    ParserI(StreamI &stream,
            BufferedStreamI * const err,
            const GrammarInfo &grammarIn);
            
    /**
     * @brief Destructor.
     */
    virtual ~ParserI();

    /**
     * @brief Parses the stream in input and builds the configuration database
     * accordingly.
     * @return true if the stream in input is parsed correctly, false otherwise.
     * In case of failure, the error causing the failure is printed on the
     * \a err stream in input (if it is not NULL).
     */
    bool Parse();

    /**
     * @brief Retrieves the grammar used by this parser.
     * @return the grammar used by this parser.
     */
    GrammarInfo GetGrammarInfo() const;

protected:
   
    /**
     * @brief Retrieves the next expected token identifiers to be
     * pushed on the stack.
     * @param[in] index is the production row.
     * @return the next expected tokens identifiers to be pushed
     * on the stack.
     */
    virtual uint32 &GetProduction(const uint32 index) const =0;

    /**
     * @brief Retrieves the index of the productions array.
     * @param[in] index is the expected token identifier.
     * @return the index of the productions array.
     */
    virtual uint32 GetProductionRow(const uint32 index) const =0;

    /**
     * @brief Retrieves the next token identifier.
     * @param[in] index is the parse row.
     * @return the next token identifier.
     */
    virtual uint32 GetParse(const uint32 index) const =0;

    /**
     * @brief Retrieves the index of the parse array.
     * @param[in] index is the expected token identifier.
     * @return the index of the parse array.
     */
    virtual uint32 GetParseRow(const uint32 index) const =0;

    /**
     * @brief Retrieves the conflict number.
     * @param[in] index is the conflict row.
     * @return the conflict number.
     */
    virtual uint32 GetConflict(const uint32 index) const =0;

    /**
     * @brief Retrieves the conflict array index.
     * @param[in] index is the expected token identifier.
     */
    virtual uint32 GetConflictRow(const uint32 index) const =0;

    /**
     * @brief Retrieves the constants used by the parser.
     * @param[in] index is one of the ParserConstant, to retrieve the
     * related constant.
     * @return the desired constant.
     */
    virtual uint32 GetConstant(const uint32 index) const =0;

    /**
     * @brief Retrieves the name associated to the token.
     * @param[in] symbol is the expected token identifier.
     */
    virtual const char8 *GetSymbolName(const uint32 symbol) const =0;

    /**
     * @brief Retrieves the identifier of the next token produced by the
     * lexical analyzer.
     * @return the identifier of the next token produced by the lexical
     * analyzer.
     */
    virtual uint32 GetNextTokenType();

    /**
     * @brief Peeks in the token stack produced by the lexical analyzer,
     * retrieves the identifier of the token in the next \a position index.
     * @return the identifier of the token in the next \a position index.
     */
    virtual uint32 PeekNextTokenType(const uint32 position);

    /**
     * @brief Pushes the expected token to the stack.
     * @param[in] symbol is the expected token.
     * @param[in] stack is the internal token stack.
     * @param[in, out] top is a pointer to the stack head.
     */
    inline void StackPush(const uint32 symbol,
                          const uint32 * const stack,
                          uint32* &top) const;

    /**
     * @brief Pops the expected token from the internal stack.
     * @param[out] top is a pointer to the stack head.
     */
    inline uint32 StackPop(uint32* &top) const;

    /**
     * @brief Executes the specified function.
     * @param[in] number if the number of the callback to be executed.
     */
    virtual void Execute(const uint32 number)=0;

    /**
     * A pointer to the last token produced by the lexical analyzer.
     */
    Token *currentToken;
    
    /**
     * A flag to specify if an error occurred.
     */
    bool isError;

    /**
     * The lexical analyzer reading the stream and providing the tokens.
     */
    LexicalAnalyzer tokenProducer;

    /**
     * The stream to print the error messages.
     */
    BufferedStreamI *errorStream;
    
    /**
     * Stores the information about the language to be parsed.
     */
    GrammarInfo grammar;

};


namespace ParserConstant {

/**
 * The index of initial expected token identifier.
 */
static const uint32 START_SYMBOL = 0u;

/**
 * The end of the input.
 */
static const uint32 END_OF_SLK_INPUT = 1u;

/**
 * The start index of the expected token identifiers array.
 */
static const uint32 START_STATE = 2u;

/**
 * The start index of the conflicts array.
 */
static const uint32 START_CONFLICT = 3u;

/**
 * The end index of the conflicts array.
 */
static const uint32 END_CONFLICT = 4u;

/**
 * The start index of the actions array.
 */
static const uint32 START_ACTION = 5u;

/**
 * The end index of the actions array.
 */
static const uint32 END_ACTION = 6u;

/**
 * The number of possible conflicts.
 */
static const uint32 TOTAL_CONFLICTS = 7u;

/**
 * The internal stack size for the expected token identifiers.
 */
static const uint32 PARSE_STACK_SIZE = 512u;

}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

void ParserI::StackPush(const uint32 symbol,
                        const uint32 * const stack,
                        uint32 *&top) const {
    /*lint -e{946} [MISRA C++ Rule 5-0-15], [MISRA C++ Rule 5-0-17]. Justification: stack implementation requires operational applied to pointer. */
    if (top > stack) {
        /*lint -e{165} , [MISRA C++ Rule 5-0-18]. Justification: stack implementation requires subtraction applied to pointer. */
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

