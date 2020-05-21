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

//TEST(BareMetal_L4Configuration_PseudoCodeGTest,TestContextConstructor) {
    //PseudoCodeTest pseudoCodeTest;
    //ASSERT_TRUE(pseudoCodeTest.TestContextConstructor());
//}

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_CONST_CommaAsSeparator) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST,float32,1\n"
            "WRITE,ret\n"
    ;
    
    //TODO ExtractVariables returns true as expected but the actual output should also be checked.
    
    ASSERT_TRUE(pseudoCodeTest.TestExtractVariables(rpnCode));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_CONST_TabAsSeparator) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST\tfloat32\t1\n"
            "WRITE\tret\n"
    ;
    
    //TODO ExtractVariables returns true as expected but the actual output should also be checked.
    
    ASSERT_TRUE(pseudoCodeTest.TestExtractVariables(rpnCode));
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

/*---------------------------------------------------------------------------*/
/*                                  READ                                     */
/*---------------------------------------------------------------------------*/


TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_READ_MissingParameter) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "READ \n"
            "WRITE ret\n"
    ;
    
    // expected to fail at ExtractVariables()
    ErrorManagement::ErrorType expectedError = ErrorManagement::SyntaxError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_READ_ExceedingParameters) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "READ one two\n"
            "WRITE ret\n"
    ;
    
    // expected to fail at ExtractVariables()
    ErrorManagement::ErrorType expectedError = ErrorManagement::SyntaxError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

/// the following cases are considered valid syntax as for now

//TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_READ_NumericParameter) {
    
    //RuntimeEvaluatorTest pseudoCodeTest;
    
    //CCString rpnCode=
            //"READ 10\n"
            //"WRITE ret\n"
    //;
    
    //// expected to fail at ExtractVariables()
    //ErrorManagement::ErrorType expectedError = ErrorManagement::SyntaxError;
    
    //ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
//}

//TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_READ_ReservedKeywordAsParameter_1) {
    
    //RuntimeEvaluatorTest pseudoCodeTest;
    
    //CCString rpnCode=
            //"READ float32\n"
            //"WRITE ret\n"
    //;
    
    //// expected to fail at ExtractVariables()
    //ErrorManagement::ErrorType expectedError = ErrorManagement::SyntaxError;
    
    //ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
//}

//TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_READ_ReservedKeywordAsParameter_2) {
    
    //RuntimeEvaluatorTest pseudoCodeTest;
    
    //CCString rpnCode=
            //"READ WRITE\n"
            //"WRITE ret\n"
    //;
    
    //// expected to fail at ExtractVariables()
    //ErrorManagement::ErrorType expectedError = ErrorManagement::SyntaxError;
    
    //ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
//}

/*---------------------------------------------------------------------------*/
/*                                  WRITE                                    */
/*---------------------------------------------------------------------------*/


TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_WRITE_MissingParameter) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST float64 10\n"
            "WRITE \n"
    ;
    
    // expected to fail at ExtractVariables()
    ErrorManagement::ErrorType expectedError = ErrorManagement::SyntaxError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_WRITE_ExceedingParameters) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST float64 10\n"
            "WRITE ret1 ret2\n"
    ;
    
    // expected to fail at ExtractVariables()
    ErrorManagement::ErrorType expectedError = ErrorManagement::SyntaxError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

/// the following cases are considered valid syntax as for now

//TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_WRITE_NumericParameter) {
    
    //RuntimeEvaluatorTest pseudoCodeTest;
    
    //CCString rpnCode=
            //"CONST float64 10\n"
            //"WRITE 10\n"
    //;
    
    //// expected to fail at ExtractVariables()
    //ErrorManagement::ErrorType expectedError = ErrorManagement::SyntaxError;
    
    //ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
//}

//TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_WRITE_ReservedKeywordAsParameter_1) {
    
    //RuntimeEvaluatorTest pseudoCodeTest;
    
    //CCString rpnCode=
            //"CONST float64 10\n"
            //"WRITE float32\n"
    //;
    
    //// expected to fail at ExtractVariables()
    //ErrorManagement::ErrorType expectedError = ErrorManagement::SyntaxError;
    
    //ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
//}

//TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_WRITE_ReservedKeywordAsParameter_2) {
    
    //RuntimeEvaluatorTest pseudoCodeTest;
    
    //CCString rpnCode=
            //"READ WRITE\n"
            //"WRITE WRITE\n"
    //;
    
    //// expected to fail at ExtractVariables()
    //ErrorManagement::ErrorType expectedError = ErrorManagement::SyntaxError;
    
    //ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
//}

/*---------------------------------------------------------------------------*/
/*                                 CONST                                     */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_CONST_MissingParameter) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST \n"
            "WRITE ret\n"
    ;
    
    // expected to fail at ExtractVariables()
    ErrorManagement::ErrorType expectedError = ErrorManagement::SyntaxError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_CONST_SwitchedParameters) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST 1 float32\n"
            "WRITE ret\n"
    ;
    
    // expected to fail at ExtractVariables()
    ErrorManagement::ErrorType expectedError = ErrorManagement::SyntaxError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_CONST_ExceedingParameters) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST float32 1 2\n"
            "WRITE ret\n"
    ;
    
    // expected to fail at ExtractVariables()
    ErrorManagement::ErrorType expectedError = ErrorManagement::SyntaxError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_CONST_UnsupportedType) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST unsupportedtype 15\n"
            "WRITE ret\n"
    ;
    
    // expected to fail at ExtractVariables()
    ErrorManagement::ErrorType expectedError = ErrorManagement::SyntaxError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_CONST_UnspecifiedType) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST 10\n"
            "WRITE ret\n"
    ;
    
    // expected to fail at ExtractVariables()
    ErrorManagement::ErrorType expectedError = ErrorManagement::SyntaxError;
    
    ASSERT_TRUE(!pseudoCodeTest.TestError(rpnCode, expectedError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestError_CONST_NonNumericType) {
    
    RuntimeEvaluatorTest pseudoCodeTest;
    
    CCString rpnCode=
            "CONST char8 a\n"
            "WRITE ret\n"
    ;
    
    // expected to fail at ExtractVariables()
    ErrorManagement::ErrorType expectedError = ErrorManagement::SyntaxError;
    
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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_ReadNoName) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "READ \n"
            "READ IN2\n"
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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_WriteNoName) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "WRITE OUT1\n"
            "WRITE \n"
            "WRITE OUT2\n"
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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_ConstNoTypeNoValue) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST uint8 5\n"
            "CONST \n"
            "CONST float64 3.140000\n"
    ;

    evaluatorTest.AddExpectedInputVariable("Constant@0", UnsignedInteger8Bit,   0, NULL, false);

    ASSERT_TRUE(evaluatorTest.TestExtractVariables(rpnCode, ErrorManagement::IllegalOperation));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExtractVariables_ConstNoValue) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST uint8 5\n"
            "CONST int32\n"
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
