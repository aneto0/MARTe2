/**
 * @file TokenInfo.cpp
 * @brief Source file for class TokenInfo
 * @date 26/nov/2015
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

 * @details This source file contains the definition of all the methods for
 * the class TokenInfo (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TokenInfo.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
TokenInfo::TokenInfo() {
    tokenId = 0;
    description = static_cast<const char8 *>(NULL);
    dataSize = 0u;
}

/** setup */
void TokenInfo::Set(int32 tokenIdIn,
                    const char8 *descriptionIn,
                    uint32 dataSizeIn) {
    CleanUp();
    tokenId = tokenIdIn;
    description = StringHelper::StringDup(descriptionIn);
    dataSize = dataSizeIn;
}

int32 TokenInfo::GetTokenId() const {
    return tokenId;
}

const char8* TokenInfo::GetDescription() const {
    return description;
}

uint32 TokenInfo::GetDataSize() const{
    return dataSize;
}

void TokenInfo::CleanUp() {
    tokenId = 0;
    if (description != NULL) {
        HeapManager::Free((void *&) description);
    }
    description = static_cast<const char8 *>(NULL);
}
/** constructor */
TokenInfo::~TokenInfo() {
    CleanUp();
}
