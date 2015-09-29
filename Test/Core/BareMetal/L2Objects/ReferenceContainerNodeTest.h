/**
 * @file ReferenceContainerNodeTest.h
 * @brief Header file for class ReferenceContainerNodeTest
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

 * @details This header file contains the declaration of the class ReferenceContainerNodeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REFERENCECONTAINERNODETEST_H_
#define REFERENCECONTAINERNODETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainerNode.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


/**
 * @brief Tests all the ReferenceContainerNode functions
 */
class ReferenceContainerNodeTest {
public:

    /**
     * @brief Tests the default constructor.
     * @return true if the reference attribute is invalid after the constructor call, false otherwise.
     */
    bool TestConstructor();

    /**
     * @brief Tests the destructor.
     * @details Creates a reference and assign it to the node. Then calls the constructor and checks that the
     * reference number is decreased.
     * @return true if the test explained in the details section has success, false otherwise.
     */
    bool TestDestructor();

    /**
     * @brief Tests the SetReference and GetReference functions.
     * @details Assigns a reference to the node using the set function and checks that the number of references associated to the
     * object increases and that the get function returns the same reference. Then assigns to the node another reference checking that
     * the number of references related to previous reference decreases and that the get returns the new reference assigned.
     * @return true if the test explained in the details section has success, false otherwise.
     */
    bool TestSetGetReference();


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REFERENCECONTAINERNODETEST_H_ */

