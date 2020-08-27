/**
 * @file RuntimeEvaluatorVariableInformation.h
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

#ifndef RUNTIMEEVALUATORVARIABLEINFORMATION_H_
#define RUNTIMEEVALUATORVARIABLEINFORMATION_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "RuntimeEvaluator.h"
#include "AnyType.h"
#include "AnyObject.h"
#include "VariableDescriptor.h"
#include "Private/MatrixSize.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 *
 */
class RuntimeEvaluator::VariableInformation {
public:

     /**
         RuntimeEvaluatorFunction &fr = functionRecords[pCode];
      * initialises data
      */
                                        VariableInformation();

     /**
      * frees memory if necessary
      */
                                        ~VariableInformation();

     /**
      * sets name and type
     */
                                        VariableInformation(CCString name,const VariableDescriptor &vd);

     /**
      * copies from and takes memory freeing responsibilities from in
      */
                                        VariableInformation(const VariableInformation &vi);

     /**
      * allocates memory
      */
     ErrorManagement::ErrorType         AllocateMatrixMemory();

     /**
      *
      */
     inline bool                        IsAllocated();

     /**
      *
      */
     bool                               IsValidMatrix();

     /**
      * a number without a reference to an external memory
      */
     bool                               IsValidNumber();

     /**
      * a number with a reference to an external memory
      */
     bool                               IsValidReferencedNumber();

     /**
      * externalMemory is NULL and Type is void
      */
     ErrorManagement::ErrorType         IsValidVoid(bool& isVoid);

     /**
      *
      */
     inline void *                      GetExternalMemoryPtr();

     /**
      *
      */
     inline CCString                    GetName() const;

     /**
      *
      */
     inline void                        SetName(CCString name);

     /**
      *
      */
     inline DataMemoryAddress &         GetLocation() ;
     /**
      *
      */
     inline MatrixSize                  GetDimension();

     /**
      *
      */
     inline const VariableDescriptor &  GetType() const;

     /**
      *
      */
     AnyType                            GetAnyType(RuntimeEvaluator &context) const;

     /**
      * copies the type information from at.
      * in case of matrices it uses the pointer to the Matrix record to recover the dimensions
      * in case of numbers it used the pointer to indicate external reference if not NULL
      * only usable if the VariableInformation is still set as void
      * NOTE on the following use cases
      * SetType(variable)  --> will use variable type and refer to the address of variable
      * SetType(AnyType(VariableDescriptor(variable),NULL) --> will use the variable type
      * SetType(AnyType(TypeDescriptor(xxxType),NULL) --> will use the type xxxType
      * SetType(VariableDescriptor(variable)) --> will set the type to VariableDescriptor !!
      * SetType(TypeDescriptor(xxxType)) --> will set the type to TypeDescriptor !!
      */
     ErrorManagement::ErrorType         SetType(AnyType at);


     /**
      * setups the VariableInformation for an empty matrix
      * memory will be allocated using AllocateMatrixMemory according to the TypeDescriptor and the MatrixSize
      */
     ErrorManagement::ErrorType         CreateTemporaryResultMatrix(CCString name,const MatrixSize & ms,const TypeDescriptor &td);


     /* prevent use of = */
     void                               operator= (const VariableInformation &vi);

private:

     /**
      * Detects a Variable description pattern AnnnAmmm
      * On success extracts nnn and mmm and initialises dimensions
      */
     bool                               IsValidArray2D();

     /**
      *
      */
     void                               Free();

     /**
      * name of the variable.
      */
     DynamicCString                     name;

     /**
      * either a numeric type or a Matrix<float> or Matrix<double>
      */
     VariableDescriptor                 type;

     /**
      * location of the variable in the variable area.
      * if location is below the start of the variables then
      * it is a constant
      */
     DataMemoryAddress                  location;

     /**
      * NULL if the variable is local
      * the address of a referenced scalar value (int* uint* float*)
      * the address of a remotely address array for Matrix<>  if memoryAllocated = false
      * the address of a local  Matrix<>  if memoryAllocated = true
      */
     void *                             externalMemory;

     /**
      *
      */
     MatrixSize                         dimensions;


     /**
      * keep a reference to the memory allocated
      * so when all the copies of this object are deleted this also is deallocated
      */
     Reference                          memory;
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

bool RuntimeEvaluator::VariableInformation::IsAllocated(){
    return (location != InvalidDataMemoryAddress);
}


void *RuntimeEvaluator::VariableInformation::GetExternalMemoryPtr(){
    return externalMemory;
}

CCString  RuntimeEvaluator::VariableInformation::GetName() const{
    return name;
}

void RuntimeEvaluator::VariableInformation::SetName(CCString nameIn){
    name = nameIn;
}


const VariableDescriptor & RuntimeEvaluator::VariableInformation::GetType() const{
    return type;
}


RuntimeEvaluator::DataMemoryAddress & RuntimeEvaluator::VariableInformation::GetLocation(){
    return location;
}

MatrixSize RuntimeEvaluator::VariableInformation::GetDimension(){
    return dimensions;
}



} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_PRIVATE_RUNTIMEEVALUATORVARIABLEINFORMATION_H_ */
