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

typedef uint16 PCode;
typedef uint32 MemoryElement;

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

	template<typename T>
	T &Variable(uint16 variableIndex);

	PCode GetPseudoCode();

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
		variablesMemoryPtr = NULL_PTR(MemoryElement*);
		variablesMaxIndex = 0;
		stack = NULL_PTR(MemoryElement*);
		stackStart = NULL_PTR(MemoryElement*);
		stackMax = NULL_PTR(MemoryElement*);
		sizeOfConstantsArea=0;
	}

	/**
	 *  element for the list of variables
	 */
	struct VariableInformation{
		DynamicCString name;
		TypeDescriptor type;
		uint64 initialisation;
		VariableInformation(){
			type = VoidType;
			initialisation = 0;
		}
		VariableInformation(const VariableInformation &in){
			name = in.name;
			type = in.type;
			initialisation = 0;
		}
	};

	/**
	 * Cleans inputVariableInfo
	 * Cleans outputVariableInfo
	 * Scans RPNCode looking for READ, WRITE and CONST functions
	 */
	ErrorManagement::ErrorType ExtractVariables(CCString RPNCode);

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
	 * Looks for a variable at a given location
	 */
	inline ErrorManagement::ErrorType BrowseInputVariable(uint32 index,VariableInformation *&variableInformation);

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
	 * Looks for a variable at a given location
	 */
	inline ErrorManagement::ErrorType BrowseOutputVariable(uint32 index,VariableInformation *&variableInformation);

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
	ErrorManagement::ErrorType Executes(CCString RPNCode);

private:
	ErrorManagement::ErrorType  error;
	MemoryElement *variablesMemoryPtr;
	MemoryElement variablesMaxIndex;
	MemoryElement *stack;
	MemoryElement *stackStart;
	MemoryElement *stackMax;
	/// how many MemoryElement are used for constants
	uint32 sizeOfConstantsArea;

	/**
	 * allows searching for a variable with a given name
	 */
	class VariableFinder: public GenericIterator<VariableInformation>{
	public:
		VariableFinder(CCString name);
	    virtual IteratorAction Do(VariableInformation &data,uint32 depth=0);
	    VariableInformation *variable;
	private:
	    DynamicCString variableName;
	};

	/**
	 * stack and variable are allocated here
	 */
	StaticList<MemoryElement,32> dataMemory;

	/**
	 * stack and variable are allocated here
	 */
	StaticList<MemoryElement,32> codeMemory;

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

/** the type of the PCode function */
typedef void (*Function)(Context & context);

/**
 * records information necessary to be able to use it during compilation
 */
struct FunctionRecord{
	/**
	 *	The name of the functions as used in the RPN code
	 */
	CCString name;

	/**
	 * How many stack elements it will consume
	 */
	uint16 numberOfInputs;

	/**
	 * How many stack elements it will produce
	 */
	uint16 numberOfOutputs;

	/**
	 * How many parameters to read from the PseudoCode section
	 */
	uint16 numberOfParameters;

	/**
	 * array of types one for each input and output
	 */
	const TypeDescriptor *types;

	/**
	 * The function code itself
	 */
	Function function;

};


/**
 * to register a function
 */
void RegisterFunction(const FunctionRecord &record);

/**
 * generates boiler plate code to register a function
 */
#define REGISTER_PCODE_FUNCTION(name,subName,nInputs,nOutputs,nParams,function,...)\
	static const TypeDescriptor name ## subName ## _FunctionTypes[] = {__VA_ARGS__}; \
	static const FunctionRecord name ## subName ## _FunctionRecord={#name,nInputs,nOutputs,nParams,name ## subName ## _FunctionTypes,&function}; \
	static const class name ## subName ## RegisterClass { \
	public:\
	void name ## subName ## _RegisterClass(){\
			RegisterFunction(name ## subName ## _FunctionRecord);\
		}\
	} name ## subName ## _RegisterClassInstance;





/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


static inline uint32 ByteSizeToMemorySize(uint32 byteSize){
	return (byteSize+sizeof(MemoryElement)-1)/sizeof(MemoryElement);
}

template<typename T>
void Context::Pop(T &value){
	if (stack ){
		// adds granularity-1 so that also 1 byte uses 1 slot
		// stack points to the next free value. so one need to step back of the variable size
		stack -= ByteSizeToMemorySize(sizeof(T));
		value = *((T *)stack);
	}
}

template<typename T>
void Context::Push(T &value){
	if (stack ){
		*((T *)stack) = value;
		// adds granularity-1 so that also 1 byte uses 1 slot
		stack += ByteSizeToMemorySize(sizeof(T));
	}
}

template<typename T>
T &Context::Variable(PCode variableIndex){
	// note that variableIndex is an address to the memory with a granularity of sizeof(MemoryElement)
	return (T&)variablesMemoryPtr[variableIndex];
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
