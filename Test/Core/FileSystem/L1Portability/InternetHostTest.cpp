/**
 * @file InternetHostTest.cpp
 * @brief Source file for class InternetHostTest
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
 * the class InternetHostTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "StringHelper.h"
#include "InternetHostTest.h"
#include "stdio.h"
#include "MemoryOperationsHelper.h"
#include "AdvancedErrorManagement.h"

//WARNING: These defines are used to allow the "ENVIRONMENT ==" comparison without stringify-ing
#define Linux 1
#define BareUS 2
#define FreeRTOS 3

#if ENVIRONMENT==Linux //&& ARCHITECTURE==x86_gcc
#include "ifaddrs.h"
#else
#pragma warning "Compiling with limited test support for interface name and index resolution"
#endif
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#include "InternetHostTest.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool InternetHostTest::TestDefaultConstructor() {

    InternetHost addr;
    if (addr.GetPort() != 0) {
        return false;
    }

    StreamString ret = addr.GetAddress();

    return ret == "0.0.0.0";
}

bool InternetHostTest::TestFullConstructor(uint16 port,
                                           const char8 *address,
                                           const char8 *expected) {

    InternetHost addr(port, address);
    if (addr.GetPort() != port) {
        return false;
    }

    StreamString ret = addr.GetAddress();

    return (ret == expected);

}

bool InternetHostTest::TestGetPort(uint16 port) {
    InternetHost addr(port);
    return addr.GetPort() == port;
}

bool InternetHostTest::TestGetAddress(const char8 *table[][2]) {

    uint32 i = 0;
    while (table[i][0] != NULL) {
        InternetHost addr(0, table[i][0]);
        StreamString ret = addr.GetAddress();
        if (ret != table[i][1]) {
            return false;
        }
        i++;
    }
    return true;
}

bool InternetHostTest::TestGetHostName(const char8 *table[][2]) {

    uint32 i = 0;
    while (table[i][0] != NULL) {
        InternetHost addr(0, table[i][0]);
        StreamString ret = addr.GetHostName();
        if (ret != table[i][1]) {
            return false;
        }
        i++;
    }
    return true;
}

bool InternetHostTest::TestGetAddressAsNumber(const InternetHostTestTable *table) {

    uint32 i = 0;
    while (table[i].address != NULL) {
        InternetHost addr(0, table[i].address);
        uint32 ret = addr.GetAddressAsNumber();
        if (ret != table[i].relatedNumber) {
            return false;
        }
        i++;
    }
    return true;

}

bool InternetHostTest::TestGetLocalHostName() {

    StreamString ret = InternetHost::GetLocalHostName();

    return true;

}

bool InternetHostTest::TestGetLocalAddress() {

    StreamString ret = InternetHost::GetLocalAddress();
    bool ok = (StringHelper::Length(ret.Buffer()) > 0);
    StreamString tok;
    uint32 i = 0;
    char8 terminator;
    ret.Seek(0);
    while (ret.GetToken(tok, ".", terminator)) {
        tok = "";
        i++;
    }
    ok &= (i == 4u);
    return ok;
}

bool InternetHostTest::TestGetLocalAddressAsNumber() {
    return (InternetHost::GetLocalAddressAsNumber() != 0u);
}

bool InternetHostTest::TestGetMACAddress() {
    InternetHost ih;
    StreamString localHostName = "lo";
    const char8 *const interfaceName = "invalidInterfaceName";
    uint8 mac[8];
    uint8 macRef[8];
    MemoryOperationsHelper::Set(macRef, 0, 8);
    bool ok = !ih.GetMACAddress(interfaceName, mac);
#if ENVIRONMENT==Linux && ARCHITECTURE==x86_gcc
    if (ok) {
        struct ifaddrs *addrs, *tmp;
        getifaddrs(&addrs);
        for (tmp = addrs; (tmp != NULL) && ok; tmp = tmp->ifa_next) {
            if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET) {
                MemoryOperationsHelper::Set(mac, 0, 8);
                ok = ih.GetMACAddress(tmp->ifa_name, mac);
                if(ok){
                    if(localHostName != tmp->ifa_name){
                        StreamString auxStr = tmp->ifa_name;
                        auxStr += " = ";
                        for(uint32 i = 0; i < 6; i++){
                            auxStr.Printf("%02x ", mac[i]);
                        }
                        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "%s", auxStr.Buffer());
                        ok = (MemoryOperationsHelper::Compare(mac, macRef, 8)!=0);

                    }
                }
            }
        }
        freeifaddrs(addrs);
    }
#endif
    return ok;
}

bool InternetHostTest::TestConvertInterfaceNameToInterfaceAddressNumber() {
    InternetHost ih;
    bool ok = (ih.ConvertInterfaceNameToInterfaceAddressNumber("invalidInterfaceName") == 0);
#if ENVIRONMENT==Linux && ARCHITECTURE==x86_gcc
    if (ok) {
        struct ifaddrs *addrs, *tmp;
        getifaddrs(&addrs);
        tmp = addrs;
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET) {
            in_addr_t aux = ih.ConvertInterfaceNameToInterfaceAddressNumber(tmp->ifa_name);
            ok = (aux != 0);
        }
        freeifaddrs(addrs);
    }
#endif
    return ok;
}

bool InternetHostTest::TestConvertInterfaceNameToInterfaceAddress() {
    InternetHost ih;
    bool ok = (ih.ConvertInterfaceNameToInterfaceAddress("invalidInterfaceName") == "0.0.0.0");
#if ENVIRONMENT==Linux && ARCHITECTURE==x86_gcc
    if (ok) {
        struct ifaddrs *addrs, *tmp;
        getifaddrs(&addrs);
        tmp = addrs;
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET) {
            StreamString aux(ih.ConvertInterfaceNameToInterfaceAddress(tmp->ifa_name));
            ok = (aux != "0.0.0.0");
        }
        freeifaddrs(addrs);
    }
#endif
    return ok;
}

bool InternetHostTest::TestSetPort(uint16 port) {
    InternetHost addr;

    addr.SetPort(port);

    return addr.GetPort() == port;
}

bool InternetHostTest::TestSetAddress(const char8 *table[][2]) {
    InternetHost addr;

    uint32 i = 0;
    while (table[i][0] != NULL) {
        addr.SetAddress(table[i][0]);
        StreamString ret = addr.GetAddress();
        if (ret != table[i][1]) {
            return false;
        }
        i++;
    }
    return true;

}

bool InternetHostTest::TestSetAddressByHostName(const char8 *table[][2]) {

    InternetHost addr;

    uint32 i = 0;
    while (table[i][0] != NULL) {
        addr.SetAddressByHostName(table[i][1]);
        StreamString ret = addr.GetAddress();
        if (ret != table[i][0]) {
            return false;
        }
        i++;
    }
    return true;

}

bool InternetHostTest::TestSetAddressByNumber(const InternetHostTestTable *table) {
    InternetHost addr;

    uint32 i = 0;
    while (table[i].address != NULL) {
        addr.SetAddressByNumber(table[i].relatedNumber);
        StreamString ret = addr.GetAddress();
        if (ret != table[i].address) {
            return false;
        }
        i++;
    }
    return true;

}

bool InternetHostTest::TestSetLocalAddress() {
    InternetHost addr;
    addr.SetLocalAddress();

    StreamString dotAddr = addr.GetAddress();
    return ((dotAddr == "127.0.1.1") || (dotAddr == "127.0.0.1"));
}

bool InternetHostTest::TestSetMulticastGroup() {
    InternetHost host;
    host.SetMulticastGroup("234.0.0.1");
    StreamString maddr = host.GetMulticastGroup();
    return maddr == "234.0.0.1";
}

bool InternetHostTest::TestGetMulticastGroup() {
    InternetHost host;
    host.SetMulticastGroup("234.0.0.1");
    StreamString maddr = host.GetMulticastGroup();
    return maddr == "234.0.0.1";
}

bool InternetHostTest::TestSetMulticastInterfaceAddress() {
    InternetHost host;
    host.SetMulticastInterfaceAddress("127.0.0.1");
    StreamString haddr = host.GetMulticastInterfaceAddress();
    return haddr == "127.0.0.1";
}

bool InternetHostTest::TestSetMulticastInterfaceAddressWithNumber() {
    InternetHost ih;
    bool ok = true;
    struct ifaddrs *addrs, *tmp;
    bool found = false;
    char8 *addr;
#if ENVIRONMENT==Linux && ARCHITECTURE==x86_gcc
    getifaddrs(&addrs);
    for (tmp = addrs; (tmp != NULL) && !found; tmp = tmp->ifa_next) { //addrs contains several time the same interfaces with different families. Loop until AF_INET is found (ipv4). In case does not exist the test succeeds
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
            found = true;
            in_addr_t aux = ih.ConvertInterfaceNameToInterfaceAddressNumber(tmp->ifa_name);
            ok = (aux != 0);
            if (ok) {
                ih.SetMulticastInterfaceAddress(aux);
                StreamString haddr = ih.GetMulticastInterfaceAddress();
                char8 auxStr[INET_ADDRSTRLEN] = { 0, };
                inet_ntop(AF_INET, &((struct sockaddr_in*) (tmp->ifa_addr))->sin_addr, auxStr, INET_ADDRSTRLEN); //COnvert in_addr to string and save it in auxStr
                ok = haddr == auxStr;
            }
        }
    }
    freeifaddrs(addrs);
#endif
    if (!found) {
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "AF_INET family not found on this machine. Test not done");
    }
    return ok;
}

bool InternetHostTest::TestGetMulticastInterfaceAddress() {
    return TestSetMulticastInterfaceAddress();
}

bool InternetHostTest::TestGetInternetHost(const InternetHostTestTable *table) {

    uint32 i = 0;
    while (table[i].address != NULL) {
        InternetHost addr(table[i].port, table[i].address);
        InternetHostCore *copy = addr.GetInternetHost();

#if ENVIRONMENT == Linux
        if (copy->sin_addr.s_addr != addr.GetAddressAsNumber()) {
            return false;
        }
        if (copy->sin_port != htons(addr.GetPort())) {
            return false;
        }

#else
        //ENVIRONMENT==Windows
        /*if (copy->sin_addr.s_addr != addr.GetAddressAsNumber()) {
         return false;
         }
         if (copy->sin_port != htons(addr.GetPort())) {
         return false;
         }*/
