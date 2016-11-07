/**
 * @file FastResourceContainer.h
 * @brief Header file for class FastResourceContainer
 * @date 05/11/2016
 * @author Andre'Neto
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
 * @brief TODO
 */
class FastResourceContainer {
public:
    /**
     * TODO
     */
    FastResourceContainer(uint32 nOfElements,
                          bool taken = false);

    /**
     * TODO
     */
    ~FastResourceContainer();

    /**
     * TODO
     */
    uint32 GetSize() const;

    /**
     * TODO
     */
    uint32 Take();

    /**
     * TODO
     */
    void Return(uint32 pos);

private:
    /**
     * TODO
     */
    struct FastResourceContainerData {
        /**
         * Amount of buffer used.
         */
        volatile int32 size;
        /**
         * Amount of buffer used.
         */
        volatile int32 sem;
    };

    /**
     * TODO
     */
    FastResourceContainerData *buffers;

    /**
     * TODO
     */
    uint32 treeSize;

    /**
     * TODO
     */
    uint32 treeHalf;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FASTRESOURCECONTAINER_H_ */

