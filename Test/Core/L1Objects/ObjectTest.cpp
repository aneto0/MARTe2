/**
 * @file ObjectTest.cpp
 * @brief Source file for class ObjectTest
 * @date 07/08/2015
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
 * the class ObjectTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ObjectTest.h"
#include "Reference.h"
#include "StringHelper.h"
#include "ObjectTestHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ObjectTest::TestConstructor() {
    Object myObject;
    return myObject.NumberOfReferences() == 0;
}

bool ObjectTest::TestDestructor() {
    Object myObject;
    //nothing happen.
    myObject.~Object();
    return myObject.NumberOfReferences() == 0;
}

bool ObjectTest::TestGetIntrospectionCopy() {
    //TODO
    return true;
}

bool ObjectTest::TestInitialise() {
    Object myObject;
    StructuredData data;
    //returns false
    return !myObject.Initialise(data);
}

bool ObjectTest::TestNumberOfReferences() {
    Heap myHeap;
    Reference objRef("Object", myHeap);

    if (objRef->NumberOfReferences() != 1) {
        return false;
    }

    Reference secondObjRef = objRef;

    if (objRef->NumberOfReferences() != 2) {
        return false;
    }

    secondObjRef.RemoveReference();

    return (objRef->NumberOfReferences() == 1);

}

bool ObjectTest::TestGetProperties() {
    Object obj;
    ClassProperties properties;
    obj.GetClassPropertiesCopy(properties);
    const char8 *name = properties.GetName();
    const char8 *version = properties.GetVersion();
    return (StringHelper::Compare("Object", name) == 0) && (StringHelper::Compare("1.0", version) == 0);
}
