/**
 * @file RuntimeEvaluator.h
 * @brief Header file for class RuntimeEvaluator
 * @date 23/03/2020
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

#ifndef RUNTIMEEVALUATOR_H_
#define RUNTIMEEVALUATOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "AnyType.h"
#include "CompilerTypes.h"
#include "IteratorT.h"
#include "LinkedListHolderT.h"
#include "StaticList.h"
#include "StaticStack.h"
#include "TypeConversion.h"
#include "TypeDescriptor.h"
#include "VariableInformation.h"
#include "Vector.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

// Forward declaration required (RuntimeEvaluator and RuntimeEvaluatorFunction are circular-dependant).
class RuntimeEvaluatorFunction;

/**
 * @brief Runtime mathematical expression evaluation engine.
 * 
 * @details Engine to evaluate mathematical expressions at runtime.
 * 
 * Summary
 * =======
 * 
 * RuntimeEvaluator takes an expression in stack machine form at
 * construction time and is then capable of compiling and executing it.
 * The expression in stack machine form can be used straight away or
 * be derived from an infix expression (a mathematical expression
 * in the usual form) by using MARTe::MathExpressionParser.
 * 
 * RuntimeEvaluator supports scalar operands of any type.
 * 
 * Usage
 * =====
 * 
 * The steps required to use the evaluator are the following:
 * - the evaluator is instantiated and fed with the expression
 *   it will be required to evaluate
 * - the evaluator internal variable database is initialised by
 *   calling the ExtractVariable method
 * - variable properties are set by using the APIs
 * - the expression is compiled by calling the Compile() method
 * - the expression is executed by calling the Execute() method
 * 
 * Instantiating the evaluator
 * ---------------------------
 * 
 * The expression must first be fed to the RuntimeEvaluator at
 * construction time, either typed directly:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * StreamString rpnCode = "READ A\n"
 *                        "READ B\n"
 *                        "ADD\n"
 *                        "WRITE ret\n"
 * ;
 * 
 * RuntimeEvaluator expression(rpnCode);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * or by converting an infix expression (e.g. via the MathExpressionParser):
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * StreamString infixExpr = "ret = A + B;"
 * 
 * MathExpressionParser parser(infixExpr);
 * parser.Parse();
 * 
 * RuntimeEvaluator expression(parser.GetStackMachineExpression());
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Initialising the evaluator
 * --------------------------
 * 
 * First of all, RuntimeEvaluator must know what variables are contained
 * in the expression. This is done by calling the ExtractVariables()
 * method. After that, variable properties can be set by using
 * RuntimeEvaluator variable managing APIs.
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * ret = expression.ExtractVariables();
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Setting up variables
 * --------------------
 * 
 * Variable properties must then be set. Variables properties are:
 * - type (can be one of MARTe2 supported types: `Unsigned32Bit`, `Float64Bit` etc.)
 * - location (the memory location where the variable value will be held)
 * 
 * Types *must* be set for every input variable by using the
 * SetInputVariableType and SetOutputVaribleType methods:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * ret = expression.SetInputVariableType("theta", Float64Bit);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Output variable types *can* be set with SetOutputVariableType if needed.
 * However, this is not compulsory as RuntimeEvaluator will assign them
 * the output type of last operation.
 * 
 * Memory locations *can* be set by using the SetInputVariableMemory and
 * SetOutputVariableMemory methods.
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * float64 y;
 * ret = expression.SetOutputVariableMemory("y", &y);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * - In case an external location for a variable is set, the variable
 *   is considered *external* and any modification of its value will
 *   happen to the specified memory address.
 * - In case an external location for a variable is not set, the variable
 *   is considered *internal* (this is the default behavior).
 *   RuntimeEvaluator will be responsible of allocating space for all
 *   internal variables. The memory location of internal variables will
 *   be available after compilation by calling GetInputVariableMemory and
 *   GetOutputVariableMemory methods.
 * 
 * Compiling
 * ---------
 * 
 * After setting up the desired properties for variables, the expression
 * can be compiled:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * ret = expression.Compile();
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * From now on, variable values can be set and the expression can be
 * executed.
 * 
 * Executing
 * ---------
 * 
 * Before executing, values of each variable can be updated. External
 * variable values are updated simply by updating the memory they have
 * been set to follow:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * float64 theta;
 * ret = expression.SetInputVariableMemory("theta", &theta);
 * ret = expression.Compile();
 * theta = 10.0;
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Internal variables can be modified by retrieving a pointer to them:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * ret = expression.SetInputVariableType("theta", Float64Bit);
 * 
 * float64* ptr;
 * ptr = (float64*)expression.GetInputVariableMemory("theta");
 * 
 * *ptr = 10.0;
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Each time the expression is executed, all output variables
 * are updated. If they have been set external, their values are
 * directly available:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * float64 y;
 * expression.SetOutputVariableMemory("y", &y);
 * ...
 * expression.Compile();
 * ...
 * expression.Execute();
 * REPORT_ERROR(ErrorManagement::Information, "Value of y is: %f", y);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * if they are internal, GetOutputVariableMemory shall be used to
 * retrieve their addresses and obtain the final value.
 * 
 * Variables whose value can be updated in the middle of the expression:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * StreamString rpnCode = "READ A\n"      // first
 *                        "READ B\n"
 *                        "ADD\n"
 *                        "WRITE A\n"     // output variable A is set
 *                        "READ A\n"      // second
 *                        "WRITE ret\n";
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * In the example above, the former `READ A` command reads from the
 * input variable A, while the latter reads from the output variable A
 * that was just set by the `WRITE A` command.
 * 
 * Variable values can be updated any time, and when invoked the
 * Execute() method will recalculate the value of output variable
 * based on the current value of input variables.
 * 
 * Examples
 * ========
 * 
 * Example usage with the following expression:
 * `y = pow(sin(theta), 2) + pow(cos(theta), 2)`
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * #include "RuntimeEvaluator.h"
 * 
 * bool ret;
 * 
 * StreamString rpnCode = "READ theta\n"
 *                        "SIN\n"
 *                        "CONST int64 2\n"
 *                        "POW\n"
 *                        "READ theta\n"
 *                        "COS\n"
 *                        "CONST int64 2\n"
 *                        "POW\n"
 *                        "ADD\n"
 *                        "WRITE y\n"
 * ;
 * 
 * RuntimeEvaluator expression(rpnCode);
 * 
 * ret = expression.ExtractVariables();
 * 
 * float64 theta;
 * float64 y;
 * 
 * if (ret) {
 *     ret &= expression.SetInputVariableType("theta", Float64Bit);
 *     ret &= expression.SetInputVariableMemory("theta", &theta);
 * }
 * 
 * if (ret) {
 *     ret &= expression.SetOutputVariableType("y", Float64Bit);
 *     ret &= expression.SetOutputVariableMemory("y", &y);
 * }
 * 
 * if (ret) {
 *     ret = expression.Compile();
 * }
 * 
 * // now variable values can be set
 * theta = 3.14;
 * 
 * if (ret) {
 *     ret = expression.Execute();
 * }
 * 
 * // result is now available
 * if (y == 1.0) {
 *     REPORT_ERROR(ErrorManagement::Information, "OK!");
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Further details
 * ===============
 * 
 * RuntimeEvaluator is a stack machine that reads instructions from
 * a stack machine expression and executes them in a last in-first out
 * fashion. All operations rely on an internal stack: operands are
 * retrieved from the stack and results are placed in the stack.
 * 
 * Working principle
 * -----------------
 * 
 * RuntimeEvaluator scans the input stack machine code and the variable
 * types. Combination of code and types during Compile() produces
 * a list of calls to functions with specific types (the "pseudocode")
 * that will be executed during Execute().
 * Functions that will be called must be present in the #functionRecords
 * array, an array that holds all the available functions that
 * RuntimeEvaluator can call. #functionRecords is an array of
 * RuntimeEvaluatorFunction objects.
 * 
 * When RuntimeEvaluator executes an operation, it actually calls the
 * corresponding function in #functionRecords, or better calls the
 * RuntimeEvaluatorFunction::ExecuteFunction() method of that function
 * and passes itself to the method as the argument.
 * The operation is then executed by RuntimeEvaluatorFunction, which is
 * responsible for managing RuntimeEvaluator internal stack by using
 * Pop(), Push() and Peek() methods. See RuntimeEvaluatorFunction
 * documentation for further details.
 * 
 * Supported operators
 * -------------------
 * 
 * This is a table of all supported operators:
 * 
 * | Operator         | Meaning                                                                |
 * | :--------------- | :--------------------------------------------------------------------- |
 * | `READ  var`      | Pushes the value of variable `var` from memory to the top of the stack |
 * | `WRITE var`      | Pops the top of the stack and writes its value to variable `var`       |
 * | `CONST type val` | Push a constant of value `val` and type `type` to the top of the stack |
 * | `CAST type`      | Casts the top of the stack to type `type`                              |
 * | `AND`            | AND operation between top two elements of the stack                    |
 * | `OR`             | OR operation between top two elements of the stack                     |
 * | `XOR`            | XOR operation between top two elements of the stack                    |
 * | `GT`             | Greater than operation between top two elements of the stack           |
 * | `LT`             | Less than operation between top two elements of the stack              |
 * | `GTE`            | Greater or equal operation between top two elements of the stack       |
 * | `LTE`            | Less or equal operation between top two elements of the stack          |
 * | `EQ`             | Equal operation between top two elements of the stack                  |
 * | `NEQ`            | Not equal operation between top two elements of the stack              |
 * | `ADD`            | Sum between top two elements of the stack                              |
 * | `SUB`            | Subtraction between top two elements of the stack                      |
 * | `MUL`            | Multiplication between top two elements of the stack                   |
 * | `DIV`            | Division between top two elements of the stack                         |
 * | `SIN`            | Sine operation on the top of the stack                                 |
 * | `COS`            | Cosine operation on the top of the stack                               |
 * | `TAN`            | Tangent operation on the top of the stack                              |
 * | `EXP`            | Returns `e` to the power of the argument                               |
 * | `LOG`            | Log operation on the top of the stack                                  |
 * | `LOG10`          | Log10 operation on the top of the stack                                |
 * 
 * @note Comparison of floating-point types may often be implementation-dependent.
 *       The `EQ` and `NEQ` operators use the SafeMath::IsEqual()
 *       instead of the standard `==` operator in order to be safely
 *       portable: this implementation of the comparison operation takes
 *       into account the floating-point operand granularity
 *       (machine epsilon) to achieve a safely portable equality check.
 * 
 * Adding new functions
 * --------------------
 * 
 * New operations can be made available to RuntimeEvaluator by
 * adding a RuntimeEvaluatorFunction to the #functionRecords as follows:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * void NewOperation(RuntimeEvaluator &evaluator) {
 *     float32 x1,x2,x3;
 *     evaluator.Pop(x1);
 *     evaluator.Pop(x2);
 *     x3 = x2 + x1;
 *     evaluator.Push(x3);
 * }
 * 
 * TypeDescriptor types[] = {Float32Bit, Float32Bit, Float32Bit};
 * RuntimeEvaluatorFunction newAdd("NEWADD", 2, 1, types, NewOperation);
 * RegisterFunction(newAdd);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * The function above pops two float32 element from the stack,
 * sums them and then pushes the result to the stack. Upon `RegisterFunction`ing
 * the function, the function itself becomes available to the RuntimeEvaluator
 * by using the command `NEWADD`.
 * 
 */
