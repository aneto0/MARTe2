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
#include "Vector.h"
#include "List.h"
#include "CCString.h"
#include "TypeDescriptor.h"
#include "CompositeErrorManagement.h"



/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{


namespace PseudoCode{

typedef uint16 CodeMemoryElement;
typedef uint32 DataMemoryElement;
typedef uint16 DataMemoryAddress;
typedef uint16 CodeMemoryAddress;

/**
 *
 */
class Context{

public:
	/**
    * @brief Get the top of the stack and then move the pointer.
    * @param[in] value reference to the variable and then update stack pointer (note that the stack will have a specific granularity).
    * @return .
   */
	template<typename T>
	inline void Pop(T &value);

	template<typename T>
	inline void Push(T &value);

	template<typename T>
	inline void Peek(T &value);

	/**
	 * Reads from code memory
	 */
	inline CodeMemoryElement GetPseudoCode();

	/**
	 * Reads from Data Memory
	 */
	template<typename T>
	T &Variable(uint16 variableIndex);

	/**
    * @brief Context will maintain a single error flag. This function allows setting it
    * @param[in] err is the value to set
    * @return .
   */
	inline void SetErrorFlag(ErrorManagement::ErrorType err);

	/**
    * @brief Context will maintain a single error flag. This function allows getting it
    * @return the error flag.
   */
	inline ErrorManagement::ErrorType GetErrorFlag();

	/**
	 *
	 */
	Context(){
		variablesMemoryPtr = NULL_PTR(DataMemoryElement*);
		variablesMaxIndex = 0;
		pCodePtr = NULL_PTR(CodeMemoryElement*);
		codeMaxIndex = 0;

		stackPtr = NULL_PTR(DataMemoryElement*);
		stackStartPtr = NULL_PTR(DataMemoryElement*);
		stackMaxPtr = NULL_PTR(DataMemoryElement*);
		stackMaxIndex = 0;
	}

	/**
	 *  element for the list of variables
	 */
	struct VariableInformation {
		/**
		 *
		 */
		DynamicCString name;
		/**
		 *
		 */
		TypeDescriptor type;
		/**
		 *
		 */
		DataMemoryAddress location;
		/**
		 *
		 */
		VariableInformation(){
			type = VoidType;
			location = 0;
		}
		/**
		 *
		 */
		VariableInformation(const VariableInformation &in){
			name = in.name;
			type = in.type;
			location = 0;
		}
	};

	/**
	 * Cleans inputVariableInfo
	 * Cleans outputVariableInfo
	 * Scans RPNCode looking for READ, WRITE and CONST functions
	 */
	ErrorManagement::ErrorType ExtractVariables(CCString RPNCode);

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
	ErrorManagement::ErrorType Compile(CCString RPNCode);

	/**
	 * executes every command in codeMemory
	 */
	ErrorManagement::ErrorType Execute();

public:
	/**
	 *
	 */
	ErrorManagement::ErrorType  runtimeError;


	/**
	 * stack and variable are allocated here
	 */
	StaticList<CodeMemoryElement,32> codeMemory;

	/**
	 * stack and variable are allocated here
	 */
	Vector<DataMemoryElement> dataMemory;

	/*
	 * MEMORY MAP
	 *
	 * sizeOfVariablesArea     VARIABLES   --> variablesMemoryPtr   : pCodePtr
	 *                            CONSTANTS
	 *                            INPUTS
	 *                            OUTPUTS
	 * sizeOfCode              PCODE list  --> pCodePtr             : stackStartPtr
	 * sizeOfStack             STACK       --> stackStartPtr    	: stackMaxPtr
	 *
	 */

	DataMemoryElement *stackPtr;
	DataMemoryElement *stackStartPtr;
	DataMemoryElement *stackMaxPtr;
	DataMemoryAddress stackMaxIndex;
	/**
	 * how many MemoryElement are used for constants
	 */
	DataMemoryAddress variablesMaxIndex;

	/**
	 *
	 */
	DataMemoryElement *variablesMemoryPtr;

	/**
	 *
	 */
	CodeMemoryAddress codeMaxIndex;

	/**
	 *
	 */
	CodeMemoryElement *codeMemoryPtr;

private:

	/**
	 * Checks existence of name using FindInputVariable
	 * If not found add new variable
	 */
	inline ErrorManagement::ErrorType AddInputVariable(CCString name);

	/**
	 * Looks for a variable of a given name
	 */
	inline ErrorManagement::ErrorType FindInputVariable(CCString name,VariableInformation *&variableInformation);

	/**
	 * Checks existence of name using FindOutputVariable
	 * If not found add new variable
	 */
	inline ErrorManagement::ErrorType AddOutputVariable(CCString name);

	/**
	 * Looks for a variable of a given name
	 */
	inline ErrorManagement::ErrorType FindOutputVariable(CCString name,VariableInformation *&variableInformation);


	/**
	 * the input variable names
	 */
	List<VariableInformation> inputVariableInfo;

	/**
	 * the output variable names
	 */
	List<VariableInformation> outputVariableInfo;

	/**
	 *
	 */
	ErrorManagement::ErrorType AddVariable2DB(CCString name,List<VariableInformation> &db);

	/**
	 *
	 */
	ErrorManagement::ErrorType FindVariableinDB(CCString name,VariableInformation *&variableInformation,List<VariableInformation> &db);
};





/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


static inline DataMemoryAddress ByteSizeToDataMemorySize(uint32 byteSize){
	return (byteSize+sizeof(DataMemoryElement)-1)/sizeof(DataMemoryElement);
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

ErrorManagement::ErrorType Context::AddInputVariable(CCString name){
	return AddVariable2DB(name,inputVariableInfo);
}

ErrorManagement::ErrorType Context::FindInputVariable(CCString name,VariableInformation *&variableInformation){
	return FindVariableinDB(name,variableInformation,inputVariableInfo);
}

ErrorManagement::ErrorType Context::AddOutputVariable(CCString name){
	return AddVariable2DB(name,outputVariableInfo);
}

ErrorManagement::ErrorType Context::FindOutputVariable(CCString name,VariableInformation *&variableInformation){
	return FindVariableinDB(name,variableInformation,outputVariableInfo);
}


} //PseudoCode

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_PSEUDOCODE_H_ */
