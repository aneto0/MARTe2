/**
 * @file RuntimeEvaluatorFunctions.cpp
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

#include <Private/RuntimeEvaluatorFunction.h>
#include <Private/RuntimeEvaluatorVariableInformation.h>
#include <RuntimeEvaluator.h>
#include "ErrorManagement.h"
#include "RuntimeEvaluator.h"
#include "AnyType.h"


namespace MARTe{

uint32 availableFunctions = 0;

RuntimeEvaluatorFunction functionRecords[maxFunctions];

ErrorManagement::ErrorType  RuntimeEvaluatorFunction::Check(
		CCString 									nameIn,
		Stack<AnyType> &					        typeStack,
		bool 										matchOutput
){
	ErrorManagement::ErrorType ret;

	// match function name
    ret.comparisonFailure = !(name == nameIn);

    AnyType at;

    // match first output if matchOutput is set
    uint32 index = 0U;
    if (ret && matchOutput){
        ret = typeStack.Peek(at,index++);
        COMPOSITE_REPORT_ERROR(ret,"typeStack.Peek(at,",index-1,") failed");
    }
    VariableDescriptor vd;
    if (ret && matchOutput){
        vd = at.GetFullVariableDescriptor();
        ret.comparisonFailure = !types[numberOfInputs].SameAs(vd);
    }

    // match inputs types
    for (uint32 i = 0U; ret && (i < numberOfInputs); i++){
        ret = typeStack.Peek(at,index++);
        COMPOSITE_REPORT_ERROR(ret,"typeStack.Peek(vd,",index-1,") failed");
        if (ret){
            vd = at.GetFullVariableDescriptor();
            ret.comparisonFailure = !vd.SameAs(types[i]);
        }
    }

    return ret;
}

/**
 * to register a function
 */
void RegisterFunction(const RuntimeEvaluatorFunction &record){
    if (availableFunctions < maxFunctions){
        functionRecords[availableFunctions++] = record;
    }
}

ErrorManagement::ErrorType  FindPCode(
		RuntimeEvaluator::CodeMemoryElement &   code,
		CCString 					            nameIn,
		Stack<AnyType> &				        typeStack,
		bool 								    matchOutput
)
{
	ErrorManagement::ErrorType ret;

    RuntimeEvaluator::CodeMemoryElement i = 0;
    code = RuntimeEvaluator::InvalidCodeMemoryElement;
    for (i=0; ret && (i < availableFunctions);i++ ){
//        ret = functionRecords[i].TryConsume(nameIn,typeStack,matchOutput,dataStackSize);
    	ret = functionRecords[i].Check(nameIn,typeStack,matchOutput);

    	if (ret){
            code = i;
            // force exit
            i = availableFunctions;
    	} else {
            ret.comparisonFailure = false;
    	}
    }
    ret.comparisonFailure = (code == RuntimeEvaluator::InvalidCodeMemoryElement);

    return ret;
}



static inline ErrorManagement::ErrorType IsValidMatrix(bool& isValidMatrix,const AnyType &at,MatrixSize *matrixSize=NULL){
    ErrorManagement::ErrorType ret;
    VariableDescriptor vd = at.GetFullVariableDescriptor();
    CCString modifiers = CCString(vd.GetModifiers());

    // to avoid accessing NULL ptrs
    isValidMatrix = (modifiers.GetSize() != 0);

    if (isValidMatrix){
        isValidMatrix = (( modifiers == "M") || (modifiers == "m"));
    }

    if (isValidMatrix){
        isValidMatrix = ((vd.GetFinalTypeDescriptor().SameAs(Float32Bit)) || (vd.GetFinalTypeDescriptor().SameAs(Float64Bit)));
    }

    if (isValidMatrix && (matrixSize != NULL)){
        ret.internalSetupError = (at.GetVariablePointer() == NULL);
    }

    if (isValidMatrix && ret && (matrixSize != NULL)){
        const Matrix<float> *mf = reinterpret_cast<const Matrix<float> *> (at.GetVariablePointer());
        *matrixSize = MatrixSize(mf->GetNumberOfRows(),mf->GetNumberOfColumns());
    }

    return ret;
}