class RuntimeEvaluator {

public:
    /**
     * @brief     Default constructor.
     * @param[in] RPNCode the expression to be evaluated in stack machine form.
     */
    RuntimeEvaluator(StreamString RPNCodeIn);

    /**
     * @brief Destructor.
     */
    ~RuntimeEvaluator();

    /**
     * @brief Reads from code memory
     */
    inline CodeMemoryElement    GetPseudoCode();

    /**
     * @brief Reads from Data Memory
     */
    template<typename T>T &     Variable(DataMemoryAddress variableIndex);

    /**
     * @brief   Fills the variable database.
     * @details Cleans inputVariableInfo and outputVariableInfo, then
     *          scans RPNCode looking for READ, WRITE and CONST functions.
     *          Once this method has been called, variable management
     *          APIs are available.
     * @returns ErrorManagement::NoError on success.
     */
    ErrorManagement::ErrorType ExtractVariables();

    /**
     * @brief      Returns a pointer to a RuntimeEvaluator variable.
     * @param[in]  index the index of the variable to be retrieved.
     * @param[out] variableInformation a pointer to the variable.
     * @returns    ErrorManagement::NoError on success.
     *             ErrorManagement::OutOfRange otherwise.
     */
    ErrorManagement::ErrorType BrowseInputVariable(uint32 index,VariableInformation *&variableInformation);

