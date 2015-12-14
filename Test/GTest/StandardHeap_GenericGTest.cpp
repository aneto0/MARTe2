/**
 * @file StandardHeap_GenericGTest.cpp
 * @brief Source file for class StandardHeap_GenericGTest
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

 * @details This source file contains the definition of all the methods for
 * the class StandardHeap_GenericGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "StandardHeap_GenericTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(StandardHeap_GenericGTest,TestConstructor) {
    StandardHeap_GenericTest standardHeap_GenericGTest;
    ASSERT_TRUE(standardHeap_GenericGTest.TestConstructor());
}

TEST(StandardHeap_GenericGTest,TestMalloc) {
    StandardHeap_GenericTest standardHeap_GenericGTest;
    ASSERT_TRUE(standardHeap_GenericGTest.TestMalloc());
}

TEST(StandardHeap_GenericGTest,TestMallocFirstAdress) {
    StandardHeap_GenericTest standardHeap_GenericGTest;
    ASSERT_TRUE(standardHeap_GenericGTest.TestMallocFirstAdress());
}

TEST(StandardHeap_GenericGTest,TestMallocLastAdress) {
    StandardHeap_GenericTest standardHeap_GenericGTest;
    ASSERT_TRUE(standardHeap_GenericGTest.TestMallocLastAdress());
}

TEST(StandardHeap_GenericGTest,TestMallocReadWrite) {
    StandardHeap_GenericTest standardHeap_GenericGTest;
    ASSERT_TRUE(standardHeap_GenericGTest.TestMallocReadWrite());
}

TEST(StandardHeap_GenericGTest,TestFree) {
    StandardHeap_GenericTest standardHeap_GenericGTest;
    ASSERT_TRUE(standardHeap_GenericGTest.TestFree());
}

	
