/**
 * @file RuntimeEvaluatorFunctionGTest.cpp
 * @brief Source file for class RuntimeEvaluatorFunctionGTest
 * @date 06/05/2020
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
 * the class RuntimeEvaluatorFunctionGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RuntimeEvaluatorFunctionTest.h"
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Non-class functions                             */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestRegisterFunction_Successful) {
    RuntimeEvaluatorFunctionTest test;
    ASSERT_TRUE(test.TestRegisterFunction());
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestRegisterFunction_MaxFunctions) {
    RuntimeEvaluatorFunctionTest test;
    ASSERT_TRUE(test.TestRegisterFunctionMaxFunctions());
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestFindPCodeAndUpdateTypeStack_Found) {
    RuntimeEvaluatorFunctionTest test;
    RuntimeEvaluatorFunction functionUT("Test", 0, 0, NULL_PTR(TypeDescriptor*), MockFunction);

    uint32 initialAvailableFunctions = availableFunctions;
    RegisterFunction(functionUT);

    ASSERT_TRUE(test.TestFindPCodeAndUpdateTypeStack("Test", true, availableFunctions - 1));

    //Prevent that functions registered in this test affect following tests
    availableFunctions = initialAvailableFunctions;
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestFindPCodeAndUpdateTypeStack_NotFound) {
    RuntimeEvaluatorFunctionTest test;
    ASSERT_TRUE(test.TestFindPCodeAndUpdateTypeStack("Test", false, 0));
}

/*---------------------------------------------------------------------------*/
/*                          Constructors/Getters                             */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestDefaultConstructor) {
    RuntimeEvaluatorFunctionTest test;
    ASSERT_TRUE(test.TestDefaultConstructor());
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestFullConstructor) {
    RuntimeEvaluatorFunctionTest test;
    ASSERT_TRUE(test.TestFullConstructor());
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestGetName) {
    RuntimeEvaluatorFunctionTest test;
    ASSERT_TRUE(test.TestGetName());
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestGetTypes_InputsAndOutputs) {
    TypeDescriptor types[] = {UnsignedInteger8Bit, Float64Bit, SignedInteger32Bit, CharString, PointerType};
    RuntimeEvaluatorFunctionTest test;
    ASSERT_TRUE(test.TestGetTypes(2, 3, types));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestGetTypes_NoInputs) {
    TypeDescriptor types[] = {VoidType, ConstCharString, Character8Bit, Float32Bit, UnsignedInteger16Bit};
    RuntimeEvaluatorFunctionTest test;
    ASSERT_TRUE(test.TestGetTypes(0, 5, types));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestGetTypes_NoOutputs) {
    TypeDescriptor types[] = {SignedInteger64Bit, SignedInteger64Bit, UnsignedInteger64Bit, UnsignedInteger8Bit};
    RuntimeEvaluatorFunctionTest test;
    ASSERT_TRUE(test.TestGetTypes(4, 0, types));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestGetTypes_None) {
    RuntimeEvaluatorFunctionTest test;
    ASSERT_TRUE(test.TestGetTypes(0, 0, NULL_PTR(TypeDescriptor*)));
}

/*---------------------------------------------------------------------------*/
/*                              READ/RREAD                                   */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestReadFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction1Out("float64");
    test.AddExpectedFunction1Out("float32");
    test.AddExpectedFunction1Out("uint64");
    test.AddExpectedFunction1Out("int64");
    test.AddExpectedFunction1Out("uint32");
    test.AddExpectedFunction1Out("int32");
    test.AddExpectedFunction1Out("uint16");
    test.AddExpectedFunction1Out("int16");
    test.AddExpectedFunction1Out("uint8");
    test.AddExpectedFunction1Out("int8");

    ASSERT_TRUE(test.TestFunctionTypes("READ", 0, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestReadExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, Float64Bit, Float64Bit));

    float64 inputs[] = {-152.3};

    test.SetInputs(context, inputs);
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, -152.3));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestRReadFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction1Out("float64");
    test.AddExpectedFunction1Out("float32");
    test.AddExpectedFunction1Out("uint64");
    test.AddExpectedFunction1Out("int64");
    test.AddExpectedFunction1Out("uint32");
    test.AddExpectedFunction1Out("int32");
    test.AddExpectedFunction1Out("uint16");
    test.AddExpectedFunction1Out("int16");
    test.AddExpectedFunction1Out("uint8");
    test.AddExpectedFunction1Out("int8");

    ASSERT_TRUE(test.TestFunctionTypes("RREAD", 0, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestRReadExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);
    float64 input = -152.3;

    ASSERT_TRUE(test.PrepareContext(context, Float64Bit, Float64Bit, static_cast<void *>(&input)));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, -152.3));
}

