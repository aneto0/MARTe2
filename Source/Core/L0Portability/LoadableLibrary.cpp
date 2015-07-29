/**
 * @file LoadableLibrary.cpp
 * @brief Source file for class LoadableLibrary
 * @date 22/07/2015
 * @author Ivan Herrero
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
 * the class LoadableLibrary (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "LoadableLibrary.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,LoadableLibraryOS.h)

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

HANDLE LoadableLibrary::GetModule( void ) {
    return module;
}

void LoadableLibrary::SetModule(HANDLE const m) {
    module = m;
}

bool LoadableLibrary::Open(char8 const * const dllName) {
    return LoadableLibraryOS::Open(*this, dllName);
}

void LoadableLibrary::Close(void) {
    LoadableLibraryOS::Close(*this);
}

void *LoadableLibrary::Function(char8 const * const name) {
    void* ret = static_cast<void*>(NULL);

    if (name != NULL) {
        ret = LoadableLibraryOS::Function(*this, name);
    }
    return ret;
}
