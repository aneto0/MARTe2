/**
 * @file PseudoCode.cpp
 * @brief Header file for class AnyType
 * @date Mar 26, 2020
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

#include "PseudoCode.h"
#include <stdio.h>
#include "StaticStack.h"
#include "AnyType.h"



namespace MARTe{

namespace PseudoCode{

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
	 * !NOTE that for CONST
	 */
	uint16  numberOfInputs;

	/**
	 * How many stack elements it will produce
	 */
	uint16 numberOfOutputs;

	/**
	 * array of types one for each input and output
	 */
	const TypeDescriptor *types;


	/**
	 * The function code itself
	 */
	Function function;

	/**
	 * returns true if the name and types matches
	 * on success replaces the type on the stack with the result type
	 * also simulates variations on the dataStack
	 */
	bool TryConsume(CCString nameIn,StaticStack<TypeDescriptor,32> &typeStack, bool matchOutput,DataMemoryAddress &dataStackSize);

};

bool FunctionRecord::TryConsume(CCString nameIn,StaticStack<TypeDescriptor,32> &typeStack, bool matchOutput,DataMemoryAddress &dataStackSize){
	bool ret = false;

	// match function name
	ret = (name == nameIn);

	// match first output if matchOutput is set
	uint32 index = 0U;
	if (ret && matchOutput){
		TypeDescriptor type;
		ret = typeStack.Peek(index++,type);
		if (ret){
			ret = (type.SameAs(types[numberOfInputs]));
		}
	}

	// match inputs types
	for (uint32 i = 0U; ret && (i < numberOfInputs); i++){
		TypeDescriptor type;
		ret = typeStack.Peek(index++,type);
		if (ret){
			ret = (type.SameAs(types[i]));
		}
	}

	// found! commit changes
	if (ret){

		// remove first output type
		if (matchOutput){
			TypeDescriptor type;
			typeStack.Pop(type);
		}

		// remove inputs types
		for (uint32 i = 0U; ret && (i < numberOfInputs); i++){
			TypeDescriptor type;
			typeStack.Pop(type);
			dataStackSize -= ByteSizeToDataMemorySize(type.StorageSize());
		}

		// insert output types
		for (uint32 i = 0U; ret && (i < numberOfOutputs); i++){
			typeStack.Push(types[i+numberOfInputs]);
			dataStackSize += ByteSizeToDataMemorySize(types[i+numberOfInputs].StorageSize());
		}
	}

	return ret;
}

/**
 * max number of registered functions
 */
const uint32 maxFunctions = 16384;

/**
 * actually available functions
 */
uint32 availableFunctions = 0;

/**
 * the database of functions
 */
FunctionRecord functionRecords[maxFunctions];

/**
 * to register a function
 */
void RegisterFunction(const FunctionRecord &record){
	if (availableFunctions < maxFunctions){
		functionRecords[availableFunctions++] = record;
	}
}

/**
 * find the correct PCode and updates the type in the typestack
 */
bool FindPCodeAndUpdateTypeStack(CodeMemoryElement &code, CCString nameIn,StaticStack<TypeDescriptor,32> &typeStack, bool matchOutput,DataMemoryAddress dataStackSize){

	uint32 i = 0;
	bool ret = true;

	for (i=0;ret && (i < availableFunctions);i++ ){
		ret = functionRecords[i].TryConsume(nameIn,typeStack,matchOutput,dataStackSize);
	}

	if (ret){
		code = i;
	}

	return ret;
}



/*************************************************************************************************************************/
/*************************************************************************************************************************/


/**
 * The only 4 standard tokens
 */
const CCString readToken("READ");
const CCString writeToken("WRITE");
const CCString constToken("CONST");
const CCString castToken("CAST");



/**
 * allows searching for a variable with a given name
 */
class VariableFinder: public GenericIterator<Context::VariableInformation>{
public:
	/**
	 *
	 */
	VariableFinder(CCString name);
	/**
	 *
	 */
    virtual IteratorAction Do(Context::VariableInformation &data,uint32 depth=0);
    /**
     *
     */
    Context::VariableInformation *variable;
private:
    /**
     *
     */
    DynamicCString variableName;
};


