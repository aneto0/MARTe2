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
#include "HeapManager.h"
#include "CompositeErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*lint -estring(459, *MARTe::fileFilter*) global variable required for file filtering with scandir in Linux*/
// This function is called by the scandir and it is used for file filtering
static char8 *fileFilterSearchMask;

// match a shell wildcard pattern
static int32 fileFilter(const struct dirent * const de) {
    /*lint -e{9125} -e{9123} [MISRA C++ Rule 5-0-8].Justification: cast from bool to int required by this implementation*/
    return static_cast<int32>(fnmatch(fileFilterSearchMask, &de->d_name[0], 0) == 0);
}

DirectoryScanner::DirectoryScanner() :
        LinkedListHolder() {
    basePath = static_cast<char8 *>(NULL);
    size = 0u;
}

void DirectoryScanner::CleanUp() {
    size = 0;
    LinkedListHolder::CleanUp();
}

bool DirectoryScanner::Scan(CCString path, CCString fileMask, SortFilter * const sorter){
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

    int32 nOfFiles = 0;
    struct dirent **namelist = NULL;
    if (ret){
        // mallocs the filenames matched in basePath into nameList
        /*lint -e{9025} [MISRA C++ Rule 5-0-19]. Justification: struct dirent*** required by scandir(*) operating system API */
    	nOfFiles = scandir(basePath.GetList(), &namelist, &fileFilter, &alphasort);

    	ret.OSError = (nOfFiles < 0);
        REPORT_ERROR(ret, "Error: Failed scandir()");
    }

	// skip last file
    nOfFiles--;
    while ((nOfFiles >= 0) && ret) {

    	CCString fileName(&namelist[nOfFiles]->d_name[0]);
    	DynamicCString fullPath(basePath);
    	ret = fullPath().Append(fileName);

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

        nOfFiles--;
    }

    if (namelist != NULL_PTR(struct dirent **)) {
        /*lint -e{9025} [MISRA C++ Rule 5-0-19]. Justification: struct dirent*** required by scandir(*) operating system API */
        HeapManager::Free(reinterpret_cast<void*&>(namelist));
    }

    if (!ret) {
        CleanUp();
    }

    return ret;
}

DirectoryScanner::~DirectoryScanner() {
    CleanUp();
}

uint64 DirectoryScanner::DirectorySize() const {
    return size;
}

CCString DirectoryScanner::BasePath() const {
    return basePath.GetList();
}

}
