/**
 * @file IOBufferWrappers.cpp
 * @brief Header file for class AnyType
 * @date Jul 9, 2020
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#include "IOBufferWrappers.h"

namespace MARTe{


ErrorManagement::ErrorType  IOBufferWrapper::Next(){
//printf(">>\n");
    ErrorManagement::ErrorType  ret;
    ret.notCompleted = !NoMoreSpaceToWrite();
    return ret;
}

ErrorManagement::ErrorType  IOBufferWrapper::Flush(){
    return NoMoreSpaceToWrite();
}

IOBufferWrapper::IOBufferWrapper() {
    SetBufferReferencedMemory(&buffer[0],sizeof(buffer),0);
}

IOBufferWrapperStream::IOBufferWrapperStream(uint32 sizeIn): IOBufferWrapper(),size(sizeIn){
    stream = NULL;
    pointer = NULL;
}

void IOBufferWrapperStream::Wrap(void *ptr){
    pointer = reinterpret_cast<uint8 *>(ptr);
    stream = reinterpret_cast<StreamI *>(pointer);
}

ErrorManagement::ErrorType IOBufferWrapperStream::Next(){
    ErrorManagement::ErrorType  ret;

    ret.notCompleted= !NoMoreSpaceToWrite();

    Empty();

    if (ret && (size != 0)){
        pointer += size;
        stream = reinterpret_cast<StreamI *>(pointer);
    } else {
        ret.illegalOperation = true;
        REPORT_ERROR(ErrorManagement::FatalError, "size is not known. cannot handle arrays of StreamI");
    }
    // do not know how to skip to next object
    return ret;
}

bool IOBufferWrapperStream::NoMoreSpaceToWrite() {
    bool retval = false;
    if (stream != NULL) {
        // no buffering!
        if (Buffer() != NULL) {

            // how much was written?
            uint32 writeSize = UsedSize();
            if (writeSize == 0u) {
                retval = true;
            }
            // write
            else {
                if (stream->Write(Buffer(), writeSize)) {
                    retval = true;
                    Empty();
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "StreamToIOBuffer: Failed Write");
                    retval = false;
                }
            }
        }
    }
    return retval;
}

IOBufferWrapperSString::IOBufferWrapperSString():IOBufferWrapperStream(sizeof(StreamString)){
    ss = NULL_PTR(StreamString*);
}

void IOBufferWrapperSString::Wrap(void *ptr){
    ss = reinterpret_cast<StreamString *>(ptr);
    stream = ss;
}

ErrorManagement::ErrorType  IOBufferWrapperSString::Next(){
    ErrorManagement::ErrorType  ret;
    ret.notCompleted= !NoMoreSpaceToWrite();
    Empty();

    ss++;
    stream = ss;
    return ret;
}

IOBufferDynStringWrapper::IOBufferDynStringWrapper(): IOBufferWrapper(){
    string = NULL;
}

void IOBufferDynStringWrapper::Wrap(void *ptr){
    string = reinterpret_cast<DynamicCString *>(ptr);
}

ErrorManagement::ErrorType  IOBufferDynStringWrapper::Next(){
    ErrorManagement::ErrorType  ret;
    ret.notCompleted= !NoMoreSpaceToWrite();
    Empty();
    string++;
    return ret;
}

 bool IOBufferDynStringWrapper::NoMoreSpaceToWrite() {
    bool retval = false;
    if (string != NULL) {
        // no buffering!
        if (Buffer() != NULL) {

            // how much was written?
            uint32 writeSize = UsedSize();
            if (writeSize == 0u) {
                retval = true;
            }
            // write
            else {
                ErrorManagement::ErrorType ret;
                ret = (*string)().Append(Buffer(), writeSize);

                if (ret) {
                    retval = true;
                    Empty();
                }
                else {
                    REPORT_ERROR(ret, "IOBufferDynStringWrapper: Failed Write");
                }
            }
        }
    }
    return retval;
}

IOBufferCStringCompareWrapper::IOBufferCStringCompareWrapper(): IOBufferWrapper(){
    string = NULL;
    IsSame = false;
}

void IOBufferCStringCompareWrapper::Wrap(void *ptr){
    string = reinterpret_cast<CCString *>(ptr);
    currentString = string[0];
    IsSame = true;
}

ErrorManagement::ErrorType  IOBufferCStringCompareWrapper::Check(){
    ErrorManagement::ErrorType  ret;
    if (!IsSame){
        ret.comparisonFailure = true;
    }
    return ret;
}

ErrorManagement::ErrorType  IOBufferCStringCompareWrapper::Next(){
    ErrorManagement::ErrorType  ret;
    NoMoreSpaceToWrite();
    if (!IsSame){
        ret.comparisonFailure = true;
    }
    string++;
    currentString = string[0];
    return ret;
}

 bool IOBufferCStringCompareWrapper::NoMoreSpaceToWrite() {
    bool retval = false;
    if (!currentString.IsNullPtr() && IsSame) {
        // no buffering!
        if (Buffer() != NULL) {

            // how much was written?
            uint32 writeSize = UsedSize();
            if (writeSize == 0u) {
                retval = true;
            }
            // write
            else {
                if (currentString.IsSameAs(Buffer(),writeSize)){
                    retval = true;
                    Empty();
                } else {
                    DynamicCString string;
                    string().Append(Buffer(),writeSize);
                    COMPOSITE_REPORT_ERROR(ErrorManagement::ComparisonFailure,
                            "String (",string,") != (",currentString.GetList(),") ");
                    IsSame = false;
                }
            }
        }
    }
    return retval;
}


} //MARTe
