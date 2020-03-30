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
#include "CompositeErrorManagement.h"
#include "MemoryOperators.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

Directory::Directory(CCString path) : LinkedListable() {
	ErrorManagement::ErrorType ret;
    if (path.GetSize() > 0) {
    	ret.OSError = (stat(path.GetList(), &directoryHandle) != 0);
        COMPOSITE_REPORT_ERROR(ret, "Error: Failed stat() in initialization: ",errno);
    } else
    {
        ret = Memory::Set(&directoryHandle, '\0', static_cast<uint32>(sizeof(DirectoryCore)));
        REPORT_ERROR(ErrorManagement::Warning, "Failed initialization of directory handle");
    }
}

Directory::~Directory() {
}


bool Directory::SetByName(CCString path) {
	fileName = path;
    return true;
}

CCString Directory::GetName() const {
    return fileName.GetList();
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
            REPORT_ERROR(ErrorManagement::FatalError, "Error: Failed TimeStamp::ConvertFromEpoch");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::OSError, "Error: stat()");
    }

    return timeStamp;
}

TimeStamp Directory::GetLastAccessTime() {
    TimeStamp timeStamp;
    if (stat(GetName(), &directoryHandle) == 0) {
        time_t secondsFromEpoch32 = static_cast<int32>(directoryHandle.st_atime);
        if (!timeStamp.ConvertFromEpoch(secondsFromEpoch32)) {
            REPORT_ERROR(ErrorManagement::FatalError, "Error: Failed TimeStamp::ConvertFromEpoch");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::OSError, "Error: stat()");
    }

    return timeStamp;
}

bool Directory::Create(const bool isFile) {
	ErrorManagement::ErrorType ret;

	ret.parametersError = (fileName.GetSize() > 0);
    REPORT_ERROR(ret, "Error: directory/file name is empty");

    if (ret && isFile) {
        /*lint -e{9130} -e{9117} [MISRA C++ Rule 5-0-21]  [MISRA C++ Rule 5-0-4]. Justification: Operating system APIs are not linted.*/
        int32 fd = open(fileName.GetList(), static_cast<mode_t>(00777 | O_EXCL | O_CREAT | O_WRONLY | O_TRUNC));
        ret.OSError = (fd < 0);
        COMPOSITE_REPORT_ERROR(ret, "Error: Failed open(",fileName,")");

        if (ret){
            ret.OSError = (close(fd) < 0);
            REPORT_ERROR(ret, "Error: Failed close()");
        }
    }

    if (ret && !isFile) {
    	ret.OSError = (mkdir(fileName.GetList(), static_cast<mode_t>(0777)) != 0);
        COMPOSITE_REPORT_ERROR(ret, "Error: Failed mkdir(",fileName,")");
    }

    if (ret){
        ret.OSError =  (stat(fileName.GetList(), &directoryHandle) != 0);
        REPORT_ERROR(ret, "Error: Failed stat() in initialization");
    }
    return ret;
}

/*lint -e{9130} -e{9117} -e{1762} [MISRA C++ Rule 5-0-4] [MISRA C++ Rule 5-0-4] [MISRA C++ Rule 9-3-3].
 * Justification: Operating system APIs are not linted. In other operating systems Exists() might not be constant.*/
bool Directory::Exists() {
    struct stat fileStats;
    bool ok = (stat(fileName.GetList(), &fileStats) == 0);
    bool isDir = S_ISDIR(fileStats.st_mode);
    bool isFile = S_ISREG(fileStats.st_mode);
    return (ok) ? (isDir || isFile) : (false);
}

/*lint -e{1762} [MISRA C++ Rule 9-3-3] in other operating systems Delete() might not be constant*/
bool Directory::Delete() {
    bool ok = (fileName.GetSize() > 0);
    if (ok) {
        /*lint -e{668} fname cannot be NULL (checked in line above)*/
        ok = (remove(fileName.GetList()) == 0);
    }
    return ok;
}

}

