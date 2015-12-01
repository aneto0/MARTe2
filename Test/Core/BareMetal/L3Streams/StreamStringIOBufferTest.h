/**
 * @file StreamStringIOBufferTest.h
 * @brief Header file for class StreamStringIOBufferTest
 * @date 26/10/2015
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

 * @details This header file contains the declaration of the class StreamStringIOBufferTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRINGIOBUFFERTEST_H_
#define STRINGIOBUFFERTEST_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamStringIOBuffer.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

class StreamStringIOBufferTest {

public:

    /**
     * @brief Tests if the default constructor initializes position=0 and used size=0. The granularity should be 64 by default.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests if the granularity is initialized to the first 2**x minor than the specified value.
     */
    bool TestGranularityConstructor(uint32 granularity,
                                    uint32 expected);

    /**
     * @brief Tests if the constructor set the buffer pointer to null.
     */
    bool TestDestructor();

    /**
     * @brief Tests if the function allocates the first multiple of granularity greater than the specified size.
     */
    bool TestSetBufferAllocationSize(uint32 size,
                                     uint32 granularity);

    /**
     * @brief Tests if the function writes all the string passed by argument on the buffer.
     */
    bool TestWrite(uint32 writeSize,
                   const char8 *string);

    /**
     * @brief Tests if the function return false passing a null input.
     */
    bool TestWrite_NULL_Buffer();

    /**
     * @brief Tests if the function puts a '\0' at the end of the filled memory.
     */
    bool TestTerminate(uint32 writeSize,
                       const char8 *string);

    /**
     * @brief Tests if the function writes all the passed string on the buffer.
     */
    bool TestWriteAll(uint32 writeSize,
                      const char8 *string);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STRINGIOBUFFERTEST_H_ */

