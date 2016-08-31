/**
 * @file MultiThreadService.h
 * @brief Header file for class MultiThreadServerClass
 * @date Aug 30, 2016
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

 * @details This header file contains the declaration of the class MultiThreadServerClass
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4MESSAGES_MULTITHREADSERVICE_H_
#define L4MESSAGES_MULTITHREADSERVICE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EmbeddedThread.h"
#include "ReferenceContainer.h"

namespace MARTe{

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Contains instances of specialised EmbeddedThreads
 *
 */
class MultiThreadService: public ReferenceContainer{

public:
    ///
    MultiThreadService(void * context, );

private:
    /// true means that the number of threads vary with the needs
    bool dynamicThreadAllocation;

    /// either the available working threads or the maximum
    uint32 numberOfThreads;
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L4MESSAGES_MULTITHREADSERVICE_H_ */
	
