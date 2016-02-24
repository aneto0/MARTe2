/**
 * @file RealTimeDataSource.h
 * @brief Header file for class RealTimeDataSource
 * @date 22/02/2016
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

 * @details This header file contains the declaration of the class RealTimeDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEDATASOURCE_H_
#define REALTIMEDATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GlobalObjectI.h"
#include "StructuredDataI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

// it is a tree of AnyTypes
// overload of read and write to allow to store structured data leafs? (or we can use directly
// the current configuration)
// disallow the move
class RealTimeDataSource: public GlobalObjectI, public StructuredDataI {

public:

    static RealTimeDataSource *Instance();

    virtual const char8 * const GetClassName() const;

    /**
     * @brief Locks the shared semaphore.
     * @param[in] timeout maximum time to wait for the semaphore to be unlocked.
     * @return true if the shared semaphore is successfully locked.
     */
    bool Lock(const TimeoutType &timeout);

    /**
     * @brief Unlocks the shared semaphore.
     * @return true if the shared semaphore is successfully unlocked.
     */
    void Unlock();


private:
    RealTimeDataSource();

    /**
     * @brief Disallow the usage of new.
     * @param[in] size the size of the object.
     */
    static void *operator new(osulong size) throw ();
    /**
     * The shared mutex semaphore.
     */
    FastPollingMutexSem mux;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEDATASOURCE_H_ */

