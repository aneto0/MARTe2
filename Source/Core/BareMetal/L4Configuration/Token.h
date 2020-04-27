/**
 * @file Token.h
 * @brief Header file for class Token
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

 * @details This header file contains the declaration of the class Token
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TOKEN_H_
#define TOKEN_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CCString.h"
#include "DynamicCString.h"
#include "TokenInfo.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A class that describes a lexical token by identifier and description,
 * storing also its associated data and line number.
 */
class DLL_API Token {

public:
    /**
     * @brief Default constructor.
     * @post
     *   GetId() == 0 &&
     *   GetDescription() == "" &&
     *   GetData() == "" &&
     *   GetLineNumber() == 0;
     */
    Token();

    /**
     * @brief Builds a token providing all the known information in input.
     * @param[in] id is the token identifier.
     * @param[in] description is the token description.
     * @param[in] data is the token data.
     * @param[in] lineNumber is the line number of the token in the stream.
     * @post
     *   GetId() == id &&
     *   GetDescription() == description &&
     *   GetData() == data &&
     *   GetLineNumber() == lineNumber;
     */
    Token(const uint32 id,
          CCString const description,
		  CCString const data,
          const uint32 lineNumber);

    /**
     * @brief Constructor from TokenInfo.
     * @post
     *   GetId() == id &&
     *   GetDescription() == description &&
     *   GetData() == data &&
     *   GetLineNumber() == lineNumber;
     */
    Token(TokenInfo tokenInfo,
          CCString const data,
          const uint32 lineNumber);

    /**
     * @brief Destructor.
     */
    ~Token();

    /**
     * @brief Copy operator.
     * @param[in] td is the Token to be copied into this.
     */
    Token &operator=(const Token &td);

    /**
     * @brief Retrieves the token identifier.
     * @return the token identifier.
     */
    inline uint32 GetId() const;

    /**
     * @brief Retrieves the token description.
     * @return the token description.
     */
    inline CCString GetDescription() const ;

    /**
     * @brief Retrieves the token data.
     * @return the token data.
     */
    inline CCString GetData() const ;

    /**
     * @brief Retrieves the token line number.
     * @return the token line number.
     */
    inline uint32 GetLineNumber() const;

private:
    /**
     * The code identifying the lexical meaning of this part of the text
     */
    uint32 tokenId;

    /**
     * The meaning of the token
     */
    DynamicCString tokenDescription;

    /**
     * The parsed part of the text
     */
    DynamicCString tokenData;

    /**
     * What line the token was found at
     */
    uint32 lineNumber;

};

/**
 * Indicates EOF
 */
static const uint32 EOF_TOKEN = 0u;

/**
 * Indicates an identifier or a string
 */
static const uint32 STRING_TOKEN = 1u;

/**
 * Indicates a number
 */
static const uint32 NUMBER_TOKEN = 2u;

/**
 * Indicates a wrongly constructed element
 */
static const uint32 ERROR_TOKEN = 3u;

/**
 * Indicates an element that is a token on its own
 */
static const uint32 TERMINAL_TOKEN = 4u;


}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {


CCString Token::GetDescription() const {
    return CCString(tokenDescription.GetList());
}

CCString Token::GetData() const {
    return CCString(tokenData.GetList());
}

uint32 Token::GetId() const {
    return tokenId;
}

uint32 Token::GetLineNumber() const {
    return lineNumber;
}


}

#endif /* TOKEN_H_ */

