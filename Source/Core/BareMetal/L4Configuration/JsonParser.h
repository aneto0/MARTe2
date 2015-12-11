/**
 * @file JsonParser.h
 * @brief Header file for class JsonParser
 * @date 10/dic/2015
 * @author pc
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
#include "ParserI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Implementation of the parser for Json language.
 *
 * @details: Follows the grammar with the functions called if the parser matches the related expression.
 * |EXPRESSION|
 *   STRING --> GetNodeName()   : VARIABLE --> AddLeaf()
 *   STRING --> CreateNode()    : { EXPRESSION ... } --> EndBlock()
 *
 * |VARIABLE|
 *   SCALAR
 *   VECTOR
 *   MATRIX
 *
 * |SCALAR|
 *   TOKEN --> AddScalar()
 *
 * |VECTOR|
 *   [ SCALAR ... ] --> EndVector()
 *
 * |MATRIX|
 *   [ VECTOR ... ] --> EndMatrix()
 *
 * |TOKEN|
 *   STRING
 *   NUMBER
 */
class JsonParser: public ParserI {

public:

    /**
     * @see ParserI::ParserI(*)
     * @post
     *   ParserI::grammar == JsonGrammar.
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
    void (JsonParser::*Action[10])(void);

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* JSONPARSER_H_ */

