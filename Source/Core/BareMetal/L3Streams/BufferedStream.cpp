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
#include "StreamWrapperIOBuffer.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/// default destructor
BufferedStream::~BufferedStream() {
}

/** extract a token from the stream into a string data until a terminator or 0 is found.
 Skips all skip characters and those that are also terminators at the beginning
 returns true if some data was read before any error or file termination. false only on error and no data available
 The terminator (just the first encountered) is consumed in the process and saved in saveTerminator if provided
 skipCharacters=NULL is equivalent to skipCharacters = terminator
 {BUFFERED}    */
bool BufferedStream::GetToken(char8 * outputBuffer,
                              const char8 * terminator,
                              uint32 outputBufferSize,
                              char8 * saveTerminator,
                              const char8 * skipCharacters) {

// retrieve stream mechanism
    IOBuffer *inputIOBuffer = GetInputBuffer();
    if (inputIOBuffer == NULL) {
        char8 stackBuffer[64];
        StreamWrapperIOBuffer inputIOBuffer (this,stackBuffer,sizeof (stackBuffer));

        bool ret = GetTokenFromStream(inputIOBuffer, outputBuffer,terminator,outputBufferSize,saveTerminator,skipCharacters);

        return ret;
    }

    return GetTokenFromStream(*inputIOBuffer, outputBuffer, terminator, outputBufferSize, saveTerminator, skipCharacters);
}

/** extract a token from the stream into a string data until a terminator or 0 is found.
 Skips all skip characters and those that are also terminators at the beginning
 returns true if some data was read before any error or file termination. false only on error and no data available
 The terminator (just the first encountered) is consumed in the process and saved in saveTerminator if provided
 skipCharacters=NULL is equivalent to skipCharacters = terminator
 {BUFFERED}
 A character can be found in the terminator or in the skipCharacters list  in both or in none
 0) none                 the character is copied
 1) terminator           the character is not copied the string is terminated
 2) skip                 the character is not copied
 3) skip + terminator    the character is not copied, the string is terminated if not empty
 */
bool BufferedStream::GetToken(BufferedStream & output,
                              const char8 * terminator,
                              char8 * saveTerminator,
                              const char8 * skipCharacters) {

// retrieve stream mechanism
    IOBuffer *inputIOBuffer = GetInputBuffer();
    IOBuffer *outputIOBuffer = output.GetOutputBuffer();

    bool ret = false;

    if (inputIOBuffer == NULL) {
        char8 stackBuffer[64];
//create a buffer on the stack as the read buffer.
        StreamWrapperIOBuffer inputIOBufferS (this,stackBuffer,sizeof (stackBuffer));
        inputIOBuffer = &inputIOBufferS;

        if (outputIOBuffer == NULL) {
            char8 stackBuffer[64];
//create a buffer on the stack as the write buffer
            StreamWrapperIOBuffer outputIOBufferS (&output,stackBuffer,sizeof (stackBuffer));
            outputIOBuffer = &outputIOBufferS;

            ret = GetTokenFromStream(*inputIOBuffer, *outputIOBuffer,terminator,saveTerminator,skipCharacters);

        }
        else {
            ret = GetTokenFromStream(*inputIOBuffer, *outputIOBuffer,terminator,saveTerminator,skipCharacters);
        }
    }
    else {

        if (outputIOBuffer == NULL) {
            char8 stackBuffer[64];
//create a buffer on the stack as write buffer.
            StreamWrapperIOBuffer outputIOBufferS (&output,stackBuffer,sizeof (stackBuffer));
            outputIOBuffer = &outputIOBufferS;

            ret = GetTokenFromStream(*inputIOBuffer, *outputIOBuffer,terminator,saveTerminator,skipCharacters);

        }
        else {

            ret = GetTokenFromStream(*inputIOBuffer, *outputIOBuffer,terminator,saveTerminator,skipCharacters);
        }
    }

    return ret;
}

/** to skip a series of tokens delimited by terminators or 0
 {BUFFERED}    */
bool BufferedStream::SkipTokens(uint32 count,
                                const char8 * terminator) {

// retrieve stream mechanism
    IOBuffer *inputBuffer = GetInputBuffer();
    if (inputBuffer == NULL) {
        char8 stackBuffer[64];
        StreamWrapperIOBuffer inputBuffer (this,stackBuffer,sizeof (stackBuffer));

        return SkipTokensInStream(inputBuffer,count,terminator);
    }

    return SkipTokensInStream(*inputBuffer, count, terminator);
}

bool BufferedStream::Print(const AnyType& par,
                           FormatDescriptor fd) {

// retrieve stream mechanism
    IOBuffer *outputBuffer = GetOutputBuffer();
    if (outputBuffer == NULL) {
        char8 stackBuffer[64];
        StreamWrapperIOBuffer outputBuffer (this,stackBuffer,sizeof (stackBuffer));

        return PrintToStream(outputBuffer,par,fd);
    }

    return PrintToStream(*outputBuffer, par, fd);
}

bool BufferedStream::PrintFormatted(const char8 *format,
                                    const AnyType pars[]) {

// retrieve stream mechanism
// the output buffer is flushed in streamable.
    IOBuffer *outputBuffer = GetOutputBuffer();
    if (outputBuffer == NULL) {
        char8 stackBuffer[64];
        StreamWrapperIOBuffer outputBuffer (this,stackBuffer,sizeof (stackBuffer));

        return PrintFormattedToStream(outputBuffer,format,pars);

    }
    return PrintFormattedToStream(*outputBuffer, format, pars);
}

/**
 * copies a const char8* into this stream from current position
 */
bool BufferedStream::Copy(const char8 *buffer) {
    if (buffer == NULL) {
        return false;
    }

    uint32 len = (uint32 )StringHelper::Length(buffer);

    return Write(buffer,len,TTDefault,true);
}

/**
 * copies from stream current Position to end
 */
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