    /**
     * @brief Returns a pointer to a RuntimeEvaluator variable.
     * @param[in]  index the index of the variable to be retrieved.
     * @param[out] variableInformation a pointer to the variable.
     * @returns    ErrorManagement::NoError on success,
     *             ErrorManagement::OutOfRange otherwise.
     */
    ErrorManagement::ErrorType BrowseOutputVariable(uint32 index,VariableInformation *&variableInformation);
    
    /**
     * @brief     Searches the address of an input variable in memory
     *            by variable name.
     * @returns   the address of the variable in memory. `NULL_PTR(void*)`
     *            if variable is not found.
     * @param[in] varNameIn the name of the variable to retrieve.
     * @warning   the return address is `void*` and needs to be casted
     *            to the proper type before using it.
     * @pre       RuntimeEvaluator::ExtractVariables() == true
     */
    void* GetInputVariableMemory(const StreamString &varNameIn);
    
    /**
     * @brief     Searches the address of an input variable in memory
     *            by variable index.
     * @returns   the address of the variable in memory. `NULL_PTR(void*)`
     *            if variable is not found.
     * @param[in] varIndexIn the index of the variable to retrieve.
     *            The index is determined by the order in which
     *            variables appear in the input RPNCode.
     * @warning   the return address is `void*` and needs to be casted
     *            to the proper type before using it.
     * @pre       RuntimeEvaluator::ExtractVariables() == true
     */
    void* GetInputVariableMemory(const uint32 &varIndexIn);
    
