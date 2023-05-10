/**
 * @file InternetHostTest.h
 * @brief Header file for class InternetHostTest
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

 * @details This header file contains the declaration of the class InternetHostTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTERNETHOSTTEST_H_
#define INTERNETHOSTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "InternetHost.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief A structure used for InternetAddress tests
 */
struct InternetHostTestTable {
    const char8 *address;
    uint32 relatedNumber;
    uint16 port;

};

/**
 * @brief Tests all the InternetAddress public functions
 */
class InternetHostTest {

public:

    /**
     * @brief Tests if the default constructor set port=0 and address=NULL
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests if the full constructor set the attributes correctly
     */
    bool TestFullConstructor(uint16 port,
                             const char8 *address,
                             const char8 *expected);

    /**
     * @brief Tests if the function returns the port number.
     */
    bool TestGetPort(uint16 port);

    /**
     * @brief Tests if the function returns the IP address in the format x.x.x.x
     */
    bool TestGetAddress(const char8 *table[][2]);

    /**
     * @brief Tests if the function returns the correct host name.
     */
    bool TestGetHostName(const char8 *table[][2]);

    /**
     * @brief Test if the function returns the IP in number format.
     */
    bool TestGetAddressAsNumber(const InternetHostTestTable *table);

    /**
     * @brief Tests if the local host name is returned.
     */
    bool TestGetLocalHostName();

    /**
     * @brief Tests if the local IP is returned.
     */
    bool TestGetLocalAddress();

    /**
     * @brief Tests if the local IP in number format is returned.
     */
    bool TestGetLocalAddressAsNumber();

    bool TestGetMACAddress();

    /**
     * @brief Test a valid and invalid interfaceName
     * @details if no interface is available the test return true
     */
    bool TestConvertInterfaceNameToInterfaceAddressNumber();

    /**
     * @brief Test a valid and invalid interfaceName
     */
    bool TestConvertInterfaceNameToInterfaceAddress();

    /**
     * @brief Tests if the port number is set correctly.
     */
    bool TestSetPort(uint16 port);

    /**
     * @brief Tests if the IP is set correctly passing a string in the format x.x.x.x
     */
    bool TestSetAddress(const char8 *table[][2]);

    /**
     * @brief Tests if the IP is set correctly passing the host name.
     */
    bool TestSetAddressByHostName(const char8 *table[][2]);

    /**
     * @brief Tests if the IP is set correctly if initialized by number format.
     */
    bool TestSetAddressByNumber(const InternetHostTestTable *table);

    /**
     * @brief Tests if the function sets the IP to localhost.
     */
    bool TestSetLocalAddress();

    /**
     * @brief Tests if the function sets the correct IP Multicast group.
     */
    bool TestSetMulticastGroup();

    /**
     * @brief Tests if the function sets the correct IP Multicast interface address.
     */
    bool TestSetMulticastInterfaceAddress();

    /**
     * @brief Tests if the function sets the correct IP Multicast interface address using the number as input argument.
     */
    bool TestSetMulticastInterfaceAddressWithNumber();

    /**
     * @brief Tests if the function returns the correct IP Multicast group.
     */
    bool TestGetMulticastGroup();

    /**
     * @brief Tests if the function returns the correct IP Multicast interface address.
     */
    bool TestGetMulticastInterfaceAddress();

    /**
     * @brief Tests if the InternetAddressCore structure is returned correctly.
     */
    bool TestGetInternetHost(const InternetHostTestTable *table);

    /**
     * @brief Tests if the Multicast structure is returned correctly.
     */
    bool TestGetInternetMulticastHost();

    /**
     * @brief Tests if the function returns sizeof(InternetAddressCore).
     */
    bool TestSize();

    /**
     * @brief Tests if the function returns sizeof(InternetMulticastCore).
     */
    bool TestMulticastSize();

//    bool Prova();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTERNETHOSTTEST_H_ */

