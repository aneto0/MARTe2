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
#include "PseudoCodeFunctions.h"
#include "IteratorT.h"
#include "StaticStack.h"
#include "AnyType.h"
#include "TypeConversion.h"
#include "AdvancedErrorManagement.h"

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
class VariableFinder: public IteratorT<VariableInformation>{
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
    void Do(VariableInformation *data);
    /**
     *
     */
    VariableInformation *variable;
    /**
     *
     */
    ErrorManagement::ErrorType error;

private:
    /**
     *
     */
    StreamString variableName;

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


void VariableFinder::Do(VariableInformation *data){
    if (data == NULL_PTR(VariableInformation *)){
        error = ErrorManagement::FatalError;
    } else {
        if (variableName.Size() > 0){
            if (data->name == variableName){
                variable = data;
                error = ErrorManagement::NoError;
            }
        } else
        if (variableAddress < MAXDataMemoryAddress){
            if (data->location == variableAddress){
                variable = data;
                error = ErrorManagement::NoError;
            }
        }
    }
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

ErrorManagement::ErrorType Context::FindVariableinDB(CCString name,VariableInformation *&variableInformation,LinkedListHolderT<VariableInformation> &db){
	ErrorManagement::ErrorType ret;

	VariableFinder finder(name);
    db.ListIterate(&finder);
    ret = finder.error;
    REPORT_ERROR_STATIC(ret, "Iteration failed");

    variableInformation = NULL;
    if (ret){
        variableInformation = finder.variable;
        ret.unsupportedFeature = (variableInformation == NULL);
    }
    return ret;
}

ErrorManagement::ErrorType Context::AddVariable2DB(CCString name,LinkedListHolderT<VariableInformation> &db,TypeDescriptor td,DataMemoryAddress location){
	ErrorManagement::ErrorType ret;
	VariableInformation *variableInfo;
	ret = FindVariableinDB(name,variableInfo,db);

	// if it is already there we do not need to add
	if (ret.unsupportedFeature){
        VariableInformation *variableInfo = new VariableInformation;
        variableInfo->name = name;
        variableInfo->type = td;
        variableInfo->location = location;
//printf("Add %s @ %i  --> %i\n",name.GetList(),location,variableInfo.location);
        db.ListAdd(variableInfo);
        ret.unsupportedFeature = false;
	} else {

		// it would be an error if this is an output variable
		// as we do not allow to override an output
        ret.illegalOperation = true;
	}

	return ret;
}

ErrorManagement::ErrorType Context::FindVariable(DataMemoryAddress address,VariableInformation *&variableInformation){
	ErrorManagement::ErrorType ret;

	VariableFinder finder(address);

    outputVariableInfo.ListIterate(&finder);
    ret = finder.error;

	variableInformation = NULL;
	if (ret){
		variableInformation = finder.variable;
		ret.unsupportedFeature = (variableInformation == NULL);
	}

	if (!ret){
        inputVariableInfo.ListIterate(&finder);
        ret = finder.error;
		if (ret){
			variableInformation = finder.variable;
			ret.unsupportedFeature = (variableInformation == NULL);
		}
	}
	return ret;
}

ErrorManagement::ErrorType Context::BrowseInputVariable(uint32 index,VariableInformation *&variableInformation){
	ErrorManagement::ErrorType ret;
    variableInformation = inputVariableInfo.ListPeek(index);
	ret.outOfRange = (variableInformation == NULL);
	return ret;
}

ErrorManagement::ErrorType Context::BrowseOutputVariable(uint32 index,VariableInformation *&variableInformation){
	ErrorManagement::ErrorType ret;
    variableInformation = outputVariableInfo.ListPeek(index);
	ret.outOfRange = (variableInformation == NULL);
	return ret;
}

ErrorManagement::ErrorType Context::ExtractVariables(StreamString &RPNCode){
	ErrorManagement::ErrorType ret;

	DataMemoryAddress nextConstantAddress = 0;

    StreamString line;
    char8 terminator;
    while (RPNCode.GetToken(line, "\n", terminator, "\n\r") && ret){

		// extract command and parameter
        StreamString command;
        StreamString parameter;

        // extract up to two tokens per line
        line.Seek(0u);
        line.GetToken(command, " \t,", terminator," \t,");
        line.GetToken(parameter, " \t,", terminator," \t,");

		// now analyse the command
        if (command.Size() > 0){
            bool hasParameter = (parameter.Size()> 0);

            if (command == readToken){
                ret.illegalOperation = !hasParameter;
                REPORT_ERROR_STATIC(ret,"%s without variable name", command.Buffer());
				if (ret){
					// if an output variable of this name is already present
					// it means it would have already been loaded
					// so no need to fetch an external input
					VariableInformation *variableInformation;
                    ret = FindOutputVariable(parameter.Buffer(),variableInformation);

					if (!ret){
                        ret = AddInputVariable(parameter.Buffer());
                        if (ret.illegalOperation == true){
                            REPORT_ERROR_STATIC(ErrorManagement::Information,"variable %s already registered", parameter.Buffer());
							// mask out the case that we already registered this variable
                            ret.illegalOperation = false;
						}
                        REPORT_ERROR_STATIC(ret,"Failed Adding input variable %s",parameter.Buffer());
					}
				}
			} else
            if (command == writeToken){
                ret.illegalOperation = !hasParameter;
                REPORT_ERROR_STATIC(ret, "%s without variable name", command.Buffer());

				if (ret){
                    ret = AddOutputVariable(parameter.Buffer());
                    if (ret.illegalOperation == true){
                        REPORT_ERROR_STATIC(ret,"variable %s already registered", parameter.Buffer());
						// the error remains as we do not allow overwriting outputs
					}
                    REPORT_ERROR_STATIC(ret,"Failed Adding output variable %s",parameter.Buffer());
				}
			} else
            if (command == constToken){
                ret.illegalOperation = !hasParameter;
                REPORT_ERROR_STATIC(ret, " without type name", command.Buffer());

				// transform the type name into a TypeDescriptor
				// check it is one of the supported types
				TypeDescriptor td;
				if (ret){
                    td = TypeDescriptor::GetTypeDescriptorFromTypeName(parameter.Buffer());
					ret.unsupportedFeature = !td.IsNumericType();
                    REPORT_ERROR_STATIC(ret,"type %s is not a numeric supported format", parameter.Buffer());
//printf("const type = %x\n",td.all);
				}
				// if supported add up the memory needs
				if (ret){
                    StreamString constantName;
                    constantName.Printf("Constant@%u", nextConstantAddress);
                    ret = AddInputVariable(constantName.Buffer(),td,nextConstantAddress);
				}
				if (ret){
                    uint32 size = td.numberOfBits * 8u;
					ret.unsupportedFeature = (size == 0);
                    REPORT_ERROR_STATIC(ret,"type %s has 0 storageSize", parameter.Buffer());

					nextConstantAddress += ByteSizeToDataMemorySize(size);
				}
			}
		}

        line = "";

    }

	if (ret){
		startOfVariables = nextConstantAddress;
	}

	return ret;
}

ErrorManagement::ErrorType Context::Compile(StreamString &RPNCode){
	ErrorManagement::ErrorType ret;

	DataMemoryAddress nextVariableAddress = startOfVariables;
	// check that all variables have a type and allocate variables + constants

	uint32 index = 0;
	PseudoCode::VariableInformation *var;
	while(BrowseInputVariable(index,var) && ret){
		ret.unsupportedFeature = !var->type.IsNumericType();
        REPORT_ERROR_STATIC(ret,"input variable %s has incompatible non-numeric type ", var->name.Buffer());

		// skip constants are already allocated
		if (ret && (var->location == MAXDataMemoryAddress)){
			var->location = nextVariableAddress;
            nextVariableAddress += ByteSizeToDataMemorySize(var->type.numberOfBits * 8u);
		}
		index++;
	}

	index = 0;
	while(BrowseOutputVariable(index,var) && ret){
		ret.unsupportedFeature = !var->type.IsNumericType();
        REPORT_ERROR_STATIC(ret,"input variable %s has incompatible non-numeric type ", var->name.Buffer());

		if (ret){
			var->location = nextVariableAddress;
            nextVariableAddress += ByteSizeToDataMemorySize(var->type.numberOfBits * 8u);
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

    StreamString line;
    char8 terminator;
    while (RPNCode.GetToken(line, "\n", terminator, "\n\r") && ret){

		// extract command and parameter
        StreamString command;
        StreamString parameter1;
        StreamString parameter2;

        // extract up to three tokens per line
        line.Seek(0u);
        line.GetToken(command, " \t,", terminator, " \t,");
        line.GetToken(parameter1, " \t,", terminator, " \t,");
        line.GetToken(parameter2, " \t,", terminator, " \t,");

		// now analyze the command
        if (command.Size() > 0){
			// assign invalid value
			CodeMemoryElement code2 = TypeCharacteristics<CodeMemoryElement>::MaxValue();
			bool matchOutput = false;

            bool hasParameter1 = (parameter1.Size()> 0);

			// PROCESS CAST command
			// PUSH type(parameter1) --> TypeStack
			// matchOutput = true;
			if (command == castToken){
                ret.illegalOperation = !hasParameter1;
                REPORT_ERROR_STATIC(ret,"%s without type name", command.Buffer());
				if (ret){
					// transform the type name into a TypeDescriptor
					// check it is one of the supported types
					TypeDescriptor td;
                    td = TypeDescriptor::GetTypeDescriptorFromTypeName(parameter1.Buffer());
					ret.unsupportedFeature = !td.IsNumericType();
                    REPORT_ERROR_STATIC(ret,"type %s is not a numeric supported format", parameter1.Buffer());

					if (ret){
						ret.fatalError = !typeStack.Push(td);
                        REPORT_ERROR_STATIC(ret,"failed to push type into stack");
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
                ret.illegalOperation = !hasParameter1;
                REPORT_ERROR_STATIC(ret, "%s without variable name", command.Buffer());

				VariableInformation *variableInformation = NULL_PTR(VariableInformation *);
				if (ret){
                    ret = FindOutputVariable(parameter1.Buffer(),variableInformation);
                    REPORT_ERROR_STATIC(ret,"output variable %s not found", parameter1.Buffer());
				}

                TypeDescriptor td = InvalidType;
				if (ret){
					td = variableInformation->type;
					ret.unsupportedFeature = !td.IsNumericType();
                    REPORT_ERROR_STATIC(ret,"variable %s does not have a numeric supported format", parameter1.Buffer());
				}

				if (ret){
//printf("Stack[%i].Push(%x) -->",typeStack.GetSize(),td.all);
					ret.fatalError = !typeStack.Push(td);
                    REPORT_ERROR_STATIC(ret,"failed to push type into stack");
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
                ret.illegalOperation = !hasParameter1;
                REPORT_ERROR_STATIC(ret, "%s without variable name", command.Buffer());

				VariableInformation *variableInformation = NULL_PTR(VariableInformation *);

				if (ret){
					// try find an output variable with this name
                    ret = FindOutputVariable(parameter1.Buffer(),variableInformation);
					if (ret){
						// not set yet - cannot use
						ret.notCompleted = (variableInformation->variableUsed != true);
					}
					// try to see if there is an input variable
					if (!ret){
                        ret = FindInputVariable(parameter1.Buffer(),variableInformation);
                        REPORT_ERROR_STATIC(ret,"input variable %s not found", parameter1.Buffer());
					}
				}

                TypeDescriptor td = InvalidType;
				if (ret){
					td = variableInformation->type;
					ret.unsupportedFeature = !td.IsNumericType();
                    REPORT_ERROR_STATIC(ret,"variable %s does not have a numeric supported format", parameter1.Buffer());
				}
//printf("read %s type = %x  type = %x\n",variableInformation->name.GetList(),td.all,variableInformation->type.all);
				if (ret){
					ret.fatalError = !typeStack.Push(td);
                    REPORT_ERROR_STATIC(ret,"failed to push type into stack");
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
                bool hasParameter2 = (parameter2.Size()> 0);

                ret.illegalOperation = !hasParameter1 || !hasParameter2;
                REPORT_ERROR_STATIC(ret,"%s without type name and value", command.Buffer());

				// transform the type name into a TypeDescriptor
				// check it is one of the supported types
				TypeDescriptor td;
				if (ret){
                    td = TypeDescriptor::GetTypeDescriptorFromTypeName(parameter1.Buffer());
					ret.unsupportedFeature = !td.IsNumericType();
                    REPORT_ERROR_STATIC(ret,"type %s is not a numeric supported format", parameter1.Buffer());
				}

				// convert string to number and save value into memory
				if (ret){
					//nextConstantAddress
                    AnyType dest(td, 0,&variablesMemoryPtr[nextConstantAddress]);
                    ret.fatalError = !TypeConvert(dest, parameter2.Buffer());
                    REPORT_ERROR_STATIC(ret,"TypeConvert failed ");

				}

				if (ret){
					ret.fatalError = !typeStack.Push(td);
                    REPORT_ERROR_STATIC(ret,"failed to push type into stack");
				}

				if (ret){
					matchOutput = true;
					code2 = nextConstantAddress;
                    nextConstantAddress += ByteSizeToDataMemorySize(td.numberOfBits * 8u);
					// the actual command is a READ from the constant area
					command = readToken;
				}
			}

			CodeMemoryElement code = 0;
			if (ret){
                ret.unsupportedFeature = !FindPCodeAndUpdateTypeStack(code,command.Buffer(),typeStack,matchOutput,dataStackSize);
                if (!ret){
                    StreamString typeList;
					uint32 n2scan = 2;
					if (matchOutput) {
						n2scan++;
					}
                    typeList += '[';
					for(uint32 index = 0;index < n2scan;index++){
						if (index > 0){
                            typeList += '|';
						}
						TypeDescriptor td;
						typeStack.Peek(index,td);
                        typeList += TypeDescriptor::GetTypeNameFromTypeDescriptor(td);
					}
                    typeList += ']';
                    REPORT_ERROR_STATIC(ret,"command %s(%s) not found", command.Buffer(), typeList.Buffer());
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
                REPORT_ERROR_STATIC(ret,"failed to add instruction to code");

				if (ret && (code2 != TypeCharacteristics<CodeMemoryElement>::MaxValue())){
					ret.fatalError = !codeMemory.Add(code2);
                    REPORT_ERROR_STATIC(ret,"failed to add instruction to code");
				}
			}
		}

        line = "";

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
        REPORT_ERROR_STATIC(ret,"operation sequence is incomplete: %u data left in stack", typeStack.GetSize());
	}

	return ret;
}

ErrorManagement::ErrorType Context::FunctionRecordInputs2String(FunctionRecord &functionInformation,StreamString &cst,bool peekOnly,bool showData,bool showTypes){
	 ErrorManagement::ErrorType ret;

	 const CodeMemoryElement *saveCodeMemoryPtr = codeMemoryPtr;

	 if (functionInformation.name == writeToken){
		 CodeMemoryElement pCode2 = GetPseudoCode();

		 VariableInformation *vi;
		 ret = FindVariable(pCode2,vi);
         REPORT_ERROR_STATIC(ret,"No variable or constant @ %u",pCode2);

		 if (ret){
            cst.Printf(" %s", vi->name.Buffer());
		 }
	 }

	 DataMemoryAddress dataStackIndex = 0;

	if (showData || showTypes){
		for(uint32 i=0;(i<functionInformation.numberOfInputs) && ret;i++){
			TypeDescriptor td = functionInformation.types[i];

			if (i!=0) {
                cst += ',';
			} else {
                cst += '(';
			}
			if (showData && showTypes){
                cst += '(';
			}
			if (showTypes){
                 CCString typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(td);
                 if (typeName != NULL) {
                    cst += typeName.GetList();
                 } else {
                    ret.fatalError = true;
                 }
			}
			if (showData && showTypes){
                cst += ')';
			}
			if (showData){
                dataStackIndex += ByteSizeToDataMemorySize(td.numberOfBits * 8u);
                StreamString value;
                AnyType src(td, 0, stackPtr - dataStackIndex);
				AnyType dest(value);
                dest = src;

                cst += value;

			}
			if (i == (functionInformation.numberOfInputs-1U)){
                cst += ')';
			}
		 }
	 }

	 // restore any used data
	 if (peekOnly){
		 codeMemoryPtr 	= saveCodeMemoryPtr;
	 }

	 return ret;
}

ErrorManagement::ErrorType Context::FunctionRecordOutputs2String(FunctionRecord &functionInformation,StreamString &cst,bool lookBack,bool showData,bool showTypes){
	ErrorManagement::ErrorType ret;

	// if already showing the types do not show the parameter of the CAST
	if ((functionInformation.name == castToken) && (!showTypes)) {
        cst += ' ';
        CCString typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(functionInformation.types[functionInformation.numberOfInputs]);
        if (typeName != NULL) {
            cst += typeName.GetList();
        } else {
            ret.fatalError = true;
        }

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
        REPORT_ERROR_STATIC(ret,"No variable or constant @ %u",pCode2);

		if (ret){
			if (pCode2 < startOfVariables){

                cst += ' ';

                CCString typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(vi->type);
                if (typeName != NULL) {
                    cst += typeName.GetList();
                } else {
                    ret.fatalError = true;
                }

                cst += ' ';

				// Converts the value to a string
                StreamString value;
				AnyType dest(value);
                AnyType src(vi->type, 0, &variablesMemoryPtr[pCode2]);
                dest = src;
                REPORT_ERROR_STATIC(ret,"CopyTo failed ");
				if (ret){
                    cst += value;
				}
			} else {
                cst.Printf(" %s", vi->name.Buffer());
			}
		}
	}

	DataMemoryAddress dataStackIndex = 0;

	if (showData || showTypes){
		for(uint32 i=0;(i<functionInformation.numberOfOutputs) && ret;i++){
			TypeDescriptor td = functionInformation.types[i+functionInformation.numberOfInputs];
			if (i!=0) {
                cst += ',';
			} else {
                cst += " => (";
			}
			if (showData && showTypes){
                cst += '(';
			}
			if (showTypes){
                CCString typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(td);
                if (typeName != NULL) {
                    cst += typeName.GetList();
                } else {
                    ret.fatalError = true;
                }
			}
			if (showData && showTypes){
                cst += ')';
			}
			if (showData){
                dataStackIndex += ByteSizeToDataMemorySize(td.numberOfBits * 8u);
//cst.Append('[').Append(dataStackIndex).Append(']');
                StreamString value;
                AnyType src(td, 0, stackPtr - dataStackIndex);
				AnyType dest(value);
                dest = src;

                cst += value;
			}
			if (i == (functionInformation.numberOfOutputs-1U)){
                cst += ')';
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
            REPORT_ERROR_STATIC(runtimeError,"stack over/under flow %i [0 - %i]", (int64)(stackPtr-stackMinPtr), (int64)(stackMaxPtr- stackMinPtr));
		}
		runtimeError.notCompleted = (codeMemoryPtr < codeMemoryMaxPtr);
        REPORT_ERROR_STATIC(runtimeError,"code execution interrupted");
	}break;
	case debugMode:
	default:{
		if (debugStream == NULL_PTR(StreamI *)){
			runtimeError.parametersError = true;
            REPORT_ERROR_STATIC(runtimeError,"debugMode requested with debugStream set to NULL");
		} else {
            StreamString debugMessage;

            debugMessage += "[line]-[stackPtr]-[codePtr]::[CODE] stack-in => stack-out\n";
			int32 lineCounter = 1;
			while ((codeMemoryPtr < codeMemoryMaxPtr) && (runtimeError)){
				int64 stackOffset = stackPtr - static_cast<DataMemoryElement*>(stack.GetDataPointer());
				int64 codeOffset  = codeMemoryPtr - codeMemory.GetAllocatedMemoryConst();
                debugMessage.Printf("%i - %i - %i :: ", lineCounter, stackOffset, codeOffset);

				CodeMemoryElement pCode = GetPseudoCode();

				FunctionRecord &fr = functionRecords[pCode];

				// show update info
                debugMessage.Printf("%s ", fr.name.Buffer());

     			// errors due to debugging
     			ErrorManagement::ErrorType ret;

     			// show inputs
                ret = FunctionRecordInputs2String(fr,debugMessage,true,true,true);
                REPORT_ERROR_STATIC(ret,"analysing input side of function call");

				// executes code
				fr.function(*this);

				if (ret){
					// show outputs
                    ret = FunctionRecordOutputs2String(fr,debugMessage,true,true,true);
                    REPORT_ERROR_STATIC(ret,"analysing input side of function call");
				}

				if (!runtimeError.ErrorsCleared()){
                    debugMessage += " <ERROR> ";
				}

                debugMessage += '\n';

                uint32 size = debugMessage.Size();
                debugStream->Write(debugMessage.Buffer(),size);

				// reset line
                debugMessage.SetSize(0);
				lineCounter++;

			}
			if (runtimeError.ErrorsCleared()){
				int64 stackOffset = stackPtr - static_cast<DataMemoryElement*>(stack.GetDataPointer());
				int64 codeOffset  = codeMemoryPtr - codeMemory.GetAllocatedMemoryConst();
                debugMessage.Printf("%i - %i :: END", stackOffset, codeOffset);

                uint32 size = debugMessage.Size();
                debugStream->Write(debugMessage.Buffer(),size);
			}
		}
	}
	}
    REPORT_ERROR_STATIC(runtimeError,"Execution error");

	if (stackPtr  != static_cast<DataMemoryElement*>(stack.GetDataPointer())){
		runtimeError.internalSetupError = true;
		int64 offset = stackPtr - static_cast<DataMemoryElement*>(stack.GetDataPointer());
        REPORT_ERROR_STATIC(runtimeError,"stack pointer not back to origin : %i elements left", offset);
	}

	return runtimeError;
}

ErrorManagement::ErrorType Context::DeCompile(StreamString &RPNCode,bool showTypes){
	ErrorManagement::ErrorType ret ;

	codeMemoryPtr = codeMemory.GetAllocatedMemoryConst();
	CodeMemoryAddress codeMaxIndex  = static_cast<CodeMemoryAddress>(codeMemory.GetSize());
	const CodeMemoryElement *codeMemoryMaxPtr = codeMemoryPtr + codeMaxIndex;

	variablesMemoryPtr = static_cast<DataMemoryElement *>(dataMemory.GetDataPointer());

	while((codeMemoryPtr < codeMemoryMaxPtr) && ret){
		CodeMemoryElement pCode = GetPseudoCode();
		FunctionRecord &fr = functionRecords[pCode];

		if ((fr.name == readToken) && (codeMemoryPtr[0] < startOfVariables)){
            RPNCode += constToken;
		} else {
            RPNCode += fr.name;
		}

        ret = FunctionRecordInputs2String(fr,RPNCode,false,false,showTypes);

		if (ret){
            ret = FunctionRecordOutputs2String(fr,RPNCode,false,false,showTypes);
		}
        RPNCode += '\n';
	}

	return ret;
}

} //PseudoCode
} //MARTe
