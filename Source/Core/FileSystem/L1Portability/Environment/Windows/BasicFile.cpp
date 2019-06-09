/**
 * @file BasicFile.cpp
 * @brief Source file for class BasicFile
 * @date 26/10/2015
 * @author Ivan Herrero�
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

#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <windows.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicFile.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static MARTe::uint32 CheckFlags(MARTe::uint32 flags) {

    //If APPEND and Read mode are set at the same time the APPEND mode is deleted from the flags
    if ((flags & (MARTe::BasicFile::FLAG_APPEND | MARTe::BasicFile::ACCESS_MODE_R)) == (MARTe::BasicFile::FLAG_APPEND | MARTe::BasicFile::ACCESS_MODE_R)) {
        if ((flags & MARTe::BasicFile::ACCESS_MODE_W) != (MARTe::BasicFile::ACCESS_MODE_W)) {
            MARTe::uint32 clean_APPEND = ~MARTe::BasicFile::FLAG_APPEND;
            flags &= clean_APPEND;
        }
    }

    if ((flags & (MARTe::BasicFile::FLAG_CREAT | MARTe::BasicFile::FLAG_CREAT_EXCLUSIVE))
            == (MARTe::BasicFile::FLAG_CREAT | MARTe::BasicFile::FLAG_CREAT_EXCLUSIVE)) {
        MARTe::uint32 clean_CREAT_EXCLUSIVE = ~MARTe::BasicFile::FLAG_CREAT_EXCLUSIVE;
        flags &= clean_CREAT_EXCLUSIVE;
    }
    if ((flags & (MARTe::BasicFile::FLAG_TRUNC | MARTe::BasicFile::ACCESS_MODE_R)) == (MARTe::BasicFile::FLAG_TRUNC | MARTe::BasicFile::ACCESS_MODE_R)) {
        if ((flags & MARTe::BasicFile::ACCESS_MODE_W) != (MARTe::BasicFile::ACCESS_MODE_W)) {
            MARTe::uint32 clean_TRUNC = ~MARTe::BasicFile::FLAG_TRUNC;
            flags &= clean_TRUNC;
        }
    }
    if ((flags & (MARTe::BasicFile::FLAG_TRUNC | MARTe::BasicFile::FLAG_CREAT_EXCLUSIVE))
            == (MARTe::BasicFile::FLAG_TRUNC | MARTe::BasicFile::FLAG_CREAT_EXCLUSIVE)) {
        MARTe::uint32 clean_TRUNC = ~MARTe::BasicFile::FLAG_TRUNC;
        flags &= clean_TRUNC;
    }
    return flags;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

BasicFile::BasicFile() :
        StreamI() {
    properties.handle = INVALID_HANDLE_VALUE;
    properties.flags = 0u;
    properties.pathname = "";
}

BasicFile::BasicFile(const BasicFile &bf) : StreamI() {
	ErrorManagement::ErrorType ret;
    HANDLE handle;
    if (bf.properties.handle == INVALID_HANDLE_VALUE) {
        handle = INVALID_HANDLE_VALUE;
    }  else {
        ret.OSError = (DuplicateHandle(GetCurrentProcess(),
                             bf.properties.handle,
                             GetCurrentProcess(),
                             &handle,
                             0,
                             FALSE,
                             DUPLICATE_SAME_ACCESS)==0);
        REPORT_ERROR(ret, "Error: DuplicateHandle()");
    }
    if (ret) {
        properties.handle = handle;
        properties.flags = bf.properties.flags;
        properties.pathname = bf.properties.pathname;
    }
    else {
        properties.handle = INVALID_HANDLE_VALUE;
        properties.flags = 0u;
        properties.pathname = "";
    }
}

BasicFile& BasicFile::operator=(const BasicFile &bf) {
	ErrorManagement::ErrorType ret;

	if (this != &bf) {
        HANDLE handle;
        if (bf.properties.handle == INVALID_HANDLE_VALUE) {
            handle = INVALID_HANDLE_VALUE;
        } else {
            ret.OSError = (DuplicateHandle(GetCurrentProcess(),
                                 bf.properties.handle,
                                 GetCurrentProcess(),
                                 &handle,
                                 0,
                                 FALSE,
                                 DUPLICATE_SAME_ACCESS)==0);
            REPORT_ERROR(ret, "Error: DuplicateHandle()");
        }
        if (ret) {
            if (properties.handle != INVALID_HANDLE_VALUE) {
                ret.OSError = (CloseHandle(properties.handle)==0);
                REPORT_ERROR(ret, "Error: CloseHandle()");
            }
            properties.handle = handle;
            properties.flags = bf.properties.flags;
            properties.pathname = bf.properties.pathname;
        }
    }
    return *this;
}

BasicFile::~BasicFile() {
	ErrorManagement::ErrorType ret;
    /*
     * Note: No exceptions can be thrown by CloseHandle (neither IsOpen),
     * but if there is an error on closing the file, it will be reported,
     * although not propagated.
     */
    if (IsOpen()) {
        ret.OSError = (CloseHandle(properties.handle)==0);
        REPORT_ERROR(ret, "Error: CloseHandle()");
    }
}

