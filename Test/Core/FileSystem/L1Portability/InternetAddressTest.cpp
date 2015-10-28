/**
 * @file InternetAddressTest.cpp
 * @brief Source file for class InternetAddressTest
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

 * @details This source file contains the definition of all the methods for
 * the class InternetAddressTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "InternetAddressTest.h"
#include "String.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool InternetAddressTest::TestDefaultConstructor() {

    InternetAddress addr;
    if (addr.GetPort() != 0) {
        return false;
    }

    String ret = addr.GetDotName();

    printf("\n%s\n", ret.Buffer());
    return ret == "0.0.0.0";
}

bool InternetAddressTest::TestFullConstructor(uint16 port,
                                              const char8 * address,
                                              const char8 * expected) {

    InternetAddress addr(port, address);
    if (addr.GetPort() != port) {
        return false;
    }

    String ret = addr.GetDotName();

    printf("\n%s\n", ret.Buffer());
    return (ret == expected);

}

bool InternetAddressTest::TestGetPort(uint16 port) {
    InternetAddress addr(port);
    printf("\n%d\n", addr.GetPort());
    return addr.GetPort() == port;
}

bool InternetAddressTest::TestGetDotName(const char8 *table[][2]) {

    uint32 i = 0;
    while (table[i][0] != NULL) {
        InternetAddress addr(0, table[i][0]);
        String ret = addr.GetDotName();
        printf("\n%s\n", ret.Buffer());
        if (ret != table[i][1]) {
            return false;
        }
        i++;
    }
    return true;
}

bool InternetAddressTest::TestGetHostName(const char8 *table[][2]) {

    uint32 i = 0;
    while (table[i][0] != NULL) {
        InternetAddress addr(0, table[i][0]);
        String ret = addr.GetHostName();
        printf("\n%s\n", ret.Buffer());
        if (ret != table[i][1]) {
            return false;
        }
        i++;
    }
    return true;
}

bool InternetAddressTest::TestGetHostNumber(const InternetAddressTestTable *table) {

    uint32 i = 0;
    while (table[i].address != NULL) {
        InternetAddress addr(0, table[i].address);
        uint32 ret = addr.GetHostNumber();
        printf("\n%x %d\n", ret, i);
        if (ret != table[i].relatedNumber) {
            return false;
        }
        i++;
    }
    return true;

}

bool InternetAddressTest::TestGetLocalAddress() {

    String ret = InternetAddress::GetLocalAddress();

    printf("\n%s\n", ret.Buffer());

    return true;

}

bool InternetAddressTest::TestGetLocalIpNumber() {

    String ret = InternetAddress::GetLocalIpNumber();

    printf("\n%s\n", ret.Buffer());

    return ((ret == "127.0.1.1") || (ret == "127.0.0.1"));
}

bool InternetAddressTest::TestSocketInit() {

    InternetAddress::SocketInit();

    return true;
}

bool InternetAddressTest::TestGetLocalAddressAsNumber() {
    printf("\n%x\n", InternetAddress::GetLocalAddressAsNumber());

    return (InternetAddress::GetLocalAddressAsNumber() == 0x0101007f) || (InternetAddress::GetLocalAddressAsNumber() == 0x0100007f);

}

bool InternetAddressTest::TestSetPort(uint16 port) {
    InternetAddress addr;

    addr.SetPort(port);

    return addr.GetPort() == port;
}

bool InternetAddressTest::TestSetAddressByDotName(const char8 *table[][2]) {
    InternetAddress addr;

    uint32 i = 0;
    while (table[i][0] != NULL) {
        addr.SetAddressByDotName(table[i][0]);
        String ret = addr.GetDotName();
        printf("\n%s\n", ret.Buffer());
        if (ret != table[i][1]) {
            return false;
        }
        i++;
    }
    return true;

}

bool InternetAddressTest::TestSetAddressByName(const char8 *table[][2]) {

    InternetAddress addr;

    uint32 i = 0;
    while (table[i][0] != NULL) {
        addr.SetAddressByName(table[i][1]);
        String ret = addr.GetDotName();
        printf("\n%s\n", ret.Buffer());
        if (ret != table[i][0]) {
            return false;
        }
        i++;
    }
    return true;

}

bool InternetAddressTest::TestSetAddressByNumber(const InternetAddressTestTable *table) {
    InternetAddress addr;

    uint32 i = 0;
    while (table[i].address != NULL) {
        addr.SetAddressByNumber(table[i].relatedNumber);
        String ret = addr.GetDotName();
        printf("\n%s %d\n", ret.Buffer(), i);
        if (ret != table[i].address) {
            return false;
        }
        i++;
    }
    return true;

}

bool InternetAddressTest::TestSetLocalAddress() {
    InternetAddress addr;
    addr.SetLocalAddress();

    String hostname = addr.GetHostName();
    String dotAddr = addr.GetDotName();
    return ((hostname == "localhost") && ((dotAddr == "127.0.1.1") || (dotAddr == "127.0.0.1")));
}

bool InternetAddressTest::TestGetAddress(const InternetAddressTestTable *table) {

    uint32 i = 0;
    while (table[i].address != NULL) {
        InternetAddress addr(table[i].port, table[i].address);
        InternetAddressCore *copy = addr.GetAddress();
#if OPERATING_SYSTEM==Linux
        if (copy->sin_addr.s_addr != addr.GetHostNumber()) {
            printf("\n%x %x %d\n", copy->sin_addr.s_addr, addr.GetHostNumber(), i);
            return false;
        }
        if (copy->sin_port != htons(addr.GetPort())) {
            printf("\n%x %x %d\n", copy->sin_port, addr.GetPort(), i);
            return false;
        }
#endif

        i++;
    }
    return true;
}

bool InternetAddressTest::TestSize() {

    InternetAddress addr;
    return addr.Size() == sizeof(InternetAddressCore);
}

