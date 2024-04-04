/**
 * @file MutexSem.cpp
 * @brief Source file for class MutexSem
 * @date 20/06/2015
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
 * the class MutexSem (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Atomic.h"
#include "MutexSem.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

struct MutexSemProperties {

    /**
     * Mutex Handle
     */
    HANDLE mutexHandle;

    /**
     * To protect the reference handling
     */
    int32 referencesMux;

    /**
     * Is the semaphore closed?
     */
    bool closed;

    /**
     * Is the semaphore recursive
     */
    bool recursive;

    /**
     * The number of  references pointing at this structure.
     */
    uint32 references;

};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MutexSem::MutexSem() {
    handle = new MutexSemProperties();
    handle->closed = true;
    handle->recursive = false;
    handle->references = 1u;
    handle->referencesMux = 0;
}

MutexSem::MutexSem(MutexSem &source) {
    handle = source.GetProperties();
    if (handle == static_cast<MutexSemProperties *>(NULL)) {

        //Capture the case that it got the handle while the source semaphore
        //was already being destructed...
        MutexSem();
    }
    else {
        while (!Atomic::TestAndSet(&handle->referencesMux)) {
        }
        handle->references++;
        handle->referencesMux = 0;
    }
}

bool MutexSem::Create(const bool &recursive) {
    while (!Atomic::TestAndSet(&handle->referencesMux)) {
    }
    handle->mutexHandle = CreateMutex(NULL, false, NULL);

    //only recursive mode is supported.
    handle->recursive = true;
    handle->closed = false;
    handle->referencesMux = 0;
    return (handle->mutexHandle != NULL);
}

bool MutexSem::Close() {
    bool ok = true;
    if (!handle->closed) {
        handle->closed = true;

        if ((handle->mutexHandle != NULL)) {

            ok = (CloseHandle(handle->mutexHandle) == TRUE);
        }

    }
    return ok;
}

MutexSem::~MutexSem() {

    if (handle != static_cast<MutexSemProperties *>(NULL)) {

        while (!Atomic::TestAndSet(&handle->referencesMux)) {
        }

        if (handle->references == 1u) {
            if (!handle->closed) {
                Close();

            }
            delete handle;
            handle = static_cast<MutexSemProperties *>(NULL);
        }
        else {

            handle->references--;
            handle->referencesMux = 0;
        }
    }
}

ErrorManagement::ErrorType MutexSem::Lock() {
    DWORD ret = WaitForSingleObject(handle->mutexHandle, INFINITE);
    ErrorManagement::ErrorType error = ErrorManagement::NoError;
    if (ret == WAIT_FAILED) {
        error = ErrorManagement::OSError;
    }

    return error;
}

ErrorManagement::ErrorType MutexSem::Lock(const TimeoutType &timeout) {
    DWORD ret = WaitForSingleObject(handle->mutexHandle, timeout.GetTimeoutMSec());
    ErrorManagement::ErrorType error = ErrorManagement::NoError;
    if (ret == WAIT_FAILED) {
        error = ErrorManagement::OSError;
    }
    if (error == ErrorManagement::NoError && ret == WAIT_TIMEOUT) {
        error = ErrorManagement::Timeout;
    }
    return error;
}

bool MutexSem::UnLock() {

    return (ReleaseMutex(handle->mutexHandle) == TRUE);
}

bool MutexSem::IsRecursive() const {
    return handle->recursive;
}

MutexSemProperties * MutexSem::GetProperties() {
    return handle;
}

bool MutexSem::IsClosed() const {
    bool ok = true;
    if (handle != static_cast<MutexSemProperties *>(NULL)) {
        ok = handle->closed;
    }
    return ok;
}

}
