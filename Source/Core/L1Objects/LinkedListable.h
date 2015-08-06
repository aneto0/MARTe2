/**
 * @file LinkedListable.h
 * @brief Header file for class LinkedListable
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

 * @details This header file contains the declaration of the class LinkedListable
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LINKEDLISTABLE_H_
#define LINKEDLISTABLE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
class LinkedListable;

#include "GeneralDefinitions.h"
#include "Iterator.h"
#include "SearchFilter.h"
#include "SortFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A dynamic linked list object.
 *
 * This object is an item connected to others list element thanks to a pointer to the next element.
 * It is a single linked list, so it is possible moving only in one direction (forward).
 *
 * These member functions provide insert, extract, peek, sort and other operations delegating
 * specific actions to the objects defined in Iterators.h.
 */
class LinkedListable {

public:

    /**
     * @brief Constructor.
     */
    LinkedListable();

    /**
     * @brief Destructor.
     * @details Needs to be virtual to allow proper deallocation on derived classes when used on a generic holder.
     * It sets the next pointer to NULL.
     */
    virtual ~LinkedListable();

    /**
     * @brief Returns the next element of the list.
     * @details If the return value is NULL this object is the last element in the list.
     * @return a pointer to the next element in the list.
     */
    LinkedListable * Next();

    /**
     * @brief Sets the next element of the list.
     * @details The previous next element is substituted by the input parameter.
     * @param[in] p is a pointer to the element to be added as the next.
     * @pre p != NULL.
     */
    void SetNext(LinkedListable * const p);

    /**
     * @brief The amount of elements in the sub-list at the right of this element (this element included).
     * @return number of elements in the sub-list at the right of this element (this element included).
     */
    uint32 Size();

    /**
     * @brief Bubble Sort the sub-list to the right of this element.
     * @details The algorithm assumes that if the return value of the SortFilter is >0 the two elements are not ordered,
     * otherwise if it returns a value <0 than the two elements are supposed ordered.
     * @param[in] sorter defines the comparison law.
     */
    void BSort(SortFilter * const sorter);

    /**
     * @brief Bubble Sort the sub-list to the right of this element.
     * @details The algorithm assumes that if the return value of the SortFilter is >0 the two elements are not ordered,
     * otherwise if it returns a value <0 than the two elements are supposed ordered.
     * @param[in] sorter defines the comparison law. */
    void BSort(SortFilterFn * const sorter);

    /**
     * @brief Inserts the entire list in input on the next location.
     * @param[in] p is the a pointer to LinkedListable to insert.
     */
    void Insert(LinkedListable * p);

    /**
     * @brief Inserts sorted all the elements of the sub-list p beginning to the next position.
     * @details if sorter is NULL the unsorted insert function is called.
     * @param[in] p is a pointer to the LinkedListable to insert.
     * @param[in] sorter defines the comparison law.
     */
    void Insert(LinkedListable *p,
                SortFilter * const sorter);

    /**
     * @brief Inserts sorted all the elements of the sub-list p beginning to the next position.
     * @details if sorter is NULL the unsorted insert function is called.
     * @param[in] p is a pointer to the LinkedListable to insert.
     * @param sorter defines the comparison law.
     */
    void Insert(LinkedListable *p,
                SortFilterFn * const sorter);

    /**
     * @brief Appends the first element of input list to the end of this list.
     * @param[in] p is a pointer to the LinkedListable element to be appended.
     * @post After the function p points to the last element of the list.
     */
    void Add(LinkedListable * const p);

    /**
     * @brief Appends the input list to the end of this list.
     * @param[in] p is a pointer to the LinkedListable to be appended.
     * @post After the function p points to the last element of the list.
     */
    void AddL(LinkedListable * const p);

    /**
     * @brief Searches if p is a member of the sub-list on the right of this element.
     * @param[in] p is a pointer to the LinkedListable to search.
     * @return true if the element is the sub-list, false otherwise.
     */
    bool Search(const LinkedListable * const p);

    /**
     * @brief Searches an element using a specific criteria in the sub-list on the right of this element.
     * @param[in] filter defines the search criteria.
     * @return the LinkedListable pointer if it is found, NULL otherwise.
     */
    LinkedListable *Search(SearchFilter * const filter);

    /**
     * @brief Searches an element using a specific criteria in the sub-list on the right of this element.
     * @param[in] filter defines the search criteria.
     * @return the LinkedListable pointer if it is found, NULL otherwise.
     */
    LinkedListable *Search(SearchFilterFn * const filter);

    /**
     * @brief Removes the requested element from list. Start searching from next element.
     * @param[in] p is the pointer to the element to be removed.
     * @return true if the element is removed, false if the element is not found in the sub-list.
     */
    bool Extract(LinkedListable * const p);

    /**
     * @brief Finds and removes one element from list using filter as criteria. Start searching from next element.
     * @details The first element which pass the search test is removed.
     * @param[in] filter defines the search criteria.
     * @return the pointer to the LinkedListable if it is found and removed, NULL otherwise.
     */
    LinkedListable *Extract(SearchFilter * const filter);

    /**
     * @brief Finds and removes one element from list using filter as criteria.
     * @details The first element which pass the search test is removed.
     * @param[in] filter define the search criteria.
     * @return the pointer to the LinkedListable if it is removed, NULL otherwise.
     */
    LinkedListable *Extract(SearchFilterFn * const filter);

    /**
     * @brief Deletes the requested element. Start searching from next element.
     * @details The object will be destroyed.
     * @param[in] p is the pointer to the element to be deleted.
     * @return true if the element is the list, false otherwise.
     */
    bool Delete(LinkedListable * const p);

    /**
     * @brief Deletes all elements which satisfy a specific criteria. Start searching from the next element.
     * @param[in] filter define the search criteria.
     * @return the number of elements deleted.
     */
    uint32 Delete(SearchFilter * const filter);

    /**
     * @brief Deletes all elements which satisfy a specific criteria. Start searching from the next element.
     * @param[in] filter define the search criteria.
     * @return the number of elements deleted.
     */
    uint32 Delete(SearchFilterFn * const filter);

    /**
     * @brief Browses through the list.
     * @param[in] index is the position of the requested element (0 is the current).
     * @return the pointer to the desired element.
     */
    LinkedListable *Peek(uint32 index);

    /**
     * @brief For each item in the list do something specified by input.
     * @param[in] it defines the action to do for each element.
     */
    void Iterate(Iterator * const it);

    /**
     * @brief For each item in the list do something specified by input.
     * @param[in] it defines the action to do for each element.
     */
    void Iterate(IteratorFn * const it);

private:

    /**
     * Pointer to the next element in the list.
     */
    LinkedListable *next;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LINKEDLISTABLE_H_ */
