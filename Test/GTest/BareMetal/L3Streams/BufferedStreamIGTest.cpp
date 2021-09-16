/**
 * @file BufferedStreamIGTest.cpp
 * @brief Source file for class BufferedStreamIGTest
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

 * @details This source file contains the definition of all the methods for
 * the class BufferedStreamIGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */


/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <BufferedStreamITest.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestDefaultConstructor) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestDefaultConstructor());
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestAnyType) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestAnyType());
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_TableTerminators_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(1, &TokenTestTableTerminators[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_TableTerminators_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(2, &TokenTestTableTerminators[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_TableTerminators_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(8, &TokenTestTableTerminators[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_TableTerminators_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(64, &TokenTestTableTerminators[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_TableTerminators_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(128, &TokenTestTableTerminators[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_TableTerminators_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(1024, &TokenTestTableTerminators[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_TableSkipCharacters_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(1, &TokenTestTableSkipCharacters[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_TableSkipCharacters_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(2, &TokenTestTableSkipCharacters[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_TableSkipCharacters_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(8, &TokenTestTableSkipCharacters[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_TableSkipCharacters_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(64, &TokenTestTableSkipCharacters[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_TableSkipCharacters_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(128, &TokenTestTableSkipCharacters[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_TableSkipCharacters_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken(1024, &TokenTestTableSkipCharacters[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_Stream_TableTerminators_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken_Stream(1, &TokenTestTableTerminators[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_Stream_TableTerminators_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken_Stream(2, &TokenTestTableTerminators[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_Stream_TableTerminators_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken_Stream(8, &TokenTestTableTerminators[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_Stream_TableTerminators_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken_Stream(64, &TokenTestTableTerminators[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_Stream_TableTerminators_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken_Stream(128, &TokenTestTableTerminators[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_Stream_TableTerminators_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken_Stream(1024, &TokenTestTableTerminators[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_Stream_TableSkipCharacters_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken_Stream(1, &TokenTestTableSkipCharacters[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_Stream_TableSkipCharacters_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken_Stream(2, &TokenTestTableSkipCharacters[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_Stream_TableSkipCharacters_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken_Stream(8, &TokenTestTableSkipCharacters[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_Stream_TableSkipCharacters_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken_Stream(64, &TokenTestTableSkipCharacters[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_Stream_TableSkipCharacters_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken_Stream(128, &TokenTestTableSkipCharacters[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetToken_Stream_TableSkipCharacters_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetToken_Stream(1024, &TokenTestTableSkipCharacters[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestSkipTokens_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestSkipTokens(1, &SkipTokensTestTable[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestSkipTokens_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestSkipTokens(2, &SkipTokensTestTable[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestSkipTokens_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestSkipTokens(8, &SkipTokensTestTable[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestSkipTokens_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestSkipTokens(64, &SkipTokensTestTable[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestSkipTokens_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestSkipTokens(128, &SkipTokensTestTable[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestSkipTokens_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestSkipTokens(1024, &SkipTokensTestTable[0]));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_NoSkipCharacter_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(1, false));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_NoSkipCharacter_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(2, false));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_NoSkipCharacter_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(8, false));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_NoSkipCharacter_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(64, false));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_NoSkipCharacter_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(128, false));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_NoSkipCharacter_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(1024, false));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_SkipCharacter_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(1, true));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_SkipCharacter_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(2, true));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_SkipCharacter_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(8, true));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_SkipCharacter_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(64, true));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_SkipCharacter_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(128, true));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_SkipCharacter_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine(1024, true));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_Stream_NoSkipCharacter_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine_Stream(1, false));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_Stream_NoSkipCharacter_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine_Stream(2, false));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_Stream_NoSkipCharacter_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine_Stream(8, false));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_Stream_NoSkipCharacter_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine_Stream(64, false));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_Stream_NoSkipCharacter_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine_Stream(128, false));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_Stream_NoSkipCharacter_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine_Stream(1024, false));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_Stream_SkipCharacter_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine_Stream(1, true));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_Stream_SkipCharacter_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine_Stream(2, true));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_Stream_SkipCharacter_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine_Stream(8, true));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_Stream_SkipCharacter_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine_Stream(64, true));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_Stream_SkipCharacter_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine_Stream(128, true));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestGetLine_Stream_SkipCharacter_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestGetLine_Stream(1024, true));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestCopy_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestCopy(1));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestCopy_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestCopy(2));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestCopy_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestCopy(8));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestCopy_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestCopy(64));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestCopy_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestCopy(128));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestCopy_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestCopy(1024));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestCopy_Stream_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestCopy_Stream(1));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestCopy_Stream_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestCopy_Stream(2));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestCopy_Stream_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestCopy_Stream(8));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestCopy_Stream_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestCopy_Stream(64));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestCopy_Stream_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestCopy_Stream(128));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestCopy_Stream_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestCopy_Stream(1024));
}

/*TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Decimal_BufferSize_0) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(0, GeneratePrintFormattedDecimalTable()));
}*/

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Decimal_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1, GeneratePrintFormattedDecimalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Decimal_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(2, GeneratePrintFormattedDecimalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Decimal_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(8, GeneratePrintFormattedDecimalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Decimal_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(64, GeneratePrintFormattedDecimalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Decimal_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(128, GeneratePrintFormattedDecimalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Decimal_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1024, GeneratePrintFormattedDecimalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Hexadecimal_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1, GeneratePrintFormattedHexadecimalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Hexadecimal_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(2, GeneratePrintFormattedHexadecimalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Hexadecimal_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(8, GeneratePrintFormattedHexadecimalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Hexadecimal_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(64, GeneratePrintFormattedHexadecimalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Hexadecimal_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(128, GeneratePrintFormattedHexadecimalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Hexadecimal_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1024, GeneratePrintFormattedHexadecimalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Octal_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1, GeneratePrintFormattedOctalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Octal_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(2, GeneratePrintFormattedOctalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Octal_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(8, GeneratePrintFormattedOctalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Octal_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(64, GeneratePrintFormattedOctalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Octal_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(128, GeneratePrintFormattedOctalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Octal_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1024, GeneratePrintFormattedOctalTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Binary_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1, GeneratePrintFormattedBinaryTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Binary_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(2, GeneratePrintFormattedBinaryTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Binary_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(8, GeneratePrintFormattedBinaryTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Binary_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(64, GeneratePrintFormattedBinaryTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Binary_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(128, GeneratePrintFormattedBinaryTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_Binary_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1024, GeneratePrintFormattedBinaryTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatFP_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1, GeneratePrintFormattedFloatFPTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatFP_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(2, GeneratePrintFormattedFloatFPTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatFP_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(8, GeneratePrintFormattedFloatFPTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatFP_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(64, GeneratePrintFormattedFloatFPTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatFP_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(128, GeneratePrintFormattedFloatFPTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatFP_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1024, GeneratePrintFormattedFloatFPTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatFPR_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1, GeneratePrintFormattedFloatFPRTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatFPR_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(2, GeneratePrintFormattedFloatFPRTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatFPR_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(8, GeneratePrintFormattedFloatFPRTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatFPR_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(64, GeneratePrintFormattedFloatFPRTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatFPR_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(128, GeneratePrintFormattedFloatFPRTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatFPR_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1024, GeneratePrintFormattedFloatFPRTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatExp_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1, GeneratePrintFormattedFloatExpTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatExp_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(2, GeneratePrintFormattedFloatExpTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatExp_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(8, GeneratePrintFormattedFloatExpTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatExp_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(64, GeneratePrintFormattedFloatExpTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatExp_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(128, GeneratePrintFormattedFloatExpTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatExp_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1024, GeneratePrintFormattedFloatExpTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatEng_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1, GeneratePrintFormattedFloatEngTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatEng_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(2, GeneratePrintFormattedFloatEngTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatEng_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(8, GeneratePrintFormattedFloatEngTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatEng_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(64, GeneratePrintFormattedFloatEngTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatEng_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(128, GeneratePrintFormattedFloatEngTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatEng_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1024, GeneratePrintFormattedFloatEngTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatSmart_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1, GeneratePrintFormattedFloatSmartTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatSmart_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(2, GeneratePrintFormattedFloatSmartTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatSmart_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(8, GeneratePrintFormattedFloatSmartTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatSmart_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(64, GeneratePrintFormattedFloatSmartTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatSmart_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(128, GeneratePrintFormattedFloatSmartTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatSmart_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1024, GeneratePrintFormattedFloatSmartTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatCompact_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1, GeneratePrintFormattedFloatCompactTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatCompact_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(2, GeneratePrintFormattedFloatCompactTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatCompact_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(8, GeneratePrintFormattedFloatCompactTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatCompact_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(64, GeneratePrintFormattedFloatCompactTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatCompact_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(128, GeneratePrintFormattedFloatCompactTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest,TestPrintFormatted_Integer_FloatCompact_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted(1024, GeneratePrintFormattedFloatCompactTable()));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_Pointer_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_Pointer(1));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_Pointer_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_Pointer(2));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_Pointer_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_Pointer(8));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_Pointer_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_Pointer(64));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_Pointer_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_Pointer(128));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_Pointer_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_Pointer(1024));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_CCString_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_CCString(1));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_CCString_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_CCString(2));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_CCString_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_CCString(8));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_CCString_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_CCString(64));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_CCString_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_CCString(128));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_CCString_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_CCString(1024));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_BitSet_Unsigned_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_BitSet_Unsigned(1));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_BitSet_Unsigned_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_BitSet_Unsigned(2));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_BitSet_Unsigned_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_BitSet_Unsigned(8));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_BitSet_Unsigned_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_BitSet_Unsigned(64));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_BitSet_Unsigned_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_BitSet_Unsigned(128));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_BitSet_Unsigned_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_BitSet_Unsigned(1024));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_BitSet_Signed_BufferSize_1) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_BitSet_Signed(1));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_BitSet_Signed_BufferSize_2) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_BitSet_Signed(2));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_BitSet_Signed_BufferSize_8) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_BitSet_Signed(8));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_BitSet_Signed_BufferSize_64) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_BitSet_Signed(64));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_BitSet_Signed_BufferSize_128) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_BitSet_Signed(128));
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintFormatted_BitSet_Signed_BufferSize_1024) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintFormatted_BitSet_Signed(1024));
}


TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestPrintf) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestPrintf());
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestFlush) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestFlush());
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestRefill) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestRefill());
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestSetCalibReadParam) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestSetCalibReadParam());
}

TEST(BareMetal_L3Streams_BufferedStreamIGTest, TestSetCalibWriteParam) {
    BufferedStreamITest streamtest;
    ASSERT_TRUE(streamtest.TestSetCalibWriteParam());
}

