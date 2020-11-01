/**
 * @file HttpProtocol.cpp
 * @brief Source file for class HttpProtocol
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

 * @details This source file contains the definition of all the methods for
 * the class HttpProtocol (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "BasicSocket.h"
#include "DoubleBufferedStream.h"
#include "HttpDefinition.h"
#include "HttpProtocol.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

HttpProtocol::HttpProtocol(DoubleBufferedStream &outputStreamIn) :
        ConfigurationDatabase() {
    httpCommand = HttpDefinition::HSHCNone;
    httpVersion = 1100u;
    httpErrorCode = 200;
    keepAlive = true;
    outputStream = &outputStreamIn;
    lastUpdateTime = 0ull;
    /** unknown information length */
    unreadInput = -1;
    textMode = -1;
    isChunked = false;
}

/*lint -e{1551} the HttpProtocol must be purged.*/
HttpProtocol::~HttpProtocol() {
    outputStream = NULL_PTR(DoubleBufferedStream*);
    Purge();
}

bool HttpProtocol::CompleteReadOperation(BufferedStreamI * const streamout, TimeoutType msecTimeout, const uint32 bufferReadSize) {

    bool ret = true;
    //This way we can change the falsely undefined content-length when this is called from Write Header
    //complete the read only when the body is completed from the other part
    bool streamNull = (streamout == NULL);
    bool unreadInputBool = (unreadInput < 0);
    bool noMsecTimeout = (msecTimeout==0u);
    if ((streamNull) && (unreadInputBool||noMsecTimeout)) {
        unreadInput = -1;
    }
    else {
        uint64 startCounter = HighResolutionTimer::Counter();

        // convert the stop time
        uint64 maxTicks = startCounter + ((static_cast<uint64>(msecTimeout.GetTimeoutMSec()) * HighResolutionTimer::Frequency()) / 1000ULL);

        uint32 bufferSize = bufferReadSize;
        char8 *buffer = new char8[bufferSize];
        uint32 readSize = 1u;
        uint32 sizeToRead = bufferSize;

        if (unreadInput >= 0) {
            sizeToRead = static_cast<uint32>(unreadInput);
            //clip the size
            if (sizeToRead > bufferSize) {
                sizeToRead = bufferSize;
            }
        }

        while ((sizeToRead > 0u) && (readSize > 0u) && (ret)) {

            ret = outputStream->Read(buffer, sizeToRead, msecTimeout);

            if ((ret) || (sizeToRead > 0u)) {
                readSize = sizeToRead;
                uint32 sizeToWrite = readSize;
                if (streamout != NULL_PTR(BufferedStreamI *)) {
                    //complete write?
                    (void) streamout->Write(buffer, sizeToWrite, msecTimeout);
                }
                if (isChunked) {
                    //Check if it finishes with 0\r\n\r\n
                    if (sizeToWrite > 4u) {
                        bool done = (StringHelper::CompareN(&buffer[sizeToWrite - 5u], "0\r\n\r\n", 5u) == 0);
                        if (done) {
                            //break the while
                            readSize = 0u;
                        }
                    }
                }

                sizeToRead = bufferSize;

                if (unreadInput > 0) {
                    unreadInput -= static_cast<int32>(readSize);
                    sizeToRead = static_cast<uint32>(unreadInput);
                    //clip the size
                    if (sizeToRead > bufferSize) {
                        sizeToRead = bufferSize;
                    }
                }
                if (msecTimeout.IsFinite()) {
                    uint64 lastCounter = HighResolutionTimer::Counter();
                    uint64 ticksLeft = (maxTicks - lastCounter);
                    msecTimeout.SetTimeoutHighResolutionTimerTicks(ticksLeft);
                }
            }
            else {
                //REPORT_ERROR_STATIC(ErrorManagement::Timeout, "Possible Timeout on completion");
            }
        }

        delete[] buffer;
    }
    return (unreadInput <= 0);

}

