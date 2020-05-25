/**
 * @file RuntimeEvaluator.h
 * @brief Header file for class PseudoCodeTest
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

 * @details This header file contains the declaration of the class PseudoCodeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef RUNTIMEEVALUATORTEST_H_
#define RUNTIMEEVALUATORTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <iostream>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RuntimeEvaluator.h"
#include "RuntimeEvaluatorFunctions.h"
#include "LinkedListHolderT.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the PseudoCode functions
 */
class RuntimeEvaluatorTest {
public:

    /**
     * @brief Tests the constructor of Context class.
     */
    bool TestConstructor();

    /**
     * @brief TODO
     */
    bool TestIntegration();
    
    /**
     * @brief   Tests all methods to get the memory address
     *          of an input variable.
     * @details Checks coherency between method outputs and the address
     *          of the variables computed manually with pointer
     *          arithmetics. 
     */
    bool TestGetInputVariableMemory();
    
    /**
     * @brief   Tests all methods to get the memory address
     *          of an output variable.
     * @details Checks coherency between method outputs and the address
     *          of the variables computed manually with pointer
     *          arithmetics. 
     */
    bool TestGetOutputVariableMemory();
    
    /**
     * @brief   Tests all methods to set the type of a variable.
     * @details Checks if the method works by manually inspecting each
     *          `type` member of the variables.
     */
    bool TestSetVariableType();
    
    /**
     * @brief   Tests Push(), Pop() and Peek() methods.
     * @details RuntimeEvaluator has an internal #stack. The #stack is
     *          public since it's managed mostly by RuntimeEvaluatorFunctions.
     *          The stack size is increased by compiling a long sequence
     *          of `READ` and `ADD` operations. Push(), Pop() and Peek()
     *          methods are then tested on the allocated space.
     */
     bool TestPushPopPeek();
    
    /**
     * @brief   Tests the Execute() method.
     */
     bool TestExecute(CCString rpnCode, ErrorManagement::ErrorType expectedError,
                      RuntimeEvaluator::executionMode mode = RuntimeEvaluator::fastMode, StreamI *debugStream=NULL_PTR(StreamI *), CodeMemoryAddress *step=NULL);
    
    /**
     * @brief     Tests RuntimeEvaluator in case of errors.
     * @details   Checks that RuntimeEvaluator fails in case it is fed
     *            with wrong inputs and that it fails at the right stage
     *            in the evaluation sequence:
     *            - `READ`, `WRITE` and `CONST` commands are dealt with
     *              in the ExtractVariable() method, so they are expected
     *              to fail mostly there.
     *            - `CAST` is dealt with in the Compile() method and
     *              is expected to fail there.
     * @param[in] rpnCode       the input RPN code.
     * @param[in] expectedError the expected error, that is:
     *            - `SyntaxError` if RuntimeEvaluator is expected
     *              to fail at RuntimeEvaluator::ExtractVariables(),
     *            - `InitializationError` if RuntimeEvaluator is expected
     *              to fail at RuntimeEvaluator::Compile(),
     *            - `FatalError` if RuntimeEvaluator is expected
     *              to fail at RuntimeEvaluator::Execute().
     * @returns   `true` if RuntimeEvaluator fails at the correct stage.
     */
    bool TestError(CCString rpnCode, ErrorManagement::ErrorType expectedError);
    
    // tests that can be removed:
    
    /**
     * @brief TODO
     */
    bool TestExpression(CCString rpnCode, float64 valueArray[]);

// private:
    
    struct VariableListElement {
        
        VariableInformation var;
        float64             expectedValue;
        
    };
    
    void SetTestInputVariable (CCString name, TypeDescriptor type, void *externalLocation = NULL, float64 expectedVarValue = 0);
    void SetTestOutputVariable(CCString name, TypeDescriptor type, void *externalLocation = NULL, float64 expectedVarValue = 0);
    void SetTestVariable(StaticList<VariableListElement*>& list, CCString name, TypeDescriptor type, void *externalLocation, float64 expectedVarValue);
    
    StaticList<VariableListElement*> usedInputVariables;
    StaticList<VariableListElement*> usedOutputVariables;
    
    /**
     * @brief   Custom RuntimeEvaluatorFunctions function
     * @details This function is intentionally broken to reach Execute()
     *          safeMode checks.
     */
    //void MockRead(RuntimeEvaluator &context);
    
    /*---------------------------------------------------------------------------*/
    /*    ↑ N                   CODE LINE DO NOT CROSS                     D ↓   */
    /*---------------------------------------------------------------------------*/

    void AddExpectedInputVariable(CCString name, TypeDescriptor type, DataMemoryAddress location, void *externalLocation, bool variableUsed);
    void AddExpectedOutputVariable(CCString name, TypeDescriptor type, DataMemoryAddress location, void *externalLocation, bool variableUsed);
    
    void AddExpectedFunctionInMemory(StreamString name, StreamString inputType, StreamString outputType);
    void AddExpectedVariableInMemory(CodeMemoryElement location);

    bool TestExtractVariables(CCString rpnCode, ErrorManagement::ErrorType expectedError);
    bool TestCompile(RuntimeEvaluator &evaluator, ErrorManagement::ErrorType expectedError, uint32 expectedDataSize);

private:
    void AddExpectedVariable(LinkedListHolderT<VariableInformation> &varList, CCString name, TypeDescriptor type, DataMemoryAddress location, void *externalLocation, bool variableUsed);
    bool RemoveMatchingVariable(const VariableInformation *var, LinkedListHolderT<VariableInformation> &varList);
    bool VariablesMatch(const VariableInformation *var1, const VariableInformation *var2);

    bool RecordMatchesExpectedFunction(RuntimeEvaluatorFunctions &functionRecord);

    LinkedListHolderT<VariableInformation, true> expectedInputVariables;
    LinkedListHolderT<VariableInformation, true> expectedOutputVariables;

    ConfigurationDatabase expectedCodeMemory;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* RUNTIMEEVALUATORTEST_H_ */

