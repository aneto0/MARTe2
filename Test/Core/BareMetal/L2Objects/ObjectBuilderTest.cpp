/**
 * @file ObjectBuilderTest.cpp
 * @brief Source file for class ObjectBuilderTest
 * @date 27/07/2016
 * @author Ivan Herrero
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
 * the class ObjectBuilderTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ObjectBuilderTest.h"

#include "Object.h"
#include "ObjectBuilder.h"
#include "../../../../Source/Core/BareMetal/L0Types/HeapI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ObjectBuilderTest::ObjectBuilderTest() {
}

ObjectBuilderTest::~ObjectBuilderTest() {
}

bool ObjectBuilderTest::TestDefaultConstructor() {
    //Remark: The default constructor of ObjectBuilder does nothing.
    return true;
}

bool ObjectBuilderTest::TestBuild() {
    //Remark: The Build method always return a NULL and does not use the heap.
    using namespace MARTe;
    bool result;
    ObjectBuilder target;
    Object *obj = target.Build(NULL_PTR(HeapI *));
    result = (obj == NULL_PTR(Object *));
    return result;
}
