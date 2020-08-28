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

    dataStackSize++;

    return ret;
}


void MatrixRead(RuntimeEvaluator &context){
    RuntimeEvaluator::CodeMemoryElement index;
    index = context.GetPseudoCode();
    context.Push(index);
}

REGISTER_PCODE_MATRIX_FUNCTION(READ,float32_M,0,1,MatrixRead,Matrix_Read_UpdateStack,Float32BitMatrix)
REGISTER_PCODE_MATRIX_FUNCTION(READ,float64_M,0,1,MatrixRead,Matrix_Read_UpdateStack,Float64BitMatrix)


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

    if (!z2.Sum(z1,zOut)){
        context.runtimeError.internalSetupError = true;
    }
    context.Push(yOut);
}

REGISTER_PCODE_MATRIX_FUNCTION(ADD,float32_M,2,1,Matrix_Addition<float32>,Matrix_Addition_CheckSizes,Float32BitMatrix,Float32BitMatrix,Float32BitMatrix)
REGISTER_PCODE_MATRIX_FUNCTION(ADD,float64_M,2,1,Matrix_Addition<float64>,Matrix_Addition_CheckSizes,Float64BitMatrix,Float64BitMatrix,Float64BitMatrix)

static ErrorManagement::ErrorType Matrix_Scale_CheckSizes(
        Queue<MatrixSize> &                                 inputTypeQueue,
        Queue<MatrixSize> &                                 outputTypeQueue
        ){
    ErrorManagement::ErrorType ret;

    MatrixSize ms1;
    ret = inputTypeQueue.Remove(ms1);
    REPORT_ERROR(ret,"failed inputTypeQueue.Remove");

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

    if (!z.Product(a,zOut)){
        context.runtimeError.internalSetupError = true;
    }
    context.Push(yOut);
}

REGISTER_PCODE_MATRIX_FUNCTION(MUL,S_float32_M ,2,1,Matrix_Scale<float32>  ,Matrix_Scale_CheckSizes,Float32BitMatrix,Float32Scalar,Float32BitMatrix)
REGISTER_PCODE_MATRIX_FUNCTION(MUL,S_float64_M ,2,1,Matrix_Scale<float64>  ,Matrix_Scale_CheckSizes,Float64BitMatrix,Float64Scalar,Float64BitMatrix)
REGISTER_PCODE_MATRIX_FUNCTION(MUL,S_float32_MR,2,1,Matrix_Scale_R<float32>,Matrix_Scale_CheckSizes,Float32Scalar,Float32BitMatrix,Float32BitMatrix)
REGISTER_PCODE_MATRIX_FUNCTION(MUL,S_float64_MR,2,1,Matrix_Scale_R<float64>,Matrix_Scale_CheckSizes,Float64Scalar,Float64BitMatrix,Float64BitMatrix)

static ErrorManagement::ErrorType Matrix_Product_CheckSizes(
        Queue<MatrixSize> &                                 inputTypeQueue,
        Queue<MatrixSize> &                                 outputTypeQueue
        ){
    ErrorManagement::ErrorType ret;

    MatrixSize ms1,ms2,mso;
    ret = inputTypeQueue.Remove(ms1);
    REPORT_ERROR(ret,"failed inputTypeQueue.Remove");

    if (ret){
        ret = inputTypeQueue.Remove(ms2);
        REPORT_ERROR(ret,"failed inputTypeQueue.Remove");
    }

    if (ret){
        ret.fatalError = !(ms2.Product(ms1,mso));
        REPORT_ERROR(ret,"matrix sizes incompatible");
    }

    if (ret){
        ret = outputTypeQueue.Insert(mso);
        REPORT_ERROR(ret,"failed outputTypeQueue.Insert");
    }
    return ret;
}

template <typename T> void Matrix_Product(RuntimeEvaluator &context){
    RuntimeEvaluator::DataMemoryAddress  y1,y2;
    context.Pop(y1);
    context.Pop(y2);
    RuntimeEvaluator::CodeMemoryElement yOut;
    yOut = context.GetPseudoCode();

    Matrix<T> &z1   = context.Variable< Matrix<T> >(y1);
    Matrix<T> &z2   = context.Variable< Matrix<T> >(y2);
    Matrix<T> &zOut = context.Variable< Matrix<T> >(yOut);

    if (!z2.Product(z1,zOut)){
        context.runtimeError.internalSetupError = true;
    }
    context.Push(yOut);
}

REGISTER_PCODE_MATRIX_FUNCTION(MUL,float32_M,2,1,Matrix_Product<float32>,Matrix_Product_CheckSizes,Float32BitMatrix,Float32BitMatrix,Float32BitMatrix)
REGISTER_PCODE_MATRIX_FUNCTION(MUL,float64_M,2,1,Matrix_Product<float64>,Matrix_Product_CheckSizes,Float64BitMatrix,Float64BitMatrix,Float64BitMatrix)

} //MARTe
