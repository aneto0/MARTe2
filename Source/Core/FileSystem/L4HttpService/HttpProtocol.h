/**
 * @file HttpProtocol.h
 * @brief Header file for class HttpProtocol
 * @date 14/09/2018
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

 * @details This header file contains the declaration of the class HttpProtocol
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPPROTOCOL_H_
#define HTTPPROTOCOL_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "DoubleBufferedStream.h"
#include "HttpDefinition.h"
#include "HttpRealmI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Implementation of the HTTP protocol.
 * @details This implementation allows to send and receive HTTP messages.
 */
class HttpProtocol : public ConfigurationDatabase {
public:

    /**
     * @brief Constructor.
     * @details Sets the stream used to exchange HTTP messages.
     * @param[in] outputStreamIn the stream
     */
    HttpProtocol(DoubleBufferedStream &outputStreamIn);

    /**
     * @brief Destructor.
     */
    virtual ~HttpProtocol();

    /**
     * @brief Reads the HTTP header.
     * @details Parses the HTTP header recognising the HTTP, GET, PUT, POST, HEAD commands.
     * In case of POST command:\n
     *   If the content type is multipart/form-data the data between boundaries is saved into
     *   variables in InputOptions block. This variables can be accessed also using the
     *   GetInputCommand method.\n
     *   If the content type is application/x-www-form-urlencoded the url in the body is decoded
     *   and also in this case the variables are saved in the InputOptions block.
     * @param[in] bufferReadSize the size of the buffer to be used in the read operations.
     * @details Use the CompleteReadOperation method to get the unread data of the HTTP message after a
     * ReadHeader call.
     * @return true if the read operation succeeds, false otherwise.
     */
    bool ReadHeader(uint32 bufferReadSize = 1024u);


    /**
     * @brief Writes the HTTP header.
     * @param[in] isMessageCompleted specifies if the message is completed. In this case the Content-Length
     * is computed as the size of the \a payload stream passed in input and the payload is appended to the
     * header and sent as the HTTP body.
     * @param[in] command the HTTP command and can be one of HTTP, GET, PUT, POST, HEAD.
     * @param[in] payload contains the body to be appended to the header. If NULL or empty, only the header will be sent.
     * @param[in] id the url to write in the header if command is not a reply (!=HTTP).
     * @param[in] bufferWriteSize the size of the buffer to be used in the write operations.
     * @return true if the write operation succeeds, false otherwise.
     */
    bool WriteHeader(const bool isMessageCompleted,
                             const int32 command,
                             BufferedStreamI * const payload,
                             const char8 * const id,
                             uint32 bufferWriteSize = 1024u);

    /**
     * @brief Reads the unread data after a ReadHeader call.
     * @param[out] streamout the stream where the unread data will be written.
     * @param[in] msecTimeout the maximum time allowed for this operation.
     * @param[in] bufferReadSize the number of bytes to read on each operation.
     * @return true if the read operation succeeds, false otherwise.
     */
    bool CompleteReadOperation(BufferedStreamI * const streamout,
                               TimeoutType msecTimeout = TTInfiniteWait,
                               uint32 bufferReadSize = 1024u);

    /**
     * @brief Performs a security check using the realm passed in input.
     * @param[in] realm the realm that implements the security check.
     * @return true if the security check succeeds, false otherwise.
     */
    bool SecurityCheck(ReferenceT<HttpRealmI> realm);

    /**
     * @brief Asserts if the connection flag is keep-alive or close.
     * @return true if the connection is keep-alive, false if it is close.
     */
    bool KeepAlive() const;

    /**
     * @brief Sets the connection mode.
     * @param[in] isKeepAlive to set the connection mode.
     */
    void SetKeepAlive(const bool isKeepAlive);

    /**
     * @brief Retrieves the HTTP command code of the last ReadHeade call.
     * @return the HTTP command code of the last ReadHeade call.
     */
    int32 GetHttpCommand() const;

    /**
     * @brief Sets the unmatched url.
     * @param[in] unMatchedIdIn the unmatched url.
     */
    void SetUnmatchedId(const char8 * const unMatchedIdIn);

    /**
     * @brief Returns the unmatched url.
     * @param[out] unmatchedIdOut contains the unmatched url in output.
     */
    void GetUnmatchedId(StreamString& unmatchedIdOut) const;

    /**
     * @brief Retrieves the path that has been built from the received url.
     * @param[out] pathOut contains the path in output.
     */
    void GetPath(StreamString& pathOut) const;

