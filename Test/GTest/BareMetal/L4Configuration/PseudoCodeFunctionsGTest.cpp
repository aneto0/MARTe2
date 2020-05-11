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

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestDupFunctionRecordTypes) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor expectedInputTypes[][1]  = {{Float64Bit},             {Float32Bit},             {UnsignedInteger64Bit},                       {SignedInteger64Bit},                     {UnsignedInteger32Bit},                       {SignedInteger32Bit},                     {UnsignedInteger16Bit},                       {SignedInteger16Bit},                     {UnsignedInteger8Bit},                      {SignedInteger8Bit}};
    TypeDescriptor expectedOutputTypes[][2] = {{Float64Bit, Float64Bit}, {Float32Bit, Float32Bit}, {UnsignedInteger64Bit, UnsignedInteger64Bit}, {SignedInteger64Bit, SignedInteger64Bit}, {UnsignedInteger32Bit, UnsignedInteger32Bit}, {SignedInteger32Bit, SignedInteger32Bit}, {UnsignedInteger16Bit, UnsignedInteger16Bit}, {SignedInteger16Bit, SignedInteger16Bit}, {UnsignedInteger8Bit, UnsignedInteger8Bit}, {SignedInteger8Bit, SignedInteger8Bit}};

    StaticList<TypeDescriptor*> expectedInputTypesList, expectedOutputTypesList;
    pseudoCodeFunctionsTest.ConstructTypeList<10, 1>(expectedInputTypes, expectedInputTypesList);
    pseudoCodeFunctionsTest.ConstructTypeList<10, 2>(expectedOutputTypes, expectedOutputTypesList);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestFunctionRecordTypes("DUP", 1, expectedInputTypesList, 2, expectedOutputTypesList));
}


TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestReadFunctionRecordTypes) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor expectedOutputTypes[][1]  = {{Float64Bit}, {Float32Bit}, {UnsignedInteger64Bit}, {SignedInteger64Bit}, {UnsignedInteger32Bit}, {SignedInteger32Bit}, {UnsignedInteger16Bit}, {SignedInteger16Bit}, {UnsignedInteger8Bit}, {SignedInteger8Bit}};

    StaticList<TypeDescriptor*> expectedInputTypesList, expectedOutputTypesList;
    pseudoCodeFunctionsTest.ConstructTypeList<10, 1>(expectedOutputTypes, expectedOutputTypesList);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestFunctionRecordTypes("READ", 0, expectedInputTypesList, 1, expectedOutputTypesList));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestWriteFunctionRecordTypes) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor expectedInputTypes[][1]  = {{Float64Bit}, {Float32Bit}, {UnsignedInteger64Bit}, {SignedInteger64Bit}, {UnsignedInteger32Bit}, {SignedInteger32Bit}, {UnsignedInteger16Bit}, {SignedInteger16Bit}, {UnsignedInteger8Bit}, {SignedInteger8Bit},
                                               {UnsignedInteger64Bit}, {UnsignedInteger64Bit}, {UnsignedInteger64Bit}, {SignedInteger64Bit}, {SignedInteger64Bit}, {SignedInteger64Bit}, {SignedInteger64Bit}, {SignedInteger64Bit}, {SignedInteger64Bit}, {SignedInteger64Bit}, {UnsignedInteger32Bit}, {UnsignedInteger32Bit}, {SignedInteger32Bit}, {SignedInteger32Bit}, {SignedInteger32Bit}, {SignedInteger32Bit}, {SignedInteger32Bit}};

    StaticList<TypeDescriptor*> expectedInputTypesList, expectedOutputTypesList;
    pseudoCodeFunctionsTest.ConstructTypeList<27, 1>(expectedInputTypes, expectedInputTypesList);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestFunctionRecordTypes("WRITE", 1, expectedInputTypesList, 0, expectedOutputTypesList));
}

