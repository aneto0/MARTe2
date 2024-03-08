/**
 * @file DirectoryScanner.cpp
 * @brief Source file for class DirectoryScanner
 * @date Nov 17, 2015
 * @author CCS Portable Station
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
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <windows.h>
#include <iostream>
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DirectoryScanner.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

DirectoryScanner::DirectoryScanner() :
        LinkedListHolder() {
    basePath = static_cast<char8 *>(NULL);
    size = 0u;
}

DirectoryScanner::~DirectoryScanner() {
    size = 0u;
    if (basePath != NULL) {
        if (!HeapManager::Free(reinterpret_cast<void *&>(basePath))) {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: Failed HeapManager::Free");
        }
        basePath = static_cast<char8 *>(NULL);
    }
}

void DirectoryScanner::CleanUp() {
    LinkedListHolder::CleanUp();
    size = 0u;
    if (basePath != NULL) {
        if (!HeapManager::Free(reinterpret_cast<void *&>(basePath))) {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: Failed HeapManager::Free");
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
            HeapManager::Free(reinterpret_cast<void *&>(basePath));
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
        uint32 pathSize = 512u;
        char8 statAddr[512];
        char8 *searchMask = NULL;
        if (fileMask != NULL) {
            searchMask = static_cast<char8 *>(HeapManager::Malloc(strlen(basePath)+strlen(fileMask)+2u));
            StringHelper::Copy(searchMask,basePath);

            if(StringHelper::Compare(fileMask, "") != 0) {
                StringHelper::Concatenate(searchMask,fileMask);
            }
            else {
                StringHelper::Concatenate(searchMask,"*");
            }
        }
        else {
            searchMask = static_cast<char8 *>(HeapManager::Malloc(strlen(basePath)+2u));
            searchMask = StringHelper::StringDup(basePath);
            StringHelper::Concatenate(searchMask,"*");

        }

        WIN32_FIND_DATA lpFindFileData;
        HANDLE h = FindFirstFile(searchMask, &lpFindFileData);
        if (h == INVALID_HANDLE_VALUE) {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: Failed FindFirstFile() in initialization");
            FindClose(h);
            ret = false;
        }
        if (ret) {

            if (!StringHelper::Copy(&statAddr[0], basePath)) {
                ret = false;
            }
            // concatenate it with the file name to create the full path
            if (ret) {
                if (!StringHelper::Concatenate(&statAddr[0], (char8 *) lpFindFileData.cFileName)) {
                    ret = false;
                }
            }
            if (StringHelper::Compare((char8 *) lpFindFileData.cFileName, ".") != 0) {
                Directory *entry = new Directory(&statAddr[0]);
                if (ret) {
                    if (sorter == NULL) {
                        ListInsert(entry);
                    }
                    else {
                        ListInsert(entry,sorter);
                    }
                }
            }

            while (FindNextFile(h, &lpFindFileData)) {
                // empties statAddr
                if (!StringHelper::Copy(&statAddr[0], basePath)) {
                    ret = false;
                }

                if (ret) {
                    // concatenate it with the file name to create the full path
                    if (!StringHelper::Concatenate(&statAddr[0], (char8 *) lpFindFileData.cFileName)) {
                        ret = false;
                    }
                }

                if (StringHelper::Compare((char8 *) lpFindFileData.cFileName, "..") != 0) {

                    Directory * temp = new Directory(&statAddr[0]);
                    if (sorter == NULL) {
                        ListInsert(temp);
                    }
                    else {
                        ListInsert(temp, sorter);
                    }
                    size += temp->GetSize();
                }
            }
        }

        HeapManager::Free(reinterpret_cast<void *&>(searchMask));

        if (!ret) {
            CleanUp();
        }

        if (FindClose(h) == 0) {
            ret = false;
        }
    }
    return ret;
}

uint64 DirectoryScanner::DirectorySize() const {
    return size;
}

const char8* DirectoryScanner::BasePath() const {
    return basePath;
}

}

