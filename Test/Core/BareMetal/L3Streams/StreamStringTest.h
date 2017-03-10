/**
 * @file StreamStringTest.h
 * @brief Header file for class StreamStringTest
 * @date 26/ott/2015
 * @author pc
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

 * @details This header file contains the declaration of the class StreamStringTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRINGTEST_H_
#define STRINGTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StreamString.h"
#include "FormatDescriptor.h"
#include "StreamTestHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Test the StreamString public methods.
 */
class StreamStringTest {

private:

public:

    /**
     * @brief Tests if the buffer is null at the beginning, and size and position are 0.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests if the buffer is initialized correctly by the const chat string in input.
     */
    bool TestConstructor_CCString(CCString initializationString);

    /**
     * @brief Tests if the buffer of the StreamString in input is duplicated correctly into the new instantiated object buffer.
     */
    bool TestCopyConstructor(const char8 * initializationString);

    /**
     * @brief Tests if the destructor puts the buffer to NULL.
     */
    bool TestDestructor();

    /**
     * @brief Tests if the StreamString is considered as a const char string by in the conversion to AnyType.
     */
    bool TestAnyTypeOperator(const char8* initializationString);

    /**
     * @brief Tests if the specified number of chars are correctly read from the string.
     */
    bool TestRead(const char8* inputString,
                  uint32 sizeToRead);


    /**
     * @brief Tests if the behavior is the same of the normal Read.
     */
    bool TestReadTimeout(const char* inputString,
                   uint32 sizeToWrite);

    /**
     * @brief Tests if the specified number of chars are correctly write on the string.
     */
    bool TestWrite(const char* inputString,
                   uint32 sizeToWrite);


    /**
     * @brief Tests if the behavior is the same of the normal Write.
     */
    bool TestWriteTimeout(const char* inputString,
                   uint32 sizeToWrite);

    /**
     * @brief Tests if the function returns true.
     */
    bool TestCanWrite();

    /**
     * @brief Tests if the function returns true.
     */
    bool TestCanRead();

    /**
     * @brief Tests if the function returns true.
     */
    bool TestCanSeek();

    /**
     * @brief Tests if the function returns size currently used in the stream.
     */
    bool TestSize(const char8 * input);

    /**
     * @brief Tests if the function allocates the desired size.
     */
    bool TestSetSize(uint32 size);

    /**
     * @brief Tests if the function returns the internal buffer with read only permissions.
     */
    bool TestBuffer(const char8 * input);

    /**
     * @brief Tests if the function returns the internal buffer with read-write permissions.
     */
    bool TestBufferReference(const char8 * input);

    /**
     * @brief Tests if after the seek the position in the stream is the desired one ore if the function fails in case of a desired position
     * greater than the used size.
     */
    bool TestSeek(uint32 usedSize,
                  uint32 seek,
                  bool expected);

    /**
     * @brief Tests if after the relative seek the position in the stream is the desired one ore if the function fails because the final
     * position is minor than zero or greater than the used size.
     */
    bool TestRelativeSeek(uint32 initialPos,
                          int32 delta,
                          bool expected);

    /**
     * @brief Tests the RelativeSeek passing an offset out of the int32 range (supported by IOBuffer)
     */
    bool TestRelativeSeek_OutOfInt32Range();

    /**
     * @brief Tests if the position increase after read-write operations and if it changes after seek operations.
     */
    bool TestPosition();

    /**
     * @brief Tests if the function returns the correct character of it fails if the index is greater than the used size.
     */
    bool TestTail(const char8* input,
                  uint32 index);

    /**
     * @brief Tests if the char in input is copied in the stream.
     */
    bool TestCopyOperator_Char(char8 input);

    /**
     * @brief Tests if the C string in input is copied in the stream.
     */
    bool TestCopyOperator_CCString(const char8 * input);

    /**
     * @brief Tests if a StreamString in input is copied in the stream.
     */
    bool TestCopyOperator_String(const char8 * input);

    /**
     * @brief Tests if the char in input is appended on the stream queue.
     */
    bool TestConcatenateOperator_Char(char8 input);

    /**
     * @brief Tests if the C string in input is appended on the stream queue.
     */
    bool TestConcatenateOperator_CCString(const char8 *input);

    /**
     * @brief Tests if a the buffer of a StreamString in input is appended on the stream queue.
     */
    bool TestConcatenateOperator_String(const char8 *input);

    /**
     * @brief Tests if the function returns true if the buffers contain the same data of the C string in input.
     */
    bool TestIsEqualOperator_CCString(const char8 *input);

    /**
     * @brief Tests if the function returns true if the buffers of the two compared StreamString contain the same data.
     */
    bool TestIsEqualOperator_String(const char8 *input);

    /**
     * @See TestIsEqualOperator_CCString. It returns the opposite value.
     */
    bool TestIsDifferentOperator_CCString(const char8 *input);

    /**
     * @See TestIsEqualOperator_String. It returns the opposite value.
     */
    bool TestIsDifferentOperator_String(const char8 *input);

    /**
     * @brief Tests if the function returns the character at the specified position or if it fails if the index is
     * greater than the used size.
     */
    bool TestGetCharacterOperator(const char8* input,
                                  uint32 index);

    /**
     * @brief Tests if the function returns the index of the first char matched with the input, or if it returns -1 if
     * the character in input is not in the stream.
     */
    bool TestLocate_Char(const char8 *input,
                         char8 c,
                         int32 expected);

    /**
     * @brief Tests if the function returns the index of the first substring matched with the input, or if it returns -1 if
     * the string in input is not contained in the stream.
     */
    bool TestLocate_String(const char8 *input,
                           const char8* toSearch,
                           int32 expected);

    /**
     * @brief Tests the print formatted on a stream string.
     */
    bool TestPrintFormatted(const PrintfNode testTable[]);


    /**
     * @brief Tests the GetToken function.
     */
    bool TestGetToken(const TokenTestTableRow *table);



};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STRINGTEST_H_ */