//TODO IMPLEMENT THIS
/*
TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestCastFunctionRecordTypes) {

}
*/

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestSinFunctionRecordTypes) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor expectedInputTypes[][1]  = {{Float32Bit}, {Float64Bit}};
    TypeDescriptor expectedOutputTypes[][1] = {{Float32Bit}, {Float64Bit}};

    StaticList<TypeDescriptor*> expectedInputTypesList, expectedOutputTypesList;
    pseudoCodeFunctionsTest.ConstructTypeList<2, 1>(expectedInputTypes, expectedInputTypesList);
    pseudoCodeFunctionsTest.ConstructTypeList<2, 1>(expectedOutputTypes, expectedOutputTypesList);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestFunctionRecordTypes("SIN", 1, expectedInputTypesList, 1, expectedOutputTypesList));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestCosFunctionRecordTypes) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor expectedInputTypes[][1]  = {{Float32Bit}, {Float64Bit}};
    TypeDescriptor expectedOutputTypes[][1] = {{Float32Bit}, {Float64Bit}};

    StaticList<TypeDescriptor*> expectedInputTypesList, expectedOutputTypesList;
    pseudoCodeFunctionsTest.ConstructTypeList<2, 1>(expectedInputTypes, expectedInputTypesList);
    pseudoCodeFunctionsTest.ConstructTypeList<2, 1>(expectedOutputTypes, expectedOutputTypesList);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestFunctionRecordTypes("COS", 1, expectedInputTypesList, 1, expectedOutputTypesList));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestTanFunctionRecordTypes) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor expectedInputTypes[][1]  = {{Float32Bit}, {Float64Bit}};
    TypeDescriptor expectedOutputTypes[][1] = {{Float32Bit}, {Float64Bit}};

    StaticList<TypeDescriptor*> expectedInputTypesList, expectedOutputTypesList;
    pseudoCodeFunctionsTest.ConstructTypeList<2, 1>(expectedInputTypes, expectedInputTypesList);
    pseudoCodeFunctionsTest.ConstructTypeList<2, 1>(expectedOutputTypes, expectedOutputTypesList);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestFunctionRecordTypes("TAN", 1, expectedInputTypesList, 1, expectedOutputTypesList));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestExpFunctionRecordTypes) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor expectedInputTypes[][1]  = {{Float32Bit}, {Float64Bit}};
    TypeDescriptor expectedOutputTypes[][1] = {{Float32Bit}, {Float64Bit}};

    StaticList<TypeDescriptor*> expectedInputTypesList, expectedOutputTypesList;
    pseudoCodeFunctionsTest.ConstructTypeList<2, 1>(expectedInputTypes, expectedInputTypesList);
    pseudoCodeFunctionsTest.ConstructTypeList<2, 1>(expectedOutputTypes, expectedOutputTypesList);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestFunctionRecordTypes("EXP", 1, expectedInputTypesList, 1, expectedOutputTypesList));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestLogFunctionRecordTypes) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor expectedInputTypes[][1]  = {{Float32Bit}, {Float64Bit}};
    TypeDescriptor expectedOutputTypes[][1] = {{Float32Bit}, {Float64Bit}};

    StaticList<TypeDescriptor*> expectedInputTypesList, expectedOutputTypesList;
    pseudoCodeFunctionsTest.ConstructTypeList<2, 1>(expectedInputTypes, expectedInputTypesList);
    pseudoCodeFunctionsTest.ConstructTypeList<2, 1>(expectedOutputTypes, expectedOutputTypesList);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestFunctionRecordTypes("LOG", 1, expectedInputTypesList, 1, expectedOutputTypesList));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestLog10FunctionRecordTypes) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor expectedInputTypes[][1]  = {{Float32Bit}, {Float64Bit}};
    TypeDescriptor expectedOutputTypes[][1] = {{Float32Bit}, {Float64Bit}};

    StaticList<TypeDescriptor*> expectedInputTypesList, expectedOutputTypesList;
    pseudoCodeFunctionsTest.ConstructTypeList<2, 1>(expectedInputTypes, expectedInputTypesList);
    pseudoCodeFunctionsTest.ConstructTypeList<2, 1>(expectedOutputTypes, expectedOutputTypesList);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestFunctionRecordTypes("LOG10", 1, expectedInputTypesList, 1, expectedOutputTypesList));
}

TEST(BareMetal_L4Configuration_PseudoCodeFunctionsGTest,TestPowFunctionRecordTypes) {
    PseudoCodeFunctionsTest pseudoCodeFunctionsTest;

    TypeDescriptor expectedInputTypes[][2]  = {{Float32Bit, Float32Bit}, {Float64Bit, Float64Bit}};
    TypeDescriptor expectedOutputTypes[][1] = {{Float32Bit},             {Float64Bit}};

    StaticList<TypeDescriptor*> expectedInputTypesList, expectedOutputTypesList;
    pseudoCodeFunctionsTest.ConstructTypeList<2, 2>(expectedInputTypes, expectedInputTypesList);
    pseudoCodeFunctionsTest.ConstructTypeList<2, 1>(expectedOutputTypes, expectedOutputTypesList);

    ASSERT_TRUE(pseudoCodeFunctionsTest.TestFunctionRecordTypes("POW", 2, expectedInputTypesList, 1, expectedOutputTypesList));
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
