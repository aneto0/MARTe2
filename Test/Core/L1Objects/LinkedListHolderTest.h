/**
 * @file LinkedListHolderTest.h
 * @brief Header file for class LinkedListHolderTest
 * @date 06/08/2015
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

 * @details This header file contains the declaration of the class LinkedListHolderTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LINKEDLISTHOLDERTEST_H_
#define LINKEDLISTHOLDERTEST_H_
/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "LinkedListHolder.h"
#include "MutexSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests all the LinkedListHolder functions.
 */
class LinkedListHolderTest {

public:

    /**
     * @brief Tests the LinkedListHolder constructor.
     * @return true if at the beginning the size of the list is zero, false otherwise.
     */
    bool TestConstructor();

    /**
     * @brief Tests the LinkedListHolder destructor.
     * @return true if the first element of the list is null and the size is zero after the destructor call.
     */
    bool TestDestructor();

    /**
     * @brief Tests the LinkedListHolder::Reset function.
     * @param[in] nElements is the desired number of elements in the list.
     * @return true if the first element of the list is null and the size is zero after the function call.
     */
    bool TestReset(uint32 nElements);

    /**
     * @brief Tests the LinkedListHolder::CleanUp function.
     * @param[in] nElements is the desired number of elements in the list.
     * @return true if all the elements of the list are deleted after the function call.
     */
    bool TestCleanup(uint32 nElements);

    /**
     * @brief Tests the LinkedListHolder::List function.
     * @param[in] nElements is the desired number of elements in the list.
     * @return true if the function returns the first element in the list and NULL if the list is empty.
     */
    bool TestList(uint32 nElements);

    /**
     * @brief Tests the LinkedListHolder::ListSize function.
     * @details Creates a list and checks that the size of the list is correct.
     * @param[in] nElements is the desired number of elements in the list.
     * @return true if successful, false otherwise.
     */
    bool TestListSize(uint32 nElements);

    /**
     * @brief Tests the LinkedListHolder::FastListInsertSingle function.
     * @details Creates a list inserting elements one by one and checks their order in the list (they should be added on the queue).
     * @param[in] nElements is the desired number of elements in the list.
     * @return true if successful, false otherwise.
     */
    bool TestFastListInsertSingle(uint32 nElements);

    /**
     * @brief Tests the LinkedListHolder::ListInsert function.
     * @details Creates two lists and merges them checking if the insert adds the second list at the beginning.
     * @param[in] nElements is the desired number of elements in the two lists.
     * @return true if successful, false otherwise.
     */
    bool TestListInsert(uint32 nElements);

    /**
     * @brief Tests the LinkedListHolder::ListInsert with a SortFilter as input.
     * @details Creates two lists of integers and merges them specifying a decrescent order. Then checks that the elements are ordered correctly.
     * @param[in] nElements is the desired number of elements in the two lists.
     * @return true if successful, false otherwise.
     */
    bool TestListInsertSortedSorter(uint32 nElements);

    /**
     * @brief Tests the LinkedListHolder::ListInsert with a SortFilterFn function as input.
     * @details Creates two lists of integers and merges them specifying a decrescent order. Then checks that the elements are ordered correctly.
     * @param[in] nElements is the desired number of elements in the two lists.
     * @return true if successful, false otherwise.
     */
    bool TestListInsertSortedFn(uint32 nElements);

    /**
     * @brief Tests the insert function passing a null SortFilter as input.
     * @details Checks if the insert function, inserts the second list at the beginning of the first (normal insert behavior).
     * @param[in] nElements is the desired number of elements in the two lists.
     * @return true if successful, false otherwise.
     */
    bool TestListInsertNullSorter(uint32 nElements);

    /**
     * @brief Tests the LinkedListHolder::ListInsert passing the element position.
     * @details Creates two lists and inserts the second on the first after the specified position. Then checks if the result list is as expected.
     * @param[in] index is the position of the first list where the second list must be inserted.
     * @param[in] size is the number of elements of the two lists.
     * @return true if successful, false otherwise.
     */
    bool TestListInsertIndex(uint32 index,
                             uint32 size);

    /**
     * @brief Tests the LinkedListHolder::ListAdd function.
     * @details Creates two lists and adds the second to the first. Then checks that after the function only the first element of the second list
     * is added on the queue of the first.
     * @param[in] nElements is the desired number of elements in the two lists.
     * @return true if successful, false otherwise.
     */
    bool TestListAdd(uint32 nElements);

    /**
     * @brief Tests the LinkedListHolder::ListAddL function.
     * @details Creates two lists and adds the second to the first. Then checks that all the second list is added on the queue of the first.
     * @param[in] nElements is the desired number of elements of the two lists.
     * @return true if successful, false otherwise.
     */
    bool TestListAddL(uint32 nElements);

