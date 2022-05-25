/**
 * @file DirectoryScanner.cpp
 * @brief Source file for class DirectoryScanner
 * @date 04/11/2015
 * @author Giuseppe Ferro
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
#include "ff.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DirectoryScanner.h"
#include "Directory.h"
#include "GeneralDefinitions.h"
#include "GlobalObjectsDatabase.h"
#include "StringHelper.h"
#include "HeapManager.h"
#include "MemoryOperationsHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#define FATFS_MINIMUM_PATH_LENGTH 4
#define FATFS_DIRECTORY_SEPARATOR '/'

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

// /*lint -estring(459, *MARTe::fileFilter*) global variable required for file filtering with scandir in Linux*/
// // This function is called by the scandir and it is used for file filtering
// static char8 *fileFilterSearchMask;

// // match a shell wildcard pattern
// static int32 fileFilter(const struct dirent * const de) {
//     /*lint -e{9125} -e{9123} [MISRA C++ Rule 5-0-8].Justification: cast from bool to int required by this implementation*/
//     return static_cast<int32>(fnmatch(fileFilterSearchMask, &de->d_name[0], 0) == 0);
// }

DirectoryScanner::DirectoryScanner() :
        LinkedListHolder() {
    basePath = NULL_PTR(char8*);
    size = 0u;
}

void DirectoryScanner::CleanUp() {
    LinkedListHolder::CleanUp();
    size = 0u;
    if (basePath != NULL) {
        if (!HeapManager::Free(reinterpret_cast<void *&>(basePath))) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "DirectoryScanner::Cleanup() Cannot free base path");
        }
        basePath = NULL_PTR(char8*);
    }
}

bool DirectoryScanner::Scan(const char8 * const path,
                            const char8 *fileMask,
                            SortFilter * const sorter) {

    bool ret = true;
    CleanUp();
    uint32 pathLen = 0u;

    //If path is NULL, out path becomes the root, in other words only a FATFS_PARTITION_SDCARD:DIRECTORY_SEPARATOR alone
    //Which means Scan onto SD Card root
    if(path == NULL_PTR(char8*)) {
        pathLen = StringHelper::Length(FATFS_MINIMUM_PATH_LENGTH);
        basePath = static_cast<char8*>(HeapManager::Malloc(pathLen + 1u));
        basePath[pathLen] = '\0';
        if(!StringHelper::Copy(basePath, FATFS_MINIMUM_PATH_LENGTH)) {
            ret = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "DirectoryScanner::Scan() Cannot copy default path");
        }
    }
    else {
        //If path is not NULL, we have to copy it to our local reference
        pathLen = StringHelper::Length(path);
        basePath = static_cast<char8*>(HeapManager::Malloc(pathLen + 2u));
        if(!StringHelper::Copy(basePath, path)) {
            ret = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "DirectoryScanner::Scan() Cannot copy path");
        }

        if (ret) {
            //Once copy succeeded, let's terminate the path with a separator and close the string
            basePath[pathLen] = FATFS_DIRECTORY_SEPARATOR;
            basePath[pathLen + 1] = '\0';
        }
    }
    
    if (ret) {
        //When the mask is null, it becomes the catch-all wildcard
        if(fileMask == NULL_PTR(char8*)) {
            fileMask = "*";
        }

        //Once mask and path are ready, let's initialize FatFs routines to scan
        DIR directoryPointer;
        FILINFO fileInfo;
        FRESULT scanResult = f_findfirst(&directoryPointer, &fileInfo, (TCHAR*)path, (TCHAR*)(fileMask));
        char8 fullPath[512];

        while((scanResult == FR_OK) && (fileInfo.fname[0] != '\0') && ret) {
            //Prepare the temporary path holder
            if (!MemoryOperationsHelper::Set(&fullPath[0], '\0', 512)) {
                ret = false;
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "DirectoryScanner::Scan() Failure in clearing up temporary path buffer");
            }

            //Concatenate base path and current file/directory name
            if(ret) {
                if (!StringHelper::Concatenate(&fullPath[0], basePath)) {
                    ret = false;
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "DirectoryScanner::Scan() Failure in base path concatenation");
                }
                if(ret) {
                    if (!StringHelper::Concatenate(&fullPath[0], fileInfo.fname)) {
                        ret = false;
                        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "DirectoryScanner::Scan() Failure in file name concatenation");
                    }   
                }
            }

            //Prepare the directory entry and set it with the path of the item
            if (ret) {
                //Skipping "current [.]" and "parent [..]"
                if( (StringHelper::Compare(fileInfo.fname, ".") != 0) && (StringHelper::Compare(fileInfo.fname, "..") != 0)) {
                    size++;

                    Directory *entry = new Directory();

                    //Set the directory entry and handle eventual failure
                    if(!entry->SetByName(&fullPath[0])) {
                        ret = false;
                        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "DirectoryScanner::Scan() Failure in Directory entry SetByName");

                        delete entry;
                    }

                    //Add the directory entry to the list, w or w/o the sorter
                    if(ret) {
                        if (sorter == NULL) {
                            ListInsert(entry);
                        }
                        else {
                            ListInsert(entry, sorter);
                        }
                    }
                }
                scanResult = f_findnext(&directoryPointer, &fileInfo);
            }
        }
        f_closedir(&directoryPointer);
    }

    return ret;
}

DirectoryScanner::~DirectoryScanner() {
    size = 0u;
    if (basePath != NULL) {
        /*lint -e{1551} .Justification: Remove the warning "Function may throw exception '...' in destructor".*/
        if (!HeapManager::Free(reinterpret_cast<void *&>(basePath))) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "DirectoryScanner::~DirectoryScanner() Failure deallocating base path");
        }
        basePath = NULL_PTR(char8*);
    }
}

uint64 DirectoryScanner::DirectorySize() const {
    return size;
}

const char8* DirectoryScanner::BasePath() const {
    return basePath;
}

}
