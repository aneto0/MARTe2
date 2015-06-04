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
 * @brief Stack of StackAbles
 */
#ifndef STACK_HOLDER_H
#define STACK_HOLDER_H

#include "LinkedListHolder.h"

/** This is a particular case of the LinkedListHolder class.
  *
  * These functions implements the LIFO rules to add and remove elements
  * basically using only some functions already defined for list implementation.
  * 
  * Elements are added on the first position and extracted from the first position. */


/** @brief A class that can handle a Stack of StackAbles (LIFO). */
class  StackHolder: public LinkedListHolder {
public:
    /** @brief Insert on top a single element.
      * @param p is the pointer to the LinkedListable to insert at the first position.   */
    inline void StackFastPushSingle(Stackable *p){
        if (p != NULL){
            llhSize++;
            p->next = llhRoot.next;
            llhRoot.next = p;
        }
    }

    /** @brief Remove from the first position.
      * @return a pointer to the element on top. */
    inline Stackable *StackFastPop(){
        Stackable *p = llhRoot.next;
        if (p != NULL){
            llhSize--;
            llhRoot.next = p->next;
            p->next = NULL;
        }
        return p;
    }

    /** @brief Insert on top a list of elements.
      * @param p is a pointer to the LinkedListable list to insert. */
    void StackPush(Stackable *p){
        ListInsert(p);
    }

    /** @brief Remove from the first position.
      * @return a pointer to the element on top. */
    Stackable *StackPop(){
        Stackable *p = llhRoot.Next();
        if (p != NULL){
            llhSize--;
            llhRoot.next = p->next;
            p->next = NULL;
        }
        return p;
    }

    /** @brief Get the depth of the stack.
      * @return the number of elements in the stack. */
    uint32 StackDepth(){
        return ListSize();
    }

    /** @brief Looks into the stack: index = 0 is the top.
      * @param index is the position of the requested element.
      * @return a pointer to the element at index position. */
    Stackable *StackPeek(uint32 index){
        return ListPeek(index);
    }

    /** @brief Get the top of the stack.
      * @return a pointer to the root of the list. */
    Stackable *StackTop(){
        return List();
    }

    /** @brief Removes and gets the n-th element of stack.
      * @param index is the position of the requested element.
      * @return a pointer to the element at index position. */
    Stackable *StackExtract(uint32 index){
        return ListExtract(index);
    }

    /** @brief Inserts a list as the n-th element of stack (if possible, otherwise as bottom).
        @param q is the pointer to LinkedListable list to insert.
        @param index is the position. */
    void StackInsert(Stackable *q,uint32 index){
        ListInsert(q,index);
    }

};
#endif
