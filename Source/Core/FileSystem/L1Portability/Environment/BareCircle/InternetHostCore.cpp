/**
 * @file InternetHostCore.h
 * @brief Source file for class InternetHostCore
 * @date 30/04/2020
 * @author Giuseppe Avon
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

 * @details This header file contains the declaration of the class InternetHostCore
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <circle/logger.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "ErrorManagement.h"
#include "GeneralDefinitions.h"
#include "InternetHostCore.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    InternetHostCore::InternetHostCore() {
        memset(ipAddressQuadU8, 0, sizeof(uint8) * 4);
        ipPort = 0u;
    }

    InternetHostCore::~InternetHostCore() {

    }

    InternetHostCore::InternetHostCore(const char8 *ipAddressDottedQuadIn, uint16 ipPortIn) {
        ipPort = ipPortIn;
        DottedQuadToQuadU8(ipAddressDottedQuadIn, ipAddressQuadU8);
    }

    void InternetHostCore::DottedQuadToQuadU8(const char8* ipAddressDottedQuadIn, uint8* ipAddressQuadU8Out) {
        if((ipAddressDottedQuadIn != NULL_PTR(char8*)) && (ipAddressQuadU8Out != NULL_PTR(uint8*))) {
            uint32 a = 0u, b = 0u, c = 0u, d = 0u;

            sscanf(ipAddressDottedQuadIn, "%d.%d.%d.%d", &a, &b, &c, &d);
            ipAddressQuadU8Out[0] = (uint8)a;
            ipAddressQuadU8Out[1] = (uint8)b;
            ipAddressQuadU8Out[2] = (uint8)c;
            ipAddressQuadU8Out[3] = (uint8)d;
        }
    }

    void InternetHostCore::SetPort(const uint16 port) {
        ipPort = port;
    }

    void InternetHostCore::SetAddress(const char8* ipAddressDottedQuad) {
        DottedQuadToQuadU8(ipAddressDottedQuad, ipAddressQuadU8);
    }

}
