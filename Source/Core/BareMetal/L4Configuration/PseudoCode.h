/**
 * @file PseudoCodeEngine.h
 * @brief Header file for class PseudoCodeEngine
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

#ifndef SOURCE_CORE_BAREMETAL_L2OBJECTS_PSEUDOCODE_H_
#define SOURCE_CORE_BAREMETAL_L2OBJECTS_PSEUDOCODE_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "StaticList.h"
#include "LinkedListHolderT.h"
#include "Vector.h"
#include "TypeDescriptor.h"
#include "PseudoCodeVariableInformation.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{


namespace PseudoCode{

/**
 * used internally to describe and register functions
 */
struct FunctionRecord;

/**
 *  The context required to execute a PCode. It is the result of a compilation of a RPN Code
 */
class Context{

public:
    /**
     * constructor
     */
    Context(StreamString &RPNCode);

    /**
     * destructor
     */
    ~Context();

    /**
    * @brief Get the top of the stack and then move the pointer.
    * @param[in] value reference to the variable and then update stack pointer (note that the stack will have a specific granularity).
    * @return .
   */
    template<typename T>
    inline void                 Pop(T &value);

    /**
    * @brief Add to the top of the stack and then move the pointer.
    * @param[in] value reference to the variable and then update stack pointer (note that the stack will have a specific granularity).
    * @return .
   */
    template<typename T>
    inline void                 Push(T &value);

    /**
    * @brief Get the top of the stack and do not move the pointer.
    * @param[in] value reference to the variable and then update stack pointer (note that the stack will have a specific granularity).
    * @return .
   */
    template<typename T>
    inline void                 Peek(T &value);

    /**
     * Reads from code memory
     */
    inline CodeMemoryElement    GetPseudoCode();

    /**
     * Reads from Data Memory
     */
    template<typename T>T &     Variable(DataMemoryAddress variableIndex);

    /**
     * Cleans inputVariableInfo
     * Cleans outputVariableInfo
     * Scans RPNCode looking for READ, WRITE and CONST functions
     */
    ErrorManagement::ErrorType ExtractVariables();

    /**
     * Looks for a variable at a given location
     */
    ErrorManagement::ErrorType BrowseInputVariable(uint32 index,VariableInformation *&variableInformation);

    /**
     * Looks for a variable at a given location
     */
    ErrorManagement::ErrorType BrowseOutputVariable(uint32 index,VariableInformation *&variableInformation);

    /**
     * Cleans memory
     * Allocates inputVariables
     * Allocates outputVariables
     * Allocates constants
     * Allocates PCode space
     * Scans RPNCode
     *    compiles into codeMemory
     *    writes constants into dataMemory
     *    checks type consistency
     *    grow stack to required size
     */
    ErrorManagement::ErrorType Compile();

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

        /**
         * allows single step execution
         * external step counter must be maintained
         */
        singleStep
    };

    /**
     * executes every command in codeMemory
     * note that the inputs need to be loaded before calling execute
     * returns the combination of error flags reported by all the functions that were executed
     * debugStream is only used in debugMode. after every command execution a report is written to the stream
     * step is only used in step mode. step value need to be initialised to 0 and maintained between calls
     */
    ErrorManagement::ErrorType Execute(executionMode mode = fastMode,StreamI *debugStream=NULL_PTR(StreamI *),CodeMemoryAddress *step=NULL);

    /**
     * Reconstruct the RPNCode with type information
     */
    ErrorManagement::ErrorType DeCompile(StreamString &DeCompileRPNCode, bool showTypes);
    
    /**
     * @brief     Searches the address of an input variable in memory
     *            by variable name.
     * @returns   the address of the variable in memory.
     * @param[in] varNameIn the name of the variable to retrieve.
     * @warning   the return address is `void*` and needs to be casted
     *            to the proper type before using it.
     */
    void* GetInputVariableMemory(StreamString &varNameIn);
    
    /**
     * @brief     Searches the address of an input variable in memory
     *            by variable index.
     * @returns   the address of the variable in memory.
     * @param[in] varIndexIn the index of the variable to retrieve.
     * @details   the index is determined by the order in which
     *            variables appear in the input RPNCode.
     * @warning   the return address is `void*` and needs to be casted
     *            to the proper type before using it.
     */
    void* GetInputVariableMemory(uint32 &varIndexIn);

// PUBLIC VARIABLES

    /**
     * the errors produced by the functions and the checks during runtime
     */
    ErrorManagement::ErrorType          runtimeError;

    /**
     * stack and variable are allocated here
     */
    StaticList<CodeMemoryElement,32>    codeMemory;

    /**
     * variable and constants are allocated here
     * MEMORY MAP
     *
     * sizeOfVariablesArea     VARIABLES   --> variablesMemoryPtr   : pCodePtr
     *                            CONSTANTS
     *                            INPUTS
     *                            OUTPUTS
     */
    Vector<DataMemoryElement>           dataMemory;

    /**
     * address of first variable (after constants) or how many MemoryElement are used for constants
     */
    DataMemoryAddress                   startOfVariables;

    /**
     * stack is allocated here
     */
    Vector<DataMemoryElement>           stack;