    /**
     * @brief Tests the LinkedListHolder::ListSearch.
     * @details Creates a list and checks if the function returns true searching all elements in the list and if it returns false for elements not in the list.
     * @return true if successful, false otherwise.
     */
    bool TestListSearch();

    /**
     * @brief Tests the LinkedListHolder::ListSearch function with a SearchFilter in input.
     * @details Creates a list of integers and checks if the function returns true for elements in the list and false for elements not in the list
     * using a SearchFilter object.
     * @return true if successful, false otherwise.
     */
    bool TestListSearchFilter();

    /**
     * @brief Tests the LinkedListHolder::ListSearch function with a SearchFilterFn function in input.
     * @details Creates a list of integers and checks if the function returns true for elements in the list and false for elements not in the list
     * using a SearchFilterFn function.
     * @return true if successful, false otherwise.
     */
    bool TestListSearchFn();

    /**
     * @brief Tests the LinkedListHolder::ListExtract function.
     * @details Creates a list and extracts all elements in it checking that the size decreases. Checks also that
     * the function returns true for elements in the list and false otherwise.
     * @return true if successful, false otherwise.
     */
    bool TestListExtract();

    /**
     * @brief Tests the extract function with a SearchFilter in input.
     * @details Creates a list of integers and extracts all elements using a SearchFilter object and checking that the size decreases.
     * Checks also that the function returns the correct elements extracted.
     * @return true if successful, false otherwise.
     */
    bool TestListExtractSearchFilter();

    /**
     * @brief Tests the extract function with a SearchFilterFn function in input.
     * @details Creates a list of integers and extracts all elements using a SearchFilterFn function and checking that the size decreases.
     * Checks also that the function returns the correct elements extracted.
     * @return true if successful, false otherwise.
     */
    bool TestListExtractSearchFn();

    /**
     * @brief Tests the extract function with the element position in input.
     * @details Creates a list and removes all the elements by index checking that the size decreases. Checks also that the function returns
     * the correct elements extracted.
     * @return true if successful, false otherwise.
     */
    bool TestListExtractIndex();

    /**
     * @brief Tests the LinkedListHolder::Delete function.
     * @details Creates a list and deletes all elements checking that the size decreases. Checks also if
     * the function returns true if the element is in the list and false otherwise.
     * @return true if successful, false otherwise.
     */
    bool TestListDelete();

    /**
     * @brief Tests the delete function passing a SearchFilter object in input.
     * @details Creates a list of integer and checks if the delete function removes from the list all the elements in the list which
     * satisfies the specified search criteria. Checks also if the function returns true if at least one element is removed, false if the requested
     * element is not in the list.
     * @return true if successful, false otherwise.
     */
    bool TestListDeleteSearchFilter();

    /**
     * @brief Tests the delete function passing a SearchFilterFn function in input.
     * @details Creates a list of integer and checks if the delete function removes from the list all the elements in the list which
     * satisfies the specified search criteria. Checks also if the function returns true if at least one element is removed, false if the requested
     * element is not in the list.
     * @return true if successful, false otherwise.
     */
    bool TestListDeleteSearchFn();


    /**
     * @see TestListDeleteSearchFilter.
     */
    bool TestListSafeDelete();

    /**
     * @brief Tests the LinkedListHolder::ListBSort function with a SortFilter object in input.
     * @details Creates a list of integers and sorts them in a decrescent order. Then checks if the elements are sorted correctly in the list.
     * @param[in] nElements is the desired number of elements in the list.
     * @return true if successful, false otherwise.
     */
    bool TestListBSortSorter(uint32 nElements);


    /**
     * @brief Tests the LinkedListHolder::ListBSort function with a SortFilterFn function in input.
     * @details Creates a list of integers and sorts them in a decrescent order. Then checks if the elements are sorted correctly in the list.
     * @param[in] nElements is the desired number of elements in the list.
     * @return true if successful, false otherwise.
     */
    bool TestListBSortFn(uint32 nElements);

    /**
     * @brief Tests the LinkedListHolder::Peek function.
     * @details Creates a list and browses it checking that the elements are in the correct positions. Then checks if the function
     * returns NULL in case of index greater than the list size.
     * @param[in] nElements is the desired number of elements in the list.
     * @return true if successful, false otherwise.
     */
    bool TestListPeek(uint32 nElements);

    /**
     * @brief Tests the LinkedListHolder::Iterate with an Iterator object in input.
     * @details Creates a list of integers and increments each element using an iterator object. Then checks if all elements were incremented
     * as expected.
     * @return true if successful, false otherwise.
     */
    bool TestListIterateIterator();

    /**
     * @brief Tests the LinkedListHolder::Iterate with an IteratorFn function in input.
     * @details Creates a list of integers and increments each element using an iterator function. Then checks if all elements were incremented
     * as expected.
     * @return true if successful, false otherwise.
     */
    bool TestListIterateFn();

    LinkedListHolder internalList;

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LINKEDLISTHOLDERTEST_H_ */

