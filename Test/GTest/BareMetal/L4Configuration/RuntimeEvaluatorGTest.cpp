/**
 * @file RuntimeEvaluatorGTest.cpp
 * @brief Source file for class RuntimeEvaluatorGTest
 * @date 04/05/2020
 * @author Nicolo Ferron
 * @author Dídac Magriñá
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

#include "MARTe2UTest.h"
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

/*---------------------------------------------------------------------------*/
/*                      Constructor/Setters/Getters                          */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest,TestConstructor) {
    RuntimeEvaluatorTest evaluatorTest;
    ASSERT_TRUE(evaluatorTest.TestConstructor());
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestGetPseudoCode) {
    RuntimeEvaluatorTest evaluatorTest;
    ASSERT_TRUE(evaluatorTest.TestGetPseudoCode());
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestBrowseInputVariables_Successful) {
    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode = "READ A\n"
                       "READ B\n"
                       "ADD\n"
                       "WRITE C\n"
    ;
    RuntimeEvaluator evaluator(rpnCode);

    ASSERT_TRUE(evaluator.ExtractVariables() == ErrorManagement::NoError);

    ASSERT_TRUE(evaluatorTest.TestBrowseInputVariable(evaluator, 0, ErrorManagement::NoError, "A"));
    ASSERT_TRUE(evaluatorTest.TestBrowseInputVariable(evaluator, 1, ErrorManagement::NoError, "B"));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestBrowseInputVariables_OutOfRange) {
    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode = "READ A\n"
                       "READ B\n"
                       "ADD\n"
                       "WRITE C\n"
    ;
    RuntimeEvaluator evaluator(rpnCode);

    ASSERT_TRUE(evaluator.ExtractVariables() == ErrorManagement::NoError);

    ASSERT_TRUE(evaluatorTest.TestBrowseInputVariable(evaluator, 2, ErrorManagement::OutOfRange, "C"));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestBrowseOutputVariables_Successful) {
    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode = "READ A\n"
                       "READ B\n"
                       "READ C\n"
                       "ADD\n"
                       "WRITE D\n"
                       "WRITE E\n"
    ;
    RuntimeEvaluator evaluator(rpnCode);

    ASSERT_TRUE(evaluator.ExtractVariables() == ErrorManagement::NoError);

    ASSERT_TRUE(evaluatorTest.TestBrowseOutputVariable(evaluator, 0, ErrorManagement::NoError, "D"));
    ASSERT_TRUE(evaluatorTest.TestBrowseOutputVariable(evaluator, 1, ErrorManagement::NoError, "E"));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestBrowseOutputVariables_OutOfRange) {
    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode = "READ A\n"
                       "READ B\n"
                       "READ C\n"
                       "ADD\n"
                       "WRITE D\n"
                       "WRITE E\n"
    ;
    RuntimeEvaluator evaluator(rpnCode);

    ASSERT_TRUE(evaluator.ExtractVariables() == ErrorManagement::NoError);

    ASSERT_TRUE(evaluatorTest.TestBrowseOutputVariable(evaluator, 2, ErrorManagement::OutOfRange, "C"));
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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestSetVariableMemory) {
    RuntimeEvaluatorTest evaluatorTest;
    ASSERT_TRUE(evaluatorTest.TestSetVariableMemory());
}


TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestVariable) {
    RuntimeEvaluatorTest evaluatorTest;
    ASSERT_TRUE(evaluatorTest.TestVariable());
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestPushPopPeek) {
    RuntimeEvaluatorTest evaluatorTest;
    ASSERT_TRUE(evaluatorTest.TestPushPopPeek());
}

