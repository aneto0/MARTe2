/**
 * @file LinkedListableT.h
 * @brief Header file for class LinkedListableT
 * @date 12/04/2016
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class LinkedListableT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LINKEDLISTABLET_H_
#define LINKEDLISTABLET_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "LinkedListable.h"
#include "SortFilterT.h"
#include "SearchFilterT.h"
#include "IteratorT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


namespace MARTe {
/**
 * T must inherit from LinkedListable
 * */
template <class T>
class LinkedListableT: public T {

public:

    /**
     * @brief Destructor.
     * @details Needs to be virtual to allow proper deallocation on derived classes when used on a generic holder.
     * It sets the next pointer to NULL.
     */
    virtual ~LinkedListableT();

    /**
     * @brief Returns the next element of the list.
     * @details If the return value is NULL this object is the last element in the list.
     * @return a pointer to the next element in the list.
     */
    inline LinkedListableT<T> * Next();

    /**
     * @brief The number of elements in the sub-list at the right of this element (this element included).
     * @return the number of elements in the sub-list at the right of this element (this element included).
     */
    inline uint32 Size();

    /**
     * @brief Sets the next element of the list.
     * @details The previous next element is substituted by the input parameter.
     * @param[in] p is a pointer to the element to be added as the next.
     * @pre p != NULL.
     */
    inline void SetNext(LinkedListableT<T> * const p);

    /**
     * @brief Bubble Sort the sub-list to the right of this element.
     * @details The algorithm assumes that two elements are ordered if the SortFilter returns a value < 0.
     * @param[in] sorter implements the comparison criteria for the sorting.
     */
    inline void BSort(SortFilterT<T> * const sorter);

    /**
     * @brief Inserts the entire list as in input to the next location.
     * @param[in] p the pointer to the LinkedListable to be inserted.
     */
    inline void Insert(LinkedListableT<T> * p);

    /**
     * @brief Inserts a sorted set of all the elements from the sub-list p.
     * @details The list is inserted in the next position. If sorter is NULL,
     * the unsorted Insert(LinkedListable *) function is called.
     * @param[in] p the pointer to the LinkedListable to be inserted .
     * @param[in] sorter implements the comparison criteria for the sorting.
     */
    inline void Insert(LinkedListableT<T> *p,SortFilterT<T> * const sorter);

    /**
     * @brief Appends the first element of the input list \a p to the end of this list.
     * @param[in] p a pointer to the LinkedListable element to be appended.
     * @post After executing the function \a p will point to the last element of the list.
     * @pre \a p must be a single LinkedListable element, as his next pointer will be set
     *      to NULL after the execution of the function. If instead \a p is a list, Add
     *      could lead to memory leaks.
     */
    inline void Add(LinkedListableT<T> * const p);

    /**
     * @brief Appends the input list \a p to the end of this list.
     * @param[in] p a pointer to the LinkedListable to be appended.
     */
    inline void AddL(LinkedListableT<T> * const p);

    /**
     * @brief Searches if \a p is a member of the sub-list on the right of this element.
     * @param[in] p a pointer to the LinkedListable to be searched.
     * @return true if the element \a p is in the sub-list, false otherwise.
     */
    inline bool Search(const LinkedListableT<T> * const p);

    /**
     * @brief Searches an element using a specific criteria in the sub-list to the right of this element.
     * @param[in] filter defines the search criteria.
     * @return a pointer to the LinkedListable member or NULL if the element was not found.
     */
    inline LinkedListableT<T> *Search(SearchFilterT<T> * const filter);

    /**
     * @brief Removes the requested element \a p from list.
     * @details Searching starts from the next element.
     * @param[in] p the pointer to the element to be removed.
     * @return true if the element \a p is removed, false if the element is not found in the sub-list.
     */
    inline bool Extract(LinkedListableT<T> * const p);

