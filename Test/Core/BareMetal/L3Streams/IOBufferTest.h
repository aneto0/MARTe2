/**
 * @file IOBufferTest.h
 * @brief Header file for class IOBufferTest
 * @date 09/10/2015
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

 * @details This header file contains the declaration of the class IOBufferTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef IOBUFFERTEST_H_
#define IOBUFFERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "IOBuffer.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;
class IOBufferTest {

public:
    bool TestDefaultConstructor();

    bool TestFullConstructor(uint32 undoLevel);

    /**
     * @brief Tests if the size returned is the first granularity multiple greater than size.
     */
    bool TestBufferSize(uint32 size,
                        uint32 granularity);

    bool TestMaxUsableAmount(uint32 size,
                             uint32 endSpace,
                             uint32 granularity);

    bool TestAmountLeft(uint32 size,
                        uint32 endSpace,
                        uint32 granularity);

    bool TestUsedAmountLeft(uint32 size,
                            uint32 endSpace,
                            uint32 granularity);

    bool TestBuffer();

    bool TestBufferReference();

    bool TestPosition();

    bool TestUsedSize(uint32 size,
                      uint32 endSpace,
                      uint32 granularity);

    bool TestAllocationGranularity(uint32 granularity,
                                   uint32 expected);

    bool TestUndoLevel(uint32 undoLevel);

    bool TestSetBufferHeapMemory(uint32 size,
                                 uint32 endSpace,
                                 uint32 granularity);

    bool TestSetBufferHeapMemoryIncrease();

    bool TestSetBufferHeapMemoryDecrease();

    bool TestSetBufferReadOnlyReferencedMemory();

    bool TestSetBufferReferencedMemory();

    bool TestResync();

    bool TestSeek(uint32 usedSize,
                  uint32 seek,
                  bool expected);

    bool TestRelativeSeek(uint32 initialPos,
                          int32 delta,
                          bool expected);

    bool TestPutC_Heap();

    bool TestPutC_MemoryReference();

    bool TestPutC_MemoryReferenceRO();

    bool TestGetC_Heap();

    bool TestGetC_MemoryReference();

    bool TestGetC_MemoryReferenceRO();

    bool TestUnPutC();

    bool TestUnGetC();

    bool TestSetUsedSize(uint32 allocatedSize, uint32 desiredSize);

    bool TestWrite_Heap(uint32 allocationSize, uint32 writeSize, const char8 *string);

    bool TestWrite_Memoryreference();

    bool TestWrite_MemoryreferenceRO();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* IOBUFFERTEST_H_ */

