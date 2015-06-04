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
 * @brief Linked list implementation
 */
#ifndef LINKED_LISTABLE_H
#define LINKED_LISTABLE_H

#include "GeneralDefinitions.h"
#include "Iterators.h"

/** @brief A dynamic linked list object.
  *
  * This class contains an attribute which is a pointer to the same object type allowing
  * a link with other objects like it creating a list.
  *
  * These member functions provide insert, extract, peek, sort and other operations delegating
  * specific actions to the objects defined in Iterators.h for a more general implementation. */





class LinkedListable {
friend class MultiLinkedListHolder;
friend class LinkedListHolder;
friend class StackHolder;
friend class QueueHolder;
protected:

    /** Pointer to the next element. */		
    LinkedListable *next;
public:
    
    /** @brief Constructor. */
    LinkedListable(){
        next = NULL;
    }

    /** @brief Destructor.
      * Needs to be virtual to allow proper deallocation on derived classes when used on a generic holder. */
    virtual ~LinkedListable(){ next = NULL; }

    /** @brief Return the next element.
      * @return a pointer to the next LinkedListable object.
      * If the return value is NULL this is the lost in the list. */
    LinkedListable *Next()const{
        return next;
    }

    /** @brief Set the next element.
      * @param p is a pointer to the desired next element. */
    void SetNext(LinkedListable *p){
        next = p;
    }

    /** @brief The amount of elements in this sub-list the current element included.
      * @return number of elements in the list after the current (current included). */
    uint32 Size(){
        LinkedListable *p = this;
        uint32 count = 0;
        while (p!=NULL){ p = p->next;count++;}
        return count;
    }

    /** @brief Bubble Sort the sub-list to the right of this element.
      * @param sorter define the comparation law. (>0 not ordered, <0 ordered). */
    void BSort(SortFilter *sorter){
        if (sorter == NULL) return ;
        if (next == NULL) return ;
        uint32 count = 1;
        LinkedListable *p = this;
        while(p->next->next != NULL){
            count++;
            LinkedListable *a1 = p->next;
            LinkedListable *a2 = p->next->next;
            if (sorter->Compare(a1,a2) > 0){
                a1->next = a2->next;
                a2->next = a1;
                p->next = a2;
                p = a2;
            } else p = a1;
        }
        while (count > 2){
            LinkedListable *p = this;
            uint32 index = count;
            while(index > 2){
                index--;
                LinkedListable *a1 = p->next;
                LinkedListable *a2 = p->next->next;
                if (sorter->Compare(a1,a2) > 0){
                    a1->next = a2->next;
                    a2->next = a1;
                    p->next = a2;
                    p = a2;
                } else p = a1;
            }
            count--;
        }
    }

    /** @brief Bubble Sort the sub-list to the right of this element.
      * @param sorter define the comparation law. (>0 not ordered, <0 ordered). */
    void BSort(SortFilterFn *sorter){
        if (sorter == NULL) return ;
        if (next == NULL) return ;
        uint32 count = 1;
        LinkedListable *p = this;
        while(p->next->next != NULL){
            count++;
            LinkedListable *a1 = p->next;
            LinkedListable *a2 = p->next->next;
            if (sorter(a1,a2) > 0){
                a1->next = a2->next;
                a2->next = a1;
                p->next = a2;
                p = a2;
            } else p = a1;
        }
        while (count > 2){
            LinkedListable *p = this;
            uint32 index = count;
            while(index > 2){
                index--;
                LinkedListable *a1 = p->next;
                LinkedListable *a2 = p->next->next;
                if (sorter(a1,a2) > 0){
                    a1->next = a2->next;
                    a2->next = a1;
                    p->next = a2;
                    p = a2;
                } else p = a1;
            }
            count--;
        }
    }

    /** @brief Insert in the next location the list p.
      * @param p is the a pointer to LinkedListable to insert. */
    void Insert(LinkedListable *p){
        if (p == NULL) return ;
        LinkedListable *q = next;
        next = p;
        if (q == NULL) return;
        while(p->next != NULL) p = p->next;
        p->next = q;
    }

    /** @brief Insert sorted all the elements of the sub-list p.
      * @param p is a pointer to the LinkedListable to insert.
      * @param sorter define the comparation law. */
    void Insert(LinkedListable *p,SortFilter *sorter){
        if (p == NULL) return;
        if (sorter == NULL){
            Insert(p);
            return;
        }
        if (p->next != NULL){
            LinkedListable root;
            root.next = p;
            root.BSort(sorter);
            p = root.next;
        }
        LinkedListable *list = this;
        while ((p != NULL) &&(list->next != NULL)){
            if (sorter->Compare(list->next,p)>0){
                LinkedListable *item = p;
                p = p->next;
                item->next = list->next;
                list->next = item;
            } else list = list->next;
        }
        if (p !=NULL) list->next = p;
    }

    /** @brief Insert sorted all the elements of the sub-list p.
      * @param p is a pointer to the LinkedListable to insert.
      * @param sorter define the comparation law. */
    void Insert(LinkedListable *p,SortFilterFn *sorter){
        if (p == NULL) return;
        if (sorter == NULL){
            Insert(p);
            return;
        }
        if (p->next != NULL){
            LinkedListable root;
            root.next = p;
            root.BSort(sorter);
            p = root.next;
        }
        LinkedListable *list = this;
        while ((p != NULL) &&(list->next != NULL)){
            if (sorter(list->next,p)>0){
                LinkedListable *item = p;
                p = p->next;
                item->next = list->next;
                list->next = item;
            } else list = list->next;
        }
        if (p !=NULL) list->next = p;
    }

