/**
 * @file Base64EncoderTest.cpp
 * @brief Source file for class Base64EncoderTest
 * @date 26/09/2018
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
 * the class Base64EncoderTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Base64EncoderTest.h"
#include "StreamString.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

Base64EncoderTest::Base64EncoderTest() {
}

Base64EncoderTest::~Base64EncoderTest() {
}


bool Base64EncoderTest::TestEncode(const char8 *toEncodeIn, const char8* expected){
    StreamString toEncode = toEncodeIn;
    StreamString encoded;
    bool ret=Base64Encoder::Encode(toEncode, encoded);
    if(ret){
        ret=(StringHelper::Compare(encoded.Buffer(), expected)==0);
    }
    return ret;
}

bool Base64EncoderTest::TestDecode(const char8 *toDecodeIn, const char8* expected){
    StreamString toDecode = toDecodeIn;
    StreamString decoded;
    bool ret=Base64Encoder::Decode(toDecode, decoded);
    if(ret){
        ret=(StringHelper::Compare(decoded.Buffer(),expected)==0);
    }
    return ret;

}
