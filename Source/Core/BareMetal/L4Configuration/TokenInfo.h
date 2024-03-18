/**
 * @file TokenInfo.h
 * @brief Header file for class TokenInfo
 * @date 26/11/2015
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

 * @details This header file contains the declaration of the class TokenInfo
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TOKENINFO_H_
#define TOKENINFO_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A class that describes a lexical token by identifier and description.
 */
class DLL_API TokenInfo {

public:

    /**
     * @brief Default Constructor
     * @post
     *   GetTokenId() == 0 &&
     *   GetDescription() == ""
     */
    TokenInfo();

    /**
     * @brief Sets the token id and description.
     * @param[in] tokenIdIn is the token identifier.
     * @param[in] descriptionIn is the token description.
     * @post
     *   GetTokenId() == tokenIdIn &&
     *   GetDescription() == descriptionIn
     */
    void Set(const uint32 tokenIdIn,
             const char8 * const descriptionIn);

    /**
     * @brief Retrieves the token identifier.
     * @return the token identifier.
     */
    uint32 GetTokenId() const;

    /**
     * @brief Retrieves the token description.
     * @return the token description.
     */
    const char8* GetDescription();

    /**
     * @brief Destructor.
     */
    ~TokenInfo();

private:

    /**
     * The code identifying the lexical meaning of this part of the text
     */
    uint32 tokenId;

    /**
     * The meaning of the token
     */
    StreamString description;
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TOKENINFO_H_ */

