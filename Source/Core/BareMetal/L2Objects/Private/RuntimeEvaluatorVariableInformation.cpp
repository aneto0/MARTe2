/**
 * @file RuntimeEvaluatorVariableInformation.cpp
 * @brief Header file for class AnyType
 * @date Jun 18, 2020
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
#include "Private/RuntimeEvaluatorVariableInformation.h"
#include "RuntimeEvaluator.h"
#include "AnyObject.h"



namespace MARTe{


RuntimeEvaluator::VariableInformation::VariableInformation(){
    type                       =   VoidType;
    location                   =   InvalidDataMemoryAddress;
    externalMemory             =   NULL;
    dimensions.numberOfColumns =  1;
    dimensions.numberOfRows    =  1;
}


RuntimeEvaluator::VariableInformation::VariableInformation(CCString nameIn, const VariableDescriptor &vd){
    name            = nameIn;
    type            = vd;
    location        = InvalidDataMemoryAddress;
    externalMemory  = NULL;
}

RuntimeEvaluator::VariableInformation::VariableInformation(const VariableInformation &vi){
    *this = vi;
}

void RuntimeEvaluator::VariableInformation::operator= (const VariableInformation &vi){
    name                    =   vi.name;
    type                    =   vi.type;
    location                =   vi.location;
    externalMemory          =   vi.externalMemory;
    dimensions              =   vi.dimensions;
    memory                  =   vi.memory;
}


RuntimeEvaluator::VariableInformation::~VariableInformation(){
    Free();
}


void RuntimeEvaluator::VariableInformation::Free(){
    memory.RemoveReference();
    externalMemory = NULL;
}

AnyType RuntimeEvaluator::VariableInformation::GetAnyType( RuntimeEvaluator &context) const{
    AnyType at;

    if (location == InvalidDataMemoryAddress){
        at = AnyType(type,NULL);
    } else
    if (externalMemory == NULL){
        at = AnyType(type,&context.Variable<int>(location));
    } else {
        if (CCString(type.GetModifiers()).GetSize() == 0){
            at = AnyType(type,externalMemory);
        } else {
            at = AnyType(type,&context.Variable<int>(location));
        }
    }

    return at;
}


ErrorManagement::ErrorType RuntimeEvaluator::VariableInformation::AllocateMatrixMemory(){
    ErrorManagement::ErrorType ret;

    Free();

    SaturatedInteger<uint32>  size;
    size = type.GetFinalTypeDescriptor().StorageSize();
    size *= dimensions.numberOfColumns;
    size *= dimensions.numberOfRows;
    uint32 bytesToAllocate;
    ret = size.ToNumber(bytesToAllocate);
    REPORT_ERROR(ret,"size overflow");

    if (ret){
        memory = AnyObject::Allocate(bytesToAllocate,type);
        ret.outOfMemory = !memory.IsValid();
        REPORT_ERROR(ret,"allocation failure");
    }

    if (ret){
        externalMemory = memory.operator ->();
    }

/*
    void * address;
    if (ret){
        address = HeapManager::Malloc(bytesToAllocate);
        ret.outOfMemory = (address == NULL);
    }

    if (ret){
        externalMemory = address;
        memoryAllocated = true;
    }
*/
    return ret;
}


ErrorManagement::ErrorType RuntimeEvaluator::VariableInformation::CreateTemporaryResultMatrix(CCString nameIn,const MatrixSize & ms,const TypeDescriptor &td){
    ErrorManagement::ErrorType ret;

    bool isVoid;
    ret = IsValidVoid(isVoid);

    if (ret){
        ret.unsupportedFeature = !isVoid;
        REPORT_ERROR(ret,"already initialised");
    }

    if (ret){
        type = VariableDescriptor(td,"M");
        externalMemory = NULL;
        dimensions = ms;
        ret = AllocateMatrixMemory();
        REPORT_ERROR(ret,"AllocateMatrixMemory failed");
    }

    if (ret){
        name = nameIn;
    }

    return ret;
}

