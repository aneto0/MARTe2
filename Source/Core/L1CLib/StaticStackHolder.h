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
 * @brief A class that can handle a stack of any type.
 */
#ifndef STATIC_STACK_HOLDER_H
#define STATIC_STACK_HOLDER_H

#include "GeneralDefinitions.h"
#include "StaticListHolder.h"

/** @brief This class implements a stack (LIFO) using the StaticListHolder functions.
 *
 * These are functions for the stack manipulation like push, pop, peek.
 */

/** @brief Implementation of a Stack derived from StaticListHolder (LIFO)*/
class StaticStackHolder: protected StaticListHolder {

public:

    /** @brief Constructor. Creates a Stack with the given element size.
     @param elementSize is the factor sizeof(element)/sizeof(intptr*). */
    StaticStackHolder(int elementSize = 1) {
        this->elementSize = elementSize;
    }
    ;

    /** @brief Insert on top a single element. 
     * @param element is a pointer to the element to copy in the stack. */
    inline void StackPush(const intptr *element) {
        ListAdd(element);
    }

    /** @brief Get from Top.
     * @param element is a pointer to the element in return.
     * @return false in case of semaphore errors or if the stack is empty, true otherwise. */
    inline bool StackPop(intptr *element) {
        return ListExtract(element);
    }

    /** @brief Get the depth of the stack.
     * @return the number of elements in the stack. */
    inline uint32 StackDepth() {
        return ListSize();
    }

    /** @brief Looks into the stack.
     * @param element is a pointer to the element in return.
     * @param position is the index of the requested element (0 is the top).
     * @return true if the position is correct and the semaphore lock does not fail. */
    inline bool StackPeek(intptr *element, int position) {
        return ListPeek(element, ListSize() - 1 - position);
    }

    /** @brief Show the top of stack.
     * @param element is the pointer of the element in return.
     * @return true if the stack is not empty and the semaphore lock does not fail.*/
    inline bool StackTop(intptr *element) {
        return ListPeek(element);
    }

};

#endif
