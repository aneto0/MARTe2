/**
 * @file BufferTest.h
 * @brief Header file for class BufferTest
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

 * @details This header file contains the declaration of the class BufferTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BUFFERTEST_H_
#define BUFFERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Buffer.h"
#include "CompilerTypes.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe{

/**
 * @brief Tests all the Buffer functions.
 */
class BufferTest {

public:

    /**
     * @brief Tests the Buffer constructor.
     * @return true if at the beginning the size of the buffer is zero, false otherwise.
     * @param[in] bufferSize is the size of the buffer to allocate memory.
     * @param[in] typeSize is the size of type of the buffer elements.
     */
    bool TestConstructor_BasicType(const uint32 bufferSize);

    /**
     * @brief Tests the Buffer constructor with a class.
     */
    bool TestConstructor_ComplexType(const uint32 bufferSize);

    /**
     * @brief Tests the Buffer initializer function with a basic type.
     */
    bool TestInitialise_BasicType(const uint32 bufferSize);

    /**
     * @brief Tests the Buffer initializer function with a class.
     */
    bool TestInitialise_ComplexType(const uint32 bufferSize);

    /**
     * @brief Tests the Buffer size getter with a basic type.
     */
    bool TestGetSize_BasicType(const uint32 bufferSize);

    /**
     * @brief Tests the Buffer size getter with a class.
     */
    bool TestGetSize_ComplexType(const uint32 bufferSize);

    /**
     * @brief Tests the Buffer getter with a basic type.
     */
    bool TestGetData_BasicType(const uint32 bufferSize);

    /**
     * @brief Tests the Buffer getter with a class.
     */
    bool TestGetData_ComplexType(const uint32 bufferSize);

    /**
     * @brief Tests the Buffer putter  with a basic type.
     */
    bool TestPutData_BasicType(const uint32 bufferSize);

    /**
     * @brief Tests the Buffer putter with a class.
     */
    bool TestPutData_ComplexType(const uint32 bufferSize);
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BUFFERTEST_H_ */
	
