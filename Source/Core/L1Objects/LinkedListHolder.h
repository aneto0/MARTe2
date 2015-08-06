/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
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
 * See the Licence for the specific language governing 
   permissions and limitations under the Licence. 
 *
 * $Id: LinkedListHolder.h 3 2012-01-15 16:26:07Z aneto $
 *
**/

/**
 * @file
 * A class that can handle one or more linked list
 */

#ifndef LINKEDLISTHOLDER_H
#define LINKEDLISTHOLDER_H

#include "LinkedListable.h"

class LinkedListHolder {
protected:
    /** */
    LinkedListable  llhRoot;

    /** */
    uint32          llhSize;

public:

    /** will deallocate all the contents */
    void CleanUp(){
        LinkedListable *p = llhRoot.next;
        llhRoot.next = NULL;
        while(p != NULL){
            LinkedListable *q = p;
            p = p->Next();
            delete q;
        }
        llhSize = 0;
    }


    /** does the equivalent of initialization: sets 0 to llhSize and NULL
        to llhRoot->next */
    void Reset(){
        llhRoot.next = NULL;
        llhSize = 0;
    }

    /** */
    LinkedListHolder(){
        llhSize = 0;
        llhRoot.next = NULL;
    }

    /**  will deallocate all the
         contents providing all the members have virtual destructors */
    virtual ~LinkedListHolder(){
        CleanUp();
    }

    /**  return the first element */
    LinkedListable *List() const{
        return llhRoot.Next();
    };

    /**  return the first element */
    uint32 ListSize()const {
        return llhSize;
    };

    /**  insert in the first location the element p */
    inline void FastListInsertSingle(LinkedListable &p){
        llhSize++;
        p.next = llhRoot.next;
        llhRoot.next = &p;
    }

    /**  insert in the first location the list p */
    void ListInsert(LinkedListable *p){
        llhSize += p->Size();
        llhRoot.Insert(p);
    }

    /**  insert in the right location the list p using sorting sorter */
    void ListInsert(LinkedListable *p,SortFilter *sorter){
        llhSize += p->Size();
        llhRoot.Insert(p,sorter);
    }

    /**  insert in the right location the list p using sorting sorter */
    void ListInsert(LinkedListable *p,SortFilterFn *sorter){
        llhSize += p->Size();
        llhRoot.Insert(p,sorter);
    }

    /**  inserts as the n-th element of list (if possible otherwise as bottom) */
    void ListInsert(LinkedListable *q,uint32 index){
        if (q == NULL) return;
        LinkedListable *p = &llhRoot;
        while ((p->next != NULL) && (index > 0)){ p = p->next; index--; }
        llhSize += q->Size();
        p->Insert(q);
    }

    /**  insert in the first location the list p */
    void ListInsertL(LinkedListHolder *p){
        if (p == NULL) return;
        llhSize += p->ListSize();
        llhRoot.Insert(p->List());
        p->llhRoot.next = NULL;
        p->llhSize = 0;
    }

    /**  insert in the right location the list p using sorting sorter */
    void ListInsertL(LinkedListHolder *p,SortFilter *sorter){
        if (p == NULL) return;
        llhSize += p->ListSize();
        llhRoot.Insert(p->List(),sorter);
        p->llhRoot.next = NULL;
        p->llhSize = 0;
    }

    /**  insert in the right location the list p using sorting sorter */
    void ListInsertL(LinkedListHolder *p,SortFilterFn *sorter){
        if (p == NULL) return;
        llhSize += p->ListSize();
        llhRoot.Insert(p->List(),sorter);
        p->llhRoot.next = NULL;
        p->llhSize = 0;
    }

    /**  inserts as the n-th element of list (if possible otherwise as bottom) */
    void ListInsertL(LinkedListHolder *p,uint32 index){
        if (p == NULL) return;
        ListInsert(p->llhRoot.next,index);
        p->llhRoot.next = NULL;
        p->llhSize = 0;
    }

    /**  add an element at the end of the list */
    void ListAdd(LinkedListable *p){
        llhSize ++;
        llhRoot.Add(p);
    }