bool HttpProtocol::ReadHeader(const uint32 bufferReadSize) {
    /** unknown information length */
    unreadInput = -1;
    lastUpdateTime = HighResolutionTimer::Counter();

    StreamString line;
    char8 terminator;
    // Reads the HTTP command
    bool ret = outputStream->GetLine(line, false);
    if (!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Failed reading a line from socket");
    }
    if (ret) {
        ret = line.Seek(0ull);
    }
    if (ret) {
        StreamString command;
        //check the arrived command
        (void) line.GetToken(command, " \r\n\t", terminator, " \r\n\t");
        ret = RetrieveHttpCommand(command, line);
    }

    // if httpCommand is a HttpDefinition::HSHCReply then the version has already been calculated
    if (HttpDefinition::HSHCReply > httpCommand) {

        if (ret) {
            // extract the uri and build a path based n that
            char8 termChar = BuildUrl(line);
            if (termChar == '?') {
                // extracts commands
                ret = StoreCommands(line);
                if (ret) {
                    int8 textModeT = textMode;
                    if (!Read("TextMode", textMode)) {
                        textMode = textModeT;
                    }
                }
            }
        }

        //store the HTTP version
        if (ret) {
            StreamString version;
            ret = line.GetToken(version, " \r\n\t", terminator, " \r\n\t");
            if (ret) {
                float32 fVersion = 0.F;
                //skip HTTP
                const char8 *fVersionStr = &(version.Buffer()[5u]);
                if (!TypeConvert(fVersion, fVersionStr)) {
                    fVersion = 0.0F;
                }
                if (fVersion <= 0.0F) {
                    httpVersion = 1100u;
                }
                else {
                    /*lint -e{9122} allowed cast from float to integer*/
                    httpVersion = static_cast<uint32>(fVersion * 1000.0F);
                }
            }
        }
    }

    if (ret) {
        ret = StoreInputOptions();
    }

    StreamString contentType;
    if (ret) {
        // now evaluate options
        // first check what policy to follow: if dataSize is available and connection keep-alive,
        // then do not shut the connection and load only the specified size
        keepAlive = (httpVersion >= 1100u);
        StreamString connection;
        if (Read("Connection", connection)) {
            if (StringHelper::CompareNoCaseSensN(connection.Buffer(), "keep-alive", 10u) == 0) {
                keepAlive = true;
            }
            else if (StringHelper::CompareNoCaseSensN(connection.Buffer(), "close", 5u) == 0) {
                keepAlive = false;
            }
            else {

            }
        }
        if (!Read("Content-Length", unreadInput)) {
            unreadInput = HttpDefinition::HTTPNoContentLengthSpecified;
        }

        if (!Read("Content-Type", contentType)) {
            contentType = "";
        }

        isChunked = false;
        StreamString encoding;
        if (Read("Transfer-Encoding", encoding)) {
            isChunked = (encoding == "chunked");
        }

    }

    //write the peer in the configuration
    if (ret) {
        //HTTP 1.1 might require to reply to 100-continue so that the client will continue to send more information
        StreamString expectStr;
        if (!Read("Expect", expectStr)) {
            expectStr = "";
        }
        if (expectStr == "100-continue") {
            (void) outputStream->Printf("%s", "HTTP/1.1 100 Continue\r\n");
            (void) outputStream->Flush();
        }
        ret = MoveToRoot();
        if (ret) {
            StreamString peer = "";
            /*lint -e{740} outputStream may be a BasicSocket*/
            BasicSocket *clientSocket = dynamic_cast<BasicSocket *>(outputStream);
            if (clientSocket != NULL) {
                // this could be very slow I think ip number will suffice
                // clientSocket->Source().HostName(peer);
                peer = (clientSocket->GetSource()).GetAddress();
                if (peer == "127.0.0.1") {
                    peer = InternetHost::GetLocalAddress();
                }
                ret = Write("Peer", peer.Buffer());
            }
        }
    }

    if (ret) {
        //If it post read the body
        if (httpCommand == HttpDefinition::HSHCPost) {
            ret = (contentType.Size() > 0u);
            if (!ret) {
                REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "If using POST the Content-Type MUST be specified");
            }

            if (ret) {
                ret = (unreadInput > 0);
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "If using POST the Content-Length MUST be specified");
                }
            }

            if (ret) {
                //???absolute or relative??
                if (!MoveRelative("InputCommands")) {
                    (void) Delete("InputCommands");
                    ret = CreateRelative("InputCommands");
                }

                if (ret) {
                    StreamString postContent;
                    char8 *buffer = new char8[bufferReadSize];
                    while ((unreadInput > 0) && (ret)) {
                        uint32 readSize = bufferReadSize;
                        ret = outputStream->Read(&buffer[0], readSize);
                        if (!ret) {
                            REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Failed reading from socket in POST section");
                        }

                        if (ret) {
                            if (bufferReadSize > 0u) {
                                ret = (postContent.Write(&buffer[0], readSize));
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Failed updating content buffer in POST section");
                                }
                            }
                            unreadInput -= static_cast<int32>(readSize);
                        }
                    }
                    if (ret) {
                        ret = (HandlePost(contentType, postContent));
                    }
                    if (ret) {
                        ret = MoveToRoot();
                    }
                    delete[] buffer;
                }
            }
        }
    }

    httpErrorCode = 200;
    return ret;
}