    /**
     * @brief     Retrieves the address of an output variable in memory
     *            by variable name.
     * @returns   the address of the variable in memory. `NULL_PTR(void*)`
     *            if variable is not found.
     * @param[in] varNameIn the name of the variable to retrieve.
     * @warning   the return address is `void*` and needs to be casted
     *            to the proper type before using it.
     * @pre       RuntimeEvaluator::ExtractVariables() == true
     */
    void* GetOutputVariableMemory(const StreamString &varNameIn);
    
    /**
     * @brief     Retrieves the address of an output variable in memory
     *            by variable index.
     * @returns   the address of the variable in memory. `NULL_PTR(void*)`
     *            if variable is not found.
     * @param[in] varIndexIn the index of the variable to retrieve.
     *            The index is determined by the order in which
     *            variables appear in the input RPNCode.
     * @warning   the return address is `void*` and needs to be casted
     *            to the proper type before usage.
     * @pre       RuntimeEvaluator::ExtractVariables() == true
     */
    void* GetOutputVariableMemory(const uint32 &varIndexIn);
    
    /**
     * @brief     Set the address of a variable to an external location.
     * @returns   true on success.
     * @param[in] varNameIn          the name of the variable to retrieve.
     * @param[in] externalLocationIn the address that will hold the memory.
     * @pre       RuntimeEvaluator::ExtractVariables() == true
     */
    bool SetInputVariableMemory(const StreamString &varNameIn, void* const externalLocationIn);
    
    /**
     * @brief     Set the address of a variable to an external location.
     * @returns   true on success.
     * @param[in] varIndexIn         the index of the variable to retrieve.
     * @param[in] externalLocationIn the address that will hold the memory.
     * @pre       RuntimeEvaluator::ExtractVariables() == true
     */
    bool SetInputVariableMemory(const uint32 &varIndexIn, void* const externalLocationIn);
    
    /**
     * @brief     Set the address of a variable to an external location.
     * @returns   true on success.
     * @param[in] varNameIn          the name of the variable to retrieve.
     * @param[in] externalLocationIn the address that will hold the memory.
     * @pre       RuntimeEvaluator::ExtractVariables() == true
     */
    bool SetOutputVariableMemory(const StreamString &varNameIn, void* const externalLocationIn);
    
    /**
     * @brief     Set the address of a variable to an external location.
     * @returns   true on success.
     * @param[in] varIndexIn         the index of the variable to retrieve.
     * @param[in] externalLocationIn the address that will hold the memory.
     * @pre       RuntimeEvaluator::ExtractVariables() == true
     */
    bool SetOutputVariableMemory(const uint32 &varIndexIn, void* const externalLocationIn);
    
