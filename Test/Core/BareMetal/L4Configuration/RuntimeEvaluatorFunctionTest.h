/**
 * @file RuntimeEvaluatorFunctionTest.h
 * @brief Header file for class RuntimeEvaluatorFunctionTest
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

 * @details This header file contains the declaration of the class RuntimeEvaluatorFunctionTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef RUNTIMEEVALUATORFUNCTIONTEST_H_
#define RUNTIMEEVALUATORFUNCTIONTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "RuntimeEvaluatorFunction.h"
#include "RuntimeEvaluator.h"
#include "ConfigurationDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#define MAX_EPSILON 1e-10

using namespace MARTe;

/**
 * @brief Tests all the RuntimeEvaluatorFunction functions.
 */
class RuntimeEvaluatorFunctionTest {

public:

    /**
     * @brief Tests RegisterFunction.
     */
    bool TestRegisterFunction();

    /**
     * @brief Tests multiple calls to RegisterFunction until maxFunctions reached.
     */
    bool TestRegisterFunctionMaxFunctions();

    /**
     * @brief Tests FindPCodeAndUpdateTypeStack.
     */
    bool TestFindPCodeAndUpdateTypeStack(const CCString functionName, bool expectedReturn, CodeMemoryElement expectedCode);

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the full constructor.
     */
    bool TestFullConstructor();

    /**
     * @brief Tests GetName().
     */
    bool TestGetName();

    /**
     * @brief Tests GetInputTypes() and GetOutputTypes().
     */
    bool TestGetTypes(uint16 numberOfInputs, uint16 numberOfOutputs, TypeDescriptor* types);

    /**
     * @brief   Tests types of registered RuntimeEvaluatorFunction.
     * @details Checks that all RuntimeEvaluatorFunction in FunctionRecords
     *          with functionName, numberOfInputs, and numberOfOutputs match
     *          with a set of types in expectedFunctionTypesCdb. It also checks
     *          that there is a FunctionRecord for each set of types in
     *          expectedFunctionTypesCdb.
     */
    bool TestFunctionTypes(CCString functionName, uint8 numberOfInputs, uint8 numberOfOutputs);

    /**
     * @brief   Tests ExecuteFunction().
     * @details Checks that calling RuntimeEvaluator::Execute on the provided
     *          RuntimeEvaluator returns expectedError.
     */
    bool TestFunctionExecution(RuntimeEvaluator &context, ErrorManagement::ErrorType expectedReturn=ErrorManagement::NoError);

    /**
     * @brief   Tests ExecuteFunction().
     * @details Checks that calling RuntimeEvaluator::Execute on the provided
     *          RuntimeEvaluator returns expectedError.
     *          If `expectedError == ErrorManagement::NoError`, then it also
     *          checks that the output variable is exactly equal to expectedResult.
     */
    template <typename T> inline bool TestIntFunctionExecution(RuntimeEvaluator &context, T expectedResult, ErrorManagement::ErrorType expectedReturn=ErrorManagement::NoError);

    /**
     * @brief   Tests ExecuteFunction().
     * @details Checks that calling RuntimeEvaluator::Execute on the provided
     *          RuntimeEvaluator returns expectedError.
     *          If `expectedError == ErrorManagement::NoError`, then it also
     *          checks that the output variable is close enough to expectedResult.
     */
    template <typename T> inline bool TestFloatFunctionExecution(RuntimeEvaluator &context, T expectedResult, ErrorManagement::ErrorType expectedReturn=ErrorManagement::NoError);

    /**
     * @brief   Tests ExecuteFunction().
     * @details Checks that calling RuntimeEvaluator::Execute on the provided
     *          RuntimeEvaluator returns expectedError.
     *          If `expectedError == ErrorManagement::NoError`, then it also
     *          checks that the output variables are close enough to elements in
     *          expectedResults array.
     */
    template <typename T> inline bool TestFloatFunctionExecution(RuntimeEvaluator &context, uint8 numberOfResults, T expectedResults[], ErrorManagement::ErrorType expectedReturn=ErrorManagement::NoError);

    /**
     * @brief   Tests TryConsume().
     * @details Checks that calling the method returns expectedRet.
     *          If expectedRet is true:
     *          - checks that dataStackSize is set to expectedDataStackSize
     *          - checks that used types are removed from typeStack and the
     *            rest is left untouched
     *          If expectedRet is false:
     *          - checks that dataStackSize is set to expectedDataStackSize
     *          - checks that typeStack is left untouched
     */
    bool TestTryConsume(RuntimeEvaluatorFunction &functionRecordUT, CCString inputName, StaticStack<TypeDescriptor,32> &typeStack, bool matchOutput, bool expectedRet, DataMemoryAddress initialDataStackSize, DataMemoryAddress expectedDataStackSize);

    /**
     * @brief Adds sets of input-output types to expectedFunctionTypesCdb with all possible
     *        combinations of provided input types with the same output type.
     */
    void AddExpectedFunctionCombinedInputs(StreamString input1TypeName, StreamString input2TypeName, StreamString outputTypeName);

