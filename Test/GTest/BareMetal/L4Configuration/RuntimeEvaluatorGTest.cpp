/**
 * @file RuntimeEvaluatorGTest.cpp
 * @brief Source file for class PseudoCodeGTest
 * @date 04/05/2020
 * @author RFX
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
 * the class PseudoCodeGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "RuntimeEvaluatorTest.h"
#include "RuntimeEvaluator.h"
#include "Vector.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest,TestIntegration) {
    RuntimeEvaluatorTest pseudoCodeTest;
    ASSERT_TRUE(pseudoCodeTest.TestIntegration());
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest,TestExpression_1) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    float64 values[2] = { 10.0, 8.9 };
    
    CCString rpnCode=
            "READ A\n"
            "READ B\n"
            "SUB\n"
            "WRITE F\n"
    ;
    
    ASSERT_TRUE(pseudoCodeTest.TestExpression(rpnCode, values));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest,TestExpression_2) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    float64 values[2] = { 10.0, 9.9 };
    
    CCString rpnCode=
            "READ A\n"
            "READ B\n"
            "ADD\n"
            "WRITE F\n"
    ;
    
    ASSERT_TRUE(pseudoCodeTest.TestExpression(rpnCode, values));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestGetInputVariableMemory) {
    RuntimeEvaluatorTest evaluatorTest;
    ASSERT_TRUE(evaluatorTest.TestGetInputVariableMemory());
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestGetOutputVariableMemory) {
    RuntimeEvaluatorTest evaluatorTest;
    ASSERT_TRUE(evaluatorTest.TestGetOutputVariableMemory());
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestSetVariableType) {
    RuntimeEvaluatorTest evaluatorTest;
    ASSERT_TRUE(evaluatorTest.TestSetVariableType());
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestPushPopPeek) {
    RuntimeEvaluatorTest evaluatorTest;
    ASSERT_TRUE(evaluatorTest.TestPushPopPeek());
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExecute_1) {
    
    RuntimeEvaluatorTest evaluatorTest;
    CCString rpnCode = "READ A\n"
                       "READ B\n"
                       "ADD\n"
                       "WRITE F\n"
    ;
    
    float32 A = 10;
    float32 B = 10;
    
    evaluatorTest.AddExpectedInputVariable("A",         Float32Bit, MAXDataMemoryAddress, &A, false);
    evaluatorTest.AddExpectedInputVariable("B",         Float32Bit, MAXDataMemoryAddress, &B, false);
    
    ASSERT_TRUE(evaluatorTest.TestExecute(rpnCode, ErrorManagement::NoError));
}

/*---------------------------------------------------------------------------*/
/*                                 CONST                                     */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_CONST_NonNumericParameter) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST float32 var\n"
            "WRITE ret\n"
    ;
    
    // expected to fail at Compile()
    ErrorManagement::ErrorType expectedError = ErrorManagement::InitialisationError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_CONST_ReservedKeywordAsParameter_1) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST float32 float32\n"
            "WRITE ret\n"
    ;
    
    // expected to fail at Compile()
    ErrorManagement::ErrorType expectedError = ErrorManagement::InitialisationError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_CONST_ReservedKeywordAsParameter_2) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST float32 WRITE\n"
            "WRITE ret\n"
    ;
    
    // expected to fail at Compile()
    ErrorManagement::ErrorType expectedError = ErrorManagement::InitialisationError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

