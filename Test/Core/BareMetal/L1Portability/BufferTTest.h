/**
 * @file BufferTTest.h
 * @brief Header file for class BufferTTest
 * @date 11/04/2017
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

#ifndef BUFFERTTEST_H_
#define BUFFERTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BufferT.h"
#include "FractionalInteger.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe{

/**
 * @brief Template implementation of Buffer test.
 */
template <typename Type>
class BufferTTest{

public:

    /**
     * @brief Tests the BufferT constructor.
     */
    bool TestConstructor(const uint32 bufferSize);

    /**
     * @brief Tests the BufferT initializer function.
     */
    bool TestInitialise(const uint32 bufferSize);


    /**
     * @brief Tests the BufferT size getter.
     */
    bool TestGetSize(const uint32 bufferSize);


    /**
     * @brief Tests the BufferT getter.
     */
    bool TestGetData(const uint32 bufferSize);


    /**
     * @brief Tests the BufferT putter.
     */
    bool TestPutData(const uint32 bufferSize);

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {

template <typename Type>
bool BufferTTest<Type>::TestConstructor(const uint32 bufferSize) {
    BufferT<Type> b(bufferSize);
    bool ok = (b.GetSize() == bufferSize);
    return ok;
}

template <typename Type>
bool BufferTTest<Type>::TestInitialise(const uint32 bufferSize) {
    BufferT<Type> b(bufferSize);
    Type val;
    bool ok = b.Initialise(val);
    return ok;
}


template <typename Type>
bool BufferTTest<Type>::TestGetSize(const uint32 bufferSize) {
    Type val;
    BufferT<Type> b(bufferSize);
    bool ok = b.Initialise(val);
    if (ok) {
        ok = (b.GetSize() == bufferSize);
    }
    return ok;
}

template <typename Type>
bool BufferTTest<Type>::TestGetData(const uint32 bufferSize) {
    Type val;
    Type test;
    Type * test_p = &test;
    BufferT<Type> b(bufferSize);
    bool ok = b.Initialise(val);
    uint64 i = 0;
    uint64 j = 0;
    uint64 testSize = sizeof(val);
    while ((ok) && (i < bufferSize)) {
        ok = b.GetData(test_p, i);
        char * test_p2 = (char *) &test;
        char * val_p2 = (char *) &val;
        if(ok){
            ok=(testSize == sizeof(test));
            for(j=0;(j< testSize) && (ok); j++){
                ok=(*test_p2==*val_p2);
                test_p2++;
                val_p2++;
            }
        }
        i++;
    }
    return ok;
}


template <typename Type>
bool BufferTTest<Type>::TestPutData(const uint32 bufferSize) {
    Type val;
    Type testArray[bufferSize];
    BufferT<Type> b(bufferSize);
    bool ok = b.Initialise(val);
    uint64 i = 0u;
    while ((ok) && (i < bufferSize)) {
        ok = b.PutData(testArray[i], i);
        i++;
    }
    return ok;
}

}
#endif /* BUFFERTTEST_H_ */
	
