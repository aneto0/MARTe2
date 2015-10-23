/**
 * @file CharBufferTest.h
 * @brief Header file for class CharBufferTest
 * @date 08/10/2015
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

 * @details This header file contains the declaration of the class CharBufferTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CHARBUFFERTEST_H_
#define CHARBUFFERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CharBuffer.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class CharBufferTest {
public:

    /**
     * @brief Tests the default constructor checking if the attributes are initialized properly.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the constructor by granularity checking that the allocation granularity is
     * rounded at the first minor power of two.
     */
    bool TestGranularityConstructor(uint32 allocationGranularity,
                                    uint32 expected);

    /**
     * @brief Tests if the destructor resets the attributes.
     */
    bool TestDestructor();

    /**
     * @brief Tests if the memory allocated is the first greater granularity multiple of the desired size to allocate.
     */
    bool TestSetBufferSize(uint32 size,
                           uint32 granularity,
                           bool expected);

    /**
     * @brief Tests if the buffer in CharBuffer is set correctly by pre existent memory reference.
     */
    bool TestSetBufferReferenceConst(const char8* bufferIn,
                                     uint32 size);

    /**
     * @brief Tests if the buffer in CharBuffer is set correctly by pre existent memory reference.
     */
    bool TestSetBufferReferenceNonConst(const char8* bufferIn,
                                        uint32 size);

    /**
     * @brief Tests if the correct buffer is returned.
     */
    bool TestBuffer();

    /**
     * @brief Tests if the buffer with write access is returned.
     */
    bool TestBufferReference();

    /**
     * @brief Tests if the size returned is the same specified by SetBufferReference function.
     */
    bool TestSize_StaticAllocation(uint32 size);

    /**
     * @brief Tests if the size returned is the first granularity multiple greater than size.
     */
    bool TestSize_DynamicAllocation(uint32 size,
                                    uint32 granularity);

    /**
     * @brief Tests if the function returns true only when the buffer is allocated dynamically.
     */
    bool TestIsAllocated();

    /**
     * @brief Tests if the function returns the first power of two minor than the granularity specified in the constructor.
     */
    bool TestAllocationGranularity(uint32 allocationGranularity,
                                   uint32 expected);

    /**
     * @brief Tests if the function returns false if the memory is initialized by const char pre existent reference
     * and true when it is initialized dynamically or by pre existent char*.
     */
    bool TestCanWrite();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CHARBUFFERTEST_H_ */

