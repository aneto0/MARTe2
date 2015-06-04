/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 *
 */
/**
 * @file StaticListHolderTest.h
 * @brief Tests the StaticListHolder class and associated functions
 */
#ifndef STATICLISCTHOLDER_test_H_
#define STATICLISCTHOLDER_test_H_
#include "StaticListHolder.h"
#include "EventSem.h"
#include "MutexSem.h"


/** @brief Class used for tests on StaticListHolder functions. */
//StaticListHolder class saves the direction of the variables in intptr (8bytes). If
//the elements has 2 fields (8+8 bytes) then the size of the static list is 2; the 8 first
//bytes are used to save the first field and the second 8 bytes are used to save the second
//field.
class StaticListHolderTest {
public:
    StaticListHolderTest() {
        numberOfElements = 20000;
        counter = 0;
        successful = false;
        eventSem.Create();
        mutexSem.Create(False);
        elementIntroduced = NULL;
        elementReturned = 0;
        elementReturnedPtr = (intptr *) &elementReturned;
    }
    /** number of element you want to introduce to the list (is not size of the list)*/
    int32 numberOfElements;

    /** Used to synchronize threads.*/
    EventSem eventSem;

    /** Used to protect shared variables;*/
    MutexSem mutexSem;

    /** Used to count. General propose */
    int32 counter;

    /** Used to indicate if the test is successful or not */
    bool successful;

    /** Is the class to be tested. */
    StaticListHolder slhr;

    /** It is used to introduce elements to the static table */
    intptr *elementIntroduced;

    int32 elementReturned;

    /** It is used to save the return value from static table */
    intptr *elementReturnedPtr;

    virtual ~StaticListHolderTest() {

    }
    /**
     * @brief Test all the available functions with a static list size per
     * element equal to 1 (sizeOf(intptr)).
     *
     * The elements introduced in the static list are int32 (4 bytes)
     * The class functions tested are: ListAdd() (specifying and not
     * specifying the position), ListSize, ListPeek, ListFind, ListExtract and
     * ListDelete.
     *
     * @return true when the behavior of the functions are as expected.
     */
    bool ListInt32();

    /**
     * @brief Test all the available functions with a static list size per
     * element equal to 1 (sizeOf(intptr)).
     *
     * The elements introduced in the static list are int64 (8 bytes)
     * The class functions tested are: ListAdd() (specifying and not
     * specifying the position), ListSize, ListPeek, ListFind, ListExtract and
     * ListDelete.
     *
     * @return true when the behavior of the functions are as expected.
     */
    bool ListInt64();

    /**
     * @brief Test all the available functions with a static list size per
     * element equal to 1 (sizeOf(intptr)).
     *
     * The elements introduced in the static list are float (4 bytes)
     * The class functions tested are: ListAdd() (specifying and not
     * specifying the position), ListSize, ListPeek, ListFind, ListExtract and
     * ListDelete.
     *
     * @return true when the behavior of the functions are as expected.
     */
    bool ListFloat();

    /**
     * @brief Test all the available functions with a static list size per
     * element equal to 1 (sizeOf(intptr)).
     *
     * The elements introduced in the static list are double (8 bytes)
     * The class functions tested are: ListAdd() (specifying and not
     * specifying the position), ListSize, ListPeek, ListFind, ListExtract and
     * ListDelete.
     *
     * @return true when the behavior of the functions are as expected.
     */
    bool ListDouble();

    /**
     * @brief Test all the available functions with a static list size per
     * element equal to 2 (sizeOf(intptr)*2)
     *
     * The elements introduced in the static list are structures with two
     * fields. The class functions tested are: ListAdd() (specifying and not
     * specifying the position), ListSize, ListPeek, ListFind, ListExtract and
     * ListDelete.
     *
     * @return true when the behavior of the functions are as expected.
     */
    bool SlhSize2();

    /**
     *@brief Test is the StaticList is protected against multiple thread access
     *
     *The test is divided in two parts. In the first part the Static list
     *is protected with infinite timeout and several threads tries to add
     *elements at the same time. The second part is exactly the same as the
     *first part but the static list is unprotected
     *
     *@return true when in the first part all the elements are added correctly
     *@return and the size of the list as expected and when in the second part
     *@return (unprotected) not all the elements are added correctly and the
     *@return size of the list is not as expected (should be less)
     */
    bool SetAccessTimeoutTest();

    /**
     * @brief Specific test for ListAdd.
     *
     * Test ListAdd in any position, end, beginning and strange position.
     * Test all the possible combinations/options of ListAdd
     *
     * @return true when the function behaves as expected.
     */
    bool ListAddTest();

    /**
     * @brief Test ListAdd
      *
     * Add elements in the first position and check that the elements are shifting one position
     * every time that an element is introduced
     *
     * @return true when the element is added correctly and the size of the list is as expected
     */
    bool AddFirstPosition();

    /**
     * @brief Test ListExtracTest
     *
     * The test consist on: Extract all the elements (element position not specified).
     * Extract an element specifying the position. And extract element from impossible
     * position
     *
     * @return true when the element is extracted correctly.
     */
    bool ListExtractTest();
};
#endif