ErrorManagement::ErrorType  RuntimeEvaluator::VariableInformation::SetType( AnyType at){
    ErrorManagement::ErrorType ret;

    bool isVoid;
    ret = IsValidVoid(isVoid);

    if (ret){
        ret.unsupportedFeature = !isVoid;
        REPORT_ERROR(ret,"already initialised");
    }

    if (ret){
        type = at.GetFullVariableDescriptor();
        externalMemory = const_cast<void *>(at.GetVariablePointer());
    }

    if (IsValidMatrix() && ret){
        ret.internalSetupError = (externalMemory == NULL);
        REPORT_ERROR(ret,"Matrix type without address");
        if (ret){
            const Matrix<float> *mf = reinterpret_cast<const Matrix<float> *>(externalMemory);
            dimensions = MatrixSize(*mf);
            externalMemory = mf->GetDataPointer();

            // TODO - probably not needed
            ret.internalSetupError = (externalMemory == NULL);
            REPORT_ERROR(ret,"referenced Matrix has NULL address");
        }
    } else

    // in case of a[n][m] transform into Matrix
    // external memory
    if (IsValidArray2D() && ret){
        type = VariableDescriptor(at.GetFullVariableDescriptor().GetFinalTypeDescriptor(),"M") ;

        // TODO - probably not needed
        ret.internalSetupError = (externalMemory == NULL);
        REPORT_ERROR(ret,"2D array type with no address");
printf("[%i][%i]\n",dimensions.numberOfColumns,dimensions.numberOfRows);
    }

    return ret;
}


bool RuntimeEvaluator::VariableInformation::IsValidNumber(){
    TypeDescriptor td       = type.GetFinalTypeDescriptor();
    CCString modifiers      = type.GetModifiers();

    bool ret = (externalMemory == NULL);

    if (ret){
        ret = td.IsNumericType();
    }

    if (ret){
        ret = (modifiers.GetSize() == 0);
    }

    return ret;
}

ErrorManagement::ErrorType RuntimeEvaluator::VariableInformation::IsValidVoid(bool& isVoid){
    ErrorManagement::ErrorType ret;
    TypeDescriptor td       = type.GetFinalTypeDescriptor();
    CCString modifiers      = type.GetModifiers();

    isVoid = td.SameAs(VoidType) ;

    if (isVoid){
        isVoid = (modifiers.GetSize() == 0);
    }

    ret.internalSetupError = (externalMemory != NULL) && isVoid;

    return ret;
}
// TODO - probably not needed

bool RuntimeEvaluator::VariableInformation::IsValidReferencedNumber(){
    TypeDescriptor td       = type.GetFinalTypeDescriptor();
    CCString modifiers      = type.GetModifiers();

    bool ret = (externalMemory != NULL);

    if (ret){
        ret = td.IsNumericType();
    }

    if (ret){
        ret = (modifiers.GetSize() == 0);
    }

    return ret;
}


bool RuntimeEvaluator::VariableInformation::IsValidMatrix(){
    CCString modifiers = CCString(type.GetModifiers());
    bool isMatrix = (( modifiers == "M") || (modifiers == "m"));
    bool isFloat = ((type.GetFinalTypeDescriptor().SameAs(Float32Bit)) || (type.GetFinalTypeDescriptor().SameAs(Float64Bit)));

    return (isMatrix && isFloat);
}

static inline int toNumber(char8 c){
    int ret = -1;
    if ((c >= '0') && (c<='9')){
        ret = static_cast<int>(c - '0');
    }
    return ret;
}

bool RuntimeEvaluator::VariableInformation::IsValidArray2D(){
    const char8 *  modifiers = type.GetModifiers();
    bool isArray2D = (modifiers != NULL);

    if (isArray2D){
        isArray2D = (modifiers[0] == 'A');
        modifiers++;
    }

    uint32 nRows = 0;
    if (isArray2D){
        int digit = 0;
        while ((digit = toNumber(modifiers[0]))> 0 ){
            nRows = nRows * 10 + static_cast<uint32>(digit);
            modifiers++;
        }
    }

    if (isArray2D){
        isArray2D = (nRows  > 0);
    }

    if (isArray2D){
        isArray2D = (modifiers[0] == 'A');
        modifiers++;
    }

    uint32 nColumns = 0;
    if (isArray2D){
        int digit = 0;
        while ((digit = toNumber(modifiers[0]))> 0 ){
            nColumns = nColumns * 10 + static_cast<uint32>(digit);
            modifiers++;
        }
    }

    if (isArray2D){
        isArray2D = (nColumns  > 0);
    }

    if (isArray2D){
        isArray2D = ((type.GetFinalTypeDescriptor().SameAs(Float32Bit)) || (type.GetFinalTypeDescriptor().SameAs(Float64Bit)));
    }

    if (isArray2D){
        dimensions.numberOfRows = nRows;
        dimensions.numberOfColumns = nColumns;
    }

    return (isArray2D);
}



} //MARTe
