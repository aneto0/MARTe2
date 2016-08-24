/**
 * @file ValidateTest.cpp
 * @brief Source file for class ValidateTest
 * @date 26/01/2016
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
 * the class ValidateTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ValidateTest.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ValidateTest::TestValidate() {
    const char8*attributes = "+Valid1 = {Class=BasicTypeStandardValidator min=2 max=120 }";

    uint32 x = 10;
    if (!Validate(x, attributes)) {
        return false;
    }
    x = 120;
    if (!Validate(x, attributes)) {
        return false;
    }
    x = 2;
    if (!Validate(x, attributes)) {
        return false;
    }
    x = 121;
    if (Validate(x, attributes)) {
        return false;
    }
    x = 1;
    if (Validate(x, attributes)) {
        return false;
    }
    return true;
}

bool ValidateTest::TestAlias() {
    const char8 * attributes = "+AliasMaker = {Class = StringAliasAll A=1 B=3 C=2 D=8}";
    const char8 * initVal[] = { "A", "B", "C", "D", 0 };
    uint8 endVal[] = { 1, 3, 2, 8, 0 };

    uint32 i = 0u;
    while (initVal[i] != NULL) {
        ConfigurationDatabase cdb;

        cdb.Write("memberToChange", initVal[i]);

        if (!Alias(cdb, "memberToChange", attributes)) {
            return false;
        }

        uint8 testVal;
        cdb.Read("memberToChange", testVal);
        if (testVal != endVal[i]) {
            return false;
        }
        i++;
    }
    return true;

}

bool ValidateTest::TestAliasFalse_MemberNotFound() {
    const char8 * attributes = "+AliasMaker = {Class = StringAliasAll A=1 B=3 C=2 D=8}";
    ConfigurationDatabase cdb;

    cdb.Write("memberToChange", "A");

    return (!Alias(cdb, "memberToChange_not_present", attributes));
}

bool ValidateTest::TestInvertAlias() {
    const char8 * attributes = "+AliasMaker = {Class = StringAliasAll A=1 B=3 C=2 D=8}";
    const char8 * initVal[] = { "A", "B", "C", "D", 0 };
    uint8 endVal[] = { 1, 3, 2, 8, 0 };

    uint32 i = 0u;
    while (initVal[i] != NULL) {
        ConfigurationDatabase cdb;

        cdb.Write("memberToChange", endVal[i]);

        if (!InvertAlias(cdb, "memberToChange", attributes)) {
            return false;
        }

        StreamString testVal;
        cdb.Read("memberToChange", testVal);
        if (testVal != initVal[i]) {
            return false;
        }
        i++;
    }
    return true;
}

bool ValidateTest::TestInvertAlias_MemberNotFound() {
    const char8 * attributes = "+AliasMaker = {Class = StringAliasAll A=1 B=3 C=2 D=8}";
    ConfigurationDatabase cdb;

    cdb.Write("memberToChange", "A");

    return (!InvertAlias(cdb, "memberToChange_not_present", attributes));
}
