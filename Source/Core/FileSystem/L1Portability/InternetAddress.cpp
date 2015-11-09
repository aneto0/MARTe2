/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the 
 Licence.
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied.
 * See the Licence for the specific language governing 
 permissions and limitations under the Licence.
 *
 * $Id: InternetAddress.cpp 3 2012-01-15 16:26:07Z aneto $
 *
 **/

#include "InternetAddress.h"
#include "Sleep.h"
#include "Atomic.h"

namespace MARTe {

bool sockInitialized = false;
bool hostnameFastSemCreated = false;
FastPollingMutexSem hostnameFastSem;

bool _InternetAddressInfoInitialised = false;
int32 initialising = 0;




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
            SleepSec(0.01);
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
const char8 *InternetAddress::GetHostName(BString &hostName) {
    if (!hostnameFastSemCreated) {
        hostnameFastSem.Create();
        hostnameFastSemCreated = true;
    }

    hostnameFastSem.FastLock();
    DotName(hostName);

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


}
