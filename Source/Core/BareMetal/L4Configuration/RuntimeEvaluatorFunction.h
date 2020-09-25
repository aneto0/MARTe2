/**
 * @file RuntimeEvaluatorFunction.h
 * @brief Header file for class RuntimeEvaluatorFunction
 * @date 08/04/2020
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef RUNTIMEEVALUATORFUNCTION_H_
#define RUNTIMEEVALUATORFUNCTION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "RuntimeEvaluator.h"
#include "SafeMath.h"
#include "StaticStack.h"
#include "VariableInformation.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief   Pointer to a runtime function.
 * @details Defines a pointer-to-function type named Function which
 *          points to a function that takes a RuntimeEvaluator
 *          as input and returns nothing.
 *          Functions for runtime evaluation defined in
 *          RuntimeEvaluatorFunction.cpp are all of this kind (e.g.
 *          Addition_3T, Subtraction_3T etc).
 *          When invoked, these functions act on the input RuntimeEvaluator
 *          object using RuntimeEvaluator::Push(), RuntimeEvaluator::Pop()
 *          and RuntimeEvaluator::Peek() methods (see RuntimeEvaluator
 *          and RuntimeEvaluatorFunction documentation for details).
 */
typedef void (*Function)(RuntimeEvaluator & context);

/**
 * @brief   Function object required by RuntimeEvaluator.
 * 
 * @details This file holds the definition of a function class
 *          as well as a bunch of macros and templates to register
 *          all combinations of functions and types required by
 *          RuntimeEvaluator during runtime execution.
 * 
 * This class is a wrapper of the actual function which is defined
 * elsewhere (often in a templated fashion). This class only holds
 * a pointer to that function (the Function member, which is
 * passed at construction time), and provides a number of helper
 * methods that allows RuntimeEvaluator to manage the function (GetName(),
 * GetInputTypes(), ExecuteFunction() etc).
 * 
 * Each function is registerd and made available in the following way:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * // Example function that multiplies a number by ten.
 * void ExampleFunction(RuntimeEvaluator &evaluator) {
 *     float32 x1;
 *     evaluator.Pop(x1);
 *     x1 = x1 * 10;
 *     evaluator.Push(x1);
 * }
 * 
 * // Registration of the function.
 * TypeDescriptor types[] = {Float32Bit, Float32Bit};
 * RuntimeEvaluatorFunction exampleFunction("EXFUN", 1, 1, types, ExampleFunction);
 * RegisterFunction(exampleFunction);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * This allows RuntimeEvaluator to call a function named `ExampleFunction`
 * (which multiplies the first element on the stack by ten) by using the name `EXFUN`.
 * The `EXFUN` function is expected to pop exactly one `float32` element
 * from the RuntimeEvaluator stack and then push exactly one `float32`
 * element on the same stack.
 * 
 * What happens behind the scenes is that upon registration each function
 * is added to the #functionRecords array (see RuntimeEvaluatorFunction.h,
 * outside the class definition).
 * When RuntimeEvaluator needs to call a function, it searches the #functionRecords
 * for the function its stack is currently pointing to, and then calls
 * the ExecuteFunction() method of that function passing itself
 * as an argument so that the called function can act on the stack of the
 * calling RuntimeEvaluator.
 * 
 */
/*lint -e(9109) Forward declaration in RuntimeEvaluator.h is required (RuntimeEvaluator and RuntimeEvaluatorFunction are circular-dependant). */
class RuntimeEvaluatorFunction {
public:

    /**
     * @brief Default constructor.
     */
    RuntimeEvaluatorFunction();

    /**
     * @brief     Full constructor.
     * @details   Instantiate a function object that can then be registered
     *            and made available to RuntimeEvaluator.
     * @param[in] nameIn            name of the function
     * @param[in] numberOfInputsIn  number of elements this function is
     *                              expected to pop from the RuntimeEvaluator stack
     * @param[in] numberOfOutputsIn number of elements this function is
     *                              expected to push to the RuntimeEvaluator stack
     * @param[in] typesIn           pointer to the array holding the types
     *                              the function is expected to manage
     * @param[in] functionIn        pointer to the actual C++ function
     *                              that will be executed.
     */
    RuntimeEvaluatorFunction(const CCString &nameIn, const uint16 numberOfInputsIn, const uint16 numberOfOutputsIn, TypeDescriptor* const typesIn, const Function functionIn);
    
    /**
     * @brief Get the name of the function.
     */
    StreamString GetName() const {return name;}
    
    /**
     * @brief Get the input types set for the function
     */
    Vector<TypeDescriptor> GetInputTypes();

    /**
     * @brief Get the output types set for the function
     * @note It only includes types of outputs going to stack.
     *       It does not include types of outputs going to memory variables
     */
    Vector<TypeDescriptor> GetOutputTypes();
    
    /**
     * @brief     Executes a function.
     * @param[in] context the RuntimeEvaluator object on whose stack
     *                    the function shall be executed
     * @details   This method executes the function on the input
     *            RuntimeEvaluator. This means that the function acts
     *            on the input RuntimeEvaluator stack and carries out
     *            the required operations on it.
     *            For example the calling the `SUM` function on a
     *            RuntimeEvaluator object will pop two elements
     *            from the input RuntimeEvaluator, sum them and then
     *            push the result to the input RuntimeEvaluator stack.
     */
    void ExecuteFunction(RuntimeEvaluator &context) const {function(context);}

