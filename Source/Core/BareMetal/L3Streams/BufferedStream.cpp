/**
 * @file BufferedStream.cpp
 * @brief Source file for class BufferedStream
 * @date 05/ott/2015
 * @author pc
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
 * the class BufferedStream (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BufferedStream.h"
#include "AdvancedErrorManagement.h"
#include "StringHelper.h"
#include "IOBufferFunctions.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

BufferedStream::BufferedStream() {
}

BufferedStream::~BufferedStream() {
}

bool BufferedStream::GetToken(char8 * const outputBuffer,
                              const char8 * const terminator,
                              const uint32 outputBufferSize,
                              char8 * const saveTerminator,
                              const char8 * const skipCharacters) {

    bool retval = false;
// retrieve stream mechanism
    IOBuffer *inputIOBuffer = GetInputBuffer();
    if (inputIOBuffer != NULL) {

        retval= GetTokenFromStream(*inputIOBuffer, outputBuffer, terminator, outputBufferSize, saveTerminator, skipCharacters);
    }
    return retval;
}

bool BufferedStream::GetToken(BufferedStream & output,
                              const char8 * const terminator,
                              char8 * const saveTerminator,
                              const char8 * const skipCharacters) {

// retrieve stream mechanism
    IOBuffer *inputIOBuffer = GetInputBuffer();
    IOBuffer *outputIOBuffer = output.GetOutputBuffer();

    bool ret = false;

    if ((inputIOBuffer != NULL) && (outputIOBuffer!=NULL)) {
        ret = GetTokenFromStream(*inputIOBuffer, *outputIOBuffer,terminator,saveTerminator,skipCharacters);
    }

    return ret;
}

bool BufferedStream::SkipTokens(const uint32 count,
                                const char8 * const terminator) {

    bool ret = false;
// retrieve stream mechanism
    IOBuffer *inputBuffer = GetInputBuffer();
    if (inputBuffer != NULL) {
        ret= SkipTokensInStream(*inputBuffer,count,terminator);
    }

    return ret;
}

bool BufferedStream::Print(const AnyType& par,
                           const FormatDescriptor &fd) {

    bool ret = false;
// retrieve stream mechanism
    IOBuffer *outputBuffer = GetOutputBuffer();
    if (outputBuffer != NULL) {
        ret= PrintToStream(*outputBuffer, par, fd);
    }

    return ret;
}

bool BufferedStream::PrintFormatted(const char8 * const format,
                                    const AnyType pars[]) {

    bool ret = false;
// retrieve stream mechanism
// the output buffer is flushed in streamable.
    IOBuffer *outputBuffer = GetOutputBuffer();
    if (outputBuffer != NULL) {

        ret=PrintFormattedToStream(*outputBuffer,format,pars);

    }
    return ret;
}

bool BufferedStream::Copy(const char8 * const buffer) {

    bool ret = false;
    if (buffer != NULL) {

        uint32 len = static_cast<uint32>(StringHelper::Length(buffer));

        ret=Write(buffer,len);
    }
    return ret;
}

bool BufferedStream::Copy(BufferedStream &stream) {

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
        bool writeOk = Write(&buffer[0], size);
        ret = (ret) && (writeOk);
//size = 0;
    }

    return ret;

}

}