private:

    /**
     * Checks existence of name using FindInputVariable
     * If not found add new variable
     */
    inline ErrorManagement::ErrorType AddInputVariable(CCString name,TypeDescriptor td = InvalidType,DataMemoryAddress location = MAXDataMemoryAddress);

    /**
     * Looks for a variable of a given name
     */
    inline ErrorManagement::ErrorType FindInputVariable(CCString name,VariableInformation *&variableInformation);

    /**
     * Checks existence of name using FindOutputVariable
     * If not found add new variable
     */
    inline ErrorManagement::ErrorType AddOutputVariable(CCString name,TypeDescriptor td = InvalidType,DataMemoryAddress location = MAXDataMemoryAddress);

    /**
     * Looks for a variable of a given name
     */
    inline ErrorManagement::ErrorType FindOutputVariable(CCString name,VariableInformation *&variableInformation);

    /**
     * Looks for a variable of a given name
     */
    ErrorManagement::ErrorType FindVariable(DataMemoryAddress address,VariableInformation *&variableInformation);

    /**
     * implements AddOutputVariable and AddInputVariable
     */
    ErrorManagement::ErrorType AddVariable2DB(CCString name,LinkedListHolderT<VariableInformation> &db,TypeDescriptor td,DataMemoryAddress location);

    /**
     * implements FindOutputVariable
     */
    ErrorManagement::ErrorType FindVariableinDB(CCString name,VariableInformation *&variableInformation,LinkedListHolderT<VariableInformation> &db);

    /**
     * expands function information input description into readable text
     * if more pCode is required for the decoding it will peek it from context. It will consume the PCode only if peekOnly=false
     * it will access DataMemory as well to decode constants
     * it will access Stack as well to decode input variables -- assumes that the stack is in the state before calling the function
     */
    ErrorManagement::ErrorType FunctionRecordInputs2String(FunctionRecord &functionInformation,StreamString &cst,bool peekOnly=true,bool showData=true,bool showTypes=true);

    /**
     * expands function information output description into readable text
     * if more pCode is required for the decoding it will peek it from context. It will re-read the last pCode if lookBack is true otherwise it will consume next
     * it will access DataMemory as well to decode constants
     * it will access Stack as well to decode output variables -- assumes that the stack has just been updated by the function
     */
    ErrorManagement::ErrorType FunctionRecordOutputs2String(FunctionRecord &functionInformation,StreamString &cst,bool lookBack=true,bool showData=true,bool showTypes=true);

    /**
     * the input variable names
     */
    LinkedListHolderT<VariableInformation>          inputVariableInfo;

    /**
     * the output variable names
     */
    LinkedListHolderT<VariableInformation>          outputVariableInfo;

    /**
     * used by Push/Pop/Peek
     */
    DataMemoryElement *                 stackPtr;

    /**
     * used by Variable()
     */
    DataMemoryElement *                 variablesMemoryPtr;

    /**
     * used by GetPseudoCode()
     */
    const CodeMemoryElement *           codeMemoryPtr;
    
    /**
     * RPN code bounded to each instance.
     */
    StreamString RPNCode;
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


static inline DataMemoryAddress ByteSizeToDataMemorySize(uint32 byteSize){
    return static_cast<DataMemoryAddress>((byteSize + sizeof(DataMemoryElement) - 1U)/sizeof(DataMemoryElement));
//  return (static_cast<DataMemoryAddress>(byteSize)+static_cast<DataMemoryAddress>(sizeof(DataMemoryElement))-1U)/static_cast<DataMemoryAddress>(sizeof(DataMemoryElement));
}

template<typename T>
void Context::Pop(T &value){
    if (stackPtr){
        // adds granularity-1 so that also 1 byte uses 1 slot
        // stack points to the next free value. so one need to step back of the variable size
        stackPtr -= ByteSizeToDataMemorySize(sizeof(T));
        value = *((T *)stackPtr);
    }
}

template<typename T>
void Context::Peek(T &value){
    if (stackPtr){
        // adds granularity-1 so that also 1 byte uses 1 slot
        // stack points to the next free value. so one need to step back of the variable size
        DataMemoryElement *p =  stackPtr- ByteSizeToDataMemorySize(sizeof(T));
        value = *((T *)p);
    }
}


template<typename T>
void Context::Push(T &value){
    if (stackPtr ){
        *((T *)stackPtr) = value;
        // adds granularity-1 so that also 1 byte uses 1 slot
        stackPtr += ByteSizeToDataMemorySize(sizeof(T));
    }
}

template<typename T>
T &Context::Variable(DataMemoryAddress variableIndex){
    // note that variableIndex is an address to the memory with a granularity of sizeof(MemoryElement)
    return (T&)variablesMemoryPtr[variableIndex];
}

CodeMemoryElement Context::GetPseudoCode(){
    return *codeMemoryPtr++;
}

ErrorManagement::ErrorType Context::AddInputVariable(CCString name,TypeDescriptor td,DataMemoryAddress location){
    return AddVariable2DB(name,inputVariableInfo,td,location);
}

ErrorManagement::ErrorType Context::FindInputVariable(CCString name,VariableInformation *&variableInformation){
    return FindVariableinDB(name,variableInformation,inputVariableInfo);
}

ErrorManagement::ErrorType Context::AddOutputVariable(CCString name,TypeDescriptor td,DataMemoryAddress location){
    return AddVariable2DB(name,outputVariableInfo,td,location);
}

ErrorManagement::ErrorType Context::FindOutputVariable(CCString name,VariableInformation *&variableInformation){
    return FindVariableinDB(name,variableInformation,outputVariableInfo);
}

} //PseudoCode

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_PSEUDOCODE_H_ */