    /**
     * @brief     Set the type of an input variable.
     * @details   RuntimeEvaluator needs to know the types of all
     *            variables that are present in the input RPNCode to
     *            allocate the required portion of memory.
     * @returns   true on success.
     * @param[in] varNameIn the name of the variable to retrieve.
     * @param[in] typeIn    the type to set the variable to.
     * @pre       RuntimeEvaluator::ExtractVariables() == true
     */
    bool SetInputVariableType(const StreamString &varNameIn, const TypeDescriptor &typeIn);
    
    /**
     * @brief     Set the type of an input variable.
     * @details   RuntimeEvaluator needs to know the types of all
     *            variables that are present in the input RPNCode to
     *            allocate the required portion of memory.
     * @returns   true on success.
     * @param[in] varIndexIn the index of the variable to retrieve.
     *            The index is determined by the order in which
     *            variables appear in the input RPNCode.
     * @param[in] typeIn    the type to set the variable to.
     * @pre       RuntimeEvaluator::ExtractVariables() == true
     */
    bool SetInputVariableType(const uint32 &varIndexIn, const TypeDescriptor &typeIn);
    
    /**
     * @brief     Set the type of an output variable.
     * @details   RuntimeEvaluator needs to know the types of all
     *            variables that are present in the input RPNCode to
     *            allocate the required portion of memory.
     * @returns   true on success.
     * @param[in] varNameIn the name of the variable to retrieve.
     * @param[in] typeIn    the type to set the variable to.
     * @pre       RuntimeEvaluator::ExtractVariables() == true
     */
    bool SetOutputVariableType(const StreamString &varNameIn, const TypeDescriptor &typeIn);
    
    /**
     * @brief     Set the type of an output variable.
     * @details   RuntimeEvaluator needs to know the types of all
     *            variables that are present in the input RPNCode to
     *            allocate the required portion of memory.
     * @returns   true on success.
     * @param[in] varIndexIn the index of the variable to retrieve.
     *            The index is determined by the order in which
     *            variables appear in the input RPNCode.
     * @param[in] typeIn    the type to set the variable to.
     * @pre       RuntimeEvaluator::ExtractVariables() == true
     */
    bool SetOutputVariableType(const uint32 &varIndexIn, const TypeDescriptor &typeIn);
    
    /**
     * @brief   Compiles the expression and prepares it for execution.
     * @details Compile() carries out the following operations:
     *          - Cleans memory
     *          - Allocates inputVariables
     *          - Allocates outputVariables
     *          - Allocates constants
     *          - Allocates PCode space
     *          - Scans RPNCode
     *            + compiles into codeMemory
     *            + writes constants into dataMemory
     *            + checks type consistency
     *            +  grow stack to required size
     * 
     * @pre     ExtractVariables() == true && all variable types must
     *          be set.
     */
    ErrorManagement::ErrorType Compile();

    /**
     * @brief   Allows choosing how to run the code.
     * @details Used as input argument of Execute().
     */
    enum executionMode {
        /**
         * Executes with minimal checks - assumes compilation was correct and function description was truthful
         */
        fastMode,

        /**
         * Checks stack,errors, and code pointer at every step
         */
        safeMode,

        /**
         * Produces a step by step evolution of the stack following each function execution
         */
        debugMode
    };

    /**
     * @brief     Executes every command in codeMemory.
     * @returns   the combination of error flags reported by all the functions that were executed.
     * @param[in] mode execution mode
     * @param[in] debugStream only used in debugMode, after every command
     *            execution a report is written to the stream
     * @pre ExtractVariables() == true && Compile() == true && all
     *      variable types must be set.
     */
    ErrorManagement::ErrorType Execute(const executionMode mode = fastMode, StreamI* const debugStream=NULL_PTR(StreamI *));

    /**
     * @brief Reconstruct the RPNCode with type information
     */
    ErrorManagement::ErrorType DeCompile(StreamString &DeCompileRPNCode, const bool showTypes);
    
    /**
     * @name    Members required by RuntimeEvaluatorFunction
     * @details These methods allow RuntimeEvaluatorFunction to manage
     *          RuntimeEvaluator internal stack so that they can execute
     *          operations on the stack itself when required by the code.
     * @note    These members are public since RuntimeEvaluatorFunction
     *          need to access them.
     */
    //@{
        /**
        * @brief      Get the value on the top of the stack and then move the pointer.
        * @param[in] value reference to the variable that will hold the value
        *                  popped from the top of the stack
        * @details    writes the value on the top of the stack to `value`
        *             and then update stack pointer (note that the stack
        *             will have a specific granularity).
        */
        template<typename T>
        inline void Pop(T &value);