/*---------------------------------------------------------------------------*/
/*                             WRITE/RWRITE                                  */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestWriteFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction1In("float64");
    test.AddExpectedFunction1In("float32");
    test.AddExpectedFunction1In("uint16");
    test.AddExpectedFunction1In("int16");
    test.AddExpectedFunction1In("uint8");
    test.AddExpectedFunction1In("int8");
    for (uint8 i = 0; i < 4; ++i) {
        test.AddExpectedFunction1In("uint64");
    }
    for (uint8 i = 0; i < 8; ++i) {
        test.AddExpectedFunction1In("int64");
    }
    for (uint8 i = 0; i < 3; ++i) {
        test.AddExpectedFunction1In("uint32");
    }
    for (uint8 i = 0; i < 6; ++i) {
        test.AddExpectedFunction1In("int32");
    }

    ASSERT_TRUE(test.TestFunctionTypes("WRITE", 1, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestWriteExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST float64 -152.3\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, -152.3));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestWriteConvExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int64 -115\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger8Bit));

    ASSERT_TRUE(test.TestIntFunctionExecution<int8>(context, -115));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestWriteConvExecution_FailedOutOfRange) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int64 -152\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, UnsignedInteger8Bit));

    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0, ErrorManagement::OutOfRange));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestRWriteFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction1In("float64");
    test.AddExpectedFunction1In("float32");
    test.AddExpectedFunction1In("uint16");
    test.AddExpectedFunction1In("int16");
    test.AddExpectedFunction1In("uint8");
    test.AddExpectedFunction1In("int8");
    for (uint8 i = 0; i < 4; ++i) {
        test.AddExpectedFunction1In("uint64");
    }
    for (uint8 i = 0; i < 8; ++i) {
        test.AddExpectedFunction1In("int64");
    }
    for (uint8 i = 0; i < 3; ++i) {
        test.AddExpectedFunction1In("uint32");
    }
    for (uint8 i = 0; i < 6; ++i) {
        test.AddExpectedFunction1In("int32");
    }

    ASSERT_TRUE(test.TestFunctionTypes("RWRITE", 1, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestRWriteExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST float64 -152.3\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);
    float64 output;

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit, NULL, static_cast<void *>(&output)));
    ASSERT_TRUE(test.TestFunctionExecution(context));
    ASSERT_TRUE(output == -152.3);
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestRWriteConvExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int64 -112\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);
    int8 output;

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger8Bit, NULL, static_cast<void *>(&output)));
    ASSERT_TRUE(test.TestFunctionExecution(context));
    ASSERT_TRUE(output == -112);
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestRWriteConvExecution_FailedOutOfRange) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int64 -112\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);
    uint8 output;

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, UnsignedInteger8Bit, NULL, static_cast<void *>(&output)));
    ASSERT_TRUE(test.TestFunctionExecution(context, ErrorManagement::OutOfRange));
}

