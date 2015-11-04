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

    fname = StringHelper::StringDup(path);

    // fill the struct with the file informations
    if (stat(path, &directoryHandle) != 0) {
        REPORT_ERROR(ErrorManagement::OSError, "Error: Failed stat() in initialization");
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

/** */
Directory::~Directory() {
    if (fname != NULL) {
        /*lint -e{1551} .Justification: Remove the warning "Function may throw exception '...' in destructor".*/
        if (!HeapManager::Free(reinterpret_cast<void *&>(fname))) {

        }
    }
    fname = static_cast<char8 *>(NULL);
}

bool Directory::SetByName(const char8 * const path) {
    bool ret = true;

    if (fname != NULL) {
        if (!HeapManager::Free(reinterpret_cast<void *&>(fname))) {
            ret=false;
        }
    }
    if (ret) {
        fname = StringHelper::StringDup(path);
        if (stat(path, &directoryHandle) != 0) {
            REPORT_ERROR(ErrorManagement::OSError, "Error: Failed stat() in initialization");
            ret = false;
        }
    }
    return ret;
}

const char8 *Directory::Name() const {
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

bool Directory::ReadOnly() const {
    return false;
}

uint64 Directory::Size() const {
    return (directoryHandle.st_size > 0) ? (static_cast<uint64>(directoryHandle.st_size)) : (0u);
}

int32 Directory::LastWriteTime() const {
    return static_cast<int32>(directoryHandle.st_mtime);
}

int32 Directory::LastAccessTime() const {
    return static_cast<int32>(directoryHandle.st_atime);
}

bool Directory::Create(const char8 * const path,
                       const bool isFile) {
    return (isFile) ? (creat(path, static_cast<mode_t>(0777)) > 0) : (mkdir(path, static_cast<mode_t>(0777)) == 0);
}

bool Directory::Exists(const char8 * const path) {
    struct stat fileStats;
    /*lint -e{9130} -e{9117} [MISRA C++ Rule 5-0-4] [MISRA C++ Rule 5-0-4]. Justification: Operating system APIs are not linted.*/
    return (stat(path, &fileStats) != 0)?(S_ISDIR(fileStats.st_mode) || S_ISREG(fileStats.st_mode)):(false);
}

bool Directory::Delete(const char8 * const path) {
    return (remove(path) == 0);
}

}