    /**
     * @brief Adds set of input-output types to expectedFunctionTypesCdb.
     */
    void AddExpectedFunction1In(StreamString inputTypeName);
    void AddExpectedFunction1Out(StreamString outputTypeName);
    void AddExpectedFunction1In1Out(StreamString inputTypeName, StreamString outputTypeName);
    void AddExpectedFunction2In1Out(StreamString input1TypeName, StreamString input2TypeName, StreamString outputTypeName);
    void AddExpectedFunction1In2Out(StreamString inputTypeName, StreamString output1TypeName, StreamString output2TypeName);

    /**
     * @brief Adds set of input-output types to expectedFunctionTypesCdb.
     */
    void AddExpectedFunction(uint8 numberOfInputs, StreamString inputs[], uint8 numberOfOutputs,  StreamString outputs[]);

    /**
     * @brief Prepares RuntimeEvaluator for its execution:
     *        - calls RuntimeEvaluator::ExtractVariables
     *        - configures RuntimeEvaluator variables with type and externalLocation
     *        - calls RuntimeEvaluator::Compile
     */
    bool PrepareContext(RuntimeEvaluator &context, TypeDescriptor inputType, TypeDescriptor outputType, void * inputExternalLocation = NULL, void *outputExternalLocation = NULL);
    bool PrepareContext(RuntimeEvaluator &context, TypeDescriptor input1Type, TypeDescriptor input2Type, TypeDescriptor outputType);

    /**
     * @brief Sets input variable values of RuntimeEvaluator.
     */
    template <typename T> void SetInputs(RuntimeEvaluator &context, T inputs[]);

private:
    /**
     * @brief   Checks if set of input-output types are within types database.
     *          Set foundIndex to the index where the set is found.
     * @return  true if set of input-output types found within types database.
     */
    bool FindTypesInCdb(CCString &foundIndex, Vector<TypeDescriptor> &inputTypes, Vector<TypeDescriptor> &outputTypes, ConfigurationDatabase &FunctionTypesCdb);

    /**
     * @brief ConfigurationDb that holds the set of input-output types expected
     *        by the test.
     */
    ConfigurationDatabase expectedFunctionTypesCdb;
};

/**
 * @brief Function to mock RuntimeEvaluatorFunction implementation
 */
void MockFunction(RuntimeEvaluator &evaluator);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <typename T>
void RuntimeEvaluatorFunctionTest::SetInputs(RuntimeEvaluator &context, T inputs[]) {

    VariableInformation *var;
    T *inputPointer;

    for (uint32 i = 0; (context.BrowseInputVariable(i,var)); ++i) {
        inputPointer = reinterpret_cast<T *>(&context.dataMemory[var->location]);
        *inputPointer = inputs[i];
    }
}

template <typename T>
inline bool RuntimeEvaluatorFunctionTest::TestIntFunctionExecution(RuntimeEvaluator &context, T expectedResult, ErrorManagement::ErrorType expectedReturn) {

    VariableInformation *var;
    T *resultPointer;
    bool ok;

    ok = TestFunctionExecution(context, expectedReturn);

    if (expectedReturn == ErrorManagement::NoError) {
        context.BrowseOutputVariable(0,var);
        resultPointer = reinterpret_cast<T *>(&context.dataMemory[var->location]);
        ok &= (*resultPointer == expectedResult);
    }

    return ok;
}

template <typename T>
inline bool RuntimeEvaluatorFunctionTest::TestFloatFunctionExecution(RuntimeEvaluator &context, T expectedResult, ErrorManagement::ErrorType expectedReturn) {

    VariableInformation *var;
    T *resultPointer;
    T epsilon;
    bool ok;

    ok = TestFunctionExecution(context, expectedReturn);

    if (expectedReturn == ErrorManagement::NoError) {
        context.BrowseOutputVariable(0,var);
        resultPointer = reinterpret_cast<T *>(&context.dataMemory[var->location]);
        epsilon = fabs(*resultPointer - expectedResult);
        ok &= (epsilon < MAX_EPSILON);
    }

    return ok;
}

template <typename T>
inline bool RuntimeEvaluatorFunctionTest::TestFloatFunctionExecution(RuntimeEvaluator &context, uint8 numberOfResults, T expectedResults[], ErrorManagement::ErrorType expectedReturn) {

    VariableInformation *var;
    T *resultPointer;
    T epsilon;
    bool ok = true;

    ok = TestFunctionExecution(context, expectedReturn);

    if (expectedReturn == ErrorManagement::NoError) {
        for (uint8 i = 0; (ok) && (i < numberOfResults); ++i) {
            context.BrowseOutputVariable(i,var);
            resultPointer = reinterpret_cast<T *>(&context.dataMemory[var->location]);
            epsilon = fabs(*resultPointer - expectedResults[i]);
            ok &= (epsilon < MAX_EPSILON);
        }
    }

    return ok;
}

#endif /* RUNTIMEEVALUATORFUNCTIONTEST_H_ */

