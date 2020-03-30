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

namespace MARTe{

namespace PseudoCode{


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


Context::VariableFinder::VariableFinder(CCString name){
	variable = NULL_PTR(VariableInformation*);
	variableName = name;
}

IteratorAction Context::VariableFinder::Do(VariableInformation &data,uint32 depth){
	IteratorAction ret;
	if (data.name == variableName){
		variable = &data;
		ret.SetActionCode(noAction);
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


ErrorManagement::ErrorType Context::ExtractVariables(CCString RPNCode){
	ErrorManagement::ErrorType ret;

	bool finished = false;
	while (!finished  && ret){
		DynamicCString line;
		uint32 limit;
		// divide RPNCode into lines
		DynamicCString::Tokenize(RPNCode,line,limit,"\n","\r",false);
		finished = (line.GetSize()==0);

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

			if (command == "READ"){
				ret.invalidOperation = !hasParameter;
				REPORT_ERROR(ret,"READ without variable name");
				if (ret){
					ret = AddInputVariable(parameter);
					if (ret.invalidOperation == true){
						COMPOSITE_REPORT_ERROR(ret,"variable",parameter,"already registered");
						ret.invalidOperation = false; // mask out the case that we already registered this variable
					}
					COMPOSITE_REPORT_ERROR(ret,"Failed Adding input variable ",parameter);
				}
			} else
			if (command == "WRITE"){
				ret.invalidOperation = !hasParameter;
				REPORT_ERROR(ret,"WRITE without variable name");
				if (ret){
					ret = AddOutputVariable(parameter);
					if (ret.invalidOperation == true){
						COMPOSITE_REPORT_ERROR(ret,"variable ",parameter," already registered");
						ret.invalidOperation = false; // mask out the case that we already registered this variable
					}
					COMPOSITE_REPORT_ERROR(ret,"Failed Adding output variable ",parameter);
				}
			} else
			if (command == "CONST"){
				ret.invalidOperation = !hasParameter;
				REPORT_ERROR(ret,"CONST without type name");

				// transform the type name into a TypeDescriptor
				// check it is one of the supported types
				TypeDescriptor td;
				if (ret){
					td = TypeDescriptor(parameter);
					ret.unsupportedFeature = !td.IsNumericType();
					COMPOSITE_REPORT_ERROR(ret,"Failed Adding constant of type ",parameter);
				}
				// if supported add up the memory needs
				if (ret){
					sizeOfConstantsArea = ByteSizeToMemorySize(td.StorageSize());
				}
			}
		}
	}

	return ret;
}


PCode Context::GetPseudoCode(){
	return 0;
}


/***********************************************************************************************/

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
	PCode index;
	index = context.GetPseudoCode();
	context.Push(context.Variable<T>(index));
}

template <typename T> void Write(Context &context){
	PCode index;
	index = context.GetPseudoCode();
	context.Pop(context.Variable<T>(index));
}


REGISTER_PCODE_FUNCTION(ADD,float,2,1,0,Addition<float>,Float32Bit,Float32Bit,Float32Bit)
REGISTER_PCODE_FUNCTION(ADD,double,2,1,0,Addition<double>,Float64Bit,Float64Bit,Float64Bit)
REGISTER_PCODE_FUNCTION(READ,double,0,1,1,Read<double>,Float64Bit)
REGISTER_PCODE_FUNCTION(WRITE,double,1,0,1,Write<double>,Float64Bit)


} //PseudoCode
} //MARTe
