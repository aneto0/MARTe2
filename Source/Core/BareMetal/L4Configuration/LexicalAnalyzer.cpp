/**
 * @file LexicalAnalyzer.cpp
 * @brief Source file for class LexicalAnalyzer
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

 * @details This source file contains the definition of all the methods for
 * the class LexicalAnalyzer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "LexicalAnalyzer.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Stores the token informations like the type id (number, string, separator...)
 * and a description.
 */
class TokenInfo {
public:

    /**
     * @brief Default constructor.
     * @post
     *   tokenId == 0 &&
     *   description == NULL;
     */
    TokenInfo() {
        tokenId = 0u;
        description = static_cast<const char8 *>(NULL);
    }

    /**
     * @brief Sets the token informations.
     * @post
     *   tokenId == tokenIdIn &&
     *   description == descriptionIn;
     */
    void SetTokenInfo(int32 tokenIdIn,
                      const char8 *descriptionIn) {
        tokenId = tokenIdIn;
        description = strdup(descriptionIn);
    }

    /**
     * @brief Returns the token identifier.
     * @return the token identifier.
     */
    uint32 GetTokenId() const {
        return tokenId;
    }

    /**
     * @brief Returns the token description.
     * @return the token description.
     */
    const char8* GetDescription() const {
        return description;
    }

    /**
     * @brief Destructor.
     * @post
     *   description == NULL &&
     *   tokenId == 0u;
     */
    ~TokenInfo() {
        if (description == NULL) {
            HeapManager::Free((void *&) description);
        }
        description = static_cast<const char8 *>(NULL);
        tokenId = 0u;
    }
private:

    /**
     * The code identifying the lexical meaning of this part of the text
     */
    uint32 tokenId;

    /**
     * The meaning of the token
     */
    const char * description;

};

}
