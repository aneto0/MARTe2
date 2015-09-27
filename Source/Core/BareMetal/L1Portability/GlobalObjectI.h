/**
 * @file GlobalObjectI.h
 * @brief Header file for class GlobalObjectI
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

 * @details This header file contains the declaration of the class GlobalObjectI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GLOBALOBJECTI_H_
#define GLOBALOBJECTI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Interface definition for all the framework global objects.
 * @details All the framework global objects (singletons included) shall
 * implement this interface and register in the GlobalObjectsDatabase by calling
 * the GlobalObjectsDatabase->Add method.
 */
class GlobalObjectI {
public:
    /**
     * @brief Destructor. NOOP
     */
    virtual ~GlobalObjectI();

    /**
     * @brief Returns the name of the class.
     * @return the class name.
     */
    virtual const char8 * const GetClassName() const = 0;

    /**
     * @brief Guarantees that the class is instantiated in the StandardHeap.
     * @return a new instance of the class allocated in the StandardHeap.
     */
    static void * operator new(const osulong size);
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GLOBALOBJECTI_H_ */

