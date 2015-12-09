/**
 * @file SlkToken.cpp
 * @brief Source file for class SlkToken
 * @date 04/12/2015
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
 * the class SlkToken (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SlkToken.h"
#include "Token.h"
#include "SlkString.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


SlkToken::SlkToken(StreamI &stream,
                   const char8 *terminals,
                   const char8 *separators) :
        tokenData(),
        lexicalAnalyzer(stream, terminals, separators) {
    tokenLineNumber = 0u;
    tokenId = 0u;
}

uint16 SlkToken::get() {
    Token* tok = lexicalAnalyzer.GetToken();
    tokenLineNumber = tok->GetLineNumber();
    tokenId = tok->GetId();
    tokenData = tok->GetData();
    uint16 endTokendId = 9u; //StringHelper::Length(terminals)+2u;
    const char8* toCompare = static_cast<const char8 *>(NULL);
    if (tok->GetId() == TERMINAL_TOKEN) {
        toCompare = tok->GetData();
    }
    else {
        toCompare = tok->GetDescription();
    }
    for (uint16 i = 0; i < endTokendId; i++) {
        if (StringHelper::Compare(toCompare, SlkGetSymbolName(i)) == 0) {
            tokenId = i;
        }
    }
    printf("\nget %s\n", tokenData.Buffer());

    return tokenId;
}

uint16 SlkToken::peek(uint16 position) {
    uint16 ret = 0u;
    Token* tok = lexicalAnalyzer.PeekToken(position - 1u);
    uint16 endTokendId = 9u; //StringHelper::Length(terminals)+2u;
    const char8* toCompare = static_cast<const char8 *>(NULL);

    if (tok->GetId() == TERMINAL_TOKEN) {
        toCompare = tok->GetData();
    }
    else {
        toCompare = tok->GetDescription();
    }
    for (uint16 i = 0; i < endTokendId; i++) {
        if (StringHelper::Compare(toCompare, SlkGetSymbolName(i)) == 0) {
            ret = i;
        }
    }
    //printf("\npeek position = %d %s\n", position, tok->GetData());

    return ret;
}

const char8 *SlkToken::GetTokenData() {
    return tokenData.Buffer();
}

uint32 SlkToken::GetLineNumber() const {
    return tokenLineNumber;
}

uint16 SlkToken::GetTokenId() const {
    return tokenId;
}
