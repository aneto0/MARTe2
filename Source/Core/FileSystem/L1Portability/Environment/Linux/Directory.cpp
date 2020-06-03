/**
 * @file Directory.cpp
 * @brief Source file for class Directory
 * @date 04/11/2015
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
 * the class Directory (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Directory.h"
#include "HeapManager.h"
#include "StringHelper.h"
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

    if (path != NULL) {
        fname = StringHelper::StringDup(path);

        // fill the struct with the file informations
        if (stat(path, &directoryHandle) != 0) {
            //errno = 2 => No such file or directory
            if (errno != 2) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: Failed stat() in initialization");
            }
        }
    }
    else {
        if(!MemoryOperationsHelper::Set(&directoryHandle, '\0', static_cast<uint32>(sizeof(DirectoryCore)))) {
            REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "Failed initialization of directory handle");
        }
        fname=NULL_PTR(char8*);
    }
    /*
     directoryHandle.st_dev = 0u; // ID of device containing file
     directoryHandle.st_ino = 0u; // inode number
     directoryHandle.st_mode = 0u; // protection
     directoryHandle.st_nlink = 0u; // number of hard links
     directoryHandle.st_uid = 0u; // user ID of owner
     directoryHandle.st_gid = 0u; // group ID of owner
     directoryHandle.st_rdev = 0u; // device ID (if special file)
     directoryHandle.st_size = 0; // total size, in bytes
     directoryHandle.st_blksize = 0; // blocksize for filesystem I/O
     directoryHandle.st_blocks = 0; // number of 512B blocks allocated
     directoryHandle.st_atime = 0; // time of last access
     directoryHandle.st_mtime = 0; // time of last modification
     directoryHandle.st_ctime = 0; // time of last status change
     */
}

Directory::~Directory() {
    if (fname != NULL) {
        /*lint -e{1551} .Justification: Remove the warning "Function may throw exception '...' in destructor".*/
        if (!HeapManager::Free(reinterpret_cast<void *&>(fname))) {

        }
    }
    fname = static_cast<char8 *>(NULL);
}

bool Directory::SetByName(const char8 * const path) {
    bool ret = (path != NULL);
    if (fname != NULL) {
        if (!HeapManager::Free(reinterpret_cast<void *&>(fname))) {
            ret = false;
        }
    }
    if (ret) {
        fname = StringHelper::StringDup(path);
        if (stat(path, &directoryHandle) != 0) {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: Failed stat() in initialization");
            ret = false;
        }
    }
    return ret;
}

const char8 *Directory::GetName() const {
    return fname;
}

bool Directory::IsDirectory() const {
    /*lint -e{9130} -e{9117} [MISRA C++ Rule 5-0-4] [MISRA C++ Rule 5-0-4]. Justification: Operating system APIs are not linted.*/
    return S_ISDIR(directoryHandle.st_mode);
}

bool Directory::IsFile() const {
    /*lint -e{9130} -e{9117} [MISRA C++ Rule 5-0-4] [MISRA C++ Rule 5-0-4]. Justification: Operating system APIs are not linted.*/
    return S_ISREG(directoryHandle.st_mode);
}

uint64 Directory::GetSize() {
    uint64 size = 0u;
    if (stat(GetName(), &directoryHandle) == 0) {
        size = static_cast<uint64>(directoryHandle.st_size);
    }
    return size;
}

TimeStamp Directory::GetLastWriteTime() {
    TimeStamp timeStamp;

    if (stat(GetName(), &directoryHandle) == 0) {
        time_t secondsFromEpoch32 = static_cast<time_t>(directoryHandle.st_mtime);
        if (!timeStamp.ConvertFromEpoch(secondsFromEpoch32)) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Error: Failed TimeStamp::ConvertFromEpoch");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: stat()");
    }

    return timeStamp;
}

TimeStamp Directory::GetLastAccessTime() {
    TimeStamp timeStamp;
    if (stat(GetName(), &directoryHandle) == 0) {
        time_t secondsFromEpoch32 = static_cast<int32>(directoryHandle.st_atime);
        if (!timeStamp.ConvertFromEpoch(secondsFromEpoch32)) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Error: Failed TimeStamp::ConvertFromEpoch");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: stat()");
    }

    return timeStamp;
}

bool Directory::Create(const bool isFile) {
    bool ret = (fname != NULL_PTR(char8 *));

    if (ret) {
        if (isFile) {
            /*lint -e{9130} -e{9117} [MISRA C++ Rule 5-0-21]  [MISRA C++ Rule 5-0-4]. Justification: Operating system APIs are not linted.*/
            int32 fd = open(fname, static_cast<mode_t>(00777 | O_EXCL | O_CREAT | O_WRONLY | O_TRUNC), (S_IRWXU | S_IRWXG | S_IRWXO));
            if (fd < 0) {
                ret = false;
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: Failed creat()");
            }
            else {
                if (close(fd) < 0) {
                    ret = false;
                    REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: Failed close()");
                }
            }
        }
        else {
            ret = (mkdir(fname, static_cast<mode_t>(0777)) == 0);
        }

        if (stat(fname, &directoryHandle) != 0) {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: Failed stat() in initialization");
        }
    }
    return ret;
}

/*lint -e{9130} -e{9117} -e{1762} [MISRA C++ Rule 5-0-4] [MISRA C++ Rule 5-0-4] [MISRA C++ Rule 9-3-3].
 * Justification: Operating system APIs are not linted. In other operating systems Exists() might not be constant.*/
bool Directory::Exists() {
    struct stat fileStats;
    bool ok = (stat(fname, &fileStats) == 0);
    bool isDir = S_ISDIR(fileStats.st_mode);
    bool isFile = S_ISREG(fileStats.st_mode);
    return (ok) ? (isDir || isFile) : (false);
}

/*lint -e{1762} [MISRA C++ Rule 9-3-3] in other operating systems Delete() might not be constant*/
bool Directory::Delete() {
    bool ok = (fname != NULL_PTR(char8 *));
    if (ok) {
        /*lint -e{668} fname cannot be NULL (checked in line above)*/
        ok = (remove(fname) == 0);
    }
    return ok;
}

}

