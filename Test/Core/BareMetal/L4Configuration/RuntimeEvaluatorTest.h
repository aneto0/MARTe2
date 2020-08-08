/**
 * @file RuntimeEvaluatorTest.h
 * @brief Header file for class RuntimeEvaluatorTest
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

 * @details This header file contains the declaration of the class RuntimeEvaluatorTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef RUNTIMEEVALUATORTEST_H_
#define RUNTIMEEVALUATORTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RuntimeEvaluator.h"
#include "RuntimeEvaluatorFunction.h"
#include "LinkedListHolderT.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the RuntimeEvaluator functions
 */
class RuntimeEvaluatorTest {
public:

    /**
     * @brief Tests the constructor of Context class.
     */
    bool TestConstructor();

    /**
     * @brief   Tests the RuntimeEvaluator through all stages of its
     *          lifecycle i.e. ExtractVariables - Compile - Execute,
     *          with a rich RPN Code.
     */
    bool TestIntegration();

    /**
     * @brief   Tests GetPseudoCode.
     */
    bool TestGetPseudoCode();

    /**
     * @brief   Tests BrowseInputVariable.
     */
    bool TestBrowseInputVariable(RuntimeEvaluator &evaluator, uint32 index, ErrorManagement::ErrorType expectedError, CCString expectedName);

    /**
     * @brief   Tests BrowseOutputVariable.
     */
    bool TestBrowseOutputVariable(RuntimeEvaluator &evaluator, uint32 index, ErrorManagement::ErrorType expectedError, CCString expectedName);
    
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
     * @brief   Tests all methods to set the memory location of a variable.
     * @details Checks coherency set addresses and the address
     *          of the variables computed manually with pointer
     *          arithmetics.
     */
    bool TestSetVariableMemory();
    
    /**
     * @brief   Tests Variable.
     */
    bool TestVariable();

    /**
     * @brief   Tests Push(), Pop() and Peek() methods.
     * @details RuntimeEvaluator has an internal #stack. The #stack is
     *          public since it's managed mostly by RuntimeEvaluatorFunction.
     *          The stack size is increased by compiling a long sequence
     *          of `READ` and `ADD` operations. Push(), Pop() and Peek()
     *          methods are then tested on the allocated space.
     */
     bool TestPushPopPeek();
    
    /**
     * @brief   Tests the Execute() method.
     * @details Checks if execution stops with the correct expectedError.
     *          If `expectedError == ErrorManagement::NoError`, then
     *          expected values for output variables are checked.
     *          Expected values are set with the last parameter of
     *          AddExpectedOutputVariable().
     */
     bool TestExecute(CCString rpnCode, ErrorManagement::ErrorType expectedError, RuntimeEvaluator::executionMode mode = RuntimeEvaluator::fastMode);

     /**
      * @brief   Tests the ExtractVariables() method.
      * @details Checks if execution stops with the correct expectedError.
      *          If `expectedError == ErrorManagement::NoError`, then checks
      *          - variableInformation returned by RuntimeEvaluator::BrowseInputVariables
      *            match those in expectedInputVariables
      *          - variableInformation returned by RuntimeEvaluator::BrowseOutputVariables
      *            match those in expectedOutputVariables
      */
     bool TestExtractVariables(CCString rpnCode, ErrorManagement::ErrorType expectedError);

     /**
      * @brief   Tests the Compile() method.
      * @details Checks if execution stops with the correct expectedError.
      *          If `expectedError == ErrorManagement::NoError`, then checks
      *          - variableInformation returned by RuntimeEvaluator::BrowseInputVariables
      *            match those in expectedInputVariables
      *          - variableInformation returned by RuntimeEvaluator::BrowseOutputVariables
      *            match those in expectedOutputVariables
      *          - locations stored in RuntimeEvaluator::codeMemory match those
      *            in expectedCodeMemory
      */
     bool TestCompile(RuntimeEvaluator &evaluator, ErrorManagement::ErrorType expectedError, uint32 expectedDataSize);

    /**
     * @brief Insert a variable in usedInputVariables so that TestExecute
     *        can load it in a RuntimeEvaluator object.
     */
    void SetTestInputVariable (CCString name, TypeDescriptor type, void *externalLocation = NULL, float64 expectedVarValue = 0);

    /**
     * @brief Insert a variable in usedOutputVariables so that TestExecute
     *        can load it in a RuntimeEvaluator object.
     */
    void SetTestOutputVariable(CCString name, TypeDescriptor type, void *externalLocation = NULL, float64 expectedVarValue = 0);