/*---------------------------------------------------------------------------*/
/*                                 CAST                                      */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestCastFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;
    CCString typeNames[] = {"float64", "float32", "uint64", "int64", "uint32", "int32", "uint16", "int16", "uint8", "int8"};

    for (uint8 i = 0; i < 10; ++i) {
        for (uint8 j = 0; j < 10; ++j) {
            test.AddExpectedFunction1In1Out(typeNames[i], typeNames[j]);
        }
    }

    ASSERT_TRUE(test.TestFunctionTypes("CAST", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestCastExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST float64 1.57079\n"
            "CAST uint8\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, UnsignedInteger8Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestCastExecution_FailedOutOfRange) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint16 256\n"
            "CAST uint8\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, UnsignedInteger8Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int8>(context, 255, ErrorManagement::OutOfRange));
}

/*---------------------------------------------------------------------------*/
/*                                 MATH                                      */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSinFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction1In1Out("float32", "float32");
    test.AddExpectedFunction1In1Out("float64", "float64");

    ASSERT_TRUE(test.TestFunctionTypes("SIN", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSinExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST float64 1.57079\n"
            "SIN\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestCosFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction1In1Out("float32", "float32");
    test.AddExpectedFunction1In1Out("float64", "float64");

    ASSERT_TRUE(test.TestFunctionTypes("COS", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestCosExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST float64 0\n"
            "COS\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestTanFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction1In1Out("float32", "float32");
    test.AddExpectedFunction1In1Out("float64", "float64");

    ASSERT_TRUE(test.TestFunctionTypes("TAN", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestTanExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST float64 0.78539816339\n"
            "TAN\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestExpFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction1In1Out("float32", "float32");
    test.AddExpectedFunction1In1Out("float64", "float64");

    ASSERT_TRUE(test.TestFunctionTypes("EXP", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestExpExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST float64 0\n"
            "EXP\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestLogFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction1In1Out("float32", "float32");
    test.AddExpectedFunction1In1Out("float64", "float64");

    ASSERT_TRUE(test.TestFunctionTypes("LOG", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestLogExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST float64 1\n"
            "LOG\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestLog10FunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction1In1Out("float32", "float32");
    test.AddExpectedFunction1In1Out("float64", "float64");

    ASSERT_TRUE(test.TestFunctionTypes("LOG10", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestLog10Execution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST float64 1000\n"
            "LOG10\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, 3));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestPowFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction2In1Out("float32",  "float32",  "float32");
    test.AddExpectedFunction2In1Out("float64",  "float64",  "float64");

    ASSERT_TRUE(test.TestFunctionTypes("POW", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestPowExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST float64 10.2\n"
            "CONST float64 3\n"
            "POW\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, 1061.208));
}

/*---------------------------------------------------------------------------*/
/*                              COMPARISON                                   */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestEqFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction2In1Out("float64",  "float64",  "uint8");
    test.AddExpectedFunction2In1Out("float32",  "float32",  "uint8");
    test.AddExpectedFunction2In1Out("uint64",   "uint64",   "uint8");
    test.AddExpectedFunction2In1Out("int64",    "int64",    "uint8");
    test.AddExpectedFunction2In1Out("uint32",   "uint32",   "uint8");
    test.AddExpectedFunction2In1Out("int32",    "int32",    "uint8");
    test.AddExpectedFunction2In1Out("uint16",   "uint16",   "uint8");
    test.AddExpectedFunction2In1Out("int16",    "int16",    "uint8");
    test.AddExpectedFunction2In1Out("uint8",    "uint8",    "uint8");
    test.AddExpectedFunction2In1Out("int8",     "int8",     "uint8");

    test.AddExpectedFunctionCombinedInputs("int8",   "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int8",   "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int32",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint64", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "uint32",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint32",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int8",   "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int32",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "uint64",   "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("EQ", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestEqExecution) {
    RuntimeEvaluatorFunctionTest test;

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestEqDiffTypesExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestEqDiffTypesExecution_FailedOutOfRange) {
    RuntimeEvaluatorFunctionTest test;

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestNeqFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction2In1Out("float64",  "float64",  "uint8");
    test.AddExpectedFunction2In1Out("float32",  "float32",  "uint8");
    test.AddExpectedFunction2In1Out("uint64",   "uint64",   "uint8");
    test.AddExpectedFunction2In1Out("int64",    "int64",    "uint8");
    test.AddExpectedFunction2In1Out("uint32",   "uint32",   "uint8");
    test.AddExpectedFunction2In1Out("int32",    "int32",    "uint8");
    test.AddExpectedFunction2In1Out("uint16",   "uint16",   "uint8");
    test.AddExpectedFunction2In1Out("int16",    "int16",    "uint8");
    test.AddExpectedFunction2In1Out("uint8",    "uint8",    "uint8");
    test.AddExpectedFunction2In1Out("int8",     "int8",     "uint8");

    test.AddExpectedFunctionCombinedInputs("int8",   "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int8",   "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int32",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint64", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "uint32",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint32",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int8",   "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int32",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "uint64",   "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("NEQ", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestNeqExecution) {
    RuntimeEvaluatorFunctionTest test;

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestNeqDiffTypesExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestNeqDiffTypesExecution_FailedOutOfRange) {
    RuntimeEvaluatorFunctionTest test;

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestGtFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction2In1Out("float64",  "float64",  "uint8");
    test.AddExpectedFunction2In1Out("float32",  "float32",  "uint8");
    test.AddExpectedFunction2In1Out("uint64",   "uint64",   "uint8");
    test.AddExpectedFunction2In1Out("int64",    "int64",    "uint8");
    test.AddExpectedFunction2In1Out("uint32",   "uint32",   "uint8");
    test.AddExpectedFunction2In1Out("int32",    "int32",    "uint8");
    test.AddExpectedFunction2In1Out("uint16",   "uint16",   "uint8");
    test.AddExpectedFunction2In1Out("int16",    "int16",    "uint8");
    test.AddExpectedFunction2In1Out("uint8",    "uint8",    "uint8");
    test.AddExpectedFunction2In1Out("int8",     "int8",     "uint8");

    test.AddExpectedFunctionCombinedInputs("int8",   "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int8",   "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int32",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint64", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "uint32",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint32",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int8",   "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int32",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "uint64",   "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("GT", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestGtExecution) {
    RuntimeEvaluatorFunctionTest test;

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestGtDiffTypesExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestGtDiffTypesExecution_FailedOutOfRange) {
    RuntimeEvaluatorFunctionTest test;

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestLtFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction2In1Out("float64",  "float64",  "uint8");
    test.AddExpectedFunction2In1Out("float32",  "float32",  "uint8");
    test.AddExpectedFunction2In1Out("uint64",   "uint64",   "uint8");
    test.AddExpectedFunction2In1Out("int64",    "int64",    "uint8");
    test.AddExpectedFunction2In1Out("uint32",   "uint32",   "uint8");
    test.AddExpectedFunction2In1Out("int32",    "int32",    "uint8");
    test.AddExpectedFunction2In1Out("uint16",   "uint16",   "uint8");
    test.AddExpectedFunction2In1Out("int16",    "int16",    "uint8");
    test.AddExpectedFunction2In1Out("uint8",    "uint8",    "uint8");
    test.AddExpectedFunction2In1Out("int8",     "int8",     "uint8");

    test.AddExpectedFunctionCombinedInputs("int8",   "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int8",   "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int32",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint64", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "uint32",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint32",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int8",   "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int32",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "uint64",   "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("LT", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestLtExecution) {
    RuntimeEvaluatorFunctionTest test;

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestLtDiffTypesExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "LT\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, SignedInteger32Bit, UnsignedInteger32Bit, UnsignedInteger8Bit));

    int64 inputsGreater[] = {2147483647, 2147483646};

    test.SetInputs(context, inputsGreater);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));

    int64 inputsEq[] = {2147483647, 2147483647};

    test.SetInputs(context, inputsEq);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));

    int64 inputsLower[] = {2147483646, 2147483647};

    test.SetInputs(context, inputsLower);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestLtDiffTypesExecution_FailedOutOfRange) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "LT\n"
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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestGteFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction2In1Out("float64",  "float64",  "uint8");
    test.AddExpectedFunction2In1Out("float32",  "float32",  "uint8");
    test.AddExpectedFunction2In1Out("uint64",   "uint64",   "uint8");
    test.AddExpectedFunction2In1Out("int64",    "int64",    "uint8");
    test.AddExpectedFunction2In1Out("uint32",   "uint32",   "uint8");
    test.AddExpectedFunction2In1Out("int32",    "int32",    "uint8");
    test.AddExpectedFunction2In1Out("uint16",   "uint16",   "uint8");
    test.AddExpectedFunction2In1Out("int16",    "int16",    "uint8");
    test.AddExpectedFunction2In1Out("uint8",    "uint8",    "uint8");
    test.AddExpectedFunction2In1Out("int8",     "int8",     "uint8");

    test.AddExpectedFunctionCombinedInputs("int8",   "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int8",   "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int32",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint64", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "uint32",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint32",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int8",   "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int32",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "uint64",   "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("GTE", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestGteExecution) {
    RuntimeEvaluatorFunctionTest test;

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestGteDiffTypesExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "GTE\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, SignedInteger32Bit, UnsignedInteger32Bit, UnsignedInteger8Bit));

    int64 inputsGreater[] = {2147483647, 2147483646};

    test.SetInputs(context, inputsGreater);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));

    int64 inputsEq[] = {2147483647, 2147483647};

    test.SetInputs(context, inputsEq);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));

    int64 inputsLower[] = {2147483646, 2147483647};

    test.SetInputs(context, inputsLower);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestGteDiffTypesExecution_FailedOutOfRange) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "GTE\n"
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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestLteFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction2In1Out("float64",  "float64",  "uint8");
    test.AddExpectedFunction2In1Out("float32",  "float32",  "uint8");
    test.AddExpectedFunction2In1Out("uint64",   "uint64",   "uint8");
    test.AddExpectedFunction2In1Out("int64",    "int64",    "uint8");
    test.AddExpectedFunction2In1Out("uint32",   "uint32",   "uint8");
    test.AddExpectedFunction2In1Out("int32",    "int32",    "uint8");
    test.AddExpectedFunction2In1Out("uint16",   "uint16",   "uint8");
    test.AddExpectedFunction2In1Out("int16",    "int16",    "uint8");
    test.AddExpectedFunction2In1Out("uint8",    "uint8",    "uint8");
    test.AddExpectedFunction2In1Out("int8",     "int8",     "uint8");

    test.AddExpectedFunctionCombinedInputs("int8",   "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "int32",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int8",   "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("int32",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint64", "int64",    "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "uint32",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint32",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int8",   "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int16",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("int32",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint8",  "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint64",   "uint8");
    test.AddExpectedFunctionCombinedInputs("uint32", "uint64",   "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("LTE", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestLteExecution) {
    RuntimeEvaluatorFunctionTest test;

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestLteDiffTypesExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "LTE\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, SignedInteger32Bit, UnsignedInteger32Bit, UnsignedInteger8Bit));

    int64 inputsGreater[] = {2147483647, 2147483646};

    test.SetInputs(context, inputsGreater);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));

    int64 inputsEq[] = {2147483647, 2147483647};

    test.SetInputs(context, inputsEq);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));

    int64 inputsLower[] = {2147483646, 2147483647};

    test.SetInputs(context, inputsLower);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestLteDiffTypesExecution_FailedOutOfRange) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "READ IN2\n"
            "LTE\n"
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

/*---------------------------------------------------------------------------*/
/*                                LOGICAL                                    */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestAndFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction2In1Out("uint8", "uint8", "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("AND", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestAndExecution) {
    RuntimeEvaluatorFunctionTest test;

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestOrFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction2In1Out("uint8", "uint8", "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("OR", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestOrExecution) {
    RuntimeEvaluatorFunctionTest test;

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestXorFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction2In1Out("uint8", "uint8", "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("XOR", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestXorExecution) {
    RuntimeEvaluatorFunctionTest test;

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

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestNotFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction1In1Out("uint8", "uint8");

    ASSERT_TRUE(test.TestFunctionTypes("NOT", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestNotExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "READ IN1\n"
            "NOT\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, UnsignedInteger8Bit, UnsignedInteger8Bit));

    uint8 inputFalse[] = {0};

    test.SetInputs(context, inputFalse);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 1));

    uint8 inputTrue[] = {1};

    test.SetInputs(context, inputTrue);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));

    uint8 inputGreaterThanOne[] = {2};

    test.SetInputs(context, inputGreaterThanOne);
    ASSERT_TRUE(test.TestIntFunctionExecution<uint8>(context, 0));
}

/*---------------------------------------------------------------------------*/
/*                                  ADD                                      */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestAddFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction2In1Out("float64", "float64", "float64");
    test.AddExpectedFunction2In1Out("float32", "float32", "float32");

    test.AddExpectedFunction2In1Out("int8",   "int8",     "int32");
    test.AddExpectedFunction2In1Out("int16",  "int16",    "int32");
    test.AddExpectedFunction2In1Out("int32",  "int32",    "int32");

    test.AddExpectedFunction2In1Out("int32",  "int32",    "int64");
    test.AddExpectedFunction2In1Out("int64",  "int64",    "int64");

    test.AddExpectedFunction2In1Out("uint8",  "uint8",    "uint32");
    test.AddExpectedFunction2In1Out("uint16", "uint16",   "uint32");
    test.AddExpectedFunction2In1Out("uint32", "uint32",   "uint32");

    test.AddExpectedFunction2In1Out("uint32", "uint32",   "uint64");
    test.AddExpectedFunction2In1Out("uint64", "uint64",   "uint64");

    test.AddExpectedFunctionCombinedInputs("int8",   "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("int16",  "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("uint16", "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("uint32", "int32",    "int32");

    test.AddExpectedFunctionCombinedInputs("int8",   "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("int16",  "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("int32",  "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint16", "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint32", "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint64", "int64",    "int64");

    test.AddExpectedFunctionCombinedInputs("uint8",  "uint32",   "uint32");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint32",   "uint32");

    test.AddExpectedFunctionCombinedInputs("uint8",  "uint64",   "uint64");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint64",   "uint64");
    test.AddExpectedFunctionCombinedInputs("uint32", "uint64",   "uint64");

    ASSERT_TRUE(test.TestFunctionTypes("ADD", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestAddFloatExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST float64 10.65\n"
            "CONST float64 -3.15\n"
            "ADD\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, 7.5));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestAddExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int8 -128\n"
            "CONST int8 -128\n"
            "ADD\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, -256));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSAddExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int8 -1\n"
            "CONST int32 -2147483647\n"
            "ADD\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, -2147483648));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSAddExecution_FailedUnderflow) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int8 -1\n"
            "CONST int32 -2147483648\n"
            "ADD\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, 2147483647, ErrorManagement::Underflow));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSAddExecution_FailedPreviousError) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int8 -1\n"
            "CONST int8 -1\n"
            "CONST int32 -2147483648\n"
            "ADD\n"
            "ADD\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, 2147483647, ErrorManagement::Underflow));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSSAddExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint32 1\n"
            "CONST int32 2147483646\n"
            "ADD\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, 2147483647));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSSAddExecution_FailedOverflow) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint32 1\n"
            "CONST int32 2147483647\n"
            "ADD\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, -2147483648, ErrorManagement::Overflow));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSSAddExecution_FailedPreviousError) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint32 1\n"
            "CONST uint32 1\n"
            "CONST int32 2147483647\n"
            "ADD\n"
            "ADD\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, -2147483648, ErrorManagement::Overflow));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSSAddExecution_FailedSaturated) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint32 2147483648\n"
            "CONST int32 -10\n"
            "ADD\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, 0, ErrorManagement::Overflow));
}

/*---------------------------------------------------------------------------*/
/*                                   SUB                                     */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSubFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction2In1Out("float64",  "float64",  "float64");
    test.AddExpectedFunction2In1Out("float32",  "float32",  "float32");

    test.AddExpectedFunction2In1Out("int8",     "int8",     "int32");
    test.AddExpectedFunction2In1Out("int16",    "int16",    "int32");
    test.AddExpectedFunction2In1Out("int32",    "int32",    "int32");
    test.AddExpectedFunction2In1Out("uint8",    "uint8",    "int32");
    test.AddExpectedFunction2In1Out("uint16",   "uint16",   "int32");
    test.AddExpectedFunction2In1Out("uint32",   "uint32",   "int32");

    test.AddExpectedFunction2In1Out("uint64",   "uint64",   "int64");
    test.AddExpectedFunction2In1Out("int64",    "int64",    "int64");

    test.AddExpectedFunctionCombinedInputs("int8",   "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("int16",  "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("uint16", "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("uint32", "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("uint8",  "uint32",   "int32");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint32",   "int32");

    test.AddExpectedFunctionCombinedInputs("int8",   "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("int16",  "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("int32",  "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint16", "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint32", "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint64", "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint8",  "uint64",   "int64");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint64",   "int64");
    test.AddExpectedFunctionCombinedInputs("uint32", "uint64",   "int64");

    ASSERT_TRUE(test.TestFunctionTypes("SUB", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSubFloatExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST float64 -47\n"
            "CONST float64 33.48\n"
            "SUB\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, -80.48));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSubExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int8 -128\n"
            "CONST int8 127\n"
            "SUB\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, -255));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSSubExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int8 -1\n"
            "CONST int32 2147483647\n"
            "SUB\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, -2147483648));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSSubExecution_FailedUnderflow) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int8 -2\n"
            "CONST int32 2147483647\n"
            "SUB\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, 2147483647, ErrorManagement::Underflow));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSSubExecution_FailedPreviousError) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int8 -10\n"
            "CONST int8 -2\n"
            "CONST int32 2147483647\n"
            "SUB\n"
            "SUB\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, 2147483647, ErrorManagement::Underflow));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSSSubExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint32 2147483646\n"
            "CONST int32 -1\n"
            "SUB\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, 2147483647));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSSSubExecution_FailedOverflow) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint32 2147483647\n"
            "CONST int32 -1\n"
            "SUB\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, -2147483648, ErrorManagement::Overflow));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSSSubExecution_FailedPreviousError) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint32 1\n"
            "CONST uint32 2147483647\n"
            "CONST int32 -1\n"
            "SUB\n"
            "SUB\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, -2147483648, ErrorManagement::Overflow));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSSSubExecution_FailedSaturated) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint32 2147483648\n"
            "CONST int32 -10\n"
            "SUB\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, 0, ErrorManagement::Overflow));
}

/*---------------------------------------------------------------------------*/
/*                                   MUL                                     */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestMulFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction2In1Out("float64", "float64", "float64");
    test.AddExpectedFunction2In1Out("float32", "float32", "float32");

    test.AddExpectedFunction2In1Out("int8",   "int8",     "int32");
    test.AddExpectedFunction2In1Out("int16",  "int16",    "int32");
    test.AddExpectedFunction2In1Out("int32",  "int32",    "int32");

    test.AddExpectedFunction2In1Out("int32",  "int32",    "int64");
    test.AddExpectedFunction2In1Out("int64",  "int64",    "int64");

    test.AddExpectedFunction2In1Out("uint8",  "uint8",    "uint32");
    test.AddExpectedFunction2In1Out("uint16", "uint16",   "uint32");
    test.AddExpectedFunction2In1Out("uint32", "uint32",   "uint32");

    test.AddExpectedFunction2In1Out("uint32", "uint32",   "uint64");
    test.AddExpectedFunction2In1Out("uint64", "uint64",   "uint64");

    test.AddExpectedFunctionCombinedInputs("int8",   "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("int16",  "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("uint16", "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("uint32", "int32",    "int32");

    test.AddExpectedFunctionCombinedInputs("int8",   "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("int16",  "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("int32",  "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint16", "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint32", "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint64", "int64",    "int64");

    test.AddExpectedFunctionCombinedInputs("uint8",  "uint32",   "uint32");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint32",   "uint32");

    test.AddExpectedFunctionCombinedInputs("uint8",  "uint64",   "uint64");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint64",   "uint64");
    test.AddExpectedFunctionCombinedInputs("uint32", "uint64",   "uint64");

    ASSERT_TRUE(test.TestFunctionTypes("MUL", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestMulFloatExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST float64 10.32\n"
            "CONST float64 -2.5\n"
            "MUL\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, -25.8));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestMulExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int8 2\n"
            "CONST int8 -128\n"
            "MUL\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, -256));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSMulExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int8 -2\n"
            "CONST int32 1073741824\n"
            "MUL\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, -2147483648));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSMulExecution_FailedUnderflow) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int8 -2\n"
            "CONST int32 1073741825\n"
            "MUL\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, 2147483646, ErrorManagement::Underflow));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSMulExecution_FailedPreviousError) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int8 -1\n"
            "CONST int8 -2\n"
            "CONST int32 1073741825\n"
            "MUL\n"
            "MUL\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, 2147483646, ErrorManagement::Underflow));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSSMulExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint32 2\n"
            "CONST int32 -1073741824\n"
            "MUL\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, -2147483648));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSSMulExecution_FailedOverflow) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint32 2\n"
            "CONST int32 1073741825\n"
            "MUL\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, -2147483646, ErrorManagement::Overflow));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSSMulExecution_FailedPreviousError) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint32 2\n"
            "CONST uint32 2\n"
            "CONST int32 1073741825\n"
            "MUL\n"
            "MUL\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, -2147483646, ErrorManagement::Overflow));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSSMulExecution_FailedSaturated) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint32 2147483648\n"
            "CONST int32 -10\n"
            "MUL\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, 0, ErrorManagement::Overflow));
}

/*---------------------------------------------------------------------------*/
/*                                   DIV                                     */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestDivFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction2In1Out("float64", "float64", "float64");
    test.AddExpectedFunction2In1Out("float32", "float32", "float32");

    test.AddExpectedFunction2In1Out("int8",   "int8",     "int32");
    test.AddExpectedFunction2In1Out("int16",  "int16",    "int32");
    test.AddExpectedFunction2In1Out("int32",  "int32",    "int32");
    test.AddExpectedFunction2In1Out("int64",  "int64",    "int64");

    test.AddExpectedFunction2In1Out("uint8",  "uint8",    "uint32");
    test.AddExpectedFunction2In1Out("uint16", "uint16",   "uint32");
    test.AddExpectedFunction2In1Out("uint32", "uint32",   "uint32");
    test.AddExpectedFunction2In1Out("uint64", "uint64",   "uint64");

    test.AddExpectedFunctionCombinedInputs("int8",   "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("int16",  "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("uint16", "int32",    "int32");
    test.AddExpectedFunctionCombinedInputs("uint32", "int32",    "int32");

    test.AddExpectedFunctionCombinedInputs("int8",   "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("int16",  "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("int32",  "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint8",  "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint16", "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint32", "int64",    "int64");
    test.AddExpectedFunctionCombinedInputs("uint64", "int64",    "int64");

    test.AddExpectedFunctionCombinedInputs("uint8",  "uint32",   "uint32");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint32",   "uint32");

    test.AddExpectedFunctionCombinedInputs("uint8",  "uint64",   "uint64");
    test.AddExpectedFunctionCombinedInputs("uint16", "uint64",   "uint64");
    test.AddExpectedFunctionCombinedInputs("uint32", "uint64",   "uint64");

    ASSERT_TRUE(test.TestFunctionTypes("DIV", 2, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestDivFloatExecution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST float64 48.26\n"
            "CONST float64 -2\n"
            "DIV\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, -24.13));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestDivExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int8 -84\n"
            "CONST int32 -4\n"
            "DIV\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, 21));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestDivExecution_FailedOverflow) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int8 -84\n"
            "CONST int32 0\n"
            "DIV\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, 0, ErrorManagement::Overflow));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSDivExecution_Successful) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint32 915\n"
            "CONST int32 -3\n"
            "DIV\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, -305));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSDivExecution_FailedOverflow) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint32 84\n"
            "CONST int32 0\n"
            "DIV\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, 0, ErrorManagement::Overflow));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestSDivExecution_FailedSaturated) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint32 2147483648\n"
            "CONST int32 -2\n"
            "DIV\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, 0, ErrorManagement::Overflow));
}

/*---------------------------------------------------------------------------*/
/*                                  NEG                                      */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestNegFunctionTypes) {
    RuntimeEvaluatorFunctionTest test;

    test.AddExpectedFunction1In1Out("float64", "float64");
    test.AddExpectedFunction1In1Out("float32", "float32");

    test.AddExpectedFunction1In1Out("int8",   "int8" );
    test.AddExpectedFunction1In1Out("int16",  "int16");
    test.AddExpectedFunction1In1Out("int32",  "int32");
    test.AddExpectedFunction1In1Out("int64",  "int64");

    test.AddExpectedFunction1In1Out("uint8",  "int16" );
    test.AddExpectedFunction1In1Out("uint16", "int32");
    test.AddExpectedFunction1In1Out("uint32", "int64");
    test.AddExpectedFunction1In1Out("uint64", "int64");

    ASSERT_TRUE(test.TestFunctionTypes("NEG", 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestNegFloat64Execution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST float64 10.65\n"
            "NEG\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float64Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float64>(context, -10.65));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestNegFloat32Execution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST float32 10.65\n"
            "NEG\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, Float32Bit));
    ASSERT_TRUE(test.TestFloatFunctionExecution<float32>(context, -10.65));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestNegInt8Execution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int8 127\n"
            "NEG\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger8Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int8>(context, -127));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestNegInt16Execution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int16 127\n"
            "NEG\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger16Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int16>(context, -127));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestNegInt32Execution) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST int32 127\n"
            "NEG\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger32Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int32>(context, -127));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestNegExecution_UpcastSuccessful) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint32 4294967295\n"
            "NEG\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger64Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int64>(context, -4294967295));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestNegExecution_FailedUnderflow) {
    RuntimeEvaluatorFunctionTest test;

    StreamString rpnCode=
            "CONST uint64 9223372036854775808\n"
            "NEG\n"
            "WRITE RES1\n";

    RuntimeEvaluator context(rpnCode);

    ASSERT_TRUE(test.PrepareContext(context, InvalidType, SignedInteger64Bit));
    ASSERT_TRUE(test.TestIntFunctionExecution<int64>(context, -9223372036854775807, ErrorManagement::Underflow));
}

/*---------------------------------------------------------------------------*/
/*                                TryConsume                                 */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestTryConsumeSuccessful) {
    RuntimeEvaluatorFunctionTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunction functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, false, true, 1, 2));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestTryConsumeSuccessful_NoInputs) {
    RuntimeEvaluatorFunctionTest test;

    TypeDescriptor functionTypes[] = {Float64Bit};
    RuntimeEvaluatorFunction functionUT("Test", 0, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, false, true, 0, 2));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestTryConsumeSuccessful_NoOutputs) {
    RuntimeEvaluatorFunctionTest test;

    TypeDescriptor functionTypes[] = {Float32Bit};
    RuntimeEvaluatorFunction functionUT("Test", 1, 0, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, false, true, 1, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestTryConsumeSuccessful_MultipleInputs) {
    RuntimeEvaluatorFunctionTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, UnsignedInteger64Bit, Float64Bit};
    RuntimeEvaluatorFunction functionUT("Test", 2, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(UnsignedInteger64Bit);
    typeStack.Push(Float32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, false, true, 3, 2));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestTryConsumeSuccessful_MultipleOutputs) {
    RuntimeEvaluatorFunctionTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit, UnsignedInteger32Bit};
    RuntimeEvaluatorFunction functionUT("Test", 1, 2, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, false, true, 1, 3));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestTryConsumeSuccessful_StackLeftover) {
    RuntimeEvaluatorFunctionTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunction functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(UnsignedInteger8Bit);
    typeStack.Push(Float32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, false, true, 2, 3));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestTryConsumeSuccessful_MatchOutput) {
    RuntimeEvaluatorFunctionTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunction functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);
    typeStack.Push(Float64Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, true, true, 1, 2));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestTryConsumeFailed_WrongName) {
    RuntimeEvaluatorFunctionTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunction functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "WrongName", typeStack, false, false, 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestTryConsumeFailed_MissingInput) {
    RuntimeEvaluatorFunctionTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunction functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, false, false, 0, 0));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestTryConsumeFailed_WrongInputType) {
    RuntimeEvaluatorFunctionTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunction functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(SignedInteger32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, false, false, 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestTryConsumeFailed_MatchOutputMissingOutput) {
    RuntimeEvaluatorFunctionTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunction functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, true, false, 1, 1));
}

TEST(BareMetal_L4Configuration_RuntimeEvaluatorFunctionGTest,TestTryConsumeFailed_MatchOutputWrongOutput) {
    RuntimeEvaluatorFunctionTest test;

    TypeDescriptor functionTypes[] = {Float32Bit, Float64Bit};
    RuntimeEvaluatorFunction functionUT("Test", 1, 1, &(functionTypes[0]), &MockFunction);
    StaticStack<TypeDescriptor,32> typeStack;

    typeStack.Push(Float32Bit);
    typeStack.Push(Float32Bit);

    ASSERT_TRUE(test.TestTryConsume(functionUT, "Test", typeStack, true, false, 2, 2));
}
