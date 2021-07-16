/**
 * @file InternetHost.cpp
 * @brief Source file for class InternetHost
 * @date 28/10/2015
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
 * the class InternetHost (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <circle/net/dnsclient.h>
#include <circle/net/ipaddress.h>
#include <circle/net/netconfig.h>
#include <circle/string.h>
#include <circle/util.h>
#include <circle/logger.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ErrorManagement.h"
#include "InternetHost.h"
#include "InternetHostCore.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

extern char8* globalHostName;

class LocalHostInfo {
    private:
        //Here for the C++98 style singleton which has automagically
        //Single creation, destruction and lazy initialization
        LocalHostInfo(){}
        LocalHostInfo(LocalHostInfo const&);
        void operator=(LocalHostInfo const&);

        char8* ipDottedQuadString;
        uint8* ipQuadU8;

        /**
         * @brief   Refreshes internal variables which keep IP address in the different notations
         *          Called internally every time a request is done.
         */
        void RefreshData();

    public:
        /**
         * @brief Get the Singleton Instance object
         * 
         * @return LocalHostInfo& The singleton instance
         */
        static LocalHostInfo& GetInstance() {
            static LocalHostInfo instance;
            return instance;
        }

        /**
         * @brief Gets the pointer to the char8 string which contains the IP address in dotted quad notation
         * @return char8* Pointer to the char8 string or NULL if the operation fails
         */
        char8* GetIPDottedQuadString();

        /**
         * @brief Gets the pointer to the uint8 array which contains the IP address in quad u8 notation
         * @return uint8* Pointer to the uint8 array or NULL if the operation fails
         */
        uint8* GetIPQuadU8();
};

void LocalHostInfo::RefreshData() {
    bool returnVal = true;
    //TODO FIX HERE 
    CNetConfig* networkConfiguration = NULL;//networkSubSystem.GetConfig();
    
    if(networkConfiguration == NULL_PTR(CNetConfig*)) {
        returnVal = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "InternetHost::GetLocalAddress() Failed during network configuration retrieval");
    }

    const CIPAddress* ipAddress;

    if(returnVal) {
        ipAddress = networkConfiguration->GetIPAddress();
        if(ipAddress == NULL_PTR(CIPAddress*)) {
            returnVal = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "InternetHost::GetLocalAddress() Failed during IP address retrieval");
        }
    }

    if(returnVal) {
        if(ipQuadU8 != NULL_PTR(uint8*)) {
            delete [] ipQuadU8;
        }
        ipQuadU8 = new uint8[4];
        
        if(ipQuadU8 == NULL_PTR(uint8*)) {
            returnVal = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "InternetHost::GetLocalAddress() Failed during IP address to quad u8 array retrieval");
        }

        ipQuadU8[0] = ipAddress->Get()[0];
        ipQuadU8[1] = ipAddress->Get()[1];
        ipQuadU8[2] = ipAddress->Get()[2];
        ipQuadU8[3] = ipAddress->Get()[3];
    }

    if(returnVal) {
        ::CString tempIPAddressString;
        ipAddress->Format(&tempIPAddressString);
    }
    
    delete ipAddress;
}

char8* LocalHostInfo::GetIPDottedQuadString() {
    RefreshData();
    return ipDottedQuadString;
}

uint8* LocalHostInfo::GetIPQuadU8() {
    RefreshData();
    return ipQuadU8;
}

    /*---------------------------------------------------------------------------*/
    /*                           InternetHost                                    */
    /*---------------------------------------------------------------------------*/

    InternetHost::InternetHost(const uint16 port, const char8 * const addr) : address(addr, port) {
    }

    uint16 InternetHost::GetPort() const {
        return address.ipPort;
    }

    StreamString InternetHost::GetAddress() const {
        StreamString quadDotNotation;
        quadDotNotation.Printf( "%u.%u.%u.%u",
                                address.ipAddressQuadU8[0],
                                address.ipAddressQuadU8[1],
                                address.ipAddressQuadU8[2],
                                address.ipAddressQuadU8[3]);

        return quadDotNotation;
    }

    StreamString InternetHost::GetHostName() const {
        //Circle does not support the typical OS's gethostbyname() function
        //We always return an empty string which means failure
        StreamString noHost = "";
        return noHost;
    }

    uint32 InternetHost::GetAddressAsNumber() const {
        uint32 retVal = 0u;
        retVal = (address.ipAddressQuadU8[0] << 24) | (address.ipAddressQuadU8[1] << 16) | (address.ipAddressQuadU8[2] << 8) | address.ipAddressQuadU8[3];
        return retVal;
    }

    const char8 *InternetHost::GetLocalHostName() {
        //We return the hostname chosen at startup/configuration stage and globally stored
        //There is no need to do DNS querying or auto-resolution as the hostname can be only set by ourselves
        //(i.e. there is no OS or concurrent process which can change this during runtime)
        return "PiCircle";
    }

    const char8 *InternetHost::GetLocalAddress() {
        return "127.0.0.1";
    }

    uint32 InternetHost::GetLocalAddressAsNumber() {
        //TODO Take current ip address from the Circle library or HAL instead of local host
        uint32 retVal = 0u;
        retVal = (static_cast<uint8>(127) << 24) || (static_cast<uint8>(1));
        return retVal;
    }

    void InternetHost::SetPort(const uint16 port) {
        address.SetPort(port);
    }

    bool InternetHost::SetAddress(const char8 * const addr) {
        address.SetAddress(addr);
        return true;
    }

    bool InternetHost::SetAddressByHostName(const char8 * hostName) {
        return false;
    }

    void InternetHost::SetAddressByNumber(const uint32 number) {
        address.ipAddressQuadU8[3] = (number >> 24) && 0xFF;
        address.ipAddressQuadU8[2] = (number >> 16) && 0xFF;
        address.ipAddressQuadU8[1] = (number >> 8) && 0xFF;
        address.ipAddressQuadU8[0] = number && 0xFF;
    }

    /**  The address of the local host */
    bool InternetHost::SetLocalAddress() {
        return SetAddressByHostName(static_cast<const char8*>(NULL));
    }

    InternetHostCore *InternetHost::GetInternetHost() {
        return &address;
    }

    uint32 InternetHost::Size() const {
        return static_cast<uint32>(sizeof(address));
    }

}

