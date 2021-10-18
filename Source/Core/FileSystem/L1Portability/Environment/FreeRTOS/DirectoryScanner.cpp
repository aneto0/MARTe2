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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DirectoryScanner.h"
#include "Directory.h"
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

void DirectoryScanner::CleanUp() {
}

bool DirectoryScanner::Scan(const char8 * const path,
                            const char8 *fileMask,
                            SortFilter * const sorter) {

    return false;
}

DirectoryScanner::~DirectoryScanner() {
}

uint64 DirectoryScanner::DirectorySize() const {
    return size;
}

const char8* DirectoryScanner::BasePath() const {
    return basePath;
}

}
