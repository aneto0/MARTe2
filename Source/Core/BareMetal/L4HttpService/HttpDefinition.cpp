/**
 * @file HttpDefinition.cpp
 * @brief Source file for class HttpDefinition
 * @date 17/08/2018
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
 * the class HttpDefinition (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "HttpDefinition.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
namespace HttpDefinition {
static const char8 *Http2Convert = " #%<>&~,$+=:/?[]\"@{}";

bool HttpEncode(BufferedStreamI &converted,
                const char8 * const original) {

    const char8 * copy = original;
    bool ret = true;
    if (copy != NULL) {
        while (*copy != '\0') {
            char8 c = *copy;
            copy=&copy[1];
            if (StringHelper::SearchChar(Http2Convert, c)!=NULL) {
                const char8* perc="%";
                uint8 num=static_cast<uint8>(c);
                ret=converted.Printf("%s%2x", perc, num);
            }
            else {
                uint32 size=1u;
                ret=converted.Write(&c, size);
            }
        }
    }
    return ret;
}

int32 HexDecode(const char8 c) {
    int32 ret = -1;
    uint8 ch = static_cast<uint8>(c);
    if ((ch >= static_cast<uint8>('0')) && (ch <= static_cast<uint8>('9'))) {
        /*lint -e{9123} allowed cast to larger type*/
        ret = static_cast<int32>(static_cast<int8>(c) - static_cast<int8>('0'));
    }
    if ((ch >= static_cast<uint8>('a')) && (ch <= static_cast<uint8>('f'))) {
        /*lint -e{9123} allowed cast to larger type*/
        ret = (static_cast<int32>(static_cast<int8>(c) - static_cast<int8>('a')) + 10);
    }
    if ((ch >= static_cast<uint8>('A')) && (ch <= static_cast<uint8>('F'))) {
        /*lint -e{9123} allowed cast to larger type*/
        ret = (static_cast<int32>(static_cast<int8>(c) - static_cast<int8>('A')) + 10);
    }
    return ret;
}

bool HttpDecode(BufferedStreamI &destination,
                BufferedStreamI &source) {
    char8 c;
    bool ret = true;
    uint32 sizeR = 1u;
    while ((sizeR > 0u) && (ret)) {
        (void) source.Read(&c, sizeR);
        if (sizeR > 0u) {
            if (c == '%') {
                char8 buffer[2];
                uint32 size = 2u;
                (void) source.Read(&buffer[0], size);
                if (size > 0u) {
                    c = static_cast<char8>((HexDecode(buffer[0]) * 16) + HexDecode(buffer[1]));
                }
            }
            uint32 sizeW = 1u;
            ret = destination.Write(&c, sizeW);
            sizeR = 1u;
        }
    }
    return ret;

}

}
}