        /**
        * @brief     Push a value to the top of the stack and then move the pointer.
        * @param[in] value reference to the variable that holds the value
        *                  to be pushed on the top of the stack
        * @details   writes the value of `value` on the top of the stack
        *            and then update stack pointer (note that the stack
        *            will have a specific granularity).
        */
        template<typename T>
        inline void Push(T &value);

        /**
        * @brief      Get the value on the top of the stack and do not move the pointer.
        * @param[in] value reference to the variable that will hold the value
        *                  on the top of the stack
        * @details    writes the value on the top of the stack to `value`
        *             and do not update stack pointer (note that the stack
        *             will have a specific granularity).
        */
        template<typename T>
        inline void Peek(T &value);
        
        /**
         * @brief The errors produced by the functions and the checks during runtime.
         */
        ErrorManagement::ErrorType runtimeError;
        
        /**
         * @brief   Variable and constants are allocated here.
         * @details 
         * MEMORY MAP
         *
         * sizeOfVariablesArea     VARIABLES   --> variablesMemoryPtr   : pCodePtr
         *                            CONSTANTS
         *                            INPUTS
         *                            OUTPUTS
         */
        Vector<DataMemoryElement> dataMemory;
    //@}
    
private:

    /**
     * @brief Stack and variable are allocated here.
     */
    StaticList<CodeMemoryElement,32u>   codeMemory;
    
    /**
     * @brief Address of first variable (after constants)
     *        or how many MemoryElement are used for constants
     */
    DataMemoryAddress                  startOfVariables;
    
    /**
     * @brief Stack is allocated here.
     */
    Vector<DataMemoryElement>          stack;

    /**
     * @brief   Adds a new input variable.
     * @details Checks existence of name using FindInputVariable.
     *          If not found add new variable to inputVariableInfo.
     */
    inline ErrorManagement::ErrorType AddInputVariable(const CCString &name, const TypeDescriptor &td = VoidType, DataMemoryAddress location = MAXDataMemoryAddress);

    /**
     * @brief Looks for a variable of a given name
     */
    inline ErrorManagement::ErrorType FindInputVariable(const CCString &name, VariableInformation *&variableInformation);

    /**
     * @brief   Adds a new output variable.
     * @details Checks existence of name using FindInputVariable.
     *          If not found add new variable to outputVariableInfo.
     */
    inline ErrorManagement::ErrorType AddOutputVariable(const CCString &name, const TypeDescriptor &td = VoidType,DataMemoryAddress location = MAXDataMemoryAddress);

    /**
     * @brief Looks for a variable of a given name
     */
    inline ErrorManagement::ErrorType FindOutputVariable(const CCString &name, VariableInformation *&variableInformation);

    /**
     * @brief Looks for a variable of a given name
     */
    ErrorManagement::ErrorType FindVariable(DataMemoryAddress address,VariableInformation *&variableInformation);

    /**
     * @brief Implements AddOutputVariable and AddInputVariable
     */
    ErrorManagement::ErrorType AddVariable2DB(const CCString &name,LinkedListHolderT<VariableInformation> &db, const TypeDescriptor &td,DataMemoryAddress location) const;

    /**
     * @brief Implements FindOutputVariable
     */
    ErrorManagement::ErrorType FindVariableinDB(const CCString &name,VariableInformation *&variableInformation,LinkedListHolderT<VariableInformation> &db) const;

    /**
     * @brief expands function information input description into readable text
     * @details
     * if more pCode is required for the decoding it will peek it from context. It will consume the PCode only if peekOnly=false
     * it will access DataMemory as well to decode constants
     * it will access Stack as well to decode input variables -- assumes that the stack is in the state before calling the function
     */
    ErrorManagement::ErrorType FunctionRecordInputs2String(RuntimeEvaluatorFunction &functionInformation,StreamString &cst, const bool peekOnly=true, const bool showData=true, const bool showTypes=true);

