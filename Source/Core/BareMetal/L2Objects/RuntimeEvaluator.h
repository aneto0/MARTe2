/**
 * @file RuntimeEvaluatorEngine.h
 * @brief Header file for class RuntimeEvaluatorEngine
 * @date Mar 23, 2020
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

#include "CompilerTypes.h"
#include "StaticList.h"
#include "Vector.h"
#include "List.h"
#include "Queue.h"
#include "CCString.h"
#include "CompositeErrorManagement.h"
#include "Reference.h"
#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


namespace MARTe{

/**
 * used internally to describe and register functions
 */
struct RuntimeEvaluatorFunction;

/**
 * @brief Runtime mathematical expression evaluation engine.
 *
 * @details Engine to evaluate mathematical expressions at runtime.
 *
 * Summary
 * =======
 *
 * Runtime evaluator takes an expression in stack machine form at
 * construction time and is then capable of compiling and executing it.
 * The expression in stack machine form can be used straight away or
 * be derived from an infix expression (a mathematical expression
 * in the usual form) by using MARTe::MathExpressionParser.
 *
 * Usage
 * =====
 *
 * To use the evaluator, the following steps must be followed:
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
 *
 * StreamString rpnCode = "READ A\n"
 *                        "READ B\n"
 *                        "ADD\n"
 *                        "WRITE ret\n"
 * ;
 *
 * RuntimeEvaluator expression(rpnCode);
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * or by converting an infix expression via the MathExpressionParser:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 *
 * StreamString infixExpr = "ret = A + B;"
 *
 * MathExpressionParser parser(infixExpr);
 * parser.Parse();
 *
 * RuntimeEvaluator expression(parser.GetStackMachineExpression());
 *
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
 * Types *must* be set by using the SetInputVariableType and
 * SetOutputVaribleType methods:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * ret = expression.SetInputVariableType("theta", Float64Bit);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Locations *can* be set by using the SetInputVariableMemory and
 * SetOutputVariableMemory methods.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * float64 y;
 * ret = expression.SetOutputVariableMemory("y", &y);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * - In case and external location for a variable is set, the variable
 *   is considered external and any modification of its value will be
 *   reflected to the specified memory address.
 * - In case an external location for a variable is not set, the variable
 *   is considered internal (this is the default behavior.
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
 * Variable values can be updated mutiple times, and each time the
 * Execute() method will recalculate the output variable values.
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
 *
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
 * RuntimeEvaluatorFunctions objects.
 *
 * When RuntimeEvaluator executes an operation, it actually calls the
 * corresponding function in #functionRecords, or better calls the
 * RuntimeEvaluatorFunctions::ExecuteFunction() method of that function
 * and passes itself to the method as the argument.
 * The operation is then executed by RuntimeEvaluatorFunctions, which is
 * responsible for managing RuntimeEvaluator internal stack by using
 * Pop(), Push() and Peek() methods. See RuntimeEvaluatorFunctions
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
 * | `NE`             | Not equal operation between top two elements of the stack              |
 * | `ADD`            | Sum between top two elements of the stack                              |
 * | `SUB`            | Subtraction between top two elements of the stack                      |
 * | `MUL`            | Multiplication between top two elements of the stack                   |
 * | `DIV`            | Division between top two elements of the stack                         |
 * | `SIN`            | Sine operation on the top of the stack                                 |
 * | `COS`            | Cosine operation on the top of the stack                               |
 * | `POW`            | Power operation between top two elements of the stack                  |
 *
 * Adding new functions
 * --------------------
 *
 * New operations can be made available to RuntimeEvaluator by
 * adding functions to the #functionRecords as follows:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 *
 * void NewAddition(RuntimeEvaluator &evaluator) {
 *     float32 x1,x2,x3;
 *     evaluator.Pop(x1);
 *     evaluator.Pop(x2);
 *     x3 = x2 + x1;
 *     evaluator.Push(x3);
 * }
 *
 * TypeDescriptor types[] = {Float32Bit, Float32Bit, Float32Bit};
 * RuntimeEvaluatorFunctions newAddition("NEWADD", 2, 1, types, NewAddition);
 * RegisterFunction(newAddition);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * The function above pops two float32 element from the stack,
 * sum them and then pushes the result to the stack. Upon `RegisterFunction`ing
 * the function, the function itself becomes available to the RuntimeEvaluator
 * by using the command `NEWADD`.
 *
 */
