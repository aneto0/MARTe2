/**
 * @file RuntimeEvaluatorMatrixFunctions.cpp
 * @brief Header file for class AnyType
 * @date Aug 14, 2020
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
#include "AnyType.h"
#include "Private/RuntimeEvaluatorFunction.h"
#include <Private/RuntimeEvaluatorVariableInformation.h>

namespace MARTe{




static const VariableDescriptor Float32BitMatrix = VariableDescriptor(Float32Bit,"M");
static const VariableDescriptor Float64BitMatrix = VariableDescriptor(Float64Bit,"M");
static const VariableDescriptor Float32Scalar = VariableDescriptor(Float32Bit);
static const VariableDescriptor Float64Scalar = VariableDescriptor(Float64Bit);


#if 0
ErrorManagement::ErrorType Matrix_Write_UpdateStack(
        const RuntimeEvaluatorFunction &                    ref,
        Stack<AnyType> &                                    typeStack,
        RuntimeEvaluator::DataMemoryAddress &               dataStackSize,
        List<RuntimeEvaluator::VariableInformation> &       db,
        uint32 &                                            dummyID){

    ErrorManagement::ErrorType ret;

    AnyType at1,at2;
    ret = typeStack.Pop(at1);
    REPORT_ERROR(ret,"typeStack.Pop(at1) failed");
    dataStackSize -= RuntimeEvaluator::ByteSizeToDataMemorySize(sizeof(RuntimeEvaluator::DataMemoryAddress));

    if (ret){
        ret = typeStack.Pop(at2);
        REPORT_ERROR(ret,"typeStack.Pop(at2) failed");
        dataStackSize -= RuntimeEvaluator::ByteSizeToDataMemorySize(sizeof(RuntimeEvaluator::DataMemoryAddress));
    }

    if (ret){
        ret.unsupportedFeature = !(at1.GetFullVariableDescriptor().SameAs(at2.GetFullVariableDescriptor()));
        REPORT_ERROR(ret,"error expecting two matrices");
    }

    uint32 nRows        = 1;
    uint32 nColumns     = 1;
    bool isValidMatrix  = false;

    if (ret){
        ret = IsValidMatrix(isValidMatrix,at1,nColumns,nRows);
        REPORT_ERROR(ret,"error first param not valid Matrix");
    }

    if (ret){
        ret.internalSetupError = !isValidMatrix;
        REPORT_ERROR(ret,"error expecting valid matrix as param 1");
    }

    uint32 nRows2        = 1;
    uint32 nColumns2     = 1;
    if (ret){
        ret = IsValidMatrix(isValidMatrix,at2,nColumns2,nRows2);
        REPORT_ERROR(ret,"error second param not valid Matrix");
    }

    if (ret){
        ret.internalSetupError = !isValidMatrix;
        REPORT_ERROR(ret,"error expecting valid matrix as param 2");
    }

    if (ret){
        ret.unsupportedFeature = (nRows != nRows2) || (nColumns != nColumns2);
        REPORT_ERROR(ret,"error expecting matrices of same size");
    }

    return ret;
}
#endif


static ErrorManagement::ErrorType Matrix_Write_CheckSizes(
        Queue<MatrixSize> &                                 inputTypeQueue,
        Queue<MatrixSize> &                                 outputTypeQueue
        ){
    ErrorManagement::ErrorType ret;

    MatrixSize ms1,ms2;
    ret = inputTypeQueue.Remove(ms1);
    REPORT_ERROR(ret,"failed inputTypeQueue.Remove");

    if (ret){
        ret = inputTypeQueue.Remove(ms2);
        REPORT_ERROR(ret,"failed inputTypeQueue.Remove");
    }

    if (ret){
        ret.fatalError = !(ms1  == ms2);
        REPORT_ERROR(ret,"matrix size mismatch");
    }

    return ret;
}

template <typename T> void MatrixWrite(RuntimeEvaluator &context){
    RuntimeEvaluator::DataMemoryAddress  y1;
    context.Pop(y1);
    RuntimeEvaluator::CodeMemoryElement yOut;
    yOut = context.GetPseudoCode();

    Matrix<T> &z1   = context.Variable< Matrix<T> >(y1);
    Matrix<T> &zOut = context.Variable< Matrix<T> >(yOut);
//printf ("[%i %i %f %f %p,%i,%i]\n",z1.GetNumberOfColumns(),z1.GetNumberOfRows(),z1[0][0],z1[1][1],&z1,y1,yOut);

    if (!zOut.Copy(z1)){
        context.runtimeError.internalSetupError = true;
    }
}


REGISTER_PCODE_MATRIX_FUNCTION(WRITE,float32_M,1,0,MatrixWrite<float32>,Matrix_Write_CheckSizes,Float32BitMatrix,Float32BitMatrix)
REGISTER_PCODE_MATRIX_FUNCTION(WRITE,float64_M,1,0,MatrixWrite<float64>,Matrix_Write_CheckSizes,Float64BitMatrix,Float64BitMatrix)

ErrorManagement::ErrorType Matrix_Read_UpdateStack(
        const RuntimeEvaluatorFunction &                    ref,
        Stack<AnyType> &                                    typeStack,
        RuntimeEvaluator::DataMemoryAddress &               dataStackSize,
        List<RuntimeEvaluator::VariableInformation> &       db,
        uint32 &                                            dummyID){

    ErrorManagement::ErrorType ret;

    return ret;
}

#if 0
static ErrorManagement::ErrorType Matrix_Read_CheckSizes(
        Queue<MatrixSize> &                                 inputTypeQueue,
        Queue<MatrixSize> &                                 outputTypeQueue
        ){
    ErrorManagement::ErrorType ret;

#if 0
    MatrixSize ms1;
    ret = inputTypeQueue.Remove(ms1);
    REPORT_ERROR(ret,"failed inputTypeQueue.Remove");

    if (ret){
        ret = outputTypeQueue.Insert(ms1);
        REPORT_ERROR(ret,"failed outputTypeQueue.Insert");
    }
#endif
    return ret;
}
#endif


void MatrixRead(RuntimeEvaluator &context){
    RuntimeEvaluator::CodeMemoryElement index;
    index = context.GetPseudoCode();
//printf("pcode = %i\n",index);
    context.Push(index);
}


REGISTER_PCODE_MATRIX_FUNCTION(READ,float32_M,0,1,MatrixRead,Matrix_Read_UpdateStack,Float32BitMatrix)
REGISTER_PCODE_MATRIX_FUNCTION(READ,float64_M,0,1,MatrixRead,Matrix_Read_UpdateStack,Float64BitMatrix)

#if 0
ErrorManagement::ErrorType Matrix_Scale_UpdateStack(
        const RuntimeEvaluatorFunction &                    ref,
        Stack<AnyType> &                                    typeStack,
        RuntimeEvaluator::DataMemoryAddress &               dataStackSize,
        List<RuntimeEvaluator::VariableInformation> &       db,
        uint32 &                                            dummyID){

    ErrorManagement::ErrorType ret;

    AnyType at1,at2;
    ret = typeStack.Pop(at1);
    REPORT_ERROR(ret,"typeStack.Pop(at1) failed");
    dataStackSize -= RuntimeEvaluator::ByteSizeToDataMemorySize(sizeof(RuntimeEvaluator::DataMemoryAddress));

    if (ret){
        ret = typeStack.Pop(at2);
        REPORT_ERROR(ret,"typeStack.Pop(at2) failed");
        dataStackSize -= RuntimeEvaluator::ByteSizeToDataMemorySize(sizeof(RuntimeEvaluator::DataMemoryAddress));
    }

    if (ret){
        ret.unsupportedFeature = !(at1.GetFullVariableDescriptor().GetFinalTypeDescriptor().SameAs(at2.GetFullVariableDescriptor().GetFinalTypeDescriptor()));
        REPORT_ERROR(ret,"error matrices and scalar not of the same basic type");
    }

    uint32 nRows        = 1;
    uint32 nColumns     = 1;
    bool isValidMatrix  = false;

    if (ret){
        ret = IsValidMatrix(isValidMatrix,at1,nColumns,nRows);
        REPORT_ERROR(ret,"error first param not valid Matrix");
    }

    if (ret){
        ret.internalSetupError = !isValidMatrix;
        REPORT_ERROR(ret,"error expecting valid matrix as param 1");
    }

    // prepare the space for the intermediate results
    RuntimeEvaluator::VariableInformation variableInformation;

    if (ret){
        dataStackSize += sizeof(RuntimeEvaluator::DataMemoryAddress);

        // need to allocate variable and memory
        DynamicCString name;
        name().Append("Temp").Append('@').Append(dummyID++);
        variableInformation.SetName(name);
        // extracts matrix size from at1
        ret = variableInformation.SetType(at1);
        REPORT_ERROR(ret,"error setting type of variableInformation");
    }

    if (ret){
        ret = variableInformation.AllocateMatrixMemory();
        REPORT_ERROR(ret,"error variableInformation external memory");
    }
    if (ret){
        // should move memory management responsibility to new member of database
        ret = db.Insert(variableInformation );
    }

    return ret;
}
#endif

static ErrorManagement::ErrorType Matrix_Scale_CheckSizes(
        Queue<MatrixSize> &                                 inputTypeQueue,
        Queue<MatrixSize> &                                 outputTypeQueue
        ){
    ErrorManagement::ErrorType ret;

//printf("\[nsize = %i ",inputTypeQueue.Size());
    MatrixSize ms1;
    ret = inputTypeQueue.Remove(ms1);
    REPORT_ERROR(ret,"failed inputTypeQueue.Remove");
//printf("  size = %i]\n",inputTypeQueue.Size());

    if (ret){
        ret = outputTypeQueue.Insert(ms1);
        REPORT_ERROR(ret,"failed outputTypeQueue.Insert");
    }

    return ret;
}

template <typename T> void Matrix_Scale(RuntimeEvaluator &context){
    RuntimeEvaluator::DataMemoryAddress  y;
    T a;
    context.Pop(y);
    context.Pop(a);
    RuntimeEvaluator::CodeMemoryElement yOut;
    yOut = context.GetPseudoCode();

    Matrix<T> &z   = context.Variable< Matrix<T> >(y);
    Matrix<T> &zOut = context.Variable< Matrix<T> >(yOut);

    if (!z.Product(a,zOut)){
        context.runtimeError.internalSetupError = true;
    }
    context.Push(yOut);
}

template <typename T> void Matrix_Scale_R(RuntimeEvaluator &context){
    RuntimeEvaluator::DataMemoryAddress  y;
    T a;
    context.Pop(a);
    context.Pop(y);

    RuntimeEvaluator::CodeMemoryElement yOut;
    yOut = context.GetPseudoCode();

    Matrix<T> &z   = context.Variable< Matrix<T> >(y);
    Matrix<T> &zOut = context.Variable< Matrix<T> >(yOut);

//    printf("y = %i yOut = %i  a = %f\n",y,yOut,a);

    if (!z.Product(a,zOut)){
        context.runtimeError.internalSetupError = true;
    }
    context.Push(yOut);
}

REGISTER_PCODE_MATRIX_FUNCTION(MUL,float32_M ,2,1,Matrix_Scale<float32>  ,Matrix_Scale_CheckSizes,Float32BitMatrix,Float32Scalar,Float32BitMatrix)
REGISTER_PCODE_MATRIX_FUNCTION(MUL,float64_M ,2,1,Matrix_Scale<float64>  ,Matrix_Scale_CheckSizes,Float64BitMatrix,Float64Scalar,Float64BitMatrix)
REGISTER_PCODE_MATRIX_FUNCTION(MUL,float32_MR,2,1,Matrix_Scale_R<float32>,Matrix_Scale_CheckSizes,Float32Scalar,Float32BitMatrix,Float32BitMatrix)
REGISTER_PCODE_MATRIX_FUNCTION(MUL,float64_MR,2,1,Matrix_Scale_R<float64>,Matrix_Scale_CheckSizes,Float64Scalar,Float64BitMatrix,Float64BitMatrix)

#if 0
ErrorManagement::ErrorType Matrix_Addition_UpdateStack(
        const RuntimeEvaluatorFunction &                    ref,
        Stack<AnyType> &                                    typeStack,
        RuntimeEvaluator::DataMemoryAddress &               dataStackSize,
        List<RuntimeEvaluator::VariableInformation> &       db,
        uint32 &                                            dummyID){

    ErrorManagement::ErrorType ret;

    AnyType at1,at2;
    ret = typeStack.Pop(at1);
    REPORT_ERROR(ret,"typeStack.Pop(at1) failed");
    dataStackSize -= RuntimeEvaluator::ByteSizeToDataMemorySize(sizeof(RuntimeEvaluator::DataMemoryAddress));

    if (ret){
        ret = typeStack.Pop(at2);
        REPORT_ERROR(ret,"typeStack.Pop(at2) failed");
        dataStackSize -= RuntimeEvaluator::ByteSizeToDataMemorySize(sizeof(RuntimeEvaluator::DataMemoryAddress));
    }

    if (ret){
        ret.unsupportedFeature = !(at1.GetFullVariableDescriptor().SameAs(at2.GetFullVariableDescriptor()));
        REPORT_ERROR(ret,"error expecting two matrices");
    }

    uint32 nRows        = 1;
    uint32 nColumns     = 1;
    bool isValidMatrix  = false;

    if (ret){
        ret = IsValidMatrix(isValidMatrix,at1,nColumns,nRows);
        REPORT_ERROR(ret,"error first param not valid Matrix");
    }

    if (ret){
        ret.internalSetupError = !isValidMatrix;
        REPORT_ERROR(ret,"error expecting valid matrix as param 1");
    }

    uint32 nRows2        = 1;
    uint32 nColumns2     = 1;
    if (ret){
        ret = IsValidMatrix(isValidMatrix,at2,nColumns2,nRows2);
        REPORT_ERROR(ret,"error second param not valid Matrix");
    }

    if (ret){
        ret.internalSetupError = !isValidMatrix;
        REPORT_ERROR(ret,"error expecting valid matrix as param 2");
    }

    if (ret){
        ret.unsupportedFeature = (nRows != nRows2) || (nColumns != nColumns2);
        REPORT_ERROR(ret,"error expecting matrices of same size");
    }

    // prepare the space for the intermediate results
    RuntimeEvaluator::VariableInformation variableInformation;

    if (ret){
        dataStackSize += sizeof(RuntimeEvaluator::DataMemoryAddress);

        // need to allocate variable and memory
        DynamicCString name;
        name().Append("Temp").Append('@').Append(dummyID++);
        variableInformation.SetName(name);
        // extracts matrix size from at1
        ret = variableInformation.SetType(at1);
        REPORT_ERROR(ret,"error setting type of variableInformation");
    }

    if (ret){
        ret = variableInformation.AllocateMatrixMemory();
        REPORT_ERROR(ret,"error variableInformation external memory");
    }
#if 0
    if (ret){
        AnyType at(at1.GetFullVariableDescriptor(),);
        ret = typeStack.Push(at);
        REPORT_ERROR(ret,"error adding result type to stack");
    }
#endif
    if (ret){
        // should move memory management responsibility to new member of database
        ret = db.Insert(variableInformation );
    }

    return ret;
}
#endif

static ErrorManagement::ErrorType Matrix_Addition_CheckSizes(
        Queue<MatrixSize> &                                 inputTypeQueue,
        Queue<MatrixSize> &                                 outputTypeQueue
        ){
    ErrorManagement::ErrorType ret;

    MatrixSize ms1,ms2;
    ret = inputTypeQueue.Remove(ms1);
    REPORT_ERROR(ret,"failed inputTypeQueue.Remove");

    if (ret){
        ret = inputTypeQueue.Remove(ms2);
        REPORT_ERROR(ret,"failed inputTypeQueue.Remove");
    }

    if (ret){
        ret.fatalError = !(ms1  == ms2);
        REPORT_ERROR(ret,"matrix size mismatch");
    }

    if (ret){
        ret = outputTypeQueue.Insert(ms1);
        REPORT_ERROR(ret,"failed outputTypeQueue.Insert");
    }


    return ret;
}

template <typename T> void Matrix_Addition(RuntimeEvaluator &context){
    RuntimeEvaluator::DataMemoryAddress  y1,y2;
    context.Pop(y1);
    context.Pop(y2);
    RuntimeEvaluator::CodeMemoryElement yOut;
    yOut = context.GetPseudoCode();

    Matrix<T> &z1   = context.Variable< Matrix<T> >(y1);
    Matrix<T> &z2   = context.Variable< Matrix<T> >(y2);
    Matrix<T> &zOut = context.Variable< Matrix<T> >(yOut);

    if (!z1.Sum(z2,zOut)){
        context.runtimeError.internalSetupError = true;
    }
    context.Push(yOut);
}

REGISTER_PCODE_MATRIX_FUNCTION(ADD,float32_M,2,1,Matrix_Addition<float32>,Matrix_Addition_CheckSizes,Float32BitMatrix,Float32BitMatrix,Float32BitMatrix)
REGISTER_PCODE_MATRIX_FUNCTION(ADD,float64_M,2,1,Matrix_Addition<float64>,Matrix_Addition_CheckSizes,Float64BitMatrix,Float64BitMatrix,Float64BitMatrix)


} //MARTe
