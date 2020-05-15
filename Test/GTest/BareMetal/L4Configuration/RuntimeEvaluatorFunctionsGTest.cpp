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
    RuntimeEvaluatorFunctionsTest test;
    ASSERT_TRUE(test.TestDefaultConstructor());
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestDupFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add1In2OutFunction("float64",  "float64",  "float64");
    test.Add1In2OutFunction("float32",  "float32",  "float32");
    test.Add1In2OutFunction("uint64",   "uint64",   "uint64");
    test.Add1In2OutFunction("int64",    "int64",    "int64");
    test.Add1In2OutFunction("uint32",   "uint32",   "uint32");
    test.Add1In2OutFunction("int32",    "int32",    "int32");
    test.Add1In2OutFunction("uint16",   "uint16",   "uint16");
    test.Add1In2OutFunction("int16",    "int16",    "int16");
    test.Add1In2OutFunction("uint8",    "uint8",    "uint8");
    test.Add1In2OutFunction("int8",     "int8",     "int8");

    ASSERT_TRUE(test.TestFunctionTypes("DUP", 1, 2));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestDupExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "CONST float64 -152.3\n"
            "DUP\n"
            "WRITE RES1\n"
            "WRITE RES2\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));

    float64 expectedResults[] = {-152.3, -152.3};

    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, 2, expectedResults));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestReadFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add1OutFunction("float64");
    test.Add1OutFunction("float32");
    test.Add1OutFunction("uint64");
    test.Add1OutFunction("int64");
    test.Add1OutFunction("uint32");
    test.Add1OutFunction("int32");
    test.Add1OutFunction("uint16");
    test.Add1OutFunction("int16");
    test.Add1OutFunction("uint8");
    test.Add1OutFunction("int8");

    ASSERT_TRUE(test.TestFunctionTypes("READ", 0, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestReadExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, Float64Bit, Float64Bit));

    float64 inputs[] = {-152.3};

    test.SetInputs(context, inputs);
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, -152.3));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestWriteFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add1InFunction("float64");
    test.Add1InFunction("float32");
    test.Add1InFunction("uint16");
    test.Add1InFunction("int16");
    test.Add1InFunction("uint8");
    test.Add1InFunction("int8");
    for (uint8 i = 0; i < 4; ++i) {
        test.Add1InFunction("uint64");
    }
    for (uint8 i = 0; i < 8; ++i) {
        test.Add1InFunction("int64");
    }
    for (uint8 i = 0; i < 3; ++i) {
        test.Add1InFunction("uint32");
    }
    for (uint8 i = 0; i < 6; ++i) {
        test.Add1InFunction("int32");
    }

    ASSERT_TRUE(test.TestFunctionTypes("WRITE", 1, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestWriteExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "CONST float64 -152.3\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));

    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, -152.3));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestWriteConvExecution_Successful) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "CONST int64 -115\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger8Bit));

    ASSERT_TRUE(test.TestIntFunctionExecution<int8>(context, -115));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestWriteConvExecution_FailedOutOfRange) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "CONST int64 -152\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, UnsignedInteger8Bit));

    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0, ErrorManagement::OutOfRange));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestCastFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;
    CCString typeNames[] = {"float64", "float32", "uint64", "int64", "uint32", "int32", "uint16", "int16", "uint8", "int8"};

    for (uint8 i = 0; i < 10; ++i) {
        for (uint8 j = 0; j < 10; ++j) {
            test.Add1In1OutFunction(typeNames[i], typeNames[j]);
        }
    }

    ASSERT_TRUE(test.TestFunctionTypes("CAST", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestCastExecution_Successful) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "CONST float64 1.57079\n"
            "CAST uint8\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, UnsignedInteger8Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestCastExecution_FailedOutOfRange) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "CONST uint16 256\n"
            "CAST uint8\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, UnsignedInteger8Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int8>(context, 255, ErrorManagement::OutOfRange));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestSinFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add1In1OutFunction("float32", "float32");
    test.Add1In1OutFunction("float64", "float64");

    ASSERT_TRUE(test.TestFunctionTypes("SIN", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestSinExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "CONST float64 1.57079\n"
            "SIN\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestCosFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add1In1OutFunction("float32", "float32");
    test.Add1In1OutFunction("float64", "float64");

    ASSERT_TRUE(test.TestFunctionTypes("COS", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestCosExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "CONST float64 0\n"
            "COS\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTanFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add1In1OutFunction("float32", "float32");
    test.Add1In1OutFunction("float64", "float64");

    ASSERT_TRUE(test.TestFunctionTypes("TAN", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTanExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "CONST float64 0.78539816339\n"
            "TAN\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestExpFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add1In1OutFunction("float32", "float32");
    test.Add1In1OutFunction("float64", "float64");

    ASSERT_TRUE(test.TestFunctionTypes("EXP", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestExpExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "CONST float64 0\n"
            "EXP\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestLogFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add1In1OutFunction("float32", "float32");
    test.Add1In1OutFunction("float64", "float64");

    ASSERT_TRUE(test.TestFunctionTypes("LOG", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestLogExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "CONST float64 1\n"
            "LOG\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestLog10FunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add1In1OutFunction("float32", "float32");
    test.Add1In1OutFunction("float64", "float64");

    ASSERT_TRUE(test.TestFunctionTypes("LOG10", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestLog10Execution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "CONST float64 1000\n"
            "LOG10\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, 3));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestPowFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add2In1OutFunction("float32",  "float32",  "float32");
    test.Add2In1OutFunction("float64",  "float64",  "float64");

    ASSERT_TRUE(test.TestFunctionTypes("POW", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestPowExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "CONST float64 10.2\n"
            "CONST float64 3\n"
            "POW\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, 1061.208));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestEqFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add2In1OutFunction("float64",  "float64",  "uint8");
    test.Add2In1OutFunction("float32",  "float32",  "uint8");
    test.Add2In1OutFunction("uint64",   "uint64",   "uint8");
    test.Add2In1OutFunction("int64",    "int64",    "uint8");
    test.Add2In1OutFunction("uint32",   "uint32",   "uint8");
    test.Add2In1OutFunction("int32",    "int32",    "uint8");
    test.Add2In1OutFunction("uint16",   "uint16",   "uint8");
    test.Add2In1OutFunction("int16",    "int16",    "uint8");
    test.Add2In1OutFunction("uint8",    "uint8",    "uint8");
    test.Add2In1OutFunction("int8",     "int8",     "uint8");

    test.AddInputCombinationFunctions("int8",   "int32",    "uint8");
    test.AddInputCombinationFunctions("int16",  "int32",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "int32",    "uint8");
    test.AddInputCombinationFunctions("uint16", "int32",    "uint8");
    test.AddInputCombinationFunctions("uint32", "int32",    "uint8");
    test.AddInputCombinationFunctions("int8",   "int64",    "uint8");
    test.AddInputCombinationFunctions("int16",  "int64",    "uint8");
    test.AddInputCombinationFunctions("int32",  "int64",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "int64",    "uint8");
    test.AddInputCombinationFunctions("uint16", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint32", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint64", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "uint32",   "uint8");
    test.AddInputCombinationFunctions("uint16", "uint32",   "uint8");
    test.AddInputCombinationFunctions("int8",   "uint64",   "uint8");
    test.AddInputCombinationFunctions("int16",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("int32",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint8",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint16", "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint32", "uint64",   "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("EQ", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestEqExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "EQ\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, Float64Bit, UnsignedInteger8Bit));

    float64 inputsEq[] = {15.3, 15.3};

    test.SetInputs(context, inputsEq);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));

    float64 inputsNEq[] = {15.4, 15.3};

    test.SetInputs(context, inputsNEq);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestEqDiffTypesExecution_Successful) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "EQ\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, SignedInteger32Bit, UnsignedInteger32Bit, UnsignedInteger8Bit));

    int64 inputsEq[] = {2147483647, 2147483647};

    test.SetInputs(context, inputsEq);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));

    int64 inputsNEq[] = {-2147483648, 2147483647};

    test.SetInputs(context, inputsNEq);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestEqDiffTypesExecution_FailedOutOfRange) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "EQ\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, SignedInteger32Bit, UnsignedInteger32Bit, UnsignedInteger8Bit));

    int64 inputs[] = {2147483647, 2147483650};

    test.SetInputs(context, inputs);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0, ErrorManagement::OutOfRange));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestNeqFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add2In1OutFunction("float64",  "float64",  "uint8");
    test.Add2In1OutFunction("float32",  "float32",  "uint8");
    test.Add2In1OutFunction("uint64",   "uint64",   "uint8");
    test.Add2In1OutFunction("int64",    "int64",    "uint8");
    test.Add2In1OutFunction("uint32",   "uint32",   "uint8");
    test.Add2In1OutFunction("int32",    "int32",    "uint8");
    test.Add2In1OutFunction("uint16",   "uint16",   "uint8");
    test.Add2In1OutFunction("int16",    "int16",    "uint8");
    test.Add2In1OutFunction("uint8",    "uint8",    "uint8");
    test.Add2In1OutFunction("int8",     "int8",     "uint8");

    test.AddInputCombinationFunctions("int8",   "int32",    "uint8");
    test.AddInputCombinationFunctions("int16",  "int32",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "int32",    "uint8");
    test.AddInputCombinationFunctions("uint16", "int32",    "uint8");
    test.AddInputCombinationFunctions("uint32", "int32",    "uint8");
    test.AddInputCombinationFunctions("int8",   "int64",    "uint8");
    test.AddInputCombinationFunctions("int16",  "int64",    "uint8");
    test.AddInputCombinationFunctions("int32",  "int64",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "int64",    "uint8");
    test.AddInputCombinationFunctions("uint16", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint32", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint64", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "uint32",   "uint8");
    test.AddInputCombinationFunctions("uint16", "uint32",   "uint8");
    test.AddInputCombinationFunctions("int8",   "uint64",   "uint8");
    test.AddInputCombinationFunctions("int16",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("int32",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint8",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint16", "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint32", "uint64",   "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("NEQ", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestNeqExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "NEQ\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, Float64Bit, UnsignedInteger8Bit));

    float64 inputsEq[] = {15.3, 15.3};

    test.SetInputs(context, inputsEq);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));

    float64 inputsNEq[] = {15.299999, 15.3};

    test.SetInputs(context, inputsNEq);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestNeqDiffTypesExecution_Successful) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "NEQ\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, SignedInteger32Bit, UnsignedInteger32Bit, UnsignedInteger8Bit));

    int64 inputsEq[] = {2147483647, 2147483647};

    test.SetInputs(context, inputsEq);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));

    int64 inputsNEq[] = {-2147483648, 2147483647};

    test.SetInputs(context, inputsNEq);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestNeqDiffTypesExecution_FailedOutOfRange) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "NEQ\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, SignedInteger32Bit, UnsignedInteger32Bit, UnsignedInteger8Bit));

    int64 inputs[] = {2147483647, 2147483650};

    test.SetInputs(context, inputs);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0, ErrorManagement::OutOfRange));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestGtFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add2In1OutFunction("float64",  "float64",  "uint8");
    test.Add2In1OutFunction("float32",  "float32",  "uint8");
    test.Add2In1OutFunction("uint64",   "uint64",   "uint8");
    test.Add2In1OutFunction("int64",    "int64",    "uint8");
    test.Add2In1OutFunction("uint32",   "uint32",   "uint8");
    test.Add2In1OutFunction("int32",    "int32",    "uint8");
    test.Add2In1OutFunction("uint16",   "uint16",   "uint8");
    test.Add2In1OutFunction("int16",    "int16",    "uint8");
    test.Add2In1OutFunction("uint8",    "uint8",    "uint8");
    test.Add2In1OutFunction("int8",     "int8",     "uint8");

    test.AddInputCombinationFunctions("int8",   "int32",    "uint8");
    test.AddInputCombinationFunctions("int16",  "int32",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "int32",    "uint8");
    test.AddInputCombinationFunctions("uint16", "int32",    "uint8");
    test.AddInputCombinationFunctions("uint32", "int32",    "uint8");
    test.AddInputCombinationFunctions("int8",   "int64",    "uint8");
    test.AddInputCombinationFunctions("int16",  "int64",    "uint8");
    test.AddInputCombinationFunctions("int32",  "int64",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "int64",    "uint8");
    test.AddInputCombinationFunctions("uint16", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint32", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint64", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "uint32",   "uint8");
    test.AddInputCombinationFunctions("uint16", "uint32",   "uint8");
    test.AddInputCombinationFunctions("int8",   "uint64",   "uint8");
    test.AddInputCombinationFunctions("int16",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("int32",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint8",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint16", "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint32", "uint64",   "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("GT", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestGtExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "GT\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, Float64Bit, UnsignedInteger8Bit));

    float64 inputsGreater[] = {27.8, 15.3};

    test.SetInputs(context, inputsGreater);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));

    float64 inputsEq[] = {27.8, 27.8};

    test.SetInputs(context, inputsEq);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));

    float64 inputsLower[] = {27.8, 30.3};

    test.SetInputs(context, inputsLower);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestGtDiffTypesExecution_Successful) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "GT\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, SignedInteger32Bit, UnsignedInteger32Bit, UnsignedInteger8Bit));

    int64 inputsGreater[] = {2147483647, 2147483646};

    test.SetInputs(context, inputsGreater);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));

    int64 inputsEq[] = {2147483647, 2147483647};

    test.SetInputs(context, inputsEq);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));

    int64 inputsLower[] = {2147483646, 2147483647};

    test.SetInputs(context, inputsLower);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestGtDiffTypesExecution_FailedOutOfRange) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "GT\n"
            "WRITE RES1\n";

    RuntimeEvaluator context1(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context1, SignedInteger32Bit, UnsignedInteger32Bit, UnsignedInteger8Bit));

    int64 inputsLower[] = {2147483647, 2147483648};

    test.SetInputs(context1, inputsLower);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context1, 0, ErrorManagement::OutOfRange));



    RuntimeEvaluator context2(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context2, UnsignedInteger32Bit, SignedInteger32Bit, UnsignedInteger8Bit));

    int64 inputsGreater[] = {2147483648, 2147483647};

    test.SetInputs(context2, inputsGreater);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context2, 0, ErrorManagement::OutOfRange));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestLtFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add2In1OutFunction("float64",  "float64",  "uint8");
    test.Add2In1OutFunction("float32",  "float32",  "uint8");
    test.Add2In1OutFunction("uint64",   "uint64",   "uint8");
    test.Add2In1OutFunction("int64",    "int64",    "uint8");
    test.Add2In1OutFunction("uint32",   "uint32",   "uint8");
    test.Add2In1OutFunction("int32",    "int32",    "uint8");
    test.Add2In1OutFunction("uint16",   "uint16",   "uint8");
    test.Add2In1OutFunction("int16",    "int16",    "uint8");
    test.Add2In1OutFunction("uint8",    "uint8",    "uint8");
    test.Add2In1OutFunction("int8",     "int8",     "uint8");

    test.AddInputCombinationFunctions("int8",   "int32",    "uint8");
    test.AddInputCombinationFunctions("int16",  "int32",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "int32",    "uint8");
    test.AddInputCombinationFunctions("uint16", "int32",    "uint8");
    test.AddInputCombinationFunctions("uint32", "int32",    "uint8");
    test.AddInputCombinationFunctions("int8",   "int64",    "uint8");
    test.AddInputCombinationFunctions("int16",  "int64",    "uint8");
    test.AddInputCombinationFunctions("int32",  "int64",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "int64",    "uint8");
    test.AddInputCombinationFunctions("uint16", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint32", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint64", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "uint32",   "uint8");
    test.AddInputCombinationFunctions("uint16", "uint32",   "uint8");
    test.AddInputCombinationFunctions("int8",   "uint64",   "uint8");
    test.AddInputCombinationFunctions("int16",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("int32",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint8",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint16", "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint32", "uint64",   "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("LT", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestLtExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "LT\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, Float64Bit, UnsignedInteger8Bit));

    float64 inputsGreater[] = {-10.4, -15.3};

    test.SetInputs(context, inputsGreater);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));

    float64 inputsEq[] = {-27.8, -27.8};

    test.SetInputs(context, inputsEq);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));

    float64 inputsLower[] = {-27.8, 15.3};

    test.SetInputs(context, inputsLower);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestGteFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add2In1OutFunction("float64",  "float64",  "uint8");
    test.Add2In1OutFunction("float32",  "float32",  "uint8");
    test.Add2In1OutFunction("uint64",   "uint64",   "uint8");
    test.Add2In1OutFunction("int64",    "int64",    "uint8");
    test.Add2In1OutFunction("uint32",   "uint32",   "uint8");
    test.Add2In1OutFunction("int32",    "int32",    "uint8");
    test.Add2In1OutFunction("uint16",   "uint16",   "uint8");
    test.Add2In1OutFunction("int16",    "int16",    "uint8");
    test.Add2In1OutFunction("uint8",    "uint8",    "uint8");
    test.Add2In1OutFunction("int8",     "int8",     "uint8");

    test.AddInputCombinationFunctions("int8",   "int32",    "uint8");
    test.AddInputCombinationFunctions("int16",  "int32",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "int32",    "uint8");
    test.AddInputCombinationFunctions("uint16", "int32",    "uint8");
    test.AddInputCombinationFunctions("uint32", "int32",    "uint8");
    test.AddInputCombinationFunctions("int8",   "int64",    "uint8");
    test.AddInputCombinationFunctions("int16",  "int64",    "uint8");
    test.AddInputCombinationFunctions("int32",  "int64",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "int64",    "uint8");
    test.AddInputCombinationFunctions("uint16", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint32", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint64", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "uint32",   "uint8");
    test.AddInputCombinationFunctions("uint16", "uint32",   "uint8");
    test.AddInputCombinationFunctions("int8",   "uint64",   "uint8");
    test.AddInputCombinationFunctions("int16",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("int32",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint8",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint16", "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint32", "uint64",   "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("GTE", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestGteExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "GTE\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, Float64Bit, UnsignedInteger8Bit));

    float64 inputsGreater[] = {15.10, -4.67};

    test.SetInputs(context, inputsGreater);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));

    float64 inputsEq[] = {-4.67, -4.67};

    test.SetInputs(context, inputsEq);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));

    float64 inputsLower[] = {-14.9, -4.67};

    test.SetInputs(context, inputsLower);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestLteFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add2In1OutFunction("float64",  "float64",  "uint8");
    test.Add2In1OutFunction("float32",  "float32",  "uint8");
    test.Add2In1OutFunction("uint64",   "uint64",   "uint8");
    test.Add2In1OutFunction("int64",    "int64",    "uint8");
    test.Add2In1OutFunction("uint32",   "uint32",   "uint8");
    test.Add2In1OutFunction("int32",    "int32",    "uint8");
    test.Add2In1OutFunction("uint16",   "uint16",   "uint8");
    test.Add2In1OutFunction("int16",    "int16",    "uint8");
    test.Add2In1OutFunction("uint8",    "uint8",    "uint8");
    test.Add2In1OutFunction("int8",     "int8",     "uint8");

    test.AddInputCombinationFunctions("int8",   "int32",    "uint8");
    test.AddInputCombinationFunctions("int16",  "int32",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "int32",    "uint8");
    test.AddInputCombinationFunctions("uint16", "int32",    "uint8");
    test.AddInputCombinationFunctions("uint32", "int32",    "uint8");
    test.AddInputCombinationFunctions("int8",   "int64",    "uint8");
    test.AddInputCombinationFunctions("int16",  "int64",    "uint8");
    test.AddInputCombinationFunctions("int32",  "int64",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "int64",    "uint8");
    test.AddInputCombinationFunctions("uint16", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint32", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint64", "int64",    "uint8");
    test.AddInputCombinationFunctions("uint8",  "uint32",   "uint8");
    test.AddInputCombinationFunctions("uint16", "uint32",   "uint8");
    test.AddInputCombinationFunctions("int8",   "uint64",   "uint8");
    test.AddInputCombinationFunctions("int16",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("int32",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint8",  "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint16", "uint64",   "uint8");
    test.AddInputCombinationFunctions("uint32", "uint64",   "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("LTE", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestLteExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "LTE\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, Float64Bit, UnsignedInteger8Bit));

    float64 inputsGreater[] = {-151.9, -457.3};

    test.SetInputs(context, inputsGreater);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));

    float64 inputsEq[] = {-151.9, -151.9};

    test.SetInputs(context, inputsEq);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));

    float64 inputsLower[] = {-745.2, -157.9};

    test.SetInputs(context, inputsLower);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestAndFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add2In1OutFunction("uint8", "uint8", "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("AND", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestAndExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "AND\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, UnsignedInteger8Bit, UnsignedInteger8Bit));

    uint8 inputs00[] = {0, 0};

    test.SetInputs(context, inputs00);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));

    uint8 inputs10[] = {1, 0};

    test.SetInputs(context, inputs10);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));

    uint8 inputs11[] = {1, 1};

    test.SetInputs(context, inputs11);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestOrFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add2In1OutFunction("uint8", "uint8", "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("OR", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestOrExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "OR\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, UnsignedInteger8Bit, UnsignedInteger8Bit));

    uint8 inputs00[] = {0, 0};

    test.SetInputs(context, inputs00);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));

    uint8 inputs10[] = {1, 0};

    test.SetInputs(context, inputs10);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));

    uint8 inputs11[] = {1, 1};

    test.SetInputs(context, inputs11);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestXorFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add2In1OutFunction("uint8", "uint8", "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("XOR", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestXorExecution) {
    RuntimeEvaluatorFunctionsTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "XOR\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, UnsignedInteger8Bit, UnsignedInteger8Bit));

    uint8 inputs00[] = {0, 0};

    test.SetInputs(context, inputs00);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));

    uint8 inputs10[] = {1, 0};

    test.SetInputs(context, inputs10);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));

    uint8 inputs11[] = {1, 1};

    test.SetInputs(context, inputs11);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestAddFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add2In1OutFunction("float64", "float64", "float64");
    test.Add2In1OutFunction("float32", "float32", "float32");

    test.Add2In1OutFunction("int8",   "int8",     "int32");
    test.Add2In1OutFunction("int16",  "int16",    "int32");

    test.Add2In1OutFunction("int32",  "int32",    "int64");
    test.Add2In1OutFunction("int64",  "int64",    "int64");

    test.Add2In1OutFunction("uint8",  "uint8",    "uint32");
    test.Add2In1OutFunction("uint16", "uint16",   "uint32");
    test.Add2In1OutFunction("uint32", "uint32",   "uint32");

    test.Add2In1OutFunction("uint32", "uint32",   "uint64");
    test.Add2In1OutFunction("uint64", "uint64",   "uint64");

    test.AddInputCombinationFunctions("int8",   "int32",    "int32");
    test.AddInputCombinationFunctions("int16",  "int32",    "int32");
    test.AddInputCombinationFunctions("uint8",  "int32",    "int32");
    test.AddInputCombinationFunctions("uint16", "int32",    "int32");
    test.AddInputCombinationFunctions("uint32", "int32",    "int32");

    test.AddInputCombinationFunctions("int8",   "int64",    "int64");
    test.AddInputCombinationFunctions("int16",  "int64",    "int64");
    test.AddInputCombinationFunctions("int32",  "int64",    "int64");
    test.AddInputCombinationFunctions("uint8",  "int64",    "int64");
    test.AddInputCombinationFunctions("uint16", "int64",    "int64");
    test.AddInputCombinationFunctions("uint32", "int64",    "int64");
    test.AddInputCombinationFunctions("uint64", "int64",    "int64");

    test.AddInputCombinationFunctions("uint8",  "uint32",   "uint32");
    test.AddInputCombinationFunctions("uint16", "uint32",   "uint32");

    test.AddInputCombinationFunctions("uint8",  "uint64",   "uint64");
    test.AddInputCombinationFunctions("uint16", "uint64",   "uint64");
    test.AddInputCombinationFunctions("uint32", "uint64",   "uint64");

    ASSERT_TRUE(test.TestFunctionTypes("ADD", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestSubFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add2In1OutFunction("float64",  "float64",  "float64");
    test.Add2In1OutFunction("float32",  "float32",  "float32");

    test.Add2In1OutFunction("int8",     "int8",     "int32");
    test.Add2In1OutFunction("int16",    "int16",    "int32");
    test.Add2In1OutFunction("int32",    "int32",    "int32");
    test.Add2In1OutFunction("uint8",    "uint8",    "int32");
    test.Add2In1OutFunction("uint16",   "uint16",   "int32");
    test.Add2In1OutFunction("uint32",   "uint32",   "int32");

    test.Add2In1OutFunction("uint64",   "uint64",   "int64");
    test.Add2In1OutFunction("int64",    "int64",    "int64");

    test.AddInputCombinationFunctions("int8",   "int32",    "int32");
    test.AddInputCombinationFunctions("int16",  "int32",    "int32");
    test.AddInputCombinationFunctions("uint8",  "int32",    "int32");
    test.AddInputCombinationFunctions("uint16", "int32",    "int32");
    test.AddInputCombinationFunctions("uint32", "int32",    "int32");
    test.AddInputCombinationFunctions("uint8",  "uint32",   "int32");
    test.AddInputCombinationFunctions("uint16", "uint32",   "int32");

    test.AddInputCombinationFunctions("int8",   "int64",    "int64");
    test.AddInputCombinationFunctions("int16",  "int64",    "int64");
    test.AddInputCombinationFunctions("int32",  "int64",    "int64");
    test.AddInputCombinationFunctions("uint8",  "int64",    "int64");
    test.AddInputCombinationFunctions("uint16", "int64",    "int64");
    test.AddInputCombinationFunctions("uint32", "int64",    "int64");
    test.AddInputCombinationFunctions("uint64", "int64",    "int64");
    test.AddInputCombinationFunctions("uint8",  "uint64",   "int64");
    test.AddInputCombinationFunctions("uint16", "uint64",   "int64");
    test.AddInputCombinationFunctions("uint32", "uint64",   "int64");

    ASSERT_TRUE(test.TestFunctionTypes("SUB", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestMulFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add2In1OutFunction("float64", "float64", "float64");
    test.Add2In1OutFunction("float32", "float32", "float32");

    test.Add2In1OutFunction("int8",   "int8",     "int32");
    test.Add2In1OutFunction("int16",  "int16",    "int32");

    test.Add2In1OutFunction("int32",  "int32",    "int64");
    test.Add2In1OutFunction("int64",  "int64",    "int64");

    test.Add2In1OutFunction("uint8",  "uint8",    "uint32");
    test.Add2In1OutFunction("uint16", "uint16",   "uint32");
    test.Add2In1OutFunction("uint32", "uint32",   "uint32");

    test.Add2In1OutFunction("uint32", "uint32",   "uint64");
    test.Add2In1OutFunction("uint64", "uint64",   "uint64");

    test.AddInputCombinationFunctions("int8",   "int32",    "int32");
    test.AddInputCombinationFunctions("int16",  "int32",    "int32");
    test.AddInputCombinationFunctions("uint8",  "int32",    "int32");
    test.AddInputCombinationFunctions("uint16", "int32",    "int32");
    test.AddInputCombinationFunctions("uint32", "int32",    "int32");

    test.AddInputCombinationFunctions("int8",   "int64",    "int64");
    test.AddInputCombinationFunctions("int16",  "int64",    "int64");
    test.AddInputCombinationFunctions("int32",  "int64",    "int64");
    test.AddInputCombinationFunctions("uint8",  "int64",    "int64");
    test.AddInputCombinationFunctions("uint16", "int64",    "int64");
    test.AddInputCombinationFunctions("uint32", "int64",    "int64");
    test.AddInputCombinationFunctions("uint64", "int64",    "int64");

    test.AddInputCombinationFunctions("uint8",  "uint32",   "uint32");
    test.AddInputCombinationFunctions("uint16", "uint32",   "uint32");

    test.AddInputCombinationFunctions("uint8",  "uint64",   "uint64");
    test.AddInputCombinationFunctions("uint16", "uint64",   "uint64");
    test.AddInputCombinationFunctions("uint32", "uint64",   "uint64");

    ASSERT_TRUE(test.TestFunctionTypes("MUL", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestDivFunctionTypes) {
    RuntimeEvaluatorFunctionsTest test;

    test.Add2In1OutFunction("float64", "float64", "float64");
    test.Add2In1OutFunction("float32", "float32", "float32");

    test.Add2In1OutFunction("int8",   "int8",     "int8");
    test.Add2In1OutFunction("int16",  "int16",    "int16");
    test.Add2In1OutFunction("int32",  "int32",    "int32");
    test.Add2In1OutFunction("int64",  "int64",    "int64");

    test.Add2In1OutFunction("uint8",  "uint8",    "uint8");
    test.Add2In1OutFunction("uint16", "uint16",   "uint16");
    test.Add2In1OutFunction("uint32", "uint32",   "uint32");
    test.Add2In1OutFunction("uint64", "uint64",   "uint64");

    test.AddInputCombinationFunctions("int8",   "int32",    "int32");
    test.AddInputCombinationFunctions("int16",  "int32",    "int32");
    test.AddInputCombinationFunctions("uint8",  "int32",    "int32");
    test.AddInputCombinationFunctions("uint16", "int32",    "int32");
    test.AddInputCombinationFunctions("uint32", "int32",    "int32");

    test.AddInputCombinationFunctions("int8",   "int64",    "int64");
    test.AddInputCombinationFunctions("int16",  "int64",    "int64");
    test.AddInputCombinationFunctions("int32",  "int64",    "int64");
    test.AddInputCombinationFunctions("uint8",  "int64",    "int64");
    test.AddInputCombinationFunctions("uint16", "int64",    "int64");
    test.AddInputCombinationFunctions("uint32", "int64",    "int64");
    test.AddInputCombinationFunctions("uint64", "int64",    "int64");

    test.AddInputCombinationFunctions("uint8",  "uint32",   "uint32");
    test.AddInputCombinationFunctions("uint16", "uint32",   "uint32");

    test.AddInputCombinationFunctions("uint8",  "uint64",   "uint64");
    test.AddInputCombinationFunctions("uint16", "uint64",   "uint64");
    test.AddInputCombinationFunctions("uint32", "uint64",   "uint64");

    ASSERT_TRUE(test.TestFunctionTypes("DIV", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeSuccessful) {
    RuntimeEvaluatorFunctionsTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, false, true, 1, 2));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeSuccessful_NoInputs) {
    RuntimeEvaluatorFunctionsTest test;

    TypeDescriptor functionTypes[] = {Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 0, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, false, true, 0, 2));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeSuccessful_NoOutputs) {
    RuntimeEvaluatorFunctionsTest test;

    TypeDescriptor functionTypes[] = {Float32Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 0, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, false, true, 1, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeSuccessful_MultipleInputs) {
    RuntimeEvaluatorFunctionsTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, UnsignedInteger64Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 2, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(UnsignedInteger64Bit);
    typeStack.Push(Float32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, false, true, 3, 2));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeSuccessful_MultipleOutputs) {
    RuntimeEvaluatorFunctionsTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit, UnsignedInteger32Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 2, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, false, true, 1, 3));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeSuccessful_StackLeftover) {
    RuntimeEvaluatorFunctionsTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(UnsignedInteger8Bit);
    typeStack.Push(Float32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, false, true, 2, 3));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeSuccessful_MatchOutput) {
    RuntimeEvaluatorFunctionsTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);
    typeStack.Push(Float64Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, true, true, 1, 2));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeFailed_WrongName) {
    RuntimeEvaluatorFunctionsTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "WrongName", typeStack, false, false, 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeFailed_MissingInput) {
    RuntimeEvaluatorFunctionsTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, false, false, 0, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeFailed_WrongInputType) {
    RuntimeEvaluatorFunctionsTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(SignedInteger32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, false, false, 1, 1));
}


TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeFailed_MatchOutputMissingOutput) {
    RuntimeEvaluatorFunctionsTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, true, false, 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionsGTest,TestTryConsumeFailed_MatchOutputWrongOutput) {
    RuntimeEvaluatorFunctionsTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunctions functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);
    typeStack.Push(Float32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, true, false, 2, 2));
}
