/**
 * @file Fnv1aHashFunctionGTest.cpp
 * @brief Source file for class Fnv1aHashFunctionGTest
 * @date 19/08/2019
 * @author Giuseppe Ferro
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
 * the class Fnv1aHashFunctionGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "Fnv1aHashFunctionTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

TEST(BareMetal_L0Types_Fnv1aHashFunctionGTest,TestConstructor) {
    Fnv1aHashFunctionTest  test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L0Types_Fnv1aHashFunctionGTest,TestCompute) {
    Fnv1aHashFunctionTest  test;
    ASSERT_TRUE(test.TestCompute("ciao", 0, 4153443133u));
}

TEST(BareMetal_L0Types_Fnv1aHashFunctionGTest,TestComputeSize) {
    Fnv1aHashFunctionTest  test;
    ASSERT_TRUE(test.TestCompute("ciao", 4, 4153443133u));
}

TEST(BareMetal_L0Types_Fnv1aHashFunctionGTest,TestComputeLessSize) {
    Fnv1aHashFunctionTest  test;
    ASSERT_TRUE(test.TestCompute("ciao", 3, 3783764352u));
}
