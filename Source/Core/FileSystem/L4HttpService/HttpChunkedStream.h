/**
 * @file HttpChunkedStream.h
 * @brief Header file for class HttpChunkedStream
 * @date 28/09/2018
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

 * @details This header file contains the declaration of the class HttpChunkedStream
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPCHUNKEDSTREAM_H_
#define HTTPCHUNKEDSTREAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TCPSocket.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief Implementation of the chunked body send for the HTTP protocol.
 * @see HttpService.
 * @details The chunked transfer encoding requires that the body is divided in chunks
 * with a size that must be sent before the chunk itself. For instance, if the HTTP body
 * to be sent is HelloWorld and the maximum chunk size is 4, the following must be sent:\n
 *   4\n
 *   Hell\n
 *   4\n
 *   oWor\n
 *   2\n
 *   ld\n
 *   0\n
 *   \n
 */
class HttpChunkedStream: public TCPSocket{

public:
    /**
     * @brief Default constructor.
     * @details Sets calibReadParam and calibWriteParam to zero to force
     * the buffer mode.
     */
    HttpChunkedStream();

    /**
     * @brief Destructor.
     */
    virtual ~HttpChunkedStream();

    /**
     * @brief Implementation of the Flush on the low-level stream (the TCP socket)
     * to send the chunk size before the chunk data implementing the chunked transfer
     * encoding protocol.
     * @see TCPSocket::Flush
     */
    virtual bool Flush();

    /**
     * @brief Sends a zero-size final chunk to tell the host that the HTTP
     * message is terminated.
     */
    bool FinalChunk();

    /**
     * @brief Sets the chunk mode.
     * @details If this stream is in chunk mode, the Flush implements the chunk transfer
     * encoding protocol, otherwise it will flush the data on the socket as it is a normal
     * TCPSocket object
     * @param[in] chunkModeIn the chunk mode.
     */
    void SetChunkMode(const bool chunkModeIn);

    /**
     * @brief Returns true if this stream is in chunk mode, false otherwise.
     * @return true if this stream is in chunk mode, false otherwise.
     */
    bool IsChunkMode() const;

private:

    /**
     * Chunk mode flag
     */
    bool chunkMode;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPCHUNKEDSTREAM_H_ */

