/**
 * @file StructuredDataI.h
 * @brief Header file for class StructuredDataI
 * @date 04/08/2015
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class StructuredDataI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONFIGURATION_DATABASE_H_
#define CONFIGURATION_DATABASE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief TODO
 * @details TODO
 */
class StructuredDataI {
public:
    /**
     * TODO
     */
    virtual bool Read(const char * const name, const AnyType &value) = 0;

    /**
     * TODO
     */
    virtual bool Write(const char * const name, const AnyType &value) = 0;

    /**
     * TODO
     */
    virtual bool Copy(StructuredDataI &destination, bool fromRoot) = 0;

    /**
     * TODO
     */
    virtual bool MoveToAncestor(uint32 generations) = 0;

    /**
     * TODO. Delete empty nodes?
     */
    virtual bool Move(const char * const path, bool relative) = 0;

    /**
     * TODO
     */
    virtual bool CreateNodes(const char * const path, bool relative) = 0;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
}
#endif /* CONFIGURATION_DATABASE_H_ */

