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
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "StringHelper.h"
#include "HeapManager.h"
#include "TimeStamp.h"
#include "MemoryOperationsHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

Directory::Directory(const char8 * const path) :
        LinkedListable() {
    fname = StringHelper::StringDup(path);
    if (path == NULL) {
        HeapManager::Free(reinterpret_cast<void *&>(fname));
    }
    HANDLE h = FindFirstFile(fname, &directoryHandle);
    if (h == INVALID_HANDLE_VALUE) {
        //free((void *&) fname);
        HeapManager::Free(reinterpret_cast<void *&>(directoryHandle));
    }
}

Directory::~Directory() {
    if (fname != NULL) {
        if (!HeapManager::Free(reinterpret_cast<void *&>(fname))) {
            //REPORT_ERROR(ErrorManagement::OSError, "Error: Failed HeapManager::Free()");
        }
    }
    fname = static_cast<char8 *>(NULL);
}

bool Directory::SetByName(const char8 * const path) {
    bool ret = true;

    /*if (fname != NULL) {
     if (!HeapManager::Free(reinterpret_cast<void *&>(fname))) {
     ret = false;
     }
     }*/
    if (StringHelper::Compare(fname, path) != 0) {
        fname = StringHelper::StringDup(path);
    }
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
            HeapManager::Free(reinterpret_cast<void *&>(fname));
            HeapManager::Free(reinterpret_cast<void *&>(fnameTemp));
            HeapManager::Free(reinterpret_cast<void *&>(directoryHandle));
            ret = false;
        }
    }
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
    bool ok = (IsDirectory() == false);
    return ok;
}

uint64 Directory::GetSize() {
    HANDLE h = FindFirstFile(fname, &directoryHandle);
    if (h == INVALID_HANDLE_VALUE) {
        HeapManager::Free(reinterpret_cast<void *&>(fname));
        HeapManager::Free(reinterpret_cast<void *&>(directoryHandle));
    }
    uint64 size = 0L;
    //size = directoryHandle.nFileSizeHigh;
    //size <<= 32;
    //size += directoryHandle.nFileSizeLow;
    size = (((uint64)directoryHandle.nFileSizeHigh) * (MAXDWORD) + 1) + directoryHandle.nFileSizeLow;

    return size;
}

TimeStamp Directory::GetLastWriteTime() {
    TimeStamp timeStamp;
    /*uint64 t = *((uint64 *) &directoryHandle.ftLastWriteTime);
     uint64 div = 10000000;
     uint64 t2 = t / div;
     t2 -= 0x2B61082f0;
     uint32 *p2 = (uint32 *) &t2;
     //(time_t) p2[0];
     struct tm *dt = localtime((const time_t *)p2);
     bool ret = (dt != NULL);*/
    SYSTEMTIME systemTime;
    HANDLE h = FindFirstFile(fname, &directoryHandle);
    if (h == INVALID_HANDLE_VALUE) {
        HeapManager::Free(reinterpret_cast<void *&>(fname));
        HeapManager::Free(reinterpret_cast<void *&>(directoryHandle));
    }
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
        //REPORT_ERROR(ErrorManagement::OSError, "Error: localtime()");
    }
    return timeStamp;
}

TimeStamp Directory::GetLastAccessTime() {
    TimeStamp timeStamp;
    /*uint64 t = *((uint64 *) &directoryHandle.ftLastAccessTime);
     uint64 div = 10000000;
     uint64 t2 = t / div;
     t2 -= 0x2B61082f0;
     uint32 *p2 = (uint32 *) &t2;
     struct tm *dt = localtime(&p2);
     bool ret = (dt != NULL);*/
    SYSTEMTIME systemTime;
    HANDLE h = FindFirstFile(fname, &directoryHandle);
    if (h == INVALID_HANDLE_VALUE) {
        HeapManager::Free(reinterpret_cast<void *&>(fname));
        HeapManager::Free(reinterpret_cast<void *&>(directoryHandle));
    }
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
        //REPORT_ERROR(ErrorManagement::OSError, "Error: localtime()");
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
        HANDLE h = FindFirstFile(fname, &directoryHandle);
        if (h == INVALID_HANDLE_VALUE) {
            HeapManager::Free(reinterpret_cast<void *&>(fname));
            HeapManager::Free(reinterpret_cast<void *&>(directoryHandle));
            ret = false;
            //REPORT_ERROR(ErrorManagement::OSError, "Error: Failed FindFirstFile() in initialization");
        }
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
        if(SetByName(fname)) {
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
        else {
            del = false;
        }
    }
    return del;
}
}
