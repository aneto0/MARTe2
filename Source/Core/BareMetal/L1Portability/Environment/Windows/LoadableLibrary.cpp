/**
 * @file LoadableLibrary.cpp
 * @brief Source file for class LoadableLibrary
 * @date 28/09/2015
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
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "LoadableLibrary.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

HANDLE LoadableLibrary::GetModule() {
    return module;
}

void LoadableLibrary::SetModule(HANDLE const m) {
    module = m;
}

void LoadableLibrary::Close() {
    HANDLE m = GetModule();

    if (m != 0) {
        FreeLibrary((HMODULE) m);
    }
}

bool LoadableLibrary::Open(char8 const * const dllName) {
    HANDLE m = GetModule();

    if (m != 0) {
        Close();
    }
    m = LoadLibrary(dllName);
    if (m == NULL) {
        LPVOID lpMsgBuf;
        DWORD errorCode = GetLastError();
        
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            errorCode,
            0,
            (LPSTR)&lpMsgBuf,
            0,
            NULL
        );
        const char8 *err1 = dllName;
        const char8 *space = ":";
        char8 *err2 =  (char8*)lpMsgBuf;
        uint32 errLen = (StringHelper::Length(err1) + StringHelper::Length(err2) + 2u);
        char8* err_= new char8[errLen];
        (void) StringHelper::Concatenate(err1, space, &err_[0]);
        (void) StringHelper::Concatenate(err_, err2, &err_[0]);
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, &err_[0]);
        printf("Error Code (%d)\n",errorCode);
        delete [] err_;
        LocalFree(lpMsgBuf);
        return false;
    }   
    SetModule(m);
    return GetModule() != NULL;
}

void *LoadableLibrary::Function(char8 const * const name) {

    void* ret = NULL;
    HANDLE m = GetModule();

    if ((m != NULL) && (name != NULL)) {

        ret = GetProcAddress((HMODULE) m, name);
    }

    return ret;
}

}

