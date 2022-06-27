/**
 * @file RuntimeEvaluator.cpp
 * @brief Source file for class RuntimeEvaluator
 * @date 26/03/2020
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
#include "RuntimeEvaluatorFunction.h"

/*---------------------------------------------------------------------------*/
/*                          Static definitions                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
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
 * Allows searching for a variable with a given name or address
 */
class VariableFinder: public IteratorT<VariableInformation>{
public:
    /**
     *
     */
    VariableFinder(const CCString &name) {
        variable = NULL_PTR(VariableInformation*);
        variableName = name;
        variableAddress = MAXDataMemoryAddress;
    }
    /**
     *
     */
    VariableFinder(const DataMemoryAddress address) {
        variable = NULL_PTR(VariableInformation*);
        variableAddress = address;
    }
    /**
     *
     */
    virtual void Do(VariableInformation* const data) {
        if (data == NULL_PTR(VariableInformation *)) {
            error = ErrorManagement::FatalError;
        }
        else {
            if (variableName.Size() > 0ull) {
                if (data->name == variableName) {
                    variable = data;
                    error = ErrorManagement::NoError;
                }
            }
            else {
                if (variableAddress < MAXDataMemoryAddress) {
                    if (data->location == variableAddress) {
                        variable = data;
                        error = ErrorManagement::NoError;
                    }
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
    startOfVariables = 0u;
}

RuntimeEvaluator::~RuntimeEvaluator(){
    
    if (variablesMemoryPtr != NULL) {
        variablesMemoryPtr = NULL_PTR(DataMemoryElement*);
    }
    if (stackPtr != NULL) {
        stackPtr = NULL_PTR(DataMemoryElement*);
    }
    if (codeMemoryPtr != NULL) {
        codeMemoryPtr = NULL_PTR(CodeMemoryElement*);
    }
}

ErrorManagement::ErrorType RuntimeEvaluator::FindVariableinDB(const CCString &name,VariableInformation *&variableInformation,LinkedListHolderT<VariableInformation> &db) const {
    ErrorManagement::ErrorType ret;

    variableInformation = NULL_PTR(VariableInformation*);
    VariableFinder finder(name);
    db.ListIterate(&finder);
    ret = finder.error;

    if (!ret.ErrorsCleared()){
        REPORT_ERROR_STATIC(ret, "Iteration failed");
    } else {
        variableInformation = finder.variable;
        ret.unsupportedFeature = (variableInformation == NULL);
    }
    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::AddVariable2DB(const CCString &name, LinkedListHolderT<VariableInformation> &db, const TypeDescriptor &td, const DataMemoryAddress location) const {
    ErrorManagement::ErrorType ret;
    VariableInformation *variableToSearch;
    ret = FindVariableinDB(name,variableToSearch,db);
    
    /*lint -e(909) . Justification: the conversion of unsupportedFeature to bool is handled. */
    if (ret.unsupportedFeature) { // if it is already there we do not need to add
        /*lint --e{429} . Justification: the allocated memory is freed by the class destructor. */
        VariableInformation *variableInfo = new VariableInformation;
        variableInfo->name = name;
        variableInfo->type = td;
        variableInfo->location = location;

        db.ListAdd(variableInfo);
        ret.unsupportedFeature = false;
    } else {

        // it would be an error if this is an output variable
        // as we do not allow to override an output
        ret.illegalOperation = true;
    }

    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::FindVariable(const DataMemoryAddress address, VariableInformation *&variableInformation){
    ErrorManagement::ErrorType ret;

    VariableFinder finder(address);

    outputVariableInfo.ListIterate(&finder);
    ret = finder.error;

    variableInformation = NULL_PTR(VariableInformation*);
    if (ret.ErrorsCleared()){
        variableInformation = finder.variable;
        ret.unsupportedFeature = (variableInformation == NULL);
    }

    if (!ret.ErrorsCleared()){
        inputVariableInfo.ListIterate(&finder);
        ret = finder.error;
        if (ret.ErrorsCleared()){
            variableInformation = finder.variable;
            ret.unsupportedFeature = (variableInformation == NULL);
        }
    }
    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::BrowseInputVariable(const uint32 index,VariableInformation *&variableInformation){
    ErrorManagement::ErrorType ret;
    variableInformation = inputVariableInfo.ListPeek(index);
    ret.outOfRange = (variableInformation == NULL);
    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::BrowseOutputVariable(const uint32 index,VariableInformation *&variableInformation){
    ErrorManagement::ErrorType ret;
    variableInformation = outputVariableInfo.ListPeek(index);
    ret.outOfRange = (variableInformation == NULL);
    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::ExtractVariables(){
    
    ErrorManagement::ErrorType ret;

    DataMemoryAddress nextConstantAddress = 0u;
    
    StreamString line;
    char8 terminator;
    
    ret.exception = !RPNCode.Seek(0ull);
    
    bool noErrors = ret.ErrorsCleared();
    while (RPNCode.GetToken(line, "\n", terminator, "\n\r") && noErrors) {

        // extract command and parameter
        StreamString command;
        StreamString parameter1;
        StreamString parameter2;
        StreamString parameter3;

        // extract up to three tokens per line
        ret.illegalOperation = !line.Seek(0ull);
        bool hasCommand      =  line.GetToken(command,    " \t,", terminator," \t,");
        bool hasParameter1   =  line.GetToken(parameter1, " \t,", terminator," \t,");
        bool hasParameter2   =  line.GetToken(parameter2, " \t,", terminator," \t,");
        bool hasParameter3   =  line.GetToken(parameter3, " \t,", terminator," \t,");
        
        hasCommand    = ( (command.Size()   > 0u) && (hasCommand)    );
        hasParameter1 = ( (parameter1.Size()> 0u) && (hasParameter1) );
        hasParameter2 = ( (parameter2.Size()> 0u) && (hasParameter2) );
        hasParameter3 = ( (parameter3.Size()> 0u) && (hasParameter3) );
        
        // now analyse the command
        if (hasCommand){

            if (command == readToken){
                ret.illegalOperation = ( (!hasParameter1) || (hasParameter2) );

                if (!ret.ErrorsCleared()){
                    REPORT_ERROR_STATIC(ret, "%s without variable name", command.Buffer());
                } else{
                    // if an output variable of this name is already present
                    // it means it would have already been loaded
                    // so no need to fetch an external input
                    VariableInformation *variableInformation;
                    ret = FindOutputVariable(parameter1.Buffer(),variableInformation);

                    if (!ret.ErrorsCleared()){
                        ret = AddInputVariable(parameter1.Buffer());
                        /*lint -e(909) . Justification: the conversion of illegalOperation to bool is handled. */
                        if (ret.illegalOperation){
                            REPORT_ERROR_STATIC(ErrorManagement::Information, "Variable %s already registered", parameter1.Buffer());
                            // mask out the case that we already registered this variable
                            ret.illegalOperation = false;
                        }
                        if (!ret.ErrorsCleared()){
                            REPORT_ERROR_STATIC(ret, "Failed Adding input variable %s",parameter1.Buffer());
                        }
                    }
                }
            }
            if (command == writeToken){
                ret.illegalOperation = ( (!hasParameter1) || (hasParameter2) );
                if (!ret.ErrorsCleared()){
                    REPORT_ERROR_STATIC(ret, "%s without variable name", command.Buffer());
                }

                if (ret.ErrorsCleared()){
                    ret = AddOutputVariable(parameter1.Buffer());
                    /*lint -e(909) . Justification: the conversion of illegalOperation to bool is handled. */
                    if (ret.illegalOperation){
                        REPORT_ERROR_STATIC(ret, "Variable %s already registered", parameter1.Buffer());
                        // the error remains as we do not allow overwriting outputs
                    }
                    if (!ret.ErrorsCleared()){
                        REPORT_ERROR_STATIC(ret, "Failed Adding output variable %s",parameter1.Buffer());
                    }
                }
            }
            if (command == constToken){
                ret.illegalOperation = ( (!hasParameter1) || (!hasParameter2) || (hasParameter3) );
                if (!ret.ErrorsCleared()){
                    REPORT_ERROR_STATIC(ret, "%s without type name", command.Buffer());
                }

                // transform the type name into a TypeDescriptor
                // check it is one of the supported types
                TypeDescriptor td;
                if (ret.ErrorsCleared()){
                    td = TypeDescriptor::GetTypeDescriptorFromTypeName(parameter1.Buffer());
                    ret.unsupportedFeature = !td.IsNumericType();
                    if (!ret.ErrorsCleared()){
                        REPORT_ERROR_STATIC(ret, "Type %s is not a numeric supported format", parameter1.Buffer());
                    }
//printf("const type = %x\n",td.all);
                }
                // if supported add up the memory needs
                if (ret.ErrorsCleared()){
                    StreamString constantName;
                    ret.illegalOperation = constantName.Printf("Constant@%u", nextConstantAddress);
                    ret = AddInputVariable(constantName.Buffer(),td,nextConstantAddress);
                }
                if (ret.ErrorsCleared()){
                    uint16 size = td.numberOfBits/8u;
                    ret.unsupportedFeature = (size == 0u);
                    if (!ret.ErrorsCleared()){
                        REPORT_ERROR_STATIC(ret, "Type %s has 0 storageSize", parameter1.Buffer());
                    }

                    nextConstantAddress += ByteSizeToDataMemorySize(size);
                }
            }
        }

        line = "";
        noErrors = ret.ErrorsCleared();
    }

    if (ret.ErrorsCleared()){
        startOfVariables = nextConstantAddress;
    }

    return ret;
}

void* RuntimeEvaluator::GetInputVariableMemory(const StreamString &varNameIn) {
    
    void* retAddress = NULL_PTR(void*);
    bool isFound = false;
    
    uint32 index = 0u;
    VariableInformation *var;

    while( (BrowseInputVariable(index,var) == ErrorManagement::NoError) && (!isFound)){
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

void* RuntimeEvaluator::GetInputVariableMemory(const uint32 &varIndexIn) {
    
    void* retAddress = NULL_PTR(void*);
    
    VariableInformation *var;
    
    if (BrowseInputVariable(varIndexIn,var) == ErrorManagement::NoError) {
        if (var->externalLocation != NULL) {
            retAddress = var->externalLocation;
        }
        else {
            retAddress  = reinterpret_cast<void*>(&dataMemory[var->location]);
        }
    }
    
    return retAddress;
    
}

void* RuntimeEvaluator::GetOutputVariableMemory(const StreamString &varNameIn) {
    
    void* retAddress = NULL_PTR(void*);
    bool isFound = false;
    
    uint32 index = 0u;
    VariableInformation *var;

    while( (BrowseOutputVariable(index,var) == ErrorManagement::NoError) && (!isFound) ){
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

void* RuntimeEvaluator::GetOutputVariableMemory(const uint32 &varIndexIn) {
    
    void* retAddress = NULL_PTR(void*);
    
    VariableInformation *var;
    
    if (BrowseOutputVariable(varIndexIn,var) == ErrorManagement::NoError) {
        if (var->externalLocation != NULL) {
            retAddress = var->externalLocation;
        }
        else {
            retAddress  = reinterpret_cast<void*>(&dataMemory[var->location]);
        }
    }
    
    return retAddress;
    
}

bool RuntimeEvaluator::SetInputVariableMemory(const StreamString &varNameIn, void* const externalLocationIn) {
    
    bool isFound = false;
    
    uint32 index = 0u;
    VariableInformation *var;

    while( (BrowseInputVariable(index,var) == ErrorManagement::NoError) && (!isFound) ){
        if (var->name == varNameIn){
            var->externalLocation = externalLocationIn;
            isFound = true;
        }
        index++;
    }
    
    return isFound;
    
}

bool RuntimeEvaluator::SetInputVariableMemory(const uint32 &varIndexIn, void* const externalLocationIn) {
    
    bool isFound = false;
    
    VariableInformation *var;
    
    if (BrowseInputVariable(varIndexIn,var) == ErrorManagement::NoError) {
        var->externalLocation = externalLocationIn;
        isFound = true;
    }
    
    return isFound;
    
}

/*lint -e{429} -e{593} externalLocationIn is to be freed by the caller*/
bool RuntimeEvaluator::SetOutputVariableMemory(const StreamString &varNameIn, void* const externalLocationIn) {
    
    bool isFound = false;
    
    uint32 index = 0u;
    VariableInformation *var;

    while( (BrowseOutputVariable(index,var) == ErrorManagement::NoError) && (!isFound)){
        if (var->name == varNameIn){
            var->externalLocation = externalLocationIn;
            isFound = true;
        }
        index++;
    }
    
    return isFound;
    
/*lint -e{429} -e{593} externalLocationIn is to be freed by the caller*/
}

bool RuntimeEvaluator::SetOutputVariableMemory(const uint32 &varIndexIn, void* const externalLocationIn) {
    
    bool isFound = false;
    
    VariableInformation *var;
    
    if (BrowseOutputVariable(varIndexIn,var) == ErrorManagement::NoError) {
        var->externalLocation = externalLocationIn;
        isFound = true;
    }
    
    return isFound;
    
}

bool RuntimeEvaluator::SetInputVariableType(const StreamString &varNameIn, const TypeDescriptor &typeIn) {
    
    bool isFound = false;
    
    uint32 index = 0u;
    VariableInformation *var;

    while( (BrowseInputVariable(index,var) == ErrorManagement::NoError) && (!isFound) ){
        if (var->name == varNameIn){
            var->type = typeIn;
            isFound = true;
        }
        index++;
    }
    
    return isFound;
    
}

bool RuntimeEvaluator::SetInputVariableType(const uint32 &varIndexIn, const TypeDescriptor &typeIn) {
    
    bool isFound = false;
    
    VariableInformation *var;
    
    if (BrowseInputVariable(varIndexIn,var) == ErrorManagement::NoError) {
        var->type = typeIn;
        isFound = true;
    }
    
    return isFound;
    
}

bool RuntimeEvaluator::SetOutputVariableType(const StreamString &varNameIn, const TypeDescriptor &typeIn) {
    
    bool isFound = false;
    
    uint32 index = 0u;
    VariableInformation *var;

    while( (BrowseOutputVariable(index,var) == ErrorManagement::NoError) && (!isFound) ){
        if (var->name == varNameIn){
            var->type = typeIn;
            isFound = true;
        }
        index++;
    }
    
    return isFound;
    
}

bool RuntimeEvaluator::SetOutputVariableType(const uint32 &varIndexIn, const TypeDescriptor &typeIn) {
    
    bool isFound = false;
    
    VariableInformation *var;
    
    if (BrowseOutputVariable(varIndexIn,var) == ErrorManagement::NoError) {
        var->type = typeIn;
        isFound = true;
    }
    
    return isFound;
    
}

ErrorManagement::ErrorType RuntimeEvaluator::Compile()
{
    
    ErrorManagement::ErrorType ret;
    
    DataMemoryAddress nextVariableAddress = startOfVariables;
    // check that all variables have a type and allocate variables + constants

    uint32 index = 0u;
    VariableInformation *var;
    
    bool noErrors = ret.ErrorsCleared();
    while(BrowseInputVariable(index,var) && noErrors) {
        ret.unsupportedFeature = !var->type.IsNumericType();
        if (!ret.ErrorsCleared()){
            REPORT_ERROR_STATIC(ret, "Input variable %s has incompatible non-numeric type ", var->name.Buffer());
        }

        // skip constants are already allocated
        noErrors = ret.ErrorsCleared();
        if ( (var->location == MAXDataMemoryAddress) && noErrors ){
            var->location = nextVariableAddress;
            if (var->externalLocation == NULL){
                // if NULL variable is in DataMemory
                nextVariableAddress += ByteSizeToDataMemorySize(var->type.numberOfBits/8u);
            } else {
                // variable address is in DataMemory
                nextVariableAddress += ByteSizeToDataMemorySize(static_cast<uint16>(sizeof (void *)));
            }
        }
        index++;
        noErrors = ret.ErrorsCleared();
    }

    index = 0u;
    noErrors = ret.ErrorsCleared();
    while(BrowseOutputVariable(index,var) && noErrors) {
        // local variable reserve 8 bytes for it
        if ((var->type == VoidType) && (var->externalLocation == NULL)){
            var->location = nextVariableAddress;
            nextVariableAddress += ByteSizeToDataMemorySize(static_cast<uint16>(sizeof(float64)));
        } else {
            ret.unsupportedFeature = !var->type.IsNumericType();
            if (!ret.ErrorsCleared()){
                REPORT_ERROR_STATIC(ret, "Output variable %s has incompatible non-numeric type", var->name.Buffer());
            }

            if (ret.ErrorsCleared()){
                var->location = nextVariableAddress;
                if (var->externalLocation == NULL){
                    // if NULL variable is in DataMemory
                    nextVariableAddress += ByteSizeToDataMemorySize(var->type.numberOfBits/8u);
                } else {
                    // variable address is in DataMemory
                    nextVariableAddress += ByteSizeToDataMemorySize(static_cast<uint16>(sizeof (void *)));
                }
            }
        }
        index++;
        noErrors = ret.ErrorsCleared();
    }

    // already
    dataMemory.SetSize(nextVariableAddress);
    variablesMemoryPtr = static_cast<DataMemoryElement *>(dataMemory.GetDataPointer());

    // initialise compilation memory
    StaticStack<TypeDescriptor,32u> typeStack;
    DataMemoryAddress maxDataStackSize = 0u;    // max value of dataStackSize
    DataMemoryAddress dataStackSize    = 0u;    // current simulated value of data stack size
//  startOfVariables = 0;                       // for now no constants - so variables start at 0
    DataMemoryAddress nextConstantAddress = 0u; // pointer to next constant memory area

    // clean all the memory
    codeMemory.Clean();

    StreamString line;
    char8 terminator;
    
    ret.exception = !RPNCode.Seek(0ull);
    
    noErrors = ret.ErrorsCleared();
    while (RPNCode.GetToken(line, "\n", terminator, "\n\r") && noErrors) {

        // extract command and parameter
        StreamString command;
        StreamString parameter1;
        StreamString parameter2;
        StreamString parameter3;

        // extract up to three tokens per line
        ret.illegalOperation = !line.Seek(0ull);
        bool hasCommand      =  line.GetToken(command,    " \t,", terminator," \t,");
        bool hasParameter1   =  line.GetToken(parameter1, " \t,", terminator," \t,");
        bool hasParameter2   =  line.GetToken(parameter2, " \t,", terminator," \t,");
        bool hasParameter3   =  line.GetToken(parameter3, " \t,", terminator," \t,");
        
        hasCommand    = ( (command.Size()   > 0u) && (hasCommand)    );
        hasParameter1 = ( (parameter1.Size()> 0u) && (hasParameter1) );
        hasParameter2 = ( (parameter2.Size()> 0u) && (hasParameter2) );
        hasParameter3 = ( (parameter3.Size()> 0u) && (hasParameter3) );

        // now analyze the command
        if (hasCommand){
            // assign invalid value
            CodeMemoryElement code2 = TypeCharacteristics<CodeMemoryElement>::MaxValue();
            bool matchOutput = false;

            // PROCESS CAST command
            // PUSH type(parameter1) --> TypeStack
            // matchOutput = true;
            if (command == castToken){
                ret.illegalOperation = ( (!hasParameter1) || (hasParameter2) );
                if (!ret.ErrorsCleared()){
                    REPORT_ERROR_STATIC(ret,"%s without type name", command.Buffer());
                } else{
                    // transform the type name into a TypeDescriptor
                    // check it is one of the supported types
                    TypeDescriptor td;
                    td = TypeDescriptor::GetTypeDescriptorFromTypeName(parameter1.Buffer());
                    ret.unsupportedFeature = !td.IsNumericType();
                    if (!ret.ErrorsCleared()){
                        REPORT_ERROR_STATIC(ret, "Type %s is not a numeric supported format", parameter1.Buffer());
                    } else{
                        ret.fatalError = !typeStack.Push(td);
                        if (!ret.ErrorsCleared()){
                            REPORT_ERROR_STATIC(ret, "Failed to push type into stack");
                        }
                    }

                    if (ret.ErrorsCleared()){
                        matchOutput = true;
                    }
                }
            }

            // PROCESS WRITE command
            // find_variable(parameter1) on outputs
            //    mark variable as already written
            // PUSH variable.type --> TypeStack
            // matchOutput = true;
            // assign code2 to address of variable
            if (command == writeToken){
                VariableInformation *variableInformation = NULL_PTR(VariableInformation *);

                ret.illegalOperation = ( (!hasParameter1) || (hasParameter2) );
                if (!ret.ErrorsCleared()){
                    REPORT_ERROR_STATIC(ret, "%s without variable name", command.Buffer());
                } else{
                    ret = FindOutputVariable(parameter1.Buffer(),variableInformation);
                    if (!ret.ErrorsCleared()){
                        REPORT_ERROR_STATIC(ret, "Output variable %s not found", parameter1.Buffer());
                    }
                }

                if (ret.ErrorsCleared()){
                    // change from WRITE to RWRITE
                    if (variableInformation->externalLocation != NULL){
                        command = remoteWriteToken;
                        // save address now
                        Variable<void *>(variableInformation->location)= variableInformation->externalLocation;
                    }
                }

                TypeDescriptor td = InvalidType;
                if (ret.ErrorsCleared()){
                    td = variableInformation->type;

                    // found local variable - allocate and give type here
                    if ((td == VoidType) && (variableInformation->externalLocation == NULL)){
                        ret.fatalError = !typeStack.Peek(0u,td);
                        if (!ret.ErrorsCleared()){
                            REPORT_ERROR_STATIC(ret, "Expecting source type in stack");
                        }
                        variableInformation->type = td;
                    } else {
                        ret.unsupportedFeature = !td.IsNumericType();
                        if (!ret.ErrorsCleared()){
                            REPORT_ERROR_STATIC(ret, "Variable %s does not have a numeric supported format", parameter1.Buffer());
                        }
                    }
                }

                if (ret.ErrorsCleared()){
//printf("Stack[%i].Push(%x) -->",typeStack.GetSize(),td.all);
                    ret.fatalError = !typeStack.Push(td);
                    if (!ret.ErrorsCleared()){
                        REPORT_ERROR_STATIC(ret, "Failed to push type into stack");
                    }
//printf("Stack[%i] \n",typeStack.GetSize());
                }

                if (ret.ErrorsCleared()){
                    matchOutput = true;
                    code2 = variableInformation->location;
                    variableInformation->variableUsed = true;
                }

            }

            // PROCESS READ command
            // find_variable(parameter1)
            //    search first on outputs if already written
            //    search then on inputs
            // PUSH variable.type --> TypeStack
            // matchOutput = true;
            // assign code2 to address of variable
            if (command == readToken){
                VariableInformation *variableInformation = NULL_PTR(VariableInformation *);

                ret.illegalOperation = ( (!hasParameter1) || (hasParameter2) );
                if (!ret.ErrorsCleared()){
                    REPORT_ERROR_STATIC(ret, "%s without variable name", command.Buffer());
                } else{
                    // try find an output variable with this name
                    ret = FindOutputVariable(parameter1.Buffer(),variableInformation);
                    if (ret.ErrorsCleared()){
                        // not set yet - cannot use
                        ret.notCompleted = (!variableInformation->variableUsed);
                    }
                    // try to see if there is an input variable
                    if (!ret.ErrorsCleared()){
                        ret = FindInputVariable(parameter1.Buffer(),variableInformation);
                        if (!ret.ErrorsCleared()){
                            REPORT_ERROR_STATIC(ret, "Input variable %s not found", parameter1.Buffer());
                        }
                    }
                }

                if (ret.ErrorsCleared()){
                    // change from READ to RREAD
                    if (variableInformation->externalLocation != NULL){
                        command = remoteReadToken;
                        // save address now
                        Variable<void *>(variableInformation->location)= variableInformation->externalLocation;
                    }
                }

                TypeDescriptor td = InvalidType;
                if (ret.ErrorsCleared()){
                    td = variableInformation->type;
                    ret.unsupportedFeature = !td.IsNumericType();
                    if (!ret.ErrorsCleared()){
                        REPORT_ERROR_STATIC(ret, "Variable %s does not have a numeric supported format", parameter1.Buffer());
                    }
                }
//printf("read %s type = %x  type = %x\n",variableInformation->name.GetList(),td.all,variableInformation->type.all);
                if (ret.ErrorsCleared()){
                    ret.fatalError = !typeStack.Push(td);
                    if (!ret.ErrorsCleared()){
                        REPORT_ERROR_STATIC(ret, "Failed to push type into stack");
                    }
                }

                if (ret.ErrorsCleared()){
                    matchOutput = true;
                    code2 = variableInformation->location;
                }

            }

            // PROCESS CONST command
            // PUSH type(parameter1) --> TypeStack
            // matchOutput = true;
            // create AnyType and convert constant from string to variable memory
            // assign code2 to address of constant
            // command = READ
            if (command == constToken){
                TypeDescriptor td;

                ret.illegalOperation = ( (!hasParameter1) || (!hasParameter2) || (hasParameter3) );
                if (!ret.ErrorsCleared()){
                    REPORT_ERROR_STATIC(ret,"%s without type name and value", command.Buffer());
                } else{
                // transform the type name into a TypeDescriptor
                // check it is one of the supported types

                    td = TypeDescriptor::GetTypeDescriptorFromTypeName(parameter1.Buffer());
                    ret.unsupportedFeature = !td.IsNumericType();
                    if (!ret.ErrorsCleared()){
                        REPORT_ERROR_STATIC(ret, "Type %s is not a numeric supported format", parameter1.Buffer());
                    }
                }

                // convert string to number and save value into memory
                if (ret.ErrorsCleared()){
                    //nextConstantAddress
                    AnyType dest(td, 0u, &variablesMemoryPtr[nextConstantAddress]);
                    ret.fatalError = !TypeConvert(dest, parameter2.Buffer());
                    if (!ret.ErrorsCleared()){
                        REPORT_ERROR_STATIC(ret, "TypeConvert failed ");
                    }

                }

                if (ret.ErrorsCleared()){
                    ret.fatalError = !typeStack.Push(td);
                    if (!ret.ErrorsCleared()){
                        REPORT_ERROR_STATIC(ret, "Failed to push type into stack");
                    }
                }

                if (ret.ErrorsCleared()){
                    matchOutput = true;
                    code2 = nextConstantAddress;
                    nextConstantAddress += ByteSizeToDataMemorySize(td.numberOfBits/8u);
                    // the actual command is a READ from the constant area
                    command = readToken;
                }
            }

            CodeMemoryElement code = 0u;
            if (ret.ErrorsCleared()){
                ret.unsupportedFeature = !FindPCodeAndUpdateTypeStack(code,command.Buffer(),typeStack,matchOutput,dataStackSize);
                if (!ret.ErrorsCleared()){
                    StreamString typeList;
                    uint32 n2scan = 2u;
                    if (matchOutput) {
                        n2scan++;
                    }
                    typeList += '[';
                    for(uint32 typeIndex = 0u; typeIndex < n2scan; typeIndex++){
                        if (typeIndex > 0u){
                            typeList += '|';
                        }
                        TypeDescriptor td;
                        if(!typeStack.Peek(typeIndex,td)) {
                            REPORT_ERROR_STATIC(ErrorManagement::Debug, "Peek() failed");
                        }
                        typeList += TypeDescriptor::GetTypeNameFromTypeDescriptor(td);
                    }
                    typeList += ']';
                    if (!ret.ErrorsCleared()){
                        REPORT_ERROR_STATIC(ret, "Command %s(%s) not found", command.Buffer(), typeList.Buffer());
                    }
                }
//printf("after %s %i elements in dataStack\n ",command.GetList(),dataStackSize);
            }

            // finally compile!
            if (ret.ErrorsCleared()){
                // update stackSize
                if (dataStackSize > maxDataStackSize){
                    maxDataStackSize = dataStackSize;
                }

                ret.fatalError = !codeMemory.Add(code);
                if (!ret.ErrorsCleared()){
                    REPORT_ERROR_STATIC(ret, "Failed to add instruction to code");
                }
                
                noErrors = ret.ErrorsCleared();
                if ( (code2 != TypeCharacteristics<CodeMemoryElement>::MaxValue()) && noErrors ){
                    ret.fatalError = !codeMemory.Add(code2);
                    if (!ret.ErrorsCleared()){
                        REPORT_ERROR_STATIC(ret, "Failed to add instruction to code");
                    }
                }
            }
        }

        line = "";
        noErrors = ret.ErrorsCleared();
    }

    // final checks
    if (ret.ErrorsCleared()){

        // assign the code pointer
        codeMemoryPtr = codeMemory.GetAllocatedMemoryConst();

        // size the stack
        stack.SetSize(maxDataStackSize);
        stackPtr = static_cast<DataMemoryElement*>(stack.GetDataPointer());

        variablesMemoryPtr = static_cast<DataMemoryElement *>(dataMemory.GetDataPointer());
    }

    // check that the TypeStack is empty
    if (ret.ErrorsCleared()){
        ret.internalSetupError = (typeStack.GetSize() > 0u);
        if (!ret.ErrorsCleared()){
            REPORT_ERROR_STATIC(ret, "Operation sequence is incomplete: %u data left in stack", typeStack.GetSize());
        }
    }

    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::FunctionRecordInputs2String(RuntimeEvaluatorFunction &functionInformation,StreamString &cst,const bool peekOnly,const bool showData,const bool showTypes){
    ErrorManagement::ErrorType ret;

    const CodeMemoryElement *saveCodeMemoryPtr = codeMemoryPtr;
    StreamString functionName = functionInformation.GetName();
    
    bool isWriteToken       = (functionName == writeToken);
    bool isRemoteWriteToken = (functionName == remoteWriteToken);
    
    if ( isWriteToken || isRemoteWriteToken ){
        CodeMemoryElement pCode2 = GetPseudoCode();

        VariableInformation *vi;
        ret = FindVariable(pCode2,vi);
        if (!ret.ErrorsCleared()){
            REPORT_ERROR_STATIC(ret, "No variable or constant @ %u",pCode2);
        } else{
            ret.exception = !cst.Printf(" %s", vi->name.Buffer());
        }
    }

    DataMemoryAddress dataStackIndex = 0u;

    if (showData || showTypes){
        Vector<TypeDescriptor> functionInputTypes = functionInformation.GetInputTypes();
        
        bool noErrors = ret.ErrorsCleared();
        for(uint32 i = 0u; (i<functionInputTypes.GetNumberOfElements()) && (noErrors); i++){
            TypeDescriptor td = functionInputTypes[i];

            if (i != 0u) {
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
                AnyType src(td, 0u, stackPtr - dataStackIndex);
                ret.exception = !cst.Printf("%!", src);
            }
            if (i == (functionInputTypes.GetNumberOfElements()-1U)){
                cst += ')';
            }
            
            noErrors = ret.ErrorsCleared();
        }
    }

    // restore any used data
    if (peekOnly){
        codeMemoryPtr  = saveCodeMemoryPtr;
    }

    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::FunctionRecordOutputs2String(RuntimeEvaluatorFunction &functionInformation,StreamString &cst, const bool lookBack, const bool showData, const bool showTypes){
    ErrorManagement::ErrorType ret;
    StreamString functionName = functionInformation.GetName();
    Vector<TypeDescriptor> functionOutputTypes = functionInformation.GetOutputTypes();
    
    bool isCastToken       = (functionName == castToken);
    bool isReadToken       = (functionName == readToken);
    bool isRemoteReadToken = (functionName == remoteReadToken);
    
    // if already showing the types do not show the parameter of the CAST
    if ( (isCastToken) && (!showTypes)) {
        cst += ' ';
        CCString typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(functionOutputTypes[0u]);
        if (typeName != NULL) {
            cst += typeName.GetList();
        } else {
            ret.fatalError = true;
        }

    }
    if ( isReadToken || isRemoteReadToken ) {
        CodeMemoryElement pCode2;
        if (lookBack){
            pCode2 = codeMemoryPtr[-1];
        } else {
            pCode2 = GetPseudoCode();
        }

        VariableInformation *vi;
        ret = FindVariable(pCode2,vi);
        if (!ret.ErrorsCleared()){
            REPORT_ERROR_STATIC(ret, "No variable or constant @ %u",pCode2);
        } else{
            if (pCode2 < startOfVariables){

                cst += ' ';

                CCString typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(vi->type);
                if (typeName != NULL) {
                    cst += typeName.GetList();
                } else {
                    ret.fatalError = true;
                }

                if (!ret.ErrorsCleared()){
                    REPORT_ERROR_STATIC(ret, "GetTypeNameFromTypeDescriptor failed ");
                } else {
                    AnyType src(vi->type, 0u, &variablesMemoryPtr[pCode2]);
                    ret.exception = !cst.Printf(" %!", src);
                }

            } else {
                ret.exception = !cst.Printf(" %s", vi->name.Buffer());
            }
        }
    }

    DataMemoryAddress dataStackIndex = 0u;

    if (showData || showTypes){
        
        bool noErrors = ret.ErrorsCleared();
        for(uint32 i = 0u; (i<functionOutputTypes.GetNumberOfElements()) && (noErrors); i++) {
            TypeDescriptor td = functionOutputTypes[i];
            if (i != 0u) {
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
                AnyType src(td, 0u, stackPtr - dataStackIndex);
                ret.exception = !cst.Printf("%!", src);
            }
            if (i == (functionOutputTypes.GetNumberOfElements()-1U)){
                cst += ')';
            }
            noErrors = ret.ErrorsCleared();
        }
    }

    return ret;
}

/*lint -e{946, 947, 9016} codeMemoryMaxPtr is calculated from pointers pointing to the same array
 * and is only used as a safety check, thus it cannot go out of bounds */
ErrorManagement::ErrorType RuntimeEvaluator::Execute(const executionMode mode, StreamI* const debugStream){

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
        
        bool noErrors = runtimeError.ErrorsCleared();
        while ((codeMemoryPtr < codeMemoryMaxPtr) && (noErrors)) {
            CodeMemoryElement pCode = GetPseudoCode();
            functionRecords[pCode].ExecuteFunction(*this);
            // note that the stackPtr will reach the max value - as it points to the next value to write
            runtimeError.outOfRange = ((runtimeError.outOfRange) || (stackPtr > stackMaxPtr) ||  (stackPtr < stackMinPtr));
            if (!runtimeError){
                REPORT_ERROR_STATIC(runtimeError, "Stack over/under flow %i [0 - %i]", static_cast<int64>(stackPtr-stackMinPtr), static_cast<int64>(stackMaxPtr- stackMinPtr));
            }
            noErrors = runtimeError.ErrorsCleared();
        }
        runtimeError.notCompleted = (codeMemoryPtr < codeMemoryMaxPtr);
        if (!runtimeError){
            REPORT_ERROR_STATIC(runtimeError, "Code execution interrupted");
        }
    }break;
    case debugMode:
    default:{
        if (debugStream == NULL_PTR(StreamI *)){
            runtimeError.parametersError = true;
            if (!runtimeError){
                REPORT_ERROR_STATIC(runtimeError, "DebugMode requested with debugStream set to NULL");
            }
        } else {
            StreamString debugMessage;

            debugMessage += "[line]-[stackPtr]-[codePtr]::[CODE] stack-in => stack-out\n";
            int32 lineCounter = 1;
            StreamString currentFunctionName = "";
            
            bool noErrors = runtimeError.ErrorsCleared();
            while ((codeMemoryPtr < codeMemoryMaxPtr) && (noErrors)){
                int64 stackOffset = stackPtr - static_cast<DataMemoryElement*>(stack.GetDataPointer());
                int64 codeOffset  = codeMemoryPtr - codeMemory.GetAllocatedMemoryConst();
                runtimeError.exception = !debugMessage.Printf("%i - %i - %i :: ", lineCounter, stackOffset, codeOffset);

                CodeMemoryElement pCode = GetPseudoCode();

                RuntimeEvaluatorFunction &fr = functionRecords[pCode];

                // show update info
                currentFunctionName = fr.GetName();
                runtimeError.exception = !debugMessage.Printf("%s ", currentFunctionName.Buffer());

                // errors due to debugging
                ErrorManagement::ErrorType ret;

                // show inputs
                ret = FunctionRecordInputs2String(fr,debugMessage,true,true,true);
                if (!ret.ErrorsCleared()){
                    REPORT_ERROR_STATIC(ret, "Analysing input side of function call");
                }

                // executes code
                fr.ExecuteFunction(*this);

                if (ret.ErrorsCleared()){
                    // show outputs
                    ret = FunctionRecordOutputs2String(fr,debugMessage,true,true,true);
                    if (!ret.ErrorsCleared()){
                        REPORT_ERROR_STATIC(ret, "Analysing input side of function call");
                    }
                }

                if (!runtimeError.ErrorsCleared()){
                    debugMessage += " <ERROR> ";
                }

                debugMessage += '\n';

                uint32 size = static_cast<uint32>(debugMessage.Size());
                runtimeError.exception = !debugStream->Write(debugMessage.Buffer(),size);

                // reset line
                runtimeError.exception = !debugMessage.SetSize(0ull);
                lineCounter++;
                
                noErrors = runtimeError.ErrorsCleared();
            }
            if (runtimeError.ErrorsCleared()){
                int64 stackOffset = stackPtr - static_cast<DataMemoryElement*>(stack.GetDataPointer());
                int64 codeOffset  = codeMemoryPtr - codeMemory.GetAllocatedMemoryConst();
                runtimeError.exception = !debugMessage.Printf("%i - %i :: END", stackOffset, codeOffset);

                uint32 size = static_cast<uint32>(debugMessage.Size());
                runtimeError.exception = !debugStream->Write(debugMessage.Buffer(),size);
            }
        }
    }
    }
    if (!runtimeError){
        REPORT_ERROR_STATIC(runtimeError, "Execution error");
    }

    if (stackPtr  != static_cast<DataMemoryElement*>(stack.GetDataPointer())){
        runtimeError.internalSetupError = true;
        int64 offset = stackPtr - static_cast<DataMemoryElement*>(stack.GetDataPointer());
        if (!runtimeError){
            REPORT_ERROR_STATIC(runtimeError, "Stack pointer not back to origin : %i elements left", offset);
        }
    }

    return runtimeError;
}

/*lint -e{946, 947, 9016} codeMemoryMaxPtr is calculated from pointers pointing to the same array
 * and is only used as a safety check, thus it cannot go out of bounds */
ErrorManagement::ErrorType RuntimeEvaluator::DeCompile(StreamString &DeCompileRPNCode, const bool showTypes) {
    
    ErrorManagement::ErrorType ret;
    
    codeMemoryPtr = codeMemory.GetAllocatedMemoryConst();
    CodeMemoryAddress codeMaxIndex  = static_cast<CodeMemoryAddress>(codeMemory.GetSize());
    const CodeMemoryElement *codeMemoryMaxPtr = codeMemoryPtr + codeMaxIndex;

    variablesMemoryPtr = static_cast<DataMemoryElement *>(dataMemory.GetDataPointer());
    
    bool noErrors = ret.ErrorsCleared();
    while((codeMemoryPtr < codeMemoryMaxPtr) && noErrors){
        CodeMemoryElement pCode = GetPseudoCode();
        RuntimeEvaluatorFunction &fr = functionRecords[pCode];
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

        if (ret.ErrorsCleared()){
            ret = FunctionRecordOutputs2String(fr,DeCompileRPNCode,false,false,showTypes);
        }
        DeCompileRPNCode += '\n';
        noErrors = ret.ErrorsCleared();
    }

    return ret;
}

} //MARTe