class RuntimeEvaluator {

public:
    /**
     *  Forward declaration of the record used to store variable information obtained from the compilation
     */
    class VariableInformation;

    /**
     * constructor
     */
    RuntimeEvaluator();

    /**
     * destructor
     */
    ~RuntimeEvaluator();

    /**
     * @brief   Fills the variable database.
     * @details Cleans inputVariableInfo and outputVariableInfo, then
     *          scans RPNCode looking for READ, WRITE and CONST functions.
     *          Once this method has been called, variable management
     *          APIs are available.
     */
    ErrorManagement::ErrorType 						ExtractVariables(CCString RPNCode);

    VariableInformation *                           SelectVariable(uint32 index,bool input);
    VariableInformation *                           SelectVariable(CCString name,bool input);
    ErrorManagement::ErrorType                      ReadVariable(VariableInformation *vi,AnyType x);
    ErrorManagement::ErrorType                      WriteVariable(VariableInformation *vi,AnyType x);


    /**
     * Looks for a variable at a given location
     */
    ErrorManagement::ErrorType 						BrowseInputVariable(uint32 index,VariableInformation *&variableInformation);

    /**
     * Looks for a variable at a given location
     */
    ErrorManagement::ErrorType 						BrowseOutputVariable(uint32 index,VariableInformation *&variableInformation);

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
     *            + writes constants into variablesMemory
     *            + checks type consistency
     *            +  grow stack to required size
     *
     * @pre     ExtractVariables() == true && all variable types must
     *          be set.
     */
    ErrorManagement::ErrorType 						Compile(CCString RPNCode);
    
    /**
     * allow choosing how to run the code
     */
    enum executionMode {
        /**
         * fastMode: executes with minimal checks - assumes compilation was correct and function description was truthful
         */
        fastMode,

        /**
         * safeMode: checks stack,errors, and code pointer at every step
         */
        safeMode,

        /**
         * debugMode: produces a step by step evolution of the stack following each function execution
         */
        debugMode,

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
    ErrorManagement::ErrorType 						Execute(executionMode mode = fastMode, StreamI *debugStream=NULL_PTR(StreamI *));

    /**
     * Reconstruct the RPNCode with type information
     */
    ErrorManagement::ErrorType 						DeCompile(DynamicCString &DeCompileRPNCode, bool showTypes);
    
    /**
     *  TODO
     *  size of constants area
     */
    inline uint32 									GetSizeOfVariables();

    /**
     *  TODO
     *  size of code
     */
    inline uint32 									GetSizeOfCode();

    /**
     *  TODO
     *  size of stack
     */
    inline uint32 									GetSizeOfStack();


    /**
     * @name    Members required by RuntimeEvaluatorFunctions
     * @details These methods allow RuntimeEvaluatorFunctions to manage
     *          RuntimeEvaluator internal stack so that they can execute
     *          operations on the stack itself when required by the code.
     * @note    These members are public since RuntimeEvaluatorFunctions
     *          need to access them.
     */
    //@{

        /**
         * type used to store 1 PseudoCode element
         */
        typedef uint16 CodeMemoryElement;

        /**
         * type used to address 1 PseudoCode element
         */
        typedef uint16 CodeMemoryAddress;

        /**
         * type used to store 1 Data element in the stack and data area
         */
        typedef uint32 DataMemoryElement;

        /**
         * type used to address 1 Data element MUST be the same as a CodeMemoryElement
         */
        typedef CodeMemoryElement DataMemoryAddress;

