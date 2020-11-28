/**
 * @file LoadableLibrary.cpp
 * @brief Source file for class LoadableLibrary
 * @date 26/08/2015
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
 * the class LoadableLibrary (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <dlfcn.h>
#include "LoadableLibrary.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

HANDLE LoadableLibrary::GetModule() {
    return module;
}

void LoadableLibrary::SetModule(HANDLE const m) {
    module = m;
}

void LoadableLibrary::Close() {
    HANDLE m = GetModule();
    if (m != 0) {
        (void)dlclose(m);
    }

}

bool LoadableLibrary::Open(char8 const * const dllName) {
    bool ret = true;

    HANDLE m = GetModule();
    if (m != 0) {
        Close ();
    }

    /*lint -e{9130} (bitwise operator '|' applied to signed underlying type) the operands are defined
     * in operating system api*/
    m = dlopen(dllName, RTLD_NOW | RTLD_GLOBAL);
    if (m == NULL) {
        const char8 *err1 = "LoadableLibrary: Failed dlopen(): ";
        char8 *err2 = dlerror();
        uint32 errLen = (StringHelper::Length(err1) + StringHelper::Length(err2) + 1u);
        char8 err[errLen];
        (void) StringHelper::Concatenate(err1, err2, &err[0]);
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, &err[0]);
        ret = false;
    }
    SetModule(m);
    return ret;
}

void *LoadableLibrary::Function(char8 const * const name) {
    void* ret = static_cast<void*>(NULL);

    if (name != NULL) {
        HANDLE m = GetModule();
        if (m != NULL) {
            ret = dlsym(m, name);
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "LoadableLibrary: Failed GetModule()");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "LoadableLibrary: Invalid input arguments");

    }

    return ret;
}

}
