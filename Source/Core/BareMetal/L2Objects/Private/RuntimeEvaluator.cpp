/**
 * @file RuntimeEvaluator.cpp
 * @brief Header file for class RuntimeEvaluator
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
#include "Stack.h"
#include "StaticStack.h"
#include "AnyType.h"

#include "RuntimeEvaluator.h"
#include "Private/RuntimeEvaluatorVariables.h"
#include "Private/RuntimeEvaluatorVariableInformation.h"
#include "Private/RuntimeEvaluatorFunction.h"

namespace MARTe{

/**
 * The only 4 standard tokens
 */
const CCString readToken("READ");
const CCString writeToken("WRITE");
const CCString constToken("CONST");
const CCString castToken("CAST");
const CCString remoteWriteToken("RWRITE");
const CCString remoteReadToken("RREAD");


const RuntimeEvaluator::DataMemoryAddress RuntimeEvaluator::InvalidDataMemoryAddress = TypeCharacteristics<RuntimeEvaluator::DataMemoryAddress>::MaxValue();
const RuntimeEvaluator::CodeMemoryElement RuntimeEvaluator::InvalidCodeMemoryElement = TypeCharacteristics<RuntimeEvaluator::CodeMemoryElement>::MaxValue();

/**
 * allows searching for a variable with a given name in the database
 * it allows modifying the content of the database
 */
class VariableFinder: public GenericIterator<RuntimeEvaluator::VariableInformation>{
public:
    /**
     *
     */
    VariableFinder(CCString name);
    /**
     *
     */
    VariableFinder(RuntimeEvaluator::DataMemoryAddress address);
    /**
     *
     */
    virtual IteratorAction Do(RuntimeEvaluator::VariableInformation &data,uint32 depth=0);
    /**
     *
     */
    RuntimeEvaluator::VariableInformation *variable;
private:
    /**
     *
     */
    DynamicCString variableName;

    /**
     *
     */
    RuntimeEvaluator::DataMemoryAddress variableAddress;

};

VariableFinder::VariableFinder(CCString name){
    variable = NULL_PTR(RuntimeEvaluator::VariableInformation*);
    variableName = name;
    variableAddress = RuntimeEvaluator::InvalidDataMemoryAddress;
}

VariableFinder::VariableFinder(RuntimeEvaluator::DataMemoryAddress address){
    variable = NULL_PTR(RuntimeEvaluator::VariableInformation*);
    variableAddress = address;
}


IteratorAction VariableFinder::Do(RuntimeEvaluator::VariableInformation &data,uint32 depth){
    IteratorAction ret;
    if (variableName.GetSize() > 0){
        if (data.GetName() == variableName){
            variable = &data;
            ret.SetActionCode(noAction);
        }
    } else
    if (variableAddress < RuntimeEvaluator::InvalidDataMemoryAddress){
        if (data.GetLocation() == variableAddress){
            variable = &data;
            ret.SetActionCode(noAction);
        }
    }
    return ret;
}



/*************************************************************************/

RuntimeEvaluator::RuntimeEvaluator(){
    variablesMemoryPtr = NULL_PTR(DataMemoryElement*);
    sizeOfVariablesMemory = 0;
    codeMemoryPtr = NULL_PTR(CodeMemoryElement*);
    stackPtr = NULL_PTR(DataMemoryElement*);
}

RuntimeEvaluator::~RuntimeEvaluator(){
    RuntimeEvaluatorVariables temp(variablesMemoryPtr);
    temp.DeAllocate();
}

