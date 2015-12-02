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
#include <tchar.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "DirectoryScanner.h"
#include "Directory.h"
#include "StringHelper.h"
#include "HeapManager.h"
#include "MemoryOperationsHelper.h"
#include "LinkedListHolder.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

DirectoryScanner::DirectoryScanner(const char8 * const path) :
        LinkedListHolder() {
    basePath = StringHelper::StringDup(path);
    size = 0u;
}

DirectoryScanner::~DirectoryScanner() {
    size = 0u;
    if (basePath != NULL) {

        if (!HeapManager::Free(reinterpret_cast<void *&>(basePath))) {

        }
        basePath = static_cast<char8 *>(NULL);
    }
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
        uint32 pathSize = 512u;
        char8 statAddr[512];
        char *searchMask = NULL;
        if (fileMask != NULL) {
            searchMask = static_cast<char8 *>(HeapManager::Malloc(strlen(basePath)+strlen(fileMask)+2u));
            StringHelper::Copy(searchMask,basePath);
            //StringHelper::Concatenate(searchMask,&DIRECTORY_SEPARATOR);
            if(StringHelper::Compare(fileMask, "") != 0) {
                StringHelper::Concatenate(searchMask,fileMask);
            }
            else {
                StringHelper::Concatenate(searchMask,"*");

            }
        }
        else {
            searchMask = static_cast<char8 *>(HeapManager::Malloc(strlen(basePath)+3u));
            searchMask = StringHelper::StringDup(basePath);
            //StringHelper::Concatenate(searchMask,&DIRECTORY_SEPARATOR);
            StringHelper::Concatenate(searchMask,"*");

        }

        WIN32_FIND_DATA lpFindFileData;
        HANDLE h = FindFirstFile(searchMask, &lpFindFileData);
        if (h == INVALID_HANDLE_VALUE) {
            HeapManager::Free(reinterpret_cast<void *&>(searchMask));
            HeapManager::Free(reinterpret_cast<void *&>(basePath));
            ret = false;
            printf("FindFirstFile invalid");
            //REPORT_ERROR(ErrorManagement::OSError, "Error: Failed FindFirstFile() in initialization");
        }
        if (ret) {
            Directory *entry = new Directory();
            entry->SetByName(searchMask);

            if (sorter == NULL) {
                ListInsert(entry);
                printf("ListInsert:%s\n", entry->GetName());
            }
            else {
                ListInsert(entry,sorter);
                printf("ListInsertsorter:%s\n", entry->GetName());
            }

            while (FindNextFile(h, &lpFindFileData)) {
                // empties statAddr
                if (!StringHelper::Copy(&statAddr[0], basePath)) {
                    ret = false;
                }

                /*if (!StringHelper::Concatenate(&statAddr[0], &DIRECTORY_SEPARATOR)) {
                 ret = false;
                 }*/

                if (ret) {
                    // concatenate it with the file name to create the full path
                    if (!StringHelper::Concatenate(&statAddr[0], (char8 *) lpFindFileData.cFileName)) {
                        ret = false;
                    }

                }

                entry->SetByName(&statAddr[0]);
                if (StringHelper::Compare((char8 *) lpFindFileData.cFileName, "..") == 0) {

                }
                else {
                    size += entry->GetSize();
                }
                Directory * temp = new Directory(&statAddr[0]);
                if (sorter == NULL) {
                    //ListInsert(entry);
                    ListInsert(temp);
                    printf("ListInsert:%s\n", temp->GetName());
                }
                else {
                    ListInsert(temp, sorter);
                    printf("ListInsertsorter:%s\n", temp->GetName());
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