        /**
         *
         */
        static const DataMemoryAddress                  InvalidDataMemoryAddress;

        /**
         * used to mark an address to be invalid
         */
        static const CodeMemoryElement                  InvalidCodeMemoryElement;

        /**
         * converts bytes to number of Data Memory elements
         */
        static inline DataMemoryAddress                 ByteSizeToDataMemorySize(uint32 byteSize);

    	/**
    	 * Reads from code memory
    	 */
    	inline CodeMemoryElement                        GetPseudoCode();

    	/**
    	 * Reads from Data Memory
    	 */
    	template<typename T>T &     					Variable(DataMemoryAddress variableIndex);

        /**
        * @brief      Get the value on the top of the stack and then move the pointer.
        * @param[in] value reference to the variable that will hold the value
        *                  popped from the top of the stack
        * @details    writes the value on the top of the stack to `value`
        *             and then update stack pointer (note that the stack
        *             will have a specific granularity).
        */
        template<typename T>
        inline void 									Pop(T &value);

        /**
        * @brief     Push a value to the top of the stack and then move the pointer.
        * @param[in] value reference to the variable that holds the value
        *                  to be pushed on the top of the stack
        * @details   writes the value of `value` on the top of the stack
        *            and then update stack pointer (note that the stack
        *            will have a specific granularity).
        */
        template<typename T>
        inline void 									Push(T &value);

        /**
        * @brief      Get the value on the top of the stack and do not move the pointer.
        * @param[in] value reference to the variable that will hold the value
        *                  on the top of the stack
        * @details    writes the value on the top of the stack to `value`
        *             and do not update stack pointer (note that the stack
        *             will have a specific granularity).
        */
        template<typename T>
        inline void 									Peek(T &value);

        /**
         * @brief The errors produced by the functions and the checks during runtime.
         */
        ErrorManagement::ErrorType 						runtimeError;

    //@}



private:

    /**
     * parses a line and extracts the command name and the parameter
     */
    ErrorManagement::ErrorType          ParseLine(CCString line,DynamicCString &command, Queue<Reference> parameters);

    /**
     * Checks existence of name using FindInputVariable
     * If not found add new variable
     */
    inline ErrorManagement::ErrorType 	AddInputVariable(CCString name,VariableDescriptor vd = VoidType);

    /**
     * Looks for a variable of a given name
     */
    inline ErrorManagement::ErrorType	FindInputVariable(CCString name,VariableInformation *&variableInformation);

    /**
     * Checks existence of name using FindOutputVariable
     * If not found add new variable
     */
    inline ErrorManagement::ErrorType 	AddOutputVariable(CCString name,VariableDescriptor vd = VoidType);

    /**
     * Looks for a variable of a given name
     */
    inline ErrorManagement::ErrorType 	FindOutputVariable(CCString name,VariableInformation *&variableInformation);

    /**
     * Looks for a variable of a given name
     */
    ErrorManagement::ErrorType 			FindVariable(DataMemoryAddress address,VariableInformation *&variableInformation);

    /**
     * implements AddOutputVariable and AddInputVariable
     */
    ErrorManagement::ErrorType 			AddVariable2DB(CCString name,List<VariableInformation> &db,VariableDescriptor vd/*,DataMemoryAddress location*/);

    /**
     * implements FindOutputVariable
     */
    ErrorManagement::ErrorType 			FindVariableinDB(CCString name,VariableInformation *&variableInformation,List<VariableInformation> &db);

    /**
     * expands function information input description into readable text
     * if more pCode is required for the decoding it will peek it from context. It will consume the PCode only if peekOnly=false
     * it will access DataMemory as well to decode constants
     * it will access Stack as well to decode input variables -- assumes that the stack is in the state before calling the function
     */
    ErrorManagement::ErrorType 			FunctionRecordInputs2String(RuntimeEvaluatorFunction &functionInformation,CStringTool &cst,bool peekOnly=true,bool showData=true,bool showTypes=true);