    /**
     * @brief expands function information output description into readable text
     * @details
     * if more pCode is required for the decoding it will peek it from context. It will re-read the last pCode if lookBack is true otherwise it will consume next
     * it will access DataMemory as well to decode constants
     * it will access Stack as well to decode output variables -- assumes that the stack has just been updated by the function
     */
    ErrorManagement::ErrorType FunctionRecordOutputs2String(RuntimeEvaluatorFunction &functionInformation,StreamString &cst, const bool lookBack=true, const bool showData=true, const bool showTypes=true);

    /**
     * @brief   The list containing all input variables.
     * @details This list is filled by ExtractVariables().
     */
    LinkedListHolderT<VariableInformation>          inputVariableInfo;

    /**
     * @brief   The list containing all output variables.
     * @details This list is filled by ExtractVariables().
     */
    LinkedListHolderT<VariableInformation>          outputVariableInfo;

    /**
     * @brief   Pointer to the current active location in the stack.
     * @details Used by Push(), Pop() and Peek().
     */
    DataMemoryElement *                 stackPtr;

    /**
     * @brief   A pointer to the starting address of variable memory.
     * @details Used by Variable().
     */
    DataMemoryElement *                 variablesMemoryPtr;

    /**
     * @brief   A pointer to the starting address of code memory.
     * @details Used by GetPseudoCode().
     */
    const CodeMemoryElement *           codeMemoryPtr;
    
    /**
     * @brief   The code to be evaluated in stack machine form.
     * @details This code is bound to each instance of RuntimeEvaluator.
     */
    StreamString RPNCode;
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


static inline DataMemoryAddress ByteSizeToDataMemorySize(const uint16 byteSize){
    return static_cast<DataMemoryAddress>(((byteSize + sizeof(DataMemoryElement)) - 1U)/(sizeof(DataMemoryElement)));
}

template<typename T>
void RuntimeEvaluator::Pop(T &value){
    if (stackPtr){
        // adds granularity-1 so that also 1 byte uses 1 slot
        // stack points to the next free value. so one need to step back of the variable size
        stackPtr -= ByteSizeToDataMemorySize(sizeof(T));
        value = *((T *)stackPtr);
    }
    else {
        value = static_cast<T>(0u);
    }
}

template<typename T>
void RuntimeEvaluator::Peek(T &value){
    if (stackPtr){
        // adds granularity-1 so that also 1 byte uses 1 slot
        // stack points to the next free value. so one need to step back of the variable size
        DataMemoryElement *p =  stackPtr- ByteSizeToDataMemorySize(sizeof(T));
        value = *((T *)p);
    }
    else {
        value = static_cast<T>(0u);
    }
}


template<typename T>
void RuntimeEvaluator::Push(T &value){
    if (stackPtr){
        *((T *)stackPtr) = value;
        // adds granularity-1 so that also 1 byte uses 1 slot
        stackPtr += ByteSizeToDataMemorySize(sizeof(T));
    }
}

template<typename T>
T &RuntimeEvaluator::Variable(DataMemoryAddress variableIndex){
    // note that variableIndex is an address to the memory with a granularity of sizeof(MemoryElement)
    return (T&)variablesMemoryPtr[variableIndex];
}

CodeMemoryElement RuntimeEvaluator::GetPseudoCode() {
    const CodeMemoryElement* currentCodeMemoryPtr = codeMemoryPtr;
    codeMemoryPtr++;
    return *currentCodeMemoryPtr;
}

ErrorManagement::ErrorType RuntimeEvaluator::AddInputVariable(const CCString &name, const TypeDescriptor &td, const DataMemoryAddress location){
    return AddVariable2DB(name,inputVariableInfo,td,location);
}

ErrorManagement::ErrorType RuntimeEvaluator::FindInputVariable(const CCString &name, VariableInformation *&variableInformation){
    return FindVariableinDB(name,variableInformation,inputVariableInfo);
}

ErrorManagement::ErrorType RuntimeEvaluator::AddOutputVariable(const CCString &name, const TypeDescriptor &td, const DataMemoryAddress location){
    return AddVariable2DB(name,outputVariableInfo,td,location);
}

ErrorManagement::ErrorType RuntimeEvaluator::FindOutputVariable(const CCString &name, VariableInformation *&variableInformation){
    return FindVariableinDB(name,variableInformation,outputVariableInfo);
}

} // MARTe

#endif /* RUNTIMEEVALUATOR_H_ */
