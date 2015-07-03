/**
 * @file MutexSem.cpp
 * @brief Source file for class MutexSem
 * @date 17/06/2015
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
 * the class MutexSem (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MutexSem.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MutexSem::MutexSem(const MutexSem &source) {
    const HANDLE h = source.GetHandle();
    MutexSemOS::DuplicateHandle(h);
    isRecursive = source.IsRecursive();
    semH = h;
}

MutexSem::MutexSem() {
    semH = static_cast<HANDLE>(NULL);
    isRecursive = false;
}

MutexSem::~MutexSem() {
    MutexSemOS::Close(semH);
}

bool MutexSem::Create(bool locked, bool recursive) {
    bool ret = MutexSemOS::Create(semH, locked, recursive);
    isRecursive = recursive;
    return ret;
}

bool MutexSem::Close() {
    return MutexSemOS::Close(semH);
}

bool MutexSem::Lock(TimeoutType msecTimeout, Error &error) const {
    return MutexSemOS::Lock(semH, msecTimeout, error);
}

bool MutexSem::UnLock() const {
    return MutexSemOS::UnLock(semH);
}

const HANDLE MutexSem::GetHandle() const {
    return semH;
}

