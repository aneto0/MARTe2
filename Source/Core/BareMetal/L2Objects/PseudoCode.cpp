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
#include <stdio.h>

#include "PseudoCode.h"
#include "Private/PseudoCodeFunctions.h"
#include "StaticStack.h"
#include "AnyType.h"

namespace MARTe{

namespace PseudoCode{

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
class VariableFinder: public GenericIterator<VariableInformation>{
public:
	/**
	 *
	 */
	VariableFinder(CCString name);
	/**
	 *
	 */
	VariableFinder(DataMemoryAddress address);
	/**
	 *
	 */
    virtual IteratorAction Do(VariableInformation &data,uint32 depth=0);
    /**
     *
     */
    VariableInformation *variable;
private:
    /**
     *
     */
    DynamicCString variableName;

    /**
     *
     */
    DataMemoryAddress variableAddress;

};


VariableFinder::VariableFinder(CCString name){
	variable = NULL_PTR(VariableInformation*);
	variableName = name;
	variableAddress = MAXDataMemoryAddress;
}

VariableFinder::VariableFinder(DataMemoryAddress address){
	variable = NULL_PTR(VariableInformation*);
	variableAddress = address;
}


IteratorAction VariableFinder::Do(VariableInformation &data,uint32 depth){
	IteratorAction ret;
	if (variableName.GetSize() > 0){
		if (data.name == variableName){
			variable = &data;
			ret.SetActionCode(noAction);
		}
	} else
	if (variableAddress < MAXDataMemoryAddress){
		if (data.location == variableAddress){
			variable = &data;
			ret.SetActionCode(noAction);
		}
	}
	return ret;
}



/*************************************************************************/

Context::Context(){
	variablesMemoryPtr = NULL_PTR(DataMemoryElement*);
	codeMemoryPtr = NULL_PTR(CodeMemoryElement*);
	stackPtr = NULL_PTR(DataMemoryElement*);
	startOfVariables = 0;
}

Context::~Context(){
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

ErrorManagement::ErrorType Context::AddVariable2DB(CCString name,List<VariableInformation> &db,TypeDescriptor td,DataMemoryAddress location){
	ErrorManagement::ErrorType ret;
	VariableInformation *variableInfo;
	ret = FindVariableinDB(name,variableInfo,db);

	// if it is already there we do not need to add
	if (ret.unsupportedFeature){
		VariableInformation variableInfo;
		variableInfo.name = name;
		variableInfo.type = td;
		variableInfo.location = location;
//printf("Add %s @ %i  --> %i\n",name.GetList(),location,variableInfo.location);
		ret = db.Insert(variableInfo );
	} else {

		// it would be an error if this is an output variable
		// as we do not allow to override an output
		ret.invalidOperation = true;
	}

	return ret;
}

ErrorManagement::ErrorType Context::FindVariable(DataMemoryAddress address,VariableInformation *&variableInformation){
	ErrorManagement::ErrorType ret;

	VariableFinder finder(address);

	ret = outputVariableInfo.Iterate(finder);

	variableInformation = NULL;
	if (ret){
		variableInformation = finder.variable;
		ret.unsupportedFeature = (variableInformation == NULL);
	}

	if (!ret){
		ret = inputVariableInfo.Iterate(finder);
		if (ret){
			variableInformation = finder.variable;
			ret.unsupportedFeature = (variableInformation == NULL);
		}
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

	DataMemoryAddress nextConstantAddress = 0;

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
					// if an output variable of this name is already present
					// it means it would have already been loaded
					// so no need to fetch an external input
					VariableInformation *variableInformation;
					ret = FindOutputVariable(parameter,variableInformation);

					if (!ret){
						ret = AddInputVariable(parameter);
						if (ret.invalidOperation == true){
							COMPOSITE_REPORT_ERROR(ErrorManagement::Information,"variable ",parameter," already registered");
							// mask out the case that we already registered this variable
							ret.invalidOperation = false;
						}
						COMPOSITE_REPORT_ERROR(ret,"Failed Adding input variable ",parameter);
					}
				}
			} else
			if (command == writeToken){
				ret.invalidOperation = !hasParameter;
				COMPOSITE_REPORT_ERROR(ret,writeToken," without variable name");

				if (ret){
					ret = AddOutputVariable(parameter);
					if (ret.invalidOperation == true){
						COMPOSITE_REPORT_ERROR(ret,"variable ",parameter," already registered");
						// the error remains as we do not allow overwriting outputs
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
//printf("const type = %x\n",td.all);
				}
				// if supported add up the memory needs
				if (ret){
					DynamicCString constantName;
					constantName().Append("Constant").Append('@').Append(nextConstantAddress);
					ret = AddInputVariable(constantName,td,nextConstantAddress);
				}
				if (ret){
					nextConstantAddress += ByteSizeToDataMemorySize(td.StorageSize());
				}
			}
		}
	}