bool HttpProtocol::WriteHeader(const bool isMessageCompleted, const int32 command, BufferedStreamI * const payload, const char8 * const id,
                               const uint32 bufferWriteSize) {

    //if sending something with bodyCompleted=false
    //remember to write Transfer-Encoding: chunked in options

    // complete transaction with remote host
    if (!CompleteReadOperation(NULL_PTR(BufferedStreamI*))) {
        REPORT_ERROR(ErrorManagement::Warning, "Failed CompleteReadOperation");
    }
    // if it is a reply get errorCode
    // otherwise mark the httpCommand as none
    bool isReply = HttpDefinition::IsReplyCode(command, httpErrorCode);
    if (!isReply) {
        httpCommand = HttpDefinition::HSHCNone;
    }

    // saves all the pending changes
    bool ret = true;

    // assemble the header
    // deal with a reply
    uint32 majorVersion = (httpVersion / 1000u);
    uint32 minorVersion = ((httpVersion % 1000u) / 100u);

    const char8* urlToUse = id;
    if (urlToUse == NULL_PTR(const char8 *)) {
        urlToUse = "/";
    }

    if (isReply) {
        ret = outputStream->Printf("HTTP/%i.%i %i %s\r\n", majorVersion, minorVersion, httpErrorCode, HttpDefinition::GetErrorCodeString(httpErrorCode));
    }
    else if (command == HttpDefinition::HSHCGet) {
        ret = outputStream->Printf("GET %s HTTP/%i.%i\r\n", urlToUse, majorVersion, minorVersion);
    }
    else if (command == HttpDefinition::HSHCPut) {
        ret = outputStream->Printf("PUT %s HTTP/%i.%i\r\n", urlToUse, majorVersion, minorVersion);
    }
    else if (command == HttpDefinition::HSHCPost) {
        ret = outputStream->Printf("POST %s HTTP/%i.%i\r\n", urlToUse, majorVersion, minorVersion);
    }
    else if (command == HttpDefinition::HSHCHead) {
        ret = outputStream->Printf("HEAD %s HTTP/%i.%i\r\n", urlToUse, majorVersion, minorVersion);
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Command code %i unknown", command);
        ret = false;
    }

    if (ret) {
        if (!MoveAbsolute("OutputOptions")) {
            ret = CreateAbsolute("OutputOptions");
        }
    }
    if (ret) {
        if (isMessageCompleted) {
            uint32 payloadSize = 0u;
            if (payload != NULL_PTR(BufferedStreamI*)) {
                payloadSize = static_cast<uint32>(payload->Size());
            }
            ret = Write("Content-Length", payloadSize);
        }
    }
    if (ret) {
        StreamString connection;
        if (Read("Connection", connection)) {
            if (StringHelper::CompareNoCaseSensN(connection.Buffer(), "keep-alive", 10u) == 0) {
                keepAlive = true;
            }
            else if (StringHelper::CompareNoCaseSensN(connection.Buffer(), "close", 5u) == 0) {
                keepAlive = false;
            }
            else {

            }
        }
        if (keepAlive) {
            ret = Write("Connection", "keep-alive");
        }
        else {
            ret = Write("Connection", "close");
        }
    }
    // write all options
    uint32 numberOfChildren = GetNumberOfChildren();
    for (uint32 i = 0u; (i < numberOfChildren) && (ret); i++) {
        StreamString value;
        StreamString key = GetChildName(i);
        if (Read(key.Buffer(), value)) {
            ret = outputStream->Printf("%s: %s\r\n", key.Buffer(), value.Buffer());
            if (!ret) {
                REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "write key %s on socket failed\n", key.Buffer());
            }
        }
    }
    if (ret) {
        ret = outputStream->Printf("%s", "\r\n");
    }
    if (ret) {
        // return to root
        ret = MoveToRoot();
    }
    if (ret) {
        // send out the body
        if (payload != NULL_PTR(BufferedStreamI*)) {
            uint32 toWrite = static_cast<uint32>(payload->Size());
            char8 *buff = new char8[bufferWriteSize];
            while (toWrite > 0u) {
                uint32 wSize = toWrite;
                if (wSize > bufferWriteSize) {
                    wSize = bufferWriteSize;
                }

                ret = payload->Read(&buff[0], wSize);
                if (ret) {
                    ret = outputStream->Write(&buff[0], wSize);
                }
                toWrite -= wSize;
            }
            delete[] buff;
        }
    }
    if (ret) {
        //flush the stream
        ret = outputStream->Flush();
    }
    return ret;

}