    /**  add an element at the end of the list */
    void ListAddL(LinkedListable *p){
        llhSize += p->Size();
        llhRoot.AddL(p);
    }

    /**  search if p is a member */
    bool ListSearch(LinkedListable *p){
        if (llhRoot.Next() == NULL) return false;
        return llhRoot.Next()->Search(p);
    }

    /**  search an element using a specific criteria */
    LinkedListable *ListSearch(SearchFilter *filter){
        if (llhRoot.Next() == NULL) return NULL;
        return llhRoot.Next()->Search(filter);
    }

    /**  search an element using a specific criteria */
    LinkedListable *ListSearch(SearchFilterFn *filter){
        if (llhRoot.Next() == NULL) return NULL;
        return llhRoot.Next()->Search(filter);
    }

    /**  remove the requested element from the list.
         start searching from next element */
    bool ListExtract(LinkedListable *p){
        if (llhRoot.Extract(p) == true){
            llhSize --;
            return true;
        } else return false;
    }

    /**  find and remove one element from list using filter as criteria */
    LinkedListable *ListExtract(SearchFilter *filter){
        LinkedListable *p = llhRoot.Extract(filter);
        if (p != NULL) llhSize --;
        return p;
    }

    /**  find and remove one element from list using filter */
    LinkedListable *ListExtract(SearchFilterFn *filter){
        LinkedListable *p = llhRoot.Extract(filter);
        if (p != NULL) llhSize --;
        return p;
    }

    /**  delete the requested element. start searching from next element */
    bool ListDelete(LinkedListable *p){
        if (llhRoot.Delete(p) == true){
            llhSize --;
            return true;
        } else return false;
    }

    /**  delete an element using a specific criteria */
    bool ListDelete(SearchFilter *filter){
        uint32 deleted = llhRoot.Delete(filter);
        llhSize -= deleted;
        return (deleted !=0);
    }

    /** delete an element using a specific criteria.
        Safe from reentrance from destructor of object
       (complex graph destruction) */
    bool ListSafeDelete(SearchFilter *filter){
        if (filter == NULL) return false;
        int deleted = 0;
        LinkedListable *p = List();
        while(p != NULL){
            if (filter->Test(p)){
                if (ListExtract(p))  {
                    delete p;
                    // p->next may not be a safe pointer so assume the worst and start over again
                    p = List();
                    deleted++;
                } else p = p->next;
            } else p = p->next;
        }
        return (deleted != 0);
    }

    /**  delete an element using a specific criteria */
    bool ListDelete(SearchFilterFn *filter){
        uint32 deleted = llhRoot.Delete(filter);
        llhSize -= deleted;
        return (deleted !=0);
    }


    /**  Bubble Sort the sub-list to the right of this element */
    void ListBSort(SortFilter *sorter){
        llhRoot.BSort(sorter);
    }

    /**  Bubble Sort the sub-list to the right of this element */
    void ListBSort(SortFilterFn *sorter){
        llhRoot.BSort(sorter);
    }

    /**  looks into the list in an ordered fashion: index = 0 is the top */
    LinkedListable *ListPeek(uint32 index){
        return llhRoot.Next()->Peek(index);
    }

    /**  removes and Gets the n-th element of the list */
    LinkedListable *ListExtract(uint32 index=0){
        LinkedListable *p = &llhRoot;
        while ((p != NULL) && (index > 0)){ p = p->next; index--; }
        LinkedListable *q = p->next;
        if (q != NULL){
            llhSize--;
            p->next = q->next;
        }
        if (q != NULL) q->next = NULL;
        return q;
    }

    /**  for each item in the list do <it> */
    void ListIterate(Iterator *it){
        if (llhRoot.Next() != NULL) llhRoot.Next()->Iterate(it);
    }

    /**  for each item in the list do <it> */
    void ListIterate(IteratorFn *it){
        if (llhRoot.Next() != NULL) llhRoot.Next()->Iterate(it);
    }

};



#endif
