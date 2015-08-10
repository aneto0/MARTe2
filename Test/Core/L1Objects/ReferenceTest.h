/**
 * @file ReferenceTest.h
 * @brief Header file for class ReferenceTest
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

 * @details This header file contains the declaration of the class ReferenceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REFERENCETEST_H_
#define REFERENCETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Reference.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class ReferenceTest {
public:


    /**
     * @brief Tests the default constructor
     * @return true if the internal object pointer is null after the reference constructor call.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the copy constructor.
     * @return true if the object pointer is equal to the input object pointer passed in the contructor.
     */
    bool TestCopyConstructor();

    /**
     * @brief Tests the copy constructor passing a null input.
     * @return true if the object pointer is initializet to null.
     */
    bool TestCopyConstructorNullPtr();


    /**
     * @brief Tests the constructor which instantiate a new object.
     * @return true if
     */
    bool TestBuildObjectConstructor();

    bool TestBuildFakeObjectConstructor();

    bool TestCopyFromObjPtrConstructor();

    bool TestCopyFromObjPtrConstructorNullPtr();

    bool TestDestructor();

    //TODO
    bool TestInitialise();

    bool TestRemoveReference();

    bool TestCopyOperatorReference();

    bool TestCopyOperatorReferenceNull();

    bool TestCopyOperatorObject();

    bool TestCopyOperatorObjectNull();

    bool TestIsValid();

    bool TestNumberOfReferences();

    bool TestEqualOperator();

    bool TestDifferentOperator();

    bool TestClone();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REFERENCETEST_H_ */

