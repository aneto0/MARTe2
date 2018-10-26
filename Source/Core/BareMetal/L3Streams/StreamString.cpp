/**
 * @file StreamString.cpp
 * @brief Source file for class StreamString
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
 * the class StreamString (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API
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
        BufferedStreamI() {
    //Initialise and terminate an empty string
    bool ret;
    ret = buffer.SetBufferAllocationSize(0u);

    if (!ret) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StreamString: Failed initialization of the StreamString buffer during construction.");
    }
}

StreamString::StreamString(const char8 * const initialisationString) :
        BufferedStreamI() {
    //Initialise and terminate an empty string
    bool ret;
    ret = buffer.SetBufferAllocationSize(0u);

    if (!ret) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StreamString: Failed initialization of the StreamString buffer during construction.");
    }

    if (initialisationString != static_cast<const char8 *>(NULL)) {
        if (!Set(initialisationString)) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StreamString: Failed Set() function");
        }
    }
}

StreamString::StreamString(CCString initialisationString) :
        BufferedStreamI() {
    //Initialise and terminate an empty string
    bool ret;
    ret = buffer.SetBufferAllocationSize(0u);

    if (!ret) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StreamString: Failed initialization of the StreamString buffer during construction.");
    }

    if (initialisationString.GetList()!= static_cast<const char8 *>(NULL)) {
        if (!Set(initialisationString.GetList())) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StreamString: Failed Set() function");
        }
    }
}

/*lint -e{1738} . Justification: StreamI is only an interface there is nothing to be copied. */
StreamString::StreamString(const StreamString &toCopy) :
        BufferedStreamI() {
    //Initialise and terminate an empty string
    bool ret;
    ret = buffer.SetBufferAllocationSize(0u);

    if (!ret) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StreamString: Failed initialization of the StreamString buffer during construction.");
    }

    if (&toCopy != this) {
        if (!Set(toCopy)) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StreamString: Failed Set() function");
        }
    }
}

StreamString::operator AnyType() {
    void *dataPointer = static_cast<void *>(this);
    TypeDescriptor dataDescriptor(false, SString, static_cast<uint16>(sizeof(StreamString) * 8u));

    return AnyType(dataDescriptor, static_cast<uint8>(0u), dataPointer);
}

StreamString::~StreamString() {
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: BufferedStreamI must have the access to the final buffers.*/
IOBuffer *StreamString::GetReadBuffer() {
    return &buffer;
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: BufferedStreamI must have the access to the final buffers.*/
IOBuffer *StreamString::GetWriteBuffer() {
    return &buffer;
}

bool StreamString::Read(char8* const output,
                        uint32 & size) {
    return this->buffer.Read(&output[0], size);
}

bool StreamString::Write(const char8* const input,
                         uint32 & size) {
    return this->buffer.Write(&input[0], size);

}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the timeout parameter is not used here but it is
 * used by other buffered streams. */
bool StreamString::Read(char8 * const output,
                        uint32 & size,
                        const TimeoutType &timeout) {
    return Read(output, size);
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the timeout parameter is not used here but it is
 * used by other buffered streams. */
bool StreamString::Write(const char8 * const input,
                         uint32 & size,
                         const TimeoutType &timeout) {
    return Write(input, size);
}

bool StreamString::CanWrite() const {
    return true;
}

bool StreamString::CanRead() const {
    return true;
}

uint64 StreamString::Size() {
    return buffer.UsedSize();
}

bool StreamString::Seek(const uint64 pos) {
    bool retval = true;
    uint32 usedSize = buffer.UsedSize();
    if (pos > usedSize) {
        if (!buffer.Seek(usedSize)) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StreamString: Failed IOBuffer::Seek() function");
        }
        retval = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StreamString: Desired Position greater than current size: moved to end");
    }

    return (retval) ? (buffer.Seek(static_cast<uint32>(pos))) : false;
}

bool StreamString::RelativeSeek(const int64 deltaPos) {

    bool ret = true;
    if ((deltaPos > MAX_INT32) || (deltaPos < MIN_INT32)) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "RelativeSeek: The seek offset should be in the int32 range");
        ret = false;
    }
    else {
        ret = buffer.RelativeSeek(static_cast<int32>(deltaPos));
    }
    return ret;
}

uint64 StreamString::Position() {
    return buffer.Position();
}

bool StreamString::SetSize(const uint64 size) {
    return buffer.SetBufferAllocationSize(static_cast<uint32>(size));
}

bool StreamString::CanSeek() const {
    return true;
}

bool StreamString::Append(const char8 c) {
    bool ret = false;
    if (!buffer.Seek(buffer.UsedSize())) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StreamString: Failed IOBuffer::Seek() function");
    }
    else {
        ret = buffer.PutC(c);
    }
    //TODO: Call to buffer.Terminate(); ??
    return ret;
}

bool StreamString::Set(const char8 c) {
    buffer.Empty();
    bool ret = buffer.PutC(c);
    //TODO: Call to buffer.Terminate(); ??
    return ret;
}

bool StreamString::Append(const char8 * const s) {
    bool ret = false;
    if (s != NULL) {
        uint32 size = StringHelper::Length(s);
        if (!buffer.Seek(buffer.UsedSize())) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StreamString: Failed IOBuffer::Seek() function");
        }
        else {
            ret = buffer.Write(s, size);
        }
    }
    return ret;
}

bool StreamString::Set(const char8 * const s) {
    bool ret = false;
    if (s != NULL) {
        uint32 size = StringHelper::Length(s);
        buffer.Empty();
        ret = buffer.Write(s, size);
    }
    return ret;
}

bool StreamString::Append(const StreamString &s) {
    bool ret = false;
    if (!buffer.Seek(buffer.UsedSize())) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "StreamString: Failed IOBuffer::Seek() function");
    }
    else {
        uint32 size = s.buffer.UsedSize();
        ret = buffer.Write(s.buffer.Buffer(), size);
    }
    return ret;
}

bool StreamString::Set(const StreamString &s) {
    buffer.Empty();
    uint32 size = s.buffer.UsedSize();
    return buffer.Write(s.buffer.Buffer(), size);
}

int32 StreamString::Locate(const char8 c) const {

    uint32 ret = 0xffffffffu;
    if (buffer.UsedSize() > 0u) {

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

int32 StreamString::Locate(const StreamString &x) const {

    bool ok = (x.buffer.UsedSize() > 0u) && (buffer.UsedSize() > 0u) && (x.buffer.UsedSize() <= buffer.UsedSize());

    uint32 ret = 0xffffffffu;
    if (ok) {

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
                    ret = index;
                    break;
                }
            }
            index++;
        }
    }

    return static_cast<int32>(ret);
}

}
