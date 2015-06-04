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
 * @brief Queue of QueueAbles
 */
#ifndef QUEUE_HOLDER_H
#define QUEUE_HOLDER_H

#include "StackHolder.h"

/** This is a particular case of the LinkedListHolder class.
  *
  * These functions implements the FIFO rules to add and remove elements
  * basically using only some functions already defined for list implementation.
  * 
  * Elements are added on the last position and extracted from the first position. */




/** @brief A class that can handle a Queue of QueueAbles (FIFO). */
class QueueHolder: protected StackHolder {
protected:
    /** pointer to the end of the list */
    Queueable *insertionPoint;

public:
    /** @brief Constructor. */
    QueueHolder() {
        insertionPoint = &llhRoot;
    }

    /** @brief Return the oldest inserted element (the root namely the first position).
      * @return a pointer to the oldest element. */
    Queueable *Oldest() {
        return List();
    }

    /** @brief Return the number of elements in the queue.
      * @return the size of the queue. */
    uint32 QueueSize() {
        return llhSize;
    }

    /** @brief Insert an element or a list at the end of the list.
      * @param p is a pointer to the LinkedListable to add on the queue. */
    void QueueAdd(Queueable *p) {
        if (p == NULL) {
            return;
        }
        llhSize += p->Size();
        insertionPoint->Insert(p);
        while (p->next != NULL)
            p = p->next;
        insertionPoint = p;
    }

    /** @brief Insert an element or a list at the beginning of the list.
      * @param p is a pointer to the LinkedListable to add on the top. */
    void QueueInsert(Queueable *p) {
        ListInsert(p);
        if (insertionPoint == &llhRoot && p != NULL) {
            while (p->next != NULL) {
                p = p->next;
            }
            insertionPoint = p;
        }
    }

    /** @brief Removes the oldest element from the queue.
      * @return a pointer to the oldest element of the list (the first). */
    Queueable *QueueExtract() {
        Queueable *p = StackPop();
        if (llhRoot.next == NULL)
            insertionPoint = &llhRoot;
        return p;
    }

    /** @brief Removes a specific element.
      * @param p is a pointer to the element which must be removed.
      * @return true if the element is the list, false otherwise. */
    bool QueueExtract(Queueable *p) {
        bool ret = ListExtract(p);
        if (p == insertionPoint) {
            insertionPoint = &llhRoot;
            while (insertionPoint->next != NULL) {
                insertionPoint = insertionPoint->next;
            }
        }
        return ret;
    }

    /** @brief Looks into the queue.
      * @param index defines the position of the requested element.
      * @return the pointer to the element if it is found, NULL otherwise. 
      * Index=0 is the element at the end of the list. */
    Queueable *QueuePeek(uint32 index) {
        return ListPeek(llhSize - index - 1);
    }

    /** @brief Looks into the queue to the last element inserted.
      * @return the element at the end of the list. */
    Queueable *QueuePeekLast() {
        return insertionPoint;
    }

    /** @brief Reset the queue. */
    void Reset() {
        LinkedListHolder::Reset();
        insertionPoint = &llhRoot;
    }

    /** @brief Insert in the first location the element p.
      * @param p is the element to insert on the queue. */
    inline void FastQueueInsertSingle(LinkedListable &p) {
        llhSize++;
        insertionPoint->next = &p;
        insertionPoint = &p;
        p.next = NULL;
    }

};

#endif