    /**
     * @brief Finds and removes one element from the list using a filter as the search criteria.
     * @details Searching starts from the next element. The first element which passes the search criteria is removed.
     * @param[in] filter defines the search criteria.
     * @return a pointer to the LinkedListable if it is found and removed, NULL otherwise.
     */
    inline LinkedListable *Extract(SearchFilterT<T> * const filter);

    /**
     * @brief Deletes the requested element \a p.
     * @details Searching starts from the next element. The object will be destroyed.
     * @param[in] p is the pointer to the element to be deleted.
     * @return true if the element \a p was in the list, false otherwise.
     */
    inline bool Delete(LinkedListableT<T> * const p);

    /**
     * @brief Deletes all the elements which satisfy a specific criteria.
     * @details Searching starts from the next element.
     * @param[in] filter define the search criteria.
     * @return the number of elements deleted.
     */
    inline uint32 Delete(SearchFilterT<T> * const filter);

    /**
     * @brief Returns a pointer to the element at position \a index.
     * @param[in] index is the position of the requested element (0 is the current).
     * @return the pointer to the desired element.
     */
    inline LinkedListable *Peek(uint32 index);

    /**
     * @brief For each item in the list perform the action defined by the Iterator.
     * @param[in] it defines the action to be performed on each element.
     */
    inline void Iterate(IteratorT<T> * const it);


};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template <class T>
LinkedListableT<T>::~LinkedListableT() {
};

template <class T>
LinkedListableT<T> * LinkedListableT<T>::Next(){
    return static_cast<LinkedListableT<T>*>(LinkedListable::Next());
}

template <class T>
uint32 LinkedListableT<T>::Size(){
    return LinkedListable::Size();
}

template <class T>
void LinkedListableT<T>::SetNext(LinkedListableT<T> * const p){
    LinkedListable::SetNext(p);
}

template <class T>
void LinkedListableT<T>::BSort(SortFilterT<T> * const sorter){
    LinkedListable::BSort(sorter);
}

template <class T>
void LinkedListableT<T>::Insert(LinkedListableT<T> * p){
    LinkedListable::Insert(p);
}

template <class T>
void LinkedListableT<T>::Insert(LinkedListableT<T> *p,SortFilterT<T> * const sorter){
    LinkedListable::Insert(p,sorter);
}

template <class T>
void LinkedListableT<T>::Add(LinkedListableT<T> * const p){
    LinkedListable::Add(p);
}

template <class T>
void LinkedListableT<T>::AddL(LinkedListableT<T> * const p){
    LinkedListable::AddL(p);
}

template <class T>
bool LinkedListableT<T>::Search(const LinkedListableT<T> * const p){
    return LinkedListable::Search(p);
}

template <class T>
LinkedListableT<T> *LinkedListableT<T>::Search(SearchFilterT<T> * const filter){
    return static_cast<LinkedListableT<T>*>(LinkedListable::Search(filter));
}

template <class T>
bool LinkedListableT<T>::Extract(LinkedListableT<T> * const p){
    return LinkedListable::Extract(p);
}

template <class T>
LinkedListable *LinkedListableT<T>::Extract(SearchFilterT<T> * const filter){
    return static_cast<LinkedListableT<T> *>(LinkedListable::Extract(filter));
}

template <class T>
bool LinkedListableT<T>::Delete(LinkedListableT<T> * const p){
    return LinkedListable::Delete(p);
}

template <class T>
uint32 LinkedListableT<T>::Delete(SearchFilterT<T> * const filter){
    return LinkedListable::Delete(filter);
}

template <class T>
LinkedListable *LinkedListableT<T>::Peek(uint32 index){
    return static_cast<LinkedListableT<T> *>(LinkedListable::Peek(index));
}

template <class T>
void LinkedListableT<T>::Iterate(IteratorT<T> * const it){
    LinkedListable::Iterate(it);
}

}


#endif /* LINKEDLISTABLET_H_ */
	