/*---------------------------------------------------------------------------*/
/*                                  CAST                                     */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_CAST_MissingParameter) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST float64 10\n"
            "CAST \n"
            "WRITE ret\n"
    ;
    
    // expected to fail at Compile()
    ErrorManagement::ErrorType expectedError = ErrorManagement::InitialisationError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_CAST_ExceedingParameters) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST float64 10\n"
            "CAST float32 float64\n"
            "WRITE ret\n"
    ;
    
    // expected to fail at Compile()
    ErrorManagement::ErrorType expectedError = ErrorManagement::InitialisationError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_CAST_NumericParameter) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST float64 10\n"
            "CAST 10\n"
            "WRITE ret\n"
    ;
    
    // expected to fail at Compile()
    ErrorManagement::ErrorType expectedError = ErrorManagement::InitialisationError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_CAST_NonNumericParameter) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST float64 10\n"
            "CAST A\n"
            "WRITE ret\n"
    ;
    
    // expected to fail at Compile()
    ErrorManagement::ErrorType expectedError = ErrorManagement::InitialisationError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_CAST_ReservedKeywordAsParameter) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST float64 10\n"
            "CAST WRITE\n"
            "WRITE ret\n"
    ;
    
    // expected to fail at Compile()
    ErrorManagement::ErrorType expectedError = ErrorManagement::InitialisationError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_CAST_UnsupportedType) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST float64 10\n"
            "CAST unsupportedtype\n"
            "WRITE ret\n"
    ;
    
    // expected to fail at Compile()
    ErrorManagement::ErrorType expectedError = ErrorManagement::InitialisationError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_CAST_NonNumericType) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST float64 10\n"
            "CAST char8\n"
            "WRITE ret\n"
    ;
    
    // expected to fail at Compile()
    ErrorManagement::ErrorType expectedError = ErrorManagement::InitialisationError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

