/**
 * @file GAMContextManager.h
 * @brief Header file for class GAMContextManager
 * @date 23/02/2016
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

 * @details This header file contains the declaration of the class GAMContextManager
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMCONTEXTMANAGER_H_
#define GAMCONTEXTMANAGER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe{
class GAMContextManager: public Object {
public:
    GAMContextManager();

    virtual bool Initialise(StructuredDataI & data);

    // call GAMContextContainer::Validate initialising itself
    bool Validate();

    bool AddContext(ReferenceT<GAMContext> newContext);

    // triggered by the scheduler
    void SwitchToNext();


private:
    StreamString contextContainerPath;

    MemoryArea contextMemory;

    NamedPointer* items;

    uint32 numberOfItems;

    Reference<GAMContext> *contexts;

    uint32 numberOfContexts;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMCONTEXTMANAGER_H_ */

