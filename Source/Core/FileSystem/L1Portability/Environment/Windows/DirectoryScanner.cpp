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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DirectoryScanner.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

DirectoryScanner::DirectoryScanner() :
        LinkedListHolder() {
    size = 0u;
}

DirectoryScanner::~DirectoryScanner() {
    size = 0u;
}

void DirectoryScanner::CleanUp() {
    LinkedListHolder::CleanUp();
    size = 0u;
}

bool DirectoryScanner::Scan(CCString path,
                            CCString fileMask,
                            SortFilter * const sorter) {

    ErrorManagement::ErrorType ret;
    CleanUp();

    basePath = path;
    if (basePath.GetSize()==0){
    	ret = basePath().Append((char8)DIRECTORY_SEPARATOR);
        REPORT_ERROR(ret, "Error: Failed appending strings");
    }

    if (ret && (basePath[basePath.GetSize()-1] != DIRECTORY_SEPARATOR)){
    	ret = basePath().Append((char8)DIRECTORY_SEPARATOR);
        REPORT_ERROR(ret, "Error: Failed appending strings");
    }

    if (fileMask.GetSize()==0){
    	fileMask = "*";
    }
    DynamicCString searchMask;
    if (ret){
    	ret = searchMask().Append(basePath).Append(fileMask);
        REPORT_ERROR(ret, "Error: Failed appending strings");
    }

    WIN32_FIND_DATA lpFindFileData;
    HANDLE h = INVALID_HANDLE_VALUE;
    if (ret){
        h = FindFirstFile(searchMask.GetList(), &lpFindFileData);
        ret.OSError = (h == INVALID_HANDLE_VALUE);
        REPORT_ERROR(ret, "Error: Failed FindFirstFile() in initialization");
    }

    if (ret){
        do {
        	DynamicCString fullPath;
        	CCString fileName(&lpFindFileData.cFileName[0]);
        	ret = fullPath().Append(basePath).Append(fileName);
            REPORT_ERROR(ret, "Error: Failed appending strings");

        	if (ret && !fileName.IsSameAs(".") && !fileName.IsSameAs("..")){
                Directory *entry = new Directory(fullPath);
                if (sorter == NULL) {
                    ListInsert(entry);
                }
                else {
                    ListInsert(entry,sorter);
                }
                size += entry->GetSize();
        	}
        }
        while(ret && FindNextFile(h, &lpFindFileData));
    }

    if (h != INVALID_HANDLE_VALUE){
    	ret.OSError = (FindClose(h) == 0);
    	REPORT_ERROR(ret, "Error: closing handle");
    }

    return ret;
}

uint64 DirectoryScanner::DirectorySize() const {
    return size;
}

CCString DirectoryScanner::BasePath() const {
    return basePath;
}

}

