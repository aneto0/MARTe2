/**
 * @file MutexSem.h
 * @brief Header file for class MutexSem
 * @date 17/06/2015
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

 * @details This header file contains the declaration of the class MutexSem
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MUTEXSEM_H_
#define MUTEXSEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "ErrorType.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * Forward declaration of the operating system specific properties (defined in the operating system
 * specific unit file).
 */
struct MutexSemProperties;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Mutual exclusion semaphore.
 */
class DLL_API MutexSem {

public:

    /**
     * @brief Initialises the semaphore operating system specific properties.
     */
    MutexSem();

    /**
     * @brief This semaphore instance will share the same low-level semaphore
     * (i.e. that share the same handle) with the source semaphore.
     * @param[in] source the semaphore providing the access to the low-level semaphore.
     */
    MutexSem(MutexSem &source);

    /**
     * @brief If it was not already closed, the destructor closes the semaphore.
     */
    ~MutexSem();

    /**
     * @brief Opens the semaphore with a given initial state.
     * @param[in] recursive specifies if the mutex is to be set recursive.
     * A recursive semaphore does not dead-lock if two consecutive locks are issued by the
     * same thread.
     * @return true if the operating system call returns with no errors.
     */
    bool Create(const bool &recursive = false);

    /**
     * @brief Closes the semaphore handle.
     * @return true if the operating system call returns with no errors.
     */
    bool Close();

    /**
     * @brief Locks the semaphore without timeout (i.e. possibly forever).
     * @return ErrorManagement::NoError if the operating system call returns with no errors.
     * @pre the semaphore was successfully created.
     */
    ErrorManagement::ErrorType Lock();

    /**
     * @brief Locks the semaphore up to a maximum period defined by the timeout.
     * @param[in] timeout the maximum time up until when the semaphore will be locked.
     * @return ErrorManagement::NoError if the operating system call returns with no errors or
     * Timeout if the time waiting in the Lock (from when the function was called)
     * was greater than the specified timeout.
     * @pre the semaphore was successfully created.
     */
    ErrorManagement::ErrorType Lock(const MilliSeconds &timeout);

    /**
     * @brief Unlocks the semaphore.
     * @return true if the operating system call returns with no errors.
     * @pre the semaphore was successfully created.
     */
    bool UnLock();

    /**
     * @brief Checks if the semaphore is recursive.
     * @return true if the mutex is recursive, false otherwise.
     */
    bool IsRecursive() const;

    /**
     * @brief Checks if the semaphore is closed.
     * @return true if the semaphore is closed.
     */
    bool IsClosed() const;

    /**
     * @brief Return the operating system low level properties.
     * @return the operating system low level semaphore properties structure.
     */
    MutexSemProperties *GetProperties();

private:

    /**
     * Operating system specific properties to be used by the operating system specific implementation
     */
    MutexSemHandle handle;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}
#endif /* MUTEXSEM_H_ */

