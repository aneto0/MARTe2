/**
 * @file BasicUDPSocket.h
 * @brief Header file for class BasicUDPSocket
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

 * @details This header file contains the declaration of the class BasicUDPSocket
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICUDPSOCKET_H_
#define BASICUDPSOCKET_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "BasicSocket.h"
#include "Sleep.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Class which represents a datagram network socket, also known as
 * connectionless socket, which use User Datagram Protocol (UDP).
 * @details The main responsibilities of this class are:
 * + Establishing a connection with a remote host.
 * + Binding the socket to a local port and begin listening from it.
 * + Allowing reading/writing using a generic stream interface approach.
 * @remark Although being stated as a connection-less socket, it actually adopts
 * a connection-oriented role, because it remembers the destination host and
 * uses it each time a data packet is sent. Nevertheless, as a datagram socket,
 * the order and reliability of the data packets are not guaranteed, hence no
 * buffering of the data packets is expected to happen.
 */
class DLL_API BasicUDPSocket: public BasicSocket {
public:

    /**
     * @brief Default constructor.
     */
    BasicUDPSocket();

    /**
     * @brief Destructor.
     */
    virtual ~BasicUDPSocket();

    /**
     * @see StreamI::Read
     */
    virtual bool Read(char8 *const output,
                      uint32 &size);

    /**
     * @brief Read without removing data from the socket pipe.
     * @param[out] output is the buffer used to store the read data.
     * @param[in,out] size is the number of bytes to read.
     * @return false in case of errors.
     * @post
     *   size is the number of read bytes.
     */
    virtual bool Peek(char8 *const output,
                      uint32 &size);

    /**
     * @see StreamI::Write
     */
    virtual bool Write(const char8 *const input,
                       uint32 &size);

    /**
     * @brief Opens an UDP socket.
     * @return true if the socket is successfully initialised.
     */
    bool Open();

    /**
     * @brief Sets the port where the socket will listen from.
     * @param[in] port the port number.
     * @return true if the socket is successfully bind into the \a port.
     */
    bool Listen(const uint16 port);

    /**
     * @brief Sets the multicast group where the socket will register itself.
     * @details It will bind to the default IPv4 multicast address (see Join with multicastInterfaceAddress).
     * @param[in] group the multicast IP address.
     * @return true if the socket is successfully registered to the multicast group.
     */
    bool Join(const char8 *const group) const;

    /**
     * @brief Sets the multicast group where the socket will register, allowing to specify the network interface binding address.
     * @param[in] group the multicast IP address.
     * @param[in] multicastInterfaceAddress the address of the interface to which to bind the multicast.
     * @return true if the socket is successfully registered to the multicast group.
     */
    bool Join(const char8 *const group,
              const char8 *const multicastInterfaceAddress) const;

    /**
     * @brief Sets the writing destination address.
     * @param[in] address the destination IP address.
     * @param[in] port the destination port.
     * @return true if the address is successfully set.
     * @pre
     *   address must have the IPv4 format x.x.x.x
     * @post
     *   GetDestination() == InternetHost(port, address)
     */
    bool Connect(const char8 *const address,
                 const uint16 port);

    /**
     * @brief The UDP socket support writing.
     * @return true.
     */
    virtual bool CanWrite() const;

    /**
     * @brief The UDP socket support reading.
     * @return true.
     */
    virtual bool CanRead() const;

    /**
     * @brief The UDP socket does not support seeking.
     * @return false.
     */
    virtual bool CanSeek() const;

    /**
     * @see StreamI::Read
     */
    virtual bool Read(char8 *const output,
                      uint32 &size,
                      const TimeoutType &timeout);

    /**
     * @see StreamI::Write
     */
    virtual bool Write(const char8 *const input,
                       uint32 &size,
                       const TimeoutType &timeout);

    /**
     * @brief Unsupported feature.
     * @return 0xFFFFFFFFFFFFFFFF.
     */
    virtual uint64 Size();

    /**
     * @brief Unsupported feature.
     * @return false.
     */
    virtual bool Seek(uint64 pos);

    /**
     * @brief Unsupported feature.
     * @return false.
     */
    virtual bool RelativeSeek(const int64 deltaPos);

    /**
     * @brief Unsupported feature.
     * @return 0xFFFFFFFFFFFFFFFF.
     */
    virtual uint64 Position();

    /**
     * @brief Unsupported feature.
     * @return false.
     */
    virtual bool SetSize(uint64 size);

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICUDPSOCKET_H_ */

