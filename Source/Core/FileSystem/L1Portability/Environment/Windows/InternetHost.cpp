/**
 * @file InternetHost.cpp
 * @brief Source file for class InternetHost
 * @date Nov 24, 2015
 * @author Frank Perez Paz
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
 * the class InternetHost (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
#include <stdio.h>
#include <Windows.h>
#include <Iphlpapi.h>
#include <Assert.h>
#pragma comment(lib, "iphlpapi.lib")
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ErrorManagement.h"
#include "FastPollingMutexSem.h"
#include "InternetHost.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

FastPollingMutexSem hostnameFastSem;

class DLL_API LocalHostInfo {
public:

    static LocalHostInfo *Instance() {
        static LocalHostInfo instance;
        return &instance;
    }

    ~LocalHostInfo() {
        if (ipAddress != NULL) {
            free(reinterpret_cast<void *>(const_cast<char8 *>(ipAddress)));
        }
        if (localHostName != NULL) {
            free(reinterpret_cast<void *>(const_cast<char8 *>(localHostName)));
        }
    }

    const char8 *GetLocalHostName() {
        Init();
        return localHostName;
    }

    const char8 *GetIpAddress() {
        Init();
        return ipAddress;
    }

    bool Initialized() const {
        return internetAddressInfoInitialised;
    }
private:
    const char8 *localHostName;
    const char8 *ipAddress;
    bool internetAddressInfoInitialised;
    FastPollingMutexSem internalFastSem;

    LocalHostInfo():localHostName(static_cast<const char8*>(NULL)),ipAddress(static_cast<const char8*>(NULL)),internetAddressInfoInitialised(false),internalFastSem() {
        Init();
    }

    void Init() {

        WSADATA wsaData;
        int iResult;
        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError,"LocalHostInfo: Failed WSAStartup");
        }

        if (!internetAddressInfoInitialised) {

            if(internalFastSem.FastLock()!=ErrorManagement::NoError) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError,"LocalHostInfo: Failed FastPollingMutexSem::FastLock() in initialization of local address");
            }

            localHostName = static_cast<const char8*>(NULL);
            ipAddress = static_cast<const char8*>(NULL);

            char8 hostname[128];
            int32 ret = gethostname(&hostname[0], static_cast<osulong>(128u));
            struct hostent *h = static_cast<struct hostent *>(NULL);
            if (ret == 0) {
                h = gethostbyname(&hostname[0]);
            }
            if (h != NULL) {
                localHostName = StringHelper::StringDup(h->h_name);
                struct in_addr sin_addr;
                char8* addr = h->h_addr_list[0];
                if(addr != static_cast<char8 *>(NULL)) {
                    sin_addr.S_un.S_addr = *(reinterpret_cast<uint32 *> (addr));

                    // Convert the ip number in a dot notation string
                    ipAddress = StringHelper::StringDup(inet_ntoa(sin_addr));
                    internetAddressInfoInitialised = true;
                    internalFastSem.FastUnLock();
                }
                else {
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError,"LocalHostInfo: Failed local address initialization");
                }
            }
            else {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError,"LocalHostInfo: Failed local address initialization");
            }
        }
        return;
    }
};

StreamString InternetHost::GetHostName() const {

    if (hostnameFastSem.FastLock() != ErrorManagement::NoError) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "InternetHost: Failed FastPollingMutexSem::FastLock() in initialization of local address");
    }

    StreamString hostName = GetAddress();

    struct hostent *h = gethostbyaddr(reinterpret_cast<const char8 *>(&address.sin_addr.s_addr), 4, AF_INET);

    if (h != NULL) {
        hostName = h->h_name;
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError,"InternetHost: Failed gethostbyaddr()");
    }
    hostnameFastSem.FastUnLock();

    return (hostName.Size() == 0u) ? (static_cast<const char8 *>(NULL)):(hostName);
}

const char8 *InternetHost::GetLocalHostName() {
    return LocalHostInfo::Instance()->GetLocalHostName();
}

const char8 *InternetHost::GetLocalAddress() {
    return LocalHostInfo::Instance()->GetIpAddress();
}

uint32 InternetHost::GetLocalAddressAsNumber() {
    uint32 ret = 0u;
    uint32 comp[4];
    const char8* name = LocalHostInfo::Instance()->GetIpAddress();
    if (name != NULL) {
        sscanf(name, "%u.%u.%u.%u", &comp[3], &comp[2], &comp[1], &comp[0]);
        uint32 addressN = (comp[3] + (256u * (comp[2] + (256u * (comp[1] + (256u * comp[0]))))));
        ret= addressN;
    }
    return ret;
}
bool InternetHost::GetMACAddress(const char8 *const interfaceName,
                                 uint8 *const mac) {


  bool ret=true;
  PIP_ADAPTER_INFO AdapterInfo;
  DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
  char *mac_addr = (char*)malloc(18);

  AdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof(IP_ADAPTER_INFO));
  if (AdapterInfo == NULL) {
    printf("Error allocating memory needed to call GetAdaptersinfo\n");
    free(mac_addr);
    ret=false; // it is safe to call free(NULL)
  }

  if(ret){
  // Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen variable
  if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
    free(AdapterInfo);
    AdapterInfo = (IP_ADAPTER_INFO *) malloc(dwBufLen);
    if (AdapterInfo == NULL) {
      printf("Error allocating memory needed to call GetAdaptersinfo\n");
      free(mac_addr);
       ret=false;
    }
  }
  }

  if(ret){
  if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
    // Contains pointer to current adapter info
    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
    ret=false;
    do {
      // technically should look at pAdapterInfo->AddressLength
      //   and not assume it is 6.
      sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
        pAdapterInfo->Address[0], pAdapterInfo->Address[1],
        pAdapterInfo->Address[2], pAdapterInfo->Address[3],
        pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
        printf("Address: %s, mac: %s\n", pAdapterInfo->IpAddressList.IpAddress.String, mac_addr);
        if(interfaceName==pAdapterInfo->AdapterName){
            int32 i;
            for (i = 0; i < 6; ++i) {
                uint8 auxChar = static_cast<uint8>(pAdapterInfo->Address[i]);
                mac[i] = auxChar;
            }
            ret=true;
            break;
        }

      pAdapterInfo = pAdapterInfo->Next;        
    } while(pAdapterInfo);                        
  }
  }
  if(ret){
     free(AdapterInfo);
     free(mac_addr);
  }

    return ret;
}

/*

WORD wVersionRequested;
      WSADATA wsaData;
      char name[255];
      CString ip;
      PHOSTENT hostinfo;
      wVersionRequested = MAKEWORD( 2, 0 );

      if ( WSAStartup( wVersionRequested, &wsaData ) == 0 )
      {

            if( gethostname ( name, sizeof(name)) == 0)
            {
                  if((hostinfo = gethostbyname(name)) != NULL)
                  {
                        ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
                  }
            }

            WSACleanup( );
      } 
*/


