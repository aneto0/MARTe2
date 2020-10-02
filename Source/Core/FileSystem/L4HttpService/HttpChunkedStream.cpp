/**
 * @file HttpChunkedStream.cpp
 * @brief Source file for class HttpChunkedStream
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

 * @details This source file contains the definition of all the methods for
 * the class HttpChunkedStream (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ErrorManagement.h"
#include "HttpChunkedStream.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

HttpChunkedStream::HttpChunkedStream() :
        TCPSocket() {
    //use always buffer mode
    chunkMode = false;
    calibReadParam = 0u;
    calibWriteParam = 0u;

    uint32 readBufferSize = 32u;
    uint32 writeBufferSize = 4096u;

    /*lint -e{1506} Buffer size needs to be set before any method is called */
    bool ret = SetBufferSize(readBufferSize, writeBufferSize);
    if(!ret) {
        REPORT_ERROR_STATIC(
            ErrorManagement::ParametersError,
            "Failure setting buffer sizes, expected %u/%u - actual %u/%u (R/W)",
	    readBufferSize, writeBufferSize,
            GetReadBufferSize(), GetWriteBufferSize());
    }
}

HttpChunkedStream::~HttpChunkedStream() {
}

bool HttpChunkedStream::Flush() {

    bool ret = true;
    uint32 size = writeBuffer.UsedSize();

    if (chunkMode) {
        //get the size
        if (size > 0u) {
            StreamString totStr;
            ret = totStr.Printf("%x\r\n", size);

            uint32 totalSize = static_cast<uint32>(totStr.Size());
            if (ret) {
                ret = OSWrite(totStr.Buffer(), totalSize);
            }
        }

    }

    if (ret) {
        ret = DoubleBufferedStream::Flush();
    }
    if (ret) {

        if (chunkMode && (size > 0u)) {
            uint32 totalSize = 2u;
            ret = OSWrite("\r\n", totalSize);
        }
    }

    return ret;
}

bool HttpChunkedStream::FinalChunk() {
    const char8 *finalChunk = "0\r\n\r\n";
    uint32 totalSize = StringHelper::Length(finalChunk);

    return OSWrite(finalChunk, totalSize);

}

void HttpChunkedStream::SetChunkMode(const bool chunkModeIn) {
    chunkMode = chunkModeIn;
}

bool HttpChunkedStream::IsChunkMode() const {
    return chunkMode;
}

}

