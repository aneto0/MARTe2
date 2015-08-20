/**
 * @file ReferenceContainerFilterReferencesTest.h
 * @brief Header file for class ReferenceContainerFilterReferencesTest
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

 * @details This header file contains the declaration of the class ReferenceContainerFilterReferencesTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REFERENCECONTAINERFILTERREFERENCESTEST_H_
#define REFERENCECONTAINERFILTERREFERENCESTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainerFilterReferences.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Tests all the ReferenceContainerFilter and ReferenceContainerFilterReference functions
 */
class ReferenceContainerFilterReferencesTest {

public:

    /**
     * @brief Tests the default constructor.
     * @return true if the number of occurrence and the mode are initialized to zero.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the full constructor,
     * @param[in] occurrenceNumber is the desired pattern occurrence.
     * @param[in] mode is the desired search mode.
     * @return true if the number of occurrence and the mode are initialized as expected. If
     * the occurrence is -1, the path mode should be automatically disabled.
     */
    bool TestFullConstructor(int32 occurrenceNumber,
                             uint32 mode);

    /**
     * @brief Tests the Test function
     * @param[in] nRefs number of references to the same object.
     * @param[in] occurrence is the desired pattern occurrence.
     * @return true if after the specified \a occurrence number of test operations, the filter is finished.
     */
    bool TestTest(uint32 nRefs,
                  int32 occurrence);


    /**
     * @brief Tests the failure of test function with a reference to the same type but different object.
     * @return true if the test function fails on a different reference than the specified one in the filter construction.
     */
    bool TestTestFailure();

    /**
     * @brief Tests the IncrementFound function.
     * @return true if the test function decrements the occurrence number.
     */
    bool TestIncrementFound();

    /**
     * @brief Tests the IsRemove function.
     * @return true if the function returns true if in the mode mask its bit is set.
     */
    bool TestIsRemove();

    /**
     * @brief Tests the IsSearchAll function.
     * @return true if with the filter initialized with occurrence=-1 the function returns true and
     * if the search all mode removes automatically the path mode if specified.
     */
    bool TestIsSearchAll();

    /**
     * @brief Tests the IsStorePath function.
     * @return true if the store path mode is unset automatically if a search all mode is specified.
     */
    bool TestIsStorePath();

    /**
     * @brief Tests the IsReverse function.
     * @return true if the function returns true when the reverse mode is specified.
     */
    bool TestIsReverse();


    /**
     * @brief Tests the IsRecursive function.
     * @return true if the path mode enables automatically the recursive mode.
     */
    bool TestIsRecursive();

    /**
     * @brief Tests the SetMode and GetMode functions.
     * @param[in] occurrence is the desired pattern occurrence.
     * @return true if GetMode function returns the mask set by SetMode function except for these particular cases:
     * -if search all true => store path false
     * -if store path true => recursive true
     */
    bool TestSetGetMode(int32 occurrence);

    /**
     * @brief Tests the Reset function.
     * @details Calls a reset after that the filter is finished and checks that the number
     * of occurrences returns to the original value.
     * @param[in] occurrence is the desired pattern occurrence.
     * @return true if the test explained in the details section passes, false otherwise.
     */
    bool TestReset(uint32 occurrence);

    /**
     * @brief Tests the TestSetGetOriginalSetOccurrence.
     * @details Checks if the number of occurrence is set properly by this function and if
     * passing to the search all mode, also mode is automatically refreshed disabling the path mode.
     * @param[in] InitialOccurrence is the initial desired pattern occurrence.
     * @param[in] finalOccurrence is the final desired pattern occurrence.
     * @return true if the test explained in the details section passes, false otherwise.
     */
    bool TestSetGetOriginalSetOccurrence(int32 initialOccurrence,
                                         int32 finalOccurrence);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REFERENCECONTAINERFILTERREFERENCESTEST_H_ */

