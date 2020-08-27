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

#ifndef RUNTIMEEVALUATORFUNCTION_H_
#define RUNTIMEEVALUATORFUNCTION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Stack.h"
#include "Queue.h"
#include "Matrix.h"
#include "Private/MatrixSize.h"
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
/**
 * the input Q contains the sizes of each input matrix used in the operation
 * the output shall conyain the size of each input matrix that need to be built as a temporary output
 * if the output is an existing matrix than its size will not be here
 * note that this means that any pass-through matrix need to be at the end of the output list
 * note that READ simply copies the position of the Matrix within Variables to the stack. For this reason READ will not produce a MatrixSize in the outputTypeQueue
 */
typedef ErrorManagement::ErrorType (*CheckMatrixSizesFun)(
        Queue<MatrixSize> &                                 inputTypeQueue,
        Queue<MatrixSize> &                                 outputTypeQueue
);
static const CheckMatrixSizesFun NULLCheckMatrixSizesFun = NULL;

/** custom stack processing and variable allocation*/
typedef ErrorManagement::ErrorType (*UpdateStackFun)(
		const RuntimeEvaluatorFunction &					ref,
		Stack<AnyType> &							        typeStack,
		RuntimeEvaluator::DataMemoryAddress & 			    dataStackSize,
		List<RuntimeEvaluator::VariableInformation> &	    intermediateResultVariablesdb,
		uint32 &                                            dummyID);

static const UpdateStackFun NULLUpdateStackFun = NULL;

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
    Function                 	executeFunction;

    /**
     * Custom UpdateStackFun
     */
    UpdateStackFun              updateStackFun;

    /**
     * Custom CheckMatrixSizesFun
     */
    CheckMatrixSizesFun        checkMatrixSizeFunction;


    RuntimeEvaluatorFunction()    :
        name(""),
        numberOfInputs(0),
        numberOfOutputs(0),
        types(NULL),
        executeFunction(NULL),
        updateStackFun(NULL),
        checkMatrixSizeFunction(NULL)
    {
    }

    RuntimeEvaluatorFunction(
            CCString                    nameIn,
            uint16                      numberOfInputsIn,
            uint16                      numberOfOutputsIn,
            const VariableDescriptor *  typesIn,
            Function                    executeFunctionIn,
            CheckMatrixSizesFun         checkMatrixSizeFunctionIn):
                name(nameIn),
                numberOfInputs(numberOfInputsIn),
                numberOfOutputs(numberOfOutputsIn),
                types(typesIn),
                executeFunction(executeFunctionIn),
                checkMatrixSizeFunction(checkMatrixSizeFunctionIn)
                {
        updateStackFun = NULL;
    }

    RuntimeEvaluatorFunction(
            CCString                    nameIn,
            uint16                      numberOfInputsIn,
            uint16                      numberOfOutputsIn,
            const VariableDescriptor *  typesIn,
            Function                    executeFunctionIn,
            UpdateStackFun              updateStackFunIn):
                name(nameIn),
                numberOfInputs(numberOfInputsIn),
                numberOfOutputs(numberOfOutputsIn),
                types(typesIn),
                executeFunction(executeFunctionIn),
                updateStackFun(updateStackFunIn)
                {
        checkMatrixSizeFunction = NULL;
    }


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
    		Stack<AnyType> &					        typeStack,
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
    		Stack<AnyType> &							        typeStack,
			RuntimeEvaluator::DataMemoryAddress & 			    dataStackSize,
    		List<RuntimeEvaluator::VariableInformation> &	    db,
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


/**
 *  @brief Searches the list of functions for one matching the needs
 *  @param[out] code the address within functionRecords to the selected function
 *  @param[in] 	nameIn the function name to be matched
 *  @param[in] 	typeStack the current stack of input types which need to be matched, consumed and replaced with the outputType
 *
 */
ErrorManagement::ErrorType  FindPCode(
		RuntimeEvaluator::CodeMemoryElement &   code,
		CCString 								nameIn,
		Stack<AnyType> &				        typeStack,
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
    static const RuntimeEvaluatorFunction name ## subName ## _FunctionRecord=RuntimeEvaluatorFunction(#name,nInputs,nOutputs,name ## subName ## _FunctionTypes,&function,NULLCheckMatrixSizesFun); \
    static class name ## subName ## RegisterClass { \
    public: name ## subName ## RegisterClass(){\
            RegisterFunction(name ## subName ## _FunctionRecord);\
        }\
    } name ## subName ## RegisterClassInstance;

/**
 * generates boiler plate code to register a matrix processing function
 * allows specifying custom matrix size checks or a custom UpdateStack function
 */
#define REGISTER_PCODE_MATRIX_FUNCTION(name,subName,nInputs,nOutputs,function,checkFunction,...)\
    static const VariableDescriptor name ## subName ## _FunctionTypes[] = {__VA_ARGS__}; \
    static const RuntimeEvaluatorFunction name ## subName ## _FunctionRecord= RuntimeEvaluatorFunction(#name,nInputs,nOutputs,name ## subName ## _FunctionTypes,&function,checkFunction); \
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
