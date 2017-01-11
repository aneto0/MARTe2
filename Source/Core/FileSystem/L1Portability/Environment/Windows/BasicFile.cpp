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

BasicFile::BasicFile(const BasicFile &bf) :
        StreamI() {
    bool ok = true;
    HANDLE handle;
    if (bf.properties.handle == INVALID_HANDLE_VALUE) {
        handle = INVALID_HANDLE_VALUE;
    }
    else {
        ok = DuplicateHandle(GetCurrentProcess(),
                             bf.properties.handle,
                             GetCurrentProcess(),
                             &handle,
                             0,
                             FALSE,
                             DUPLICATE_SAME_ACCESS);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::OSError, "Error: DuplicateHandle()");
        }
    }
    if (ok) {
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
    if (this != &bf) {
        bool ok = true;
        HANDLE handle;
        if (bf.properties.handle == INVALID_HANDLE_VALUE) {
            handle = INVALID_HANDLE_VALUE;
        }
        else {
            ok = DuplicateHandle(GetCurrentProcess(),
                                 bf.properties.handle,
                                 GetCurrentProcess(),
                                 &handle,
                                 0,
                                 FALSE,
                                 DUPLICATE_SAME_ACCESS);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::OSError, "Error: DuplicateHandle()");
            }
        }
        if (ok) {
            if (properties.handle != INVALID_HANDLE_VALUE) {
                ok = CloseHandle(properties.handle);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::OSError, "Error: CloseHandle()");
                }
            }
            properties.handle = handle;
            properties.flags = bf.properties.flags;
            properties.pathname = bf.properties.pathname;
        }
    }
    return *this;
}

BasicFile::~BasicFile() {
    /*
     * Note: No exceptions can be thrown by CloseHandle (neither IsOpen),
     * but if there is an error on closing the file, it will be reported,
     * although not propagated.
     */
    if (IsOpen()) {
        bool ok = true;
        ok = CloseHandle(properties.handle);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::OSError, "Error: CloseHandle()");
        }
    }
}

bool BasicFile::SetFlags(const uint32 setFlags) {
    bool ok = true;

    if (IsOpen()) {
        //Sets the flags
        properties.flags = setFlags;
    }
    else {
        ok = false;
        REPORT_ERROR(ErrorManagement::FatalError, "Error: Precondition violated");
    }

    return ok;
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

bool BasicFile::Open(const char * pathname,
                     const uint32 flags) {
    bool ok = true;
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
        ok = (handle != INVALID_HANDLE_VALUE);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::OSError, "Error: CreateFile()");
        }

        //Sets the properties
        if (ok) {
            properties.handle = handle;
            properties.flags = CheckFlags(flags);
            properties.pathname = pathname;
        }

        //Truncates the file if needed
        if (ok) {
            if (((flags & FLAG_TRUNC) == FLAG_TRUNC) && ((flags & ACCESS_MODE_W) == ACCESS_MODE_W)) {
                SetSize(0u);
            }
        }
    }
    else {
        ok = false;
        REPORT_ERROR(ErrorManagement::FatalError, "Error: Precondition violated");
    }

    return ok;
}

bool BasicFile::IsOpen() const {
    return (properties.handle != INVALID_HANDLE_VALUE);
}

bool BasicFile::Close() {
    /*
     * Note: If the file is not opened, then
     * it is already closed, so it does nothing.
     */

    bool ok = true;

    if (IsOpen()) {

        //Closes the file
        ok = CloseHandle(properties.handle);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::OSError, "Error: CloseHandle()");
        }

        //Resets the properties
        if (ok) {
            properties.handle = INVALID_HANDLE_VALUE;
            properties.flags = 0u;
            properties.pathname = "";
        }
    }

    return ok;
}

bool BasicFile::Read(char8* const output,
                     uint32 & size) {
    bool ok = true;

    if (IsOpen() && CanRead() /* && size >= 0 */) {
        //Reads the data setting an infinite timeout
        ok = BasicFile::Read(output, size, TTInfiniteWait);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Error: Read()");
        }
    }
    else {
        ok = false;
        REPORT_ERROR(ErrorManagement::FatalError, "Error: Precondition violated");
    }

    return ok;
}