    /**
     * expands function information output description into readable text
     * if more pCode is required for the decoding it will peek it from context. It will re-read the last pCode if lookBack is true otherwise it will consume next
     * it will access DataMemory as well to decode constants
     * it will access Stack as well to decode output variables -- assumes that the stack has just been updated by the function
     */
    ErrorManagement::ErrorType 			FunctionRecordOutputs2String(RuntimeEvaluatorFunction &functionInformation,CStringTool &cst,bool lookBack=true,bool showData=true,bool showTypes=true);

    /**
     * @name    Symbol table generated in the various compilation stages and user input stages
     * @details This information allows interpreting data in variablesMemory
     */
    //@{

    /**
     * the input variable names
     */
    List<VariableInformation>         inputVariableInfo;

    /***
     * the output variable names
     */
    List<VariableInformation>         outputVariableInfo;

    /**
     * @name    Members reinitialized every Execute()
     * @details These variables allow rapid access to stack,variableMemory and codeMemory.
     */
    //@{

    /**
     * used by Push/Pop/Peek
     */
    DataMemoryElement *               stackPtr;

    /**
     * used by GetPseudoCode()
     */
    const CodeMemoryElement *         codeMemoryPtr;

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
    DataMemoryElement *               variablesMemoryPtr;

    //@}

    /**
     * @name    Members generated by Compile()
     * @details this is the code execution context.
     */
    //@{

    /**
     * code instructions  are allocated here
     */
    StaticList<CodeMemoryElement,32>  codeMemory;

    /**
     * number of DataMemoryElement
     */
    uint32                            sizeOfVariablesMemory;

    /**
     * stack is allocated here
     */
    Vector<DataMemoryElement>         stack;

    //@}

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

uint32 RuntimeEvaluator::GetSizeOfVariables(){
    return sizeOfVariablesMemory;
}

uint32 RuntimeEvaluator::GetSizeOfCode(){
	return codeMemory.GetSize();
}

uint32 RuntimeEvaluator::GetSizeOfStack(){
	return stack.GetNumberOfElements();
}

template<typename T>
void RuntimeEvaluator::Pop(T &value){
    if (stackPtr){
        // adds granularity-1 so that also 1 byte uses 1 slot
        // stack points to the next free value. so one need to step back of the variable size
        stackPtr -= ByteSizeToDataMemorySize(sizeof(T));
        value = *((T *)stackPtr);
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
}


template<typename T>
void RuntimeEvaluator::Push(T &value){
    if (stackPtr ){
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

RuntimeEvaluator::CodeMemoryElement RuntimeEvaluator::GetPseudoCode(){
    return *codeMemoryPtr++;
}

ErrorManagement::ErrorType RuntimeEvaluator::AddInputVariable(CCString name,VariableDescriptor vd){
    return AddVariable2DB(name,inputVariableInfo,vd);
}

ErrorManagement::ErrorType RuntimeEvaluator::FindInputVariable(CCString name,VariableInformation *&variableInformation){
    return FindVariableinDB(name,variableInformation,inputVariableInfo);
}

ErrorManagement::ErrorType RuntimeEvaluator::AddOutputVariable(CCString name,VariableDescriptor vd){
    return AddVariable2DB(name,outputVariableInfo,vd);
}

ErrorManagement::ErrorType RuntimeEvaluator::FindOutputVariable(CCString name,VariableInformation *&variableInformation){
    return FindVariableinDB(name,variableInformation,outputVariableInfo);
}

RuntimeEvaluator::DataMemoryAddress RuntimeEvaluator::ByteSizeToDataMemorySize(uint32 byteSize) {
    return static_cast<DataMemoryAddress>((byteSize + sizeof(DataMemoryElement) - 1U)/sizeof(DataMemoryElement));
}


} // MARTe

#endif /* RUNTIMEEVALUATOR_H_ */
