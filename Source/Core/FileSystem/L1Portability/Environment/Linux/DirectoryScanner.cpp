/**
 * @file DirectoryScanner.cpp
 * @brief Source file for class DirectoryScanner
 * @date 04/nov/2015
 * @author pc
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
 * the class DirectoryScanner (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fnmatch.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DirectoryScanner.h"
#include "Directory.h"
#include "StringHelper.h"
#include "HeapManager.h"
#include "MemoryOperationsHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

// This function is called by the scandir and it is used for file filtering
static char8 *fileFilterSearchMask;

// match a shell wildcard pattern
static int32 fileFilter(const struct dirent * const de) {
    /*lint -e{9125} -e{9123} [MISRA C++ Rule 5-0-8].Justification: cast from bool to int required by this implementation*/
    return static_cast<int32>(fnmatch(fileFilterSearchMask, &de->d_name[0], 0) == 0);
}

DirectoryScanner::DirectoryScanner(const char8 * const path) :
        LinkedListHolder() {
    basePath = StringHelper::StringDup(path);
    size = 0u;
}

void DirectoryScanner::CleanUp() {
    LinkedListHolder::CleanUp();
    size = 0u;
    if (basePath != NULL) {
        if (!HeapManager::Free(reinterpret_cast<void *&>(basePath))) {

        }
        basePath = static_cast<char8 *>(NULL);
    }
}

bool DirectoryScanner::Scan(const char8 * const path,
                            const char8 *fileMask,
                            SortFilter * const sorter) {

    bool ret = true;

    CleanUp();

    // if path is not null it is copied into basePath
    uint32 addressLen = StringHelper::Length(path);
    if ((path != NULL) && (addressLen > 0u)) {
        basePath = static_cast<char8 *>(HeapManager::Malloc(addressLen + 2u));

        if (!StringHelper::Copy(basePath, path)) {
            ret = false;
        }
    }

    if (ret) {
        // if basePath is still NULL it becomes "\" (root)
        if (basePath == NULL) {
            basePath = static_cast<char8 *>(HeapManager::Malloc(2u));
            if (basePath != NULL) {
                basePath[0] = DIRECTORY_SEPARATOR;
                basePath[1] = '\0';
            }
            else {
                ret = false;
            }
        }
        else {
            // otherwise append the separator at the end of basePath
            uint32 baseAddressLen = StringHelper::Length(basePath);
            if (baseAddressLen > 0u) {
                uint32 index = baseAddressLen - 1u;
                if ((path[index] != '\\') || (path[index] != '/')) {
                    basePath[baseAddressLen] = DIRECTORY_SEPARATOR;
                    index = baseAddressLen + 1u;
                    basePath[index] = '\0';
                }
            }
            else {
                ret = false;
            }
        }
    }

    if (ret) {

        // if the file mask is NULL it becomes * (all the files)
        if (fileMask == NULL) {
            fileMask = "*";
        }

        // copy fileMask in fileFilterSearchMask

        fileFilterSearchMask = static_cast<char8 *>(HeapManager::Malloc(StringHelper::Length(fileMask) + 1u));

        if (!StringHelper::Copy(fileFilterSearchMask, fileMask)) {
            ret = false;
        }

    }
    if (ret) {
        uint32 pathSize = 512u;
        char8 statAddr[512];

        struct dirent **namelist;

        // mallocs the filenames matched in basePath into nameList
        /*lint -e{9025} [MISRA C++ Rule 5-0-19]. Justification: struct dirent*** required by scandir(*) operating system API */
        int32 n = scandir(basePath, &namelist, &fileFilter, &alphasort);

        if (n < 0) {
            REPORT_ERROR(ErrorManagement::OSError, "Error: Failed scandir()");
            ret = false;
        }
        else {
            n--;
            while (n >= 0) {
                Directory *entry = new Directory();

                // empties statAddr
                if (!MemoryOperationsHelper::Set(&statAddr[0], '\0', pathSize)) {
                    ret = false;
                }

                if (ret) {
                    // put in statAddr the base path
                    if (!StringHelper::Concatenate(&statAddr[0], basePath)) {
                        ret = false;
                    }
                    // put the separator
                    //if (!StringHelper::Concatenate(&statAddr[0], &DIRECTORY_SEPARATOR)) {
                    //    ret = false;
                    //}
                }

                if (ret) {
                    // concatenate it with the file name to create the full path
                    if (!StringHelper::Concatenate(&statAddr[0], &(namelist[n]->d_name[0]))) {
                        ret = false;
                    }
                }

                if (ret) {

                    // store the file name
                    if (!entry->SetByName(&statAddr[0])) {
                        ret = false;
                    }

                }

                if (ret) {
                    if (sorter == NULL) {
                        ListInsert(entry);
                    }
                    else {
                        ListInsert(entry, sorter);
                    }
                }
                else {
                    delete entry;
                    break;
                }

                // refresh the directory size adding the size of the current file
                //size += entry->Size();
                if ((StringHelper::Compare(&(namelist[n]->d_name[0]),".")!=0) && (StringHelper::Compare(&(namelist[n]->d_name[0]),"..") != 0)){
                    size += entry->GetSize();
                }

                n--;
            }
        }
    }

    if (!ret) {
        CleanUp();
    }

    if (!HeapManager::Free(reinterpret_cast<void *&>(fileFilterSearchMask))) {

    }
    return ret;
}

DirectoryScanner::~DirectoryScanner() {
    size = 0u;
    if (basePath != NULL) {
        /*lint -e{1551} .Justification: Remove the warning "Function may throw exception '...' in destructor".*/
        if (!HeapManager::Free(reinterpret_cast<void *&>(basePath))) {

        }
        basePath = static_cast<char8 *>(NULL);
    }
}

uint64 DirectoryScanner::DirectorySize() const {
    return size;
}

const char8* DirectoryScanner::BasePath() const {
    return basePath;
}

}