InternetAddress InternetHost::ConvertInterfaceNameToInterfaceAddressNumber(const char8 *const interfaceName) {
    InternetAddress retVal = 0u;
  /*  int32 fd;
    struct ifreq ifr;
    bool ret = MemoryOperationsHelper::Set(&ifr, static_cast<char8>(0), static_cast<uint32>(sizeof(ifr)));
    InternetAddress retVal = 0u;
    //lint -e{641} Converting enum '__socket_type' to 'int. Definition and function outside the MARTe library. SOCK_DGRAM is a int type.
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    //Type of address to retrieve - IPv4 IP address
    ifr.ifr_addr.sa_family = static_cast<uint16>(AF_INET);
    //Copy the interface name in the ifreq structure
    if (ret) {
        ret = MemoryOperationsHelper::Copy(reinterpret_cast<void*>(ifr.ifr_name), interfaceName, static_cast<uint32>(IFNAMSIZ - 1));
    }
    if (ret) {
        if (ioctl(fd, static_cast<uint64>(SIOCGIFADDR), &ifr) != -1) {
            //lint -e{740} Unusual pointer cast (incompatible indirect types) [MISRA C++ Rule 5-2-6], [MISRA C++ Rule 5-2-7]
            in_addr addr = (reinterpret_cast<struct sockaddr_in*>(&ifr.ifr_addr))->sin_addr;
            retVal = addr.s_addr;
        }
    }
    close(fd);
*/
    return retVal;
}

