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
     * @brief Tests types of registered RuntimeEvaluatorFunctions.
     */
    bool TestFunctionTypes(CCString functionName, uint8 numberOfInputs, uint8 numberOfOutputs);

    void AddInputCombinationFunctions(StreamString input1TypeName, StreamString input2TypeName, StreamString outputTypeName);
    void Add1InFunction(StreamString inputTypeName);
    void Add1OutFunction(StreamString outputTypeName);
    void Add1In1OutFunction(StreamString inputTypeName, StreamString outputTypeName);
    void Add2In1OutFunction(StreamString input1TypeName, StreamString input2TypeName, StreamString outputTypeName);
    void Add1In2OutFunction(StreamString inputTypeName, StreamString output1TypeName, StreamString output2TypeName);
    void AddFunction(uint8 numberOfInputs, StreamString inputs[], uint8 numberOfOutputs,  StreamString outputs[]);

    /**
     * @brief Tests TryConsume.
     */
    bool TestTryConsume(RuntimeEvaluatorFunctions &functionRecordUT, CCString inputName, StaticStack<TypeDescriptor,32> &typeStack, bool matchOutput, bool expectedRet, DataMemoryAddress initialDataStackSize, DataMemoryAddress expectedDataStackSize);

    /**
     * @brief Tests function execution.
     */
    template <typename T> inline bool TestIntFunctionExecution(RuntimeEvaluator &context, T expectedResult, ErrorManagement::ErrorType expectedReturn=ErrorManagement::NoError);
    template <typename T> inline bool TestFloatFunctionExecution(RuntimeEvaluator &context, T expectedResult, ErrorManagement::ErrorType expectedReturn=ErrorManagement::NoError);
    template <typename T> inline bool TestFloatFunctionExecution(RuntimeEvaluator &context, uint8 numberOfResults, T expectedResults[], ErrorManagement::ErrorType expectedReturn=ErrorManagement::NoError);

    bool PrepareContext(RuntimeEvaluator &context, TypeDescriptor inputType, TypeDescriptor outputType);
    template <typename T> void SetInputs(RuntimeEvaluator &context, T inputs[]);

private:
    /**
     * @brief Checks if types provided are within types lists.
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

    ret = context.Execute(RuntimeEvaluator::fastMode);

    context.BrowseOutputVariable(0,var);
    resultPointer = reinterpret_cast<T *>(&context.dataMemory[var->location]);

    return (ret == expectedReturn) && (*resultPointer == expectedResult);
}

template <typename T>
inline bool RuntimeEvaluatorFunctionsTest::TestFloatFunctionExecution(RuntimeEvaluator &context, T expectedResult, ErrorManagement::ErrorType expectedReturn) {

    ErrorManagement::ErrorType ret;
    VariableInformation *var;
    T *resultPointer;
    T epsilon;

    ret = context.Execute(RuntimeEvaluator::fastMode);

    context.BrowseOutputVariable(0,var);
    resultPointer = reinterpret_cast<T *>(&context.dataMemory[var->location]);
    epsilon = fabs(*resultPointer - expectedResult);

    return (ret == expectedReturn) && (epsilon < MAX_EPSILON);
}

template <typename T>
inline bool RuntimeEvaluatorFunctionsTest::TestFloatFunctionExecution(RuntimeEvaluator &context, uint8 numberOfResults, T expectedResults[], ErrorManagement::ErrorType expectedReturn) {

    ErrorManagement::ErrorType ret;
    bool resultOk = true;
    VariableInformation *var;
    T *resultPointer;

    ret = context.Execute(RuntimeEvaluator::fastMode);

    for (uint8 i = 0; (resultOk) && (i < numberOfResults); ++i) {
        context.BrowseOutputVariable(i,var);
        resultPointer = reinterpret_cast<T *>(&context.dataMemory[var->location]);
        resultOk &= (fabs(*resultPointer - expectedResults[i]) < MAX_EPSILON);
    }

    return (ret == expectedReturn) && (resultOk);
}

#endif /* RUNTIMEEVALUATORFUNCTIONSTEST_H_ */

