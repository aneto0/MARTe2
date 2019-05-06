/**
 * @file MutexSem.cpp
 * @brief Source file for class MutexSem
 * @date 08/07/2015
 * @author André Neto
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

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <pthread.h>
#include <math.h>
#include <sys/timeb.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MutexSem.h"
#include "Atomic.h"
#include "FastPollingMutexSem.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*lint -e{9109} forward declaration in MutexSem.h is required to define the class*/
struct MutexSemProperties {

    FastPollingMutexSem sem;

};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MutexSem::MutexSem() {
    handle=new MutexSemProperties;
}

MutexSem::MutexSem(MutexSem &source) {
    *handle = *(source.handle);
}

/*lint -e{1551} only C calls are performed. No exception can be raised*/
MutexSem::~MutexSem() {
    delete handle;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool MutexSem::Create(const bool &recursive) {
    handle->sem.Create();
    return true;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool MutexSem::Close() {

    return true;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
/*lint -e{454} -e{456} false positive, the semaphore will be unlocked by the UnLock function*/
ErrorManagement::ErrorType MutexSem::Lock() {
    return !(handle->sem.FastLock(TTInfiniteWait, 1e-3));
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
ErrorManagement::ErrorType MutexSem::Lock(const TimeoutType &timeout) {
    return !(handle->sem.FastLock(timeout, 1e-3));
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool MutexSem::UnLock() {
    handle->sem.FastUnLock();
    return true;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool MutexSem::IsRecursive() const {
    return true;
}

MutexSemProperties * MutexSem::GetProperties() {
    return handle;
}

bool MutexSem::IsClosed() const {
}

}
