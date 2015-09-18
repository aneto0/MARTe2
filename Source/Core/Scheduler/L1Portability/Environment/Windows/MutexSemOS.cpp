/**
 * @file MutexSemOS.cpp
 * @brief Source file for class MutexSemOS
 * @date 20/06/2015
 * @author Giuseppe Ferr�
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
 * the class MutexSemOS (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "../../Atomic.h"
#include "MutexSem.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

struct MutexSemOSProperties {

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
     * The number of handle references pointing at this structure.
     */
    uint32 references;

};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MutexSem::MutexSem() {
    osProperties = new MutexSemOSProperties();
    osProperties->closed = true;
    osProperties->recursive = false;
    osProperties->references = 1u;
    osProperties->referencesMux = 0;
}

MutexSem::MutexSem(MutexSem &source) {
    osProperties = source.GetOSProperties();
    if (osProperties == static_cast<MutexSemOSProperties *>(NULL)) {

        //Capture the case that it got the osProperties while the source semaphore
        //was already being destructed...
        MutexSem();
    }
    else {
        while (!Atomic::TestAndSet(&osProperties->referencesMux)) {
        }
        osProperties->references++;
        osProperties->referencesMux = 0;
    }
}

bool MutexSem::Create(const bool &recursive) {
    while (!Atomic::TestAndSet(&osProperties->referencesMux)) {
    }
    osProperties->mutexHandle = CreateMutex(NULL, false, NULL);

    //only recursive mode is supported.
    osProperties->recursive = true;
    osProperties->closed = false;
    osProperties->referencesMux = 0;
    return (osProperties->mutexHandle != NULL);
}

bool MutexSem::Close() {
    bool ok = true;
    if (!osProperties->closed) {
        osProperties->closed = true;

        if ((osProperties->mutexHandle != NULL)) {

            ok = (CloseHandle(osProperties->mutexHandle) == TRUE);
        }

    }
    return ok;
}

MutexSem::~MutexSem() {

    if (osProperties != static_cast<MutexSemOSProperties *>(NULL)) {

        while (!Atomic::TestAndSet(&osProperties->referencesMux)) {
        }

        if (osProperties->references == 1u) {
            if (!osProperties->closed) {
                Close();

            }
            delete osProperties;
            osProperties = static_cast<MutexSemOSProperties *>(NULL);
        }
        else {

            osProperties->references--;
            osProperties->referencesMux = 0;
        }
    }
}

ErrorType MutexSem::Lock() {
    DWORD ret = WaitForSingleObject(osProperties->mutexHandle, INFINITE);
    ErrorType error = ErrorManagement::NoError;
    if (ret == WAIT_FAILED) {
        error = ErrorManagement::OSError;
    }

    return error;
}

ErrorType MutexSem::Lock(const TimeoutType &timeout) {
    DWORD ret = WaitForSingleObject(osProperties->mutexHandle, timeout.GetTimeoutMSec());
    ErrorType error = ErrorManagement::NoError;
    if (ret == WAIT_FAILED) {
        error = ErrorManagement::OSError;
    }
    if (error == ErrorManagement::NoError && ret == WAIT_TIMEOUT) {
        error = Timeout;
    }
    return error;
}

bool MutexSem::UnLock() {

    return (ReleaseMutex(osProperties->mutexHandle) == TRUE);
}

bool MutexSem::IsRecursive() const {
    return osProperties->recursive;
}

MutexSemOSProperties * MutexSem::GetOSProperties() {
    return osProperties;
}

bool MutexSem::IsClosed() const {
    bool ok = true;
    if (osProperties != static_cast<MutexSemOSProperties *>(NULL)) {
        ok = osProperties->closed;
    }
    return ok;
}
