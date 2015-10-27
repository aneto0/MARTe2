/**
 * @file BasicTCPSocket.h
 * @brief Header file for class BasicTCPSocket
 * @date 23/10/2015
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

 * @details This header file contains the declaration of the class BasicTCPSocket
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICTCPSOCKET_H_
#define BASICTCPSOCKET_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TimeoutType.h"
#include "BasicSocket.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Implementation of TCP socket.
 */
class BasicTCPSocket: public BasicSocket {
public:

    /**
     * @brief Default constructor.
     */
    BasicTCPSocket();

    /**
     * @brief Copy constructor.
     * @param[in] socketIn is the socket to set.
     */
    BasicTCPSocket(const SocketCore socketIn);

    /**
     * @brief Destructor
     * @details Closes the socket.
     */
    virtual ~BasicTCPSocket();

    /**
     * @brief Read without timeout.
     * @param[out] output is the buffer where the read data must be stored.
     * @param [in,out] size is the number of bytes to read.
     * @post size is the number of read bytes.
     */
    virtual bool Read(char8* const output,
                      uint32 &size);

    /**
     * @brief Write without timeout.
     * @param[in] input is the buffer which contains the data to be written.
     * @param[in,out] size is the number of bytes to write.
     * @post size is the number of written bytes
     */
    virtual bool Write(const char8* const input,
                       uint32 &size);

    /**
     * @brief Read with timeout.
     * @param[out] output is the buffer where the read data must be stored.
     * @param [in,out] size is the number of bytes to read.
     * @param[in] msecTimeout is the desired timeout.
     * @post size is the number of read bytes.
     */
    virtual bool Read(char8* const output,
                      uint32 &size,
                      const TimeoutType &msecTimeout);

    /**
     * @brief Write with timeout.
     * @param[in] input is the buffer which contains the data to be written.
     * @param[in,out] size is the number of bytes to write.
     * @param[in] msecTimeout is the desired timeout.
     * @post size is the number of read bytes.
     */
    virtual bool Write(const char8* const input,
                       uint32 &size,
                       const TimeoutType &msecTimeout);

    /**
     * @brief Unsupported feature.
     * @return false.
     */
    virtual bool Seek(uint64 pos);

    /**
     * @brief Unsupported feature.
     * @return max uint64 0xFFFFFFFFFFFFFFFF.
     */
    virtual uint64 Size();

    /**
     * @brief Unsupported feature.
     * @return false.
     */
    virtual bool RelativeSeek(const int32 deltaPos);

    /**
     * @brief Unsupported feature.
     * @return max uint64 0xFFFFFFFFFFFFFFFF.
     */
    virtual uint64 Position();

    /**
     * @brief Unsupported feature.
     * @return false.
     */
    virtual bool SetSize(uint64 size);

    /**
     * @see StreamI::CanWrite()
     * @return true.
     */
    virtual bool CanWrite() const;

    /**
     * @see StreamI::CanRead()
     * @return true.
     */
    virtual bool CanRead() const;

    /**
     * @see StreamI::CanSeek()
     * @return false.
     */
    virtual bool CanSeek() const;

    /**
     * @brief Read without removing data from the socket pipe.
     * @param[out] buffer is the buffer used to store the read data.
     * @param[in,out] size is the number of bytes to read.
     * @post size is the number of read bytes.
     */
    bool Peek(char8 * const buffer,
              uint32 &size) const;

    /**
     * @brief Opens a stream socket
     */
    bool Open();

    /**
     * @brief Listen for a connection on a specific port. (server function)
     * @param[in] port is the listen port.
     * @param[in] maxConnections is the size of the pending connections queue not accepted yet.
     */
    bool Listen(const uint16 port,
                const int32 maxConnections = 1) const;

    /**
     * @brief Listen for a connection on the port specified by the internet service in input. (server function)
     * @param[in] serviceName is the internet service in input.
     * @param[in] maxConnections is the size of the pending connections queue not accepted yet.
     */
    bool Listen(const char8 * const serviceName,
                const int32 maxConnections = 1) const;

    /**
     * @brief Connect to a specific host.
     * @param[in] address is the IP address of the server.
     * @param[in] port is the server port.
     * @param[in] msecTimeout is the desired timeout.
     * @param[in] retry is the maximum number of times to call the connect function in case of interruptions.
     */
    bool Connect(const char8 * const address,
                 const uint16 port,
                 const TimeoutType &msecTimeout = TTInfiniteWait,
                 int32 retry = 12);

    /**
     * @brief Connect to a specific host.
     * @param[in] address is the IP address of the server.
     * @param[in] port is the server service name.
     * @param[in] msecTimeout is the desired timeout.
     */
    bool Connect(const char8 * const address,
                 const char8 * const serviceName,
                 const TimeoutType &msecTimeout = TTInfiniteWait);

    /**
     * @brief Returns true if the connection is alive, false otherwise
     */
    bool IsConnected() const;

    /**
     * @brief Accepts the next connection in the pending queue returning the relative socket.
     * @return NULL in case of failure, the new created socket otherwise.
     */
    BasicTCPSocket *WaitConnection(const TimeoutType &msecTimeout = TTInfiniteWait,
                                   BasicTCPSocket *client = static_cast<BasicTCPSocket *>(NULL));

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICTCPSOCKET_H_ */