bool BasicFile::SetFlags(const uint32 setFlags) {
	ErrorManagement::ErrorType ret;

    if (IsOpen()) {
        //Sets the flags
        properties.flags = setFlags;
    } else {
    	ret.illegalOperation = true;
        REPORT_ERROR(ret, "Error: SetFlags on file not opened");
    }

    return ret;
}

uint32 BasicFile::GetFlags() const {
    return properties.flags;
}

bool BasicFile::CanWrite() const {
    return ((properties.handle != INVALID_HANDLE_VALUE) && (((properties.flags & ACCESS_MODE_W) == ACCESS_MODE_W)));
}

bool BasicFile::CanRead() const {
    return ((properties.handle != INVALID_HANDLE_VALUE) && (((properties.flags & ACCESS_MODE_R) == ACCESS_MODE_R)));
}

bool BasicFile::CanSeek() const {
    return (properties.handle != INVALID_HANDLE_VALUE);
}

bool BasicFile::Open(const char * pathname,const uint32 flags) {
	ErrorManagement::ErrorType ret;
    HANDLE handle;

    if (!IsOpen()) {
        uint32 desiredAccess = 0u;
        uint32 creationDisposition = 0u;

        //Sets the desired access
        if ((flags & ACCESS_MODE_R) == ACCESS_MODE_R) {
            desiredAccess |= GENERIC_READ;
        }
        if ((flags & ACCESS_MODE_W) == ACCESS_MODE_W) {
            desiredAccess |= GENERIC_WRITE;
        }

        //Sets the creation disposition
        if ((flags & FLAG_CREAT) == FLAG_CREAT) {
            creationDisposition = OPEN_ALWAYS;
        }
        else if ((flags & FLAG_CREAT_EXCLUSIVE) == FLAG_CREAT_EXCLUSIVE) {
            creationDisposition = CREATE_NEW;
        }
        else {
            creationDisposition = OPEN_EXISTING;
        }

        //Opens the file by pathname with the right flags
        handle = CreateFile(pathname, desiredAccess, FILE_SHARE_READ, NULL, creationDisposition, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
        ret.OSError = (handle == INVALID_HANDLE_VALUE);
        REPORT_ERROR(ret, "Error: CreateFile()");

        //Sets the properties
        if (ret) {
            properties.handle = handle;
            properties.flags = CheckFlags(flags);
            properties.pathname = pathname;
        }

        //Truncates the file if needed
        if (ret) {
            if (((flags & FLAG_TRUNC) == FLAG_TRUNC) && ((flags & ACCESS_MODE_W) == ACCESS_MODE_W)) {
                SetSize(0u);
            }
        }
    } else {
        ret.illegalOperation=true;
        REPORT_ERROR(ret, "Error: Trying to Open a file already Opened");
    }

    return ret;
}

bool BasicFile::IsOpen() const {
    return (properties.handle != INVALID_HANDLE_VALUE);
}

bool BasicFile::Close() {
	ErrorManagement::ErrorType ret;
    /*
     * Note: If the file is not opened, then
     * it is already closed, so it does nothing.
     */
    if (IsOpen()) {

        //Closes the file
        ret.OSError = (CloseHandle(properties.handle)==0);
        REPORT_ERROR(ret, "Error: CloseHandle()");

        //Resets the properties
        if (ret) {
            properties.handle = INVALID_HANDLE_VALUE;
            properties.flags = 0u;
            properties.pathname = "";
        }
    }

    return ret;
}

bool BasicFile::Read(char8* const output,uint32 & size) {
	ErrorManagement::ErrorType ret;

    if (IsOpen() && CanRead() /* && size >= 0 */) {
        //Reads the data setting an infinite timeout
        ret.fatalError = !BasicFile::Read(output, size, MilliSeconds::Infinite);
        REPORT_ERROR(ret, "Error: Read()");
    } else {
        ret.invalidOperation = true;
        REPORT_ERROR(ret, "Error: Read on a Closed File or a Write only file");
    }

    return ret;
}

bool BasicFile::Read(char8 * const output,
                     uint32 & size,
                     const MilliSeconds &msecTimeout) {
	ErrorManagement::ErrorType ret;

	ret.invalidOperation = (!IsOpen() || !CanRead());
    REPORT_ERROR(ret, "Read from Close File or from writeonly file");

    OVERLAPPED ol = { 0 };
    uint64 oldPos = 0U;
    DWORD timeout = INFINITE;
    if (ret) {

    	//Caches the old position
        oldPos = Position();

        //Sets the timeout value
        if (msecTimeout.IsValid() ) {
            timeout = msecTimeout.GetTimeRaw();
        }

        //Creates a waiting event
        ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        ret.OSError = (ol.hEvent == NULL);
        REPORT_ERROR(ret, "Error: CreateEvent()");

    }

    //Starts the reading operation
    if (ret) {
        bool failed = (ReadFile(properties.handle, (void *) output, size, NULL, &ol)==0);
        ret.OSError = !(failed && (GetLastError() == ERROR_IO_PENDING));
        REPORT_ERROR(ret, "ReadFile !failed or last error !ERROR_IO_PENDING");
    }

    //Waits for reading completion
    if (ret) {
        DWORD waitVal;
        waitVal = WaitForSingleObject(ol.hEvent, timeout);
        ret.OSError    = (waitVal != WAIT_OBJECT_0) && (waitVal != WAIT_TIMEOUT);
        REPORT_ERROR(ret, "Error: WaitForSingleObject()");
        if (ret){
            ret.warning = (waitVal == WAIT_TIMEOUT);
            REPORT_ERROR(ret, "Error: Timeout while reading");
        }
    }

    //Updates size with the actual bytes read
    if (ret) {
        DWORD actual;
        ret.OSError = (GetOverlappedResult(properties.handle, &ol, &actual, false)==0);
        REPORT_ERROR(ret, "Error: GetOverlappedResult()");
        if (ret) {
            size = actual;
        }
    }

    //Updates position to the last written byte
    if (ret) {
        uint64 newPos = oldPos + size;

        LARGE_INTEGER newPosLI;
        ret.outOfRange = SafeNumber2Number(newPos,newPosLI.QuadPart);
        REPORT_ERROR(ret, "Error: file position > int64");
//            newpos.QuadPart = oldPos + size;

        if (ret) {
        	ret.OSError = (SetFilePointerEx(properties.handle, newPosLI, NULL, FILE_CURRENT)==0);
            REPORT_ERROR(ret, "Error: SetFilePointerEx()");
        }
    }

    return ret;
}

bool BasicFile::Write(const char8 * const input,
                      uint32 & size) {
	ErrorManagement::ErrorType ret;

    //Writes the data setting an infinite timeout
    ret.fatalError = (BasicFile::Write(input, size, MilliSeconds::Infinite)==0);
    REPORT_ERROR(ret, "Error: Write()");

    return ret;
}

bool BasicFile::Write(const char8 * const input,
                      uint32 & size,
                      const MilliSeconds &msecTimeout) {
	ErrorManagement::ErrorType ret;

	ret.invalidOperation = (!IsOpen() || !CanWrite());
    REPORT_ERROR(ret, "Write to a closed or ReadOnly file");

    uint64 oldPos = 0;
    OVERLAPPED ol = { 0 };
    DWORD timeout = INFINITE;
    if (ret) {

        //Caches the old position
        oldPos = Position();

        //Sets the timeout value
        if (msecTimeout.IsValid() ) {
            timeout = msecTimeout.GetTimeRaw();
        }

        //Sets position to EOF if needed
        if ((properties.flags & FLAG_APPEND) == FLAG_APPEND) {
            LARGE_INTEGER zero;
            zero.QuadPart = 0;
            ret.OSError = (SetFilePointerEx(properties.handle, zero, NULL, FILE_END)==0);
            REPORT_ERROR(ret, "Error: SetFilePointerEx()");
        }
    }

    //Creates a waiting event
    if (ret) {
        ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        ret.OSError = (ol.hEvent == NULL);
        REPORT_ERROR(ret, "Error: CreateEvent()");
    }

    //Starts the write operation
    if (ret) {
        bool failed;
        failed = (WriteFile(properties.handle, input, size, NULL, &ol)==0);
        ret.OSError = !(failed && (GetLastError() == ERROR_IO_PENDING));
        REPORT_ERROR(ret, "Error: WriteFile()");
    }

    //Waits for write completion
    if (ret) {
        DWORD waitVal;
        waitVal = WaitForSingleObject(ol.hEvent, timeout);
        ret.OSError    = (waitVal != WAIT_OBJECT_0) && (waitVal != WAIT_TIMEOUT);
        REPORT_ERROR(ret, "Error: WaitForSingleObject()");
        if (ret){
            ret.warning = (waitVal == WAIT_TIMEOUT);
            REPORT_ERROR(ret, "Error: Timeout while writing");
        }
    }

    //Updates size with the actual bytes written
    if (ret) {
        DWORD actual;
        ret.OSError = (GetOverlappedResult(properties.handle, &ol, &actual, false)==0);
        REPORT_ERROR(ret, "Error: GetOverlappedResult()");
        if (ret) {
            size = actual;
        }
    }

    //Updates position to the last written byte
    if (ret) {
        uint64 newPos = oldPos + size;

        LARGE_INTEGER newPosLI;
        ret.outOfRange = SafeNumber2Number(newPos,newPosLI.QuadPart);
        REPORT_ERROR(ret, "Error: file position > int64");

        if (ret) {
        	ret.OSError = (SetFilePointerEx(properties.handle, newPosLI, NULL, FILE_CURRENT)==0);
            REPORT_ERROR(ret, "Error: SetFilePointerEx()");
        }
    }

    return ret;
}

uint64 BasicFile::Size() const{

	uint64 value = 0u;

    //Gets the size of the file if it is opened, otherwise it is 0.
    if (IsOpen()) {
        value = StreamI::Invalid;
    	ErrorManagement::ErrorType ret;

    	LARGE_INTEGER pos;
        ret.OSError = (GetFileSizeEx(properties.handle, &pos)==0);
        REPORT_ERROR(ret, "Error: GetFileSizeEx()");
        if (ret) {
            SafeNumber2Number(pos.QuadPart,value);
        }
    }

    return value;
}

bool BasicFile::Seek(uint64 pos) {
	ErrorManagement::ErrorType ret;

	ret.invalidOperation = (!IsOpen() || !CanSeek());
    REPORT_ERROR(ret, "Error: Seek on a closed file or on a not seekable file");

    uint64 size = 0;
    if (ret) {
        //Caches the current size
        size = Size();
        ret.fatalError = (size == StreamI::Invalid);
        REPORT_ERROR(ret, "Error: Size()");
    }

    //Calculates the absolute position value
    if (ret) {
        LARGE_INTEGER dist;
        if (pos <= size) {
        	/** always correct as pos is checked against actual file size which is limited to int64 range */
            dist.QuadPart = static_cast<int64>(pos);
        }
        else {
        	/** always correct as pos is checked against actual file size which is limited to int64 range */
            dist.QuadPart = static_cast<int64>(size);
        }

        //Sets the file pointer to the absolute position
        ret.OSError = (SetFilePointerEx(properties.handle, dist, NULL, FILE_BEGIN)==0);
        REPORT_ERROR(ret, "Error: SetFilePointerEx()");
    }

    return ret;
}

bool BasicFile::RelativeSeek(const int64 deltaPos) {
	ErrorManagement::ErrorType ret;

	ret.invalidOperation = (!IsOpen() || !CanSeek());
    REPORT_ERROR(ret, "Cannot RelativeSeek on a closed file or one without seek ability");

    uint64 size = 0;
    uint64 position = 0;
    if (ret) {
        size = Size();
        position = Position();

    	ret.fatalError = ((size == StreamI::Invalid) || (position == StreamI::Invalid));
        REPORT_ERROR(ErrorManagement::FatalError, "Error calling Size() and/or Position()");
    }

    //Calculates the absolute position
    if (ret) {
        LARGE_INTEGER dist;

        SaturatedInteger<uint64> newPos;
        if (deltaPos > 0){
        	newPos += SaturatedInteger<uint64> (deltaPos);
        } else {
        	newPos -= SaturatedInteger<uint64> (-deltaPos);
        }

        if (newPos.IsPositiveInf()){
            dist.QuadPart = static_cast<int64>(size-1U);
            REPORT_ERROR(ErrorManagement::Warning, "Error: Seek beyond end of file (saturated)");
        } else
        if (newPos.IsValid()){
            dist.QuadPart = static_cast<int64>(newPos.GetData());
        } else
        {
            dist.QuadPart = 0;
            REPORT_ERROR(ErrorManagement::Warning, "Error: Seek beyond start of file (saturated)");
        }

        ret.OSError = (SetFilePointerEx(properties.handle, dist, NULL, FILE_BEGIN)==0);
        REPORT_ERROR(ret, "Error: SetFilePointerEx()");
    }

    return ret;
}

uint64 BasicFile::Position() const{
	ErrorManagement::ErrorType ret;
    uint64 value = 0u;

    //Gets the position of the file if it is opened, otherwise it is 0.
    if (IsOpen()) {
        value = StreamI::Invalid;
        LARGE_INTEGER zero;
        LARGE_INTEGER pos;
        zero.QuadPart = 0;
        ret.OSError = (SetFilePointerEx(properties.handle, zero, &pos, FILE_CURRENT)==0);
        REPORT_ERROR(ret, "Error: SetFilePointerEx()");
        if (ret) {
        	// valid as
            value = static_cast<uint64>(pos.QuadPart);
        }
    }

    return value;
}

bool BasicFile::SetSize(uint64 size) {
	ErrorManagement::ErrorType ret;

	ret.illegalOperation = (!IsOpen() && !CanWrite());
    REPORT_ERROR(ret, "SetSize closed or read only file ");

    uint64 oldPos = 0U;
    if (ret) {
    	oldPos =  Position();
        ret.fatalError = (oldPos == StreamI::Invalid);
        REPORT_ERROR(ret, "SetSize Position() call failed");
    }

    //Sets the size of the file to size
    LARGE_INTEGER dist= {0};
    if (ret){
        ret.outOfRange = !SafeNumber2Number(size,dist.QuadPart);
        REPORT_ERROR(ret, "SetSize size is too large > int64");
    }

    if (ret) {
        ret.OSError = (SetFilePointerEx(properties.handle, dist, NULL, FILE_BEGIN)==0);
        REPORT_ERROR(ret, "Error: SetFilePointerEx()");
    }

    if (ret) {
    	ret.OSError = (SetEndOfFile(properties.handle)==0);
    	REPORT_ERROR(ret, "Error: SetEndOfFile()");
    }

    if (ret && (oldPos < size)) {
        LARGE_INTEGER olddist;
        olddist.QuadPart = static_cast<int32>(oldPos);
        ret.OSError = (SetFilePointerEx(properties.handle, olddist, NULL, FILE_BEGIN)==0);
        REPORT_ERROR(ret, "Error: SetFilePointerEx()");
    }

    return ret;
}

StreamString BasicFile::GetPathName() const {
    return properties.pathname;
}

Handle BasicFile::GetReadHandle() const {
    return properties.handle;
}

Handle BasicFile::GetWriteHandle() const {
    return properties.handle;
}
}

