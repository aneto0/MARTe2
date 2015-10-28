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
 * @brief Implementation of UDP socket.
 */
class BasicUDPSocket: public BasicSocket {
public:

    /**
     * @brief Default constructor.
     * @post
     * BasicSocket()
     */
    BasicUDPSocket();

    /**
     * @brief Copy constructor.
     * @post
     * BasicSocket(socketIn)
     */
    BasicUDPSocket(const SocketCore socketIn = 0);

    /**
     * @brief Destructor.
     */
    virtual ~BasicUDPSocket();

    /**
     * @brief Receive data from socket.
     * @param[out] output is where read data will be stored.
     * @param[in,out] size is the number of bytes to read.
     * @post size is the number of read bytes.
     */
    virtual bool Read(char8* const output,
                      uint32 &size);

    /**
     * @brief Send data to socket.
     * @param[in] input contains the data to send.
     * @param[in,out] size is the number of bytes to write.
     * @post size is the number of written bytes.
     */
    virtual bool Write(const char8* const input,
                       uint32 &size);

    /**
     * @brief Open an UDP socket.
     * @post
     * The socket is initialized.
     */
    bool Open();

    /**
     * @brief Set the port to listen from.
     * @param[in] port is the port number in input.
     * @param[in] maxConnection is the maximum number of pending connection requests in the queue.
     */
    bool Listen(const uint16 port,
                const int32 maxConnections = 1);

    /**
     * @brief Select the destination of next sends.
     * @param[in] address is the destination IP address.
     * @param[in] port is the destination port.
     * @pre
     * address must have the format x.x.x.x
     */
    bool Connect(const char8 * const address,
                 const uint16 port);

    /**
     * @brief Select the destination of next sends.
     * @param[in] dest contains all the destination informations (IP address, port, ...).
     */
    bool Connect(const InternetHost &dest);

    /**
     * @brief If the socket allows write operations.
     * @return true.
     */
    virtual bool CanWrite() const;

    /**
     * @brief If the socket allows read operations.
     * @return true.
     */
    virtual bool CanRead() const;

    /**
     * @brief If the socket allows seek operations.
     * @return false.
     */
    virtual bool CanSeek() const;

    /**
     * @brief Receive data from socket within timeout.
     * @param[out] output is where read data will be stored.
     * @param[in,out] size is the number of bytes to read.
     * @param[in] msecTimeout is the desired timeout.
     * @return false if the timeout expires before reading.
     * @post size is the number of read bytes.
     */
    virtual bool Read(char8 * const output,
                      uint32 & size,
                      const TimeoutType &msecTimeout);

    /**
     * @brief Send data to socket within timeout.
     * @param[in] input contains the data to send.
     * @param[in,out] size is the number of bytes to write.
     * @param[in] msecTimeout is the desired timeout.
     * @return false if the timeout expires before writing.
     * @post size is the number of written bytes.
     */
    virtual bool Write(const char8 * const input,
                       uint32 & size,
                       const TimeoutType &msecTimeout);

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
    virtual bool RelativeSeek(const int32 deltaPos);

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