	if (ret){
		startOfVariables = nextConstantAddress;
	}

	return ret;
}

ErrorManagement::ErrorType Context::Compile(CCString RPNCode){
	ErrorManagement::ErrorType ret;

	DataMemoryAddress nextVariableAddress = startOfVariables;
	// check that all variables have a type and allocate variables + constants

	uint32 index = 0;
	PseudoCode::VariableInformation *var;
	while(BrowseInputVariable(index,var) && ret){
		ret.unsupportedFeature = !var->type.IsNumericType();
		COMPOSITE_REPORT_ERROR(ret,"input variable ",var->name," has incompatible non-numeric type ");

		// skip constants are already allocated
		if (ret && (var->location == MAXDataMemoryAddress)){
			var->location = nextVariableAddress;
			nextVariableAddress += ByteSizeToDataMemorySize(var->type.StorageSize());
		}
		index++;
	}

	index = 0;
	while(BrowseOutputVariable(index,var) && ret){
		ret.unsupportedFeature = !var->type.IsNumericType();
		COMPOSITE_REPORT_ERROR(ret,"input variable ",var->name," has incompatible non-numeric type ");

		if (ret){
			var->location = nextVariableAddress;
			nextVariableAddress += ByteSizeToDataMemorySize(var->type.StorageSize());
		}
		index++;
	}

	// already
	dataMemory.SetSize(nextVariableAddress);
	variablesMemoryPtr = static_cast<DataMemoryElement *>(dataMemory.GetDataPointer());

	// initialise compilation memory
	StaticStack<TypeDescriptor,32> typeStack;
	DataMemoryAddress maxDataStackSize = 0;    // max value of dataStackSize
	DataMemoryAddress dataStackSize = 0;       // current simulated value of data stack size
//	startOfVariables = 0;                      // for now no constants - so variables start at 0
	DataMemoryAddress nextConstantAddress = 0; // pointer to next constant memory area

    // clean all the memory
	codeMemory.Clean();

	bool finished = false;
	while ((!finished)  && ret){
		DynamicCString line;
		uint32 limit;
		// divide RPNCode into lines
		RPNCode = DynamicCString::Tokenize(RPNCode,line,limit,"\n","\n\r",false);

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

				VariableInformation *variableInformation = NULL_PTR(VariableInformation *);
				if (ret){
					ret = FindOutputVariable(parameter1,variableInformation);
					COMPOSITE_REPORT_ERROR(ret,"output variable ",parameter1, " not found");
				}

				TypeDescriptor td = InvalidType(0);
				if (ret){
					td = variableInformation->type;
					ret.unsupportedFeature = !td.IsNumericType();
					COMPOSITE_REPORT_ERROR(ret,"variable ",parameter1, "does not have a numeric supported format");
				}

				if (ret){
//printf("Stack[%i].Push(%x) -->",typeStack.GetSize(),td.all);
					ret.fatalError = !typeStack.Push(td);
					REPORT_ERROR(ret,"failed to push type into stack");
//printf("Stack[%i] \n",typeStack.GetSize());
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

				VariableInformation *variableInformation = NULL_PTR(VariableInformation *);

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

				TypeDescriptor td = InvalidType(0);
				if (ret){
					td = variableInformation->type;
					ret.unsupportedFeature = !td.IsNumericType();
					COMPOSITE_REPORT_ERROR(ret,"variable ",parameter1, "does not have a numeric supported format");
				}
//printf("read %s type = %x  type = %x\n",variableInformation->name.GetList(),td.all,variableInformation->type.all);
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

				// convert string to number and save value into memory
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
					// the actual command is a READ from the constant area
					command = readToken;
				}
			}

			CodeMemoryElement code = 0;
			if (ret){
				ret.unsupportedFeature = !FindPCodeAndUpdateTypeStack(code,command,typeStack,matchOutput,dataStackSize);
				if (!ret){
					DynamicCString typeList;
					CStringTool cst = typeList();
					uint32 n2scan = 2;
					if (matchOutput) {
						n2scan++;
					}
					cst.Append('[');
					for(uint32 index = 0;index < n2scan;index++){
						if (index > 0){
							cst.Append('|');
						}
						TypeDescriptor td;
						typeStack.Peek(index,td);
						td.ToString(cst);
					}
					cst.Append(']');
					COMPOSITE_REPORT_ERROR(ret,"command ",command, "(",typeList,") not found");
				}
//printf("after %s %i elements in dataStack\n ",command.GetList(),dataStackSize);
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

	// final checks
	if (ret){

		// assign the code pointer
		codeMemoryPtr = codeMemory.GetAllocatedMemoryConst();

		// size the stack
		stack.SetSize(maxDataStackSize);
		stackPtr = static_cast<DataMemoryElement*>(stack.GetDataPointer());

		variablesMemoryPtr = static_cast<DataMemoryElement *>(dataMemory.GetDataPointer());
	}

	// check that the TypeStack is empty
	if (ret){
		ret.internalSetupError = (typeStack.GetSize() > 0);
		COMPOSITE_REPORT_ERROR(ret,"operation sequence is incomplete: ",typeStack.GetSize()," data left in stack");
	}

	return ret;
}

ErrorManagement::ErrorType Context::FunctionRecordInputs2String(FunctionRecord &functionInformation,CStringTool &cst,bool peekOnly,bool showData,bool showTypes){
	 ErrorManagement::ErrorType ret;

	 const CodeMemoryElement *saveCodeMemoryPtr = codeMemoryPtr;

	 if (functionInformation.name == writeToken){
		 CodeMemoryElement pCode2 = GetPseudoCode();

		 VariableInformation *vi;
		 ret = FindVariable(pCode2,vi);
		 COMPOSITE_REPORT_ERROR(ret,"No variable or constant @ ",pCode2);

		 if (ret){
			cst.Append(' ').Append(vi->name);
		 }
	 }

	 DataMemoryAddress dataStackIndex = 0;

	if (showData || showTypes){
		for(uint32 i=0;(i<functionInformation.numberOfInputs) && ret;i++){
			if (i!=0) {
				cst.Append(',');
			} else {
				cst.Append('(');
			}
			if (showData && showTypes){
				cst.Append('(');
			}
			if (showTypes){
				 ret.fatalError = !functionInformation.types[i].ToString(cst);
			}
			if (showData && showTypes){
				cst.Append(')');
			}
			if (showData){
				dataStackIndex += ByteSizeToDataMemorySize(functionInformation.types[i].StorageSize());
				DynamicCString value;
				AnyType src(functionInformation.types[i],stackPtr - dataStackIndex);
				AnyType dest(value);
				ret = src.CopyTo(dest);

				cst.Append(value.GetList());

			}
			if (i == (functionInformation.numberOfInputs-1U)){
				cst.Append(')');
			}
		 }
	 }

	 // restore any used data
	 if (peekOnly){
		 codeMemoryPtr 	= saveCodeMemoryPtr;
	 }

	 return ret;
}

ErrorManagement::ErrorType Context::FunctionRecordOutputs2String(FunctionRecord &functionInformation,CStringTool &cst,bool lookBack,bool showData,bool showTypes){
	ErrorManagement::ErrorType ret;

	// if already showing the types do not show the parameter of the CAST
	if ((functionInformation.name == castToken) && (!showTypes)) {
		cst.Append(' ');
		ret.fatalError = !functionInformation.types[functionInformation.numberOfInputs].ToString(cst);
	} else
	if (functionInformation.name == readToken) {
		CodeMemoryElement pCode2;
		if (lookBack){
			pCode2 = codeMemoryPtr[-1];
		} else {
			pCode2 = GetPseudoCode();
		}

		VariableInformation *vi;
		ret = FindVariable(pCode2,vi);
		COMPOSITE_REPORT_ERROR(ret,"No variable or constant @ ",pCode2);

		if (ret){
			if (pCode2 < startOfVariables){

				cst.Append(' ');
				ret.fatalError = !vi->type.ToString(cst);
				cst.Append(' ');

				// Converts the value to a string
				DynamicCString value;
				AnyType dest(value);
				AnyType src(vi->type,&variablesMemoryPtr[pCode2]);
				ret = src.CopyTo(dest);
				REPORT_ERROR(ret,"CopyTo failed ");
				if (ret){
					cst.Append(value);
				}
			} else {
				cst.Append(' ').Append(vi->name);
			}
		}
	}

	DataMemoryAddress dataStackIndex = 0;

	if (showData || showTypes){
		for(uint32 i=0;(i<functionInformation.numberOfOutputs) && ret;i++){
			if (i!=0) {
				cst.Append(',');
			} else {
				cst.Append(" => (");
			}
			if (showData && showTypes){
				cst.Append('(');
			}
			if (showTypes){
				ret.fatalError = !functionInformation.types[i+functionInformation.numberOfInputs].ToString(cst);
			}
			if (showData && showTypes){
				cst.Append(')');
			}
			if (showData){
				dataStackIndex += ByteSizeToDataMemorySize(functionInformation.types[i].StorageSize());
				DynamicCString value;
				AnyType src(functionInformation.types[i+functionInformation.numberOfInputs],stackPtr - dataStackIndex);
				AnyType dest(value);
				ret = src.CopyTo(dest);

				cst.Append(value.GetList());
			}
			if (i == (functionInformation.numberOfOutputs-1U)){
				cst.Append(')');
			}
		}
	}

	return ret;
}

ErrorManagement::ErrorType Context::Execute(executionMode mode,StreamI *debugStream,CodeMemoryAddress *step){

	stackPtr = static_cast<DataMemoryElement*>(stack.GetDataPointer());

	codeMemoryPtr = codeMemory.GetAllocatedMemoryConst();
	CodeMemoryAddress codeMaxIndex  = static_cast<CodeMemoryAddress>(codeMemory.GetSize());

	const CodeMemoryElement *codeMemoryMaxPtr = codeMemoryPtr + codeMaxIndex;

	variablesMemoryPtr = static_cast<DataMemoryElement *>(dataMemory.GetDataPointer());
	runtimeError = ErrorManagement::ErrorType(true);

	switch (mode){
	case fastMode:{
		while(codeMemoryPtr < codeMemoryMaxPtr){
			CodeMemoryElement pCode = GetPseudoCode();
			functionRecords[pCode].function(*this);
		}
	}break;
	case safeMode:{
		DataMemoryElement *stackMinPtr = stackPtr;
		DataMemoryElement *stackMaxPtr = stackPtr + stack.GetNumberOfElements();
		while ((codeMemoryPtr < codeMemoryMaxPtr) && (runtimeError.ErrorsCleared())){
			CodeMemoryElement pCode = GetPseudoCode();
			functionRecords[pCode].function(*this);
			// note that the syackPtr will reach the max value - as it points to the next value to write
			runtimeError.outOfRange = ((stackPtr > stackMaxPtr) ||  (stackPtr < stackMinPtr));
			COMPOSITE_REPORT_ERROR(runtimeError,"stack over/under flow ", (int64)(stackPtr-stackMinPtr), " [0 - ", (int64)(stackMaxPtr- stackMinPtr), "]");
		}
		runtimeError.notCompleted = (codeMemoryPtr < codeMemoryMaxPtr);
		REPORT_ERROR(runtimeError,"code execution interrupted");
	}break;
	case debugMode:
	default:{
		if (debugStream == NULL_PTR(StreamI *)){
			runtimeError.parametersError = true;
			REPORT_ERROR(runtimeError,"debugMode requested with debugStream set to NULL");
		} else {
			DynamicCString debugMessage;
			CStringTool cst = debugMessage();

			cst.Append("[stackPtr]-[codePtr] :: [CODE] stack-in => stack-out\n");
			while ((codeMemoryPtr < codeMemoryMaxPtr) && (runtimeError)){
				int64 stackOffset = stackPtr - static_cast<DataMemoryElement*>(stack.GetDataPointer());
				int64 codeOffset  = codeMemoryPtr - codeMemory.GetAllocatedMemoryConst();
				cst.Append(stackOffset).Append(" - ").Append(codeOffset).Append(" :: ");

				CodeMemoryElement pCode = GetPseudoCode();

				FunctionRecord &fr = functionRecords[pCode];

				// show update info
     			cst.Append(fr.name).Append(' ');

				if (runtimeError.ErrorsCleared()){
					// show inputs
					runtimeError = FunctionRecordInputs2String(fr,cst,true,true,true);
				}

				if (runtimeError.ErrorsCleared()){
					// executes code
					fr.function(*this);
				}

				if (runtimeError.ErrorsCleared()){
					// show outputs
					runtimeError = FunctionRecordOutputs2String(fr,cst,true,true,true);
				}
				cst.Append('\n');

				if (runtimeError.ErrorsCleared()){
					uint32 size = debugMessage.GetSize();
					debugStream->Write(debugMessage.GetList(),size);
				}
				// reset line
				cst.SetSize(0);
			}
			if (runtimeError.ErrorsCleared()){
				int64 stackOffset = stackPtr - static_cast<DataMemoryElement*>(stack.GetDataPointer());
				int64 codeOffset  = codeMemoryPtr - codeMemory.GetAllocatedMemoryConst();
				cst.Append(stackOffset).Append(" - ").Append(codeOffset).Append(" :: END");

				uint32 size = debugMessage.GetSize();
				debugStream->Write(debugMessage.GetList(),size);
			}
		}
	}
	}

	if (stackPtr  != static_cast<DataMemoryElement*>(stack.GetDataPointer())){
		runtimeError.internalSetupError = true;
		int64 offset = stackPtr - static_cast<DataMemoryElement*>(stack.GetDataPointer());
		COMPOSITE_REPORT_ERROR(runtimeError,"stack pointer not back to origin : ",offset, " elements left");
	}

	return runtimeError;
}

ErrorManagement::ErrorType Context::DeCompile(DynamicCString &RPNCode,bool showTypes){
	ErrorManagement::ErrorType ret ;

	codeMemoryPtr = codeMemory.GetAllocatedMemoryConst();
	CodeMemoryAddress codeMaxIndex  = static_cast<CodeMemoryAddress>(codeMemory.GetSize());
	const CodeMemoryElement *codeMemoryMaxPtr = codeMemoryPtr + codeMaxIndex;

	variablesMemoryPtr = static_cast<DataMemoryElement *>(dataMemory.GetDataPointer());

	CStringTool cst = RPNCode();

	while((codeMemoryPtr < codeMemoryMaxPtr) && ret){
		CodeMemoryElement pCode = GetPseudoCode();
		FunctionRecord &fr = functionRecords[pCode];

		if ((fr.name == readToken) && (codeMemoryPtr[0] < startOfVariables)){
			cst.Append(constToken);
		} else {
			cst.Append(fr.name);
		}

		ret = FunctionRecordInputs2String(fr,cst,false,false,showTypes);

		if (ret){
			ret = FunctionRecordOutputs2String(fr,cst,false,false,showTypes);
		}
		cst.Append('\n');
	}

	return ret;
}

} //PseudoCode
} //MARTe
