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

/** Destructor */
StreamString::~StreamString() {
}

///
IOBuffer *StreamString::GetInputBuffer() {
    return &buffer;
}

///
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
bool StreamString::Read(char8* buffer,
                        uint32 & size,
                        TimeoutType msecTimeout,
                        bool complete) {
    this->buffer.Read(buffer, size);
    return true;
}

/**
 Write data from a buffer to the stream.
 As much as size byte are written,
 actual written size is returned in size.
 msecTimeout is how much the operation should last.
 timeout behaviour depends on class characteristics and sync mode.
 */
bool StreamString::Write(const char8* buffer,
                         uint32 & size,
                         TimeoutType msecTimeout,
                         bool complete) {
    this->buffer.Write(buffer, size);

//this->buffer.Terminate();
    return true;

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
bool StreamString::Seek(uint64 pos) {
    bool retval = true;
    uint32 usedSize = buffer.UsedSize();
    if (pos > usedSize) {
//REPORT_ERROR_PARAMETERS(ParametersError,"pos=%i out of range=[0-%i] , moving to end of stream",pos,usedSize)
        buffer.Seek(usedSize);
        retval = false;
    }

    return (retval) ? (buffer.Seek((uint32) pos)) : false;
}

/** Moves within the file relative to current location */
bool StreamString::RelativeSeek(int32 deltaPos) {
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
bool StreamString::SetSize(uint64 size) {
    buffer.SetBufferAllocationSize(size + 1u);
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
bool StreamString::AppendOrSet(char8 c,
                               bool append) {
    if (append) {
        buffer.Seek(buffer.UsedSize());
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
bool StreamString::AppendOrSet(const char8 *s,
                               bool append) {

    bool ret = false;
    if (s != NULL) {

        uint32 size = StringHelper::Length(s);

        if (append) {
            buffer.Seek(buffer.UsedSize());
        }
        else {
            buffer.Empty();
        }
        buffer.Write(s,size);
        ret=true;
    }
    return ret;
}

/**
 * @brief Copy a StreamString into a StreamString.
 * @param  s The StreamString to be copied.
 * @return true if successful. false otherwise.
 */
bool StreamString::AppendOrSet(const StreamString &s,
                               bool append) {

    if (append) {
        buffer.Seek(buffer.UsedSize());
    }
    else {
        buffer.Empty();
    }

    uint32 size = s.buffer.UsedSize();
    buffer.Write(s.buffer.Buffer(), size);
//buffer.Terminate();
    return true;
}

/** Checks if a char8 is in the string
 @param c The character to look for.
 @return >0 the first position if found. -1 otherwise.
 */
int32 StreamString::Locate(char8 c) const {
// Stream::Size is not const!

    int32 ret = -1;
    if (buffer.UsedSize() != 0) {

        const char8 *string = buffer.Buffer();
        if (string != NULL) {

            uint32 index = 0;
            while (index < buffer.UsedSize()) {
                if (string[index] == c) {
                    ret = index;
                    break;
                }
                index++;
            }
        }
    }
    return ret;
}

/** Checks if a string is contained in the string.
 @param x The string to look for.
 @return >0 the first position if found. -1 otherwise.
 */
int32 StreamString::Locate(const StreamString &x) const {

    bool ok = (x.buffer.UsedSize() != 0) && (x.buffer.UsedSize() <= buffer.UsedSize()) && (x.buffer.Buffer() != NULL)&& (buffer.Buffer() != NULL);

    int32 ret=-1;
    if(ok) {

        const char8 *string = buffer.Buffer();
        const char8 *pattern = x.buffer.Buffer();

        uint32 index = 0u;
// no point to try match the tail of the string if it is smaller than the pattern
            uint32 maxIndex = (1u + buffer.UsedSize() - x.buffer.UsedSize());
// loop through the string characters
            while (index < maxIndex) {
                // detect the start as a potential match
                if (string[index] == pattern[0]) {
                    uint32 index2 = 1u;
                    const char8 *stringSegment = string + index;
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

        return ret;
    }
}

