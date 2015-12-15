/**
 * @file StandardParser.h
 * @brief Header file for class StandardParser
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

 * @details This header file contains the declaration of the class StandardParser
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STANDARDPARSER_H_
#define STANDARDPARSER_H_

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
 * @brief Implementation of the parser for the standard MARTe configuration language.
 *
 * @details: Follows the grammar with the functions called if the parser matches the related expression. The grammar is written using
 * the SLK language.
 * EXPRESSION:
 *   STRING __GetNodeName   = VARIABLE __AddLeaf
 *   STRING __GetNodeName   = ( STRING __GetTypeCast ) VARIABLE __AddLeaf
 *   STRING __CreateNode    = \{ { EXPRESSION }+ \} __EndBlock
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
 */
class DLL_API StandardParser: public ParserI {

public:

    /**
     * @brief Default constructor.
     * @param[in] stream is the stream to be parsed.
     * @param[out] databaseIn is the built StructuredData in output.
     * @param[out] err is the stream where error messages are printed to.
     * @post
     *   ParserI::grammar == StandardGrammar.
     */
    StandardParser(StreamI &stream,
                   StructuredDataI &databaseIn,
                   BufferedStreamI * const err = static_cast<BufferedStreamI*>(NULL));

    /**
     * @brief Destructor.
     */
    virtual ~StandardParser();

protected:

    /**
     * @see ParserI::GetProduction(*).
     */
    virtual uint32 &GetProduction(const uint32 index) const;

    /**
     * @see ParserI::GetProductionRow(*).
     */
    virtual uint32 GetProductionRow(const uint32 index) const;

    /**
     * @see ParserI::GetParse(*).
     */
    virtual uint32 GetParse(const uint32 index) const;

    /**
     * @see ParserI::GetParseRow(*).
     */
    virtual uint32 GetParseRow(const uint32 index) const;

    /**
     * @see ParserI::GetConflict(*).
     */
    virtual uint32 GetConflict(const uint32 index) const;

    /**
     * @see ParserI::GetConflictRow(*).
     */
    virtual uint32 GetConflictRow(const uint32 index) const;

    /**
     * @see ParserI::GetConstant(*).
     */
    virtual uint32 GetConstant(const uint32 index) const;

    /**
     * @see ParserI::GetSymbolName(*).
     */
    virtual const char8 *GetSymbolName(const uint32 symbol) const;

    /**
     * @see ParserI::Execute(*).
     */
    virtual void Execute(const uint32 number);

private:

    /**
     * The array of functions needed by the parser.
     */
    void (StandardParser::*Action[10])(void);

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STANDARDPARSER_H_ */