bool HttpProtocol::RetrieveHttpCommand(StreamString &command, StreamString &line) {
    char8 terminator;

    bool ret = true;
    if (command == "GET") {
        httpCommand = HttpDefinition::HSHCGet;
    }
    else if (command == "PUT") {
        httpCommand = HttpDefinition::HSHCPut;
    }
    else if (command == "POST") {
        httpCommand = HttpDefinition::HSHCPost;
    }
    else if (command == "HEAD") {
        httpCommand = HttpDefinition::HSHCHead;
    }
    else if (StringHelper::CompareN(command.Buffer(), "HTTP", 4u) == 0) {
        // in a reply there is no command
        // it starts with HTTP ...
        float32 fVersion = 0.0F;
        const char8* versionStr = &(command.Buffer()[5u]);
        if (!TypeConvert(fVersion, versionStr)) {
            fVersion = 0.0F;
        }

        //convert the version
        if (fVersion <= 0.0F) {
            httpVersion = 1100u;
        }
        else {
            /*lint -e{9122} allowed cast from float to integer*/
            httpVersion = static_cast<uint32>(fVersion * 1000.0F);
        }

        //convert to the error code
        StreamString errorCode;
        (void) line.GetToken(errorCode, " \r\n\t", terminator, " \r\n\t");
        int32 errorCodeInt;
        ret = TypeConvert(errorCodeInt, errorCode.Buffer());
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Failed converting the error code of the reply %s to an integer", errorCode.Buffer());
        }
        else {
            httpCommand = HttpDefinition::GenerateReplyCode(errorCodeInt);
        }

    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Cannot deal with a %s request (GET/PUT/POST/HEAD only)", command.Buffer());
        ret = false;
    }
    return ret;
}

char8 HttpProtocol::BuildUrl(StreamString &line) {

    StreamString tempUrl;
    char8 terminator = '\n';
//get the url
    (void) line.GetToken(tempUrl, " \r\n?", terminator, " \r\n?");
    bool ret = tempUrl.Seek(0ULL);
    if (ret) {
        StreamString decoded;
        //decode the url
        (void) HttpDefinition::HttpDecode(decoded, tempUrl);
        ret = decoded.Seek(0ULL);
        if (ret) {
            bool ok = true;
            StreamString urlPart;
            char8 saveTerm;
            //build url and path
            ret = url.SetSize(0ULL);
            if (ret) {
                ret = path.SetSize(0ULL);
            }
            if (ret) {
                while (ok) {
                    urlPart = "";
                    ok = decoded.GetToken(urlPart, "\\/", saveTerm);
                    if (ok) {
                        //url += urlPart.Buffer();
                        path += urlPart.Buffer();
                        if (saveTerm != '\0') {
                            if (urlPart.Size() > 0LLU) {
                                //url += saveTerm;
                                path += '.';
                            }
                        }
                    }
                }
            }
        }
        url = path;
        unMatchedUrl = path;
    }
    return terminator;
}

