/**
 * @file CountingSem.h
 * @brief Header file for class CountingSem
 * @date 23/05/2020
 * @author Giuseppe Ferro
 * @author Martino Ferrari
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

 * @details This header file contains the declaration of the class CountingSem
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef COUNTINGSEM_H_
#define COUNTINGSEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EventSem.h"
#include "FastPollingMutexSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief Counting Semaphore 
 *
 * @details The CountingSem extends the EventSem class by implementing a 
 * simple counter logic. In order to release the barrier the method 
 * `WaitForAll` must be called at least N times, where N is the number of 
 * actors defined in the `Create` method.
 */
class CountingSem: public EventSem {
public:
    /**
     * @brief Initialize parent class and private fields.
     */
    CountingSem();
    
    /**
     * @brief TODO
     */
    virtual ~CountingSem();

    /**
     * @brief Create the semaphore and set the number of actors to wait.
     * @return true if the operating system call returns without errors.  
     */
    /*lint -e{1511} allow to use the same function name as the parent class*/
    bool Create(uint32 numberOfActorsIn);

    /**
     * @brief Wait for a `Post` event when all actors have checked in or
     * when the timeout is over.
     *
     * @param[in] timeout the maximum time the barrier will last
     * @return ErrorManagement::NoError if the operating system call returns 
     * with no errors or Timeout if the time waiting in the barrier (from when 
     * the function was called) was greater than the specified timeout.
     * @pre the semaphore was successfully created.
     */
    ErrorManagement::ErrorType WaitForAll(const TimeoutType &timeout);


    /**
     * @brief Reset the semaphore, raises the barrier and resets the counter.
     * The Reset is only possible when the `counter` is equal or greater then
     * the defined number of actors.
     * @return true if the operating system call returns without errors.
     * @pre the semaphore was successfully created.
     */
    virtual bool Reset();
    
    /**
     * @brief Force the reset of the semaphore even if the condition on the 
     * counter is not met.
     * @return true if the operating system call returns without errors.
     * @pre the semaphore was successfully created.
     */ 
    bool ForceReset();
    
    /**
     * @brief Force the post action even if the conditions on the counter and
     * on the timeout are not met.
     * @return true if the operating systemc call returns without errors.
     * @pre the semaphore was successfully created.
     */
    bool ForcePass();

private:
    /**
     * Number of Actors to wait and synchronize.
     */
    uint32 numberOfActors;

    /**
     * Counter used to count the number of actros that have already
     * checked in.
     */
    uint32 counter;

    /**
     * Simple semaphore used to protect the access to the counter 
     * field.
     */
    FastPollingMutexSem sem;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* COUNTINGSEMAPHORE_H_ */

