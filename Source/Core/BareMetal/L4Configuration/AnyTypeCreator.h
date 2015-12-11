/**
 * @file AnyTypeCreator.h
 * @brief Header file for class AnyTypeCreator
 * @date 07/12/2015
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

 * @details This header file contains the declaration of the class AnyTypeCreator
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ANYTYPECREATOR_H_
#define ANYTYPECREATOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StaticListHolder.h"
#include "AnyType.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*lint -save -e754 -e750 -e753*/
namespace MARTe {

class AnyTypeCreator {

public:

    AnyTypeCreator(const uint32 granularityIn = 1u);

    bool SetType(AnyType &element,
                 const uint8 nOfDimensions,
                 const uint32 dimensionSize[3]) const;

    bool ToType(const char8 * const type,
                const char8 * const data);

    ~AnyTypeCreator();

    void CleanUp(const uint32 granularityIn);

    uint32 GetSize() const;

    uint32 GetGranularity() const;

private:
    StaticListHolder *memory;
    uint32 granularity;
    uint32 typeIndex;

};

const char8 * const defaultTypeName = "string";

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ANYTYPECREATOR_H_ */

