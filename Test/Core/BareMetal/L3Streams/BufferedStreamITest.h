/**
 * @file BufferedStreamITest.h
 * @brief Header file for class BufferedStreamITest
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

 * @details This header file contains the declaration of the class BufferedStreamITest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BUFFEREDSTREAMITEST_H_
#define BUFFEREDSTREAMITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TimeoutType.h"
#include "StreamTestHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Class for testing of StreamI functions.
 */
class BufferedStreamITest {
public:
    /**
     * @brief Tests the default constructor
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the operator AnyType.
     */
    bool TestAnyType();

    /**
     * @brief Tests the GetToken function. The possible tables to be used are defined in StreamTestHelper.
     */
    bool TestGetToken(uint32 bufferSize,
                      const TokenTestTableRow *table);

    /**
     * @brief Tests the GetToken function with the stream input. The possible tables to be used are defined in StreamTestHelper.
     */
    bool TestGetToken_Stream(uint32 bufferSize,
                             const TokenTestTableRow *table);

    /**
     * @brief Tests the SkipTokens function. The possible tables to be used are defined in StreamTestHelper.
     */
    bool TestSkipTokens(uint32 bufferSize,
                        const SkipTokensTestTableRow *table);

    /**
     * @brief Tests the GetLine function.
     */
    bool TestGetLine(uint32 bufferSize,
                     bool skipCharacter);

    /**
     * @brief Tests the GetLine function with the stream input version.
     */
    bool TestGetLine_Stream(uint32 bufferSize,
                            bool skipCharacter);

    /**
     * @brief Tests the PrintfFormatted function. The possible tables to be used are defined in StreamTestHelper.
     */
    bool TestPrintFormatted(uint32 bufferSize,
                            const PrintfNode testTable[]);

    /**
     * @brief Tests the PrintFormatted function with a CCString
     */
    bool TestPrintFormatted_CCString(uint32 bufferSize);

    /**
     * @brief Tests the print of a pointer.
     */
    bool TestPrintFormatted_Pointer(uint32 bufferSize);

    /**
     * @brief Tests the PrintFormatted function with an unsigned bitset
     */
    bool TestPrintFormatted_BitSet_Unsigned(uint32 bufferSize);

    /**
     * @brief Tests the PrintFormatted function with a signed bitset
     */
    bool TestPrintFormatted_BitSet_Signed(uint32 bufferSize);

    /**
     * @brief Tests the Copy function.
     */
    bool TestCopy(uint32 bufferSize);

    /**
     * @brief Tests the Copy function using the stream version.
     */
    bool TestCopy_Stream(uint32 bufferSize);

    /**
     * @brief Tests the Printf function with a different number of arguments. Note that the true printing engine is the PrintFormatted.
     */
    bool TestPrintf();

    /**
     * @brief Tests the Flush method
     */
    bool TestFlush();

    /**
     * @brief Tests the Refill method
     */
    bool TestRefill();

    /**
     * @brief Tests the SetCalibReadParam method
     */
    bool TestSetCalibReadParam();

    /**
     * @brief Tests the SetCalibWriteParam method
     */
    bool TestSetCalibWriteParam();


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


#endif /* BUFFEREDSTREAMITEST_H_ */

