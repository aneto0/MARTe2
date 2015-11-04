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
    dyrectoryHandle.st_dev = 0u; /* ID of device containing file */
    dyrectoryHandle.st_ino = 0u; /* inode number */
    dyrectoryHandle.st_mode = 0u; /* protection */
    dyrectoryHandle.st_nlink = 0u; /* number of hard links */
    dyrectoryHandle.st_uid = 0u; /* user ID of owner */
    dyrectoryHandle.st_gid = 0u; /* group ID of owner */
    dyrectoryHandle.st_rdev = 0u; /* device ID (if special file) */
    dyrectoryHandle.st_size = 0; /* total size, in bytes */
    dyrectoryHandle.st_blksize = 0; /* blocksize for filesystem I/O */
    dyrectoryHandle.st_blocks = 0; /* number of 512B blocks allocated */
    dyrectoryHandle.st_atime = 0; /* time of last access */
    dyrectoryHandle.st_mtime = 0; /* time of last modification */
    dyrectoryHandle.st_ctime = 0; /* time of last status change */

}

/** */
Directory::~Directory() {
    if (fname != NULL) {
        /*lint -e{1551} .Justification: Remove the warning "Function may throw exception '...' in destructor".*/
        if(!HeapManager::Free(reinterpret_cast<void *&>(fname))) {

        }
    }
    fname =static_cast<char8 *>(NULL);
}

const char8 *Directory::Name() const {
    return fname;
}

bool Directory::IsDirectory() const {
    /*lint -e{9130} -e{9117} [MISRA C++ Rule 5-0-4] [MISRA C++ Rule 5-0-4]. Justification: Operating system APIs are not linted.*/
    return S_ISDIR(dyrectoryHandle.st_mode);
}

bool Directory::IsFile() const {
    /*lint -e{9130} -e{9117} [MISRA C++ Rule 5-0-4] [MISRA C++ Rule 5-0-4]. Justification: Operating system APIs are not linted.*/
    return S_ISREG(dyrectoryHandle.st_mode);
}

bool Directory::ReadOnly() const {
    return false;
}

uint64 Directory::Size() const {
    return (dyrectoryHandle.st_size > 0) ? (static_cast<uint64>(dyrectoryHandle.st_size)) : (0u);
}

int32 Directory::Time() const {
    return static_cast<int32>(dyrectoryHandle.st_mtime);
}

int32 Directory::LastAccessTime() const {
    return static_cast<int32>(dyrectoryHandle.st_atime);
}
}
