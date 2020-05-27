/**
 * @file RuntimeEvaluator.cpp
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

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "RuntimeEvaluator.h"
#include "RuntimeEvaluatorFunctions.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

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
 * allows searching for a variable with a given name or address
 */
class VariableFinder: public IteratorT<VariableInformation>{
public:
    /**
     *
     */
    VariableFinder(CCString name) {
        variable = NULL_PTR(VariableInformation*);
        variableName = name;
        variableAddress = MAXDataMemoryAddress;
    }
    /**
     *
     */
    VariableFinder(DataMemoryAddress address) {
        variable = NULL_PTR(VariableInformation*);
        variableAddress = address;
    }
    /**
     *
     */
    void Do(VariableInformation *data) {
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

RuntimeEvaluator::RuntimeEvaluator(StreamString RPNCodeIn){
    RPNCode = RPNCodeIn;
    variablesMemoryPtr = NULL_PTR(DataMemoryElement*);
    codeMemoryPtr = NULL_PTR(CodeMemoryElement*);
    stackPtr = NULL_PTR(DataMemoryElement*);
    startOfVariables = 0;
}

RuntimeEvaluator::~RuntimeEvaluator(){
}

ErrorManagement::ErrorType RuntimeEvaluator::FindVariableinDB(CCString name,VariableInformation *&variableInformation,LinkedListHolderT<VariableInformation> &db){
    ErrorManagement::ErrorType ret;

    variableInformation = NULL;
    VariableFinder finder(name);
    db.ListIterate(&finder);
    ret = finder.error;

    if (!ret){
        REPORT_ERROR_STATIC(ret, "Iteration failed");
    } else {
        variableInformation = finder.variable;
        ret.unsupportedFeature = (variableInformation == NULL);
    }
    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::AddVariable2DB(CCString name,LinkedListHolderT<VariableInformation> &db,TypeDescriptor td,DataMemoryAddress location){
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

ErrorManagement::ErrorType RuntimeEvaluator::FindVariable(DataMemoryAddress address,VariableInformation *&variableInformation){
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

ErrorManagement::ErrorType RuntimeEvaluator::BrowseInputVariable(uint32 index,VariableInformation *&variableInformation){
    ErrorManagement::ErrorType ret;
    variableInformation = inputVariableInfo.ListPeek(index);
    ret.outOfRange = (variableInformation == NULL);
    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::BrowseOutputVariable(uint32 index,VariableInformation *&variableInformation){
    ErrorManagement::ErrorType ret;
    variableInformation = outputVariableInfo.ListPeek(index);
    ret.outOfRange = (variableInformation == NULL);
    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::ExtractVariables(){
    ErrorManagement::ErrorType ret;

    DataMemoryAddress nextConstantAddress = 0;
    
    StreamString line;
    char8 terminator;
    
    RPNCode.Seek(0);
    
    while (RPNCode.GetToken(line, "\n", terminator, "\n\r") && ret){

        // extract command and parameter
        StreamString command;
        StreamString parameter1;
        StreamString parameter2;
        StreamString parameter3;

        // extract up to two tokens per line
        line.Seek(0u);
        line.GetToken(command, " \t,", terminator," \t,");
        line.GetToken(parameter1, " \t,", terminator," \t,");
        line.GetToken(parameter2, " \t,", terminator," \t,");
        line.GetToken(parameter3, " \t,", terminator," \t,");

        bool hasParameter1 = (parameter1.Size()> 0);
        bool hasParameter2 = (parameter2.Size()> 0);
        bool hasParameter3 = (parameter3.Size()> 0);

        // now analyse the command
        if (command.Size() > 0){

            if (command == readToken){
                ret.illegalOperation = !hasParameter1 || hasParameter2;

                if (!ret){
                    REPORT_ERROR_STATIC(ret,"%s without variable name", command.Buffer());
                } else{
                    // if an output variable of this name is already present
                    // it means it would have already been loaded
                    // so no need to fetch an external input
                    VariableInformation *variableInformation;
                    ret = FindOutputVariable(parameter1.Buffer(),variableInformation);

                    if (!ret){
                        ret = AddInputVariable(parameter1.Buffer());
                        if (ret.illegalOperation == true){
                            REPORT_ERROR_STATIC(ErrorManagement::Information,"variable %s already registered", parameter1.Buffer());
                            // mask out the case that we already registered this variable
                            ret.illegalOperation = false;
                        }
                        if (!ret){
                            REPORT_ERROR_STATIC(ret,"Failed Adding input variable %s",parameter1.Buffer());
                        }
                    }
                }
            } else
            if (command == writeToken){
                ret.illegalOperation = !hasParameter1 || hasParameter2;
                if (!ret){
                    REPORT_ERROR_STATIC(ret, "%s without variable name", command.Buffer());
                }

                if (ret){
                    ret = AddOutputVariable(parameter1.Buffer());
                    if (ret.illegalOperation == true){
                        REPORT_ERROR_STATIC(ret,"variable %s already registered", parameter1.Buffer());
                        // the error remains as we do not allow overwriting outputs
                    }
                    if (!ret){
                        REPORT_ERROR_STATIC(ret,"Failed Adding output variable %s",parameter1.Buffer());
                    }
                }
            } else
            if (command == constToken){
                ret.illegalOperation = !hasParameter1 || !hasParameter2 || hasParameter3;
                if (!ret){
                    REPORT_ERROR_STATIC(ret, "%s without type name", command.Buffer());
                }

                // transform the type name into a TypeDescriptor
                // check it is one of the supported types
                TypeDescriptor td;
                if (ret){
                    td = TypeDescriptor::GetTypeDescriptorFromTypeName(parameter1.Buffer());
                    ret.unsupportedFeature = !td.IsNumericType();
                    if (!ret){
                        REPORT_ERROR_STATIC(ret,"type %s is not a numeric supported format", parameter1.Buffer());
                    }
//printf("const type = %x\n",td.all);
                }
                // if supported add up the memory needs
                if (ret){
                    StreamString constantName;
                    constantName.Printf("Constant@%u", nextConstantAddress);
                    ret = AddInputVariable(constantName.Buffer(),td,nextConstantAddress);
                }
                if (ret){
                    uint32 size = td.numberOfBits/8u;
                    ret.unsupportedFeature = (size == 0);
                    if (!ret){
                        REPORT_ERROR_STATIC(ret,"type %s has 0 storageSize", parameter1.Buffer());
                    }

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

void* RuntimeEvaluator::GetVariablesMemory() {

    return reinterpret_cast<void*>(&dataMemory[0]);
    
}

void* RuntimeEvaluator::GetInputVariableMemory(StreamString varNameIn) {
    
    void* retAddress = NULL_PTR(void*);
    bool isFound = false;
    
    int32 index = 0;
    VariableInformation *var;

    while(BrowseInputVariable(index,var) && !isFound){
        if (var->name == varNameIn){
            if (var->externalLocation != NULL) {
                retAddress = var->externalLocation;
            }
            else {
                retAddress  = reinterpret_cast<void*>(&dataMemory[var->location]);
            }
            isFound = true;
        }
        index++;
    }
    
    return retAddress;
    
}

void* RuntimeEvaluator::GetInputVariableMemory(uint32 varIndexIn) {
    
    void* retAddress = NULL_PTR(void*);
    
    VariableInformation *var;
    
    if (BrowseInputVariable(varIndexIn,var)) {
        if (var->externalLocation != NULL) {
            retAddress = var->externalLocation;
        }
        else {
            retAddress  = reinterpret_cast<void*>(&dataMemory[var->location]);
        }
    }
    
    return retAddress;
    
}

void* RuntimeEvaluator::GetOutputVariableMemory(StreamString varNameIn) {
    
    void* retAddress = NULL_PTR(void*);
    bool isFound = false;
    
    int32 index = 0;
    VariableInformation *var;

    while(BrowseOutputVariable(index,var) && !isFound){
        if (var->name == varNameIn){
            if (var->externalLocation != NULL) {
                retAddress = var->externalLocation;
            }
            else {
                retAddress  = reinterpret_cast<void*>(&dataMemory[var->location]);
            }
            isFound = true;
        }
        index++;
    }
    
    return retAddress;
    
}

void* RuntimeEvaluator::GetOutputVariableMemory(uint32 varIndexIn) {
    
    void* retAddress = NULL_PTR(void*);
    
    VariableInformation *var;
    
    if (BrowseOutputVariable(varIndexIn,var)) {
        if (var->externalLocation != NULL) {
            retAddress = var->externalLocation;
        }
        else {
            retAddress  = reinterpret_cast<void*>(&dataMemory[var->location]);
        }
    }
    
    return retAddress;
    
}

bool RuntimeEvaluator::SetInputVariableMemory(StreamString varNameIn, void* externalLocationIn) {
    
    bool isFound = false;
    
    int32 index = 0;
    VariableInformation *var;

    while(BrowseInputVariable(index,var) && !isFound){
        if (var->name == varNameIn){
            var->externalLocation = externalLocationIn;
            isFound = true;
        }
        index++;
    }
    
    return isFound;
    
}

bool RuntimeEvaluator::SetInputVariableMemory(uint32 varIndexIn, void* externalLocationIn) {
    
    bool isFound = false;
    
    VariableInformation *var;
    
    if (BrowseInputVariable(varIndexIn,var)) {
        var->externalLocation = externalLocationIn;
        isFound = true;
    }
    
    return isFound;
    
}

bool RuntimeEvaluator::SetOutputVariableMemory(StreamString varNameIn, void* externalLocationIn) {
    
    bool isFound = false;
    
    int32 index = 0;
    VariableInformation *var;

    while(BrowseOutputVariable(index,var) && !isFound){
        if (var->name == varNameIn){
            var->externalLocation = externalLocationIn;
            isFound = true;
        }
        index++;
    }
    
    return isFound;
    
}

bool RuntimeEvaluator::SetOutputVariableMemory(uint32 varIndexIn, void* externalLocationIn) {
    
    bool isFound = false;
    
    VariableInformation *var;
    
    if (BrowseOutputVariable(varIndexIn,var)) {
        var->externalLocation = externalLocationIn;
        isFound = true;
    }
    
    return isFound;
    
}

bool RuntimeEvaluator::SetInputVariableType(StreamString varNameIn, TypeDescriptor typeIn) {
    
    bool isFound = false;
    
    int32 index = 0;
    VariableInformation *var;

    while(BrowseInputVariable(index,var) && !isFound){
        if (var->name == varNameIn){
            var->type = typeIn;
            isFound = true;
        }
        index++;
    }
    
    return isFound;
    
}

bool RuntimeEvaluator::SetInputVariableType(uint32 varIndexIn, TypeDescriptor typeIn) {
    
    bool isFound = false;
    
    VariableInformation *var;
    
    if (BrowseInputVariable(varIndexIn,var)) {
        var->type = typeIn;
        isFound = true;
    }
    
    return isFound;
    
}

bool RuntimeEvaluator::SetOutputVariableType(StreamString varNameIn, TypeDescriptor typeIn) {
    
    bool isFound = false;
    
    int32 index = 0;
    VariableInformation *var;

    while(BrowseOutputVariable(index,var) && !isFound){
        if (var->name == varNameIn){
            var->type = typeIn;
            isFound = true;
        }
        index++;
    }
    
    return isFound;
    
}

bool RuntimeEvaluator::SetOutputVariableType(uint32 varIndexIn, TypeDescriptor typeIn) {
    
    bool isFound = false;
    
    VariableInformation *var;
    
    if (BrowseOutputVariable(varIndexIn,var)) {
        var->type = typeIn;
        isFound = true;
    }
    
    return isFound;
    
}

ErrorManagement::ErrorType RuntimeEvaluator::Compile(){
    ErrorManagement::ErrorType ret;

    DataMemoryAddress nextVariableAddress = startOfVariables;
    // check that all variables have a type and allocate variables + constants

    {
    uint32 index = 0;
    VariableInformation *var;
    while(BrowseInputVariable(index,var) && ret){
        ret.unsupportedFeature = !var->type.IsNumericType();
        if (!ret){
            REPORT_ERROR_STATIC(ret,"input variable %s has incompatible non-numeric type ", var->name.Buffer());
        }

        // skip constants are already allocated
        if (ret && (var->location == MAXDataMemoryAddress)){
            var->location = nextVariableAddress;
            if (var->externalLocation == NULL){
                // if NULL variable is in DataMemory
                nextVariableAddress += ByteSizeToDataMemorySize(var->type.numberOfBits/8u);
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
        if ((var->type == VoidType) && (var->externalLocation == NULL)){
            var->location = nextVariableAddress;
            nextVariableAddress += ByteSizeToDataMemorySize(sizeof(double));
        } else {
            ret.unsupportedFeature = !var->type.IsNumericType();
            if (!ret){
                REPORT_ERROR_STATIC(ret, "output variable %s has incompatible non-numeric type", var->name.Buffer());
            }

            if (ret){
                var->location = nextVariableAddress;
                if (var->externalLocation == NULL){
                    // if NULL variable is in DataMemory
                    nextVariableAddress += ByteSizeToDataMemorySize(var->type.numberOfBits/8u);
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
    dataMemory.SetSize(nextVariableAddress);
    variablesMemoryPtr = static_cast<DataMemoryElement *>(dataMemory.GetDataPointer());

    // initialise compilation memory
    StaticStack<TypeDescriptor,32> typeStack;
    DataMemoryAddress maxDataStackSize = 0;    // max value of dataStackSize
    DataMemoryAddress dataStackSize = 0;       // current simulated value of data stack size
//  startOfVariables = 0;                      // for now no constants - so variables start at 0
    DataMemoryAddress nextConstantAddress = 0; // pointer to next constant memory area

    // clean all the memory
    codeMemory.Clean();

    StreamString line;
    char8 terminator;
    
    RPNCode.Seek(0);
    while (RPNCode.GetToken(line, "\n", terminator, "\n\r") && ret){

        // extract command and parameter
        StreamString command;
        StreamString parameter1;
        StreamString parameter2;
        StreamString parameter3;

        // extract up to three tokens per line
        line.Seek(0u);
        line.GetToken(command, " \t,", terminator, " \t,");
        line.GetToken(parameter1, " \t,", terminator, " \t,");
        line.GetToken(parameter2, " \t,", terminator, " \t,");
        line.GetToken(parameter3, " \t,", terminator, " \t,");

        bool hasParameter1 = (parameter1.Size()> 0);
        bool hasParameter2 = (parameter2.Size()> 0);
        bool hasParameter3 = (parameter3.Size()> 0);

        // now analyze the command
        if (command.Size() > 0){
            // assign invalid value
            CodeMemoryElement code2 = TypeCharacteristics<CodeMemoryElement>::MaxValue();
            bool matchOutput = false;

            // PROCESS CAST command
            // PUSH type(parameter1) --> TypeStack
            // matchOutput = true;
            if (command == castToken){
                ret.illegalOperation = !hasParameter1 || hasParameter2;
                if (!ret){
                    REPORT_ERROR_STATIC(ret,"%s without type name", command.Buffer());
                } else{
                    // transform the type name into a TypeDescriptor
                    // check it is one of the supported types
                    TypeDescriptor td;
                    td = TypeDescriptor::GetTypeDescriptorFromTypeName(parameter1.Buffer());
                    ret.unsupportedFeature = !td.IsNumericType();
                    if (!ret){
                        REPORT_ERROR_STATIC(ret,"type %s is not a numeric supported format", parameter1.Buffer());
                    } else{
                        ret.fatalError = !typeStack.Push(td);
                        if (!ret){
                            REPORT_ERROR_STATIC(ret,"failed to push type into stack");
                        }
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
                VariableInformation *variableInformation = NULL_PTR(VariableInformation *);

                ret.illegalOperation = !hasParameter1 || hasParameter2;
                if (!ret){
                    REPORT_ERROR_STATIC(ret, "%s without variable name", command.Buffer());
                } else{
                    ret = FindOutputVariable(parameter1.Buffer(),variableInformation);
                    if (!ret){
                        REPORT_ERROR_STATIC(ret,"output variable %s not found", parameter1.Buffer());
                    }
                }

                if (ret){
                    // change from WRITE to RWRITE
                    if (variableInformation->externalLocation != NULL){
                        command = remoteWriteToken;
                        // save address now
                        Variable<void *>(variableInformation->location)= variableInformation->externalLocation;
                    }
                }

                TypeDescriptor td = InvalidType;
                if (ret){
                    td = variableInformation->type;

                    // found local variable - allocate and give type here
                    if ((td == VoidType) && (variableInformation->externalLocation == NULL)){
                        ret.fatalError = !typeStack.Peek(0,td);
                        if (!ret){
                            REPORT_ERROR_STATIC(ret,"expecting source type in stack");
                        }
                        variableInformation->type = td;
                    } else {
                        ret.unsupportedFeature = !td.IsNumericType();
                        if (!ret){
                            REPORT_ERROR_STATIC(ret,"variable %s does not have a numeric supported format", parameter1.Buffer());
                        }
                    }
                }

                if (ret){
//printf("Stack[%i].Push(%x) -->",typeStack.GetSize(),td.all);
                    ret.fatalError = !typeStack.Push(td);
                    if (!ret){
                        REPORT_ERROR_STATIC(ret,"failed to push type into stack");
                    }
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
                VariableInformation *variableInformation = NULL_PTR(VariableInformation *);

                ret.illegalOperation = !hasParameter1 || hasParameter2;
                if (!ret){
                    REPORT_ERROR_STATIC(ret, "%s without variable name", command.Buffer());
                } else{
                    // try find an output variable with this name
                    ret = FindOutputVariable(parameter1.Buffer(),variableInformation);
                    if (ret){
                        // not set yet - cannot use
                        ret.notCompleted = (variableInformation->variableUsed != true);
                    }
                    // try to see if there is an input variable
                    if (!ret){
                        ret = FindInputVariable(parameter1.Buffer(),variableInformation);
                        if (!ret){
                            REPORT_ERROR_STATIC(ret,"input variable %s not found", parameter1.Buffer());
                        }
                    }
                }

                if (ret){
                    // change from READ to RREAD
                    if (variableInformation->externalLocation != NULL){
                        command = remoteReadToken;
                        // save address now
                        Variable<void *>(variableInformation->location)= variableInformation->externalLocation;
                    }
                }

                TypeDescriptor td = InvalidType;
                if (ret){
                    td = variableInformation->type;
                    ret.unsupportedFeature = !td.IsNumericType();
                    if (!ret){
                        REPORT_ERROR_STATIC(ret,"variable %s does not have a numeric supported format", parameter1.Buffer());
                    }
                }
//printf("read %s type = %x  type = %x\n",variableInformation->name.GetList(),td.all,variableInformation->type.all);
                if (ret){
                    ret.fatalError = !typeStack.Push(td);
                    if (!ret){
                        REPORT_ERROR_STATIC(ret,"failed to push type into stack");
                    }
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
                TypeDescriptor td;

                ret.illegalOperation = !hasParameter1 || !hasParameter2 || hasParameter3;
                if (!ret){
                    REPORT_ERROR_STATIC(ret,"%s without type name and value", command.Buffer());
                } else{
                // transform the type name into a TypeDescriptor
                // check it is one of the supported types

                    td = TypeDescriptor::GetTypeDescriptorFromTypeName(parameter1.Buffer());
                    ret.unsupportedFeature = !td.IsNumericType();
                    if (!ret){
                        REPORT_ERROR_STATIC(ret,"type %s is not a numeric supported format", parameter1.Buffer());
                    }
                }

                // convert string to number and save value into memory
                if (ret){
                    //nextConstantAddress
                    AnyType dest(td, 0,&variablesMemoryPtr[nextConstantAddress]);
                    ret.fatalError = !TypeConvert(dest, parameter2.Buffer());
                    if (!ret){
                        REPORT_ERROR_STATIC(ret,"TypeConvert failed ");
                    }

                }

                if (ret){
                    ret.fatalError = !typeStack.Push(td);
                    if (!ret){
                        REPORT_ERROR_STATIC(ret,"failed to push type into stack");
                    }
                }

                if (ret){
                    matchOutput = true;
                    code2 = nextConstantAddress;
                    nextConstantAddress += ByteSizeToDataMemorySize(td.numberOfBits/8u);
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
                    if (!ret){
                        REPORT_ERROR_STATIC(ret,"command %s(%s) not found", command.Buffer(), typeList.Buffer());
                    }
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
                if (!ret){
                    REPORT_ERROR_STATIC(ret,"failed to add instruction to code");
                }

                if (ret && (code2 != TypeCharacteristics<CodeMemoryElement>::MaxValue())){
                    ret.fatalError = !codeMemory.Add(code2);
                    if (!ret){
                        REPORT_ERROR_STATIC(ret,"failed to add instruction to code");
                    }
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
        if (!ret){
            REPORT_ERROR_STATIC(ret,"operation sequence is incomplete: %u data left in stack", typeStack.GetSize());
        }
    }

    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::FunctionRecordInputs2String(RuntimeEvaluatorFunctions &functionInformation,StreamString &cst,bool peekOnly,bool showData,bool showTypes){
     ErrorManagement::ErrorType ret;

     const CodeMemoryElement *saveCodeMemoryPtr = codeMemoryPtr;
     StreamString functionName = functionInformation.GetName();

     if ((functionName == writeToken) || (functionName == remoteWriteToken)){
         CodeMemoryElement pCode2 = GetPseudoCode();

         VariableInformation *vi;
         ret = FindVariable(pCode2,vi);
         if (!ret){
            REPORT_ERROR_STATIC(ret,"No variable or constant @ %u",pCode2);
         } else{
            cst.Printf(" %s", vi->name.Buffer());
         }
     }

     DataMemoryAddress dataStackIndex = 0;

    if (showData || showTypes){
        Vector<TypeDescriptor> functionInputTypes = functionInformation.GetInputTypes();

        for(uint32 i=0;(i<functionInputTypes.GetNumberOfElements()) && ret;i++){
            TypeDescriptor td = functionInputTypes[i];

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
                dataStackIndex += ByteSizeToDataMemorySize(td.numberOfBits/8u);
                AnyType src(td, 0, stackPtr - dataStackIndex);
                cst.Printf("%!", src);
            }
            if (i == (functionInputTypes.GetNumberOfElements()-1U)){
                cst += ')';
            }
         }
     }

     // restore any used data
     if (peekOnly){
         codeMemoryPtr  = saveCodeMemoryPtr;
     }

     return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::FunctionRecordOutputs2String(RuntimeEvaluatorFunctions &functionInformation,StreamString &cst,bool lookBack,bool showData,bool showTypes){
    ErrorManagement::ErrorType ret;
    StreamString functionName = functionInformation.GetName();
    Vector<TypeDescriptor> functionOutputTypes = functionInformation.GetOutputTypes();

    // if already showing the types do not show the parameter of the CAST
    if ((functionName == castToken) && (!showTypes)) {
        cst += ' ';
        CCString typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(functionOutputTypes[0]);
        if (typeName != NULL) {
            cst += typeName.GetList();
        } else {
            ret.fatalError = true;
        }

    } else
    if ((functionName == readToken)||(functionName == remoteReadToken)) {
        CodeMemoryElement pCode2;
        if (lookBack){
            pCode2 = codeMemoryPtr[-1];
        } else {
            pCode2 = GetPseudoCode();
        }

        VariableInformation *vi;
        ret = FindVariable(pCode2,vi);
        if (!ret){
            REPORT_ERROR_STATIC(ret,"No variable or constant @ %u",pCode2);
        } else{
            if (pCode2 < startOfVariables){

                cst += ' ';

                CCString typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(vi->type);
                if (typeName != NULL) {
                    cst += typeName.GetList();
                } else {
                    ret.fatalError = true;
                }

                if (!ret){
                    REPORT_ERROR_STATIC(ret,"GetTypeNameFromTypeDescriptor failed ");
                } else{
                    AnyType src(vi->type, 0, &variablesMemoryPtr[pCode2]);
                    cst.Printf(" %!", src);
                }

            } else {
                cst.Printf(" %s", vi->name.Buffer());
            }
        }
    }

    DataMemoryAddress dataStackIndex = 0;

    if (showData || showTypes){

        for(uint32 i=0;(i<functionOutputTypes.GetNumberOfElements()) && ret;i++){
            TypeDescriptor td = functionOutputTypes[i];
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
                dataStackIndex += ByteSizeToDataMemorySize(td.numberOfBits/8u);
                AnyType src(td, 0, stackPtr - dataStackIndex);
                cst.Printf("%!", src);
            }
            if (i == (functionOutputTypes.GetNumberOfElements()-1U)){
                cst += ')';
            }
        }
    }

    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::Execute(executionMode mode, StreamI *debugStream){

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
            functionRecords[pCode].ExecuteFunction(*this);
        }
    }break;
    case safeMode:{
        DataMemoryElement *stackMinPtr = stackPtr;
        DataMemoryElement *stackMaxPtr = stackPtr + stack.GetNumberOfElements();
        while ((codeMemoryPtr < codeMemoryMaxPtr) && (runtimeError.ErrorsCleared())){
            CodeMemoryElement pCode = GetPseudoCode();
            functionRecords[pCode].ExecuteFunction(*this);
            // note that the stackPtr will reach the max value - as it points to the next value to write
            runtimeError.outOfRange = ((runtimeError.outOfRange) || (stackPtr > stackMaxPtr) ||  (stackPtr < stackMinPtr));
            if (!runtimeError){
                REPORT_ERROR_STATIC(runtimeError,"stack over/under flow %i [0 - %i]", (int64)(stackPtr-stackMinPtr), (int64)(stackMaxPtr- stackMinPtr));
            }
        }
        runtimeError.notCompleted = (codeMemoryPtr < codeMemoryMaxPtr);
        if (!runtimeError){
            REPORT_ERROR_STATIC(runtimeError,"code execution interrupted");
        }
    }break;
    case debugMode:
    default:{
        if (debugStream == NULL_PTR(StreamI *)){
            runtimeError.parametersError = true;
            if (!runtimeError){
                REPORT_ERROR_STATIC(runtimeError,"debugMode requested with debugStream set to NULL");
            }
        } else {
            StreamString debugMessage;

            debugMessage += "[line]-[stackPtr]-[codePtr]::[CODE] stack-in => stack-out\n";
            int32 lineCounter = 1;
            while ((codeMemoryPtr < codeMemoryMaxPtr) && (runtimeError)){
                int64 stackOffset = stackPtr - static_cast<DataMemoryElement*>(stack.GetDataPointer());
                int64 codeOffset  = codeMemoryPtr - codeMemory.GetAllocatedMemoryConst();
                debugMessage.Printf("%i - %i - %i :: ", lineCounter, stackOffset, codeOffset);

                CodeMemoryElement pCode = GetPseudoCode();

                RuntimeEvaluatorFunctions &fr = functionRecords[pCode];

                // show update info
                debugMessage.Printf("%s ", fr.GetName().Buffer());

                // errors due to debugging
                ErrorManagement::ErrorType ret;

                // show inputs
                ret = FunctionRecordInputs2String(fr,debugMessage,true,true,true);
                if (!ret){
                    REPORT_ERROR_STATIC(ret,"analysing input side of function call");
                }

                // executes code
                fr.ExecuteFunction(*this);

                if (ret){
                    // show outputs
                    ret = FunctionRecordOutputs2String(fr,debugMessage,true,true,true);
                    if (!ret){
                        REPORT_ERROR_STATIC(ret,"analysing input side of function call");
                    }
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
    if (!runtimeError){
        REPORT_ERROR_STATIC(runtimeError,"Execution error");
    }

    if (stackPtr  != static_cast<DataMemoryElement*>(stack.GetDataPointer())){
        runtimeError.internalSetupError = true;
        int64 offset = stackPtr - static_cast<DataMemoryElement*>(stack.GetDataPointer());
        if (!runtimeError){
            REPORT_ERROR_STATIC(runtimeError,"stack pointer not back to origin : %i elements left", offset);
        }
    }

    return runtimeError;
}

ErrorManagement::ErrorType RuntimeEvaluator::DeCompile(StreamString &DeCompileRPNCode, bool showTypes){
    ErrorManagement::ErrorType ret ;
    
    codeMemoryPtr = codeMemory.GetAllocatedMemoryConst();
    CodeMemoryAddress codeMaxIndex  = static_cast<CodeMemoryAddress>(codeMemory.GetSize());
    const CodeMemoryElement *codeMemoryMaxPtr = codeMemoryPtr + codeMaxIndex;

    variablesMemoryPtr = static_cast<DataMemoryElement *>(dataMemory.GetDataPointer());

    while((codeMemoryPtr < codeMemoryMaxPtr) && ret){
        CodeMemoryElement pCode = GetPseudoCode();
        RuntimeEvaluatorFunctions &fr = functionRecords[pCode];
        StreamString fName = fr.GetName();

        if ((fName == readToken) && (codeMemoryPtr[0] < startOfVariables)){
            DeCompileRPNCode += constToken;
        } else
        if (fName == remoteReadToken) {
            DeCompileRPNCode += readToken;
        } else
        if (fName == remoteWriteToken) {
            DeCompileRPNCode += writeToken;
        } else {
            DeCompileRPNCode += fName;
        }

        ret = FunctionRecordInputs2String(fr,DeCompileRPNCode,false,false,showTypes);

        if (ret){
            ret = FunctionRecordOutputs2String(fr,DeCompileRPNCode,false,false,showTypes);
        }
        DeCompileRPNCode += '\n';
    }

    return ret;
}

} //MARTe
