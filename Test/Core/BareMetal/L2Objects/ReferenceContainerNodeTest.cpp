/**
 * @file ReferenceContainerNodeTest.cpp
 * @brief Source file for class ReferenceContainerNodeTest
 * @date 18/08/2015
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
 * the class ReferenceContainerNodeTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainerNodeTest.h"
using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ReferenceContainerNodeTest::TestConstructor() {
    ReferenceContainerNode myNode;
    return !(myNode.GetReference().IsValid());
}

bool ReferenceContainerNodeTest::TestDestructor() {

    ReferenceContainerNode myNode;

    Reference ref("Object");

    myNode.SetReference(ref);

    if (ref.NumberOfReferences() != 2) {
        return false;
    }

    myNode.~ReferenceContainerNode();
    return ref.NumberOfReferences() == 1;

}

bool ReferenceContainerNodeTest::TestSetGetReference() {

    Reference testRef("Object");

    ReferenceContainerNode myNode;
    myNode.SetReference(testRef);

    bool test1 = (myNode.GetReference() != testRef);
    bool test2 = (testRef.NumberOfReferences() != 2);
    if (test1 || test2) {
        return false;
    }

    Reference ref;
    myNode.SetReference(ref);

    test1 = (myNode.GetReference() == ref);
    test2 = (testRef.NumberOfReferences() == 1);
    return (test1 && test2);

}

