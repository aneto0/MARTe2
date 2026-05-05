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

StreamString StreamString::SubString(const int32 start, const int32 stop){
    uint64 rStart = 0u;
    if(start < 0) {
        //lint -e{9125} -e{571} -e{9123} start < 0 => -start > 0
        uint64 x = static_cast<uint64>(-start);
        if(x < Size()){
            rStart = (Size() - x); 
        }
    }
    else{
        //lint -e{571} cast to the final type
        rStart = static_cast<uint64>(start);
    }
    uint64 rStop = 0u;
    if(stop < 0){
        //lint -e{9125} -e{571} -e{9123} stop < 0 => -stop > 0
        uint64 x = static_cast<uint64>(-stop);
        if(x < Size()){
            rStop = (Size() - x); 
        }
    }
    else{
        //lint -e{571} cast to the final type
        rStop = static_cast<uint64>(stop);
    }
    StreamString ret;
    //lint -e{9016} -e{620} This implementation uses pointer arithmetic instead of array indexing
    if(rStop >= rStart){
        ret = (Buffer() + rStart);
        if (!ret.SetSize((rStop - rStart) + 1llu)) {
            ret = "";
        }
    }
    return ret;
}


Vector<StreamString> StreamString::Split(const char8 * const pattern, const uint32 nSplits){
    Vector<StreamString> res(nSplits + 1u);
    if(pattern != NULL){
        uint32 patternL = StringHelper::Length(pattern);
        uint32 rnSplits = nSplits;
        //0 splits means all
        //Unfortunately need to compute the size before... need to add append in Vector
        if(nSplits == 0u){
            uint32 i = 0u;
            const char8 *buff = Buffer();
            uint32 curSize = static_cast<uint32>(Size());
            while(i < curSize){
                if(StringHelper::CompareN(&buff[i], pattern, patternL) == 0){
                    rnSplits++;
                    i+=patternL;
                }
                else{
                    i++;
                }
            }    
        }
        res.SetSize(rnSplits + 1u);
        int32 idx = 0;
        uint32 splitsCnt = 0u;
        StreamString lastElement = Buffer();
        uint32 i = 0u;
        while((idx >= 0) && (splitsCnt < rnSplits)){
            int32 newidx = lastElement.Locate(pattern);
            StreamString newElement;
            if(newidx >= 0){
                if(newidx > 0){
                    newElement = lastElement.SubString(0, (newidx - 1));
                }
                lastElement = lastElement.SubString(newidx + static_cast<int32>(patternL), -1);
                splitsCnt++;
            }
            else if(newidx < 0){
                newElement = lastElement;
            }
            else {
            }
            res[i] = newElement;
            i++;
            idx = newidx;
        }

        //add the rest of the string
        if(idx >= 0){
            res[i] = lastElement;
        }
    }
    
    return res;
}


StreamString StreamString::Replace(const char8 * const pattern, const char8 * const replacement, const uint32 nReplacements){
    Vector<StreamString> tokens = Split(pattern, nReplacements);
    StreamString res;
    uint32 replacements = (tokens.GetNumberOfElements() - 1u);
    for(uint32 i = 0u; i < replacements; i++){
        res += tokens[i];
        res += replacement;
    }
    res += tokens[replacements];
    return res;
}

}