    /**
     * @brief   Replaces the type on the stack with the result type.
     * @returns `true` if the name and types matches.
     * @note    Also simulates variations on the dataStack.
     */
    bool TryConsume(CCString nameIn,StaticStack<TypeDescriptor,32u> &typeStack, const bool matchOutput,DataMemoryAddress &dataStackSize) const;

private:
    /**
     * @brief   The name of the functions as used in the RPN code.
     * @details This is the name of the function as it will be referred
     *          to in the RPN code processed by RuntimeEvaluator.
     *          On the other hand, RuntimeEvaluator will need to use
     *          this name to call the function.
     */
    StreamString            name;

    /**
     * @brief   How many stack elements the function will consume.
     * @details When called with `function.ExecuteFunction(evaluator)`,
     *          each function acts on the `evaluator` stack. #numberOfInputs
     *          is the number of stack elements this function will pop
     *          from the stack when called.
     */
    uint16                  numberOfInputs;

    /**
     * @brief   How many stack elements the function will produce.
     * @details When called with `function.ExecuteFunction(evaluator)`,
     *          each function acts on the `evaluator` stack. #numberOfOutputs
     *          is the number of stack elements this function will push
     *          to the stack when called.
     */
    uint16                  numberOfOutputs;

    /**
     * @brief   The array holding the types of inputs and outputs.
     * @details Types are saved in the array in the following order:
     *          `stack inputs | stack outputs | memory variables outputs`
     */
    TypeDescriptor*   types;

    /**
     * @brief The pointer to the actual function code this class wraps.
     */
    Function                function;

}; /* class RuntimeEvaluatorFunction */

/**
 * @brief   Max number of functions that can be registered.
 * @details RuntimeEvaluator uses a set of functions to evaluate
 *          mathematical expression in real-time. These functions are
 *          stored in the functionRecords array which is of
 *          maxFunctions length.
 */
/*lint -esym(551,MARTe::maxFunctions) the symbol is access in the cpp file*/
static const uint32 maxFunctions = 16384u;

/**
 * @brief Number of currently available functions.
 */
extern uint32 availableFunctions;

/**
 * @brief   Database of all registered functions to be used by RuntimeEvaluator.
 * @details This array holds all RuntimeEvaluatorFunction objects
 *          used by RuntimeEvaluator.
 *          To add a function to this database the RegisterFunction()
 *          method shall be used.
 */
extern RuntimeEvaluatorFunction functionRecords[maxFunctions];

/**
 * @brief Finds a PCode elements and updates the typestack accordingly.
 */
bool FindPCodeAndUpdateTypeStack(CodeMemoryElement &code, const CCString &nameIn, StaticStack<TypeDescriptor,32u> &typeStack, const bool matchOutput,DataMemoryAddress &dataStackSize);

/**
 * @brief   Adds a function to #functionRecord.
 * @details This function is used to add a RuntimeEvaluatorFunction
 *          function to the database of functions (#functionRecord)
 *          that can be called at runtime by RuntimeEvaluator.
 */
void RegisterFunction(const RuntimeEvaluatorFunction &record);

/**
 * @brief   Generates boilerplate code to register a function.
 * @details The macro performs the following actions:
 *          1. creates a TypeDescriptor array of types (passed to the 
 *             macro via the variable size input ... and catched by the
 *             __VA_ARGS__ keyword)
 *          2. creates a RuntimeEvaluatorFunction function that points
 *             to the function passed as 5th input to the macro
 *          3. creates a class that wraps the function and provides it
 *             with a constructor who is responsible for calling the
 *             RegisterFunction method for this function
 *          4. creates an instance of this class
 *          
 *          Each time this macro is called a class is defined and 
 *          instantiated, thus resulting in the functionRecords array
 *          to be fed with the corresponding function via the
 *          RegisterFunction method.
 */
/*lint -emacro( {446}, REGISTER_PCODE_FUNCTION ) Type2TypeDescriptor<>() has no side-effects and can thus be used safely in  ..._FunctionTypes[] array initialization.*/
/*lint --emacro( {1502}, REGISTER_PCODE_FUNCTION ) Justification: name ## subName ## RegisterClass class intentionally has no data member. */
#define REGISTER_PCODE_FUNCTION(name,subName,nInputs,nOutputs,functionIn,...)\
    static TypeDescriptor name ## subName ## _FunctionTypes[] = {__VA_ARGS__}; \
    static const RuntimeEvaluatorFunction name ## subName ## _RuntimeEvaluatorFunction(#name,nInputs,nOutputs, &name ## subName ## _FunctionTypes[0u], static_cast<Function>(&functionIn)); \
    static class name ## subName ## RegisterClass { \
    public: name ## subName ## RegisterClass(){\
            RegisterFunction(name ## subName ## _RuntimeEvaluatorFunction);\
        }\
    } name ## subName ## RegisterClassInstance;


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* RUNTIMEEVALUATORFUNCTION_H_ */
