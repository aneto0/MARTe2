/**
 * @file StreamMemoryReferenceTest.h
 * @brief Header file for class StreamMemoryReferenceTest
 * @date 19/10/2015
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

 * @details This header file contains the declaration of the class StreamMemoryReferenceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STREAMMEMORYREFERENCETEST_H_
#define STREAMMEMORYREFERENCETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StreamMemoryReference.h"
#include "StreamTestHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Test the StreamMemoryReference public methods.
 */
class StreamMemoryReferenceTest {

private:

public:

    /**
     * @brief Tests if the buffer is null at the beginning and if position and size are equal to zero.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests if the buffer is equal to the char* input and if position and size are zero.
     */
    bool TestConstructor_CharPointer();

    /**
     * @brief Tests if the buffer is equal to the const char* input and if position is zero and size is equal to the size in input.
     */
    bool TestConstructor_ConstCharPointer();

    /**
     * @brief Tests if the buffer is null after the destructor call.
     */
    bool TestDestructor();

    /**
     * @brief Tests if this stream is treated as a C String in the AnyType cast.
     */
    bool TestAnyTypeOperator(const char8* initializationString);

    /**
     * @brief Tests if the function returns false if the buffer is null, true if the buffer is a char*, false if the buffer is const char*.
     */
    bool TestCanWrite();

    /**
     * @brief Tests if the function returns false if the buffer is null, true otherwise.
     */
    bool TestCanRead();

    /**
     * @brief Tests if the function returns true.
     */
    bool TestCanSeek();

    /**
     * @brief Tests if the function writes min(bufferSize, writeSize) characters on the stream.
     */
    bool TestWrite(const char8 *string,
                   uint32 writeSize,
                   const uint32 bufferSize);

    /**
     * @brief Tests if the function returns false in case of read only stream.
     */
    bool TestWrite_RO();

    /**
     * @brief Tests if the size is equal to the bufferSize for a read only stream.
     */
    bool TestSize_Const(const char8 *input,
                        const uint32 bufferSize);

    /**
     * @brief Tests if the size is equal to the written memory size in case of stream with write access.
     */
    bool TestSize(const char8 *input,
                  const uint32 bufferSize);

    /**
     * @brief Tests if the function returns true when the final position is minor than the size of the written memory and false otherwise.
     */
    bool TestSeek(const uint32 bufferSize,
                  uint32 writeSize,
                  uint32 seek,
                  bool expected);

    /**
     * @brief Tests if the function returns true when the final position is greater than zero and minor than the size of the written memory, false otherwise.
     */
    bool TestRelativeSeek(const uint32 bufferSize,
                          uint32 initialPos,
                          int32 delta,
                          bool expected);

    /**
     * @brief Tests if the position changes correctly after read, write and seek operations.
     */
    bool TestPosition(const uint32 bufferSize,
                      uint32 finalPos);

    /**
     * @brief Tests if the function returns true if the input is minor than the buffer size and false otherwise.
     */
    bool TestSetSize(const uint32 bufferSize,
                     uint32 size,
                     bool expected);

    /**
     * @brief Tests the print formatted on a stream string.
     */
    bool TestPrintFormatted(uint32 bufferSize,
                            const PrintfNode testTable[]);

    /**
     * @brief Tests the GetToken function.
     */
    bool TestGetToken(uint32 bufferSize,
                      const TokenTestTableRow *table);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STREAMMEMORYREFERENCETEST_H_ */

