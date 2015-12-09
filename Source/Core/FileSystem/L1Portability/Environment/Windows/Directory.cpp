/**
 * @file Directory.cpp
 * @brief Source file for class Directory
 * @date Nov 16, 2015
 * @author Frank Perez Paz
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
 * the class Directory (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <windows.h>
#include <iostream>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Directory.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

Directory::Directory(const char8 * const path) :
        LinkedListable() {
    if (fname != NULL) {
        if (!HeapManager::Free(reinterpret_cast<void *&>(fname))) {
            REPORT_ERROR(ErrorManagement::OSError, "Error: Failed HeapManager::Free");
        }
    }
    fname = StringHelper::StringDup(path);
    if (path == NULL) {
        HeapManager::Free(reinterpret_cast<void *&>(fname));
    }
    HANDLE h = FindFirstFile(fname, &directoryHandle);
    if (h == INVALID_HANDLE_VALUE) {
        REPORT_ERROR(ErrorManagement::OSError, "Error: Failed INVALID_HANDLE_VALUE");
    }
    FindClose(h);
}

Directory::~Directory() {
    if (fname != NULL) {
        if (!HeapManager::Free(reinterpret_cast<void *&>(fname))) {
            REPORT_ERROR(ErrorManagement::OSError, "Error: Failed HeapManager::Free()");
        }
    }
    fname = static_cast<char8 *>(NULL);
}

bool Directory::SetByName(const char8 * const path) {
    bool ret = true;

    if (fname != NULL) {
        if (!HeapManager::Free(reinterpret_cast<void *&>(fname))) {
            ret = false;
        }
    }
    fname = StringHelper::StringDup(path);
    uint32 size = StringHelper::Length(path);
    char8* fnameTemp;
    if (path[size - 1] != '\\') {
        fnameTemp = StringHelper::StringDup(path);
    }
    else {
        fnameTemp = StringHelper::StringDup(path);
        fnameTemp[size - 1] = '\0';
    }

    if (ret) {
        HANDLE h = FindFirstFile(fnameTemp, &directoryHandle);
        if (h == INVALID_HANDLE_VALUE) {
            REPORT_ERROR(ErrorManagement::OSError, "Error: SetByName Failed INVALID_HANDLE_VALUE");
            ret = false;
        }
        FindClose(h);
    }
    HeapManager::Free(reinterpret_cast<void *&>(fnameTemp));

    return ret;
}

const char8 *Directory::GetName() const {
    return fname;
}

bool Directory::IsDirectory() const {
    bool ok = ((directoryHandle.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
    return ok;
}

bool Directory::IsFile() const {
    return (IsDirectory() == false);
}

uint64 Directory::GetSize() {
    HANDLE h = FindFirstFile(fname, &directoryHandle);
    if (h == INVALID_HANDLE_VALUE) {
        REPORT_ERROR(ErrorManagement::OSError, "Error: GetSize Failed INVALID_HANDLE_VALUE");
    }
    FindClose(h);

    uint64 size = 0L;
    size = (((uint64) directoryHandle.nFileSizeHigh) * (MAXDWORD) + 1) + directoryHandle.nFileSizeLow;
    return size;
}

TimeStamp Directory::GetLastWriteTime() {
    TimeStamp timeStamp;
    SYSTEMTIME systemTime;
    HANDLE h = FindFirstFile(fname, &directoryHandle);
    if (h == INVALID_HANDLE_VALUE) {
        REPORT_ERROR(ErrorManagement::OSError, "Error: GetLastWriteTime Failed INVALID_HANDLE_VALUE");
    }
    FindClose(h);
    bool ret = FileTimeToSystemTime(&directoryHandle.ftLastWriteTime, &systemTime);
    if (ret) {
        timeStamp.SetMicroseconds(static_cast<uint32>(systemTime.wMilliseconds));
        timeStamp.SetSeconds(static_cast<uint32>(systemTime.wSecond));
        timeStamp.SetMinutes(static_cast<uint32>(systemTime.wMinute));
        timeStamp.SetHour(static_cast<uint32>(systemTime.wHour));
        timeStamp.SetDay(static_cast<uint32>(systemTime.wDay));
        timeStamp.SetMonth(static_cast<uint32>(systemTime.wMonth));
        timeStamp.SetYear(static_cast<uint32>(systemTime.wYear));
    }
    else {
        REPORT_ERROR(ErrorManagement::OSError, "Error: localtime()");
    }
    return timeStamp;
}

TimeStamp Directory::GetLastAccessTime() {
    TimeStamp timeStamp;
    SYSTEMTIME systemTime;
    HANDLE h = FindFirstFile(fname, &directoryHandle);
    if (h == INVALID_HANDLE_VALUE) {
        REPORT_ERROR(ErrorManagement::OSError, "Error: GetLastAccessTime Failed INVALID_HANDLE_VALUE");
    }
    FindClose(h);
    bool ret = FileTimeToSystemTime(&directoryHandle.ftLastAccessTime, &systemTime);
    if (ret) {
        timeStamp.SetMicroseconds(static_cast<uint32>(systemTime.wMilliseconds));
        timeStamp.SetSeconds(static_cast<uint32>(systemTime.wSecond));
        timeStamp.SetMinutes(static_cast<uint32>(systemTime.wMinute));
        timeStamp.SetHour(static_cast<uint32>(systemTime.wHour));
        timeStamp.SetDay(static_cast<uint32>(systemTime.wDay));
        timeStamp.SetMonth(static_cast<uint32>(systemTime.wMonth));
        timeStamp.SetYear(static_cast<uint32>(systemTime.wYear));
    }
    else {
        REPORT_ERROR(ErrorManagement::OSError, "Error: FileTimeToSystemTime()");
    }
    return timeStamp;
}

bool Directory::Create(const bool isFile) {
    bool ret = (fname != NULL); //NULL_PTR(char8 *)
    if (ret) {
        if (isFile) {
            HANDLE file = CreateFile(fname, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
            if (file == INVALID_HANDLE_VALUE) {
                ret = false;
            }
            if(CloseHandle(file) == 0) {
                ret = false;
            }
        }
        else {
            if (CreateDirectory(fname,NULL) == 0) {
                ret = false;
            }
        }
    }
    if(ret) {
        HANDLE h = FindFirstFile(fname, &directoryHandle);
        if (h == INVALID_HANDLE_VALUE) {
            REPORT_ERROR(ErrorManagement::OSError, "Error: Failed FindFirstFile() in Create");
            ret = false;
        }
        FindClose(h);
    }
    return ret;
}

bool Directory::Exists() {
    bool exist = true;
    DWORD e = GetFileAttributes(fname);
    if (e == INVALID_FILE_ATTRIBUTES) {
        exist = false;
    }
    return exist;
}

bool Directory::Delete() {
    bool del = (fname != NULL);
    if (del) {
        HANDLE h = FindFirstFile(fname, &directoryHandle);
        if (h == INVALID_HANDLE_VALUE) {
            REPORT_ERROR(ErrorManagement::OSError, "Error: Failed FindFirstFile() in Delete");
            del = false;
        }
        else {
            if (IsDirectory()) {
                if (RemoveDirectory(fname) == 0) {
                    del = false;
                }
            }
            else {
                if (DeleteFile(fname) == 0) {
                    del = false;
                }
            }
        }
        if (FindClose(h) == 0) {
            del = false;
        }
    }
    return del;
}
}