ErrorManagement::ErrorType RuntimeEvaluator::FindVariableinDB(CCString name,VariableInformation *&variableInformation,List<VariableInformation> &db){
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

ErrorManagement::ErrorType RuntimeEvaluator::AddVariable2DB(CCString name,List<VariableInformation> &db,VariableDescriptor vd/*,DataMemoryAddress location*/){
    ErrorManagement::ErrorType 					ret;
    VariableInformation *	variableInformationPtr;

    ret = FindVariableinDB(name,variableInformationPtr,db);

    // if it is already there we do not need to add
    if (ret.unsupportedFeature){
        VariableInformation variableInformation(name,vd);
        ret = db.Insert(variableInformation );
    } else {

        // it would be an error if this is an output variable
        // as we do not allow to override an output
        ret.invalidOperation 	= true;
    }

    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::FindVariable(DataMemoryAddress address,VariableInformation *&variableInformationPtr){
    ErrorManagement::ErrorType ret;

    VariableFinder finder(address);

    ret = outputVariableInfo.Iterate(finder);

    variableInformationPtr = NULL;
    if (ret){
        variableInformationPtr = finder.variable;
        ret.unsupportedFeature = (variableInformationPtr == NULL);
    }

    if (!ret){
        ret = inputVariableInfo.Iterate(finder);
        if (ret){
            variableInformationPtr = finder.variable;
            ret.unsupportedFeature = (variableInformationPtr == NULL);
        }
    }
    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::BrowseInputVariable(uint32 index,VariableInformation *&variableInformationPtr){
    ErrorManagement::ErrorType ret;
    variableInformationPtr = inputVariableInfo[index];
    ret.outOfRange = (variableInformationPtr == NULL);
    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::BrowseOutputVariable(uint32 index,VariableInformation *&variableInformationPtr){
    ErrorManagement::ErrorType ret;
    variableInformationPtr = outputVariableInfo[index];
    ret.outOfRange = (variableInformationPtr == NULL);
    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::ExtractVariables(CCString RPNCode){
    ErrorManagement::ErrorType ret;

    bool finished = false;
    while (!finished  && ret){
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
        bool hasParameter1 = (parameter1.GetSize()> 0);
        bool hasParameter2 = (parameter2.GetSize()> 0);

        // now analyse the command
        if (command.GetSize() > 0){

            if (command == readToken){
                ret.invalidOperation = !hasParameter1 || hasParameter2;
                COMPOSITE_REPORT_ERROR(ret,readToken," without variable name");
                if (ret){
                    // if an output variable of this name is already present
                    // it means it would have already been loaded
                    // so no need to fetch an external input
                    VariableInformation *variableInformationPtr = NULL_PTR(VariableInformation *);
                    ret = FindOutputVariable(parameter1,variableInformationPtr);

                    if (!ret){
                        ret = AddInputVariable(parameter1);
                        if (ret.invalidOperation == true){
                            COMPOSITE_REPORT_ERROR(ErrorManagement::Information,"variable ",parameter1," already registered");
                            // mask out the case that we already registered this variable
                            ret.invalidOperation = false;
                        }
                        COMPOSITE_REPORT_ERROR(ret,"Failed Adding input variable ",parameter1);
                    }
                }
            } else
            if (command == writeToken){
                ret.invalidOperation = !hasParameter1 || hasParameter2;
                COMPOSITE_REPORT_ERROR(ret,writeToken," without variable name");

                if (ret){
                    ret = AddOutputVariable(parameter1);
                    if (ret.invalidOperation == true){
                        COMPOSITE_REPORT_ERROR(ret,"variable ",parameter1," already registered");
                        // the error remains as we do not allow overwriting outputs
                    }
                    COMPOSITE_REPORT_ERROR(ret,"Failed Adding output variable ",parameter1);
                }
            }
        }
    }

    return ret;
}


static inline void ShowStack(CStringTool cst,bool matchOutput,const Stack<AnyType> &typeStack){
    uint32 n2scan = 4;
    if (matchOutput) {
        n2scan++;
    }
    cst.Append('[');
    ErrorManagement::ErrorType ret2;
    for(uint32 index = 0;(index < n2scan)&& ret2;index++){
        AnyType at;
        ret2 = typeStack.Peek(at,index);
        if (ret2){
            if (index > 0){
                cst.Append('|');
            }
        	at.GetFullVariableDescriptor().ToString(cst);
        	if (at.GetVariablePointer() != NULL){
        	    TypeDescriptor td;
        	    uint32 n=3,dims[3];
        	    at.GetVariableInformation(td,n,dims);
                cst.Append('(');
        	    for (uint32 ix = 0;ix < n;ix++ ){
                    if (ix > 0){
                        cst.Append(',');
                    }
                    cst.Append(dims[ix]);
        	    }
                cst.Append(')');
        	}
        }
    }
    cst.Append(']');
};


static inline ErrorManagement::ErrorType Allocate(
        RuntimeEvaluator::VariableInformation &variableInformation,
        RuntimeEvaluatorVariables & variablesMemory){

    ErrorManagement::ErrorType ret;
    CCString varName = variableInformation.GetName();
    bool hasExternalPointer = (variableInformation.GetExternalMemoryPtr() != NULL);

    // handle matrices by saving the full RuntimeEvaluatorInfo in the variable area
    if (variableInformation.IsValidMatrix()){

        // ALLOCATE memory for matrix : INTERMEDIATE NAMED OUTPUT
        // if at this stage a matrix has no storage it means it is a temporary output
        if (!hasExternalPointer){
            ret = variableInformation.AllocateMatrixMemory();
        }

        RuntimeEvaluator::DataMemoryAddress variableLocation;
        if (ret){
            ret = variablesMemory.Allocate< Matrix<float> >(variableLocation);
            COMPOSITE_REPORT_ERROR(ret,"failed to allocate ",varName);
        }

        if (ret){
            variableInformation.GetLocation() = variableLocation;

            MatrixSize ms = variableInformation.GetDimension();
            // saves the information about the variable size and position
            variablesMemory.Variable<Matrix<float> >(variableLocation).InitMatrix(reinterpret_cast<float *>(variableInformation.GetExternalMemoryPtr()),ms.numberOfRows,ms.numberOfColumns);
        }
    }

    else  // external referenced numbers
    if (variableInformation.IsValidReferencedNumber()){
        ret = variablesMemory.Allocate< void * >(variableInformation.GetLocation());
        COMPOSITE_REPORT_ERROR(ret,"failed to allocate ",varName);

        if (ret){
            // saves the information about the variable size and position
            variablesMemory.Variable<void *>(variableInformation.GetLocation())= variableInformation.GetExternalMemoryPtr();
        }
    }
    else
    if (variableInformation.IsValidNumber()){
        ret = variablesMemory.AllocateBySize(variableInformation.GetType().GetFinalTypeDescriptor().StorageSize(),variableInformation.GetLocation());
        COMPOSITE_REPORT_ERROR(ret,"failed to allocate ",varName);
    }
    else
    {
        ret.unsupportedFeature = true;
        COMPOSITE_REPORT_ERROR(ret,"variable ",varName, "does not have a numeric supported format");
    }

    return ret;
}


ErrorManagement::ErrorType RuntimeEvaluator::Compile(CCString RPNCode){
    ErrorManagement::ErrorType ret;

    // allows managing the variablesMemoryPtr;
    // deallocates any previously allocated ram
    RuntimeEvaluatorVariables variablesMemory(variablesMemoryPtr);

    // clean all the memory
    codeMemory.Clean();

    // used only for the compilation.
    // contains the TypeDescriptor for the simple variable on stack
    // contains modifiers to indicate the use of matrices
    // contains pointer to Variable area in case of matrices so that the matrix size can be obtained
    Stack<AnyType> typeStack;

    DataMemoryAddress maxDataStackSize = 0;    // max value of dataStackSize
    DataMemoryAddress dataStackSize = 0;       // current simulated value of data stack size
    uint32 dummyID = 0;  // used to give unique IDs to unnamed variables

    // compilation STEP 3: compile code
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
        bool hasParameter1 = (parameter1.GetSize()> 0);
        bool hasParameter2 = (parameter2.GetSize()> 0);

        // now analyze the command
        if (command.GetSize() > 0){
            // assign invalid value
            CodeMemoryElement code2 = TypeCharacteristics<CodeMemoryElement>::MaxValue();
            bool matchOutput = false;

            // TRAP RREAD
            // TRAP RWRITE
            //
            if (command == remoteReadToken){
            	ret.syntaxError = true;
                REPORT_ERROR(ret,"RREAD command is reserved and cannot be used");
            } else
            if (command == remoteWriteToken){
            	ret.syntaxError = true;
                REPORT_ERROR(ret,"RWRITE command is reserved and cannot be used");
            } else

            // PROCESS CAST command
            // PUSH type(parameter1) --> TypeStack
            // matchOutput = true;
            if (command == castToken){
                ret.invalidOperation = !hasParameter1 || hasParameter2;
                COMPOSITE_REPORT_ERROR(ret,command," without type name");
                if (ret){

                    // transform the type name into a TypeDescriptor
                    // check it is one of the supported types
                    VariableDescriptor vd;
                    ret = vd.FromString(parameter1);
                    COMPOSITE_REPORT_ERROR(ret,"syntax error: type ",parameter1);

                    if (ret){
                        AnyType at(vd,NULL);
                        ret.fatalError = !typeStack.Push(at);
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

                ret.invalidOperation = !hasParameter1 || hasParameter2;
                COMPOSITE_REPORT_ERROR(ret,writeToken," without variable name");

                // all named variables should be in the database
                VariableInformation *variableInformation = NULL_PTR(VariableInformation *);
                if (ret){
                    ret = FindOutputVariable(parameter1,variableInformation);
                    COMPOSITE_REPORT_ERROR(ret,"output variable ",parameter1, " not found");
                }

                //   MAKE SURE WE ARE NOT OVERWRITING
                if (ret){
                    ret.unsupportedFeature = variableInformation->IsAllocated();
                    COMPOSITE_REPORT_ERROR(ret,"trying to overwrite output variable ",parameter1);
                }

                //
                //       ASSIGN TYPE TO UNTYPED OUTPUT VARIABLES
                // untyped intermediate results will be given a type during next compilation steps
                bool vdIsVoid           = true;
                if (ret){
                    ret = variableInformation->IsValidVoid(vdIsVoid);
                    COMPOSITE_REPORT_ERROR(ret,"untyped variable ", parameter1 ,"with assigned address encountered");
                }
                AnyType at;
                if (ret  && vdIsVoid){
                    ret = typeStack.Peek(at,0);
                    REPORT_ERROR(ret,"expecting source type in stack");
                }
                if (ret && vdIsVoid){
                    // assigns the type and in case of matrices gets the dimensions
                    ret = variableInformation->SetType(at);
                    COMPOSITE_REPORT_ERROR(ret,"SetType error", parameter1);
                }

                // ALLOCATE VARIABLES
                // we cannot overwrite so they are not allocated at this stage
                // earlier this condition was checked
                if (ret){
                    if (variableInformation->IsValidReferencedNumber()){
                        command = remoteWriteToken;
                    }

                    ret = Allocate(*variableInformation,variablesMemory);
                }

                if (ret){
                    AnyType at(variableInformation->GetType(),variablesMemory.Address<void>(variableInformation->GetLocation()));

                	// to specify the output Type
                    ret = typeStack.Push(at);
                    REPORT_ERROR(ret,"failed to push type into stack");
                }

                if (ret){
                    matchOutput = true;
                    code2 = variableInformation->GetLocation();
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
                bool isAlreadyAllocated = false;
                ret.invalidOperation = !hasParameter1 || hasParameter2;
                COMPOSITE_REPORT_ERROR(ret,readToken," without variable name");

                VariableInformation *variableInformation = NULL_PTR(VariableInformation *);

                // GET SPECIFIED VARIABLE
                // USE OUTPUT VERSION IF EXIST AND ALREADY ALLOCATED
                if (ret){
                    // try find an output variable with this name
                    ret = FindOutputVariable(parameter1,variableInformation);
                    if (ret){
                        // not set yet - cannot use
                        ret.notCompleted = !variableInformation->IsAllocated();
                    }
                    // try to see if there is an input variable
                    if (!ret){
                        ret = FindInputVariable(parameter1,variableInformation);
                        COMPOSITE_REPORT_ERROR(ret,"input variable ",parameter1, " not found");
                    }
                }

                if (ret){
                    // USE RREAD FOR NUMERIC REFERENCED
                    if (variableInformation->IsValidReferencedNumber()){
                        command = remoteReadToken;
                    }
                    isAlreadyAllocated = variableInformation->IsAllocated();
                }

                // ALLOCATE VARIABLE IF NOT ALREADY
                if (ret && !isAlreadyAllocated){
                    ret = Allocate(*variableInformation,variablesMemory);
                }

                if (ret){

                    AnyType at(variableInformation->GetType(),variablesMemory.Address<void>(variableInformation->GetLocation()));
                    // to specify the input Type
                    ret.fatalError = !typeStack.Push(at);
                    REPORT_ERROR(ret,"failed to push type into stack");
                }

                if (ret){
                    matchOutput = true;
                    code2 = variableInformation->GetLocation();
                }

            } else

            // PROCESS CONST command
            // PUSH type(parameter1) --> TypeStack
            // matchOutput = true;
            // create AnyType and convert constant from string to variable memory
            // assign code2 to address of constant
            // command = CONST
            if (command == constToken){

                ret.invalidOperation = !hasParameter1 || !hasParameter2;
                COMPOSITE_REPORT_ERROR(ret,constToken," without type name and value");

                // FROM HERE SUPPORT ALL TYPES
                VariableDescriptor vd;
                if (ret){
                    ret = vd.FromString(parameter1);
                    COMPOSITE_REPORT_ERROR(ret,"type ",parameter1, " is not a know format");
                }

                DynamicCString constantName;
                if (ret){
                    constantName().Append("Constant").Append('@').Append(dummyID++);
                    ret = AddInputVariable(constantName,vd);
                    COMPOSITE_REPORT_ERROR(ret,"failed creating ", constantName);
                }

                VariableInformation * variableInformation;

                if (ret){
                    ret = FindInputVariable(constantName,variableInformation);
                    COMPOSITE_REPORT_ERROR(ret,constantName, " not found!");
                }

                if (ret){
                    ret = Allocate(*variableInformation,variablesMemory);
                }

                // FROM HERE ONLY SUPPORT LOADING SCALAR TYPES

                if (ret){
                    ret.unsupportedFeature = !vd.GetSummaryTypeDescriptor().IsNumericType();
                    COMPOSITE_REPORT_ERROR(ret,"type ",parameter1, " is not a numeric supported format");
                }

                uint32 storageSize = 0;
                if (ret){
                    storageSize = vd.GetSummaryTypeDescriptor().StorageSize();
                    ret.unsupportedFeature = (storageSize == 0);
                    COMPOSITE_REPORT_ERROR(ret,"type ",parameter1, " has 0 storgaSize");
                }

                // convert string to number and save value into memory
                if (ret){
                    //nextConstantAddress
                    AnyType src(parameter2);
                    AnyType dest(vd,&variablesMemory.Variable<uint8>(variableInformation->GetLocation()));
                    ret = src.CopyTo(dest);
                    COMPOSITE_REPORT_ERROR(ret,"Assigning ", parameter2, "to a variable of type ", parameter1," failed");
                }

                if (ret){
                    AnyType at(variableInformation->GetType(),variablesMemory.Address<void>(variableInformation->GetLocation()));

                    // to specify the input Type
                    ret.fatalError = !typeStack.Push(at);
                    REPORT_ERROR(ret,"failed to push type into stack");
                }

                if (ret){
                    matchOutput = true;
                    code2 = variableInformation->GetLocation();
                    // the actual command is a READ from the constant area
                    command = readToken;
                }
            }

            CodeMemoryElement code = InvalidCodeMemoryElement;
            /**
             * FIND A MATCHING PCODE
             */
            DynamicCString typeList;
            if (ret){
                // prepare a description of the type and stack to have a nice reporting of error
                ShowStack(typeList(),matchOutput,typeStack);
                ret.unsupportedFeature = !FindPCode(code,command,typeStack,matchOutput);
                COMPOSITE_REPORT_ERROR(ret,"command ",command, "(",typeList,") not found");
#if 1
printf("%s:\n stack before search: %s,size = %i\n",command.GetList(),typeList.GetList(),dataStackSize);fflush(stdout);
typeList = "";
#endif
            }

            if (ret){
                ret.internalSetupError = (code >= availableFunctions);
                COMPOSITE_REPORT_ERROR(ret,"invalid code ",code,"selected");
            }


            List<VariableInformation> intermediateResultVariablesdb;
            /**
             * UPDATE TYPE STACK
             * this call may be calling PCODE specialised update function
             */
            if (ret){
            	ret = functionRecords[code].UpdateStack(typeStack,dataStackSize,intermediateResultVariablesdb,matchOutput,dummyID);
#if 1
typeList = "";
ShowStack(typeList(),matchOutput,typeStack);
printf("stack after update stack: %s,size = %i\n",typeList.GetList(),dataStackSize);fflush(stdout);
#endif
            }

            // COMPILE
            if (ret){
                // update stackSize
                if (dataStackSize > maxDataStackSize){
                    maxDataStackSize = dataStackSize;
                }

                ret.fatalError = !codeMemory.Add(code);
                REPORT_ERROR(ret,"failed to add instruction to code");

                if (ret && (code2 != InvalidCodeMemoryElement)){
                    ret.fatalError = !codeMemory.Add(code2);
                    REPORT_ERROR(ret,"failed to add instruction to code");
                }
            }

            /**
             * HANDLE TEMPORARY MATRIX VARIABLES
             */
            while (ret && (intermediateResultVariablesdb.Size() > 0)){
            	VariableInformation variableInformation;
            	ret = intermediateResultVariablesdb.Remove(variableInformation);
                REPORT_ERROR(ret,"intermediateResultVariablesdb.Remove failed ");

            	// ALLOCATE new variables
                if (ret){
                    ret = Allocate(variableInformation,variablesMemory);
                    REPORT_ERROR(ret,"Allocate failed ");
                }

                // ADD ADDITIONAL PCODE INFO
                if (ret){
                    ret.fatalError = !codeMemory.Add(variableInformation.GetLocation());
                    REPORT_ERROR(ret,"failed to add instruction to code");
                }

                if (ret){
                	ret = outputVariableInfo.Insert(variableInformation);
                    REPORT_ERROR(ret,"failed to add new output variable");
                }

                if (ret){
                    AnyType at(variableInformation.GetType(),reinterpret_cast<const void *>(&Variable<Matrix<float> >(variableInformation.GetLocation())));
                    ret = typeStack.Push(at);
                    REPORT_ERROR(ret,"error adding result type to stack");
                }
            }
#if 0
DynamicCString typeList;
ShowStack(typeList(),matchOutput,typeStack);
printf("stack after allocation: %s\n",typeList.GetList());fflush(stdout);
#endif
        }
    }

    // final checks
    if (ret){

        // assign the code pointer
        codeMemoryPtr = codeMemory.GetAllocatedMemoryConst();

        // size the stack
printf("maxDataStackSize = %i\n",maxDataStackSize);
        stack.SetSize(maxDataStackSize);
        stackPtr = static_cast<DataMemoryElement*>(stack.GetDataPointer());

    }

    // check that the TypeStack is empty
    if (ret){
        ret.internalSetupError = (typeStack.Size() > 0);
        COMPOSITE_REPORT_ERROR(ret,"operation sequence is incomplete: ",typeStack.Size()," data left in stack");
    }

    if (ret){
        sizeOfVariablesMemory = variablesMemory.GetUsedSize();
    }


    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::FunctionRecordInputs2String(RuntimeEvaluatorFunction &functionInformation,CStringTool &cst,bool peekOnly,bool showData,bool showTypes){
    ErrorManagement::ErrorType ret;

    const CodeMemoryElement *saveCodeMemoryPtr = codeMemoryPtr;

    if ((functionInformation.name == writeToken) || (functionInformation.name == remoteWriteToken)){
        CodeMemoryElement pCode2 = GetPseudoCode();

        VariableInformation *variableInformation;
        ret = FindVariable(pCode2,variableInformation);
        COMPOSITE_REPORT_ERROR(ret,"No variable or constant @ ",pCode2);

        if (ret){
           cst.Append(' ').Append(variableInformation->GetName());
        }
    }

    DataMemoryAddress dataStackIndex = 0;

    if (showData || showTypes){
        for(uint32 i=0;(i<functionInformation.numberOfInputs) && ret;i++){
            VariableDescriptor vd = functionInformation.types[i];

            if (i!=0) {
                cst.Append(',');
            } else {
                cst.Append('(');
            }
            if (showData && showTypes){
                cst.Append('(');
            }
            if (showTypes){
                 ret.fatalError = !vd.ToString(cst);
                 REPORT_ERROR(ret,"failed converting type 2 string");
            }
            if (showData && showTypes){
                cst.Append(')');
            }
            if (showData){
                DynamicCString value;
                AnyType dest(value);
                AnyType src;
                // complex variables
                if (CCString(vd.GetModifiers()).GetSize() != 0){
                    dataStackIndex += ByteSizeToDataMemorySize(sizeof(RuntimeEvaluator::DataMemoryAddress));
                    DataMemoryElement *pY = stackPtr- dataStackIndex;
                    DataMemoryAddress  y1 = pY[0];
                    const void * p = (const void *)&Variable<uint8>(y1);
                    src = AnyType (vd,p);
                 } else // scalars
                {
                    dataStackIndex += ByteSizeToDataMemorySize(vd.GetSummaryTypeDescriptor().StorageSize());
                    src = AnyType (vd,stackPtr - dataStackIndex);
                }
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
         codeMemoryPtr     = saveCodeMemoryPtr;
     }

     return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::FunctionRecordOutputs2String(RuntimeEvaluatorFunction &functionInformation,CStringTool &cst,bool lookBack,bool showData,bool showTypes){
    ErrorManagement::ErrorType ret;

    // if already showing the types do not show the parameter of the CAST
    if ((functionInformation.name == castToken) && (!showTypes)) {
        cst.Append(' ');
        ret.fatalError = !functionInformation.types[functionInformation.numberOfInputs].ToString(cst);
    } else
    if ((functionInformation.name == readToken)||(functionInformation.name == remoteReadToken)) {
        CodeMemoryElement pCode2;
        if (lookBack){
            pCode2 = codeMemoryPtr[-1];
        } else {
            pCode2 = GetPseudoCode();
        }

        VariableInformation *variableInformation;
        ret = FindVariable(pCode2,variableInformation);
        COMPOSITE_REPORT_ERROR(ret,"No variable or constant @ ",pCode2);

        if (ret){
            CCString varName = variableInformation->GetName();

            if (varName.CompareContent("Constant@",9)==0){

                cst.Append(' ');
                ret.fatalError = !variableInformation->GetType().ToString(cst);
                cst.Append(' ');
                // Converts the value to a string
                DynamicCString value;
                AnyType dest(value);
                AnyType src(variableInformation->GetType(),&Variable<uint8>(pCode2));
                ret = src.CopyTo(dest);
                REPORT_ERROR(ret,"failed converting 2 string");
                if (ret){
                    cst.Append(value);
                }
            } else {
                cst.Append(' ').Append(varName);
            }
        }
    }

    DataMemoryAddress dataStackIndex = 0;

    if (showData || showTypes){
        for(uint32 i=0;(i<functionInformation.numberOfOutputs) && ret;i++){
            VariableDescriptor vd = functionInformation.types[i+functionInformation.numberOfInputs];
            if (i!=0) {
                cst.Append(',');
            } else {
                cst.Append(" => (");
            }
            if (showData && showTypes){
                cst.Append('(');
            }
            if (showTypes){
                ret.fatalError = !vd.ToString(cst);
            }
            if (showData && showTypes){
                cst.Append(')');
            }
            if (showData){
                DynamicCString value;
                AnyType dest(value);
                AnyType src;
                // complex variables
                if (CCString(vd.GetModifiers()).GetSize() != 0){
                    dataStackIndex += ByteSizeToDataMemorySize(sizeof(RuntimeEvaluator::DataMemoryAddress));
                    DataMemoryElement *pY = stackPtr- dataStackIndex;
                    DataMemoryAddress  y1 = pY[0];
                    const void * p = (const void *)&Variable<uint8>(y1);
                    src = AnyType (vd,p);
                }
                else // scalars
                {
                    dataStackIndex += ByteSizeToDataMemorySize(vd.GetSummaryTypeDescriptor().StorageSize());
                    src = AnyType (vd,stackPtr - dataStackIndex);
                }
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

ErrorManagement::ErrorType RuntimeEvaluator::Execute(executionMode mode,StreamI *debugStream){

    stackPtr = static_cast<DataMemoryElement*>(stack.GetDataPointer());

    codeMemoryPtr = codeMemory.GetAllocatedMemoryConst();
    CodeMemoryAddress codeMaxIndex  = static_cast<CodeMemoryAddress>(codeMemory.GetSize());

    const CodeMemoryElement *codeMemoryMaxPtr = codeMemoryPtr + codeMaxIndex;

    runtimeError = ErrorManagement::ErrorType(true);

    switch (mode){
    case fastMode:{
        while(codeMemoryPtr < codeMemoryMaxPtr){
            CodeMemoryElement pCode = GetPseudoCode();
            functionRecords[pCode].executeFunction(*this);
        }
    }break;
    case safeMode:{
        DataMemoryElement *stackMinPtr = stackPtr;
        DataMemoryElement *stackMaxPtr = stackPtr + stack.GetNumberOfElements();
        while ((codeMemoryPtr < codeMemoryMaxPtr) && (runtimeError)){
            CodeMemoryElement pCode = GetPseudoCode();
            functionRecords[pCode].executeFunction(*this);
            // note that the syackPtr will reach the max value - as it points to the next value to write
            runtimeError.outOfRange = ((stackPtr > stackMaxPtr) ||  (stackPtr < stackMinPtr));
            COMPOSITE_REPORT_ERROR(runtimeError,"stack over/under flow ", (int64)(stackPtr-stackMinPtr), " [0 - ", (int64)(stackMaxPtr- stackMinPtr), "]");
        }
        runtimeError.notCompleted = runtimeError.notCompleted || (codeMemoryPtr < codeMemoryMaxPtr);
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

            cst.Append("[line]-[stackPtr]-[codePtr]::[CODE] stack-in => stack-out\n");
            int32 lineCounter = 1;
            while ((codeMemoryPtr < codeMemoryMaxPtr) && (runtimeError)){
                int64 stackOffset = stackPtr - static_cast<DataMemoryElement*>(stack.GetDataPointer());
                int64 codeOffset  = codeMemoryPtr - codeMemory.GetAllocatedMemoryConst();
                cst.Append(lineCounter).Append(" - ").Append(stackOffset).Append(" - ").Append(codeOffset).Append(" :: ");

                CodeMemoryElement pCode = GetPseudoCode();

                RuntimeEvaluatorFunction &fr = functionRecords[pCode];

                // show update info
                 cst.Append(fr.name).Append(' ');

                 // errors due to debugging
                 ErrorManagement::ErrorType ret;

                 // show inputs
                ret = FunctionRecordInputs2String(fr,cst,true,true,true);
                REPORT_ERROR(ret,"analysing input side of function call");

                // executes code
                fr.executeFunction(*this);

                if (ret){
                    // show outputs
                    ret = FunctionRecordOutputs2String(fr,cst,true,true,true);
                    REPORT_ERROR(ret,"analysing input side of function call");
                }

                if (!runtimeError){
                    cst.Append(" <ERROR> ");
                }

                cst.Append('\n');

                uint32 size = debugMessage.GetSize();
                debugStream->Write(debugMessage.GetList(),size);

                // reset line
                cst.SetSize(0);
                lineCounter++;

            }
            if (runtimeError){
                int64 stackOffset = stackPtr - static_cast<DataMemoryElement*>(stack.GetDataPointer());
                int64 codeOffset  = codeMemoryPtr - codeMemory.GetAllocatedMemoryConst();
                cst.Append(stackOffset).Append(" - ").Append(codeOffset).Append(" :: END");

                uint32 size = debugMessage.GetSize();
                debugStream->Write(debugMessage.GetList(),size);
            }
        }
    }
    }// case
    REPORT_ERROR(runtimeError,"Execution error");

    if (stackPtr  != static_cast<DataMemoryElement*>(stack.GetDataPointer())){
        runtimeError.internalSetupError = true;
        int64 offset = stackPtr - static_cast<DataMemoryElement*>(stack.GetDataPointer());
        COMPOSITE_REPORT_ERROR(runtimeError,"stack pointer not back to origin : ",offset, " elements left");
    }

    return runtimeError;
}


ErrorManagement::ErrorType RuntimeEvaluator::DeCompile(DynamicCString &RPNCode,bool showTypes){
    ErrorManagement::ErrorType ret ;

    codeMemoryPtr = codeMemory.GetAllocatedMemoryConst();
    CodeMemoryAddress codeMaxIndex  = static_cast<CodeMemoryAddress>(codeMemory.GetSize());
    const CodeMemoryElement *codeMemoryMaxPtr = codeMemoryPtr + codeMaxIndex;

    CStringTool cst = RPNCode();

    while((codeMemoryPtr < codeMemoryMaxPtr) && ret){
        CodeMemoryElement pCode = GetPseudoCode();
        RuntimeEvaluatorFunction &fr = functionRecords[pCode];

        VariableInformation *variableInformation = NULL_PTR(VariableInformation *);

        bool isRead = (fr.name == readToken);
        bool isRRead = (fr.name == remoteReadToken);
        bool isRWrite = (fr.name == remoteWriteToken);
        bool isConstant = false;
        if (isRead) {
            ret = FindVariable(codeMemoryPtr[0],variableInformation);
            if (ret){
                isConstant = (variableInformation->GetName().CompareContent("Constant@",9)==0);
            }
        }

        if (isConstant){
            cst.Append(constToken);
        } else
        if (isRRead) {
            cst.Append(readToken);
        } else
        if (isRWrite) {
            cst.Append(writeToken);
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

} //MARTe
