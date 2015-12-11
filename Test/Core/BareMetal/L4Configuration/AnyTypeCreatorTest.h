/**
 * @file AnyTypeCreatorTest.h
 * @brief Header file for class AnyTypeCreatorTest
 * @date 11/12/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class AnyTypeCreatorTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ANYTYPECREATORTEST_H_
#define ANYTYPECREATORTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AnyTypeCreator.h"
#include "TypeConversion.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
class AnyTypeCreatorTest {

public:

    bool TestConstructor();

    bool TestCleanUp();

    bool TestGetSize(uint32 size);

    bool TestGetGranularity(uint32 granularity);

    bool TestToType(const char8* table[][3]);

    bool TestToType_TypeMismatch();

    template<typename T>
    bool TestSetType(uint32 nDim,
                     uint32 rows,
                     uint32 cols,
                     TypeDescriptor descriptor,
                     const char8* typeIn,
                     const char8* data,
                     T typeS);

    bool TestSetType_String();

    bool TestSetType_NullMemory();

    bool TestSetType_SizeMismatch();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
template<typename T>
bool AnyTypeCreatorTest::TestSetType(uint32 nDim,
                                     uint32 rows,
                                     uint32 cols,
                                     TypeDescriptor descriptor,
                                     const char8* typeIn,
                                     const char8* data,
                                     T typeS) {
    AnyTypeCreator type;

    uint32 size = rows * cols;
    for (uint32 i = 0; i < size; i++) {
        type.ToType(typeIn, data);
    }

    uint32 dimSizes[3] = { cols, rows, 1 };
    AnyType element;
    if (!type.SetType(element, nDim, dimSizes)) {
        return false;
    }

    T* elementPointer = static_cast<T*>(element.GetDataPointer());

    if (element.GetTypeDescriptor() != descriptor) {
        return false;
    }

    T dataTest;
    TypeConvert(dataTest, data);

    for (uint32 i = 0; i < rows; i++) {
        for (uint32 j = 0; j < cols; j++) {
            if (elementPointer[i * cols + j] != dataTest) {
                return false;
            }
        }
    }

    return true;
}
#endif /* ANYTYPECREATORTEST_H_ */

