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

namespace MARTe {

/**
 * @brief Creates a new AnyType object which could be a scalar, vector or matrix.
 */
class DLL_API AnyTypeCreator {

public:

    /**
     * @brief Constructor which initializes the instance with a given granularity.
     * @param[in] granularityIn is the granularity of the allocated memory (the
     * number of new elements which could be added after a reallocation).
     * @post
     *   GetGranularity() == granularityIn
     */
    AnyTypeCreator(const uint32 granularityIn = 1u);

    /**
     * @brief Creates the AnyType.
     * @param[in] nOfDimensions specifies if the type is a scalar, vector or matrix.
     * @param[in] dimensionSize specifies the number of elements for each dimension.
     * @return the AnyType created using the parameters in input and the internal allocated memory.
     * voidAnyType is returned in case of input parameters inconsistency.
     * @pre
     *   nOfDimensions < 3 &&
     *   nOfDimensions == 0 => dimensionSize[0:2] == 1 &&
     *   nOfDimensions == 1 => dimensionSize[0] >= 1 dimensionSize[1:2] == 1 &&
     *   nOfDimensions == 1 => dimensionSize[0:1] >= 1 dimensionSize[2] == 1
     */
    AnyType Create(const uint8 nOfDimensions,
                   const uint32 dimensionSize[3]) const;

    /**
     * @brief Adds an element to the memory converting it from a string token.
     * @details If the type is unrecognized, the default type "string" will be considered.
     * @param[in] type is the desired type for the element to be added.
     * @param[in] data is the element in string format which has to be converted.
     * @return false if in the memory there are already element with a different type or if
     * the conversion fails. True otherwise.
     */
    bool Add(const char8 * const type,
             const char8 * const data);

    /**
     * @brief Destructor.
     */
    ~AnyTypeCreator();

    /**
     * @brief Frees the memory and sets the new granularity.
     * @param[in] granularityIn is the granularity for the new memory allocation.
     * @post
     *   GetGranularity() == granularityIn.
     */
    void CleanUp(const uint32 granularityIn);

    /**
     * @brief Retrieves how many elements currently are in the memory.
     * @return The current number of elements in the memory
     */
    uint32 GetSize() const;

    /**
     * @brief Retrieves the memory allocation granularity.
     * @return The memory allocation granularity.
     */
    uint32 GetGranularity() const;

private:

    /**
     * The AnyType memory.
     */
    StaticListHolder *memory;

    /**
     * The memory allocation granularity.
     */
    uint32 granularity;

    /**
     * The type identifier.
     */
    uint32 typeIndex;

};

/**
 * The default type (C-String)
 */
const char8 * const defaultTypeName = "string";

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ANYTYPECREATOR_H_ */

