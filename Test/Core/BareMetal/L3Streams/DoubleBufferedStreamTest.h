/**
 * @file DoubleBufferedStreamTest.h
 * @brief Header file for class DoubleBufferedStreamTest
 * @date 17/10/2015
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class DoubleBufferedStreamTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DOUBLEBUFFEREDSTREAMTEST_H_
#define DOUBLEBUFFEREDSTREAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include "StreamTestHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
class DoubleBufferedStreamTest {
public:
    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the constructor with a timeout.
     */
    bool TestConstructor_Timeout();


    /**
     * @brief Tests the GetTimeout function.
     */
    bool TestGetTimeout();

    /**
     * @brief Tests the SetTimeout function.
     */
    bool TestSetTimeout();


    /**
     * @brief Tests that calling SetBufferSize(bufferSize) => bufferSize == GetBufferSize()
     */
    bool TestSetBufferSize(MARTe::uint32 bufferSize);

    /**
     * @brief Tests that readSize bytes can be successfully read from the stream.
     */
    bool TestRead(MARTe::uint32 bufferSize,
                  MARTe::uint32 readSize);

    /**
     * @brief Tests that readSize bytes can be successfully written to the stream.
     */
    bool TestWrite(MARTe::uint32 bufferSize,
                   MARTe::uint32 writeSize);

    /**
     * @brief Tests that the Read function returns false when CanRead() == false
     */
    bool TestRead_NotCanRead();

    /**
     * @brief Tests that the Write function returns false when CanWrite() == false
     */
    bool TestWrite_NotCanWrite();

    /**
     * @brief Tests that writeSize bytes can be successfully written to the stream even when
     * the number of bytes to write overflow the internal buffer capacity.
     * @pre
     *   bufferSize >= 8u
     *   writeSize >= 4 * bufferSize
     */
    bool TestWrite_OverflowInternalBuffer(MARTe::uint32 bufferSize,
                                          MARTe::uint32 writeSize);

    /**
     * @brief Tests that calling SetSize(size) => size == Size()
     */
    bool TestSize(MARTe::uint32 size);

    /**
     * @brief Seeks the stream into several positions and verifies that reading on the new
     * position retrieves the expected data.
     */
    bool TestSeek();

    /**
     * @brief Seeks the stream into several relative positions and verifies that reading on the new
     * position retrieves the expected data.
     */
    bool TestRelativeSeek();

    /**
     * @brief Tests that the Position is correctly retrieved (after performing Seek operations)
     */
    bool TestPosition();

    /**
     * @brief Tests that calling SetSize(size) => size == Size()
     */
    bool TestSetSize(MARTe::uint32 size);

    /**
     * @brief Performs a Printf and verifies that without Flushing the data is not written into the buffer
     * and that after calling Flush the data is written into the buffer.
     */
    bool TestFlush(MARTe::uint32 bufferSize);

    /**
     * @brief Tests the GetToken function. The possible tables to be used are defined in StreamTestHelper.
     */
    bool TestGetToken(MARTe::uint32 bufferSize,
                      const TokenTestTableRow *table);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DOUBLEBUFFEREDSTREAMTEST_H_ */

