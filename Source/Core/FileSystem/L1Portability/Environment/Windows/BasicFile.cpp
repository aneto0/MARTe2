/**
 * @file BasicFile.cpp
 * @brief Source file for class BasicFile
 * @date 26/10/2015
 * @author Ivan Herrero 
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
 * the class BasicFile (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

//#include <windows.h>
//#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicFile.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

BasicFile::BasicFile() :
        handle_(INVALID_HANDLE_VALUE) {

}

bool BasicFile::CanWrite() const {
    return ((handle_ != INVALID_HANDLE_VALUE) && (mode_ == ACCESS_MODE_W || mode_ == ACCESS_MODE_R_W));
}

bool BasicFile::CanRead() const {
    return ((handle_ != INVALID_HANDLE_VALUE) && (mode_ == ACCESS_MODE_R || mode_ == ACCESS_MODE_R_W));
}

bool BasicFile::CanSeek() const {
    return (handle_ != INVALID_HANDLE_VALUE /* && ... */);
}

bool BasicFile::Open(const char * pathname,
                     const uint32 mode) {
    bool retVal;

    uint32 desiredAccess = 0;
    uint32 creationDistribution  = 0;

    //Sets the desired mode
    switch (mode) {
    case ACCESS_MODE_R:
        desiredAccess |= GENERIC_READ;
        break;
    case ACCESS_MODE_W:
        desiredAccess |= GENERIC_WRITE;
        break;
    case ACCESS_MODE_R_W:
        desiredAccess |= (GENERIC_READ | GENERIC_WRITE);
        break;
    default:
        retVal = false;
    }

    //Sets the creation distribution
    switch (mode) {
    case FLAG_APPEND:
        creationDistribution |= OPEN_EXISTING;
        break;
    case FLAG_CREAT:
        creationDistribution |= OPEN_ALWAYS;
        break;
    case FLAG_TRUNC:
        creationDistribution |= TRUNCATE_EXISTING;
        break;
    default:
        retVal = false;
    }

    if (retVal) {
        handle_ = CreateFile(pathname, desiredAccess, FILE_SHARE_READ, NULL, creationDistribution, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
        retVal = (handle_ == INVALID_HANDLE_VALUE);
    }

    if (retVal) {
        mode_ = mode;
        if (mode == FLAG_APPEND) {
            //Sets the absolute position to the end of the file
            int64 newpos;
            newpos = SetFilePointer(handle_, 0, NULL, FILE_END);
            retVal = (newpos != INVALID_SET_FILE_POINTER);
        }
    }

    return retVal;
}

bool BasicFile::IsOpen() {
    return (handle_ != INVALID_HANDLE_VALUE);
}

const char* BasicFile::GetPathName() {
    static char pathname[MAX_PATH];
    GetFinalPathNameByHandle(handle_, pathname, MAX_PATH, VOLUME_NAME_NT);
    //TODO: Decide if it is ok GetFinalPathNameByHandle using , because it is
    //not available on versions previous to Windows Vista and Windows Server 2008.
    return pathname;
}

uint32 BasicFile::GetMode() {
    return mode_;
}

bool BasicFile::Close() {
    //TODO: assert IsOpen()
    bool retVal = CloseHandle(handle_);
    handle_ = INVALID_HANDLE_VALUE;
    return false;
}

bool BasicFile::Read(char8* const output,
                     uint32 & size) {
    //TODO: assert IsOpen()
    bool retVal;
    DWORD actual;

    //Reads size bytes into output
    retVal = ReadFile(handle_, (void *) output, size, &actual, NULL);

    //Updates size to the actual bytes read
    if (retVal) {
        size = actual;
    }

    return retVal;
}

bool BasicFile::Read(char8 * const output,
                     uint32 & size,
                     const TimeoutType &msecTimeout) {
    //TODO: assert IsOpen()

    //TODO: For setting a timeout, the FILE_FLAG_OVERLAPPED flag must be set
    //on open and ...

    bool retVal;
//    fd_set set;
//    struct timeval timeout;
//    int32 retSelect;
//    FD_ZERO(&set);
//    FD_SET(hFile, &set);
//    timeout.tv_usec = msecTimeout.GetTimeoutMSec() * 1000;
//    retSelect = select(hFile + 1, &set, NULL, NULL, &timeout);
//    if (retSelect == -1) {
//        REPORT_ERROR(ErrorManagement::FatalError, "BasicFile:error while waiting to read a file");
//        retVal = false;
//    }
//    else if (retSelect == 0) {
//        REPORT_ERROR(ErrorManagement::Timeout, "BasicFile:Timeout file not read");
//        retVal = false;
//    }
//    else {
//        retVal = BasicFile::Read(output, size);
//    }
    return retVal;
}

bool BasicFile::Write(const char8 * const input,
                      uint32 & size) {
    //TODO: assert IsOpen()
    bool retVal;
    DWORD actual;

    //Reads size bytes from input
    retVal = WriteFile(handle_, input, size, &actual, NULL);

    //Updates size to the actual bytes written
    if (retVal) {
        size = actual;
    }

    return retVal;
}

bool BasicFile::Write(const char8 * const input,
                      uint32 & size,
                      const TimeoutType &msecTimeout) {
    //TODO: assert IsOpen()
    bool retVal;
//    fd_set set;
//    struct timeval timeout;
//    int32 retSelect;
//    FD_ZERO(&set);
//    FD_SET(hFile, &set);
//    timeout.tv_usec = msecTimeout.GetTimeoutMSec() * 1000;
//    retSelect = select(hFile + 1, NULL, &set, NULL, &timeout);
//    if (retSelect == -1) {
//        REPORT_ERROR(ErrorManagement::FatalError, "BasicFile:error while waiting to write a file");
//        retVal = false;
//    }
//    else if (retSelect == 0) {
//        REPORT_ERROR(ErrorManagement::Timeout, "BasicFile:Timeout file not written");
//        retVal = false;
//    }
//    else {
//        retVal = BasicFile::Write(input, size);
//    }
    return retVal;
}

uint64 BasicFile::Size() {
    //TODO: assert IsOpen()
    uint64 numberOfCharactersFile = -1;

    //Gets the size of the file
    numberOfCharactersFile = GetFileSize(handle_, NULL);
    //TODO: Check for INVALID_FILE_SIZE ??

    return numberOfCharactersFile;
}

bool BasicFile::Seek(uint64 pos) {
    //TODO: assert IsOpen()
    bool retVal = true;
    int64 newpos;

    //Sets the absolute position to pos
    newpos = SetFilePointer(handle_, pos, NULL, FILE_BEGIN);
    retVal = (newpos != INVALID_SET_FILE_POINTER);

    return retVal;
}

bool BasicFile::RelativeSeek(const int32 deltaPos) {
    //TODO: assert IsOpen()
//    int64 retSeek;
    bool retVal = true;
    int64 newpos;

    //Sets the absolute position to pos
    newpos = SetFilePointer(handle_, deltaPos, NULL, FILE_CURRENT);
    retVal = (newpos != INVALID_SET_FILE_POINTER);

    return retVal;
}

uint64 BasicFile::Position() {
    //TODO: assert IsOpen()
    return SetFilePointer(handle_, 0, NULL,FILE_CURRENT);
}

bool BasicFile::SetSize(uint64 size) {
    //TODO: assert IsOpen() && CanWrite && ¿CanSeek()?

    bool retVal = false;
    int64 oldpos = 0;

    //Caches the old position
    oldpos = Position();  //TODO: Check if int64 or uint64 ... 32 or 64

    //Sets the size of the file to size
    {
        int64 newpos = SetFilePointer(handle_, size, NULL,FILE_BEGIN);
        retVal = (newpos != INVALID_SET_FILE_POINTER);
        if (retVal) {
            retVal = SetEndOfFile(handle_);
        }
    }

    //Restores the position if needed
    if (retVal) {
        if (oldpos < size) {
            int64 newpos = SetFilePointer(handle_, oldpos, NULL,FILE_BEGIN);
            retVal = (newpos != INVALID_SET_FILE_POINTER);
        }
    }

    return retVal;
}

}

