/**
 * @file CircularStaticListTest.h
 * @brief Header file for class CircularStaticListTest
 * @date 18/04/2017
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

 * @details This header file contains the declaration of the class BufferTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CIRCULARSTATICLISTTEST_H_
#define CIRCULARSTATICLISTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CircularStaticList.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Template implementation of CircularBuffer test.
 */
template<typename Type>
class CircularStaticListTest {

public:

    /**
     * @brief Tests the PushData method.
     */
    bool TestPushData(const uint32 bufferSize);

    /**
     * @brief Tests the PushData method saving the removed element.
     */
    bool TestPushData2(const uint32 bufferSize);

    /**
     * @brief Tests the GetLast method.
     */
    bool TestGetLast(const uint32 bufferSize);

    /**
     * @brief Tests the GetIndex method.
     */
    uint32 TestGetIndex(const uint32 bufferSize);

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {

template<typename Type>
bool CircularStaticListTest<Type>::TestPushData(const uint32 bufferSize) {
    CircularStaticList<Type> b(bufferSize);
    Type elementIn;
    Type elementOut;
    bool ok = (true);
    uint32 i = 0u;
    uint32 j = 0u;
    uint32 testSize = sizeof(elementIn);
    while ((ok) && (i < 2 * bufferSize)) {
        ok = b.PushData(elementIn);
        if (ok) {
            ok = b.Peek(i % bufferSize, elementOut);
            char * elementOut_p = (char *) &elementOut;
            char * elementIn_p = (char *) &elementIn;
            if (ok) {
                for (j = 0; (j < testSize) && (ok); j++) {
                    ok = (*elementOut_p == *elementIn_p);
                    elementOut_p++;
                    elementIn_p++;
                }
            }
        }
        i++;
    }
    return ok;
}

template<typename Type>
bool CircularStaticListTest<Type>::TestPushData2(const uint32 bufferSize) {
    CircularStaticList<Type> b(bufferSize);
    Type elementIn;
    Type elementOut;
    bool ok = (true);
    uint32 i = 0u;
    uint32 j = 0u;
    uint32 testSize = sizeof(elementIn);
    while ((ok) && (i < 2 * bufferSize)) {
        ok = b.PushData(elementIn, elementOut);
        char * elementOut_p = (char *) &elementOut;
        char * elementIn_p = (char *) &elementIn;
        if ((ok) && (i >= bufferSize)) {
            for (j = 0; (j < testSize) && (ok); j++) {
                ok = (*elementOut_p == *elementIn_p);
                elementOut_p++;
                elementIn_p++;
            }
        }
        i++;
    }
    return ok;
}

template<typename Type>
bool CircularStaticListTest<Type>::TestGetLast(const uint32 bufferSize) {
    CircularStaticList<Type> b(bufferSize);
    Type elementIn;
    Type elementOut;
    bool ok = (true);
    uint32 i = 0u;
    uint32 j = 0u;
    uint32 testSize = sizeof(elementIn);
    while ((ok) && (i < 2 * bufferSize)) {
        ok = b.PushData(elementIn);
        if (ok) {
            ok = b.GetLast(elementOut);
            char * elementOut_p = (char *) &elementOut;
            char * elementIn_p = (char *) &elementIn;
            if ((ok) && (i >= bufferSize)) {
                for (j = 0; (j < testSize) && (ok); j++) {
                    ok = (*elementOut_p == *elementIn_p);
                    elementOut_p++;
                    elementIn_p++;
                }
            }

        }
        i++;
    }
    return ok;
}

template<typename Type>
uint32 CircularStaticListTest<Type>::TestGetIndex(const uint32 bufferSize) {
    CircularStaticList<Type> b(bufferSize);
    bool ok = (true);
    uint32 i = 0u;
    while ((ok) && (i < 2 * bufferSize)) {
        Type elementIn;
        if (ok) {
            ok = (b.GetIndex() == (i % bufferSize));
        }
        ok = b.PushData(elementIn);
        i++;
    }
    return ok;
}

}
#endif /* CIRCULARSTATICLISTTEST_H_ */

