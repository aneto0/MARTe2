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
#include <Private/RuntimeEvaluatorFunctions.h>
#include <RuntimeEvaluator.h>
#include <stdio.h>

#include "Stack.h"
#include "StaticStack.h"
#include "AnyType.h"

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

/**
 * allows searching for a variable with a given name
 */
class VariableFinder: public GenericIterator<RuntimeEvaluatorInfo::VariableInformation>{
public:
    /**
     *
     */
    VariableFinder(CCString name);
    /**
     *
     */
    VariableFinder(RuntimeEvaluatorInfo::DataMemoryAddress address);
    /**
     *
     */
    virtual IteratorAction Do(RuntimeEvaluatorInfo::VariableInformation &data,uint32 depth=0);
    /**
     *
     */
    RuntimeEvaluatorInfo::VariableInformation *variable;
private:
    /**
     *
     */
    DynamicCString variableName;

    /**
     *
     */
    RuntimeEvaluatorInfo::DataMemoryAddress variableAddress;

};

VariableFinder::VariableFinder(CCString name){
    variable = NULL_PTR(RuntimeEvaluatorInfo::VariableInformation*);
    variableName = name;
    variableAddress = MAXDataMemoryAddress;
}

VariableFinder::VariableFinder(RuntimeEvaluatorInfo::DataMemoryAddress address){
    variable = NULL_PTR(RuntimeEvaluatorInfo::VariableInformation*);
    variableAddress = address;
}


