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


bool BufferedStream::GetToken(char8 * outputBuffer,
                              const char8 * terminator,
                              uint32 outputBufferSize,
                              char8 * saveTerminator,
                              const char8 * skipCharacters) {

    bool retval = false;
// retrieve stream mechanism
    IOBuffer *inputIOBuffer = GetInputBuffer();
    if (inputIOBuffer != NULL) {

        retval= GetTokenFromStream(*inputIOBuffer, outputBuffer, terminator, outputBufferSize, saveTerminator, skipCharacters);
    }
    return retval;
}


bool BufferedStream::GetToken(BufferedStream & output,
                              const char8 * terminator,
                              char8 * saveTerminator,
                              const char8 * skipCharacters) {

// retrieve stream mechanism
    IOBuffer *inputIOBuffer = GetInputBuffer();
    IOBuffer *outputIOBuffer = output.GetOutputBuffer();

    bool ret = false;

    if ((inputIOBuffer != NULL) && (outputIOBuffer!=NULL)) {
        ret = GetTokenFromStream(*inputIOBuffer, *outputIOBuffer,terminator,saveTerminator,skipCharacters);
    }

    return ret;
}

bool BufferedStream::SkipTokens(uint32 count,
                                const char8 * terminator) {

    bool ret = false;
// retrieve stream mechanism
    IOBuffer *inputBuffer = GetInputBuffer();
    if (inputBuffer != NULL) {
        ret= SkipTokensInStream(*inputBuffer,count,terminator);
    }

    return ret;
}

bool BufferedStream::Print(const AnyType& par,
                           FormatDescriptor fd) {

    bool ret = false;
// retrieve stream mechanism
    IOBuffer *outputBuffer = GetOutputBuffer();
    if (outputBuffer != NULL) {
        ret= PrintToStream(*outputBuffer, par, fd);
    }

    return ret;
}

bool BufferedStream::PrintFormatted(const char8 *format,
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

bool BufferedStream::Copy(const char8 *buffer) {

    bool ret = false;
    if (buffer != NULL) {

        uint32 len = (uint32 )StringHelper::Length(buffer);

        ret=Write(buffer,len,TTDefault,true);
    }
    return ret;
}

bool BufferedStream::Copy(BufferedStream &stream) {

    char8 buffer[256];
    uint32 size = sizeof(buffer);

//read in buffer
    bool ret = stream.Read(buffer, size);
    while ((ret) && (size > 0)) {

//write in buffer
        ret = Write(buffer, size, TTDefault, true);
        //in case of ret false don't write again
        size = 0;

//if successful, read again and size becomes zero if
        //there is no more data to read
        if (ret) {
            size = sizeof(buffer);
            ret = stream.Read(buffer, size);
        }
    }
//if exit because ret is false
    //something was read in buffer
    if (size > 0) {
//write on the stream
//ret = ret && Write(buffer,size,TTDefault,true);
        Write(buffer, size, TTDefault, true);
//size = 0;
    }

    return ret;

}

}