StreamString InternetHost::ConvertInterfaceNameToInterfaceAddress(const char8 *const interfaceName) {
    in_addr aux;
    aux.s_addr = ConvertInterfaceNameToInterfaceAddressNumber(interfaceName);
    StreamString dotName(inet_ntoa(aux));
    return dotName;
}

InternetHost::InternetHost(const uint16 port,
                           const char8 * const addr) {
    WSADATA wsaData;
    int32 iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "LocalHostInfo: Failed WSAStartup");
    }
    address.sin_family = static_cast<uint16>(AF_INET);
    SetPort(port);

    if (!SetAddress(addr)) {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "InternetHost: Failed SetAddress");
    }
}

uint16 InternetHost::GetPort() const {
    return htons(address.sin_port);
}

StreamString InternetHost::GetAddress() const {
    StreamString dotName(inet_ntoa(address.sin_addr));
    return dotName;
}

uint32 InternetHost::GetAddressAsNumber() const {
    return static_cast<uint32>(address.sin_addr.s_addr);
}

void InternetHost::SetPort(const uint16 port) {
    address.sin_port = htons(port);
}

bool InternetHost::SetAddress(const char8 * const addr) {
    address.sin_addr.s_addr = INADDR_ANY;
    bool ret = true;
    if (addr != NULL) {
        uint32 iaddr = inet_addr(const_cast<char8 *>(addr));
        if (iaddr != INADDR_NONE) {
            address.sin_addr.S_un.S_addr = iaddr;
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "InternetHost: Failed inet_addr(), address=0xFFFFFFFF");
            ret = false;
        }
    }
    return ret;
}

bool InternetHost::SetAddressByHostName(const char8 * hostName) {
    bool ret = false;
    //  hostName can be NULL meaning localhost

    if (hostName == NULL) {
        hostName = "localhost";
    }
    struct hostent *h = gethostbyname(hostName);

    if (h != NULL) {
        address.sin_addr.s_addr = *(reinterpret_cast<uint32 *>(h->h_addr_list[0]));
        ret= true;
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError,"InternetHost: Failed gethostbyname()");
    }
    return ret;
}

void InternetHost::SetMulticastGroup(const char8 *const addr) {
    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "SetMulticastGroup not implemented");
}

void InternetHost::SetMulticastInterfaceAddress(const char8 *const addr) {
    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "SetMulticastInterfaceAddress not implemented");
}

void InternetHost::SetMulticastInterfaceAddress(const InternetAddress interfaceAddr) {
    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "SetMulticastInterfaceAddress not implemented");
}

void InternetHost::SetAddressByNumber(const uint32 number) {
    address.sin_addr.s_addr = number;
}

bool InternetHost::SetLocalAddress() {
    return SetAddressByHostName(static_cast<const char8*>(NULL));
}

InternetHostCore *InternetHost::GetInternetHost() {
    return &address;
}

uint32 InternetHost::Size() const {
    return static_cast<uint32>(sizeof(address));
}

StreamString InternetHost::GetMulticastGroup() const {
    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "GetMulticastGroup not implemented");
    return "";
}

StreamString InternetHost::GetMulticastInterfaceAddress() const {
    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "GetMulticastInterfaceAddress not implemented");
    return "";
}

InternetMulticastCore* InternetHost::GetInternetMulticastHost() {
    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "GetInternetMulticastHost not implemented");
    return NULL_PTR(InternetMulticastCore*);
}

uint32 InternetHost::MulticastSize() const {
    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Multicast size not implemented");
    return 0u;
}

}