    /**
     * @brief Insert a variableInformation in expectedInputVariables so that it can be
     *        checked against the variables contained by RuntimeEvaluator
     */
    void AddExpectedInputVariable(CCString name, TypeDescriptor type, DataMemoryAddress location, void *externalLocation, bool variableUsed);

    /**
     * @brief Insert a variableInformation in expectedOutputVariables so that it can be
     *        checked against the variables contained by RuntimeEvaluator
     */
    void AddExpectedOutputVariable(CCString name, TypeDescriptor type, DataMemoryAddress location, void *externalLocation, bool variableUsed);

    /**
     * @brief Insert a Function in expectedCodeMemory so that it can be
     *        checked against the RuntimeEvaluator::codeMemory
     */
    void AddExpectedFunctionInMemory(StreamString name, StreamString inputType, StreamString outputType);

    /**
     * @brief Insert a variable location in expectedCodeMemory so that it can be
     *        checked against the RuntimeEvaluator::codeMemory
     */
    void AddExpectedVariableInMemory(CodeMemoryElement location);

private:

    /**
     * @brief Insert a variableInformation in a LinkedList
     */
    void AddExpectedVariable(LinkedListHolderT<VariableInformation> &varList, CCString name, TypeDescriptor type, DataMemoryAddress location, void *externalLocation, bool variableUsed);

    /**
     * @brief Searches inside a LinkedList a VariableInformation equals to var
     *        and removes it
     * @return true if found inside list a VariableInformation equals to var
     */
    bool RemoveMatchingVariable(const VariableInformation *var, LinkedListHolderT<VariableInformation> &varList);

    /**
     * @brief Compares two VariableInformation arguments
     * @return true if the two VariableInformation arguments are equal
     */
    bool VariablesMatch(const VariableInformation *var1, const VariableInformation *var2);

    /**
     * @brief Compares RuntimeEvaluatorFunction with next element in expectedCodeMemory
     * @pre functionRecord has at at most 1 input/output
     * @return true if functionRecord has expected name, input and output type
     */
    bool RecordMatchesExpectedFunction(RuntimeEvaluatorFunction &functionRecord);

    /**
     * @brief LinkedList that holds the expected input VariableInformation by the test.
     */
    LinkedListHolderT<VariableInformation, true> expectedInputVariables;

    /**
     * @brief LinkedList that holds the expected output VariableInformation by the test.
     */
    LinkedListHolderT<VariableInformation, true> expectedOutputVariables;

    /**
     * @brief ConfigurationDb that holds the codeMemory (function details
     *        and variable locations) expected by the test.
     */
    ConfigurationDatabase expectedCodeMemory;
    
    /**
     * @brief A structure that holds a RuntimeEvaluator variable along
     *        with its value.
     */
    struct VariableListElement {
        VariableInformation var;
        float64             expectedValue;
    };
    
    /**
     * @brief Helper function that insert a VariableListElement in the
     *        lists used by TestExecute().
     */
    void SetTestVariable(StaticList<VariableListElement*>& list, CCString name, TypeDescriptor type, void *externalLocation, float64 expectedVarValue);
    
    /**
     * @brief Helper lists that holds variables used by TestExecute().
     */
    //@{
        StaticList<VariableListElement*> usedInputVariables;
        StaticList<VariableListElement*> usedOutputVariables;
    //@}
};

/**
 * @brief   Custom RuntimeEvaluatorFunction function 
 * @details This function intentionally pushes to the stack without
 *          declaring to do so when registered in order to reach Execute()
 *          safeMode checks. 
 */
void MockRead(RuntimeEvaluator &context);

/**
 * @brief   Custom RuntimeEvaluatorFunction function 
 * @details This function intentionally pops from the stack without
 *          declaring to do so when registered in order to reach Execute()
 *          safeMode checks. 
 */
void MockWrite(RuntimeEvaluator &context);

/**
 * @brief   Custom RuntimeEvaluatorFunction function 
 * @details This function intentionally raises an error during
 *          execution in order to reach Execute() safeMode checks. 
 */
void MockExecutionError(RuntimeEvaluator &context);

/**
 * @brief   Custom RuntimeEvaluatorFunction function 
 * @details This function intentionally raises an ErrorManagement::OutOfRange
 *          error during execution in order to reach Execute() safeMode checks. 
 */
void MockOutOfRangeExecutionError(RuntimeEvaluator &context);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* RUNTIMEEVALUATORTEST_H_ */