bool BasicFile::Read(char8 * const output,
                     uint32 & size,
                     const TimeoutType &msecTimeout) {
    bool ok = true;

    if (IsOpen() && CanRead() /* && size >= 0 */) {
        uint64 oldpos;
        DWORD timeout;
        OVERLAPPED ol = { 0 };

        //Caches the old position
        oldpos = Position();

        //Sets the timeout value
        if (msecTimeout == TTInfiniteWait) {
            timeout = INFINITE;
        }
        else {
            timeout = msecTimeout.GetTimeoutMSec();
        }

        //Creates a waiting event
        ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        ok = (ol.hEvent != NULL);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::OSError, "Error: CreateEvent()");
        }

        //Starts the reading operation
        if (ok) {
            bool readVal;
            readVal = ReadFile(properties.handle, (void *) output, size, NULL, &ol);
            ok = (!readVal && (GetLastError() == ERROR_IO_PENDING));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::OSError, "Error: ReadFile()");
            }
        }

        //Waits for reading completion
        if (ok) {
            DWORD waitVal;
            waitVal = WaitForSingleObject(ol.hEvent, timeout);
            ok = (waitVal == WAIT_OBJECT_0);
            if (!ok) {
                if (waitVal == WAIT_TIMEOUT) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Error: Timeout while reading");
                }
                else {
                    REPORT_ERROR(ErrorManagement::OSError, "Error: WaitForSingleObject()");
                }
            }
        }

        //Updates size with the actual bytes read
        if (ok) {
            DWORD actual;
            ok = GetOverlappedResult(properties.handle, &ol, &actual, false);
            if (ok) {
                size = actual;
            }
            else {
                REPORT_ERROR(ErrorManagement::OSError, "Error: GetOverlappedResult()");
            }
        }

        //Updates position to the last written byte
        if (ok) {
            LARGE_INTEGER newpos;
            newpos.QuadPart = oldpos + size;
            ok = SetFilePointerEx(properties.handle, newpos, NULL, FILE_CURRENT);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::OSError, "Error: SetFilePointerEx()");
            }
        }
    }
    else {
        ok = false;
        REPORT_ERROR(ErrorManagement::FatalError, "Error: Precondition violated");
    }

    return ok;
}

bool BasicFile::Write(const char8 * const input,
                      uint32 & size) {
    bool ok = true;

    if (IsOpen() && CanWrite() /* && size >= 0 */) {
        //Writes the data setting an infinite timeout
        ok = BasicFile::Write(input, size, TTInfiniteWait);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Error: Write()");
        }
    }
    else {
        ok = false;
        REPORT_ERROR(ErrorManagement::FatalError, "Error: Precondition violated");
    }

    return ok;
}

bool BasicFile::Write(const char8 * const input,
                      uint32 & size,
                      const TimeoutType &msecTimeout) {
    bool ok = true;

    if (IsOpen() && CanWrite() /* && size >= 0 */) {
        uint64 oldpos;
        DWORD timeout;
        OVERLAPPED ol = { 0 };

        //Caches the old position
        oldpos = Position();

        //Sets position to EOF if needed
        if ((properties.flags & FLAG_APPEND) == FLAG_APPEND) {
            LARGE_INTEGER zero;
            zero.QuadPart = 0;
            ok = SetFilePointerEx(properties.handle, zero, NULL, FILE_END);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::OSError, "Error: SetFilePointerEx()");
            }
        }

        //Sets the timeout value
        if (ok) {
            if (msecTimeout == TTInfiniteWait) {
                timeout = INFINITE;
            }
            else {
                timeout = msecTimeout.GetTimeoutMSec();
            }
        }

        //Creates a waiting event
        if (ok) {
            ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
            ok = (ol.hEvent != NULL);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::OSError, "Error: CreateEvent()");
            }
        }

        //Starts the reading operation
        if (ok) {
            bool writeVal;
            writeVal = WriteFile(properties.handle, input, size, NULL, &ol);
            ok = (!writeVal && (GetLastError() == ERROR_IO_PENDING));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::OSError, "Error: WriteFile()");
            }
        }

        //Waits for reading completion
        if (ok) {
            DWORD waitVal;
            waitVal = WaitForSingleObject(ol.hEvent, timeout);
            ok = (waitVal == WAIT_OBJECT_0);
            if (!ok) {
                if (waitVal == WAIT_TIMEOUT) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Error: Timeout while writing");
                }
                else {
                    REPORT_ERROR(ErrorManagement::OSError, "Error: WaitForSingleObject()");
                }
            }
        }

        //Updates size with the actual bytes written
        if (ok) {
            DWORD actual;
            ok = GetOverlappedResult(properties.handle, &ol, &actual, false);
            if (ok) {
                size = actual;
            }
            else {
                REPORT_ERROR(ErrorManagement::OSError, "Error: GetOverlappedResult()");
            }
        }

        //Updates position to the last written byte
        if (ok) {
            LARGE_INTEGER newpos;
            newpos.QuadPart = oldpos + size;
            ok = SetFilePointerEx(properties.handle, newpos, NULL, FILE_CURRENT);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::OSError, "Error: SetFilePointerEx()");
            }
        }
    }
    else {
        ok = false;
        REPORT_ERROR(ErrorManagement::FatalError, "Error: Precondition violated");
    }

    return ok;
}