VariableFinder::VariableFinder(CCString name){
	variable = NULL_PTR(Context::VariableInformation*);
	variableName = name;
}

IteratorAction VariableFinder::Do(Context::VariableInformation &data,uint32 depth){
	IteratorAction ret;
	if (data.name == variableName){
		variable = &data;
		ret.SetActionCode(noAction);
	}
	return ret;
}



/*************************************************************************/

Context::Context(){
	variablesMemoryPtr = NULL_PTR(DataMemoryElement*);
	variablesMaxIndex = 0;
	codeMemoryPtr = NULL_PTR(CodeMemoryElement*);
	codeMaxIndex = 0;

	stackPtr = NULL_PTR(DataMemoryElement*);
	stackStartPtr = NULL_PTR(DataMemoryElement*);
	stackMaxPtr = NULL_PTR(DataMemoryElement*);
}

ErrorManagement::ErrorType Context::FindVariableinDB(CCString name,VariableInformation *&variableInformation,List<VariableInformation> &db){
	ErrorManagement::ErrorType ret;

	VariableFinder finder(name);
	ret = db.Iterate(finder);
	REPORT_ERROR(ret,"Iteration failed");

	variableInformation = NULL;
	if (ret){
		variableInformation = finder.variable;
		ret.unsupportedFeature = (variableInformation == NULL);
	}
	return ret;
}

ErrorManagement::ErrorType Context::AddVariable2DB(CCString name,List<VariableInformation> &db){
	ErrorManagement::ErrorType ret;

	VariableInformation *variableInfo;
	ret = FindVariableinDB(name,variableInfo,db);

	// if it is already there we cannot add
	if (ret.unsupportedFeature){
		VariableInformation variableInfo;
		variableInfo.name = name;
		ret = db.Insert(variableInfo );
	} else {
		ret.invalidOperation = true;
	}

	return ret;
}

ErrorManagement::ErrorType Context::BrowseInputVariable(uint32 index,VariableInformation *&variableInformation){
	ErrorManagement::ErrorType ret;
	variableInformation = inputVariableInfo[index];
	ret.outOfRange = (variableInformation == NULL);
	return ret;
}

ErrorManagement::ErrorType Context::BrowseOutputVariable(uint32 index,VariableInformation *&variableInformation){
	ErrorManagement::ErrorType ret;
	variableInformation = outputVariableInfo[index];
	ret.outOfRange = (variableInformation == NULL);
	return ret;
}


ErrorManagement::ErrorType Context::ExtractVariables(CCString RPNCode){
	ErrorManagement::ErrorType ret;

	variablesMaxIndex = 0;

	bool finished = false;
	while (!finished  && ret){
		DynamicCString line;
		uint32 limit;
		// divide RPNCode into lines
		RPNCode = DynamicCString::Tokenize(RPNCode,line,limit,"\n","\n\r",false);
		finished = (line.GetSize()==0);
//printf("LINE = %s\n",line.GetList());
		// extract command and parameter
		DynamicCString command;
		DynamicCString parameter;
		if (!finished){
			CCString lineP = line;
			// extract up to two tokens per line
			lineP = DynamicCString::Tokenize(lineP,command,limit," \t,"," \t,",false);
			DynamicCString::Tokenize(lineP,parameter,limit," \t,"," \t,",false);
		}

		// now analyse the command
		if (command.GetSize() > 0){
			bool hasParameter = (parameter.GetSize()> 0);

			if (command == readToken){
				ret.invalidOperation = !hasParameter;
				COMPOSITE_REPORT_ERROR(ret,readToken," without variable name");
				if (ret){
					ret = AddInputVariable(parameter);
					if (ret.invalidOperation == true){
						COMPOSITE_REPORT_ERROR(ret,"variable ",parameter," already registered");
						ret.invalidOperation = false; // mask out the case that we already registered this variable

					}
					COMPOSITE_REPORT_ERROR(ret,"Failed Adding input variable ",parameter);
				}
			} else
			if (command == writeToken){
				ret.invalidOperation = !hasParameter;
				COMPOSITE_REPORT_ERROR(ret,writeToken," without variable name");

				if (ret){
					ret = AddOutputVariable(parameter);
					if (ret.invalidOperation == true){
						COMPOSITE_REPORT_ERROR(ret,"variable ",parameter," already registered");
						ret.invalidOperation = false; // mask out the case that we already registered this variable
					}
					COMPOSITE_REPORT_ERROR(ret,"Failed Adding output variable ",parameter);
				}
			} else
			if (command == constToken){
				ret.invalidOperation = !hasParameter;
				COMPOSITE_REPORT_ERROR(ret,constToken," without type name");

				// transform the type name into a TypeDescriptor
				// check it is one of the supported types
				TypeDescriptor td;
				if (ret){
					td = TypeDescriptor(parameter);
					ret.unsupportedFeature = !td.IsNumericType();
					COMPOSITE_REPORT_ERROR(ret,"type ",parameter, " is not a numeric supported format");
				}
				// if supported add up the memory needs
				if (ret){
					variablesMaxIndex += ByteSizeToDataMemorySize(td.StorageSize());
				}
			}
		}
	}

	return ret;
}




