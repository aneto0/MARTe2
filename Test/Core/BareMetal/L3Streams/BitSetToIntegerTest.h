/**
 * @file BitSetToIntegerTest.h
 * @brief Header file for class BitSetToIntegerTest
 * @date 21/10/2015
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

 * @details This header file contains the declaration of the class BitSetToIntegerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BITSETTOINTEGERTEST_H_
#define BITSETTOINTEGERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "BitSetToInteger.h"
#include "StreamTestHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class BitSetToIntegerTest {

private:

public:

    /**
     * @brief Tests the conversion form BitSet to BitSet
     */
    template<typename T>
    bool TestBitSetToBitSet();

    /**
     * @brief Tests the conversion from BitSet to 8-bits integer.
     */
    bool TestBitSetToInteger_8();

    /**
     * @brief Tests the conversion from BitSet to 16-bits integer.
     */
    bool TestBitSetToInteger_16();

    /**
     * @brief Tests the conversion from BitSet to 32-bits integer.
     */
    bool TestBitSetToInteger_32();

    /**
     * @brief Tests the conversion from 8-bits integer to BitSet.
     */
    bool TestIntegerToBitSet_8();

    /**
     * @brief Tests the conversion from 16-bits integer to BitSet.
     */
    bool TestIntegerToBitSet_16();

    /**
     * @brief Tests the conversion from 32-bits integer to BitSet.
     */
    bool TestIntegerToBitSet_32();
};

template<typename T>
bool BitSetToIntegerTest::TestBitSetToBitSet() {

    uint32 dest[] = { 0, 0, 0, 0 };
    const BitSetToBitSetTableRow *table = GeneratedBitSetToBitSetTable();

    uint32 i = 0;
    while (table[i].sPointer != NULL) {
        T *destPointer = (T*) dest;
        uint8 destShift = table[i].destShift;
        uint8 destSize = table[i].destSize;
        bool destIsSigned = table[i].destIsSigned;
        uint8 sourceShift = table[i].sourceShift;
        uint8 sourceSize = table[i].sourceSize;
        bool sourceIsSigned = table[i].sourceIsSigned;
        T* source = (T*) (table[i].sPointer);
        uint32 typeSize = sizeof(T) * 8;
        T* newSource = source + (sourceShift / typeSize);
        uint32 newSourceShift = sourceSize + (sourceShift % typeSize);

        T* newDest = destPointer + (destShift / typeSize);
        uint32 newDestShift = destSize + (destShift % typeSize);

        BitSetToBitSet(destPointer, destShift, destSize, destIsSigned, source, sourceShift, sourceSize, sourceIsSigned);

        if (dest[table[i].destArrayIndex] != table[i].result) {
            return false;
        }

        if (source != newSource) {
            return false;
        }

        if (sourceShift != newSourceShift) {
            return false;
        }
        if (destPointer != newDest) {
            return false;
        }

        if (destShift != newDestShift) {
            return false;
        }

        i++;
    }
    return true;

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BITSETTOINTEGERTEST_H_ */

