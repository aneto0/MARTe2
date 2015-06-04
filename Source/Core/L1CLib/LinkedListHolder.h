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
 * $Id: $
 *
 **/
/**
 * @file
 * @brief A class that can handle one or more linked list
 */

#ifndef LINKED_LIST_HOLDER_H
#define LINKED_LIST_HOLDER_H

#include "LinkedListable.h"

/** @brief Implementation of a dynamic single linked list.
  *
  * Adding the memory of the root and using functions defined for LinkedListable objects,
  * this class performs all the foundamental methods to manipulate a list.
  *
  * To implement specific object lists this class must be derived. */


/** @brief The LinkedListHolder class. */
class LinkedListHolder {
protected:
    /** List root */
    LinkedListable llhRoot;

    /** List size */
    uint32 llhSize;

public:

    /** @brief Deallocate all the contents. */
    void CleanUp() {
        LinkedListable *p = llhRoot.next;
        llhRoot.next = NULL;
        while (p != NULL) {
            LinkedListable *q = p;
            p = p->Next();
            delete q;
        }
        llhSize = 0;
    }

    /** @brief Sets 0 to llhSize and NULL to llhRoot->next. */
    void Reset() {
        llhRoot.next = NULL;
        llhSize = 0;
    }

    /** @brief Constructor. */
    LinkedListHolder() {
        llhSize = 0;
        llhRoot.next = NULL;
    }

    /** @brief Destructor. */
    virtual ~LinkedListHolder() {
        CleanUp();
    }

    /** @brief Return the first element.
      * @return a pointer to the first element of the list. */
    LinkedListable *List() const {
        return llhRoot.Next();
    }
    ;

    /** @brief Return the number of elements in the list. 
      * @return the number of the elements in the list. */
    uint32 ListSize() const {
        return llhSize;
    }
    ;

    /** @brief Insert in the first location the element p.
      * @param p is the LinkedListable object to insert. */
    inline void FastListInsertSingle(LinkedListable &p) {
        llhSize++;
        p.next = llhRoot.next;
        llhRoot.next = &p;
    }

    /** @brief Insert in the first location the list p.
      * @param p is a pointer to the LinkedListable to insert. */
    void ListInsert(LinkedListable *p) {
        if (p == NULL) {
            return;
        }
        llhSize += p->Size();
        llhRoot.Insert(p);
    }

    /** @brief Insert in the right location the list p using sorting sorter.
      * @param p is the pointer to the LinkedListable to insert.
      * @param sorter defines the comparation criterium. */
    void ListInsert(LinkedListable *p, SortFilter *sorter) {
        if (p == NULL) {
            return;
        }
        llhSize += p->Size();
        llhRoot.Insert(p, sorter);
    }

    /** @brief Insert in the right location the list p using sorting sorter.
      * @param p is the pointer to the LinkedListable to insert.
      * @param sorter defines the comparation criterium. */
    void ListInsert(LinkedListable *p, SortFilterFn *sorter) {
        if (p == NULL) {
            return;
        }
        llhSize += p->Size();
        llhRoot.Insert(p, sorter);
    }

    /** @brief Inserts a list at the n-th position of the list (if possible, otherwise as bottom)
      * @param q is the pointer to the LinkedListable to insert.
      * @param index is the position in the list where p must be inserted.*/
    void ListInsert(LinkedListable *q, uint32 index) {
        if (q == NULL)
            return;
        LinkedListable *p = &llhRoot;
        while ((p->next != NULL) && (index > 0)) {
            p = p->next;
            index--;
        }
        llhSize += q->Size();
        p->Insert(q);
    }

    /** @brief Insert in the first location the list p
      * @param p is the pointer to the LinkedListHolder to insert. */ 
    void ListInsertL(LinkedListHolder *p) {
        if (p == NULL)
            return;
        llhSize += p->ListSize();
        llhRoot.Insert(p->List());
        p->llhRoot.next = NULL;
        p->llhSize = 0;
    }

    /** @brief Insert in the right location the list p using sorting sorter.
      * @param p is the pointer to the list to insert.
      * @param sorter defines the comparation criterium.  */
    void ListInsertL(LinkedListHolder *p, SortFilter *sorter) {
        if (p == NULL)
            return;
        llhSize += p->ListSize();
        llhRoot.Insert(p->List(), sorter);
        p->llhRoot.next = NULL;
        p->llhSize = 0;
    }

    /** @brief Insert in the right location the list p using sorting sorter.
      * @param p is a pointer to the list to insert.
      * @param sorter defines the comparation criterium. */
    void ListInsertL(LinkedListHolder *p, SortFilterFn *sorter) {
        if (p == NULL)
            return;
        llhSize += p->ListSize();
        llhRoot.Insert(p->List(), sorter);
        p->llhRoot.next = NULL;
        p->llhSize = 0;
    }

    /** @brief Inserts as the n-th element of list (if possible, otherwise as bottom).
      * @param p is the pointer to the list to insert.
      * @param index is the location where p must be inserted. */
    void ListInsertL(LinkedListHolder *p, uint32 index) {
        if (p == NULL)
            return;
        ListInsert(p->llhRoot.next, index);
        p->llhRoot.next = NULL;
        p->llhSize = 0;
    }

    /** @brief Add an element at the end of the list.
      * @param p is a pointer to the LinkedListable element to add. */
    void ListAdd(LinkedListable *p) {
        if (p == NULL) {
            return;
        }
        llhSize++;
        llhRoot.Add(p);
    }