ErrorManagement::ErrorType Context::Compile(CCString RPNCode){
	ErrorManagement::ErrorType ret;

	// check that all variables have a type and allocate variables + constants

	uint32 index = 0;
	PseudoCode::Context::VariableInformation *var;
	while(BrowseInputVariable(index,var) && ret){
		ret.unsupportedFeature = !var->type.IsNumericType();
		COMPOSITE_REPORT_ERROR(ret,"input variable ",var->name," has incompatible non-numeric type ");

		if (ret){
			var->location = variablesMaxIndex;
			variablesMaxIndex += ByteSizeToDataMemorySize(var->type.StorageSize());
		}
		index++;
	}

	index = 0;
	while(BrowseOutputVariable(index,var)){
		ret.unsupportedFeature = !var->type.IsNumericType();
		COMPOSITE_REPORT_ERROR(ret,"input variable ",var->name," has incompatible non-numeric type ");

		if (ret){
			var->location = variablesMaxIndex;
			variablesMaxIndex += ByteSizeToDataMemorySize(var->type.StorageSize());
		}
		index++;
	}

	// already
	dataMemory.SetSize(variablesMaxIndex);
	variablesMemoryPtr = static_cast<DataMemoryElement *>(dataMemory.GetDataPointer());


	// initialise compilation memory
	StaticStack<TypeDescriptor,32> typeStack;
	DataMemoryAddress maxDataStackSize = 0;    // max value of dataStackSize
	DataMemoryAddress dataStackSize = 0;       // current simulated value of data stack size
	DataMemoryAddress nextConstantAddress = 0; // pointer to next constant memory area

    // clean all the memory
	codeMemory.Clean();


	bool finished = false;
	while (!finished  && ret){
		DynamicCString line;
		uint32 limit;
		// divide RPNCode into lines
		RPNCode = DynamicCString::Tokenize(RPNCode,line,limit,"\n","\n\r",false);

printf("new line: %s\n",line.GetList());

		finished = (line.GetSize()==0);
		// extract command and parameter
		DynamicCString command;
		DynamicCString parameter1;
		DynamicCString parameter2;
		if (!finished){
			CCString lineP = line;
			// extract up to two tokens per line
			lineP = DynamicCString::Tokenize(lineP,command,limit," \t,"," \t,",false);
			lineP = DynamicCString::Tokenize(lineP,parameter1,limit," \t,"," \t,",false);
			DynamicCString::Tokenize(lineP,parameter2,limit," \t,"," \t,",false);
		}

		// now analyze the command
		if (command.GetSize() > 0){
			// assign invalid value
			CodeMemoryElement code2 = TypeCharacteristics<CodeMemoryElement>::MaxValue();
			bool matchOutput = false;

			bool hasParameter1 = (parameter1.GetSize()> 0);

			// PROCESS CAST command
			// PUSH type(parameter1) --> TypeStack
			// matchOutput = true;
			if (command == castToken){
				ret.invalidOperation = !hasParameter1;
				COMPOSITE_REPORT_ERROR(ret,command," without type name");
				if (ret){
					// transform the type name into a TypeDescriptor
					// check it is one of the supported types
					TypeDescriptor td;
					td = TypeDescriptor(parameter1);
					ret.unsupportedFeature = !td.IsNumericType();
					COMPOSITE_REPORT_ERROR(ret,"type ",parameter1, " is not a numeric supported format");

					if (ret){
						ret.fatalError = !typeStack.Push(td);
						REPORT_ERROR(ret,"failed to push type into stack");
					}

					if (ret){
						matchOutput = true;
					}
				}
			} else

			// PROCESS WRITE command
			// find_variable(parameter1) on outputs
			//    mark variable as already written
			// PUSH variable.type --> TypeStack
			// matchOutput = true;
			// assign code2 to address of variable
			if (command == writeToken){
				ret.invalidOperation = !hasParameter1;
				COMPOSITE_REPORT_ERROR(ret,writeToken," without variable name");

				VariableInformation *variableInformation;
				if (ret){
					ret = FindOutputVariable(parameter1,variableInformation);
					COMPOSITE_REPORT_ERROR(ret,"output variable ",parameter1, " not found");
				}

				TypeDescriptor td;
				if (ret){
					td = variableInformation->type;
					ret.unsupportedFeature = !td.IsNumericType();
					COMPOSITE_REPORT_ERROR(ret,"variable ",parameter1, "does not have a numeric supported format");
				}

				if (ret){
					ret.fatalError = !typeStack.Push(td);
					REPORT_ERROR(ret,"failed to push type into stack");
				}

				if (ret){
					matchOutput = true;
					code2 = variableInformation->location;
					variableInformation->variableUsed = true;
				}

			} else

			// PROCESS READ command
			// find_variable(parameter1)
			//    search first on outputs if already written
			//    search then on inputs
			// PUSH variable.type --> TypeStack
			// matchOutput = true;
			// assign code2 to address of variable
			if (command == readToken){
				ret.invalidOperation = !hasParameter1;
				COMPOSITE_REPORT_ERROR(ret,readToken," without variable name");

				VariableInformation *variableInformation;
				if (ret){
					// try find an output variable with this name
					ret = FindOutputVariable(parameter1,variableInformation);
					if (ret){
						// not set yet - cannot use
						ret.notCompleted = (variableInformation->variableUsed != true);
					}
					// try to see if there is an input variable
					if (!ret){
						ret = FindInputVariable(parameter1,variableInformation);
						COMPOSITE_REPORT_ERROR(ret,"input variable ",parameter1, " not found");
					}
				}

				TypeDescriptor td;
				if (ret){
					td = variableInformation->type;
					ret.unsupportedFeature = !td.IsNumericType();
					COMPOSITE_REPORT_ERROR(ret,"variable ",parameter1, "does not have a numeric supported format");
				}

				if (ret){
					ret.fatalError = !typeStack.Push(td);
					REPORT_ERROR(ret,"failed to push type into stack");
				}

				if (ret){
					matchOutput = true;
					code2 = variableInformation->location;
				}


			} else

			// PROCESS CONST command
			// PUSH type(parameter1) --> TypeStack
			// matchOutput = true;
			// create AnyType and convert constant from string to variable memory
			// assign code2 to address of constant
			// command = READ
			if (command == constToken){
				bool hasParameter2 = (parameter2.GetSize()> 0);

				ret.invalidOperation = !hasParameter1 || !hasParameter2;
				COMPOSITE_REPORT_ERROR(ret,constToken," without type name and value");

				// transform the type name into a TypeDescriptor
				// check it is one of the supported types
				TypeDescriptor td;
				if (ret){
					td = TypeDescriptor(parameter1);
					ret.unsupportedFeature = !td.IsNumericType();
					COMPOSITE_REPORT_ERROR(ret,"type ",parameter1, " is not a numeric supported format");
				}

				// convert and save constant into memory
				if (ret){
					//nextConstantAddress
					AnyType src(parameter2);
					AnyType dest(td,&variablesMemoryPtr[nextConstantAddress]);
					ret = src.CopyTo(dest);
					REPORT_ERROR(ret,"CopyTo failed ");
				}

				if (ret){
					ret.fatalError = !typeStack.Push(td);
					REPORT_ERROR(ret,"failed to push type into stack");
				}

				if (ret){
					matchOutput = true;
					code2 = nextConstantAddress;
					nextConstantAddress += ByteSizeToDataMemorySize(td.StorageSize());
				}
			}

			CodeMemoryElement code = 0;
			if (ret){
				ret.unsupportedFeature = !FindPCodeAndUpdateTypeStack(code,command,typeStack,matchOutput,dataStackSize);
			}

			// finally compile!
			if (ret){
				// update stackSize
				if (dataStackSize > maxDataStackSize){
					maxDataStackSize = dataStackSize;
				}

				ret.fatalError = !codeMemory.Add(code);
				REPORT_ERROR(ret,"failed to add instruction to code");

				if (ret && (code2 != TypeCharacteristics<CodeMemoryElement>::MaxValue())){
					ret.fatalError = !codeMemory.Add(code2);
					REPORT_ERROR(ret,"failed to add instruction to code");
				}
			}
		}

	}

	// check that the TypeStack is empty
	if (ret){
		ret.internalSetupError = (typeStack.GetSize() > 0);
		REPORT_ERROR(ret,"operation sequence is incomplete - data left in stack");
	}

	// final checks
	if (ret){

		// variables are already done

		// assign the code pointer
		codeMemoryPtr = codeMemory.GetAllocatedMemoryConst();
		codeMaxIndex  = codeMemory.GetSize();

		// size the stack
		stack.SetSize(maxDataStackSize);
		stackPtr = static_cast<DataMemoryElement*>(stack.GetDataPointer());
		stackStartPtr = stackPtr;
		stackMaxPtr = stackPtr + maxDataStackSize;
	}


	return ret;
}