ErrorManagement::ErrorType RuntimeEvaluatorFunction::UpdateStack(
        Stack<AnyType> &                                    typeStack,
        RuntimeEvaluator::DataMemoryAddress &               dataStackSize,
        List<RuntimeEvaluator::VariableInformation> &       intermediateResultVariablesdb,
        bool                                                matchOutput,
        uint32 &                                            dummyID){

    ErrorManagement::ErrorType ret;

    if (updateStackFun != NULL){
        return updateStackFun(*this,typeStack,dataStackSize,intermediateResultVariablesdb,dummyID);
    }

    Queue<MatrixSize> mSizeInputQueue;
    Queue<MatrixSize> mSizeOutputQueue;
//printf("\n [S] size = %i \n",mSizeInputQueue.Size());

    // remove first element which is the matched output type
    if (ret && matchOutput){
        AnyType at;
        ret = typeStack.Pop(at);
        REPORT_ERROR(ret,"typeStack.Pop(at) failed");

        // check if it is a matrix
        bool isValidMatrix = false;
        MatrixSize matrixSize;

        if (ret){
            ret = IsValidMatrix(isValidMatrix,at,&matrixSize);
            REPORT_ERROR(ret,"error in IsValidMatrix");
        }

        if (ret && isValidMatrix){
            ret = mSizeInputQueue.Insert(matrixSize);
            REPORT_ERROR(ret,"failed mSizeStack.Insert ");
        }
    }

    // remove inputs types
    for (uint32 i = 0U; ret && (i < numberOfInputs); i++){
        AnyType at;
        ret = typeStack.Pop(at);
        REPORT_ERROR(ret,"typeStack.Pop(at) failed");

        // check if it is a matrix
        bool isValidMatrix = false;
        MatrixSize matrixSize;

        if (ret){
            ret = IsValidMatrix(isValidMatrix,at,&matrixSize);
            REPORT_ERROR(ret,"error in IsValidMatrix");
        }

        if (ret && isValidMatrix){
            ret = mSizeInputQueue.Insert(matrixSize);
            REPORT_ERROR(ret,"failed mSizeStack.Insert ");
        }

        if (ret && isValidMatrix){
            dataStackSize -= RuntimeEvaluator::ByteSizeToDataMemorySize(sizeof(RuntimeEvaluator::DataMemoryAddress));
        }

        if (ret && !isValidMatrix){
            dataStackSize -= RuntimeEvaluator::ByteSizeToDataMemorySize(at.GetFullVariableDescriptor().GetSummaryTypeDescriptor().StorageSize());
        }
    }
//printf("\n [X] size = %i \n",mSizeInputQueue.Size());

    // custom processing of matrix size
    if (ret && (checkMatrixSizeFunction != NULL)){
        ret = checkMatrixSizeFunction(mSizeInputQueue,mSizeOutputQueue);
        REPORT_ERROR(ret,"checkMatrixSizeFunction failed ");
    }
//printf("\n [E] size = %i \n",mSizeInputQueue.Size());

    // default processing is that all the matrices shall have same dimensions and there is only one output
    if (ret && (checkMatrixSizeFunction == NULL) && (mSizeInputQueue.Size() > 0)){

        MatrixSize matrixSize;
        ret = mSizeInputQueue.Remove(matrixSize);
        COMPOSITE_REPORT_ERROR(ret,this->name,": failed mSizeStack.Remove ");

        while(ret && (mSizeInputQueue.Size() > 0)){
            MatrixSize matrixSize2;
            ret = mSizeInputQueue.Remove(matrixSize2);
            COMPOSITE_REPORT_ERROR(ret,this->name,": failed mSizeStack.Remove ");

            if (ret){
                ret.internalSetupError = !(matrixSize == matrixSize2);
                COMPOSITE_REPORT_ERROR(ret,this->name,": size mismatch among matrices ");
            }
        }

        // the output matrix size would be the same as the input
        if (ret){
            ret = mSizeOutputQueue.Insert(matrixSize);
            COMPOSITE_REPORT_ERROR(ret,this->name,": failed mSizeStack.Push ");
        }
    }

    // check that we did not have a mismatch between number of expected output matrices and their sizes
    if (ret){
        ret.internalSetupError = (mSizeInputQueue.Size() != 0);
        COMPOSITE_REPORT_ERROR(ret,this->name,": error mSizeInputQueue size is not 0");
    }

    // insert output types
    for (uint32 i = 0U; ret && (i < numberOfOutputs); i++){
        VariableDescriptor vd = types[i+numberOfInputs];

        // check if this parameter is a matrix
        bool isValidMatrix = true;
        AnyType at(vd,NULL);
        if (ret){
            ret = IsValidMatrix(isValidMatrix,at);
            REPORT_ERROR(ret,"cannot handle matrices as output");
        }

        MatrixSize ms;
        bool hasSize = false;
        if (ret && isValidMatrix){
            // get the size of the temp matrix to produce
            hasSize = mSizeOutputQueue.Remove(ms);
        }

        // need to create a temporary variable
        if (ret && hasSize){
            // prepare the space for the intermediate results
            RuntimeEvaluator::VariableInformation variableInformation;

            COMPOSITE_REPORT_ERROR(ret,this->name,": failed mSizeOutputQueue.Remove ");

            if (ret){
                // need to allocate variable and memory
                DynamicCString name;
                name().Append("Temp").Append('@').Append(dummyID++);

                ret = variableInformation.CreateTemporaryResultMatrix(name,ms,vd.GetFinalTypeDescriptor());
                REPORT_ERROR(ret,"error CreateTemporaryResultMatrix()");
            }

            if (ret){
                // should move memory management responsibility to new member of database
                ret = intermediateResultVariablesdb.Insert(variableInformation );
            }
        }

        // check that we did not have a mismatch between number of expected output matrices and their sizes
        if (ret){
            ret.internalSetupError = (mSizeOutputQueue.Size() != 0);
            REPORT_ERROR(ret,"error mSizeOutputQueue size is not 0");
        }

        // if it is a matrix then we need to indicate the need of a temporary variable
        // allocation will be performed later in the calling function
        // also p-code injection will be performed later
        if (ret && isValidMatrix){
            // we know that the address of the matrix will be produced on the stack
//            dataStackSize += sizeof(RuntimeEvaluator::DataMemoryAddress);
            dataStackSize += RuntimeEvaluator::ByteSizeToDataMemorySize(sizeof(RuntimeEvaluator::DataMemoryAddress));

        } else {
            ret = typeStack.Push(AnyType(vd,NULL));
            COMPOSITE_REPORT_ERROR(ret,this->name,": typeStack.Push(..) failed");

            dataStackSize += RuntimeEvaluator::ByteSizeToDataMemorySize(vd.GetSummaryTypeDescriptor().StorageSize());
        }

    }

    return ret;
}



} //MARTe
