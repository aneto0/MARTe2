/**
 * @file Fnv1aHashFunctionTest.cpp
 * @brief Source file for class Fnv1aHashFunctionTest
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
 * the class Fnv1aHashFunctionTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Fnv1aHashFunctionTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

Fnv1aHashFunctionTest::Fnv1aHashFunctionTest() {
}

Fnv1aHashFunctionTest::~Fnv1aHashFunctionTest() {
}

bool Fnv1aHashFunctionTest::TestConstructor() {
    Fnv1aHashFunction *test = new Fnv1aHashFunction;
    bool ret = (test != NULL);
    delete test;
    return ret;
}

bool Fnv1aHashFunctionTest::TestCompute(const char8 *in, uint32 size, uint32 result) {
    Fnv1aHashFunction test;
    return (test.Compute(in, size)==result);
}

}

