/**
 * @file TokenInfoTest.cpp
 * @brief Source file for class TokenInfoTest
 * @date 02/12/2015
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
 * the class TokenInfoTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TokenInfoTest.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;
bool TokenInfoTest::TestDefaultConstructor() {
    TokenInfo tokenInfo;
    if (tokenInfo.GetTokenId() != 0) {
        return false;
    }

    return (StringHelper::Compare(tokenInfo.GetDescription(), "") == 0);
}

bool TokenInfoTest::TestSet(const char8* description,
                            uint32 id) {
    TokenInfo tokenInfo;

    tokenInfo.Set(id, description);
    if (tokenInfo.GetTokenId() != id) {
        return false;
    }

    return (description != NULL) ? (StringHelper::Compare(tokenInfo.GetDescription(), description) == 0) : (StringHelper::Compare(tokenInfo.GetDescription(), "") == 0);
}

bool TokenInfoTest::TestGetTokenId(uint32 id) {
    TokenInfo tokenInfo;

    tokenInfo.Set(id, NULL);
    return (tokenInfo.GetTokenId() == id);

}

bool TokenInfoTest::TestGetDescription(const char8* description) {
    TokenInfo tokenInfo;

    tokenInfo.Set(0, description);

    return (description != NULL) ? (StringHelper::Compare(tokenInfo.GetDescription(), description) == 0) : (StringHelper::Compare(tokenInfo.GetDescription(), "") == 0);
}

