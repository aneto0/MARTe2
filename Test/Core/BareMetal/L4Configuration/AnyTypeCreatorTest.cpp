/**
 * @file AnyTypeCreatorTest.cpp
 * @brief Source file for class AnyTypeCreatorTest
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

 * @details This source file contains the definition of all the methods for
 * the class AnyTypeCreatorTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AnyTypeCreatorTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;
bool AnyTypeCreatorTest::TestConstructor() {
    AnyTypeCreator type;
    if ((type.GetGranularity() != 1) && (type.GetSize() != 0)) {
        return false;
    }

    AnyTypeCreator type2(3);
    return (type2.GetGranularity() == 3) && (type.GetSize() == 0);
}

bool AnyTypeCreatorTest::TestCleanUp() {

    AnyTypeCreator type;

    if (type.GetGranularity() != 1) {
        return false;
    }

    const char8 *integer = "1";
    type.ToType("int8", integer);

    AnyType element;
    uint32 dimSizes[3] = { 1, 1, 1 };
    type.SetType(element, 0, dimSizes);

    if (type.GetSize() != 1) {
        return false;
    }

    type.CleanUp(4);
    return (type.GetGranularity() == 4) && (type.GetSize() == 0);
}

bool AnyTypeCreatorTest::TestGetSize(uint32 size) {
    AnyTypeCreator type;

    for (uint32 i = 0; i < size; i++) {
        type.ToType("int8", "1");
    }

    return (type.GetSize() == size);
}

bool AnyTypeCreatorTest::TestGetGranularity(uint32 granularity) {
    AnyTypeCreator type(granularity);

    return type.GetGranularity() == granularity;
}

bool AnyTypeCreatorTest::TestToType(const char8* table[][3]) {
    uint32 i = 0u;
    uint32 size = 0u;
    AnyTypeCreator type;
    while (table[i][0] != NULL) {
        bool ret=type.ToType(table[i][0], table[i][1]);
        if(ret!=(table[i][2]!=NULL)) {
            return false;
        }

        if(ret) {
            size++;
        }
        i++;
    }

    return type.GetSize() == size;
}

bool AnyTypeCreatorTest::TestToType_TypeMismatch() {
    AnyTypeCreator type;

    type.ToType("uint8", "1");

    return !type.ToType("int8", "-1");
}

bool AnyTypeCreatorTest::TestSetType_NullMemory() {
    AnyTypeCreator type;
    AnyType element;
    uint32 dimSizes[3] = { 0, 0, 0 };
    return !type.SetType(element, 0, dimSizes);
}

bool AnyTypeCreatorTest::TestSetType_SizeMismatch() {
    AnyTypeCreator type;
    AnyType element;

    type.ToType("uint8", "1");
    uint32 dimSizes[3] = { 1, 1, 2 };
    return !type.SetType(element, 0, dimSizes);
}

bool AnyTypeCreatorTest::TestSetType_String() {
    AnyTypeCreator type;

    const char8* input = "Hello";

    type.ToType("string", input);
    uint32 dimSizes[3] = { 1, 1, 1 };
    AnyType element;
    if (!type.SetType(element, 0, dimSizes)) {
        return false;
    }
    if (StringHelper::Compare((const char8*) (element.GetDataPointer()), input) != 0) {
        return false;
    }
    //add another
    type.ToType("string", input);
    dimSizes[1] = 2;

    if (!type.SetType(element, 1, dimSizes)) {
        return false;
    }
    if (StringHelper::Compare(((const char8**) (element.GetDataPointer()))[0], input) != 0) {
        return false;
    }

    if (StringHelper::Compare(((const char8**) (element.GetDataPointer()))[1], input) != 0) {
        return false;
    }

    return true;
}