/***********************************************************************************************/


/**
 * to register a function
 */
void RegisterFunction(const FunctionRecord &record);

/**
 * generates boiler plate code to register a function
 */
#define REGISTER_PCODE_FUNCTION(name,subName,nInputs,nOutputs,function,...)\
	static const TypeDescriptor name ## subName ## _FunctionTypes[] = {__VA_ARGS__}; \
	static const FunctionRecord name ## subName ## _FunctionRecord={#name,nInputs,nOutputs,name ## subName ## _FunctionTypes,&function}; \
	static const class name ## subName ## RegisterClass { \
	public:\
	void name ## subName ## _RegisterClass(){\
			RegisterFunction(name ## subName ## _FunctionRecord);\
		}\
	} name ## subName ## _RegisterClassInstance;


template <typename T> void Addition(Context &context){
	T result;
	T addendum1;
	T addendum2;

	context.Pop(addendum1);
	context.Pop(addendum2);
	result = addendum1+addendum2;
	context.Push(result);
}

template <typename T> void Read(Context &context){
	CodeMemoryElement index;
	index = context.GetPseudoCode();
	context.Push(context.Variable<T>(index));
}

template <typename T> void Write(Context &context){
	CodeMemoryElement index;
	index = context.GetPseudoCode();
	context.Pop(context.Variable<T>(index));
}


REGISTER_PCODE_FUNCTION(ADD,float,2,1,Addition<float>,Float32Bit,Float32Bit,Float32Bit)
REGISTER_PCODE_FUNCTION(ADD,double,2,1,Addition<double>,Float64Bit,Float64Bit,Float64Bit)
REGISTER_PCODE_FUNCTION(READ,double,0,1,Read<double>,Float64Bit)
REGISTER_PCODE_FUNCTION(WRITE,double,1,0,Write<double>,Float64Bit)


} //PseudoCode
} //MARTe
