/**
 * @file Md5EncryptTest.cpp
 * @brief Source file for class Md5EncryptTest
 * @date 08/10/2018
 * @author Giuseppe Ferro
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
 * the class Md5EncryptTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Md5EncryptTest.h"
#include "MemoryOperationsHelper.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

Md5EncryptTest::Md5EncryptTest() {
}

Md5EncryptTest::~Md5EncryptTest() {
}

bool Md5EncryptTest::TestMd5(const char8 *input, StreamString &expected) {
    uint8 output[16];
    MemoryOperationsHelper::Set(output, 0, 16);

    uint32 size = StringHelper::Length(input);
    Md5Encrypt::Md5((uint8*) input, size, output);
    StreamString result;
    for (uint32 i = 0u; (i < 16u); i++) {
        result.Printf("%02x", output[i]);
    }

    bool ret = (result == expected);
    //8b1a9953c4611296a827abf8c47804d7

    //printf("\n%s\n", output);
    return ret;
}

bool Md5EncryptTest::TestMd5Hmac(const char8 *input, const char8 *key, StreamString &expected) {
    uint8 output[16];
    MemoryOperationsHelper::Set(output, 0, 16);

    uint32 size = StringHelper::Length(input);

    uint32 keySize = StringHelper::Length(key);

    Md5Encrypt::Md5Hmac((uint8 *) key, keySize, (uint8 *) input, size, output);

    StreamString result;
    for (uint32 i = 0u; (i < 16u); i++) {
        result.Printf("%02x", output[i]);
    }

    bool ret = (result == expected);

    return ret;
}

