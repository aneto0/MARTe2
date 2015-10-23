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
 * $Id: InternetAddress.h 3 2012-01-15 16:26:07Z aneto $
 *
**/

/** 
 * @file
 * Holder for a full internet address i.e. with an URL address and a port number
 */
#if !defined (INTERNET_ADDRESS_H)
#define INTERNET_ADDRESS_H

#include "System.h"
#include "FastPollingMutexSem.h"
#include "BString.h"
#include "ErrorManagement.h"


class InternetAddress;

extern "C" {

    /** */
    extern bool sockInitialized;

    /** */
    const char *GetLocalAddress();

    /** */
    const char *GetLocalIpNumber();

    /** returns the host name (as a pointer to the BString buffer)
     * by searching the name server. NULL means failure*/
    const char *GetHostName(InternetAddress &ia,BString &hostName);

    /** */
    uint32 GetLocalAddressAsNumber();

    /** to be used only within BaseLib !*/
    void SocketInit();

}

class InternetAddress {

friend class BasicTCPSocket;
friend class BasicUDPSocket;
friend class BasicSocket;
friend class BasicATMSocket;


    friend const char *GetHostName(InternetAddress &ia,BString &hostName);

protected:

    /** */
    sockaddr_in address;

    /** sets the port value  */
    void SetPort(uint16 port){
        address.sin_port = htons(port);
    }

    /** sets the address using a x.x.x.x notation
        a NULL value will select the INADDR_ANY
        it will return True on success */
    bool SetAddressByDotName(const char *addr){
        address.sin_addr.s_addr = INADDR_ANY;
        if (addr != NULL) {
            uint32 iaddr = inet_addr((char *)addr);
            if (iaddr != 0xFFFFFFFF) address.sin_addr.s_addr = iaddr;
            else return False;
        }
        return True;
    }

    /**  The routine searches the NameServer for the name and gets the ip-number. returns True in case of success */
    bool SetAddressByName(const char *hostName){
#if defined(_LINUX) || defined(_RTAI) || defined(_MACOSX)
        if(hostName == NULL){
            hostName = "localhost";
        }
#endif
#if (defined(_WIN32) || defined(_SOLARIS) || defined(_LINUX) || defined(_MACOSX))
        //  hostName can be NULL meaning localhost
        struct hostent *h = gethostbyname(hostName);
        if (h == NULL) return False;
        address.sin_addr.s_addr = *((int *)(h->h_addr_list[0]));
        return True;
#elif defined(_RTAI)
        int h = fcomm_gethostbyname(hostName);
	if (h==-1) return False;
	address.sin_addr.s_addr = h;
	return True;
#elif defined(_VXWORKS)
        address.sin_addr.s_addr = INADDR_ANY;
        // Get the ip number (as an integer) and check for the existence of the host
        int ipNumb = hostGetByName((char *)hostName);
        if (ipNumb == ERROR) return False;
        address.sin_addr.s_addr = ipNumb;
	return True;
#endif
        return False;
    }

    /**  The address number is set, the value passed must be in the internet format */
    void SetAddressByNumber(uint32 number){
        address.sin_addr.s_addr = (int)number;
    }

    /**  The address of the local host */
    bool SetLocalAddress(){
        return SetAddressByName(NULL);
    }

    /**  returns the sockaddr pointer*/
    struct sockaddr *Address() {
        return (struct sockaddr *)&address;
    }

    /**   */
    uint32 Size() {
        return sizeof (address);
    }

public:
    /** creates an InternetAddress with the given information
        a NULL value for addr will select the INADDR_ANY  */
    InternetAddress(uint16 port=0,const char *addr=NULL){

        address.sin_family      = AF_INET;
        SetPort(port);
        address.sin_addr.s_addr = INADDR_ANY;
        if (addr != NULL) address.sin_addr.s_addr = inet_addr((char *)addr);
    }


    /**  returns the port number associated */
    int16 Port(){
        return htons(address.sin_port);
    }

    /** returns the host name in the x.x.x.x format
        the return is pointer to the BString buffer */
    const char *DotName(BString &dotName){
#ifndef _RTAI    
        dotName = inet_ntoa(address.sin_addr);
#else
	char buffer[64];
	inet_ntoa(buffer, 64, address.sin_addr);
	dotName = buffer;
#endif
        return dotName.Buffer();
    }

    /** returns the host name (as a pointer to the BString buffer)
     * by searching the name server. NULL means failure*/
    const char *HostName(BString &hostName){
        return GetHostName(*this,hostName);
    }

    /**  returns the host number associated to this InternetAddress*/
    uint32 HostNumber() {
        return (uint32) address.sin_addr.s_addr;
    }

    /**  do not use to initialize objects. It is valid after static objects initializations */
    static const char *LocalAddress(){
        return GetLocalAddress();
    }


    /**  do not use to initialize objects. It is valid after static objects initializations */
    static const char *LocalIpNumber(){
        return GetLocalIpNumber();
    }

    /** The same as LocalAddress but returns a 32 bit integer*/
    static uint32 LocalAddressAsNumber(){
        return GetLocalAddressAsNumber();
    }

};




#endif
