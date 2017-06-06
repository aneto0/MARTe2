/**
 * @file BufferTest.cpp
 * @brief Source file for class BufferTest
 * @date 06/04/2017
 * @author Andre' Torres
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
 * the class BufferTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BufferTest.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

class TestClassForBuffer{
public:
    uint64 a;
};


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


bool BufferTest::TestConstructor_BasicType(const uint32 bufferSize) {
    uint64 val = 10u;
    Buffer b(bufferSize, sizeof(val));
    bool ok = (b.GetSize() == bufferSize);
    return ok;
}

bool BufferTest::TestConstructor_ComplexType(const uint32 bufferSize) {
    TestClassForBuffer element;
    Buffer b(bufferSize, sizeof(element));
    bool ok = (b.GetSize() == bufferSize);
    return ok;
}

bool BufferTest::TestInitialise_BasicType(const uint32 bufferSize) {
    uint64 val = 10u;
    Buffer b(bufferSize, sizeof(val));
    bool ok = b.Initialise(reinterpret_cast<char8 *>(&val));
    return ok;
}

bool BufferTest::TestInitialise_ComplexType(const uint32 bufferSize) {
    TestClassForBuffer element;
    Buffer b(bufferSize, sizeof(element));
    bool ok = b.Initialise(reinterpret_cast<char8 *>(&element));
    return ok;
}

bool BufferTest::TestGetSize_BasicType(const uint32 bufferSize) {
    uint64 val = 10u;
    Buffer b(bufferSize, sizeof(val));
    bool ok = b.Initialise(reinterpret_cast<char8 *>(&val));
    if (ok) {
        ok = (b.GetSize() == bufferSize);
    }
    return ok;
}

bool BufferTest::TestGetSize_ComplexType(const uint32 bufferSize) {
    TestClassForBuffer element;
    Buffer b(bufferSize, sizeof(element));
    bool ok = b.Initialise(reinterpret_cast<char8 *>(&element));
    if (ok) {
        ok = (b.GetSize() == bufferSize);
    }
    return ok;
}

bool BufferTest::TestGetData_BasicType(const uint32 bufferSize) {
    uint64 val = 9u;
    uint64 test = 10u;
    uint64 * test_p = &test;
    Buffer b(bufferSize, static_cast<uint32>(sizeof(val)));
    bool ok = b.Initialise(reinterpret_cast<char8 *>(&val));
    uint64 i = 0;
    while ((ok) && (i < bufferSize)) {
        ok = b.GetData(reinterpret_cast<char8 *&>(test_p), i);
        if(ok){
            ok=(test == 9u);
        }
        i++;
    }
    return ok;
}

bool BufferTest::TestGetData_ComplexType(const uint32 bufferSize) {
    TestClassForBuffer element0;
    element0.a=10u;
    TestClassForBuffer element1;
    TestClassForBuffer* test_p =&element1;
    Buffer b(bufferSize, sizeof(TestClassForBuffer));
    bool ok = b.Initialise(reinterpret_cast<char8 *>(&element0));
    uint64 i = 0;
    while ((ok) && (i < bufferSize)) {
        ok = b.GetData(reinterpret_cast<char8 *&>(test_p), i);
        if(ok){
            ok=(element1.a == element0.a);
        }
        i++;
    }
    return ok;
}

bool BufferTest::TestPutData_BasicType(const uint32 bufferSize) {
    uint64 val = 10u;
    Buffer b(bufferSize, sizeof(val));
    bool ok = b.Initialise(reinterpret_cast<char8 *>(&val));
    uint64 i = 0u;
    while ((ok) && (i < bufferSize)) {
        ok = b.PutData(reinterpret_cast<char8 *>(&i), i);
        i++;
    }
    return ok;
}

bool BufferTest::TestPutData_ComplexType(const uint32 bufferSize) {
    TestClassForBuffer element0;
    Buffer b(bufferSize, sizeof(TestClassForBuffer));
    bool ok = b.Initialise(reinterpret_cast<char8 *>(&element0));
    uint64 i = 0u;
    while ((ok) && (i < bufferSize)) {
        element0.a=i;
        ok = b.PutData(reinterpret_cast<char8 *>(&element0), i);
        i++;
    }
    return ok;
}
