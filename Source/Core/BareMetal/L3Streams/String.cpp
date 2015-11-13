/**
 * @file String.cpp
 * @brief Source file for class String
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
 * the class String (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "String.h"
#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

String::String() :
        BufferedStreamI() {
    //Initialise and terminate an empty string
    bool ret;
    ret = buffer.SetBufferAllocationSize(0u);

    if (!ret) {
        REPORT_ERROR(ErrorManagement::FatalError, "String: Failed initialization of the String buffer during construction.");
    }
}

String::String(const char8 * const initialisationString) :
        BufferedStreamI() {
    //Initialise and terminate an empty string
    bool ret;
    ret = buffer.SetBufferAllocationSize(0u);

    if (!ret) {
        REPORT_ERROR(ErrorManagement::FatalError, "String: Failed initialization of the String buffer during construction.");
    }

    if (initialisationString != static_cast<const char8 *>(NULL)) {
        if (!Set(initialisationString)) {
            REPORT_ERROR(ErrorManagement::FatalError, "String: Failed Set() function");
        }
    }
}

/*lint -e{1738} . Justification: StreamI is only an interface there is nothing to be copied. */
String::String(const String &toCopy) :
        BufferedStreamI() {
    //Initialise and terminate an empty string
    bool ret;
    ret = buffer.SetBufferAllocationSize(0u);

    if (!ret) {
        REPORT_ERROR(ErrorManagement::FatalError, "String: Failed initialization of the String buffer during construction.");
    }

    if (&toCopy != this) {
        if (!Set(toCopy)) {
            REPORT_ERROR(ErrorManagement::FatalError, "String: Failed Set() function");
        }
    }
}

String::operator AnyType() {
    AnyType at(Buffer());
    return at;
}

String::~String() {
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: BufferedStreamI must have the access to the final buffers.*/
IOBuffer *String::GetReadBuffer() {
    return &buffer;
}

/*lint -e{1536} [MISRA C++ Rule 9-3-1], [MISRA C++ Rule 9-3-2]. Justification: BufferedStreamI must have the access to the final buffers.*/
IOBuffer *String::GetWriteBuffer() {
    return &buffer;
}

bool String::Read(char8* const output,
                  uint32 & size) {
    return this->buffer.Read(&output[0], size);
}

bool String::Write(const char8* const input,
                   uint32 & size) {
    return this->buffer.Write(&input[0], size);

}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the timeout parameter is not used here but it is
 * used by other buffered streams. */
bool String::Read(char8 * const output,
                  uint32 & size,
                  const TimeoutType &timeout) {
    return Read(output, size);
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the timeout parameter is not used here but it is
 * used by other buffered streams. */
bool String::Write(const char8 * const input,
                   uint32 & size,
                   const TimeoutType &timeout) {
    return Write(input, size);
}

bool String::CanWrite() const {
    return true;
}

bool String::CanRead() const {
    return true;
}

uint64 String::Size() {
    return buffer.UsedSize();
}

bool String::Seek(const uint64 pos) {
    bool retval = true;
    uint32 usedSize = buffer.UsedSize();
    if (pos > usedSize) {
        if (!buffer.Seek(usedSize)) {
            REPORT_ERROR(ErrorManagement::FatalError, "String: Failed IOBuffer::Seek() function");
        }
        retval = false;
    }

    return (retval) ? (buffer.Seek(static_cast<uint32>(pos))) : false;
}

bool String::RelativeSeek(const int32 deltaPos) {
    return buffer.RelativeSeek(deltaPos);
}

uint64 String::Position() {
    return buffer.Position();
}

bool String::SetSize(const uint64 size) {
    return buffer.SetBufferAllocationSize(static_cast<uint32>(size) + 1u);
}

bool String::CanSeek() const {
    return true;
}

bool String::Append(const char8 c) {
    bool ret = false;
    if (!buffer.Seek(buffer.UsedSize())) {
        REPORT_ERROR(ErrorManagement::FatalError, "String: Failed IOBuffer::Seek() function");
    }
    else {
        ret = buffer.PutC(c);
    }
    //TODO: Call to buffer.Terminate(); ??
    return ret;
}

bool String::Set(const char8 c) {
    buffer.Empty();
    bool ret = buffer.PutC(c);
    //TODO: Call to buffer.Terminate(); ??
    return ret;
}

bool String::Append(const char8 * const s) {
    bool ret = false;
    if (s != NULL) {
        uint32 size = StringHelper::Length(s);
        if (!buffer.Seek(buffer.UsedSize())) {
            REPORT_ERROR(ErrorManagement::FatalError, "String: Failed IOBuffer::Seek() function");
        }
        else {
            ret = buffer.Write(s, size);
        }
    }
    return ret;
}

bool String::Set(const char8 * const s) {
    bool ret = false;
    if (s != NULL) {
        uint32 size = StringHelper::Length(s);
        buffer.Empty();
        ret = buffer.Write(s, size);
    }
    return ret;
}

bool String::Append(const String &s) {
    bool ret = false;
    if (!buffer.Seek(buffer.UsedSize())) {
        REPORT_ERROR(ErrorManagement::FatalError, "String: Failed IOBuffer::Seek() function");
    }
    else {
        uint32 size = s.buffer.UsedSize();
        ret = buffer.Write(s.buffer.Buffer(), size);
    }
    return ret;
}

bool String::Set(const String &s) {
    buffer.Empty();
    uint32 size = s.buffer.UsedSize();
    return buffer.Write(s.buffer.Buffer(), size);
}

int32 String::Locate(const char8 c) const {

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

int32 String::Locate(const String &x) const {

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
