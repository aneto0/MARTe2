/**
 * @file RuntimeEvaluatorFunctionsGTest.cpp
 * @brief Source file for class RuntimeEvaluatorFunctionsGTest
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
 * the class RuntimeEvaluatorFunctionsGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RuntimeEvaluatorFunctionsTest.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestDefaultConstructor) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;
    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestDefaultConstructor());
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestDupFunctionTypes) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    StreamString expectedFunctionTypes = ""
            "Function1 = {"
            "    Inputs = {"
            "        arg1 = float64"
            "    }"
            "    Outputs = {"
            "        arg1 = float64"
            "        arg2 = float64"
            "    }"
            "}"
            "Function2 = {"
            "    Inputs = {"
            "        arg1 = float32"
            "    }"
            "    Outputs = {"
            "        arg1 = float32"
            "        arg2 = float32"
            "    }"
            "}"
            "Function3 = {"
            "    Inputs = {"
            "        arg1 = uint64"
            "    }"
            "    Outputs = {"
            "        arg1 = uint64"
            "        arg2 = uint64"
            "    }"
            "}"
            "Function4 = {"
            "    Inputs = {"
            "        arg1 = int64"
            "    }"
            "    Outputs = {"
            "        arg1 = int64"
            "        arg2 = int64"
            "    }"
            "}"
            "Function5 = {"
            "    Inputs = {"
            "        arg1 = uint32"
            "    }"
            "    Outputs = {"
            "        arg1 = uint32"
            "        arg2 = uint32"
            "    }"
            "}"
            "Function6 = {"
            "    Inputs = {"
            "        arg1 = int32"
            "    }"
            "    Outputs = {"
            "        arg1 = int32"
            "        arg2 = int32"
            "    }"
            "}"
            "Function7 = {"
            "    Inputs = {"
            "        arg1 = uint16"
            "    }"
            "    Outputs = {"
            "        arg1 = uint16"
            "        arg2 = uint16"
            "    }"
            "}"
            "Function8 = {"
            "    Inputs = {"
            "        arg1 = int16"
            "    }"
            "    Outputs = {"
            "        arg1 = int16"
            "        arg2 = int16"
            "    }"
            "}"
            "Function9 = {"
            "    Inputs = {"
            "        arg1 = uint8"
            "    }"
            "    Outputs = {"
            "        arg1 = uint8"
            "        arg2 = uint8"
            "    }"
            "}"
            "Function10 = {"
            "    Inputs = {"
            "        arg1 = int8"
            "    }"
            "    Outputs = {"
            "        arg1 = int8"
            "        arg2 = int8"
            "    }"
            "}";

    runtimeEvaluatorFunctionsTest.ParseFunctions(expectedFunctionTypes);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestFunctionTypes("DUP", 1, 2));
}


TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestReadFunctionTypes) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    StreamString expectedFunctionTypes = ""
            "Function1 = {"
            "    Outputs = {"
            "        arg1 = float64"
            "    }"
            "}"
            "Function2 = {"
            "    Outputs = {"
            "        arg1 = float32"
            "    }"
            "}"
            "Function3 = {"
            "    Outputs = {"
            "        arg1 = uint64"
            "    }"
            "}"
            "Function4 = {"
            "    Outputs = {"
            "        arg1 = int64"
            "    }"
            "}"
            "Function5 = {"
            "    Outputs = {"
            "        arg1 = uint32"
            "    }"
            "}"
            "Function6 = {"
            "    Outputs = {"
            "        arg1 = int32"
            "    }"
            "}"
            "Function7 = {"
            "    Outputs = {"
            "        arg1 = uint16"
            "    }"
            "}"
            "Function8 = {"
            "    Outputs = {"
            "        arg1 = int16"
            "    }"
            "}"
            "Function9 = {"
            "    Outputs = {"
            "        arg1 = uint8"
            "    }"
            "}"
            "Function10 = {"
            "    Outputs = {"
            "        arg1 = int8"
            "    }"
            "}";

    runtimeEvaluatorFunctionsTest.ParseFunctions(expectedFunctionTypes);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestFunctionTypes("READ", 0, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestWriteFunctionTypes) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    runtimeEvaluatorFunctionsTest.AddFunctionsWithOnlyInputs(1, "float64");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithOnlyInputs(1, "float32");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithOnlyInputs(1, "uint16");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithOnlyInputs(1, "int16");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithOnlyInputs(1, "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithOnlyInputs(1, "int8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithOnlyInputs(4, "uint64");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithOnlyInputs(8, "int64");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithOnlyInputs(3, "uint32");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithOnlyInputs(6, "int32");

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestFunctionTypes("WRITE", 1, 0));
}

//TODO IMPLEMENT THIS

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestCastFunctionTypes) {

}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestSinFunctionTypes) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    StreamString expectedFunctionTypes = ""
            "Function1 = {"
            "    Inputs = {"
            "        arg1 = float32"
            "    }"
            "    Outputs = {"
            "        arg1 = float32"
            "    }"
            "}"
            "Function2 = {"
            "    Inputs = {"
            "        arg1 = float64"
            "    }"
            "    Outputs = {"
            "        arg1 = float64"
            "    }"
            "}";

    runtimeEvaluatorFunctionsTest.ParseFunctions(expectedFunctionTypes);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestFunctionTypes("SIN", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestCosFunctionTypes) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    StreamString expectedFunctionTypes = ""
            "Function1 = {"
            "    Inputs = {"
            "        arg1 = float32"
            "    }"
            "    Outputs = {"
            "        arg1 = float32"
            "    }"
            "}"
            "Function2 = {"
            "    Inputs = {"
            "        arg1 = float64"
            "    }"
            "    Outputs = {"
            "        arg1 = float64"
            "    }"
            "}";

    runtimeEvaluatorFunctionsTest.ParseFunctions(expectedFunctionTypes);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestFunctionTypes("COS", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTanFunctionTypes) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    StreamString expectedFunctionTypes = ""
            "Function1 = {"
            "    Inputs = {"
            "        arg1 = float32"
            "    }"
            "    Outputs = {"
            "        arg1 = float32"
            "    }"
            "}"
            "Function2 = {"
            "    Inputs = {"
            "        arg1 = float64"
            "    }"
            "    Outputs = {"
            "        arg1 = float64"
            "    }"
            "}";

    runtimeEvaluatorFunctionsTest.ParseFunctions(expectedFunctionTypes);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestFunctionTypes("TAN", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestExpFunctionTypes) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    StreamString expectedFunctionTypes = ""
            "Function1 = {"
            "    Inputs = {"
            "        arg1 = float32"
            "    }"
            "    Outputs = {"
            "        arg1 = float32"
            "    }"
            "}"
            "Function2 = {"
            "    Inputs = {"
            "        arg1 = float64"
            "    }"
            "    Outputs = {"
            "        arg1 = float64"
            "    }"
            "}";

    runtimeEvaluatorFunctionsTest.ParseFunctions(expectedFunctionTypes);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestFunctionTypes("EXP", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestLogFunctionTypes) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    StreamString expectedFunctionTypes = ""
            "Function1 = {"
            "    Inputs = {"
            "        arg1 = float32"
            "    }"
            "    Outputs = {"
            "        arg1 = float32"
            "    }"
            "}"
            "Function2 = {"
            "    Inputs = {"
            "        arg1 = float64"
            "    }"
            "    Outputs = {"
            "        arg1 = float64"
            "    }"
            "}";

    runtimeEvaluatorFunctionsTest.ParseFunctions(expectedFunctionTypes);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestFunctionTypes("LOG", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestLog10FunctionTypes) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    StreamString expectedFunctionTypes = ""
            "Function1 = {"
            "    Inputs = {"
            "        arg1 = float32"
            "    }"
            "    Outputs = {"
            "        arg1 = float32"
            "    }"
            "}"
            "Function2 = {"
            "    Inputs = {"
            "        arg1 = float64"
            "    }"
            "    Outputs = {"
            "        arg1 = float64"
            "    }"
            "}";

    runtimeEvaluatorFunctionsTest.ParseFunctions(expectedFunctionTypes);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestFunctionTypes("LOG10", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestPowFunctionTypes) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    StreamString expectedFunctionTypes = ""
            "Function1 = {"
            "    Inputs = {"
            "        arg1 = float32"
            "        arg2 = float32"
            "    }"
            "    Outputs = {"
            "        arg1 = float32"
            "    }"
            "}"
            "Function2 = {"
            "    Inputs = {"
            "        arg1 = float64"
            "        arg2 = float64"
            "    }"
            "    Outputs = {"
            "        arg1 = float64"
            "    }"
            "}";

    runtimeEvaluatorFunctionsTest.ParseFunctions(expectedFunctionTypes);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestFunctionTypes("POW", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestEqFunctionTypes) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    StreamString expectedFunctionTypes = ""
            "Function1 = {"
            "    Inputs = {"
            "        arg1 = float64"
            "        arg2 = float64"
            "    }"
            "    Outputs = {"
            "        arg1 = uint8"
            "    }"
            "}"
            "Function2 = {"
            "    Inputs = {"
            "        arg1 = float32"
            "        arg2 = float32"
            "    }"
            "    Outputs = {"
            "        arg1 = uint8"
            "    }"
            "}"
            "Function3 = {"
            "    Inputs = {"
            "        arg1 = uint64"
            "        arg2 = uint64"
            "    }"
            "    Outputs = {"
            "        arg1 = uint8"
            "    }"
            "}"
            "Function4 = {"
            "    Inputs = {"
            "        arg1 = int64"
            "        arg2 = int64"
            "    }"
            "    Outputs = {"
            "        arg1 = uint8"
            "    }"
            "}"
            "Function5 = {"
            "    Inputs = {"
            "        arg1 = uint32"
            "        arg2 = uint32"
            "    }"
            "    Outputs = {"
            "        arg1 = uint8"
            "    }"
            "}"
            "Function6 = {"
            "    Inputs = {"
            "        arg1 = int32"
            "        arg2 = int32"
            "    }"
            "    Outputs = {"
            "        arg1 = uint8"
            "    }"
            "}"
            "Function7 = {"
            "    Inputs = {"
            "        arg1 = uint16"
            "        arg2 = uint16"
            "    }"
            "    Outputs = {"
            "        arg1 = uint8"
            "    }"
            "}"
            "Function8 = {"
            "    Inputs = {"
            "        arg1 = int16"
            "        arg2 = int16"
            "    }"
            "    Outputs = {"
            "        arg1 = uint8"
            "    }"
            "}"
            "Function9 = {"
            "    Inputs = {"
            "        arg1 = uint8"
            "        arg2 = uint8"
            "    }"
            "    Outputs = {"
            "        arg1 = uint8"
            "    }"
            "}"
            "Function10 = {"
            "    Inputs = {"
            "        arg1 = int8"
            "        arg2 = int8"
            "    }"
            "    Outputs = {"
            "        arg1 = uint8"
            "    }"
            "}";

    runtimeEvaluatorFunctionsTest.ParseFunctions(expectedFunctionTypes);

    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("int8",   "int32",    "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("int16",  "int32",    "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("uint8",  "int32",    "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("uint16", "int32",    "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("uint32", "int32",    "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("int8",   "int64",    "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("int16",  "int64",    "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("int32",  "int64",    "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("uint8",  "int64",    "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("uint16", "int64",    "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("uint32", "int64",    "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("uint64", "int64",    "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("uint8",  "uint32",   "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("uint16", "uint32",   "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("int8",   "uint64",   "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("int16",  "uint64",   "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("int32",  "uint64",   "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("uint8",  "uint64",   "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("uint16", "uint64",   "uint8");
    runtimeEvaluatorFunctionsTest.AddFunctionsWithInputCombination("uint32", "uint64",   "uint8");

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestFunctionTypes("EQ", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeSuccessful) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestTryConsume(functionUT, "Test", typeStack, false, true, 1, 2));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeSuccessful_NoInputs) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    TypeDescriptor functionTypes[] = {Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 0, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestTryConsume(functionUT, "Test", typeStack, false, true, 0, 2));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeSuccessful_NoOutputs) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    TypeDescriptor functionTypes[] = {Float32Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 0, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestTryConsume(functionUT, "Test", typeStack, false, true, 1, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeSuccessful_MultipleInputs) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    TypeDescriptor functionTypes[] = {Float32Bit, UnsignedInteger64Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 2, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(UnsignedInteger64Bit);
    typeStack.Push(Float32Bit);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestTryConsume(functionUT, "Test", typeStack, false, true, 3, 2));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeSuccessful_MultipleOutputs) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit, UnsignedInteger32Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 2, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestTryConsume(functionUT, "Test", typeStack, false, true, 1, 3));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeSuccessful_StackLeftover) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(UnsignedInteger8Bit);
    typeStack.Push(Float32Bit);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestTryConsume(functionUT, "Test", typeStack, false, true, 2, 3));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeSuccessful_MatchOutput) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);
    typeStack.Push(Float64Bit);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestTryConsume(functionUT, "Test", typeStack, true, true, 1, 2));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeFailed_WrongName) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestTryConsume(functionUT, "WrongName", typeStack, false, false, 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeFailed_MissingInput) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestTryConsume(functionUT, "Test", typeStack, false, false, 0, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeFailed_WrongInputType) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(SignedInteger32Bit);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestTryConsume(functionUT, "Test", typeStack, false, false, 1, 1));
}


TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeFailed_MatchOutputMissingOutput) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestTryConsume(functionUT, "Test", typeStack, true, false, 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeFailed_MatchOutputWrongOutput) {
    RuntimeEvaluatorFunctionsTest runtimeEvaluatorFunctionsTest;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);
    typeStack.Push(Float32Bit);

    ASSERT_TRUE(runtimeEvaluatorFunctionsTest.TestTryConsume(functionUT, "Test", typeStack, true, false, 2, 2));
}