    /** @brief Add a LinkedListable list at the end of the list.
      * @param p is the pointer to the LinkedListable list to add. */
    void ListAddL(LinkedListable *p) {
        if (p == NULL) {
            return;
        }
        llhSize += p->Size();
        llhRoot.AddL(p);
    }

    /** @brief Search if p is a member.
      * @param p is a pointer to the element to search.
      * @return true if p is in the list, false otherwise. */
    bool ListSearch(LinkedListable *p) {
        if (llhRoot.Next() == NULL)
            return False;
        return llhRoot.Next()->Search(p);
    }

    /** @brief Search an element using a specific criteria.
      * @param filter defines the search criterium.
      * @return a pointer to the element if it is found, NULL otherwise. */
    LinkedListable *ListSearch(SearchFilter *filter) {
        if (llhRoot.Next() == NULL)
            return NULL;
        return llhRoot.Next()->Search(filter);
    }

    /** @brief Search an element using a specific criteria.
      * @param filter defines the search criterium.
      * @return a pointer to the element if it is found, NULL otherwise. */
    LinkedListable *ListSearch(SearchFilterFn *filter) {
        if (llhRoot.Next() == NULL)
            return NULL;
        return llhRoot.Next()->Search(filter);
    }

    /** @brief Remove the requested element from the list.
      * @param p is the LinkedListable element to remove.
      * @return true if p is in the list. */
    bool ListExtract(LinkedListable *p) {
        if (llhRoot.Extract(p) == True) {
            llhSize--;
            return True;
        }
        else
            return False;
    }

    /** @brief Find and remove one element from list using filter as criteria.
      * @param filter defines the search criterium.
      * @return the pointer to the extracted element, NULL if it is not found in the list. */
    LinkedListable *ListExtract(SearchFilter *filter) {
        LinkedListable *p = llhRoot.Extract(filter);
        if (p != NULL)
            llhSize--;
        return p;
    }


    /** @brief Find and remove one element from list using filter as criteria.
      * @param filter defines the search criterium.
      * @return the pointer to the extracted element, NULL if it is not found in the list. */
    LinkedListable *ListExtract(SearchFilterFn *filter) {
        LinkedListable *p = llhRoot.Extract(filter);
        if (p != NULL)
            llhSize--;
        return p;
    }

    /** @brief Delete the requested element. 
      * @param p is the pointer to the element which must be deleted.
      * @return true if p is in the list, false otherwise. */
    bool ListDelete(LinkedListable *p) {
        if (llhRoot.Delete(p) == True) {
            llhSize--;
            return True;
        }
        else
            return False;
    }

    /** @brief Delete elements using a specific criteria.
      * @param filter defines the search criterium. 
      * @return true if at least one element it is deleted, false otherwise. */
    bool ListDelete(SearchFilter *filter) {
        uint32 deleted = llhRoot.Delete(filter);
        llhSize -= deleted;
        return (deleted != 0);
    }

    /** @brief Delete an element using a specific criteria.
      * @param filter defines the search criterium.
      * @return true if at least one object it is deleted, false otherwise.
      * Safe from reentrance from destructor of object (complex graph destruction) */
    bool ListSafeDelete(SearchFilter *filter) {
        if (filter == NULL)
            return False;
        int deleted = 0;
        LinkedListable *p = List();
        while (p != NULL) {
            if (filter->Test(p)) {
                if (ListExtract(p)) {
                    delete p;
                    // p->next may not be a safe pointer so assume the worst and start over again
                    p = List();
                    deleted++;
                }
                else
                    p = p->next;
            }
            else
                p = p->next;
        }
        return (deleted != 0);
    }

    /** @brief Delete elements using a specific criteria.
      * @param filter defines the search criterium.
      * @return true if at least one element it is deleted, false otherwise. */
    bool ListDelete(SearchFilterFn *filter) {
        uint32 deleted = llhRoot.Delete(filter);
        llhSize -= deleted;
        return (deleted != 0);
    }

    /** @brief Bubble Sort the list.
      * @param sorter defines the comparation criterium. */
    void ListBSort(SortFilter *sorter) {
        llhRoot.BSort(sorter);
    }

    /** @brief Bubble Sort the list.
      * @param sorter defines the comparation criterium. */
    void ListBSort(SortFilterFn *sorter) {
        llhRoot.BSort(sorter);
    }

    /** @brief Looks into the list in an ordered fashion: index = 0 is the top.
      * @param index is the position of the requested element.
      * @return a pointer to the element at index position. */
    LinkedListable *ListPeek(uint32 index) {
        return llhRoot.Next()->Peek(index);
    }

    /** @brief Removes and gets the n-th element of the list.
      * @param index is the position of the requested element.
      * @return a pointer to the element at index position.  */
    LinkedListable *ListExtract(uint32 index = 0) {
        LinkedListable *p = &llhRoot;
        while ((p != NULL) && (index > 0)) {
            p = p->next;
            index--;
        }
        if (p == NULL)
            return NULL;
        LinkedListable *q = p->next;
        if (q != NULL) {
            llhSize--;
            p->next = q->next;
        }
        if (q != NULL)
            q->next = NULL;
        return q;
    }

    /** @brief For each item in the list do something.
      * @param it defines what to do. */
    void ListIterate(Iterator *it) {
        if (llhRoot.Next() != NULL)
            llhRoot.Next()->Iterate(it);
    }

    /** @brief For each item in the list do something.
      * @param it defines what to do. */
    void ListIterate(IteratorFn *it) {
        if (llhRoot.Next() != NULL)
            llhRoot.Next()->Iterate(it);
    }

};

#endif
