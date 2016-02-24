/**
 * @file GAMContext.h
 * @brief Header file for class GAMContext
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

 * @details This header file contains the declaration of the class GAMContext
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMCONTEXT_H_
#define GAMCONTEXT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "MemoryArea.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe{



class NamedPointer {
public:
    void * pointer;
    StreamString type;
    StreamString name;
    uint32 size;
};

class GAMContext: public Object {

public:

    GAMContext();

    bool Read(const char8* name, void * output);

    bool Write(const char8* name, void * input);

    // initialises the pointer table and add itself to the gam manager
    bool Validate(GAMContextContainer &contextContainer);

    // gets the array of path to the memory
    virtual bool Initialise(StructuredDataI &data);

private:

    NamedPointer* items;

    uint32 numberOfItems;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMCONTEXT_H_ */