bool HttpProtocol::StoreCommands(StreamString &line) {
    StreamString commands;
    char8 terminator;
    (void) line.GetToken(commands, " \t", terminator);

    bool ret = MoveToRoot();
    if (ret) {
        //delete existing
        (void) Delete("InputCommands");
        ret = CreateRelative("InputCommands");

    }

    if (ret) {
        ret = commands.Seek(0ull);
        if (ret) {
            StreamString command;
            while ((commands.GetToken(command, "&", terminator)) && (ret)) {
                ret = command.Seek(0ull);
                if (ret) {
                    if (command.Size() > 3u) {
                        StreamString variable;
                        StreamString value;
                        ret = command.GetToken(variable, "=", terminator);
                        if (ret) {
                            //get until the end
                            (void) command.GetToken(value, "", terminator);
                            StreamString decodedValue;
                            StreamString decodedVariable;
                            ret = value.Seek(0ull);
                            if (ret) {
                                ret = variable.Seek(0ull);
                            }
                            if (ret) {
                                ret = HttpDefinition::HttpDecode(decodedValue, value);
                            }
                            if (ret) {
                                ret = HttpDefinition::HttpDecode(decodedVariable, variable);
                            }
                            if (ret) {
                                ret = Write(decodedVariable.Buffer(), decodedValue.Buffer());
                            }
                        }
                    }
                }
                if (ret) {
                    ret = command.SetSize(0ULL);
                }
            }
        }
    }
    return ret;
}

bool HttpProtocol::StoreInputOptions() {

    bool ret = MoveToRoot();
    char8 terminator;

    if (ret) {
//delete existing
        (void) Delete("InputOptions");
        ret = CreateRelative("InputOptions");

    }
    if (ret) {
        bool ok = true;
        const uint32 MAX_RETRIES = 5u;
        uint32 nOfRetries = MAX_RETRIES;
        while (ok) {
            StreamString line;
            ret = outputStream->GetLine(line);
            if (!ret) {
                nOfRetries--;
                REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Failed reading a line from socket");
            }
            if (nOfRetries == 0u) {
                ok = false;
            }

            if (ret) {
                nOfRetries = MAX_RETRIES;
                ok = (line.Size() > 0ull);
                // parse HTTP Options and add to CDB
                if (ok) {
                    ret = line.Seek(0ull);
                    StreamString key;
                    StreamString value;

                    if (ret) {
                        ret = line.GetToken(key, " \t:", terminator);
                    }
                    if (ret) {
                        (void) line.GetToken(value, " \t", terminator);
                        // any other part separated by spaces add to the token
                        // use a space as separator
                        uint64 linePosition = line.Position();
                        if (line.Size() > linePosition) {
                            uint32 sizeW = 1u;
                            char8 space = ' ';
                            ret = value.Write(&space, sizeW);
                        }
                        if (ret) {
                            (void) line.GetToken(value, "", terminator);
                            ret = Write(key.Buffer(), value.Buffer());
                        }
                    }
                }
            }
        }
    }
    return ret;
}

bool HttpProtocol::HandlePostHeader(StreamString &line, StreamString &content, StreamString &name, StreamString &filename) {
    const char8 *temp = NULL_PTR(const char8 *);

    bool ret = true;
//read the name (it is in the form name="_NAME_")
    const char8* nameTemp = StringHelper::SearchString(line.Buffer(), "name=\"");
    if (nameTemp != NULL_PTR(const char8*)) {
        name = &nameTemp[StringHelper::Length("name=\"")];
        temp = name.Buffer();
        uint32 count = 0u;
        while (temp[count] != '\"') {
            count++;
        }
        ret = name.SetSize(static_cast<uint64>(count));
    }
    else {
        nameTemp = StringHelper::SearchString(line.Buffer(), "name=");

        if (nameTemp != NULL) {
            name = &nameTemp[StringHelper::Length("name=")];
        }
    }
    if (ret) {
        //Check if the content is a file
        const char8* filenameTemp = StringHelper::SearchString(line.Buffer(), "filename=\"");
        if (filenameTemp != NULL) {
            filename = &filenameTemp[StringHelper::Length("filename=\"")];
            temp = filename.Buffer();
            uint32 count = 0u;
            while (temp[count] != '\"') {
                count++;
            }
            ret = filename.SetSize(static_cast<uint64>(count));
        }
        if (ret) {
            if (filename.Size() > 0ull) {
                StreamString key = name;
                key += ":filename";

                //Write the filename
                ret = Write(key.Buffer(), filename.Buffer());
                if (ret) {
                    //Check the file mime type
                    ret = line.SetSize(0ull);
                }
                if (ret) {
                    if (content.GetLine(line)) {
                        const char8* fcTypeTemp = StringHelper::SearchString(line.Buffer(), "Content-Type: ");
                        if (fcTypeTemp != NULL_PTR(const char8*)) {
                            StreamString fcType = &fcTypeTemp[StringHelper::Length("Content-Type: ")];
                            key = name;
                            key += ":Content-Type";
                            ret = Write(key.Buffer(), fcType.Buffer());
                        }
                    }
                }
            }
        }
    }
    return ret;
}

