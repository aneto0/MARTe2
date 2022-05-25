/**
 * @file Directory.cpp
 * @brief Source file for class Directory
 * @date 20/04/2019
 * @author Andre Neto
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
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

Directory::Directory(const char8 * const path) :
        LinkedListable() {
    fname = "";
    directoryHandle = 0;
}

Directory::~Directory() {
}

bool Directory::SetByName(const char8 * const path) {
    return false;
}

const char8 *Directory::GetName() const {
    return "";
}

bool Directory::IsDirectory() const {
    return false;
}

bool Directory::IsFile() const {
    return false;
}

uint64 Directory::GetSize() {
    return 0LLU;
}

TimeStamp Directory::GetLastWriteTime() {
    TimeStamp timeStamp;
    return timeStamp;
}

TimeStamp Directory::GetLastAccessTime() {
    TimeStamp timeStamp;
    return timeStamp;
}

bool Directory::Create(const bool isFile) {
    return false;
}

bool Directory::Exists() {
    return false;;
}

bool Directory::Delete() {
    return false;
}

}