/*---------------------------------------------------------------------------*/
/*                                Execute                                    */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExecute_Successful_1) {
    
    RuntimeEvaluatorTest evaluatorTest;
    CCString rpnCode = "READ A\n"
                       "READ B\n"
                       "ADD\n"
                       "WRITE F\n"
    ;
    
    float32 A = 10;
    float32 B = 10;
    float32 F;
    
    evaluatorTest.SetTestInputVariable("A", Float32Bit, &A, 0);
    evaluatorTest.SetTestInputVariable("B", Float32Bit, &B, 0);
    
    evaluatorTest.SetTestOutputVariable("F", Float32Bit, &F, 20);
    
    ASSERT_TRUE(evaluatorTest.TestExecute(rpnCode, ErrorManagement::NoError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExecute_Successful_2) {
    
    RuntimeEvaluatorTest evaluatorTest;
    CCString rpnCode = "READ A\n"
                       "READ B\n"
                       "ADD\n"
                       "WRITE ret1\n"
                       "READ ret1\n"
                       "READ C\n"
                       "MUL\n"
                       "WRITE ret2\n"
    ;
    
    float32 A = 10;
    float32 B = 10;
    float32 C = 5;
    float32 ret1;
    
    evaluatorTest.SetTestInputVariable("A", Float32Bit, &A, 0);
    evaluatorTest.SetTestInputVariable("B", Float32Bit, &B, 0);
    evaluatorTest.SetTestInputVariable("C", Float32Bit, &C, 0);
    
    evaluatorTest.SetTestOutputVariable("ret1", Float32Bit, &ret1, 20);
    evaluatorTest.SetTestOutputVariable("ret2", Float32Bit, NULL,  100);
    
    ASSERT_TRUE(evaluatorTest.TestExecute(rpnCode, ErrorManagement::NoError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExecute_Successful_AllExternalVariables) {
    
    RuntimeEvaluatorTest evaluatorTest;
    CCString rpnCode = "READ v1\n"
                       "READ t2\n"
                       "READ t1\n"
                       "SUB\n"
                       "DIV\n"
                       "WRITE a\n"
                       "READ v1\n"
                       "CONST float32 2\n"
                       "POW\n"
                       "CONST float32 2\n"
                       "READ a\n"
                       "CONST float32 2\n"
                       "POW\n"
                       "MUL\n"
                       "DIV\n"
                       "WRITE x2\n"
                       "READ m\n"
                       "READ a\n"
                       "MUL\n"
                       "WRITE F\n"
    ;
    
    float32 v1 = 22.5;
    float32 m  = 9;
    float32 t1 = 3.5;
    float32 t2 = 8;
    float32 F;
    
    evaluatorTest.SetTestInputVariable("v1", Float32Bit, &v1, 0);
    evaluatorTest.SetTestInputVariable("m",  Float32Bit, &m,  0);
    evaluatorTest.SetTestInputVariable("t1", Float32Bit, &t1, 0);
    evaluatorTest.SetTestInputVariable("t2", Float32Bit, &t2, 0);
    
    evaluatorTest.SetTestOutputVariable("F", Float32Bit, &F, 45);
    
    ASSERT_TRUE(evaluatorTest.TestExecute(rpnCode, ErrorManagement::NoError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExecute_Successful_AllInternalVariables) {
    
    RuntimeEvaluatorTest evaluatorTest;
    CCString rpnCode = "READ v1\n"
                       "READ t2\n"
                       "READ t1\n"
                       "SUB\n"
                       "DIV\n"
                       "WRITE a\n"
                       "READ v1\n"
                       "CONST float32 2\n"
                       "POW\n"
                       "CONST float32 2\n"
                       "READ a\n"
                       "CONST float32 2\n"
                       "POW\n"
                       "MUL\n"
                       "DIV\n"
                       "WRITE x2\n"
                       "READ m\n"
                       "READ a\n"
                       "MUL\n"
                       "WRITE F\n"
    ;
    
    float32 F;
    
    evaluatorTest.SetTestInputVariable("v1", Float32Bit, NULL, 22.5);
    evaluatorTest.SetTestInputVariable("m",  Float32Bit, NULL, 9);
    evaluatorTest.SetTestInputVariable("t1", Float32Bit, NULL, 3.5);
    evaluatorTest.SetTestInputVariable("t2", Float32Bit, NULL, 8);
    
    evaluatorTest.SetTestOutputVariable("F", Float32Bit, &F, 45);
    
    ASSERT_TRUE(evaluatorTest.TestExecute(rpnCode, ErrorManagement::NoError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExecute_Successful_MixedInternalAndExternal) {
    
    RuntimeEvaluatorTest evaluatorTest;
    CCString rpnCode = "READ v1\n"
                       "READ t2\n"
                       "READ t1\n"
                       "SUB\n"
                       "DIV\n"
                       "WRITE a\n"
                       "READ v1\n"
                       "CONST float32 2\n"
                       "POW\n"
                       "CONST float32 2\n"
                       "READ a\n"
                       "CONST float32 2\n"
                       "POW\n"
                       "MUL\n"
                       "DIV\n"
                       "WRITE x2\n"
                       "READ m\n"
                       "READ a\n"
                       "MUL\n"
                       "WRITE F\n"
    ;
    
    float32 m  = 9;
    float32 t2 = 8;
    float32 F;
    
    evaluatorTest.SetTestInputVariable("v1", Float32Bit, NULL, 22.5);
    evaluatorTest.SetTestInputVariable("m",  Float32Bit, &m,   0);
    evaluatorTest.SetTestInputVariable("t1", Float32Bit, NULL, 3.5);
    evaluatorTest.SetTestInputVariable("t2", Float32Bit, &t2,  0);
    
    evaluatorTest.SetTestOutputVariable("F", Float32Bit, &F, 45);
    
    ASSERT_TRUE(evaluatorTest.TestExecute(rpnCode, ErrorManagement::NoError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExecute_FastMode_FailedStackNotEmpty) {
    
    RuntimeEvaluatorTest evaluatorTest;
    CCString rpnCode = "READ A\n"
                       "READ B\n"
                       "WRITE ret1\n"
    ;
    
    float32 A = 10;
    float32 B = 10;
    float32 ret1;
    
    evaluatorTest.SetTestInputVariable("A", Float32Bit, &A, 0);
    evaluatorTest.SetTestInputVariable("B", Float32Bit, &B, 0);
    
    evaluatorTest.SetTestOutputVariable("ret1", Float32Bit, &ret1, 20);
    
    ASSERT_TRUE(evaluatorTest.TestExecute(rpnCode, ErrorManagement::InternalSetupError));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExecute_SafeMode_FailedStackNotEmpty) {
    
    RuntimeEvaluatorTest evaluatorTest;
    CCString rpnCode = "READ A\n"
                       "READ B\n"
                       "WRITE ret1\n"
    ;
    
    float32 A = 10;
    float32 B = 10;
    float32 ret1;
    
    evaluatorTest.SetTestInputVariable("A", Float32Bit, &A, 0);
    evaluatorTest.SetTestInputVariable("B", Float32Bit, &B, 0);
    
    evaluatorTest.SetTestOutputVariable("ret1", Float32Bit, &ret1, 20);
    
    ASSERT_TRUE(evaluatorTest.TestExecute(rpnCode, ErrorManagement::InternalSetupError, RuntimeEvaluator::safeMode));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExecute_SafeMode_FailedGeneralExecutionError) {
    
    TypeDescriptor types[] = {Float32Bit, Float32Bit};
    RuntimeEvaluatorFunction mockGeneralExecErr("MGENEXECERR", 0, 0, types, MockExecutionError);
    RegisterFunction(mockGeneralExecErr);
    
    RuntimeEvaluatorTest evaluatorTest;
    CCString rpnCode = "READ A\n"
                       "MGENEXECERR\n"
                       "WRITE ret1\n"
    ;
    
    float32 A = 10;
    float32 ret1;
    
    evaluatorTest.SetTestInputVariable("A", Float32Bit, &A, 0);
    
    evaluatorTest.SetTestOutputVariable("ret1", Float32Bit, &ret1, 20);
    
    ErrorManagement::ErrorType expectedError;
    
    expectedError.internalSetupError = true;
    expectedError.notCompleted       = true;
    expectedError.syntaxError        = true;
    
    ASSERT_TRUE(evaluatorTest.TestExecute(rpnCode, expectedError, RuntimeEvaluator::safeMode));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExecute_SafeMode_FailedOutOfRangeExecutionError) {
    
    TypeDescriptor types[] = {Float32Bit, Float32Bit};
    RuntimeEvaluatorFunction mockExecErr("MEXECERR", 0, 0, types, MockOutOfRangeExecutionError);
    RegisterFunction(mockExecErr);
    
    RuntimeEvaluatorTest evaluatorTest;
    CCString rpnCode = "READ A\n"
                       "MEXECERR\n"
                       "WRITE ret1\n"
    ;
    
    float32 A = 10;
    float32 ret1;
    
    evaluatorTest.SetTestInputVariable("A", Float32Bit, &A, 0);
    
    evaluatorTest.SetTestOutputVariable("ret1", Float32Bit, &ret1, 20);
    
    ErrorManagement::ErrorType expectedError;
    
    expectedError.internalSetupError = true;
    expectedError.notCompleted       = true;
    expectedError.outOfRange         = true;
    
    ASSERT_TRUE(evaluatorTest.TestExecute(rpnCode, expectedError, RuntimeEvaluator::safeMode));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExecute_SafeMode_FailedStackOverflow) {
    
    TypeDescriptor types[] = {Float32Bit, Float32Bit};
    RuntimeEvaluatorFunction mockRead("MREAD", 0, 1, types, MockRead);
    RegisterFunction(mockRead);
    
    RuntimeEvaluatorTest evaluatorTest;
    CCString rpnCode = "READ A\n"
                       "MREAD\n"
                       "WRITE ret1\n"
                       "READ B\n"
                       "READ C\n"
                       "READ D\n"
                       "ADD\n"
                       "ADD\n"
                       "ADD\n"
    ;
    
    float32 A = 10;
    float32 B = 10;
    float32 C = 10;
    float32 D = 10;
    float32 ret1;
    
    evaluatorTest.SetTestInputVariable("A", Float32Bit, &A, 0);
    evaluatorTest.SetTestInputVariable("B", Float32Bit, &B, 0);
    evaluatorTest.SetTestInputVariable("C", Float32Bit, &C, 0);
    evaluatorTest.SetTestInputVariable("D", Float32Bit, &C, 0);
    
    evaluatorTest.SetTestOutputVariable("ret1", Float32Bit, &ret1, 20);
    
    ErrorManagement::ErrorType expectedError;
    
    expectedError.internalSetupError = true;
    expectedError.notCompleted       = true;
    expectedError.outOfRange         = true;
    
    ASSERT_TRUE(evaluatorTest.TestExecute(rpnCode, expectedError, RuntimeEvaluator::safeMode));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestExecute_SafeMode_FailedStackUnderflow) {
    
    TypeDescriptor types[] = {Float32Bit, Float32Bit};
    RuntimeEvaluatorFunction mockWrite("MWRITE", 0, 1, types, MockWrite);
    RegisterFunction(mockWrite);
    
    RuntimeEvaluatorTest evaluatorTest;
    CCString rpnCode = "READ A\n"
                       "MWRITE\n"
                       "WRITE ret1\n"
    ;
    
    float32 A = 10;
    float32 ret1;
    
    evaluatorTest.SetTestInputVariable("A", Float32Bit, &A, 0);
    
    evaluatorTest.SetTestOutputVariable("ret1", Float32Bit, &ret1, 20);
    
    ErrorManagement::ErrorType expectedError;
    
    expectedError.internalSetupError = true;
    expectedError.notCompleted       = true;
    expectedError.outOfRange         = true;
    
    ASSERT_TRUE(evaluatorTest.TestExecute(rpnCode, expectedError, RuntimeEvaluator::safeMode));
}

/*---------------------------------------------------------------------------*/
/*                             ExtractVariables                              */
/*---------------------------------------------------------------------------*/

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

/*---------------------------------------------------------------------------*/
/*                                 Compile                                   */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_MixedSuccessful) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST int8 25\n"
            "CAST uint8\n"
            "WRITE OUT1\n"
            "READ IN1\n"
            "SIN\n"
            "WRITE OUT2\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetInputVariableType("IN1",   Float64Bit);
    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);
    evaluator.SetOutputVariableType("OUT2", Float64Bit);

    evaluatorTest.AddExpectedInputVariable("Constant@0",    SignedInteger8Bit,      0, NULL, false);
    evaluatorTest.AddExpectedInputVariable("IN1",           Float64Bit,             1, NULL, false);
    evaluatorTest.AddExpectedOutputVariable("OUT1",         UnsignedInteger8Bit,    3, NULL, true);
    evaluatorTest.AddExpectedOutputVariable("OUT2",         Float64Bit,             4, NULL, true);

    evaluatorTest.AddExpectedFunctionInMemory("READ",   "void",     "int8");
    evaluatorTest.AddExpectedVariableInMemory(0);
    evaluatorTest.AddExpectedFunctionInMemory("CAST",   "int8",    "uint8");
    evaluatorTest.AddExpectedFunctionInMemory("WRITE",  "uint8",    "void");
    evaluatorTest.AddExpectedVariableInMemory(3);
    evaluatorTest.AddExpectedFunctionInMemory("READ",   "void",     "float64");
    evaluatorTest.AddExpectedVariableInMemory(1);
    evaluatorTest.AddExpectedFunctionInMemory("SIN",    "float64",  "float64");
    evaluatorTest.AddExpectedFunctionInMemory("WRITE",  "float64",  "void");
    evaluatorTest.AddExpectedVariableInMemory(4);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::NoError, 6));
    ASSERT_TRUE(evaluator.Variable<uint8>(0) == 25);
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_ExternalInputSuccessful) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "WRITE OUT1\n"
    ;

    uint8 in1 = 12;
    DataMemoryAddress in1DataSize;
    VariableInformation *var;
    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.BrowseInputVariable(0, var);
    var->type = UnsignedInteger8Bit;
    var->externalLocation = &in1;
    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    in1DataSize = ByteSizeToDataMemorySize(sizeof(&in1));

    evaluatorTest.AddExpectedInputVariable("IN1",   UnsignedInteger8Bit,             0, &in1, false);
    evaluatorTest.AddExpectedOutputVariable("OUT1", UnsignedInteger8Bit,   in1DataSize, NULL, true);

    evaluatorTest.AddExpectedFunctionInMemory("RREAD",  "void",     "uint8");
    evaluatorTest.AddExpectedVariableInMemory(0);
    evaluatorTest.AddExpectedFunctionInMemory("WRITE",  "uint8",    "void");
    evaluatorTest.AddExpectedVariableInMemory(in1DataSize);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::NoError,  in1DataSize + 1));
    ASSERT_TRUE(evaluator.Variable<void *>(0) == &in1);
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_ExternalOutputSuccessful) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "WRITE OUT1\n"
            "WRITE OUT2\n"
    ;

    int64 out1;
    DataMemoryAddress out1DataSize;
    VariableInformation *var;
    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetInputVariableType("IN1",   UnsignedInteger16Bit);
    evaluator.SetInputVariableType("IN2",   SignedInteger64Bit);
    evaluator.BrowseOutputVariable(0, var);
    var->type = SignedInteger64Bit;
    var->externalLocation = &out1;
    evaluator.SetOutputVariableType("OUT2", UnsignedInteger16Bit);

    out1DataSize = ByteSizeToDataMemorySize(sizeof(&out1));

    evaluatorTest.AddExpectedInputVariable("IN1",   UnsignedInteger16Bit,   0,                  NULL, false);
    evaluatorTest.AddExpectedInputVariable("IN2",   SignedInteger64Bit,     1,                  NULL, false);
    evaluatorTest.AddExpectedOutputVariable("OUT1", SignedInteger64Bit,     3,                  &out1, true);
    evaluatorTest.AddExpectedOutputVariable("OUT2", UnsignedInteger16Bit,   3 + out1DataSize,   NULL, true);

    evaluatorTest.AddExpectedFunctionInMemory("READ",   "void",     "uint16");
    evaluatorTest.AddExpectedVariableInMemory(0);
    evaluatorTest.AddExpectedFunctionInMemory("READ",   "void",     "int64");
    evaluatorTest.AddExpectedVariableInMemory(1);
    evaluatorTest.AddExpectedFunctionInMemory("RWRITE", "int64",    "void");
    evaluatorTest.AddExpectedVariableInMemory(3);
    evaluatorTest.AddExpectedFunctionInMemory("WRITE",  "uint16",   "void");
    evaluatorTest.AddExpectedVariableInMemory(3 + out1DataSize);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::NoError,  out1DataSize + 4));
    ASSERT_TRUE(evaluator.Variable<void *>(3) == &out1);
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_FailedInputNoNumeric) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetInputVariableType("IN1", CharString);
    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::UnsupportedFeature, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_FailedOutputNoNumeric) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetInputVariableType("IN1", UnsignedInteger8Bit);
    evaluator.SetOutputVariableType("OUT1", CharString);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::UnsupportedFeature, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_FailedWrongCommand) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "WRONG\n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetInputVariableType("IN1", UnsignedInteger8Bit);
    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::UnsupportedFeature, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_FailedWrongInputType) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "SIN\n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetInputVariableType("IN1", UnsignedInteger64Bit);
    evaluator.SetOutputVariableType("OUT1", Float64Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::UnsupportedFeature, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_FailedWrongOutputType) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "SIN\n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetInputVariableType("IN1", Float64Bit);
    evaluator.SetOutputVariableType("OUT1", UnsignedInteger64Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::UnsupportedFeature, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_FailedTypeStackNotEmpty) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetInputVariableType("IN1", UnsignedInteger8Bit);
    evaluator.SetInputVariableType("IN2", UnsignedInteger8Bit);
    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::InternalSetupError, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_ReadSuccessfulLocalVariable) {
    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST uint8 25\n"
            "WRITE IN1\n"
            "CONST float64 3.14\n"
            "WRITE OUT1\n"
            "READ IN1\n"
            "WRITE OUT2\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetOutputVariableType("IN1",   UnsignedInteger8Bit);
    evaluator.SetOutputVariableType("OUT1", Float64Bit);
    evaluator.SetOutputVariableType("OUT2", UnsignedInteger8Bit);

    evaluatorTest.AddExpectedInputVariable("Constant@0",    UnsignedInteger8Bit,    0, NULL, false);
    evaluatorTest.AddExpectedInputVariable("Constant@1",    Float64Bit,             1, NULL, false);
    evaluatorTest.AddExpectedOutputVariable("IN1",          UnsignedInteger8Bit,    3, NULL, true);
    evaluatorTest.AddExpectedOutputVariable("OUT1",         Float64Bit,             4, NULL, true);
    evaluatorTest.AddExpectedOutputVariable("OUT2",         UnsignedInteger8Bit,    6, NULL, true);

    evaluatorTest.AddExpectedFunctionInMemory("READ",   "void",     "uint8");
    evaluatorTest.AddExpectedVariableInMemory(0);
    evaluatorTest.AddExpectedFunctionInMemory("WRITE",  "uint8",    "void");
    evaluatorTest.AddExpectedVariableInMemory(3);
    evaluatorTest.AddExpectedFunctionInMemory("READ",   "void",     "float64");
    evaluatorTest.AddExpectedVariableInMemory(1);
    evaluatorTest.AddExpectedFunctionInMemory("WRITE",  "float64",  "void");
    evaluatorTest.AddExpectedVariableInMemory(4);
    evaluatorTest.AddExpectedFunctionInMemory("READ",   "void",     "uint8");
    evaluatorTest.AddExpectedVariableInMemory(3);
    evaluatorTest.AddExpectedFunctionInMemory("WRITE",  "uint8",  "void");
    evaluatorTest.AddExpectedVariableInMemory(6);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::NoError, 7));
    ASSERT_TRUE(evaluator.Variable<uint8>(0) == 25);
    ASSERT_TRUE(evaluator.Variable<float64>(1) == 3.14);
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_ReadSuccessfulOutputFoundNotUsed) {
    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "WRITE OUT1\n"
            "CONST float64 3.14\n"
            "WRITE OUT2\n"
            "CONST uint8 25\n"
            "WRITE IN1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetInputVariableType("IN1",   UnsignedInteger8Bit);
    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);
    evaluator.SetOutputVariableType("OUT2", Float64Bit);
    evaluator.SetOutputVariableType("IN1",  UnsignedInteger8Bit);

    evaluatorTest.AddExpectedInputVariable("Constant@0",    Float64Bit,             0, NULL, false);
    evaluatorTest.AddExpectedInputVariable("Constant@2",    UnsignedInteger8Bit,    2, NULL, false);
    evaluatorTest.AddExpectedInputVariable("IN1",           UnsignedInteger8Bit,    3, NULL, false);
    evaluatorTest.AddExpectedOutputVariable("OUT1",         UnsignedInteger8Bit,    4, NULL, true);
    evaluatorTest.AddExpectedOutputVariable("OUT2",         Float64Bit,             5, NULL, true);
    evaluatorTest.AddExpectedOutputVariable("IN1",          UnsignedInteger8Bit,    7, NULL, true);

    evaluatorTest.AddExpectedFunctionInMemory("READ",   "void",     "uint8");
    evaluatorTest.AddExpectedVariableInMemory(3);
    evaluatorTest.AddExpectedFunctionInMemory("WRITE",  "uint8",    "void");
    evaluatorTest.AddExpectedVariableInMemory(4);
    evaluatorTest.AddExpectedFunctionInMemory("READ",   "void",     "float64");
    evaluatorTest.AddExpectedVariableInMemory(0);
    evaluatorTest.AddExpectedFunctionInMemory("WRITE",  "float64",  "void");
    evaluatorTest.AddExpectedVariableInMemory(5);
    evaluatorTest.AddExpectedFunctionInMemory("READ",   "void",     "uint8");
    evaluatorTest.AddExpectedVariableInMemory(2);
    evaluatorTest.AddExpectedFunctionInMemory("WRITE",  "uint8",  "void");
    evaluatorTest.AddExpectedVariableInMemory(7);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::NoError, 8));
    ASSERT_TRUE(evaluator.Variable<float64>(0) == 3.14);
    ASSERT_TRUE(evaluator.Variable<uint8>(2) == 25);
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_ReadFailedNoName) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ \n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetInputVariableType("IN1", UnsignedInteger8Bit);
    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::IllegalOperation, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_ReadFailedExceedingParameters) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1 IN1\n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetInputVariableType("IN1", UnsignedInteger8Bit);
    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::IllegalOperation, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_ReadFailedNotFound) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "WRITE OUT1\n"
    ;

    VariableInformation *var;
    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.BrowseInputVariable(0, var);
    var->name = "WRONG";
    var->type = UnsignedInteger8Bit;
    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::UnsupportedFeature, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_WriteSuccessfulTypeDeduction) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST int8 25\n"
            "CAST uint8\n"
            "WRITE OUT1\n"
            "READ IN1\n"
            "SIN\n"
            "WRITE OUT2\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetInputVariableType("IN1",   Float64Bit);

    evaluatorTest.AddExpectedInputVariable("Constant@0",    SignedInteger8Bit,      0, NULL, false);
    evaluatorTest.AddExpectedInputVariable("IN1",           Float64Bit,             1, NULL, false);
    evaluatorTest.AddExpectedOutputVariable("OUT1",         UnsignedInteger8Bit,    3, NULL, true);
    evaluatorTest.AddExpectedOutputVariable("OUT2",         Float64Bit,             5, NULL, true);

    evaluatorTest.AddExpectedFunctionInMemory("READ",   "void",     "int8");
    evaluatorTest.AddExpectedVariableInMemory(0);
    evaluatorTest.AddExpectedFunctionInMemory("CAST",   "int8",     "uint8");
    evaluatorTest.AddExpectedFunctionInMemory("WRITE",  "uint8",    "void");
    evaluatorTest.AddExpectedVariableInMemory(3);
    evaluatorTest.AddExpectedFunctionInMemory("READ",   "void",     "float64");
    evaluatorTest.AddExpectedVariableInMemory(1);
    evaluatorTest.AddExpectedFunctionInMemory("SIN",    "float64",  "float64");
    evaluatorTest.AddExpectedFunctionInMemory("WRITE",  "float64",  "void");
    evaluatorTest.AddExpectedVariableInMemory(5);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::NoError, 7));
    ASSERT_TRUE(evaluator.Variable<uint8>(0) == 25);
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_WriteFailedTypeDeduction) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST int8 25\n"
            "CAST uint8\n"
            "WRITE OUT1\n"
            "WRITE OUT2\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetInputVariableType("IN1",   Float64Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::FatalError, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_WriteFailedNoName) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "WRITE \n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetInputVariableType("IN1", UnsignedInteger8Bit);
    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::IllegalOperation, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_WriteFailedExceedingParameters) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "WRITE OUT1 OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetInputVariableType("IN1", UnsignedInteger8Bit);
    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::IllegalOperation, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_WriteFailedNotFound) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "READ IN1\n"
            "WRITE OUT1\n"
    ;

    VariableInformation *var;
    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetInputVariableType("IN1", UnsignedInteger8Bit);
    evaluator.BrowseOutputVariable(0, var);
    var->name = "WRONG";
    var->type = UnsignedInteger8Bit;

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::UnsupportedFeature, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_ConstFailedNoValue) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST int8\n"
            "CAST uint8\n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::IllegalOperation, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_ConstFailedNoTypeNoValue) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST \n"
            "CAST uint8\n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::IllegalOperation, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_ConstFailedExceedingParameters) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST int8 25 25\n"
            "CAST uint8\n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::IllegalOperation, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_ConstFailedNoNumeric) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST string 25\n"
            "CAST uint8\n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::UnsupportedFeature, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_ConstFailedValueParsingErrorCharacter) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST int8 a\n"
            "CAST uint8\n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::FatalError, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_ConstFailedValueParsingErrorFloat) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST int8 8.5\n"
            "CAST uint8\n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::FatalError, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_CastFailedNoType) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST int8 25\n"
            "CAST \n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::IllegalOperation, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_CastFailedExceedingParameters) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST int8 25\n"
            "CAST uint8 uint8\n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::IllegalOperation, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorGTest, TestCompile_CastFailedNoNumeric) {

    RuntimeEvaluatorTest evaluatorTest;

    CCString rpnCode=
            "CONST int8 25\n"
            "CAST string\n"
            "WRITE OUT1\n"
    ;

    RuntimeEvaluator evaluator(rpnCode);

    evaluator.ExtractVariables();

    evaluator.SetOutputVariableType("OUT1", UnsignedInteger8Bit);

    ASSERT_TRUE(evaluatorTest.TestCompile(evaluator, ErrorManagement::UnsupportedFeature, 0));
}
