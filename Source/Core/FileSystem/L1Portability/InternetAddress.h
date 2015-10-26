/**
 * @file InternetAddress.h
 * @brief Header file for class InternetAddress
 * @date 26/10/2015
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

 * @details This header file contains the declaration of the class InternetAddress
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTERNETADDRESS_H_
#define INTERNETADDRESS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#include "String.h"


namespace MARTe{

class InternetAddress {

friend class BasicTCPSocket;
friend class BasicUDPSocket;
friend class BasicSocket;
friend class BasicATMSocket;



public:
    /** creates an InternetAddress with the given information
        a NULL value for addr will select the INADDR_ANY  */
    InternetAddress(uint16 port=0,const char8 *addr=NULL);

    /**  returns the port number associated */
    int16 GetPort();

    /** returns the host name in the x.x.x.x format
        the return is pointer to the BString buffer */
    const char8 *GetDotName(String &dotName);

    /** returns the host name (as a pointer to the BString buffer)
     * by searching the name server. NULL means failure*/
    const char8 *GetHostName(String &hostName);


    /**  returns the host number associated to this InternetAddress*/
    uint32 GetHostNumber();

    /**  do not use to initialize objects. It is valid after static objects initializations */
    static const char8 *GetLocalAddress();


    /**  do not use to initialize objects. It is valid after static objects initializations */
    static const char8 *GetLocalIpNumber();

    static void SocketInit();

    /** The same as LocalAddress but returns a 32 bit integer*/
    static uint32 GetLocalAddressAsNumber();


private:

    /** */
    sockaddr_in address;

    /** sets the port value  */
    void SetPort(uint16 port);

    /** sets the address using a x.x.x.x notation
        a NULL value will select the INADDR_ANY
        it will return True on success */
    bool SetAddressByDotName(const char8 *addr);

    /**  The routine searches the NameServer for the name and gets the ip-number. returns True in case of success */
    bool SetAddressByName(const char8 *hostName);

    /**  The address number is set, the value passed must be in the internet format */
    void SetAddressByNumber(uint32 number);

    /**  The address of the local host */
    bool SetLocalAddress();

    /**  returns the sockaddr pointer*/
    struct sockaddr *GetAddress();

    /**   */
    uint32 Size();
};

}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTERNETADDRESS_H_ */

