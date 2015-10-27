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
#include "String.h"
#include INCLUDE_FILE_ENVIRONMENT(ENVIRONMENT,InternetAddressCore.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/



namespace MARTe{


/**
 * @brief A class to store IP address, port and host name associated to a generic host.
 */
class InternetAddress {


public:

    /**
     * @brief Default constructor.
     * @param[in] port is the desired port number.
     * @param[in] addr is the desired IP address in the format x.x.x.x
     * @pre The IP address should be made by four 3-digits numbers in [0-255] separated by dots.
     * @post
     * address.(port)=port\n
     * address.(IP address)=addr
     */
    InternetAddress(const uint16 port=0u,const char8 * const addr=static_cast<const char8 *>(NULL));

    /**
     * @brief Retrieves the port number.
     */
    uint16 GetPort() const;

    /**
     * @brief Returns the host name in the x.x.x.x format as a String
     */
    String GetDotName() const;

    /**
     * @brief Returns the host name as a String. In case of failure the String returned is empty.
     */
    String GetHostName() const;


    /**
     * @brief Returns the host number associated to this InternetAddress.
     */
    uint32 GetHostNumber() const;

    /**
     * @brief Retrieves the local host name.
     */
    static const char8 *GetLocalAddress();


    /**
     * @brief Retrieves the local IP address in the format x.x.x.x
     */
    static const char8 *GetLocalIpNumber();

    /**
     * @brief Initializes the socket.
     */
    static void SocketInit();

    /**
     * @brief Retrieves the IP address a.b.c.d as [d + 256*c + (256^2)*b + (256^3)*a]
     */
    static uint32 GetLocalAddressAsNumber();



    /**
     * @brief Sets the port number.
     * @param[in] port is the desired port number.
     */
    void SetPort(const uint16 port);

    /**
     * @brief Sets the IP address passing a string with the format x.x.x.x
     * @param[in] addr is the desired IP number.
     * @return false if the input format is wrong, true otherwise.
     * @pre addr must have the format x.x.x.x with x in [0-255].
     */
    bool SetAddressByDotName(const char8 * const addr);

    /**
     * @brief Sets the desired host name.
     * @param[in] hostName is the desired host name.
     * @return true if the host name is set correctly, false otherwise.
     */
    bool SetAddressByName(const char8 * hostName);

    /**
     * @brief Set the IP address a.b.c.d passing the equivalent input [d + 256*c + (256^2)*b + (256^3)*a].
     * @param[in] number is the IP address in unsigned int format.
     */
    void SetAddressByNumber(const uint32 number);

    /**
     * @brief Sets the address of the local host, putting the host name equal to "localhost"
     * @return true if the local host name is set correctly, false otherwise.
     */
    bool SetLocalAddress();

    /**
     * @brief Full access to the handle.
     * @return a pointer to the handle of this structure containing all
     * the Internet informations.
     * @warning The handle type depends by the operating system below.
     */
    InternetAddressCore *GetAddress();

    /**
     * @brief Gets the size in bytes of the handle.
     * @return the size of InternetAddressCore.
     */
    uint32 Size() const;


private:

    /**
     * The InternetAddress handle.
     */
    InternetAddressCore address;

};

}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTERNETADDRESS_H_ */

