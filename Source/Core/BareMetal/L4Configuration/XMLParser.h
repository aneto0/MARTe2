/**
 * @file XMLParser.h
 * @brief Header file for class XMLParser
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

 * @details This header file contains the declaration of the class XMLParser
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include "StructuredDataParserI.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {


namespace XMLParserData{
	extern const ParserData parserData;
}


/**
 * @brief Concrete class for MARTe::ParserI abstract class, configured
 * for streams of characters encoded in XML.
 *
 * @details This class is a concrete class for MARTe::ParserI providing the
 * actual lexical elements and parsing rules for interpreting a stream of
 * characters encoded in XML.
 *
 * Each instance of the parser is bound when it is constructed, with all the
 * objects involved in the parsing analysis, as follows:
 * - An input stream of characters that contains the serialization of a
 * hierarchy of objects, encoded into XML.
 * - An output structured data store where the parser will build the in-
 * memory objects defined into the input stream of characters.
 * - An output stream of characters where the parser will write all the errors
 * found on the input stream of characters.
 *
 * All the instances of the parser use the lexical elements defined
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
 *
 * Note: This grammar is written in the SLK language and refers to functions
 * declared in MARTe::ParserI.
 */
class DLL_API XMLParser: public StructuredDataParserI {
public:

    /**
     * @brief Default constructor.
     * @param[in] stream is the stream to be parsed.
     * @param[out] databaseIn is the built StructuredData in output.
     * @param[out] err is the stream where error messages are printed to.
     * @post
     *   ParserI::grammar == XMLGrammar.
     */
    XMLParser(StructuredDataI &databaseIn);

    /**
     * @brief Destructor.
     */
    virtual ~XMLParser();

private:
    /**
     * @see ParserI::Execute(*)
     */
    virtual ErrorManagement::ErrorType  Execute(const uint32 number,const Token *currentToken,BufferedStreamI *errorStream);

    /**
     * implemented in XMLParserData.cpp
     */
	void MapMethods();

    /**
     * The array of functions needed by the parser.
     */
	ErrorManagement::ErrorType  (XMLParser::*Action[10])(const Token *,BufferedStreamI *);

};

} // end namespace MARTe

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* XMLPARSER_H_ */

