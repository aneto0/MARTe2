/**
 * @file LinkedListableTest.h
 * @brief Header file for class LinkedListableTest
 * @date 05/08/2015
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

 * @details This header file contains the declaration of the class LinkedListableTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LINKEDLISTABLETEST_H_
#define LINKEDLISTABLETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "LinkedListable.h"
#include "Memory.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Tests all the LinkedListable functions.
 */
class LinkedListableTest {
public:

    /**
     * @brief Tests the constructor.
     * @return true if after the constructor function call the size is equal to one and the next pointer is null.
     */
    bool TestConstructor();

    /**
     * @brief Tests the destructor.
     * @return true if after the destructor call the next pointer is null.
     */
    bool TestDestructor();

    /**
     * @brief Tests the LinkedListable::Next and LinkedListable::SetNext.
     * @details Creates a list using SetNext, then checks that the list is created as expected browsing it using Next.
     * @return true if successful, false otherwise.
     */
    bool TestSetGetNext();

    /**
     * @brief Tests the LinkedListable::Size.
     * @details Creates a list and checks that the size is equal to the number of elements added.
     * @return true if successful, false otherwise.
     */
    bool TestSize();

    /**
     * @brief Tests The LinkedListable::BSort function using a SortFilter.
     * @details Creates a list of numbers in crescent order, then sorts them in decrescent order and checks that the list after the function
     * call is ordered correctly.
     * @return true if successful, false otherwise.
     */
    bool TestBSortSorter();

    /**
     * @brief Tests The LinkedListable::BSort function using a SortFilterFn function.
     * @details Creates a list of numbers in crescent order, then sorts them in decrescent order and checks that the list after the function
     * call is ordered correctly.
     * @return true if successful, false otherwise.
     */
    bool TestBSortFn();

    /**
     * @brief Tests the LinkedListable::Insert function.
     * @details Creates two lists and inserts a sublist of the second list after a specified point of the first list. Then checks that the
     * list elements order is as expected.
     * @param[in] firstListInsertPoint is the position of the first list after that the second list is inserted.
     * @param[in] secondListBeginPoint is the beginning of the sub-list of the second list.
     * @return true if successful, false otherwise.
     */
    bool TestInsert(uint32 firstListInsertPoint,
                    uint32 secondListBeginPoint);

    /**
     * @brief Inserts the second list at the beginning of the first list.
     * @details Calls the insert function by the root of the first list and checks if the second list is inserted at the beginning.
     * @return true if successful, false otherwise.
     */
    bool TestInsertEntireList();

    /**
     * @brief Inserts elements holding a specific order using a SortFilter.
     * @details Creates the first list of number in decrescent order and inserts a second lists passing a SortFilter to hold the
     * decrescent order. Then checks that the final list is sorded in decrescent order.
     * @return true if successful, false otherwise.
     */
    bool TestInsertSortedSorter(uint32 secondListPosition);

    /**
     * @brief Inserts elements holding a specific order using a SortFilterFn sunction.
     * @details Creates the first list of number in decrescent order and inserts a second lists passing a function to hold the
     * decrescent order. Then checks that the final list is sorded in decrescent order.
     * @return true if successful, false otherwise.
     */
    bool TestInsertSortedFn(uint32 secondListPosition);

    /**
     * @brief Tests the insert function passing a null input.
     * @return true if the original list remains the same.
     */
    bool TestInsertNullListInput();

    /**
     * @brief Tests the insert function passing a null SortFilter.
     * @return true if the elements of the second list are inserted without sorting.
     */
    bool TestInsertNullSorterInput();

    /**
     * @brief Tests the LinkedListable::Add function.
     * @details Appends an element of the second to the first and checks that the result list is correct.
     * @param[in] secondListPosition is the position of the element of the second list to be appended to the first list.
     * @return true if successful, false otherwise.
     */
    bool TestAdd(uint32 secondListPosition);

    /**
     * @brief Tests the LinkedListable::AddL function.
     * @details Creates two lists and appends a sublist of the second list to the first. Then checks that the result is correct.
     * @param[in] secondListPosition is the first position of the second list which must be appended to the first.
     * @return true if successful, false otherwise.
     */
    bool TestAddL(uint32 secondListPosition);

    /**
     * @brief Tests the LinkedListable::Search function.
     * @details Creates a list, then searches all elements and checks if the function returns true. After that checks if the function
     * return false if the element to search is not added to the list.
     * @return true if successful, false otherwise.
     */
    bool TestSearch();

    /**
     * @brief Tests the search function passing a SearchFilter.
     * @details Creates a list of integers and searches them in the list by number.
     * @return true if successful, false otherwise.
     */
    bool TestSearchFilter();

    /**
     * @brief Tries to search an element with a minor position in the list.
     * @details Creates a list, then calls the search function by an element with a greater position than the element to be searched. Checks if the
     * function returns false.
     * @return true if successful, false otherwise.
     */
    bool TestSearchFilterPreviousElement();

    /**
     * @brief Tests the search function passing a SearchFn function.
     * @details Creates a list of integers and searches them in the list by number.
     * @return true if successful, false otherwise.
     */
    bool TestSearchFn();

    /**
     * @brief Tests the LinkedListable::Extract function.
     * @details Creates a list and extracts element checking that the size of the list decreases.
     * @return true if successful, false otherwise.
     */
    bool TestExtract();

    /**
     * @brief Tests the extract function using a SearchFilter.
     * @details Creates a list of integer and extracts elements searching them by number. Checks if the size of the list decreases after each extract operation.
     * @return true if successful, false otherwise.
     */
    bool TestExtractSearchFilter();


    /**
     * @brief Tests the extract function using a SearchFn.
     * @details Creates a list of integer and extracts elements searching them by number. Checks if the size of the list decreases after each extract operation.
     * @return true if successful, false otherwise.
     */
    bool TestExtractSearchFn();


    /**
     * @brief Tests the LinkedListable::Delete function.
     * @details Creates a list and then deletes the elements checking that the list size decreases.
     * @return true if successful, false otherwise.
     */
    bool TestDelete();

    /**
     * @brief Tests the delete function using a SearchFilter.
     * @details Creates a list of integers and delete elements searching them by number. Then checks if the number of deleted objects returned by the
     * function equal to the elements with the same value in the list.
     * @return true if successful, false otherwise.
     */
    bool TestDeleteSearchFilter();

    /**
     * @brief Tests the delete function using a SearchFn.
     * @details Creates a list of integers and delete elements searching them by number. Then checks if the number of deleted objects returned by the
     * function equal to the elements with the same value in the list.
     * @return true if successful, false otherwise.
     */
    bool TestDeleteSearchFn();

    /**
     * @brief Tests the LinkedListable::Peek function.
     * @details Creates a list and browses it checking that the elements in the list are in the correct positions.
     * @return true if successful, false otherwise.
     */
    bool TestPeek();

    /**
     * @brief Tests the LinkedListable::Iterate function passing an Iterato.
     * @details Creates a list of integers and calls the function passing an iterator that increment all element value. Then checks if
     * all elements are incremented.
     * @return true if successful, false otherwise.
     */
    bool TestIterateIterator();


    /**
     * @brief Tests the LinkedListable::Iterate function passing an IteratorFn function.
     * @details Creates a list of integers and calls the function passing an iterator that increment all element value. Then checks if
     * all elements are incremented.
     * @return true if successful, false otherwise.
     */
    bool TestIterateFn();


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LINKEDLISTABLETEST_H_ */

