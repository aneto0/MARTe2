/**
 * @file PseudoCodeFunctionsGTest.cpp
 * @brief Source file for class PseudoCodeFunctionsGTest
 * @date 06/05/2020
 * @author Didac Magrinya
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
 * the class PseudoCodeFunctionsGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "PseudoCodeFunctionsTest.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestDefaultConstructor) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;
    ASSERT_TRUE(pseudoCodeFunctionsTest.TestDefaultConstructor());
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestSinFunctionRecordTypes) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor expectedInputTypes[][1]  = {{Float32Bit}, {Float64Bit}};
    TypeDescriptor expectedOutputTypes[][1] = {{Float32Bit}, {Float64Bit}};

    ASSERT_TRUE((pseudoCodeFunctionsTest.TestFunctionRecordTypes<2, 1, 1>("SIN", expectedInputTypes, expectedOutputTypes)));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestTryConsumeSuccessful) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor recordTypes[] = {Float32Bit, Float64Bit};
    PseudoCode::FunctionRecord functionRecordUT("Test", 1, 1, &(recordTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestTryConsume(functionRecordUT, "Test", typeStack, false, true, 1, 2));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestTryConsumeSuccessful_NoInputs) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor recordTypes[] = {Float64Bit};
    PseudoCode::FunctionRecord functionRecordUT("Test", 0, 1, &(recordTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestTryConsume(functionRecordUT, "Test", typeStack, false, true, 0, 2));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestTryConsumeSuccessful_NoOutputs) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor recordTypes[] = {Float32Bit};
    PseudoCode::FunctionRecord functionRecordUT("Test", 1, 0, &(recordTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestTryConsume(functionRecordUT, "Test", typeStack, false, true, 1, 0));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestTryConsumeSuccessful_MultipleInputs) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor recordTypes[] = {Float32Bit, UnsignedInteger64Bit, Float64Bit};
    PseudoCode::FunctionRecord functionRecordUT("Test", 2, 1, &(recordTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(UnsignedInteger64Bit);
    typeStack.Push(Float32Bit);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestTryConsume(functionRecordUT, "Test", typeStack, false, true, 3, 2));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestTryConsumeSuccessful_MultipleOutputs) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor recordTypes[] = {Float32Bit, Float64Bit, UnsignedInteger32Bit};
    PseudoCode::FunctionRecord functionRecordUT("Test", 1, 2, &(recordTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestTryConsume(functionRecordUT, "Test", typeStack, false, true, 1, 3));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestTryConsumeSuccessful_StackLeftover) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor recordTypes[] = {Float32Bit, Float64Bit};
    PseudoCode::FunctionRecord functionRecordUT("Test", 1, 1, &(recordTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(UnsignedInteger8Bit);
    typeStack.Push(Float32Bit);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestTryConsume(functionRecordUT, "Test", typeStack, false, true, 2, 3));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestTryConsumeSuccessful_MatchOutput) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor recordTypes[] = {Float32Bit, Float64Bit};
    PseudoCode::FunctionRecord functionRecordUT("Test", 1, 1, &(recordTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);
    typeStack.Push(Float64Bit);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestTryConsume(functionRecordUT, "Test", typeStack, true, true, 1, 2));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestTryConsumeFailed_WrongName) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor recordTypes[] = {Float32Bit, Float64Bit};
    PseudoCode::FunctionRecord functionRecordUT("Test", 1, 1, &(recordTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestTryConsume(functionRecordUT, "WrongName", typeStack, false, false, 1, 1));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestTryConsumeFailed_MissingInput) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor recordTypes[] = {Float32Bit, Float64Bit};
    PseudoCode::FunctionRecord functionRecordUT("Test", 1, 1, &(recordTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestTryConsume(functionRecordUT, "Test", typeStack, false, false, 0, 0));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestTryConsumeFailed_WrongInputType) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor recordTypes[] = {Float32Bit, Float64Bit};
    PseudoCode::FunctionRecord functionRecordUT("Test", 1, 1, &(recordTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(SignedInteger32Bit);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestTryConsume(functionRecordUT, "Test", typeStack, false, false, 1, 1));
}


TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestTryConsumeFailed_MatchOutputMissingOutput) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor recordTypes[] = {Float32Bit, Float64Bit};
    PseudoCode::FunctionRecord functionRecordUT("Test", 1, 1, &(recordTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestTryConsume(functionRecordUT, "Test", typeStack, true, false, 1, 1));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestTryConsumeFailed_MatchOutputWrongOutput) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor recordTypes[] = {Float32Bit, Float64Bit};
    PseudoCode::FunctionRecord functionRecordUT("Test", 1, 1, &(recordTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);
    typeStack.Push(Float32Bit);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestTryConsume(functionRecordUT, "Test", typeStack, true, false, 2, 2));
}