IteratorAction VariableFinder::Do(RuntimeEvaluatorInfo::VariableInformation &data,uint32 depth){
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

RuntimeEvaluator::RuntimeEvaluator(){
    variablesMemoryPtr = NULL_PTR(RuntimeEvaluatorInfo::DataMemoryElement*);
    codeMemoryPtr = NULL_PTR(RuntimeEvaluatorInfo::CodeMemoryElement*);
    stackPtr = NULL_PTR(RuntimeEvaluatorInfo::DataMemoryElement*);
    startOfVariables = 0;

}

RuntimeEvaluator::~RuntimeEvaluator(){
}

ErrorManagement::ErrorType RuntimeEvaluator::FindVariableinDB(CCString name,RuntimeEvaluatorInfo::VariableInformation *&variableInformation,List<RuntimeEvaluatorInfo::VariableInformation> &db){
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

ErrorManagement::ErrorType RuntimeEvaluator::AddVariable2DB(CCString name,List<RuntimeEvaluatorInfo::VariableInformation> &db,VariableDescriptor vd,RuntimeEvaluatorInfo::DataMemoryAddress location){
    ErrorManagement::ErrorType 					ret;
    RuntimeEvaluatorInfo::VariableInformation *	variableInfo;

    ret = FindVariableinDB(name,variableInfo,db);

    // if it is already there we do not need to add
    if (ret.unsupportedFeature){
        RuntimeEvaluatorInfo::VariableInformation variableInfo;
        variableInfo.name 		= name;
        variableInfo.type 		= vd;
        variableInfo.location 	= location;
        ret = db.Insert(variableInfo );
    } else {

        // it would be an error if this is an output variable
        // as we do not allow to override an output
        ret.invalidOperation 	= true;
    }

    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::FindVariable(RuntimeEvaluatorInfo::DataMemoryAddress address,RuntimeEvaluatorInfo::VariableInformation *&variableInformation){
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

ErrorManagement::ErrorType RuntimeEvaluator::BrowseInputVariable(uint32 index,RuntimeEvaluatorInfo::VariableInformation *&variableInformation){
    ErrorManagement::ErrorType ret;
    variableInformation = inputVariableInfo[index];
    ret.outOfRange = (variableInformation == NULL);
    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::BrowseOutputVariable(uint32 index,RuntimeEvaluatorInfo::VariableInformation *&variableInformation){
    ErrorManagement::ErrorType ret;
    variableInformation = outputVariableInfo[index];
    ret.outOfRange = (variableInformation == NULL);
    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::ExtractVariables(CCString RPNCode){
    ErrorManagement::ErrorType ret;

    RuntimeEvaluatorInfo::DataMemoryAddress nextConstantAddress = 0;

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
                    RuntimeEvaluatorInfo::VariableInformation *variableInformation = NULL_PTR(RuntimeEvaluatorInfo::VariableInformation *);
                    ret = FindOutputVariable(parameter1,variableInformation);

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
            } else
            if (command == constToken){
                ret.invalidOperation = !hasParameter1 || !hasParameter2;
                COMPOSITE_REPORT_ERROR(ret,constToken," without type name");

                // transform the type name into a TypeDescriptor
                // check it is one of the supported types
//                TypeDescriptor td;
                VariableDescriptor vd;
                if (ret){
                    vd = TypeDescriptor(parameter1);
                    ret.unsupportedFeature = !vd.GetSummaryTypeDescriptor().IsNumericType();
                    COMPOSITE_REPORT_ERROR(ret,"type ",parameter1, " is not a numeric supported format");
                }
                // if supported add up the memory needs
                if (ret){
                    DynamicCString constantName;
                    constantName().Append("Constant").Append('@').Append(nextConstantAddress);
                    ret = AddInputVariable(constantName,vd,nextConstantAddress);
                }
                if (ret){
                    uint32 size = vd.GetSummaryTypeDescriptor().StorageSize();
                    ret.unsupportedFeature = (size == 0);
                    COMPOSITE_REPORT_ERROR(ret,"type ",parameter1, " has 0 storgaSize");

                    nextConstantAddress += ByteSizeToDataMemorySize(size);
                }
            }
        }
    }

    if (ret){
        startOfVariables = nextConstantAddress;
    }

    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::Compile(CCString RPNCode){
    ErrorManagement::ErrorType ret;

    RuntimeEvaluatorInfo::DataMemoryAddress nextVariableAddress = startOfVariables;
    // check that all variables have a type and allocate variables + constants

    {
    uint32 index = 0;
    RuntimeEvaluatorInfo::VariableInformation *var;
    while(BrowseInputVariable(index,var) && ret){
        VariableDescriptor vd = var->type;
    	TypeDescriptor td = vd.GetSummaryTypeDescriptor();
        ret.unsupportedFeature = !td.IsNumericType();
        COMPOSITE_REPORT_ERROR(ret,"input variable ",var->name," has incompatible non-numeric type ");

        // skip constants are already allocated
        if (ret && (var->location == MAXDataMemoryAddress)){
            var->location = nextVariableAddress;
            if (var->externalLocation == NULL){
                // if NULL variable is in DataMemory
                nextVariableAddress += ByteSizeToDataMemorySize(td.StorageSize());
            } else {
            	// variable address is in DataMemory
                nextVariableAddress += ByteSizeToDataMemorySize(sizeof (void *));
            }
        }
        index++;
    }

    index = 0;
    while(BrowseOutputVariable(index,var) && ret){
    	// local variable reserve 8 bytes for it
    	if (var->type.SameAs(VoidType) && (var->externalLocation == NULL)){
            var->location = nextVariableAddress;
            nextVariableAddress += ByteSizeToDataMemorySize(sizeof(double));
    	} else {
            VariableDescriptor vd = var->type;
        	TypeDescriptor td = vd.GetSummaryTypeDescriptor();
            ret.unsupportedFeature = !td.IsNumericType();
            COMPOSITE_REPORT_ERROR(ret,"output variable ",var->name," has incompatible non-numeric type ");

            if (ret){
                var->location = nextVariableAddress;
                if (var->externalLocation == NULL){
                    // if NULL variable is in DataMemory
                    nextVariableAddress += ByteSizeToDataMemorySize(td.StorageSize());
                } else {
                	// variable address is in DataMemory
                    nextVariableAddress += ByteSizeToDataMemorySize(sizeof (void *));
                }
            }
    	}
        index++;
    }
    }

    // already
    variablesMemory.SetSize(nextVariableAddress);
    variablesMemoryPtr = static_cast<RuntimeEvaluatorInfo::DataMemoryElement *>(variablesMemory.GetDataPointer());

    // initialise compilation memory
//    StaticStack<TypeDescriptor,32> typeStack;
    Stack<VariableDescriptor> typeStack;
    RuntimeEvaluatorInfo::DataMemoryAddress maxDataStackSize = 0;    // max value of dataStackSize
    RuntimeEvaluatorInfo::DataMemoryAddress dataStackSize = 0;       // current simulated value of data stack size
//    startOfVariables = 0;                      // for now no constants - so variables start at 0
    RuntimeEvaluatorInfo::DataMemoryAddress nextConstantAddress = 0; // pointer to next constant memory area

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
        bool hasParameter1 = (parameter1.GetSize()> 0);
        bool hasParameter2 = (parameter2.GetSize()> 0);


        // now analyze the command
        if (command.GetSize() > 0){
            // assign invalid value
            RuntimeEvaluatorInfo::CodeMemoryElement code2 = TypeCharacteristics<RuntimeEvaluatorInfo::CodeMemoryElement>::MaxValue();
            bool matchOutput = false;

            if (command == remoteReadToken){
            	ret.syntaxError = true;
                REPORT_ERROR(ret,"RRRITE command is reserved and cannot be used");
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
                    TypeDescriptor td;
                    td = TypeDescriptor(parameter1);
                    ret.unsupportedFeature = !td.IsNumericType();
                    COMPOSITE_REPORT_ERROR(ret,"type ",parameter1, " is not a numeric supported format");

                    if (ret){
                    	VariableDescriptor vd(td);
                        ret.fatalError = !typeStack.Push(vd);
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

                RuntimeEvaluatorInfo::VariableInformation *variableInformation = NULL_PTR(RuntimeEvaluatorInfo::VariableInformation *);
                if (ret){
                    ret = FindOutputVariable(parameter1,variableInformation);
                    COMPOSITE_REPORT_ERROR(ret,"output variable ",parameter1, " not found");
                }

                if (ret){
                    // change from WRITE to RWRITE
                    if (variableInformation->externalLocation != NULL){
                    	command = remoteWriteToken;
                        // save address now
                        Variable<void *>(variableInformation->location)= variableInformation->externalLocation;
                    }
                }

                VariableDescriptor vd;
                if (ret){
                    vd = variableInformation->type;

                    // found local variable - allocate and give type here
                	if (vd.SameAs(VariableDescriptor(VoidType)) && (variableInformation->externalLocation == NULL)){
                		ret = typeStack.Peek(vd,0);
                        REPORT_ERROR(ret,"expecting source type in stack");
                        variableInformation->type = vd;
                	} else {
                        ret.unsupportedFeature = !vd.GetSummaryTypeDescriptor().IsNumericType();
                        COMPOSITE_REPORT_ERROR(ret,"variable ",parameter1, "does not have a numeric supported format");
                	}
                }

                if (ret){
                    ret = typeStack.Push(vd);
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
                ret.invalidOperation = !hasParameter1 || hasParameter2;
                COMPOSITE_REPORT_ERROR(ret,readToken," without variable name");

                RuntimeEvaluatorInfo::VariableInformation *variableInformation = NULL_PTR(RuntimeEvaluatorInfo::VariableInformation *);

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

                if (ret){
                    /// change from READ to RREAD
                    if (variableInformation->externalLocation != NULL){
                    	command = remoteReadToken;
                        // save address now
                        Variable<void *>(variableInformation->location)= variableInformation->externalLocation;
                    }
                }

                VariableDescriptor vd;
                if (ret){
                    vd = variableInformation->type;
                    TypeDescriptor td;
                    td = vd.GetSummaryTypeDescriptor();
                    ret.unsupportedFeature = !td.IsNumericType();
                    COMPOSITE_REPORT_ERROR(ret,"variable ",parameter1, "does not have a numeric supported format");
                }
                if (ret){
                    ret.fatalError = !typeStack.Push(vd);
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
            // command = CONST
            if (command == constToken){
//                bool hasParameter2 = (parameter2.GetSize()> 0);

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
                	VariableDescriptor vd(td);
                    ret.fatalError = !typeStack.Push(VariableDescriptor(vd));
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

            RuntimeEvaluatorInfo::CodeMemoryElement code = InvalidCode;
            if (ret){
                // prepare a description of the type and stack to have a nice reporting of error
                DynamicCString typeList;
                {
                    CStringTool cst = typeList();
                    uint32 n2scan = 2;
                    if (matchOutput) {
                        n2scan++;
                    }
                    cst.Append('[');
                    ErrorManagement::ErrorType ret2;
                    for(uint32 index = 0;(index < n2scan)&& ret2;index++){
                        VariableDescriptor vd;
                        ret2 = typeStack.Peek(vd,index);
                        if (ret){
                            if (index > 0){
                                cst.Append('|');
                            }
                        	vd.ToString(cst);
                        }
                    }
                    cst.Append(']');
                }

            	ret.unsupportedFeature = !FindPCodeAndUpdateTypeStack(code,command,typeStack,matchOutput,dataStackSize);
//                COMPOSITE_REPORT_ERROR(ErrorManagement::UnsupportedFeature,"command ",command, "(",typeList,") not found");
                COMPOSITE_REPORT_ERROR(ret,"command ",command, "(",typeList,") not found");

            }

            // finally compile!
            if (ret){
                // update stackSize
                if (dataStackSize > maxDataStackSize){
                    maxDataStackSize = dataStackSize;
                }

                ret.fatalError = !codeMemory.Add(code);
                REPORT_ERROR(ret,"failed to add instruction to code");

                if (ret && (code2 != TypeCharacteristics<RuntimeEvaluatorInfo::CodeMemoryElement>::MaxValue())){
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
        stackPtr = static_cast<RuntimeEvaluatorInfo::DataMemoryElement*>(stack.GetDataPointer());

        variablesMemoryPtr = static_cast<RuntimeEvaluatorInfo::DataMemoryElement *>(variablesMemory.GetDataPointer());
    }

    // check that the TypeStack is empty
    if (ret){
        ret.internalSetupError = (typeStack.Size() > 0);
        COMPOSITE_REPORT_ERROR(ret,"operation sequence is incomplete: ",typeStack.Size()," data left in stack");
    }

    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::FunctionRecordInputs2String(RuntimeEvaluatorFunction &functionInformation,CStringTool &cst,bool peekOnly,bool showData,bool showTypes){
     ErrorManagement::ErrorType ret;

     const RuntimeEvaluatorInfo::CodeMemoryElement *saveCodeMemoryPtr = codeMemoryPtr;

     if ((functionInformation.name == writeToken) || (functionInformation.name == remoteWriteToken)){
         RuntimeEvaluatorInfo::CodeMemoryElement pCode2 = GetPseudoCode();

         RuntimeEvaluatorInfo::VariableInformation *vi;
         ret = FindVariable(pCode2,vi);
         COMPOSITE_REPORT_ERROR(ret,"No variable or constant @ ",pCode2);

         if (ret){
            cst.Append(' ').Append(vi->name);
         }
     }

     RuntimeEvaluatorInfo::DataMemoryAddress dataStackIndex = 0;

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
            }
            if (showData && showTypes){
                cst.Append(')');
            }
            if (showData){

                dataStackIndex += ByteSizeToDataMemorySize(vd.GetSummaryTypeDescriptor().StorageSize());
                DynamicCString value;
                AnyType src(vd,stackPtr - dataStackIndex);
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
        RuntimeEvaluatorInfo::CodeMemoryElement pCode2;
        if (lookBack){
            pCode2 = codeMemoryPtr[-1];
        } else {
            pCode2 = GetPseudoCode();
        }

        RuntimeEvaluatorInfo::VariableInformation *vi;
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

    RuntimeEvaluatorInfo::DataMemoryAddress dataStackIndex = 0;

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
                dataStackIndex += ByteSizeToDataMemorySize(vd.GetSummaryTypeDescriptor().StorageSize());
                DynamicCString value;
                AnyType src(vd,stackPtr - dataStackIndex);
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

ErrorManagement::ErrorType RuntimeEvaluator::Execute(executionMode mode,StreamI *debugStream){

    stackPtr = static_cast<RuntimeEvaluatorInfo::DataMemoryElement*>(stack.GetDataPointer());

    codeMemoryPtr = codeMemory.GetAllocatedMemoryConst();
    RuntimeEvaluatorInfo::CodeMemoryAddress codeMaxIndex  = static_cast<RuntimeEvaluatorInfo::CodeMemoryAddress>(codeMemory.GetSize());

    const RuntimeEvaluatorInfo::CodeMemoryElement *codeMemoryMaxPtr = codeMemoryPtr + codeMaxIndex;

    variablesMemoryPtr = static_cast<RuntimeEvaluatorInfo::DataMemoryElement *>(variablesMemory.GetDataPointer());
    runtimeError = ErrorManagement::ErrorType(true);

    switch (mode){
    case fastMode:{
        while(codeMemoryPtr < codeMemoryMaxPtr){
            RuntimeEvaluatorInfo::CodeMemoryElement pCode = GetPseudoCode();
            functionRecords[pCode].ExecuteFunction(*this);
        }
    }break;
    case safeMode:{
        RuntimeEvaluatorInfo::DataMemoryElement *stackMinPtr = stackPtr;
        RuntimeEvaluatorInfo::DataMemoryElement *stackMaxPtr = stackPtr + stack.GetNumberOfElements();
        while ((codeMemoryPtr < codeMemoryMaxPtr) && (runtimeError.ErrorsCleared())){
            RuntimeEvaluatorInfo::CodeMemoryElement pCode = GetPseudoCode();
            functionRecords[pCode].ExecuteFunction(*this);
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
                int64 stackOffset = stackPtr - static_cast<RuntimeEvaluatorInfo::DataMemoryElement*>(stack.GetDataPointer());
                int64 codeOffset  = codeMemoryPtr - codeMemory.GetAllocatedMemoryConst();
                cst.Append(lineCounter).Append(" - ").Append(stackOffset).Append(" - ").Append(codeOffset).Append(" :: ");

                RuntimeEvaluatorInfo::CodeMemoryElement pCode = GetPseudoCode();

                RuntimeEvaluatorFunction &fr = functionRecords[pCode];

                // show update info
                 cst.Append(fr.name).Append(' ');

                 // errors due to debugging
                 ErrorManagement::ErrorType ret;

                 // show inputs
                ret = FunctionRecordInputs2String(fr,cst,true,true,true);
                REPORT_ERROR(ret,"analysing input side of function call");

                // executes code
                fr.ExecuteFunction(*this);

                if (ret){
                    // show outputs
                    ret = FunctionRecordOutputs2String(fr,cst,true,true,true);
                    REPORT_ERROR(ret,"analysing input side of function call");
                }

                if (!runtimeError.ErrorsCleared()){
                    cst.Append(" <ERROR> ");
                }

                cst.Append('\n');

                uint32 size = debugMessage.GetSize();
                debugStream->Write(debugMessage.GetList(),size);

                // reset line
                cst.SetSize(0);
                lineCounter++;

            }
            if (runtimeError.ErrorsCleared()){
                int64 stackOffset = stackPtr - static_cast<RuntimeEvaluatorInfo::DataMemoryElement*>(stack.GetDataPointer());
                int64 codeOffset  = codeMemoryPtr - codeMemory.GetAllocatedMemoryConst();
                cst.Append(stackOffset).Append(" - ").Append(codeOffset).Append(" :: END");

                uint32 size = debugMessage.GetSize();
                debugStream->Write(debugMessage.GetList(),size);
            }
        }
    }
    }
    REPORT_ERROR(runtimeError,"Execution error");

    if (stackPtr  != static_cast<RuntimeEvaluatorInfo::DataMemoryElement*>(stack.GetDataPointer())){
        runtimeError.internalSetupError = true;
        int64 offset = stackPtr - static_cast<RuntimeEvaluatorInfo::DataMemoryElement*>(stack.GetDataPointer());
        COMPOSITE_REPORT_ERROR(runtimeError,"stack pointer not back to origin : ",offset, " elements left");
    }

    return runtimeError;
}

ErrorManagement::ErrorType RuntimeEvaluator::DeCompile(DynamicCString &RPNCode,bool showTypes){
    ErrorManagement::ErrorType ret ;

    codeMemoryPtr = codeMemory.GetAllocatedMemoryConst();
    RuntimeEvaluatorInfo::CodeMemoryAddress codeMaxIndex  = static_cast<RuntimeEvaluatorInfo::CodeMemoryAddress>(codeMemory.GetSize());
    const RuntimeEvaluatorInfo::CodeMemoryElement *codeMemoryMaxPtr = codeMemoryPtr + codeMaxIndex;

    variablesMemoryPtr = static_cast<RuntimeEvaluatorInfo::DataMemoryElement *>(variablesMemory.GetDataPointer());

    CStringTool cst = RPNCode();

    while((codeMemoryPtr < codeMemoryMaxPtr) && ret){
        RuntimeEvaluatorInfo::CodeMemoryElement pCode = GetPseudoCode();
        RuntimeEvaluatorFunction &fr = functionRecords[pCode];

        if ((fr.name == readToken) && (codeMemoryPtr[0] < startOfVariables)){
            cst.Append(constToken);
        } else
        if (fr.name == remoteReadToken) {
            cst.Append(readToken);
        } else
        if (fr.name == remoteWriteToken) {
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
