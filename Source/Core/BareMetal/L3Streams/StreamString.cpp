/**
 * @file StreamString.cpp
 * @brief Source file for class StreamString
 * @date 06/10/2015
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
 * the class StreamString (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StreamString.h"
#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

StreamString::StreamString() :
        BufferedStream() {
}

/** @brief Copy constructor from a const char8*. */
StreamString::StreamString(const char8 * const initialisationString) :
        BufferedStream() {
    if (initialisationString != static_cast<const char8 *>(NULL)) {
        if(!AppendOrSet(initialisationString,false)) {
            // TODo
        }
    }
}

StreamString::operator AnyType() {
    AnyType at(Buffer());
    return at;
}

/** Destructor */
StreamString::~StreamString() {
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: BufferedStream must have the access to the final buffers.*/
IOBuffer *StreamString::GetInputBuffer() {
    return &buffer;
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: BufferedStream must have the access to the final buffers.*/
IOBuffer *StreamString::GetOutputBuffer() {
    return &buffer;
}

/**
 Reads data into buffer.
 As much as size byte are read,
 actual read size is returned in size. (unless complete = true)
 msecTimeout is how much the operation should last - no more - if not any (all) data read then return false
 timeout behaviour depends on class characteristics and sync mode.
 */
bool StreamString::Read(char8* const bufferIn,
                        uint32 & size) {
    return this->buffer.Read(&bufferIn[0], size);
}

/**
 Write data from a buffer to the stream.
 As much as size byte are written,
 actual written size is returned in size.
 msecTimeout is how much the operation should last.
 timeout behaviour depends on class characteristics and sync mode.
 */
bool StreamString::Write(const char8* const bufferIn,
                         uint32 & size) {
    return this->buffer.Write(&bufferIn[0], size);

}

/** whether it can be written into */
bool StreamString::CanWrite() const {
    return true;
}

/** whether it can be  read */
bool StreamString::CanRead() const {
    return true;
}

/** The size of the stream */
uint64 StreamString::Size() {
    return buffer.UsedSize();
}

/** Moves within the file to an absolute location */
bool StreamString::Seek(const uint64 pos) {
    bool retval = true;
    uint32 usedSize = buffer.UsedSize();
    if (pos > usedSize) {
//REPORT_ERROR_PARAMETERS(ParametersError,"pos=%i out of range=[0-%i] , moving to end of stream",pos,usedSize)
        if (!buffer.Seek(usedSize)) {
            //TODO
        }
        retval = false;
    }

    return (retval) ? (buffer.Seek(static_cast<uint32>(pos))) : false;
}

/** Moves within the file relative to current location */
bool StreamString::RelativeSeek(const int32 deltaPos) {
    return buffer.RelativeSeek(deltaPos);
}

/** Returns current position */
uint64 StreamString::Position() {
    return buffer.Position();
}

/** Clip the string size to a specified point
 @param newStringSize The size of the buffer.
 @return true if successful. false otherwise.
 */
bool StreamString::SetSize(const uint64 size) {
    if (!buffer.SetBufferAllocationSize(static_cast<uint32>(size) + 1u)) {
        //TODO
    }
    return true;
}

/** can you move the pointer */
bool StreamString::CanSeek() const {
    return true;
}

/**
 * @brief Copy a character into the StreamString buffer.
 * @param  c the character to be copied.
 * @return true if successful. false otherwise.
 */
bool StreamString::AppendOrSet(const char8 c,
                               const bool append) {
    if (append) {
        if (!buffer.Seek(buffer.UsedSize())) {
            //TODO
        }
    }
    else {
        buffer.Empty();
    }
    bool ret = buffer.PutC(c);
//buffer.Terminate();
    return ret;
}

/**
 * @brief Copy a string into the StreamString buffer.
 * @param  s The pointer to the string to be copied
 * @return true if successful. false otherwise.
 */
bool StreamString::AppendOrSet(const char8 * const s,
                               const bool append) {

    bool ret = false;
    if (s != NULL) {

        uint32 size = StringHelper::Length(s);

        if (append) {
            if(!buffer.Seek(buffer.UsedSize())) {
                //TODO
            }
        }
        else {
            buffer.Empty();
        }
        ret=buffer.Write(s,size);

    }
    return ret;
}

/**
 * @brief Copy a StreamString into a StreamString.
 * @param  s The StreamString to be copied.
 * @return true if successful. false otherwise.
 */
bool StreamString::AppendOrSet(const StreamString &s,
                               const bool append) {

    if (append) {
        if (!buffer.Seek(buffer.UsedSize())) {
            //TODO
        }
    }
    else {
        buffer.Empty();
    }

    uint32 size = s.buffer.UsedSize();
    return buffer.Write(s.buffer.Buffer(), size);
}

/** Checks if a char8 is in the string
 @param c The character to look for.
 @return >0 the first position if found. -1 otherwise.
 */
int32 StreamString::Locate(const char8 c) const {
// Stream::Size is not const!

    uint32 ret = 0xffffffffu;
    if (buffer.UsedSize() != 0u) {

        const char8 *string = buffer.Buffer();
        if (string != NULL) {

            uint32 index = 0u;
            while (index < buffer.UsedSize()) {
                if (string[index] == c) {
                    ret = index;
                    break;
                }
                index++;
            }
        }
    }
    return static_cast<int32>(ret);
}

/** Checks if a string is contained in the string.
 @param x The string to look for.
 @return >0 the first position if found. -1 otherwise.
 */
int32 StreamString::Locate(const StreamString &x) const {

    bool ok = (x.buffer.UsedSize() != 0u) && (x.buffer.UsedSize() <= buffer.UsedSize()) && (x.buffer.Buffer() != NULL)&& (buffer.Buffer() != NULL);

    uint32 ret=0xffffffffu;
    if(ok) {

        const char8 *string = buffer.Buffer();
        const char8 *pattern = x.buffer.Buffer();

        uint32 index = 0u;
// no point to try match the tail of the string if it is smaller than the pattern
        uint32 maxIndex = ((1u + buffer.UsedSize()) - x.buffer.UsedSize());
// loop through the string characters
        while (index < maxIndex) {
            // detect the start as a potential match
            if (string[index] == pattern[0]) {
                uint32 index2 = 1u;
                const char8 *stringSegment = &string[index];
                // check the remainder
                while (index2 < x.buffer.UsedSize()) {
                    if (stringSegment[index2] != pattern[index2]) {
                        break;
                    }
                    index2++;
                }
                // found it as we exit with index2 at the max value
                if (index2 == x.buffer.UsedSize()) {
                    ret= index;
                    break;
                }
            }
            index++;
        }
    }

    return static_cast<int32>(ret);
}

/*lint -e{715} [MISRA C++ Rule 0-1-11]. Justification: This implementation does not requires input arguments.*/
bool StreamString::UnbufferedWrite(const char8 * const bufferIn,
                                   uint32 & size,
                                   const TimeoutType &msecTimeout) {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11]. Justification: This implementation does not requires input arguments.*/
bool StreamString::UnbufferedRead(char8 * const bufferIn,
                                  uint32 & size,
                                  const TimeoutType &msecTimeout) {
    return false;
}

uint64 StreamString::UnbufferedSize() {
    return 0u;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11]. Justification: This implementation does not requires input arguments.*/
bool StreamString::UnbufferedSeek(const uint64 pos) {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11]. Justification: This implementation does not requires input arguments.*/
bool StreamString::UnbufferedRelativeSeek(const int32 deltaPos) {
    return false;
}

uint64 StreamString::UnbufferedPosition() {
    return 0u;
}

/*lint -e{715} [MISRA C++ Rule 0-1-119]. Justification: The default behavior does not need the input argument.*/
bool StreamString::UnbufferedSetSize(const uint64 size) {
    return false;
}

}

