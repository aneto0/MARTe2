/**
 * @file InternetHost.h
 * @brief Header file for class InternetHost
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

 * @details This header file contains the declaration of the class InternetHost
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTERNETHOST_H_
#define INTERNETHOST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamString.h"
#include INCLUDE_FILE_ENVIRONMENT(FileSystem,L1Portability,ENVIRONMENT,InternetHostCore.h)
#include INCLUDE_FILE_ENVIRONMENT(FileSystem,L1Portability,ENVIRONMENT,InternetMulticastCore.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Class which describes the configuration of an internet host, like
 * its IPv4 address, port and host name.
 */
class DLL_API InternetHost {

public:

    /**
     * @brief Default constructor.
     * @param[in] port the port number.
     * @param[in] addr the IPv4 address in the format x.x.x.x.
     * @pre
     *    The address must be in the IPv4 format (3-digits numbers in [0-255] separated by dots).
     *    A wrong format => GetAddress()==0.0.0.0
     * @post
     *    GetPort() == port\n
     *    GetAddress() == addr
     */
    InternetHost(const uint16 port = 0u,
                 const char8 *const addr = static_cast<const char8*>(NULL_PTR(char8*)));

    /**
     * @brief Returns the port number.
     * @return the port number.
     */
    uint16 GetPort() const;

    /**
     * @brief Returns the IP address in the IPv4 format.
     * @return the IP address in the IPv4 format.
     */
    StreamString GetAddress() const;

    /**
     * @brief Returns the host name as a StreamString.
     * @return the host name as a StreamString. In case of failure the returned StreamString is empty.
     */
    StreamString GetHostName() const;

    /**
     * @brief Returns the IP address as an integer number.
     * @return the IP address as an integer number.
     */
    uint32 GetAddressAsNumber() const;

    /**
     * @brief Returns the host name of the machine where the library is being executed.
     * @return the host name of the machine where the library is being executed.
     */
    static const char8* GetLocalHostName();

    /**
     * @brief Returns the IP address of the machine where the library is being executed in the IPv4 format.
     * @return the IP address of the machine where the library is being executed in the IPv4 format.
     */
    static const char8* GetLocalAddress();

    /**
     * @brief Returns the IP address a.b.c.d as [a + 256*b + (256^2)*c + (256^3)*d]
     * @return the IP address a.b.c.d as [a + 256*b + (256^2)*c + (256^3)*d]
     */
    static uint32 GetLocalAddressAsNumber();

    /**
     * @brief Gets the MAC address for the given network interface.
     * @note On some particular implementations where no OS (i.e. BareMetal) or no MAC retrieval primitives (i.e. FreeRTOS), the function call may require the implementation of an
     * extern hook to work.
     *
     * @param[in] interfaceName name of the network interface as seen by the underlying OS.
     * @param[out] mac MAC address of the given interface.
     * @return true if the MAC address was correctly retrieved, false otherwise.
     */
    static bool GetMACAddress(const char8 *const interfaceName,
                              uint8 *const mac);

    /**
     * @brief Translate interfaceName to internet address (i.e "enp0s25" --> 2676074688).
     * @param[in] interfaceName interface name to be converted to internet address.
     * @return the internet address number corresponding to interfaceName. If interfaceName does not exist it returns 0
     */
    static InternetAddress ConvertInterfaceNameToInterfaceAddressNumber(const char8 *const interfaceName);

    /**
     * @brief Translate interfaceName to internet address (i.e "enp0s25" --> 192.168.129.44).
     * @param[in] interfaceName interface name to be converted to internet address.
     * @return the internet address number corresponding to interfaceName. If interfaceName does not exist it returns "0.0.0.0"
     */
    static StreamString ConvertInterfaceNameToInterfaceAddress(const char8 *const interfaceName);

    /**
     * @brief Sets the port number.
     * @param[in] port is the desired port number.
     * @post
     *   GetPort() == port
     */
    void SetPort(const uint16 port);

    /**
     * @brief Sets the IP address in the IPv4 format.
     * @param[in] addr the desired IP number.
     * @return false if the input format is incorrect.
     * @pre
     *    addr must be in the IPv4 format (3-digits numbers in [0-255] separated by dots).
     */
    bool SetAddress(const char8 *const addr);

    /**
     * @brief Sets the host name.
     * @param[in] hostName the host name to be set.
     * @return true if the host name is set correctly, false otherwise.
     */
    bool SetAddressByHostName(const char8 *hostName);

    /**
     * @brief Set the IP address a.b.c.d passing the equivalent input [a + 256*b + (256^2)*c + (256^3)*d].
     * @param[in] number is the IP address in unsigned int format.
     */
    void SetAddressByNumber(const uint32 number);

    /**
     * @brief Sets the address of the local hostname as "localhost"
     * @return true if the local host name is set correctly, false otherwise.
     * @post
     *    GetLocalHostName() == "localhost"
     */
    bool SetLocalAddress();

    /**
     * @brief Set the IP address a.b.c.d belonging to a multicast group passing the equivalent input [a + 256*b + (256^2)*c + (256^3)*d].
     * @param[in] addr is the multicast IP address
     */
    void SetMulticastGroup(const char8 *const addr);

    /**
     * @brief The address of the interface to which to bind the multicast.
     * @param[in] addr the network address of the interface where to bind the multicast.
     */
    void SetMulticastInterfaceAddress(const char8 *const addr);

    /**
     * @brief The address of the interface to which to bind the multicast.
     * @param[in] interfaceAddr the interface address where to bind the multicast. addr = ConvertInterfaceNameToInterfaceAddress(interfaceName)
     */
    void SetMulticastInterfaceAddress(const InternetAddress interfaceAddr);

    /**
     * @brief Returns the multicast address of the bounding interface.
     */
    StreamString GetMulticastInterfaceAddress() const;

    /**
     * @brief Returns the Multicast address on a StreamString object
     */
    StreamString GetMulticastGroup() const;

    /**
     * @brief Returns access to the low-level handle.
     * @return a pointer to the handle of this structure containing
     * the InternetHostCore information.
     * @remark The handle type definition is specific to the operating system.
     */
    InternetHostCore* GetInternetHost();

    /**
     * @brief Returns access to the low-level handle.
     * @return a pointer to the handle of this structure containing
     * the InternetMulticastCore information.
     * @remark The handle type definition is specific to the operating system.
     */
    InternetMulticastCore* GetInternetMulticastHost();

    /**
     * @brief Gets the size in bytes of the handle.
     * @return the size of InternetHostCore handle.
     */
    uint32 Size() const;

    /**
     * @brief Gets the size in bytes of the handle.
     * @return the size of InternetMulticastCore handle.
     */
    uint32 MulticastSize() const;

private:

    /**
     * The InternetHost handle.
     */
    InternetHostCore address;

    /**
     * The InternetMulticastCore handle.
     */
    InternetMulticastCore mreq;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTERNETHOST_H_ */

