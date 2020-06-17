/**
 * @file RuntimeEvaluatorFunctions.h
 * @brief Header file for class RuntimeEvaluatorFunctions
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

#ifndef RUNTIMEEVALUATORFUNCTIONS_H_
#define RUNTIMEEVALUATORFUNCTIONS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Stack.h"
#include "RuntimeEvaluatorInfo.h"
#include "RuntimeEvaluator.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/** the type of the PCode function */
typedef void (*Function)(RuntimeEvaluator & context);

/** custom stack processing and variable allocation*/
typedef ErrorManagement::ErrorType (*UpdateStackFun)(
		const RuntimeEvaluatorFunction &					ref,
		Stack<VariableDescriptor> &							typeStack,
		RuntimeEvaluatorInfo::DataMemoryAddress & 			dataStackSize,
		List<RuntimeEvaluatorInfo::VariableInformation> &	db,
		uint32 &                                            dummyID);

const UpdateStackFun NULLUpdateStackFun = NULL;

/**
 * records information necessary to be able to use it during compilation
 */
struct RuntimeEvaluatorFunction{
    /**
     *    The name of the functions as used in the RPN code
     */
    CCString                 	name;

    /**
     * How many stack elements it will consume
     * !NOTE that for CONST
     */
    uint16                  	numberOfInputs;

    /**
     * How many stack elements it will produce
     */
    uint16                     	numberOfOutputs;

    /**
     * array of types one for each input and output
     */
    const VariableDescriptor *  types;

    /**
     * The function code itself
     */
    Function                 	ExecuteFunction;

    /**
     * Custom UpdateStack
     */
    UpdateStackFun              updateStackFun;

    /**
     * @brief returns true if the name and types matches
     * on success replaces the type on the stack with the result type
     * also simulates variations on the dataStack
	 *  @param[out] code the address within functionRecords to the selected function
	 *  @param[in] 	nameIn the function name to be matched
	 *  @param[in] 	typeStack the current stack of input types which need to be matched, consumed and replaced with the outputType
	 *  @param[in,out] dataStackSize the amount of space needed in the dataStack (it gets updated by this function upon match)
	 */
//    ErrorManagement::ErrorType TryConsume(CCString nameIn,Stack<VariableDescriptor> &typeStack, bool matchOutput,RuntimeEvaluatorInfo::DataMemoryAddress &dataStackSize);


    /**
     * @brief returns true if the name and types matches
	 *  @param[out] code the address within functionRecords to the selected function
	 *  @param[in] 	nameIn the function name to be matched
	 *  @param[in] 	typeStack the current stack of input types which need to be matched, consumed and replaced with the outputType
	 */
    ErrorManagement::ErrorType  Check(
    		CCString 									nameIn,
    		Stack<VariableDescriptor> &					typeStack,
    		bool 										matchOutput
    );

    /**
     * @brief simulates execution of this P-code
     * replaces the type on the stack with the result type
     * also simulates variations on the dataStack
	 *  @param[out] code the address within functionRecords to the selected function
	 *  @param[in] 	typeStack the current stack of input types which need to be matched, consumed and replaced with the outputType
	 *  @param[in,out] dataStackSize the amount of space needed in the dataStack (it gets updated by this function upon match)
	 */
    ErrorManagement::ErrorType UpdateStack(
    		Stack<VariableDescriptor> &							typeStack,
			RuntimeEvaluatorInfo::DataMemoryAddress & 			dataStackSize,
    		List<RuntimeEvaluatorInfo::VariableInformation> &	db,
    		bool 												matchOutput,
    		uint32 &                                            dummyID);

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
extern RuntimeEvaluatorFunction functionRecords[maxFunctions];

#if 0
/**
 *  @brief Searches the list of functions for one matching the needs and then simulates its effect on the stack of types and the dataStackSize
 *  @param[out] code the address within functionRecords to the selected function
 *  @param[in] 	nameIn the function name to be matched
 *  @param[in] 	typeStack the current stack of input types which need to be matched, consumed and replaced with the outputType
 *  @param[out] dataStackSize the amount of space needed in the dataStack
 *
 */
ErrorManagement::ErrorType FindPCodeAndUpdateTypeStack(
		RuntimeEvaluatorInfo::CodeMemoryElement &code,
		CCString nameIn,
		Stack<VariableDescriptor> &typeStack,
		bool matchOutput,
		RuntimeEvaluatorInfo::DataMemoryAddress &dataStackSize
);
#endif

/**
 *  @brief Searches the list of functions for one matching the needs
 *  @param[out] code the address within functionRecords to the selected function
 *  @param[in] 	nameIn the function name to be matched
 *  @param[in] 	typeStack the current stack of input types which need to be matched, consumed and replaced with the outputType
 *
 */
ErrorManagement::ErrorType  FindPCode(
		RuntimeEvaluatorInfo::CodeMemoryElement &code,
		CCString 								nameIn,
		Stack<VariableDescriptor> &				typeStack,
		bool 									matchOutput
);


/**
 * to register a function
 */
void RegisterFunction(const RuntimeEvaluatorFunction &record);

/**
 * generates boiler plate code to register a function
 */
#define REGISTER_PCODE_FUNCTION(name,subName,nInputs,nOutputs,function,...)\
    static const VariableDescriptor name ## subName ## _FunctionTypes[] = {__VA_ARGS__}; \
    static const RuntimeEvaluatorFunction name ## subName ## _FunctionRecord={#name,nInputs,nOutputs,name ## subName ## _FunctionTypes,&function,NULLUpdateStackFun}; \
    static class name ## subName ## RegisterClass { \
    public: name ## subName ## RegisterClass(){\
            RegisterFunction(name ## subName ## _FunctionRecord);\
        }\
    } name ## subName ## RegisterClassInstance;

/**
 * generates boiler plate code to register a function
 */
#define REGISTER_PCODE_MATRIX_FUNCTION(name,subName,nInputs,nOutputs,function,updateFunction,...)\
    static const VariableDescriptor name ## subName ## _FunctionTypes[] = {__VA_ARGS__}; \
    static const RuntimeEvaluatorFunction name ## subName ## _FunctionRecord={#name,nInputs,nOutputs,name ## subName ## _FunctionTypes,&function,updateFunction}; \
    static class name ## subName ## RegisterClass { \
    public: name ## subName ## RegisterClass(){\
            RegisterFunction(name ## subName ## _FunctionRecord);\
        }\
    } name ## subName ## RegisterClassInstance;


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_PRIVATE_PSEUDOCODEFUNCTIONS_H_ */
