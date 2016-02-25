/**
 * @file ObjectRegistryDatabase.h
 * @brief Header file for class ObjectRegistryDatabase
 * @date 18/02/2016
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

 * @details This header file contains the declaration of the class ObjectRegistryDatabase
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OBJECTREGISTRYDATABASE_H_
#define OBJECTREGISTRYDATABASE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "ReferenceT.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class DLL_API ObjectRegistryDatabase: public ReferenceContainer, public GlobalObjectI {

public:

    static ObjectRegistryDatabase *Instance();

    /**
     * @brief Destructor.
     */
    virtual ~ObjectRegistryDatabase();

    Reference Find(const char8 * const path,
                   const Reference current = Reference());

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

    bool Build(StructuredDataI &configuration);

    virtual const char8 * const GetClassName() const;

private:
    /**
     * @brief Disallow the usage of new.
     * @param[in] size the size of the object.
     */
    static void *operator new(osulong size) throw ();

    /**
     * @brief Default constructor.
     * Initialises the shared mutex Semaphore.
     * @post
     *   MoveToRoot() == true
     */
    ObjectRegistryDatabase();

    /**
     * The shared mutex semaphore.
     */
    FastPollingMutexSem mux;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OBJECTREGISTRYDATABASE_H_ */

