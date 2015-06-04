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
 * @brief A template to customise a StaticListHolder for a specific struct or class
 */
#ifndef STATIC_LIST_TEMPLATE_H
#define STATIC_LIST_TEMPLATE_H

#include "StaticListHolder.h"

/** @brief This class implements a generic template list.
 *
 * The main difference with StaticListHolder is that user should not insert manually sizeof(element)/sizeof(intptr*) because it is automatically
 * calculated by the size of the template object.  
 *
 * For each element, the add function makes a copy and stores it in the list. Then there are many functions to perform
 * the main important features of a list: extract, peek, find, ecc.   
 */

template<class T>

/** @brief a static list template class */
class StaticListTemplate: protected StaticListHolder {

public:

    /** @brief The constructor sets automatically the object size.
     * @param msecTimeout is the timeout for the internal semaphore. */
    StaticListTemplate(TimeoutType msecTimeout = TTInfiniteWait) {
        int32 templateSize = sizeof(T);
        int32 containerSize = sizeof(intptr);
        int32 factorSize = templateSize / containerSize;
        this->elementSize = factorSize;
        if (factorSize * containerSize < templateSize)
            this->elementSize++;
        this->msecTimeout = msecTimeout;
    }

    /** @brief Add an element at any position.
     * @param element is the data. The data will be copied into the list.
     * @param position is the position where data must be added (0 = add on top, -1 = add at the end).
     * @return false if the semaphore lock fails or something goes wrong with memory (re)allocation. */
    inline bool ListAdd(const T &element, int position = SLH_EndOfList) {
        return StaticListHolder::ListAdd((const intptr *) &element, position);
    }

    /** @brief Removes an element from any position.
     * @param element is the data in return.
     * @param position is the index of the requested element (0 = removes from the top, -1 = removes from the end), true otherwise.
     * @return false if the semaphore lock fails or if the element is not in the list, true otherwise. */
    inline bool ListExtract(T &element, int position = SLH_EndOfList) {
        return StaticListHolder::ListExtract((intptr *) &element, position);
    }

    /** @brief Reads a value without affecting the list.
     * @param element is the data in return.
     * @param position is the index of the requested element.
     * @return false if the semaphore lock fails, or if the position is out of bounds, true otherwise.   */
    inline bool ListPeek(T &element, int position = SLH_EndOfList) {
        return StaticListHolder::ListPeek((intptr *) &element, position);
    }

    /** @brief Removes an element from the list using a copy of the element as a search key.
     * @param element contains the value of the element to remove.
     * @return false if the semaphore lock fails or if the element is not in the list, true otherwise. */
    inline bool ListDelete(const T &element) {
        return StaticListHolder::ListDelete((const intptr *) &element);
    }

    /** @brief Finds at what index the specified data is located. 
     * @param element contains the value of the element to find. 
     * @return the position of the requested element or -1 if data is not found or in case of errors. */
    inline int ListFind(const T &element) {
        return StaticListHolder::ListFind((const intptr *) &element);
    }

    /** @brief Add a the top.
     * @param element is the element to insert in the list.
     * @return false if semaphore lock fails or if somethind goes wrong with memory (re)allocation, true otherwise. */
    inline bool ListInsert(const T &element) {
        return StaticListHolder::ListAdd((const intptr *) &element,
                                         SLH_StartOfList);
    }

    /** @brief Removes at the specified position.
     * @param position is the index of the element to delete.
     * @return true if the position is out of bounds or the semaphore lock fails, true otherwise. */
    inline bool ListDelete(int position) {
        return (StaticListHolder::ListExtract(NULL, position));
    }

    /** @brief Get the number of elements in the list.
     * @return the number of elements in the list. */
    uint32 ListSize() const {
        return StaticListHolder::ListSize();
    }

};

#endif

