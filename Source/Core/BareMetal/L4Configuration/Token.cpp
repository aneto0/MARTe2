/**
 * @file Token.cpp
 * @brief Source file for class Token
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
 * the class Token (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*
 static TokenInfo tokenTypes[] = { { 0x100u, "EOF" }, { 0x101u, "IDENT" }, { 0x102u, "NUMBER" }, { 0x103u, "FLOAT" }, { 0x104u, "ERROR" },
 { 0x105u, "SEPARATOR" }, { 0x106u, "TERMINAL" } };
 */

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

Token::Token(const uint32 id,
             const char8 *description,
             void *data,
             uint32 dataSize,
             const uint32 lineNumber) {
    tokenId = id;
    tokenDescription = description;
    tokenData = HeapManager::Malloc(dataSize);
    MemoryOperationsHelper::Copy(tokenData, data, dataSize);
}

Token::Token(TokenInfo tokenInfo,
             void *data,
             const uint32 lineNumber) {
    tokenId = tokenInfo.GetTokenId();
    tokenDescription = tokenInfo.GetDescription();
    tokenData = HeapManager::Malloc(tokenInfo.GetDataSize());
    MemoryOperationsHelper::Copy(tokenData, data, tokenInfo.GetDataSize());
}

Token::~Token() {
    if (tokenData != NULL) {
        if (!HeapManager::Free(static_cast<void* &>(tokenData))) {
            //TODO
        }
    }
}

uint32 Token::GetId() const {
    return tokenId;
}

void Token::operator=(const Token &td) {
    tokenId = td.tokenId;
    tokenData = td.tokenData;
    tokenDescription = td.tokenDescription;
}

const char8 * Token::GetDescription() const {
    return tokenDescription.Buffer();
}

void * Token::GetData() const {
    return static_cast<void *>(tokenData);
}

}

