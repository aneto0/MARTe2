/**
 * @file GlobalObjectsDatabase.h
 * @brief Header file for class GlobalObjectsDatabase
 * @date 25/09/2015
 * @author Andre' Neto
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
 *
 * @details This header file contains the declaration of the class GlobalObjectsDatabase
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GLOBALOBJECTSDATABASE_H_
#define GLOBALOBJECTSDATABASE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "GlobalObjectI.h"

#include INCLUDE_FILE_ENVIRONMENT(BareMetal,L1Portability,ENVIRONMENT,StandardHeap.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    /**
     * @brief Global objects (singletons) manager.
     * @details This class aims at addressing the static objects initialisation
     * and destruction order fiasco. The only static global object that should exist in the framework is
     * this GlobalObjectsDatabase. By registering in this database all the MARTe global objects, the destruction
     * order of such objects can be guaranteed.
     *
     * The global objects (which inherit from GlobalObjectI) are created in the StandardHeap and
     * registered by the caller in the GlobalObjectsDatabase.
     */
    class DLL_API GlobalObjectsDatabase {

    public:

        /**
         * @brief Singleton access to the GlobalObjectsDatabase.
         * @return a pointer to the GlobalObjectsDatabase.
         */
        static GlobalObjectsDatabase *Instance();

        /**
         * @brief Adds a new global object to the database.
         * @details At the end of the program, the registered objects will be
         * destroyed in ascending order of the destructionOrder. If two or more
         * objects have the same order, they will be destroyed following the
         * order of addition.
         * @param[in] globalObject a pointer to the global object to be added.
         * @param[in] destructionOrder the order at which the global object
         * should be destroyed. The higher the order, the later it will be
         * destroyed w.r.t. to the other global objects.
         * @pre
         *   globalObject!=NULL &&
         *   destructionOrder>=0 &&
         *   destructionOrder<NUMBER_OF_GLOBAL_OBJECTS
         */
        void Add(const GlobalObjectI * const globalObject,
                const uint32 destructionOrder);

        /**
         * @brief Gets a pointer to the GlobalObject in position \a idx.
         * @param[in] idx the index of the object to be peeked.
         * @pre
         *   idx>=0 &&
         *   idx<NUMBER_OF_GLOBAL_OBJECTS
         * @return a pointer to the GlobalObject in position \a idx or NULL if no object is registered in position \a idx.
         */
        const GlobalObjectI *Peek(const uint32 idx) const;

        /**
         * @brief Returns a pointer to the StandardHeap.
         * @return a pointer to the StandardHeap.
         */
        HeapI *GetStandardHeap();

    private:

        /**
         * @brief Sets all the internal database entries to NULL.
         */
        /*lint -e{1704} private constructor for singleton implementation*/
        GlobalObjectsDatabase();

        /**
         * @brief Deletes all the registered GlobalObjectI in ascending order of destructionOrder.
         */
        ~GlobalObjectsDatabase();

        /**
         * Stores a pointer to the global objects.
         */
        const GlobalObjectI *databaseList[NUMBER_OF_GLOBAL_OBJECTS];

        /**
         * Stores the destruction order of the global objects.
         */
        uint32 destructionListOrder[NUMBER_OF_GLOBAL_OBJECTS];

        /**
         * Unique instance in the framework of the StandardHeap.
         */
        StandardHeap standardHeap;
    };

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GLOBALOBJECTSDATABASE_H_ */