#endif

        i++;
    }
    return true;
}

bool InternetHostTest::TestGetInternetMulticastHost() {

    InternetHost addr;
    addr.SetMulticastGroup("234.0.0.1");
    InternetMulticastCore *copy = addr.GetInternetMulticastHost();

#if ENVIRONMENT == Linux
    StreamString dotName(inet_ntoa(copy->imr_multiaddr));
    if (dotName != addr.GetMulticastGroup()) {
        return false;
    }

#endif

    return true;
}

bool InternetHostTest::TestSize() {

    InternetHost addr;
    return addr.Size() == sizeof(InternetHostCore);
}

bool InternetHostTest::TestMulticastSize() {

    InternetHost addr;
    return addr.MulticastSize() == sizeof(InternetMulticastCore);
}

//bool InternetHostTest::Prova() {
//    InternetHost ih1;
//    in_addr_t aux = ih1.ConvertInterfaceNameToInterfaceAddress("eno2");
//    ih1.SetMulticastInterfaceAddress(aux);
//    printf("#########################\n");
//    printf("ih1.GetAddress() = %s\n", ih1.GetAddress().Buffer());
//    printf("ih1.GetAddressAsNumber() = %u\n", ih1.GetAddressAsNumber());
//    printf("ih1.GetHostName() = %s\n", ih1.GetHostName().Buffer());
//    printf("ih1.GetLocalAddress() = %s\n", ih1.GetLocalAddress());
//    printf("ih1.GetLocalHostName() = %s\n", ih1.GetLocalHostName());
//    printf("ih1.GetMulticastGroup() = %s\n", ih1.GetMulticastGroup().Buffer());
//    printf("ih1.GetMulticastInterfaceAddress() = %s\n", ih1.GetMulticastInterfaceAddress().Buffer());
//    printf("ih1.GetPort() = %u\n", ih1.GetPort());
//
//    //ih1.SetMulticastInterfaceAddress("192.168.130.46");
////    printf("ih1.GetMulticastGroup() = %s\n", ih1.GetMulticastGroup().Buffer());
////    printf("ih1.GetMulticastInterfaceAddress() = %s\n", ih1.GetMulticastInterfaceAddress().Buffer());
//
//    printf("#########################\n");
//    InternetHost ih2(44444, "192.168.130.46");
//    printf("ih2.GetAddress() = %s\n", ih2.GetAddress().Buffer());
//    printf("ih2.GetAddressAsNumber() = %u\n", ih2.GetAddressAsNumber());
//    printf("ih2.GetHostName() = %s\n", ih2.GetHostName().Buffer());
//    printf("ih2.GetLocalAddress() = %s\n", ih2.GetLocalAddress());
//    printf("ih2.GetLocalHostName() = %s\n", ih2.GetLocalHostName());
//    printf("ih2.GetMulticastGroup() = %s\n", ih2.GetMulticastGroup().Buffer());
//    printf("ih2.GetMulticastInterfaceAddress() = %s\n", ih2.GetMulticastInterfaceAddress().Buffer());
//    printf("ih2.GetPort() = %u\n", ih2.GetPort());
//
//    ih2.SetMulticastInterfaceAddress("192.168.130.46");
//    printf("ih2.GetMulticastGroup() = %s\n", ih2.GetMulticastGroup().Buffer());
//    printf("ih2.GetMulticastInterfaceAddress() = %s\n", ih2.GetMulticastInterfaceAddress().Buffer());
//    return false;
//
//}