/*---------------------------------------------------------------------------*/
/*    ↑ N                   CODE LINE DO NOT CROSS                     D ↓   */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest,TestConstructor) {
    RuntimeEvaluatorTest evaluatorTest;
    ASSERT_TRUE(evaluatorTest.TestConstructor());
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_ReadSuccessful) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
    ;

    evaluatorTest.AddExpectedInputVariable("IN1", VoidType, MAXDataMemoryAddress, NULL, false);
    evaluatorTest.AddExpectedInputVariable("IN2", VoidType, MAXDataMemoryAddress, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::NoError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_ReadExistingInput) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "READ IN1\n"
    ;

    evaluatorTest.AddExpectedInputVariable("IN1", VoidType, MAXDataMemoryAddress, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::NoError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_ReadExistingOutput) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "WRITE OUT1\n"
            "READ OUT1\n"
    ;

    evaluatorTest.AddExpectedOutputVariable("OUT1", VoidType, MAXDataMemoryAddress, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::NoError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_ReadFailedNoName) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "READ \n"
            "READ IN2\n"
    ;

    evaluatorTest.AddExpectedInputVariable("IN1", VoidType, MAXDataMemoryAddress, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::IllegalOperation));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_ReadExceedingParameters) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "READ IN2 2\n"
    ;

    evaluatorTest.AddExpectedInputVariable("IN1", VoidType, MAXDataMemoryAddress, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::IllegalOperation));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_WriteSuccessful) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "WRITE OUT1\n"
            "WRITE OUT2\n"
    ;

    evaluatorTest.AddExpectedOutputVariable("OUT1", VoidType, MAXDataMemoryAddress, NULL, false);
    evaluatorTest.AddExpectedOutputVariable("OUT2", VoidType, MAXDataMemoryAddress, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::NoError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_WriteExistingInput) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "WRITE IN1\n"
    ;

    evaluatorTest.AddExpectedInputVariable("IN1", VoidType, MAXDataMemoryAddress, NULL, false);
    evaluatorTest.AddExpectedOutputVariable("IN1", VoidType, MAXDataMemoryAddress, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::NoError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_WriteFailedExistingOutput) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "WRITE OUT1\n"
            "WRITE OUT1\n"
    ;

    evaluatorTest.AddExpectedOutputVariable("OUT1", VoidType, MAXDataMemoryAddress, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::IllegalOperation));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_WriteFailedNoName) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "WRITE OUT1\n"
            "WRITE \n"
            "WRITE OUT2\n"
    ;

    evaluatorTest.AddExpectedOutputVariable("OUT1", VoidType, MAXDataMemoryAddress, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::IllegalOperation));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_WriteFailedExceedingParameters) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "WRITE OUT1\n"
            "WRITE OUT2 2\n"
    ;

    evaluatorTest.AddExpectedOutputVariable("OUT1", VoidType, MAXDataMemoryAddress, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::IllegalOperation));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_ConstSuccessful) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST uint8 5\n"
            "CONST float64 3.140000\n"
            "CONST int32 -15\n"
    ;

    evaluatorTest.AddExpectedInputVariable("Constant@0", UnsignedInteger8Bit,   0, NULL, false);
    evaluatorTest.AddExpectedInputVariable("Constant@1", Float64Bit,            1, NULL, false);
    evaluatorTest.AddExpectedInputVariable("Constant@3", SignedInteger32Bit,    3, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::NoError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_ConstFailedNoTypeNoValue) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST uint8 5\n"
            "CONST \n"
            "CONST float64 3.140000\n"
    ;

    evaluatorTest.AddExpectedInputVariable("Constant@0", UnsignedInteger8Bit,   0, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::IllegalOperation));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_ConstFailedNoValue) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST uint8 5\n"
            "CONST int32\n"
            "CONST float64 3.140000\n"
    ;

    evaluatorTest.AddExpectedInputVariable("Constant@0", UnsignedInteger8Bit,   0, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::IllegalOperation));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_ConstFailedNoNumeric) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST uint8 5\n"
            "CONST string 30\n"
            "CONST int32 -15\n"
    ;

    evaluatorTest.AddExpectedInputVariable("Constant@0", UnsignedInteger8Bit,   0, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::UnsupportedFeature));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_ConstFailedTypeSize0) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST uint8 5\n"
            "CONST void 30\n"
            "CONST int32 -15\n"
    ;

    evaluatorTest.AddExpectedInputVariable("Constant@0", UnsignedInteger8Bit,   0, NULL, false);
    evaluatorTest.AddExpectedInputVariable("Constant@1", VoidType,              1, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::UnsupportedFeature));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_ConstFailedExceedingParameters) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST uint8 5\n"
            "CONST uint8 16 7\n"
            "CONST float64 3.140000\n"
    ;

    evaluatorTest.AddExpectedInputVariable("Constant@0", UnsignedInteger8Bit,   0, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::IllegalOperation));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_MixedSuccessful) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST int32 -15\n"
            "WRITE OUT1\n"
            "CONST uint16 5\n"
            "READ IN1\n"
            "CONST float64 3.140000\n"
            "WRITE OUT2\n"
    ;

    evaluatorTest.AddExpectedInputVariable("Constant@0",    SignedInteger32Bit,     0,                      NULL, false);
    evaluatorTest.AddExpectedOutputVariable("OUT1",         VoidType,               MAXDataMemoryAddress,   NULL, false);
    evaluatorTest.AddExpectedInputVariable("Constant@1",    UnsignedInteger16Bit,   1,                      NULL, false);
    evaluatorTest.AddExpectedInputVariable("IN1",           VoidType,               MAXDataMemoryAddress,   NULL, false);
    evaluatorTest.AddExpectedInputVariable("Constant@2",    Float64Bit,             2,                      NULL, false);
    evaluatorTest.AddExpectedOutputVariable("OUT2",         VoidType,               MAXDataMemoryAddress,   NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::NoError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_MixedSuccessfulCommaAsSeparator) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST,float32,1\n"
            "WRITE,OUT1\n"
            "READ,IN1\n"
    ;

    evaluatorTest.AddExpectedInputVariable("Constant@0",    Float32Bit, 0,                      NULL, false);
    evaluatorTest.AddExpectedOutputVariable("OUT1",         VoidType,   MAXDataMemoryAddress,   NULL, false);
    evaluatorTest.AddExpectedInputVariable("IN1",           VoidType,   MAXDataMemoryAddress,   NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::NoError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_MixedSuccessfulTabAsSeparator) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST\tfloat32\t1\n"
            "WRITE\tOUT1\n"
            "READ\tIN1\n"
    ;

    evaluatorTest.AddExpectedInputVariable("Constant@0",    Float32Bit, 0,                      NULL, false);
    evaluatorTest.AddExpectedOutputVariable("OUT1",         VoidType,   MAXDataMemoryAddress,   NULL, false);
    evaluatorTest.AddExpectedInputVariable("IN1",           VoidType,   MAXDataMemoryAddress,   NULL, false);

        ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::NoError));
}
