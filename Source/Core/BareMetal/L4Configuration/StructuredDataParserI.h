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

#ifndef STRUCTURED_DATA_PARSERI_H_
#define STRUCTURED_DATA_PARSERI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Token.h"
#include "StructuredDataI.h"
#include "DynamicCString.h"
#include "LexicalAnalyzer.h"
#include "ProgressiveTypeCreator.h"
#include "ParserI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


namespace MARTe {

/**
 * @brief Abstract parser which allows to transform a stream of characters
 * into a structured data store, applying lexical rules set at instance level
 * and parsing rules implemented into subclasses.
 *
 * @details This class is a partial abstract class providing the generic
 * functionality of a parser while expecting subclasses to provide the specific
 * rules for each concrete language (each subclass must implement the pure
 * abstract functions).
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
 * At construction time, too, the parser is initialized with the lexical
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
class DLL_API StructuredDataParserI:public ParserI {

public:

    /**
     * @brief Constructor which initializes the instance with all the items
     * involved into the parsing (input, output, and grammar configuration).
     * @param[in] stream is the stream to be read and parsed.
     * @param[in,out] databaseIn is the StructuredData in output.
     * @param[out] err is a stream where parse error messages are written into.
     * @param[in] grammarIn contains the comments patterns, the separator and
     * terminal characters.
     * @post
     *   GetGrammar() == grammarIn
     */
    StructuredDataParserI(StructuredDataI &databaseIn,const ParserData & constantsIn);

    /**
     * @brief Destructor.
     */
    virtual ~StructuredDataParserI();

    /**
     * @brief Parses the stream in input and builds the configuration database
     * accordingly.
     * @return true if the stream in input is parsed correctly, false otherwise.
     * In case of failure, the error causing the failure is printed on the
     * \a err stream in input (if it is not NULL).
     */
    ErrorManagement::ErrorType Parse(
    		StreamI &			stream,
            BufferedStreamI * 	const err,
			uint32 				debugLevel);

    /**
     * @brief Retrieves the grammar used by this parser.
     * @return the grammar used by this parser.
     */
    //GrammarInfo GetGrammarInfo() const;

protected:

// Callbacks from SLK engine
    /**
     * @brief Moves into the built structured data to the root.
     */
    ErrorManagement::ErrorType End(const Token *currentToken,BufferedStreamI *errorStream);

    /**
     * @brief Gets the node or the variable name.
     */
    ErrorManagement::ErrorType GetNodeName(const Token *currentToken,BufferedStreamI *errorStream);

    /**
     * @brief Adds a leaf in the StructuredData.
     */
    ErrorManagement::ErrorType AddLeaf(const Token *currentToken,BufferedStreamI *errorStream);

    /**
     * @brief Gets the variable type.
     */
    ErrorManagement::ErrorType GetTypeCast(const Token *currentToken,BufferedStreamI *errorStream);

    /**
     * @brief Creates a new node in the StructuredData.
     */
    ErrorManagement::ErrorType CreateNode(const Token *currentToken,BufferedStreamI *errorStream);

    /**
     * @brief Stores a read scalar in the memory.
     */
    ErrorManagement::ErrorType AddScalar(const Token *currentToken,BufferedStreamI *errorStream);

    /**
     * @brief Sets the number of dimensions to one and checks if
     * the matrix is well formed.
     */
    ErrorManagement::ErrorType EndVector(const Token *currentToken,BufferedStreamI *errorStream);

    /**
     * @brief Sets the number of dimensions to two.
     */
    ErrorManagement::ErrorType EndMatrix(const Token *currentToken,BufferedStreamI *errorStream);

    /**
     * @brief Moves into the structuredData to the father.
     */
    ErrorManagement::ErrorType BlockEnd(const Token *currentToken,BufferedStreamI *errorStream);


private:

    struct ParseStatus{

        /**
         * The type name.
         */
        TypeDescriptor 			td;

        /**
         * The StructuredData node or leaf name.
         */
        DynamicCString 			nodeName;

        /**
         * Status of a variable element parsing
         */
        enum {
        	parseElFinished,
			parseElStarted
        }						parseElStatus;

        void Init();

    } parseStatus;

// PARSING STATUS

// PARSER COMPONENTS
    /**
     * The StructuredData to be built
     */
    StructuredDataI *		database;

    /**
     * The object used to store the read element and create the AnyType leaf.
     */
    ProgressiveTypeCreator 	memory;

};



}

#endif /* PARSERI_H_ */