bool HttpProtocol::HandlePostContent(StreamString &line, StreamString &boundary, StreamString &name, StreamString &filename, StreamString &value,
                                     bool &headerHandled) {

    bool ret = true;
//search for the boundary end
    if (StringHelper::SearchString(line.Buffer(), boundary.Buffer()) != NULL_PTR(const char8*)) {

        //reset...new blank line is new handler
        headerHandled = false;

        ret = Write(name.Buffer(), value.Buffer());
        if (ret) {
            ret = value.SetSize(0ull);
            if (ret) {
                ret = line.SetSize(0ull);
            }
            if (ret) {
                ret = filename.SetSize(0ull);
            }
            if (ret) {
                ret = name.SetSize(0ull);
            }
        }
    }
//if not boundary store in value
    else {
        if (value.Size() > 0u) {
            value += "\n";
        }
        value += line;
    }

    return ret;
}

bool HttpProtocol::HandlePostMultipartFormData(StreamString &contentType, StreamString &content) {

    const char8* parsedBoundaryTemp = StringHelper::SearchString(contentType.Buffer(), "boundary=");
    bool ret = (parsedBoundaryTemp != NULL);
    if (!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "In multipart/form-data a boundary must be specified");
    }

    if (ret) {
        //Store the boundary identifier
        StreamString parsedBoundary = &parsedBoundaryTemp[StringHelper::Length("boundary=")];
        //Check if the boundary contains quotes and remove if so
        if (parsedBoundary.Size() > 1ull) {
            // remove the ""
            uint32 finalIndex = static_cast<uint32>(parsedBoundary.Size() - 1u);
            if ((parsedBoundary[0u] == '\"')) {
                if ((parsedBoundary[finalIndex] == '\"')) {
                    parsedBoundary = (&(parsedBoundary.Buffer()[1]));

                    ret = parsedBoundary.SetSize(static_cast<uint64>(finalIndex) - 1ull);
                }
            }
        }
        if (ret) {
            //add the prefix
            StreamString boundary = "--";
            boundary += parsedBoundary;

            //The header is not written in the cdb
            bool headerHandled = false;
            //The contents (after the header are stored in this field
            StreamString value;

            ret = content.Seek(0ull);
            if (ret) {
                StreamString line;
                //The cdb name key
                StreamString name = "Unknown";
                //If a filename is received
                StreamString filename;
                while ((content.GetLine(line)) && (ret)) {
                    //The header is handled when an empty line is detected
                    bool lineEmpty = (line.Size() == 0ull);
                    headerHandled = (headerHandled || lineEmpty);
                    if (line.Size() > 0ull) {
                        //While the header is not handled (separated from main content by empty line)
                        //look for name and filename
                        if (!headerHandled) {
                            //store name and filename
                            ret = HandlePostHeader(line, content, name, filename);
                        }
                        //if the header is handled
                        else {
                            //parse and store the actual message content
                            ret = HandlePostContent(line, boundary, name, filename, value, headerHandled);
                        }
                        if (ret) {
                            ret = line.SetSize(0ull);
                        }
                    }
                }
            }
        }
    }
    return ret;
}

