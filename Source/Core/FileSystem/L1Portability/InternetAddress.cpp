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

bool                 hostnameFastSemCreated = False;
FastPollingMutexSem hostnameFastSem;

/** returns the host name (as a pointer to the BString buffer)
 * by searching the name server. NULL means failure*/
const char *GetHostName(InternetAddress &ia,BString &hostName){
    if(!hostnameFastSemCreated){
        hostnameFastSem.Create();
        hostnameFastSemCreated = True;
    }

    hostnameFastSem.FastLock();
    ia.DotName(hostName);

#if (defined(_WIN32) || defined (_SOLARIS) || defined(_LINUX) || defined(_MACOSX))
    struct hostent *h = gethostbyaddr((char *)&ia.address.sin_addr.s_addr,4,AF_INET);

// what's the point ??
// and it crashes if h is NULL
//       ia.address.sin_addr.s_addr = *((int *)(h->h_addr_list[0]));

    if (h != NULL){
        hostName = h->h_name;
    }
#elif defined(_RTAI)
    char h[256];
    fcomm_gethostbyaddr((void*)&ia.address.sin_addr.s_addr,h,256);
    hostName = h;
#elif defined(_VXWORKS)
    char hName[256];
    // Get the host name
    if (hostGetByAddr(ia.address.sin_addr.s_addr,hName)==OK){
        // Simulate the behaviour of gethostbyaddr()
        hostName = hName;
    }
#endif
    hostnameFastSem.FastUnLock();

    if (hostName.Size() == 0) return NULL;

    return hostName.Buffer();
}



#if defined(_WIN32)
WSADATA globalWsaData;

static inline void sock_init(){
//printf("socket initialised\n");
    WORD wVersionRequested;
    int iWinsockVersionMajor = 2;
    int iWinsockVersionMinor = 0;
    wVersionRequested = MAKEWORD(iWinsockVersionMajor, iWinsockVersionMinor);
    WSAStartup(wVersionRequested, &globalWsaData);
}

#else

static inline void sock_init(){
}
#endif

bool _InternetAddressInfoInitialised = False;
int32 initialising = 0;

class _InternetAddressInfo{
    ///
    const char *localAddress;
    ///
    const char *ipNumber;

    ///
    void Init(){
        if (_InternetAddressInfoInitialised ) {
	    return;
	}
        while (Atomic::TestAndSet(&initialising)) SleepSec(0.01);
        if (_InternetAddressInfoInitialised ) {
	    initialising = 0;
	    return;
	}


        localAddress = NULL;
        ipNumber     = NULL;

        if(!sockInitialized){
            sock_init();
            sockInitialized = True;
        }

#if defined(_WIN32) || defined (_LINUX) || defined(_MACOSX)
#if defined(_WIN32)
    struct hostent *h = gethostbyname(NULL);
#elif defined(_LINUX) || defined(_MACOSX)
    char hostname[128];
    int ret = gethostname(hostname, 128);
    struct hostent *h = NULL;
    if(ret == 0)
        h = gethostbyname(hostname);
#endif
    if (h == NULL) return ;
    localAddress = strdup(h->h_name);
    struct in_addr sin_addr;
    sin_addr.s_addr = *((int *)(h->h_addr_list[0]));

    // Convert the ip number in a dot notation string
    ipNumber = strdup(inet_ntoa(sin_addr));
#elif defined(_VXWORKS)
    {
        // Get the localhost name
        char hName[256];
        int nameLen = sizeof(hName);
        gethostname(hName,nameLen);
        localAddress = strdup(hName);
    }
    // Get the ip number (as an integer)
    int ipNumb = hostGetByName((char *)localAddress);
    if(ipNumb == ERROR) return;
    struct in_addr sin_addr;
    sin_addr.s_addr = ipNumb;

    // Convert the ip number in a dot notation string
    ipNumber = strdup(inet_ntoa(sin_addr));
#elif defined(_RTAI)
    char hostname[128];
    char ipaddr[128];
    int ret = fcomm_get_internetinfo(hostname,ipaddr,128);
    SleepSec(1.0);
    localAddress = strdup(hostname);
    ipNumber     = strdup(ipaddr);
    printf("Detected IP: %s [%s]\n",ipNumber,localAddress);
#else
    localAddress = strdup("unknown");
    ipNumber     = strdup("0.0.0.0");
#endif
    _InternetAddressInfoInitialised = True;
    initialising = 0;
    return;
}

public:
    //
    _InternetAddressInfo(){
        Init();
    }

    //
    ~_InternetAddressInfo(){
        if (ipNumber     != NULL)free((void *&)ipNumber);
        if (localAddress != NULL)free((void *&)localAddress);
    }
    //
    const char *GetLocalAddress(){
        Init();
        return localAddress;
    }
    ///
    const char *GetIpNumber(){
        Init();
        return ipNumber;
    }

} InternetAddressInfo;


const char *GetLocalAddress(){
    return InternetAddressInfo.GetLocalAddress();
}

const char *GetLocalIpNumber(){
    return InternetAddressInfo.GetIpNumber();
}


uint32 GetLocalAddressAsNumber(){
    uint32 comp[4];
    const char* name = InternetAddressInfo.GetIpNumber();
    if (name == NULL) return 0;
    sscanf(name,"%d.%d.%d.%d",&comp[3],&comp[2],&comp[1],&comp[0]);
    //sscanf(InternetAddressInfo.GetIpNumber(),"%d.%d.%d.%d",&comp[3],&comp[2],&comp[1],&comp[0]);
    uint32 address = (comp[0] + 256*(comp[1] + 256*(comp[2] + 256*comp[3])));
    return address;
}

bool sockInitialized = False;

static class SocketInitialization{
public:
    SocketInitialization(){
        Init();
    }
    void Init(){
        if (!sockInitialized) sock_init();
        sockInitialized = True;
    }
} socketInitialization;

void SocketInit(){
    socketInitialization.Init();
}

