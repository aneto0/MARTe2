/**
 * @file RuntimeEvaluatorFunctionsTest.h
 * @brief Header file for class RuntimeEvaluatorFunctionsTest
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

 * @details This header file contains the declaration of the class RuntimeEvaluatorFunctionsTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef RUNTIMEEVALUATORFUNCTIONSTEST_H_
#define RUNTIMEEVALUATORFUNCTIONSTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "RuntimeEvaluatorFunctions.h"
#include "RuntimeEvaluator.h"
#include "ConfigurationDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#define MAX_EPSILON 1e-10

using namespace MARTe;

/**
 * @brief Tests all the RuntimeEvaluatorFunctions functions.
 */
class RuntimeEvaluatorFunctionsTest {

public:

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the full constructor.
     */
    bool TestFullConstructor();

    /**
     * @brief Tests types of registered RuntimeEvaluatorFunctions.
     */
    bool TestFunctionTypes(CCString functionName, uint8 numberOfInputs, uint8 numberOfOutputs);

    /**
     * @brief Add functions to expectedFunctionTypesCdb with a combination of provided inputs.
     */
    void AddExpectedFunctionCombinedInputs(StreamString input1TypeName, StreamString input2TypeName, StreamString outputTypeName);

    /**
     * @brief Add functions to expectedFunctionTypesCdb with different number of input, outputs.
     */
    void AddExpectedFunction1In(StreamString inputTypeName);
    void AddExpectedFunction1Out(StreamString outputTypeName);
    void AddExpectedFunction1In1Out(StreamString inputTypeName, StreamString outputTypeName);
    void AddExpectedFunction2In1Out(StreamString input1TypeName, StreamString input2TypeName, StreamString outputTypeName);
    void AddExpectedFunction1In2Out(StreamString inputTypeName, StreamString output1TypeName, StreamString output2TypeName);

    /**
     * @brief Add functions to expectedFunctionTypesCdb.
     */
    void AddExpectedFunction(uint8 numberOfInputs, StreamString inputs[], uint8 numberOfOutputs,  StreamString outputs[]);

    /**
     * @brief Tests TryConsume.
     */
    bool TestTryConsume(RuntimeEvaluatorFunctions &functionRecordUT, CCString inputName, StaticStack<TypeDescriptor,32> &typeStack, bool matchOutput, bool expectedRet, DataMemoryAddress initialDataStackSize, DataMemoryAddress expectedDataStackSize);

    /**
     * @brief Tests function execution.
     */
    bool TestFunctionExecution(RuntimeEvaluator &context, ErrorManagement::ErrorType expectedReturn=ErrorManagement::NoError);
    template <typename T> inline bool TestIntFunctionExecution(RuntimeEvaluator &context, T expectedResult, ErrorManagement::ErrorType expectedReturn=ErrorManagement::NoError);
    template <typename T> inline bool TestFloatFunctionExecution(RuntimeEvaluator &context, T expectedResult, ErrorManagement::ErrorType expectedReturn=ErrorManagement::NoError);
    template <typename T> inline bool TestFloatFunctionExecution(RuntimeEvaluator &context, uint8 numberOfResults, T expectedResults[], ErrorManagement::ErrorType expectedReturn=ErrorManagement::NoError);

    /**
     * @brief Prepares RuntimeEvaluator for its execution.
     */
    bool PrepareContext(RuntimeEvaluator &context, TypeDescriptor inputType, TypeDescriptor outputType, void * inputExternalLocation = NULL, void *outputExternalLocation = NULL);
    bool PrepareContext(RuntimeEvaluator &context, TypeDescriptor input1Type, TypeDescriptor input2Type, TypeDescriptor outputType);

    /**
     * @brief Sets input variable values of RuntimeEvaluator.
     */
    template <typename T> void SetInputs(RuntimeEvaluator &context, T inputs[]);

private:
    /**
     * @brief Checks if provided types are within types database.
     */
    bool FindTypesInCdb(CCString &foundIndex, Vector<TypeDescriptor> &inputTypes, Vector<TypeDescriptor> &outputTypes, ConfigurationDatabase &FunctionTypesCdb);

    ConfigurationDatabase expectedFunctionTypesCdb;
};


void MockFunction(RuntimeEvaluator &evaluator);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <typename T>
void RuntimeEvaluatorFunctionsTest::SetInputs(RuntimeEvaluator &context, T inputs[]) {

    VariableInformation *var;
    T *inputPointer;

    for (uint32 i = 0; (context.BrowseInputVariable(i,var)); ++i) {
        inputPointer = reinterpret_cast<T *>(&context.dataMemory[var->location]);
        *inputPointer = inputs[i];
    }
}

template <typename T>
inline bool RuntimeEvaluatorFunctionsTest::TestIntFunctionExecution(RuntimeEvaluator &context, T expectedResult, ErrorManagement::ErrorType expectedReturn) {

    ErrorManagement::ErrorType ret;
    VariableInformation *var;
    T *resultPointer;
    bool ok = true;

    ret = context.Execute(RuntimeEvaluator::fastMode);

    context.BrowseOutputVariable(0,var);
    resultPointer = reinterpret_cast<T *>(&context.dataMemory[var->location]);

    if (expectedReturn == ErrorManagement::NoError) {
        ok &= (ret.ErrorsCleared());
        ok &= (*resultPointer == expectedResult);
    } else {
        ok &= ret.Contains(expectedReturn);
    }

    return ok;
}

template <typename T>
inline bool RuntimeEvaluatorFunctionsTest::TestFloatFunctionExecution(RuntimeEvaluator &context, T expectedResult, ErrorManagement::ErrorType expectedReturn) {

    ErrorManagement::ErrorType ret;
    VariableInformation *var;
    T *resultPointer;
    T epsilon;
    bool ok = true;

    ret = context.Execute(RuntimeEvaluator::fastMode);

    context.BrowseOutputVariable(0,var);
    resultPointer = reinterpret_cast<T *>(&context.dataMemory[var->location]);
    epsilon = fabs(*resultPointer - expectedResult);

    if (expectedReturn == ErrorManagement::NoError) {
        ok &= (ret.ErrorsCleared());
        ok &= (epsilon < MAX_EPSILON);
    } else {
        ok &= ret.Contains(expectedReturn);
    }

    return ok;
}

template <typename T>
inline bool RuntimeEvaluatorFunctionsTest::TestFloatFunctionExecution(RuntimeEvaluator &context, uint8 numberOfResults, T expectedResults[], ErrorManagement::ErrorType expectedReturn) {

    ErrorManagement::ErrorType ret;
    VariableInformation *var;
    T *resultPointer;
    T epsilon;
    bool ok = true;

    ret = context.Execute(RuntimeEvaluator::fastMode);

    if (expectedReturn == ErrorManagement::NoError) {
        ok &= (ret.ErrorsCleared());
        for (uint8 i = 0; (ok) && (i < numberOfResults); ++i) {
            context.BrowseOutputVariable(i,var);
            resultPointer = reinterpret_cast<T *>(&context.dataMemory[var->location]);
            epsilon = fabs(*resultPointer - expectedResults[i]);
            ok &= (epsilon < MAX_EPSILON);
        }
    } else {
        ok &= ret.Contains(expectedReturn);
    }

    return ok;
}

#endif /* RUNTIMEEVALUATORFUNCTIONSTEST_H_ */

