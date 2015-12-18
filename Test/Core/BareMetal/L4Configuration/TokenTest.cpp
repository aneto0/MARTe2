/**
 * @file TokenTest.cpp
 * @brief Source file for class TokenTest
 * @date 02/12/2015
 * @author Giuseppe ferrò
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
 * the class TokenTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TokenTest.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
bool TokenTest::TestDefaultConstructor() {
    Token token;
    if (token.GetId() != 0) {
        return false;
    }

    if (token.GetLineNumber() != 0) {
        return false;
    }

    if (StringHelper::Compare(token.GetDescription(), "") != 0) {
        return false;
    }
    return StringHelper::Compare(token.GetData(), "") == 0;
}

bool TokenTest::TestFullConstructor(uint32 id,
                                    const char8 * description,
                                    const char8 * data,
                                    uint32 lineNumber) {
    Token token(id, description, data, lineNumber);
    if (token.GetId() != id) {
        return false;
    }
    if (token.GetLineNumber() != lineNumber) {
        return false;
    }

    if (description == NULL) {
        if (StringHelper::Compare(token.GetDescription(), "") != 0) {
            return false;
        }
    }
    else {
        if (StringHelper::Compare(token.GetDescription(), description) != 0) {
            return false;
        }
    }
    return (data == NULL) ? (StringHelper::Compare(token.GetData(), "") == 0) : (StringHelper::Compare(token.GetData(), data) == 0);

}

bool TokenTest::TestConstructorFromTokenInfo(uint32 id,
                                             const char8 * description,
                                             const char8 * const data,
                                             const uint32 lineNumber) {

    TokenInfo tokenInfo;
    tokenInfo.Set(id, description);

    Token token(tokenInfo, data, lineNumber);

    if (token.GetId() != id) {
        return false;
    }
    if (token.GetLineNumber() != lineNumber) {
        return false;
    }

    if (description == NULL) {
        if (StringHelper::Compare(token.GetDescription(), "") != 0) {
            return false;
        }
    }
    else {
        if (StringHelper::Compare(token.GetDescription(), description) != 0) {
            return false;
        }
    }
    return (data == NULL) ? (StringHelper::Compare(token.GetData(), "") == 0) : (StringHelper::Compare(token.GetData(), data) == 0);

}

bool TokenTest::TestGetId(uint32 id) {

    Token token(id, NULL, NULL, 0);

    return token.GetId() == id;

}

bool TokenTest::TestAssignOperator(uint32 id,
                                   const char8 * description,
                                   const char8 * const data,
                                   const uint32 lineNumber) {

    Token token1(id, description, data, lineNumber);

    Token token2;
    token2 = token1;
    if (token2.GetId() != id) {
        return false;
    }
    if (token2.GetLineNumber() != lineNumber) {
        return false;
    }

    if (description == NULL) {
        if (StringHelper::Compare(token2.GetDescription(), "") != 0) {
            return false;
        }
    }
    else {
        if (StringHelper::Compare(token2.GetDescription(), description) != 0) {
            return false;
        }
    }
    return (data == NULL) ? (StringHelper::Compare(token2.GetData(), "") == 0) : (StringHelper::Compare(token2.GetData(), data) == 0);
}

bool TokenTest::TestGetDescription(const char8 * description) {
    Token token(0, description, NULL, 0);
    return (description == NULL) ? (StringHelper::Compare(token.GetDescription(), "") == 0) : (StringHelper::Compare(token.GetDescription(), description) == 0);
}

bool TokenTest::TestGetData(const char8 * data) {
    Token token(0, NULL, data, 0);
    return (data == NULL) ? (StringHelper::Compare(token.GetData(), "") == 0) : (StringHelper::Compare(token.GetData(), data) == 0);
}

bool TokenTest::TestGetLineNumber(uint32 lineNumber) {
    Token token(0, NULL, NULL, lineNumber);
    return token.GetLineNumber() == lineNumber;
}