    /**
     * @brief Retrieves the received url.
     * @param[out] idOut contains the url in output.
     */
    void GetId(StreamString& idOut) const;

    /**
     * @brief Read the input command specified in \a commandName.
     * @details All the input variables are saved in the InputCommands block, so can be also directly using
     * the StructuredDataI::Read methods.
     * @param[in] commandName the command name.
     * @param[out] commandValue the command value in output.
     * @return true if the operation succeeds, false otherwise.
     */
    bool GetInputCommand(const char8 * const commandName, const AnyType &commandValue);

    /**
     * @brief Retrieves the text mode status.
     * @details. If the client specifies an input command TextMode=[0(false),1(true)] the protocol
     * will assume that the clients is expecting data in text mode (if 1) or in database mode (if 0)
     * and the service (in this case HttpService) will call DataExportI::GetAsText if TextMode==1 and
     * DataExportI::GetAsStructuredData if TextMode==0. This flag is initialised at -1, this means that
     * the client does not assert anything about how it wants to receive the data and the service will
     * proceed in its default mode.
     * @return -1 if the client has not specified a TextMode command, 1 if the client has specified
     * TextMode=1 command, 0 if the client has specified TextMode==0 command.
     */
    int8 TextMode() const;

protected:

    /**
     * The stream where to read from and write to.
     */
    DoubleBufferedStream *outputStream;

    /**
     * How much data is still waiting in the
     * input stream from the client
     */
    int32 unreadInput;

    /**
     * The Http return code
     */
    int32 httpErrorCode;

    /**
     * The Http command
     */
    int32 httpCommand;

    /**
     * The Http version
     * 1000 means v1.0 2100 means v2.1
     */
    uint32 httpVersion;

    /**
     * The keep-alive connection flag
     * True if communication should continue after transaction
     * False otherwise
     */
    bool keepAlive;

    /**
     * The last time the body has been updated
     */
    uint64 lastUpdateTime;

    /**
     * The requested page URL
     */
    StreamString url;

    /**
     * The URL with . instead of \/
     */
    StreamString path;

    /**
     * The remainder of url not matched in the search
     */
    StreamString unMatchedUrl;

    /**
     * The text mode flag
     */
    int8 textMode;

    /**
     * True if the request is chunked
     */
    bool isChunked;

private:

    /**
     * @brief Called by ReadHeader. Parses the HTTP header and recognises the HTTP command.
     */
    bool RetrieveHttpCommand(StreamString &command,
                             StreamString &line);

    /**
     * @brief Called by ReadHeader. Builds the received
     */
    char8 BuildUrl(StreamString &line);

    /**
     * @brief Called by ReadHeader. Stores the received commands in the InputCommands block.
     */
    bool StoreCommands(StreamString &line);

    /**
     * @brief Called by ReadHeader. Stores the received output options in the OutputOptions block.
     */
    bool StoreOutputOptions();

    /**
     * @brief Called by ReadHeader. Stores the received input options in the InputOptions block.
     */
    bool StoreInputOptions();

    /**
     * @brief Called by ReadHeader. Recognises the content-type and calls the relative
     * method to handle a POST HTTP request.
     */
    bool HandlePost(StreamString &contentType,
                    StreamString &content);

    /**
     * @brief Called by HandlePost. Handles a POST with Content-Type=multipart/form-data
     */
    bool HandlePostMultipartFormData(StreamString &contentType,
                                     StreamString &content);

    /**
     * @brief Called by HandlePost. Handles a POST with Content-Type=application/x-www-form-urlencoded
     * @details The parsed variables will be saved in the InputOptions block.
     */
    bool HandlePostApplicationForm(StreamString &content);

    /**
     * @brief Called by HandlePostMultipartFormData. Handles the POST header.
     * @details The parsed variables will be saved in the InputOptions block.
     */
    bool HandlePostHeader(StreamString &line,
                          StreamString &content,
                          StreamString &name,
                          StreamString &filename);

    /**
     * @brief Called by HandlePostMultipartFormData. Handles the POST content.
     * @details The parsed variables will be saved in the InputOptions block.
     */
    bool HandlePostContent(StreamString &line,
                           StreamString &boundary,
                           StreamString &name,
                           StreamString &filename,
                           StreamString &value,
                           bool &headerHandled);

/*lint -e {1712} default constructor not set as this class needs a stream to be used*/
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPPROTOCOL_H_ */

