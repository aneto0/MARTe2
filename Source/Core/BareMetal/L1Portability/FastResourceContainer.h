/**
 * @file FastResourceContainer.h
 * @brief Header file for class FastResourceContainer
 * @date 05/11/2016
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class FastResourceContainer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FASTRESOURCECONTAINER_H_
#define FASTRESOURCECONTAINER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Atomic.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief A container of resources. It allows taking and releasing resource,, in any order,
 * and concurrently by any number of tasks, interrupts and processors.
 */
class FastResourceContainer {
public:
    /**
     * @brief Constructor. Creates a container capable of holding \a nOfElements resources.
     * @param[in] nOfElements number of resources that can be managed by this container.
     * @param[in] taken if true all the resources will be marked as taken.
     */
    FastResourceContainer(uint32 nOfElements, bool taken);

    /**
     * @brief Frees the memory that was dynamically allocated to manage these resources.
     */
    ~FastResourceContainer();

    /**
     * @brief Gets the number of free resources.
     * @return the number of free resources.
     */
    uint32 GetSize() const;

    /**
     * @brief Takes a free resource and marks it as in use.
     * @return the position (< GetSize()) of the resource that was taken.
     * If all the resources are taken it returns 0xFFFFFFFFu
     */
    uint32 Take();

    /**
     * @brief Returns a free resource and marks it as free to be taken.
     * @param[in] pos position (< GetSize()) of the resource to be returned.
     */
    void Return(uint32 pos);

private:
    /**
     * Do not allow assignment nor copy construction of this class, otherwise the management
     * of the buffers would not be properly managed.
     */
    /*lint -e{1704} private copy constructor to avoid assignment of this class*/
    FastResourceContainer(const FastResourceContainer &copy);
    FastResourceContainer & operator =(const FastResourceContainer &copy);

    /**
     * @brief describes a free resource.
     */
    struct FastResourceContainerData {
        /**
         * Amount of buffer used.
         */
        volatile int32 size;
        /**
         * Semaphore that locks the resource.
         */
        volatile int32 sem;
    };

    /**
     * List of FastResourceContainerData.
     */
    FastResourceContainerData *buffers;

    /**
     * The number of FastResourceContainerData elements.
     */
    uint32 treeSize;

    /**
     * Number equal to half of the FastResourceContainerData elements.
     */
    uint32 treeHalf;

    /*lint -e{1712} This class does not have a default constructor because
     * the nOfElements must be defined on construction and remain constant
     * during object's lifetime*/
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FASTRESOURCECONTAINER_H_ */

