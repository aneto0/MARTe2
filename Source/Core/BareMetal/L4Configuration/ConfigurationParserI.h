/**
 * @file ConfigurationParserI.h
 * @brief Header file for class ConfigurationParserI
 * @date 06/05/2020
 * @author Nicolo Ferron
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

 * @details This header file contains the declaration of the class ConfigurationParserI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONFIGURATIONPARSERI_H_
#define CONFIGURATIONPARSERI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AnyTypeCreator.h"
#include "ConfigurationDatabase.h"
#include "LexicalAnalyzer.h"
#include "ParserI.h"
#include "RuntimeEvaluator.h"
#include "StructuredDataI.h"
#include "StreamString.h"
#include "Token.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Abstract parser which allows to transform MARTe2 configuration
 * file into a StructuredDataI store, applying lexical rules set at
 * instance level and parsing rules implemented into subclasses.
 *
 * @details This class is a partial abstract class providing the generic
 * functionality of a configuration file parser, while expecting
 * subclasses to provide the specific rules for each concrete language
 * (MARTe2 standard configuration language, XML, Json).
 *
 * Each instance of the parser is bound when it is constructed, with all the
 * objects involved in the parsing analysis, as follows:
 * - An input stream of characters that contains the serialization of a
 * hierarchy of objects, encoded into a given language (e.g. XML, JSON, etc).
 * - An output structured data store where the parser will build the in-
 * memory objects defined into the input stream of characters.
 * - An output stream of characters where the parser will write all the errors
 * found on the input stream of characters.
 *
 * At construction time the parser is initialized with the lexical
 * elements that the language used in the input stream of characters needs,
 * while the grammar of that language is expected to be implemented in
 * subclasses.
 *
 * After being properly created, each instance is ready to parse the input
 * stream of characters, whenever the user calls the method Parse().
 *
 * Notes about the input stream of characters:
 * - All the elements of a vector or matrix must be of the same token type
 * (NUMBER or STRING).
 * - Variables cannot be empty (i.e "scalar = " or vector = {}" or
 * "matrix = {{}}").
 * - If the type specified in the TYPE CAST expression is invalid, the value
 * will be saved in the database as a C-string (default), otherwise the token
 * will be converted to the specified type and then saved in the database.
 * - The error messages printed on the \a err stream are in the format
 * "error description [line number]".
 */
class DLL_API ConfigurationParserI : public ParserI {

public:

    /**
     * @brief Constructor which initializes the instance with all the items
     * involved into the parsing (input, output, and grammar configuration).
     * @param[in] stream is the stream to be read and parsed.
     * @param[in,out] databaseIn is the StructuredDataI in output.
     * @param[out] err is a stream where parse error messages are written into.
     * @param[in] grammarIn contains the comments patterns, the separator and
     * terminal characters.
     * @post
     *   GetGrammar() == grammarIn
     */
    ConfigurationParserI(StreamI &stream,
            StructuredDataI &databaseIn,
            BufferedStreamI * const err,
            const GrammarInfo &grammarIn);

    /**
     * @brief Destructor.
     */
    virtual ~ConfigurationParserI();

protected:

    /**
     * @brief Moves into the built structured data to the root.
     */
    void End();

    /**
     * @brief Gets the node or the variable name.
     */
    void GetNodeName();

    /**
     * @brief Adds a leaf in the StructuredData.
     */
    void AddLeaf();

    /**
     * @brief Gets the variable type.
     */
    void GetTypeCast();

    /**
     * @brief Handles a math expression cast.
     */
    void GetExprCast();

    /**
     * @brief Creates a new node in the StructuredData.
     */
    void CreateNode();

    /**
     * @brief Stores a read scalar in the memory.
     */
    void AddScalar();

    /**
     * @brief Sets the number of dimensions to one and checks if
     * the matrix is well formed.
     */
    void EndVector();

    /**
     * @brief Sets the number of dimensions to two.
     */
    void EndMatrix();

    /**
     * @brief Moves into the structuredData to the father.
     */
    void BlockEnd();

    /**
     * @brief Creates a new vector of nodes in the StructuredData.
     */
    void CreateNodeV();

    /**
     * @brief Creates a new element in a vector of nodes in the StructuredData.
     */
    void CreateNodeVE();

    /**
     * @brief Ends a vector of blocks.
     */
    void EndVectorB();

private:

    /**
     * @brief Expands all the variables available for this evaluator.
     * @details Collects all the variables and looks for them in the parsed database.
     * When found their memory and type will be assigned from the database so that they can be used by the evaluator.
     * @param[in] evaluator the evaluator.
     * @return true if all the variables can be found in the parsed database.
     */
    bool BrowseExpressionVariables(RuntimeEvaluator * evaluator);

    /**
     * @brief Parses and executes a given expression. If successful the database node originally containing the expression will be patched with the output of the expression result.
     * @param[in] nodePath path in the parsed database of the node containing the expression.
     * @param[in] nodeNameIn name of the node in the parsed database node containing the expression.
     * @param[in] outputTypeName variable type to be used as the output of the expression execution.
     * @return true if the path can be sucessfully parsed and executed.
     */
    bool ExpandExpression(const char8 * const nodePath, const char8 * const nodeNameIn, const char8 * const outputTypeName);

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
     * The token id.
     */
    uint32 tokenType;

    /**
     * The number of dimensions of the variable (0=scalar, 1=vector, 2=matrix)
     */
    uint8 numberOfDimensions;

    /**
     * The current structured index.
     */
    ConfigurationDatabase vectorStructureIdx;

    /*
     * True if a math expression has been found.
     */
    bool handleMathExpr;

    /**
     * True if the output StructuredDataI is a ConfigurationDatabase
     */
    bool outputSupportsMathExpr;

    /**
     * Stores the math expressions to be processed at the end.
     */
    ConfigurationDatabase mathExpressionsCDB;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONFIGURATIONPARSERI_H_ */

