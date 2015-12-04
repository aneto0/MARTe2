/**
 * @file Parser.h
 * @brief Header file for class Parser
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

 * @details This header file contains the declaration of the class Parser
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4CONFIGURATION_PARSER_H_
#define L4CONFIGURATION_PARSER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "StreamI.h"
#include "BufferedStreamI.h"
#include "LexicalAnalyzer.h"
#include "ParserGrammatic.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Implementation of a parser capable to create a ConfigurationDatabase structure
 * reading data from the provided stream.
 */
class Parser: public Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @param[in] grammaticIn specifies the separators and terminals characters for the parser.
     * @pre
     *   grammaticIn must be valid for this parser. @see ParserGrammatic
     * @post
     *   GetGrammatic() == grammaticIn
     */
    Parser(const ParserGrammatic &grammaticIn = StandardGrammatic);

    /**
     * @brief Destructor.
     */
    virtual ~Parser();

    /**
     * @brief Parses the stream in input and build the configuration database accordingly.
     * @param[in] stream is the stream to be parsed.
     * @param[in,out] database is the built configuration database in output.
     * @param[out] err is a stream showing parse error messages.
     * @return true if the stream in input is parsed correctly, false otherwise. In case of failure, the
     * error causing the failure is printed on the \a err stream in input (if it is not NULL).
     * @details Assuming that the StandardGrammatic is used, the stream to parse must have this syntax:
     *   - BLOCK     ---> STRING = { STRING = ... }
     *   - SCALAR    ---> STRING = NUMBER | STRING
     *   - VECTOR    ---> STRING = { NUMBER ... } | { STRING ... }
     *   - MATRIX    ---> STRING = {{ NUMBER ... } ... } | {{ STRING ... } ... }
     *   - TYPE CAST ---> ( STRING )
     *
     * @details
     *   - All the elements of a vector or matrix must be of the same token type (NUMBER or STRING).
     *   - Variables cannot be empty (i.e "scalar = " or vector = {}" or "matrix = {{}}").
     *   - If the type specified in the TYPE CAST expression is invalid, the value will be
     *   saved in the database as a C-string (default), otherwise the token will be converted to the
     *   specified type and then saved in the database.
     *   - The error messages printed on the \a err stream are in the format "error description [line number]".
     */
    bool Parse(StreamI &stream,
               ConfigurationDatabase &database,
               BufferedStreamI * const err =  static_cast<BufferedStreamI *>(NULL)) const;

    /**
     * @brief Retrieves the grammatic used by this parser.
     * @return the grammatic used by this parser.
     */
    ParserGrammatic GetGrammatic() const;

private:

    ParserGrammatic grammatic;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PARSER_H_ */

