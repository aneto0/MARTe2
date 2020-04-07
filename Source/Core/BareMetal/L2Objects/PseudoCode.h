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
#include "TypeCharacteristics.h"

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

#define MAXDataMemoryAddress TypeCharacteristics<DataMemoryAddress>::MaxValue()

/**
 * used internally to describe and register functions
 */
struct FunctionRecord;

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
    * @brief Context will maintain a single error flag. This function allows accessing it
    * @return the error flag.
   */
	inline ErrorManagement::ErrorType &GetErrorFlag();

	/**
	 *
	 */
	Context();

	/**
	 *  Element for the list of variables
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
		 * set when during compilation to mark that this output variable has already been written
		 *
		 */
		bool variableUsed;

		/**
		 *
		 */
		VariableInformation(){
			type = VoidType;
			location = 0;
			variableUsed = false;
		}

		/**
		 *
		 */
		VariableInformation(const VariableInformation &in){
			name = in.name;
			type = in.type;
			location = in.location;
			variableUsed = false;
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
	 * allow choosing how to run the code
	 */
	enum executionMode {
		fastMode,
		safeMode,
		debugMode,
		singleStep
	};

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
	 * note that the inputs need to be loaded before calling execute
	 * returns the combination of error flags reported by all the functions that were executed
	 */
	ErrorManagement::ErrorType Execute(executionMode mode = fastMode,StreamI *debugStream=NULL_PTR(StreamI *),uint32 step=0);

	/**
	 * Reconstruct the RPNCode with type information
	 */
	ErrorManagement::ErrorType DeCompile(DynamicCString &RPNCode);

// PUBLIC VARIABLES

	/**
	 *
	 */
	ErrorManagement::ErrorType  		runtimeError;


	/**
	 * stack and variable are allocated here
	 */
	StaticList<CodeMemoryElement,32> 	codeMemory;

	/**
	 * variable and constants are allocated here
	 * MEMORY MAP
	 *
	 * sizeOfVariablesArea     VARIABLES   --> variablesMemoryPtr   : pCodePtr
	 *                            CONSTANTS
	 *                            INPUTS
	 *                            OUTPUTS
	 */
	Vector<DataMemoryElement> 			dataMemory;

	/**
	 * address of first variable (after constants) or how many MemoryElement are used for constants
	 */
	DataMemoryAddress 					startOfVariables;

	/**
	 * stack is allocated here
	 */
	Vector<DataMemoryElement> 			stack;

	/**
	 * used during runtime
	 */
	DataMemoryElement *					stackPtr;

	/**
	 *
	 */
	DataMemoryElement *					variablesMemoryPtr;


	/**
	 *
	 */
	const CodeMemoryElement *			codeMemoryPtr;


private:

	/**
	 * Checks existence of name using FindInputVariable
	 * If not found add new variable
	 */
	inline ErrorManagement::ErrorType AddInputVariable(CCString name,TypeDescriptor td = InvalidType(0),DataMemoryAddress location = MAXDataMemoryAddress);

	/**
	 * Looks for a variable of a given name
	 */
	inline ErrorManagement::ErrorType FindInputVariable(CCString name,VariableInformation *&variableInformation);

	/**
	 * Checks existence of name using FindOutputVariable
	 * If not found add new variable
	 */
	inline ErrorManagement::ErrorType AddOutputVariable(CCString name,TypeDescriptor td = InvalidType(0),DataMemoryAddress location = MAXDataMemoryAddress);

	/**
	 * Looks for a variable of a given name
	 */
	inline ErrorManagement::ErrorType FindOutputVariable(CCString name,VariableInformation *&variableInformation);

	/**
	 * Looks for a variable of a given name
	 */
	ErrorManagement::ErrorType FindVariable(DataMemoryAddress address,VariableInformation *&variableInformation);

	/**
	 *
	 */
	ErrorManagement::ErrorType AddVariable2DB(CCString name,List<VariableInformation> &db,TypeDescriptor td,DataMemoryAddress location);

	/**
	 *
	 */
	ErrorManagement::ErrorType FindVariableinDB(CCString name,VariableInformation *&variableInformation,List<VariableInformation> &db);

	/**
	 * expands the variableInformation into a readable text
	 * if more pCode is required for the decoding it will get it from context.
	 * it will access DataMemory as well to decode constants
	 * if peekOnly = true does not change the codeMemoryPtr
	 */
	ErrorManagement::ErrorType FunctionRecord2String(FunctionRecord &functionInformation,CStringTool &cst,bool peekOnly=false);

	/**
	 * the input variable names
	 */
	List<VariableInformation> inputVariableInfo;

	/**
	 * the output variable names
	 */
	List<VariableInformation> outputVariableInfo;

};





/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


static inline DataMemoryAddress ByteSizeToDataMemorySize(uint32 byteSize){
	return (static_cast<DataMemoryAddress>(byteSize)+static_cast<DataMemoryAddress>(sizeof(DataMemoryElement))-1)/static_cast<DataMemoryAddress>(sizeof(DataMemoryElement));
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


ErrorManagement::ErrorType &Context::GetErrorFlag(){
	return runtimeError;
}


} //PseudoCode

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_PSEUDOCODE_H_ */
