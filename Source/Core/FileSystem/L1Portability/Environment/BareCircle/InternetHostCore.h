/**
 * @file InternetHostCore.h
 * @brief Header file for class InternetHostCore
 * @date 28/10/2015
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

 * @details This header file contains the declaration of the class InternetHostCore
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTERNETHOSTCORE_H_
#define INTERNETHOSTCORE_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <circle/net/ipaddress.h>
#include <circle/string.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe{

    class 
    InternetHostCore {
        public:
            /**
             * @brief   Construct a new Internet Host Core object
             *          Essentially is an holder for the IP/Port couple
             */
            InternetHostCore();

            /**
             * @brief Destroy the Internet Host Core object
             * 
             */
            ~InternetHostCore();

            InternetHostCore(const char8 *ipAddressDottedQuadIn, uint16 ipPortIn);

            void SetAddress(const char8* ipAddressDottedQuad);

            /**
             * @brief Sets the internal port
             */
            void SetPort(const uint16 port);

            /**
             * @brief Internal storage for the four octets of the IP Address
             */
            uint8 ipAddressQuadU8[4];

            /**
             * @brief Internal storage for the IP port
             */
            uint16 ipPort;

        private:

            /**
             * @brief Converts an IP Address from dotted quad string to quad u8
             * 
             * @param ipAddressDottedQuadIn Input string in dotted quad notation
             * @param ipAddressQuadU8Out Output array of quad u8
             */
            static void DottedQuadToQuadU8(const char8* ipAddressDottedQuadIn, uint8* ipAddressQuadU8Out);
    };

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTERNETHOSTCORE_H_ */