uint64 BasicFile::Size() {
    uint64 value = 0u;

    //Gets the size of the file if it is opened, otherwise it is 0.
    if (IsOpen()) {
        bool ok = true;
        LARGE_INTEGER pos;
        ok = GetFileSizeEx(properties.handle, &pos);
        if (ok) {
            value = pos.QuadPart;
        }
        else {
            value = 0xFFFFFFFF;
            REPORT_ERROR(ErrorManagement::OSError, "Error: GetFileSizeEx()");
        }
    }

    return value;
}

bool BasicFile::Seek(const uint64 pos) {
    bool ok = true;

    if (IsOpen() && CanSeek()) {
        uint64 size;
        LARGE_INTEGER dist;

        //Caches the current size
        size = Size();
        ok = (size != 0xFFFFFFFF);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Error: Size()");
        }

        //Calculates the absolute position value
        if (ok) {
            if (pos <= size) {
                dist.QuadPart = pos;
            }
            else {
                dist.QuadPart = size;
            }
        }

        //Sets the file pointer to the absolute position
        if (ok) {
            ok = SetFilePointerEx(properties.handle, dist, NULL, FILE_BEGIN);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::OSError, "Error: SetFilePointerEx()");
            }
        }
    }
    else {
        ok = false;
        REPORT_ERROR(ErrorManagement::FatalError, "Error: Precondition violated");
    }

    return ok;
}

bool BasicFile::RelativeSeek(const int64 deltaPos) {
    bool ok = true;

    if (IsOpen() && CanSeek()) {
        uint64 size;
        uint64 position;
        LARGE_INTEGER dist;

        //Caches the current size
        if (ok) {
            size = Size();
            ok = (size != 0xFFFFFFFF);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Error: Size()");
            }
        }

        //Caches the current position
        if (ok) {
            position = Position();
            ok = (position != 0xFFFFFFFF);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Error: Position()");
            }
        }

        //Calculates the absolute position
        if (ok) {
            int32 abspos = position + deltaPos;
            if (abspos < 0) {
                dist.QuadPart = 0;
            }
            else if (abspos <= size) {
                dist.QuadPart = abspos;
            }
            else if (abspos > size) {
                dist.QuadPart = size;
            }
        }

        //Sets the file pointer to the absolute position
        if (ok) {
            ok = SetFilePointerEx(properties.handle, dist, NULL, FILE_BEGIN);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::OSError, "Error: SetFilePointerEx()");
            }
        }
    }
    else {
        ok = false;
        REPORT_ERROR(ErrorManagement::FatalError, "Error: Precondition violated");
    }

    return ok;
}

uint64 BasicFile::Position() {
    uint64 value = 0u;

    //Gets the position of the file if it is opened, otherwise it is 0.
    if (IsOpen()) {
        bool ok = true;
        LARGE_INTEGER zero;
        LARGE_INTEGER pos;
        zero.QuadPart = 0;
        ok = SetFilePointerEx(properties.handle, zero, &pos, FILE_CURRENT);
        if (ok) {
            value = pos.QuadPart;
        }
        else {
            value = 0xFFFFFFFF;
            REPORT_ERROR(ErrorManagement::OSError, "Error: SetFilePointerEx()");
        }
    }

    return value;
}

bool BasicFile::SetSize(uint64 size) {
    bool ok = true;

    if (IsOpen() && CanWrite()) {
        int64 oldpos = 0;

        //Caches the old position
        oldpos = Position();

        //Sets the size of the file to size
        {
            LARGE_INTEGER dist;
            dist.QuadPart = size;
            ok = SetFilePointerEx(properties.handle, dist, NULL, FILE_BEGIN);
            if (ok) {
                ok = SetEndOfFile(properties.handle);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::OSError, "Error: SetEndOfFile()");
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::OSError, "Error: SetFilePointerEx()");
            }
        }

        //Restores the position if needed
        if (ok) {
            if (oldpos < size) {
                LARGE_INTEGER olddist;
                olddist.QuadPart = oldpos;
                ok = SetFilePointerEx(properties.handle, olddist, NULL, FILE_BEGIN);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::OSError, "Error: SetFilePointerEx()");
                }
            }
        }
    }
    else {
        ok = false;
        REPORT_ERROR(ErrorManagement::FatalError, "Error: Precondition violated");
    }

    return ok;
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

