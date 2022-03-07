/**
 * @file MatrixSize.h
 * @brief Header file for class MatrixSize
 * @date Aug 25, 2020
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

#ifndef MATRIXSIZE_H_
#define MATRIXSIZE_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "Matrix.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

class MatrixSize{
public:
    MatrixSize(uint32 numberOfRows=0,uint32 numberOfColumns=0):numberOfRows(numberOfRows), numberOfColumns(numberOfColumns){
    }
    template <class T>
    MatrixSize(const Matrix<T> & m):numberOfRows(m.GetNumberOfRows()), numberOfColumns(m.GetNumberOfColumns()){
    }
    MatrixSize(const MatrixSize & ms):numberOfRows(ms.numberOfRows), numberOfColumns(ms.numberOfColumns){
    }
    bool operator== (const MatrixSize & ms){
        bool check1 = (ms.numberOfColumns == numberOfColumns);
        bool check2 = (ms.numberOfRows    == numberOfRows);
        return check1 && check2;
    }
    MatrixSize operator= (const MatrixSize & ms){
        numberOfRows = ms.numberOfRows;
        numberOfColumns = ms.numberOfColumns;
        return *this;
    }
    bool Add(const MatrixSize &addend, MatrixSize &result){
        result = addend;
        return *this == addend;
    }
    bool Product(const MatrixSize &factor, MatrixSize &result){
        bool check1 = (factor.numberOfRows == numberOfColumns);
        result.numberOfRows    = numberOfRows;
        result.numberOfColumns = factor.numberOfColumns;
        return check1 ;
    }

    uint32 numberOfRows;
    uint32 numberOfColumns;
};




/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* MATRIXSIZE_H_ */
