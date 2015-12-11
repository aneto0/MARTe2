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
 * @brief Parser interface implementation.
 *
 * @details Implements the standard functions to build the StructuredData in output.
 */
class ParserI {

public:

    /**
     * @brief Default constructor.
     * @param[in] stream is the stream to be read and parsed.
     * @param[in,out] databaseIn is the StructuredData in output.
     * @param[out] err is a stream where parse error messages are written into.
     * @param[in] grammarIn contains the comments patterns, the separator and terminal characters.
     * @post
     *   typeName == "" &&
     *   nodeName == "" &&
     *   database == &databaseIn &&
     *   isError == false &&
     *   tokenProducer(grammarIn info) &&
     *   numberOfColumns == 0 &&
     *   numberOfRows == 0 &&
     *   firstNumberOfColumns == 0 &&
     *   memory (granularity=1) &&
     *   errorStream == err &&
     *   tokenType == 0 (EOF_TOKEN) &&
     *   numberOfDimensions == 0 &&
     *   grammar == grammarIn
     */
    ParserI(StreamI &stream,
            StructuredDataI &databaseIn,
            BufferedStreamI * const err,
            const ParserGrammar &grammarIn);

    /**
     * @brief Destructor.
     */
    virtual ~ParserI();

    /**
     * @brief Parses the stream in input and builds the configuration database accordingly.
     * @param[in] stream the stream to be parsed.
     * @param[in,out] database built configuration database in output.
     * @param[out] err is a stream where parse error messages are written into.
     * @return true if the stream in input is parsed correctly, false otherwise. In case of failure, the
     * error causing the failure is printed on the \a err stream in input (if it is not NULL).
     *
     * @details
     *   - All the elements of a vector or matrix must be of the same token type (NUMBER or STRING).
     *   - Variables cannot be empty (i.e "scalar = " or vector = {}" or "matrix = {{}}").
     *   - If the type specified in the TYPE CAST expression is invalid, the value will be
     *   saved in the database as a C-string (default), otherwise the token will be converted to the
     *   specified type and then saved in the database.
     *   - The error messages printed on the \a err stream are in the format "error description [line number]".
     */
    bool Parse();

    /**
     * @brief Retrieves the grammar used by this parser.
     * @return the grammar used by this parser.
     */
    ParserGrammar GetGrammar() const;

protected:

    /**
     * @brief Moves into the built structured data to the root.
     */
    virtual void End();

    /**
     * @brief Stores the node or the variable name.
     * @post
     *   nodeName == token.GetData()
     */
    virtual void GetNodeName();

    /**
     * @brief Adds a leaf in the StructuredData.
     */
    virtual void AddLeaf();

    /**
     * @brief Stored the type name.
     * @post
     *   typeName == token.GetData()
     */
    virtual void GetTypeCast();

    /**
     * @brief Creates a new node in the StructuredData.
     * @post
     *   StructuredDataI::Write(*)
     */
    virtual void CreateNode();

    /**
     * @brief Stores a read scalar in the memory.
     * @post
     *   memory.ToType(*)
     */
    virtual void AddScalar();

    /**
     * @brief Sets the number of dimensions to one and checks if the matrix is well formed.
     * @post
     *   numberOfDimensions >= 1
     */
    virtual void EndVector();

    /**
     * @brief Sets the number of dimensions to two.
     * @post
     *   numberOfDimensions == 2
     */
    virtual void EndMatrix();

    /**
     * @brief Moves into the structuredData to the father.
     * @post
     *   StructuredDataI::MoveToAncestor(1)
     */
    virtual void BlockEnd();

    /**
     * @brief Adds the special leaf containing the class name.
     */
    virtual void CreateClassLeaf();

    /**
     * @brief Retrieves the next expected token identifiers to be pushed on the stack.
     * @param[in] index is the production row.
     * @return the next expected tokens identifiers to be pushed on the stack.
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
     * @param[in] index is one of the ParserConstant, to retrieve the related constant.
     * @return the desired constant.
     */
    virtual uint32 GetConstant(const uint32 index) const =0;

    /**
     * @brief Retrieves the name associated to the token.
     * @param[in] symbol is the expected token identifier.
     */
    virtual const char8 *GetSymbolName(const uint32 symbol) const =0;

    /**
     * @brief Retrieves the identifier of the next token produced by the lexical analyzer.
     * @return the identifier of the next token produced by the lexical analyzer.
     */
    virtual uint32 GetNextTokenType();

    /**
     * @brief Peeking in the token stack produced by the lexical analyzer, retrieves the
     * identifier of the token in the next \a position index.
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

private:

    /**
     * The type name.
     */
    StreamString typeName;

    /**
     * The StructuredData node or leaf name.
     */
    StreamString nodeName;

    /**
     * The StructuredData to be built
     */
    StructuredDataI *database;

    /**
     * A flag to specify if an error occurred.
     */
    bool isError;

    /**
     * The lexical analyzer reading the stream and providing the tokens.
     */
    LexicalAnalyzer tokenProducer;

    /**
     * The number of columns in case of read of vector.
     */
    uint32 numberOfColumns;

    /**
     * The number of elements read before the vector close terminal.
     */
    uint32 firstNumberOfColumns;

    /**
     * The number of rows in case of read of matrix.
     */
    uint32 numberOfRows;

    /**
     * The object used to store the read element and create the AnyType leaf.
     */
    AnyTypeCreator memory;

    /**
     * The stream to print the error messages.
     */
    BufferedStreamI *errorStream;

    /**
     * The token id.
     */
    uint32 tokenType;

    /**
     * The number of dimensions of the variable (0=scalar, 1=vector, 2=matrix)
     */
    uint8 numberOfDimensions;

    /**
     * Stores the information about the language to be parsed.
     */
    ParserGrammar grammar;

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

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

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

