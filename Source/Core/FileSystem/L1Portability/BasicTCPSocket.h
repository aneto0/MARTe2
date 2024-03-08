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
 * @brief Class which represents a stream network socket, also known as
 * connection-oriented socket, which use Transmission Control Protocol (TCP).
 * @details The main responsibilities of this class are:
 * + Establishing a connection with a remote host.
 * + Binding the socket to a local port and begin listening from it.
 * + Allowing reading/writing using a generic stream interface approach.
 */
class BasicTCPSocket: public BasicSocket {
public:

    /**
     * @brief Default constructor.
     */
    BasicTCPSocket();

    /**
     * @brief Destructor
     * @details Closes the socket.
     */
    virtual ~BasicTCPSocket();

    /**
     * @brief Read without timeout.
     * @param[out] output is the buffer where the read data must be stored.
     * @param[in,out] size is the number of bytes to read.
     * @return false in case of errors.
     * @post
     *   size is the number of read bytes.
     */
    virtual bool Read(char8* const output,
                      uint32 &size);

    /**
     * @brief Write without timeout.
     * @param[in] input is the buffer which contains the data to be written.
     * @param[in,out] size is the number of bytes to write.
     * @return false in case of errors.
     * @post
     *   size is the number of written bytes
     */
    virtual bool Write(const char8* const input,
                       uint32 &size);

    /**
     * @brief Read within timeout.
     * @param[out] output is the buffer where the read data must be stored.
     * @param [in,out] size is the number of bytes to read.
     * @param[in] timeout is the desired timeout in milliseconds.
     * @return false in case of errors or timeout.
     * @post
     *   size is the number of read bytes.
     * @remark If the socket is in non-block mode, the timeout has no meaning.
     */
    virtual bool Read(char8* const output,
                      uint32 &size,
                      const TimeoutType &timeout);

    /**
     * @brief Write within timeout.
     * @param[in] input is the buffer which contains the data to be written.
     * @param[in,out] size is the number of bytes to write.
     * @param[in] timeout is the desired timeout.
     * @return false in case of errors or timeout.
     * @post
     *   size is the number of read bytes.
     * @remark If the socket is in non-block mode, the timeout has no meaning.
     */
    virtual bool Write(const char8* const input,
                       uint32 &size,
                       const TimeoutType &timeout);

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
    virtual bool RelativeSeek(const int64 deltaPos);

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
     * @return false in case of errors.
     * @post
     *   size is the number of read bytes.
     */
    bool Peek(char8 * const buffer,
              uint32 &size) const;

    /**
     * @brief Opens a stream socket
     * @return false if the socket can't be opened.
     */
    bool Open();

    /**
     * @brief Listen for a connection on a specific port. (server function)
     * @param[in] port is the listen port.
     * @param[in] maxConnections is the size of the pending connections queue not accepted yet.
     * @return false in case of errors.
     */
    bool Listen(const uint16 port,
                const int32 maxConnections = 1) const;

    /**
     * @brief Connect to a specific host.
     * @param[in] address is the IP address of the server.
     * @param[in] port is the server port.
     * @param[in] timeout is the desired timeout.
     * @return false in case of errors.
     * @remark If the socket is in non-block mode, the timeout has no meaning.
     */
    bool Connect(const char8 * const address,
                 const uint16 port,
                 const TimeoutType &timeout = TTInfiniteWait);

    /**
     * @brief Returns true if the client is connected to a server, false otherwise
     */
    bool IsConnected() const;

    /**
     * @brief Accepts the next connection in the pending queue returning the relative socket.
     * @param[in] timeout is the desired timeout.
     * @param[in] client is the new BasicTCPSocket which will be created for the new connection.
     * @return NULL in case of failure, the new created socket otherwise.
     * @remark If the socket is in non-block mode, the timeout has no meaning.\n
     * If the input client is NULL, a new BasicTCPSocket will be created on heap, and its deallocation is up to the caller.
     */
    BasicTCPSocket *WaitConnection(const TimeoutType &timeout = TTInfiniteWait,
                                   BasicTCPSocket *client = static_cast<BasicTCPSocket *>(NULL));

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICTCPSOCKET_H_ */