    /** @brief Add an element at the end of the queue.
      * @param p is a pointer to the LinkedListable element to add. */
    void Add(LinkedListable *p){
        if (p == NULL) return ;
        LinkedListable *q = this;
        while (q->next != NULL){
	        q = q->next;
		}
        q->next = p;
	    p->next = NULL;
    }

    /** @brief a LinkedListable list at the end of the queue.
      * @param p is a pointer to the LinkedListable to add. */
    void AddL(LinkedListable *p){
        if (p == NULL) return ;
        LinkedListable *q = this;
        while (q->next != NULL){
	    q = q->next;
	}
        q->next = p;
    }

    /** @brief Search if p is a member.
      * @param p is a pointer to the LinkedListable to search.
      * @return true if the element is the list after current, false otherwise. */
    bool Search(LinkedListable *p){
        LinkedListable *q = this;
        while (q!=NULL){
            if (q==p) return True;
            q = q->next;
        }
        return False;
    }

    /** @brief Search an element using a specific criteria.
      * @param filter define the search criterium. 
      * @return the LinkedListable pointer if it is found, NULL otherwise. */
    LinkedListable *Search(SearchFilter *filter){
        if (filter == NULL) return NULL;
        LinkedListable *q = this;
        while (q!=NULL){
            if (filter->Test(q)) return q;
            q = q->next;
        }
        return NULL;
    }

    /** @brief Search an element using a specific criteria.
      * @param filter define the search criterium. 
      * @return the LinkedListable pointer if it is found, NULL otherwise. */
    LinkedListable *Search(SearchFilterFn *filter){
        if (filter == NULL) return NULL;
        LinkedListable *q = this;
        while (q!=NULL){
            if (filter(q)) return q;
            q = q->next;
        }
        return NULL;
    }

    /** @brief Remove the requested element from list. Start searching from next element.
      * @param p is the pointer to the element due to remove it.
      * @return true if the element is removed, false otherwise (the element is not found). */
    bool Extract(LinkedListable *p){
        if (p == NULL) return False;
        LinkedListable *q = this;
        if (p == q) return False;
        while ((q->next != p) && (q->next != NULL)) q= q->next;
        if (q->next == NULL) return False;
        q->next = q->next->next;
        p->next = NULL;
        return True;
    }

    /** @brief Find and remove one element from list using filter as criteria.
      * @param filter define the search criterium.
      * @return the pointer to the LinkedListable if it is removed, NULL otherwise. */
    LinkedListable *Extract(SearchFilter *filter){
        if (filter == NULL) return NULL;
        LinkedListable *q = this;
        while (q->next != NULL){
            if (filter->Test(q->next)){
                LinkedListable *p = q->next;
                q->next = q->next->next;
                p->next = NULL;
                return p;
            } else q = q->next;
        }
        return NULL;
    }

    /** @brief Find and remove one element from list using filter as criteria.
      * @param filter define the search criterium.
      * @return the pointer to the LinkedListable if it is removed, NULL otherwise. */
    LinkedListable *Extract(SearchFilterFn *filter){
        if (filter == NULL) return NULL;
        LinkedListable *q = this;
        while (q->next != NULL){
            if (filter(q->next)){
                LinkedListable *p = q->next;
                q->next = q->next->next;
                p->next = NULL;
                return p;
            } else q = q->next;
        }
        return NULL;
    }

    /** @brief Delete the requested element. Start searching from next element.
      * @param p is the pointer to the element to delete.
      * @return true if the element is the list, false otherwise. */
    bool Delete(LinkedListable *p){
        bool ret = Extract(p);
        if (ret) delete p;
        return ret;
    }

    /** @brief Delete elements using a specific criteria.
      * @param filter define the search criterium.
      * @return the number of elements deleted. */
    uint32 Delete(SearchFilter *filter){
        uint32 deleted = 0;
        if (filter == NULL) return deleted;
        LinkedListable *q = this;
        while (q->next != NULL){
            if (filter->Test(q->next)){
                LinkedListable *p = q->next;
                q->next = q->next->next;
                delete p;
                deleted++;
            } else q = q->next;
        }
        return deleted;
    }

    /** @brief Delete elements using a specific criteria.
      * @param filter define the search criterium.
      * @return the number of elements deleted. */
    uint32 Delete(SearchFilterFn *filter){
        uint32 deleted = 0;
        if (filter == NULL) return deleted;
        LinkedListable *q = this;
        while (q->next != NULL){
            if (filter(q->next)){
                LinkedListable *p = q->next;
                q->next = q->next->next;
                delete p;
                deleted++;
            } else q = q->next;
        }
        return deleted;
    }

    /** @brief Browse through the list.
      * @param index is the index of the requested element (0 is the current).
      * @return the pointer to the element index positions after this. */
    LinkedListable *Peek(uint32 index){
        LinkedListable *p = this;
        while((p != NULL) && (index > 0)) { p = p->next; index--; }
        return p;
    }

    /** @brief For each item in the list do something.
      * @param it define what to do for each element. */
    void Iterate(Iterator *it){
        LinkedListable *p = this;
        while(p != NULL){
            it->Do(p);
            p = p->next;
        }
    }

    /** @brief For each item in the list do something.
      * @param it define what to do for each element. */
    void Iterate(IteratorFn *it){
        LinkedListable *p = this;
        while(p != NULL){
            it(p);
            p = p->next;
        }
    }

};


#endif
