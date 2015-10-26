/**
 * @file InternetAddress.cpp
 * @brief Source file for class InternetAddress
 * @date 26/ott/2015
 * @author pc
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
 * the class InternetAddress (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <arpa/inet.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "InternetAddress.h"
#include "Sleep.h"
#include "Atomic.h"
#include "FastPollingMutexSem.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/





namespace MARTe {

bool sockInitialized = false;
bool hostnameFastSemCreated = false;
FastPollingMutexSem hostnameFastSem;

bool _InternetAddressInfoInitialised = false;
int32 initialising = 0;

static inline void sock_init(){
}


class _InternetAddressInfo {
    ///
    const char8 *localAddress;
    ///
    const char8 *ipNumber;

    ///
    void Init() {
        if (_InternetAddressInfoInitialised) {
            return;
        }
        while (Atomic::TestAndSet(&initialising))
            Sleep::Sec(0.01);
        if (_InternetAddressInfoInitialised) {
            initialising = 0;
            return;
        }

        localAddress = NULL;
        ipNumber = NULL;

        if (!sockInitialized) {
            sock_init();
            sockInitialized = true;
        }

        char8 hostname[128];
        int ret = gethostname(hostname, 128);
        struct hostent *h = NULL;
        if (ret == 0)
            h = gethostbyname(hostname);
        if (h == NULL) return;
        localAddress = strdup(h->h_name);
        struct in_addr sin_addr;
        sin_addr.s_addr = *((int *) (h->h_addr_list[0]));

        // Convert the ip number in a dot notation string
        ipNumber = strdup(inet_ntoa(sin_addr));
        _InternetAddressInfoInitialised = true;
        initialising = 0;
        return;
    }

public:
    //
    _InternetAddressInfo() {
        Init();
    }

    //
    ~_InternetAddressInfo() {
        if (ipNumber != NULL)free((void *&)ipNumber);
        if (localAddress != NULL)free((void *&)localAddress);
    }
    //
    const char8 *GetLocalAddress() {
        Init();
        return localAddress;
    }
    ///
    const char8 *GetIpNumber() {
        Init();
        return ipNumber;
    }

}InternetAddressInfo;

/** returns the host name (as a pointer to the BString buffer)
 * by searching the name server. NULL means failure*/
const char8 *InternetAddress::GetHostName(String &hostName) {
    if (!hostnameFastSemCreated) {
        hostnameFastSem.Create();
        hostnameFastSemCreated = true;
    }

    hostnameFastSem.FastLock();
    GetDotName(hostName);

    struct hostent *h = gethostbyaddr((char8 *) address.sin_addr.s_addr, 4, AF_INET);

// what's the point ??
// and it crashes if h is NULL
//       ia.address.sin_addr.s_addr = *((int *)(h->h_addr_list[0]));

    if (h != NULL) {
        hostName = h->h_name;
    }
    hostnameFastSem.FastUnLock();

    if (hostName.Size() == 0)
        return NULL;

    return hostName.Buffer();
}

const char8 *InternetAddress::GetLocalAddress() {
    return InternetAddressInfo.GetLocalAddress();
}

const char8 *InternetAddress::GetLocalIpNumber() {
    return InternetAddressInfo.GetIpNumber();
}

uint32 InternetAddress::GetLocalAddressAsNumber() {
    uint32 comp[4];
    const char8* name = InternetAddressInfo.GetIpNumber();
    if (name == NULL) return 0;
    sscanf(name, "%d.%d.%d.%d", &comp[3], &comp[2], &comp[1], &comp[0]);
    //sscanf(InternetAddressInfo.GetIpNumber(),"%d.%d.%d.%d",&comp[3],&comp[2],&comp[1],&comp[0]);
    uint32 address = (comp[0] + 256 * (comp[1] + 256 * (comp[2] + 256 * comp[3])));
    return address;
}

InternetAddress::InternetAddress(uint16 port,
                                 const char8 *addr ) {

    address.sin_family = AF_INET;
    SetPort(port);
    address.sin_addr.s_addr = INADDR_ANY;
    if (addr != NULL) address.sin_addr.s_addr = inet_addr((char8 *)addr);
}

/**  returns the port number associated */
int16 InternetAddress::GetPort() {
    return htons(address.sin_port);
}

/** returns the host name in the x.x.x.x format
 the return is pointer to the BString buffer */
const char8 *InternetAddress::GetDotName(String &dotName) {
    dotName = inet_ntoa(address.sin_addr);
    return dotName.Buffer();
}

/**  returns the host number associated to this InternetAddress*/
uint32 InternetAddress::GetHostNumber() {
    return (uint32) address.sin_addr.s_addr;
}

/** sets the port value  */
void InternetAddress::SetPort(uint16 port) {
    address.sin_port = htons(port);
}

/** sets the address using a x.x.x.x notation
 a NULL value will select the INADDR_ANY
 it will return true on success */
bool InternetAddress::SetAddressByDotName(const char8 *addr) {
    address.sin_addr.s_addr = INADDR_ANY;
    if (addr != NULL) {
        uint32 iaddr = inet_addr((char8 *)addr);
        if (iaddr != 0xFFFFFFFF) address.sin_addr.s_addr = iaddr;
        else return false;
    }
    return true;
}

/**  The routine searches the NameServer for the name and gets the ip-number. returns true in case of success */
bool InternetAddress::SetAddressByName(const char8 *hostName) {
    if (hostName == NULL) {
        hostName = "localhost";
    }
    //  hostName can be NULL meaning localhost
    struct hostent *h = gethostbyname(hostName);
    if (h == NULL) return false;
    address.sin_addr.s_addr = *((int *)(h->h_addr_list[0]));
    return true;
}

/**  The address number is set, the value passed must be in the internet format */
void InternetAddress::SetAddressByNumber(uint32 number) {
    address.sin_addr.s_addr = (int) number;
}

/**  The address of the local host */
bool InternetAddress::SetLocalAddress() {
    return SetAddressByName(NULL);
}

/**  returns the sockaddr pointer*/
struct sockaddr *InternetAddress::GetAddress() {
    return (struct sockaddr *) &address;
}

/**   */
uint32 InternetAddress::Size() {
    return sizeof(address);
}

}
