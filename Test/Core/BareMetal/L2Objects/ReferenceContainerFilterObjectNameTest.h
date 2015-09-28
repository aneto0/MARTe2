/**
 * @file ReferenceContainerFilterObjectNameTest.h
 * @brief Header file for class ReferenceContainerFilterObjectNameTest
 * @date 19/08/2015
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

 * @details This header file contains the declaration of the class ReferenceContainerFilterObjectNameTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REFERENCECONTAINERFILTEROBJECTNAMETEST_H_
#define REFERENCECONTAINERFILTEROBJECTNAMETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainerFilterObjectName.h"
using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Tests all the ReferenceContainerFilterObjectName functions.
 */
class ReferenceContainerFilterObjectNameTest {

public:

    /**
     * @brief Tests the default constructor.
     * @return true if the filter is initialised with mode=0 and the number of occurrences=0
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the full constructor.
     * @details Checks if this constructor initialises properly the number of the occurrences and the number
     * of node to search. In case of more than one node, the mode should be initialised to path and recursive.
     * @param[in] occurrence is the desired pattern occurrence.
     * @param[in] toSearch is the pattern to be searched.
     * @return true if the test explained in the details section passes, false otherwise.
     */
    bool TestFullConstructor(int32 occurrence,
                             const char8 *toSearch);

    /**
     * @brief Tests the copy constructor.
     * @details Checks if after the copy constructor from a finished filter, the new created filter is reset. Then checks if the
     * copied object still works after the delete of the first object.
     * @param[in] occurrence is the desired pattern occurrence.
     * @param[in] toSearch is the pattern to be searched.
     * @return true if the test explained in the details section passes, false otherwise.
     */
    bool TestCopyConstructor(int32 occurrence,
                             const char8 *toSearch);

    /**
     * @brief Tests the destructor.
     * @return true if the filter after the destructor does not work anymore.
     */
    bool TestDestructor();

    /**
     * @brief Tests the filter test function.
     * @details Tests the function with different parameters (one and more than one nodes) comparing the result with the \a expected.
     * @param[in] previouslyFound simulate the number of previously found nodes.
     * @param[in] toSearch is the pattern to be searched.
     * @param[in] expected is the expected test result.
     * @return true if the result of the test returns the expected value, false otherwise.
     */
    bool TestTest(ReferenceContainer &previouslyFound,
                  const char8 *toSearch,
                  bool expected);

    /**
     * @brief Tests the = operator.
     * @param[in] occurrence is the desired pattern occurrence.
     * @param[in] toSearch is the pattern to be searched.
     * @see TestCopyConstructor using the = operator instead of the copy constructor.
     */
    bool TestAssignOperator(int32 occurrence,
                            const char8 *toSearch);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REFERENCECONTAINERFILTEROBJECTNAMETEST_H_ */

