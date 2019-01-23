/**
 * @file ObjectBuilderTTest.cpp
 * @brief Source file for class ObjectBuilderTTest
 * @date 17/06/2016
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
 * the class ObjectBuilderTTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ObjectBuilderTTest.h"

#include "GlobalObjectsDatabase.h"
#include "../../../../Source/Core/BareMetal/L0Types/HeapI.h"
#include "Object.h"
#include "ObjectBuilderT.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

class NotBuildableAgainObj: public MARTe::Object {
public:
    CLASS_REGISTER_DECLARATION();
};

CLASS_REGISTER(NotBuildableAgainObj, "1.0");

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ObjectBuilderTTest::ObjectBuilderTTest() {
}

ObjectBuilderTTest::~ObjectBuilderTTest() {
}

bool ObjectBuilderTTest::TestDefaultConstructor() {
    using namespace MARTe;
    bool result;
    ObjectBuilderT<NotBuildableAgainObj> target;
    result = (NotBuildableAgainObj::GetClassRegistryItem_Static()->GetObjectBuilder() == &target);
    return result;
}

bool ObjectBuilderTTest::TestBuild() {
    using namespace MARTe;
    bool result;
    ObjectBuilderT<NotBuildableAgainObj> target;
    HeapI* heap = GlobalObjectsDatabase::Instance()->GetStandardHeap();
    NotBuildableAgainObj *obj = (NotBuildableAgainObj *) target.Build(heap);
    if (obj == NULL) {
        result = false;
    }
    else {
        delete obj;
        result = true;
    }
    return result;
}
