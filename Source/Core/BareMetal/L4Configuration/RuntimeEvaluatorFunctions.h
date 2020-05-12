/**
 * @file PseudoCodeFunctions.h
 * @brief Header file for class AnyType
 * @date 8 Apr 2020
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

#ifndef PSEUDOCODE_FUNCTIONS_H_
#define PSEUDOCODE_FUNCTIONS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StaticStack.h"
#include "RuntimeEvaluator.h"
#include "PseudoCodeVariableInformation.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/** the type of the PCode function */
typedef void (*Function)(RuntimeEvaluator & context);

/**
 * records information necessary to be able to use it during compilation
 */
class RuntimeEvaluatorFunctions{
public:

    /**
     * default constructor
     */
    RuntimeEvaluatorFunctions();

    /**
     * full constructor
     */
    RuntimeEvaluatorFunctions(CCString nameIn, uint16 numberOfInputsIn, uint16 numberOfOutputsIn, TypeDescriptor* typesIn, Function functionIn);

    StreamString GetName(){return name;}

    Vector<TypeDescriptor> GetInputTypes();

    Vector<TypeDescriptor> GetOutputTypes();

    void ExecuteFunction(RuntimeEvaluator &context){function(context);}

    /**
     * returns true if the name and types matches
     * on success replaces the type on the stack with the result type
     * also simulates variations on the dataStack
     */
    bool TryConsume(CCString nameIn,StaticStack<TypeDescriptor,32> &typeStack, bool matchOutput,DataMemoryAddress &dataStackSize);

private:
    /**
     *  The name of the functions as used in the RPN code
     */
    StreamString            name;

    /**
     * How many stack elements it will consume
     * !NOTE that for CONST
     */
    uint16                  numberOfInputs;

    /**
     * How many stack elements it will produce
     */
    uint16                  numberOfOutputs;

    /**
     * array of types one for each input and output
     */
    TypeDescriptor*   types;

    /**
     * The function code itself
     */
    Function                function;

};

/**
 * max number of registered functions
 */
static const uint32 maxFunctions = 16384;

/**
 * actually available functions
 */
extern uint32 availableFunctions;

/**
 * the database of functions
 */
extern RuntimeEvaluatorFunctions functionRecords[maxFunctions];

/**
 *
 */
bool FindPCodeAndUpdateTypeStack(CodeMemoryElement &code, CCString nameIn,StaticStack<TypeDescriptor,32> &typeStack, bool matchOutput,DataMemoryAddress &dataStackSize);

/**
 * to register a function
 */
void RegisterFunction(const RuntimeEvaluatorFunctions &record);

/**
 * generates boiler plate code to register a function
 */
#define REGISTER_PCODE_FUNCTION(name,subName,nInputs,nOutputs,function,...)\
    static TypeDescriptor name ## subName ## _FunctionTypes[] = {__VA_ARGS__}; \
    static const RuntimeEvaluatorFunctions name ## subName ## _RuntimeEvaluatorFunctions(#name,nInputs,nOutputs,name ## subName ## _FunctionTypes,&function); \
    static class name ## subName ## RegisterClass { \
    public: name ## subName ## RegisterClass(){\
            RegisterFunction(name ## subName ## _RuntimeEvaluatorFunctions);\
        }\
    } name ## subName ## RegisterClassInstance;


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_PRIVATE_PSEUDOCODEFUNCTIONS_H_ */
