/**
 * @file BufferedStreamI.cpp
 * @brief Source file for class BufferedStreamI
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

 * @details This source file contains the definition of all the methods for
 * the class BufferedStreamI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BufferedStreamI.h"
#include "AdvancedErrorManagement.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

BufferedStreamI::BufferedStreamI() :
        StreamI() {

    calibReadParam=4u;
    calibWriteParam=4u;

}

BufferedStreamI::~BufferedStreamI() {
}

bool BufferedStreamI::GetToken(char8 * const outputBuffer,
                               const char8 * const terminator,
                               const uint32 outputBufferSize,
                               char8 &saveTerminator,
                               const char8 * const skipCharacters) {

    bool retval = false;
// retrieve stream mechanism
    IOBuffer *inputIOBuffer = GetReadBuffer();
    if (inputIOBuffer != NULL) {
        if(CanRead()) {
            retval = inputIOBuffer->GetToken(outputBuffer, terminator, outputBufferSize, saveTerminator, skipCharacters);
        }
    }
    return retval;
}

bool BufferedStreamI::GetToken(BufferedStreamI & output,
                               const char8 * const terminator,
                               char8 &saveTerminator,
                               const char8 * const skipCharacters) {

// retrieve stream mechanism
    IOBuffer *inputIOBuffer = GetReadBuffer();
    IOBuffer *outputIOBuffer = output.GetWriteBuffer();

    bool ret = false;

    if ((inputIOBuffer != NULL) && (outputIOBuffer != NULL)) {
        if(CanRead()) {
            ret = inputIOBuffer->GetToken(*outputIOBuffer, terminator, saveTerminator, skipCharacters);
        }
    }

    return ret;
}

bool BufferedStreamI::SkipTokens(const uint32 count,
                                 const char8 * const terminator) {

    bool ret = CanRead();
    if (ret) {
// retrieve stream mechanism
        IOBuffer *inputBuffer = GetReadBuffer();
        if (inputBuffer != NULL) {
            ret = inputBuffer->SkipTokens(count, terminator);
        }
    }

    return ret;
}

bool BufferedStreamI::GetLine(BufferedStreamI & output,
                              const bool skipTerminators) {
    const char8 *skipCharacters = "\r";
    if (!skipTerminators) {
        /*lint -e{774} [MISRA C++ Rule 0-1-1], [MISRA C++ Rule 0-1-2], [MISRA C++ Rule 0-1-9]. Justification: It is a compilation time platform constant. */
        /*lint -e{948} [MISRA C++ Rule 0-1-9]. Justification: It is a compilation time platform constant. */
        /*lint -e{506} [MISRA C++ Rule 0-1-1]. Justification: It is a compilation time platform constant. */
        if (N_CHARS_NEWLINE == 1u) {
            skipCharacters = "";
        }
    }
    char8 terminator;
    return GetToken(output, "\n", terminator, skipCharacters);
}

bool BufferedStreamI::GetLine(char8 * const outputBuffer,
                              const uint32 outputBufferSize,
                              const bool skipTerminators) {
    const char8 *skipCharacters = "\r";
    if (!skipTerminators) {
        /*lint -e{774} [MISRA C++ Rule 0-1-1], [MISRA C++ Rule 0-1-2], [MISRA C++ Rule 0-1-9]. Justification: It is a compilation time platform constant. */
        /*lint -e{948} [MISRA C++ Rule 0-1-9]. Justification: It is a compilation time platform constant. */
        /*lint -e{506} [MISRA C++ Rule 0-1-1]. Justification: It is a compilation time platform constant. */
        if (N_CHARS_NEWLINE == 1u) {
            skipCharacters = "";
        }
    }
    char8 terminator;
    return GetToken(outputBuffer, "\n", outputBufferSize, terminator, skipCharacters);
}

bool BufferedStreamI::PrintFormatted(const char8 * const format,
                                     const AnyType pars[]) {

    bool ret = CanWrite();
// retrieve stream mechanism
// the output buffer is flushed in streamable.
    if (ret) {
        IOBuffer *outputBuffer = GetWriteBuffer();
        if (outputBuffer != NULL) {

            ret = outputBuffer->PrintFormatted(format, pars);

        }
    }
    return ret;
}

bool BufferedStreamI::Copy(const char8 * const buffer) {

    bool ret = false;
    if (buffer != NULL) {

        uint32 len = static_cast<uint32>(StringHelper::Length(buffer));

        ret = Write(buffer, len);
    }

    return ret;
}

bool BufferedStreamI::Copy(BufferedStreamI &stream) {

    char8 buffer[256];
    uint32 size = static_cast<uint32>(sizeof(buffer));

    //read in buffer
    bool ret = stream.Read(&buffer[0], size);
    while ((ret) && (size > 0u)) {

        //write in buffer
        ret = Write(&buffer[0], size);
        //in case of ret false don't write again
        size = 0u;

        //if successful, read again and size becomes zero if
        //there is no more data to read
        if (ret) {
            size = static_cast<uint32>(sizeof(buffer));
            ret = stream.Read(&buffer[0], size);
        }
    }
    //if exit because ret is false
    //something was read in buffer
    if (size > 0u) {
        //write on the stream
        //ret = ret && Write(buffer,size,TTDefault,true);
        bool writeOk = (Write(&buffer[0], size));
        ret = (ret) && (writeOk);
        //size = 0;
    }

    return ret;

}

bool BufferedStreamI::Flush() {
    IOBuffer* buff = GetWriteBuffer();
    bool ret = (buff != NULL_PTR(IOBuffer*));
    if (ret) {
        ret = buff->NoMoreSpaceToWrite();
    }
    return ret;
}

bool BufferedStreamI::Refill() {
    IOBuffer* buff = GetReadBuffer();
    bool ret = (buff != NULL_PTR(IOBuffer*));
    if (ret) {
        ret = buff->NoMoreDataToRead();
    }
    return ret;
}



}
