/**
 * @file Base64Encoder.cpp
 * @brief Source file for class Base64Encoder
 * @date 16/08/2018
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
 * the class Base64Encoder (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Base64Encoder.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace Base64Encoder {

/**
 * @brief Helper class that performs the actual encoding.
 */
static class Base64Codec {
public:

    /**
     * @brief Constructs the base64 alphabet.
     */
    Base64Codec();

    /**
     * @brief Destructor.
     */
    ~Base64Codec();

    uint8 decodingTable[256];

    static const char8 * const base64Alphabet;
/*lint -e{9150} allow the Base64Codec to be used as a live POD*/
} base64Codec;

const char8 * const Base64Codec::base64Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

Base64Codec::Base64Codec() {
    for (uint32 i = 0u; i < 256u; i++) {
        decodingTable[i] = 0xFFu;
    }
    for (uint8 i = 0u; i < 64u; i++) {
        /*lint -e{1938} access to global data allowed*/
        decodingTable[static_cast<uint8>(base64Alphabet[i])] = i;
    }
    decodingTable[static_cast<uint8>('=')] = 0xFEu;
}

Base64Codec::~Base64Codec() {
}

bool Encode(StreamString &input,
            StreamString &output) {

    int32 size = static_cast<int32>(input.Size());
    const uint8 *in = reinterpret_cast<const uint8 *>(input.Buffer());
    output = "";
    uint32 word = 0u;
    uint32 code;
    while (size > 0) {
        word = in[0];
        code = (word >> 2u);
        word &= 0x3u;
        output += Base64Codec::base64Alphabet[code];
        if (size > 1) {
            word <<= 8u;
            word |= in[1];
            code = (word >> 4u);
            word &= 0xFu;
            output += Base64Codec::base64Alphabet[code];
            if (size > 2) {
                word <<= 8u;
                word |= in[2];
                code = word;
                word &= 0x3Fu;
                code >>= 6u;
                output += Base64Codec::base64Alphabet[code];
                output += Base64Codec::base64Alphabet[word];
            }
            else { // 4 bits available
                word <<= 2u;
                output += Base64Codec::base64Alphabet[word];
                output += "=";
            }
        }
        else { // 2 bits available
            word <<= 4u;
            output += Base64Codec::base64Alphabet[word];
            output += "==";
        }
        size -= 3;
        in = &in[3];
    }

    return true;
}

bool Decode(StreamString &input,
            StreamString &output) {

    uint32 size = static_cast<uint32>(input.Size());
    const uint8 *in = reinterpret_cast<const uint8*>(input.Buffer());
    output = "";
    uint32 word = 0u;
    uint32 code = 0u;
    uint8 key = 0u;
    uint8 c = 0u;
    bool done = false;
    bool ret = true;
    while ((size >= 4u) && (ret) && (!done)) {
        c = in[0];
        if (c == static_cast<uint8>('=')) {
            done = true;
        }
        if (!done) {
            key = base64Codec.decodingTable[c];
            if (key == 0xFFu) {
                ret = false;
            }
        }

        if ((ret) && (!done)) {
            word = key;

            c = in[1];
            if (c == static_cast<uint8>('=')) {
                word <<= 2u;
                output += static_cast<char8>(word);
                done = true;
            }
            if (!done) {
                key = base64Codec.decodingTable[c];
                if (key == 0xFFu) {
                    ret = false;
                }
            }
        }
        if ((ret) && (!done)) {

            word <<= 6u;
            word |= key;
            code = (word >> 4u);
            word &= 0xFu;
            output += static_cast<char8>(code);

            c = in[2];
            if (c == static_cast<uint8>('=')) {
                word <<= 4u;
                output += static_cast<char8>(word);
                done = true;
            }
            if (!done) {
                key = base64Codec.decodingTable[c];
                if (key == 0xFFu) {
                    ret = false;
                }
            }
        }

        if ((ret) && (!done)) {
            word <<= 6u;
            word |= key;
            code = (word >> 2u);
            word &= 0x3u;
            output += static_cast<char8>(code);

            c = in[3];
            if (c == static_cast<uint8>('=')) {
                done = true;
            }
            if (!done) {
                key = base64Codec.decodingTable[c];
                if (key == 0xFFu) {
                    ret = false;
                }
            }
        }
        if ((ret) && (!done)) {
            word <<= 6u;
            word |= key;
            output += static_cast<char8>(word);

            size -= 4u;
            in = &in[4];
        }
    }
    return ret;

}

}
}