bool HttpProtocol::HandlePostApplicationForm(StreamString &content) {

    bool ret = true;
    StreamString line;

    if (content.GetLine(line)) {
        ret = line.Seek(0ull);
        if (ret) {
            StreamString command;
            char8 terminator;
            while (line.GetToken(command, "&", terminator)) {
                ret = command.Seek(0ull);
                if (ret) {
                    if (command.Size() > 3ull) {
                        StreamString variable;
                        StreamString value;
                        ret = command.GetToken(variable, "=", terminator);
                        if (ret) {
                            (void) command.GetToken(value, ";", terminator);
                            ret = value.Seek(0ull);
                        }
                        if (ret) {
                            ret = variable.Seek(0ull);
                        }
                        StreamString decodedValue;
                        StreamString decodedVariable;
                        if (ret) {
                            ret = HttpDefinition::HttpDecode(decodedValue, value);
                        }
                        if (ret) {
                            ret = HttpDefinition::HttpDecode(decodedVariable, variable);
                        }
                        if (ret) {
                            ret = decodedValue.Seek(0ull);
                        }
                        if (ret) {
                            ret = Write(decodedVariable.Buffer(), decodedValue.Buffer());
                        }
                    }
                }
                if (ret) {
                    ret = command.SetSize(0ull);
                }
            }
        }
    }

    return ret;
}

bool HttpProtocol::HandlePost(StreamString &contentType, StreamString &content) {
//Write the raw content
    bool ret = content.Seek(0ull);
    if (ret) {
        StreamString key = "rawPost";

        //we are in InputCommands
        ret = Write(key.Buffer(), content.Buffer());
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed writing the raw post content");
        }

        if (ret) {
            ret = content.Seek(0ull);
            if (ret) {
                //Check if it is a "multipart/form-data"
                if (StringHelper::SearchString(contentType.Buffer(), "multipart/form-data") != NULL) {
                    ret = HandlePostMultipartFormData(contentType, content);
                }
                else if (StringHelper::SearchString(contentType.Buffer(), "application/x-www-form-urlencoded") != NULL) {
                    //read the content. Key values encoded as in a GET url
                    ret = HandlePostApplicationForm(content);
                }
                else {
                }
            }
        }
    }
    return ret;
}

bool HttpProtocol::SecurityCheck(ReferenceT<HttpRealmI> realm) {
    bool ret = false;

    // no valid realm !
    if (realm.IsValid()) {
        // get key. on failure exit
        StreamString authorisationKey;
        if (MoveAbsolute("InputOptions")) {
            if (Read("Authorization", authorisationKey)) {
                /*lint -e{740} outputStream may be a BasicSocket*/
                BasicSocket* mySocket = dynamic_cast<BasicSocket *>(outputStream);
                ret = mySocket != NULL_PTR(BasicSocket*);
                if (ret) {
                    /*lint -e{613} NULL pointer checked*/
                    ret = realm->Validate(authorisationKey.Buffer(), httpCommand, (mySocket->GetSource()).GetAddressAsNumber());
                }
            }
            (void) MoveToAncestor(1u);
        }
    }
    return ret;
}

bool HttpProtocol::KeepAlive() const {
    return keepAlive;
}

void HttpProtocol::SetKeepAlive(const bool isKeepAlive) {
    keepAlive = isKeepAlive;
}

int32 HttpProtocol::GetHttpCommand() const {
    return httpCommand;

}

void HttpProtocol::SetUnmatchedId(const char8 * const unMatchedIdIn) {
    unMatchedUrl = unMatchedIdIn;
}

void HttpProtocol::GetUnmatchedId(StreamString& unmatchedIdOut) const {
    unmatchedIdOut = unMatchedUrl;
}

void HttpProtocol::GetPath(StreamString& pathOut) const {
    pathOut = path;
}

void HttpProtocol::GetId(StreamString& idOut) const {
    idOut = url;
}

int8 HttpProtocol::TextMode() const {
    return textMode;
}

bool HttpProtocol::GetInputCommand(const char8 * const commandName, const AnyType &commandValue) {

    bool ret = MoveAbsolute("InputCommands");
    if (ret) {
        ret = Read(commandName, commandValue);
    }
    return ret;
}

}
