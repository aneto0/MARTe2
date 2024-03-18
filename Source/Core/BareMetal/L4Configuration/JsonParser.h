/**
 * @file JsonParser.h
 * @brief Header file for class JsonParser
 * @date 10/12/2015
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

 * @details This header file contains the declaration of the class JsonParser
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef JSONPARSER_H_
#define JSONPARSER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ConfigurationParserI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {



/**
 * @brief Concrete class for MARTe::ParserI abstract class, configured
 * for streams of characters encoded in JSON.
 *
 * @details This class is a concrete class for MARTe::ParserI providing the
 * actual lexical elements and parsing rules for interpreting a stream of
 * characters encoded in JSON.
 *
 * Each instance of the parser is bound when it is constructed, with all the
 * objects involved in the parsing analysis, as follows:
 * - An input stream of characters that contains the serialization of a
 * hierarchy of objects, encoded into JSON.
 * - An output structured data store where the parser will build the in-
 * memory objects defined into the input stream of characters.
 * - An output stream of characters where the parser will write all the errors
 * found on the input stream of characters.
 *
 * All the instances of the parser use the lexical elements defined
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
 * Note: This grammar is written in the SLK language and refers to functions
 * declared in MARTe::ParserI.
 */
class DLL_API JsonParser: public ConfigurationParserI {

public:

    /**
     * @brief Default constructor.
     * @param[in] stream is the stream to be parsed.
     * @param[out] databaseIn is the built StructuredData in output.
     * @param[out] err is the stream where error messages are printed to.
     */
    JsonParser(StreamI &stream,
               StructuredDataI &databaseIn,
               BufferedStreamI * const err = static_cast<BufferedStreamI*>(NULL));

    /**
     * @brief Destructor.
     */
    virtual ~JsonParser();

protected:

    /**
     * @see ParserI::GetProduction(*)
     */
    virtual uint32 &GetProduction(const uint32 index) const;

    /**
     * @see ParserI::GetProductionRow(*)
     */
    virtual uint32 GetProductionRow(const uint32 index) const;

    /**
     * @see ParserI::GetParse(*)
     */
    virtual uint32 GetParse(const uint32 index) const;

    /**
     * @see ParserI::GetParseRow(*)
     */
    virtual uint32 GetParseRow(const uint32 index) const;

    /**
     * @see ParserI::GetConflict(*)
     */
    virtual uint32 GetConflict(const uint32 index) const;

    /**
     * @see ParserI::GetConflictRow(*)
     */
    virtual uint32 GetConflictRow(const uint32 index) const;

    /**
     * @see ParserI::GetConstant(*)
     */
    virtual uint32 GetConstant(const uint32 index) const;

    /**
     * @see ParserI::GetSymbolName(*)
     */
    virtual const char8 *GetSymbolName(const uint32 symbol) const;

    /**
     * @see ParserI::Execute(*)
     */
    virtual void Execute(const uint32 number);

private:

    /**
     * The array of functions needed by the parser.
     */
    void (JsonParser::*Action[13])(void);

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* JSONPARSER_H_ */

