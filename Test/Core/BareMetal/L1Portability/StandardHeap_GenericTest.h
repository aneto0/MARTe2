/**
 * @file StandardHeap_GenericTest.h
 * @brief Header file for class StandardHeap_GenericTest
 * @date 25/08/2015
 * @author Llorenç Capellà
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

 * @details This header file contains the declaration of the class StandardHeap_GenericTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_L0PORTABILITY_STANDARDHEAP_GENERICTEST_H_
#define TEST_CORE_L0PORTABILITY_STANDARDHEAP_GENERICTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "HeapI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


using namespace MARTe;

/**
 * @brief A class to test the StandarHeap_GenericTest functions.
 */
class StandardHeap_GenericTest {
public:
    StandardHeap_GenericTest();
    virtual ~StandardHeap_GenericTest();

    bool retVal;

    void *ptr;
    int32 *intPtr;

    /**
     * @brief Test the StandardHeap default constructor
     * @return true if the constructor initialize the  firstAddress = 0 and lastAddres = 0
     */
    bool TestConstructor();

    /**
     * @brief Test that StandardHeap::Malloc does not fail.
     * @return true if the function is different to the NULL pointer.
     */
    bool TestMalloc();

    /**
     * @brief Test that StandardHeap::Malloc set the correct firstAdress
     * @return true if the firstAdress is the same that the pointer given by Malloc function.
     */
    bool TestMallocFirstAdress();

    /**
     * @brief Test that StandardHeap::Malloc set the correct lastAdress
     * @return true if the lasttAdress is the same that the pointer given by Malloc function plus size.
     */
    bool TestMallocLastAdress();

    /**
     * @brief Test that StandardHeap::Malloc returns a pointer to valid memory region
     * @detail allocate some memory and then tries write to and read from this allocate region of memory
     * @return true if the values read are equals to the values written.
     */
    bool TestMallocReadWrite();

    /**
     * @brief Test that StandardHeap::Free.
     * @detail allocate some memory checking NO NULL pointer is returned and then it is freed.
     * @return true if free function set the pointer to NULL
     */
    bool TestFree();

    //bool TestRealloc();

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_L0PORTABILITY_STANDARDHEAP_GENERICTEST_H_ */

