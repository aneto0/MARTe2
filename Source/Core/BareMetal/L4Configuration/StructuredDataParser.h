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

#include "StructuredDataI.h"
#include "DynamicCString.h"
#include "ProgressiveTypeCreator.h"
#include "ParserI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


namespace MARTe {


/**
 * @brief Multi-format parser which allows to transform a stream of characters
 * into a structured data store, applying lexical rules set at instance level
 * and parsing rules implemented into subclasses.
 *
 * when syntax=JsonParser All the instances of the parser use the lexical elements defined
 * in MARTe::JsonGrammar and apply the parsing rules of the following
 * grammar:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.py
 * EXPRESSION:
 *   STRING --> GetNodeName()   \: VARIABLE __AddLeaf
 *   STRING __CreateNode    \: \\{ { EXPRESSION }+  \\} __EndBlock
 *
 * VARIABLE:
 *   SCALAR
 *   VECTOR
 *   MATRIX
 *
 * SCALAR:
 *   TOKEN __AddScalar
 *
 * VECTOR
 *   \\[ SCALAR ... \\] __EndVector
 *
 * MATRIX:
 *   \\[ VECTOR ... \\] __EndMatrix
 *
 * TOKEN:
 *   STRING
 *   NUMBER
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * when syntax=XMLParser  All the instances of the parser use the lexical elements defined
 * in MARTe::XMLGrammar and apply the parsing rules of the following
 * grammar:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.py
 * EXPRESSION:
 *   <STRING __GetNodeName > VARIABLE </STRING>  __AddLeaf
 *   <STRING __GetNodeName > ( STRING __GetTypeCast ) VARIABLE </STRING>  __AddLeaf
 *   <STRING __CreateNode  > { EXPRESSION }+ </STRING>  __EndBlock
 *
 * VARIABLE:
 *   SCALAR
 *   VECTOR
 *   MATRIX
 *
 * SCALAR:
 *   TOKEN __AddScalar
 *
 * VECTOR:
 *   \\{ { SCALAR }+ \\} __EndVector
 *
 * MATRIX:
 *   \\{ { VECTOR }+ \\} __EndMatrix
 *
 * TOKEN:
 *   STRING
 *   NUMBER
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * when syntax=StandardParser  All the instances of the parser use the lexical elements defined
 * in MARTe::StandardGrammar and apply the parsing rules of the following
 * grammar:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.py
 * EXPRESSION:
 *   STRING __GetNodeName   = VARIABLE __AddLeaf
 *   STRING __GetNodeName   = ( STRING __GetTypeCast ) VARIABLE __AddLeaf
 *   STRING __CreateNode    = \\{ { EXPRESSION }+ \\} __EndBlock
 *
 * VARIABLE:
 *   SCALAR
 *   VECTOR
 *   MATRIX
 *
 * SCALAR:
 *   TOKEN __AddScalar
 *
 * VECTOR:
 *   { SCALAR }+ __EndVector
 *
 * MATRIX:
 *   { VECTOR }+ __EndMatrix
 *
 * TOKEN:
 *   STRING
 *   NUMBER
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Note: This grammar is written in the SLK language and refers to functions
 * declared in MARTe::ParserI.
 */
class DLL_API StructuredDataParser:public ParserI {

public:

	/**
	 *
	 */
	enum StructuredDataParserFormats{StandardParser,XMLParser,JsonParser} ;

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
    StructuredDataParser(StructuredDataI & databaseIn, StructuredDataParserFormats syntax= StandardParser);

    /**
     * @brief Destructor.
     */
    virtual ~StructuredDataParser();

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


    typedef ErrorManagement::ErrorType  (StructuredDataParser::*ParserMethod)(const ParserI::Token *,ParserI::DebugStream &);

public:

// Callbacks from SLK engine
    /**
     * @brief Moves into the built structured data to the root.
     */
    ErrorManagement::ErrorType End(const Token *currentToken,DebugStream &errorStream);

    /**
     * @brief Gets the node or the variable name.
     */
    ErrorManagement::ErrorType GetNodeName(const Token *currentToken,DebugStream &errorStream);

    /**
     * @brief Adds a leaf in the StructuredData.
     */
    ErrorManagement::ErrorType AddLeaf(const Token *currentToken,DebugStream &errorStream);

    /**
     * @brief Gets the variable type.
     */
    ErrorManagement::ErrorType GetTypeCast(const Token *currentToken,DebugStream &errorStream);

    /**
     * @brief Creates a new node in the StructuredData.
     */
    ErrorManagement::ErrorType CreateNode(const Token *currentToken,DebugStream &errorStream);

    /**
     * @brief Stores a read scalar in the memory.
     */
    ErrorManagement::ErrorType AddScalar(const Token *currentToken,DebugStream &errorStream);

    /**
     * @brief Sets the number of dimensions to one and checks if
     * the matrix is well formed.
     */
    ErrorManagement::ErrorType EndVector(const Token *currentToken,DebugStream &errorStream);

    /**
     * @brief Sets the number of dimensions to two.
     */
    ErrorManagement::ErrorType EndMatrix(const Token *currentToken,DebugStream &errorStream);

    /**
     * @brief Moves into the structuredData to the father.
     */
    ErrorManagement::ErrorType BlockEnd(const Token *currentToken,DebugStream &errorStream);


private:
    /**
     * The StructuredData to be built
     */
    StructuredDataI *		database;

    /**
     * The object used to store the read element and create the AnyType leaf.
     */
    ProgressiveTypeCreator 	memory;

    /**
     * @see ParserI::Execute(*)
     */
    virtual ErrorManagement::ErrorType  Execute(const uint32 number,const Token *currentToken,DebugStream &);

    /**
     * The array of functions needed by the parser.
     */
    ParserMethod ActionMap[10];

};



}

#endif /* PARSERI_H_ */

