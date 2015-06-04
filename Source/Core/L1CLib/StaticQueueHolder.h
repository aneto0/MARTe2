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
 * @brief A class that can handle a Queue of any type.
 */
#ifndef STATIC_QUEUE_HOLDER_H
#define STATIC_QUEUE_HOLDER_H

#include "StaticStackHolder.h"

/** @brief This class implements a queue (FIFO) using the StaticListHolder functions.
 * 
 * These functions implement the most important features of a queue, the insert and the 
 * extract of the oldest element in the list. */

/** A class for the implementation of a static queue (FIFO). */
class StaticQueueHolder: protected StaticStackHolder {
protected:

public:

    /** @brief Constructor. Creates a queue with the given element size.
     * @param elementSize32bit is sizeof(element)/sizeof(intptr*). */
    StaticQueueHolder(int elementSize32bit = 1) {
        this->elementSize = elementSize32bit;
    }
    ;

    /** @brief Return the last inserted element.
     * @param element is the pointer to the element in return. */
    inline bool Last(intptr *element) {
        return StackTop(element);
    }
    ;

    /** @brief Return the number of elements in the queue.
     * @return the number of elements in the queue. */
    inline uint32 QueueSize() {
        return StackDepth();
    }
    ;

    /** @brief Insert an element on the queue.
     * @param element is a pointer to the element to copy in the queue. */
    inline void QueueAdd(const intptr *element) {
        StackPush(element);
    }

    /** @brief Removes the oldest element from the queue.
     * @param element is a pointer to the element in return.
     * @return true if the element is in the list and the semaphore lock does not fail. */
    inline bool QueueExtract(intptr *element) {
        return ListExtract(element, SLH_StartOfList);
    }

    /** @brief Looks into the queue.
     * @param element is a pointer to the element in return.
     * @param index is the index of the requested element (index = 0 is the most recent added).
     * @return true if the index is correct and the semaphore lock does not fail. */
    inline bool QueuePeek(intptr *element, uint32 index) {
        return StackPeek(element, index);
    }

    /** @brief Looks into the queue to the last element inserted.
     * @param element is a pointer to the element in return.
     * @return true if the queue is not empty and the semaphore lock does not fail. */
    inline bool QueuePeekLast(intptr *element) {
        return QueuePeek(element, 0);
    }

};

#endif
