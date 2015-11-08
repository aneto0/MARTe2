/**
 * @file Matrix.h
 * @brief Header file for class Matrix
 * @date 05/11/2015
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class Matrix
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MATRIX_H_
#define MATRIX_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AnyType.h"
#include "Vector.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * TODO
 */
template<typename T>
class Matrix: public AnyType {
public:
    /**
     * TODO
     */
    Matrix(uint32 nOfRows,
           uint32 nOfColumns);

    /**
     * TODO
     */
    Matrix(T **existingArray,
           uint32 nOfRows,
           uint32 nOfColumns);

    /**
     * TODO
     */
    template<uint32 nOfRowsStatic, uint32 nOfColumnsStatic>
    Matrix(T (&source)[nOfRowsStatic][nOfColumnsStatic]);

    /**
     * TODO
     */
    Vector<T> operator [](uint32 element);

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {

template<typename T>
Matrix<T>::Matrix(uint32 nOfRows,
                  uint32 nOfColumns) {
    dataPointer = reinterpret_cast<T*>(new T*[nOfRows]);
    numberOfDimensions = 2u;
    numberOfElements[0] = nOfColumns;
    numberOfElements[1] = nOfRows;

    T* dataPointerT = static_cast<T *>(dataPointer);
    T** rows = reinterpret_cast<T **>(&dataPointerT[0]);
    uint32 i;
    for (i = 0; i < nOfRows; i++) {
        rows[i] = new T[nOfColumns];
    }
    T typeDiscovery = static_cast<T>(0);
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
}

template<typename T>
Matrix<T>::Matrix(T **existingArray,
                  uint32 nOfRows,
                  uint32 nOfColumns)  {
    dataPointer = existingArray;
    numberOfDimensions = 2u;
    numberOfElements[0] = nOfColumns;
    numberOfElements[1] = nOfRows;
    T typeDiscovery = static_cast<T>(0);
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
}

template<typename T>
template<uint32 nOfRowsStatic, uint32 nOfColumnsStatic>
Matrix<T>::Matrix(T (&source)[nOfRowsStatic][nOfColumnsStatic]) {
    dataPointer = reinterpret_cast<T *>(&source);
    numberOfDimensions = 2u;
    numberOfElements[0] = nOfColumnsStatic;
    numberOfElements[1] = nOfRowsStatic;
    staticDeclared = true;

    T typeDiscovery = static_cast<T>(0);
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
}

template<typename T>
Vector<T> Matrix<T>::operator [](uint32 element) {
    Vector<T> vec;
    T* dataPointerT = static_cast<T *>(dataPointer);

    if (!staticDeclared) {
        T** rows = reinterpret_cast<T **>(&dataPointerT[0]);
        vec = Vector<T>(reinterpret_cast<T *>(rows[element]), numberOfElements[0]);
    }
    else {
        vec = Vector<T>(reinterpret_cast<T *>(&dataPointerT[element * numberOfElements[0]]), numberOfElements[0]);
    }
    return vec;
}

}
#endif /*MATRIX_H_ */

