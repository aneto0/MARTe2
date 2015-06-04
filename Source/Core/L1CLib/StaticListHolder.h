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
 * @brief A list of elements that can grow and shrink
 */

#ifndef STATIC_LIST_HOLDER_H
#define STATIC_LIST_HOLDER_H

#include "GeneralDefinitions.h"
#include "Memory.h"
#include "TimeoutType.h"
#include "FastPollingMutexSem.h"

//#include "ErrorManagement.h"
/** @brief The list will grow at the pace of 64 elements */
static const int SLH_Granularity = 64;

/** @brief Use this to insert at beginning of list */
static const int SLH_StartOfList = 0;

/** @brief Use this to add at the end of list */
static const int SLH_EndOfList = -1;

///** use this to specify not to use the semaphore */
//static const int SLHT_Unprotected = -2;

///** use this to specify not to wait */
//static const int SLHT_NoWait      = 0;

///** use this to specify not to use the semaphore */
//static const int SLHT_WaitForEver = SEM_INDEFINITE_WAIT;

/** @brief This object is a container of pointers that can grow or shrink.
 *
 * This class is foundamental an array where pieces of SLH_Granularity size of memory were dynamically allocated
 * due to add new elements in the list. 
 *
 * For each element, the add function makes a copy and stores it in the list. Then there are many functions to perform
 * the main important features of a list: extract, peek, find, ecc.
 *
 * It's important highlight that elements must have a dimension multiple of (intptr*) size which is a pointer to a long
 * long integer. This size depends from the system (32 or 64 bits). 
 */



class StaticListHolder {
protected:
    /** the vector of elements */
    intptr * elements;

    /** the size of one element as number */
    int elementSize;

    /** how many elements are in the list*/
    int numberOfElements;

    /** A semaphore to allow multiple threads protected access to the data. */
    FastPollingMutexSem mux;

    /** how much to wait for a resource in milliseconds */
    TimeoutType msecTimeout;

    /** @brief Increases the list size by 1. Manages the reallocation of memory when necessary.
     * @return false if the memory (re)allocation fails, true otherwise.
     * If the number of elements is multiple than granularity a new space for 'granularity' elements is added. */
    bool IncreaseListSize() {
        if ((numberOfElements % SLH_Granularity) == 0) {
            int newSize = numberOfElements + SLH_Granularity;
            if (elements == NULL) {
                elements = (intptr *) MemoryMalloc(
                        sizeof(intptr *) * newSize * elementSize);
            }
            else {
                elements = (intptr *) MemoryRealloc(
                        (void *&) elements,
                        sizeof(intptr *) * newSize * elementSize);
            }
            if (elements == NULL) {
                numberOfElements = 0;
//                CStaticAssertErrorCondition(OSError,"StaticListHolder::IncreaseListSize:malloc(%i bytes) failed",newSize * sizeof(intptr *) * elementSize);
                return False;
            }
        }
        numberOfElements++;
        return True;
    }

    /** @brief Decreases the list size by 1. Reallocation of memory is not performed. */
    bool DecreaseListSize() {
        numberOfElements--;
        return True;
    }

    /** @brief Copies data from a buffer to the position.
     * @param destination is the destination pointer.
     * @param source is the pointer to the memory which must be copied.  */
    inline void Copy(intptr *destination, const intptr *source) {
        for (int j = 0; j < elementSize; j++) {
            destination[j] = source[j];
        }
    }

    /** @brief Compares data between source and destination. 
     * @param destination is a pointer to the first value.
     * @param source is a pointer to the second value.
     * @return true if the values are equal. */
    inline bool Compare(const intptr *destination, const intptr *source) {
        for (int j = 0; j < elementSize; j++) {
            if (destination[j] != source[j])
                return False;
        }
        return True;
    }

    /** @brief Finds data in list.
     * @param data is a pointer to the data to find.
     * @return the index of the data found in the list, -1 if it is not found. */
    inline int Find(const intptr *data) {
        int index = 0;
        while (!Compare(GetPointer(index), data) && (index < numberOfElements))
            index++;
        if (index == numberOfElements)
            return -1;
        return index;
    }

    /** @brief Get the address of data element.
     * @param position is the index of the element in the list.
     * @return a pointer to the element in 'position'. */
    inline intptr* GetPointer(int position) {
        return &elements[position * elementSize];
    }

    /** @brief Moves all the pointers from position to the right. Assumes that the last position is empty.
     * @param position is the minor index. */
    inline void RightShiftListFrom(int position) {
        for (int i = (numberOfElements - 1); i > position; i--) {
            Copy(GetPointer(i), GetPointer(i - 1));
        }
    }

    /** @brief Removes the element in position and shifts all the elements at the right of it to the left.
     * @param position is the minor index. */
    inline void LeftShiftListTo(int position) {
        for (int i = position; i < (numberOfElements - 1); i++) {
            Copy(GetPointer(i), GetPointer(i + 1));
        }
    }

public:
    /** @brief Create a List with the given element size is multiple of sizeof(intptr*).
     * @param elementSize is the factor sizeof(element)/sizeof(intptr*).  */
    StaticListHolder(int elementSize = 1) {
        elements = NULL;
        numberOfElements = 0;
        msecTimeout = TTUnProtected;
        this->elementSize = elementSize;
        mux.Create();
    }

    /** @brief Virtual destructor . This means that all descendants will be virtual. */
    virtual ~StaticListHolder() {
        if (elements != NULL)
            MemoryFree((void *&) elements);
        numberOfElements = 0;
    }

    /** @brief Set access timeout. */
    void SetAccessTimeout(TimeoutType msecTimeout = TTInfiniteWait) {
        this->msecTimeout = msecTimeout;
    }

    /** @brief Get the number of elements in the list.
     * @return the number of elements in the list. */
    uint32 ListSize() const {
        return numberOfElements;
    }

    /** @brief Add an element at any position. 
     * @param element is a pointer to a buffer of data. The data will be copied into the list.
     * @param position could be 0 = add on top, -1 = add at the end.
     * @return false if the semaphore lock fails or if something fails in the memory (re)allocation, true otherwise. */
    bool ListAdd(const intptr *element, int position = SLH_EndOfList) {
        if (msecTimeout != TTUnProtected) {
            if (!mux.FastLock(msecTimeout)) {
                //               CStaticAssertErrorCondition(Timeout,"StaticListHolder::ListAdd:access Timeout( %i ) ",msecTimeout.msecTimeout);
                return False;
            }
        }

        if (position == SLH_EndOfList)
            position = numberOfElements;

        if ((position > numberOfElements) || (position < 0)) {
//            CStaticAssertErrorCondition(ParametersError,"StaticListHolder::ListAdd:poistion outside range: %i [0 %i]",position,numberOfElements);
            if (msecTimeout != TTUnProtected)
                mux.FastUnLock();
            return False;
        }

        if (!IncreaseListSize()) {
//            CStaticAssertErrorCondition(OSError,"StaticListHolder::ListAdd:Failed allocating memory");
            if (msecTimeout != TTUnProtected)
                mux.FastUnLock();
            return False;
        }

        RightShiftListFrom(position);

        Copy(GetPointer(position), element);

        if (msecTimeout != TTUnProtected)
            mux.FastUnLock();

        return True;
    }

    /** @brief Removes an element by its position. 
     * @param element is a pointer to the data to get. 
     * @param position is the position of the requested element (0 = removes from the top, -1 = removes from the end).
     * @return false if semaphore lock fails or if the element is not in the list, true otherwise. */
    bool ListExtract(intptr *element = NULL, int position = SLH_EndOfList) {
        if (msecTimeout != TTUnProtected) {
            if (!mux.FastLock(msecTimeout)) {
//                CStaticAssertErrorCondition(Timeout,"StaticListHolder::ListExtract:access Timeout( %i ) ",msecTimeout.msecTimeout);
                return False;
            }
        }

        if (position == SLH_EndOfList)
            position = numberOfElements - 1;
        if ((position >= numberOfElements) || (position < 0)) {
            //           CStaticAssertErrorCondition(FatalError,"StaticListHolder::ListExtract:poistion outside range: %i [0 %i)",position,numberOfElements);
            if (msecTimeout != TTUnProtected)
                mux.FastUnLock();
            return False;
        }

        bool ret = True;
        if (element)
            Copy(element, GetPointer(position));
        LeftShiftListTo(position);

        if (!DecreaseListSize()) {
//            CStaticAssertErrorCondition(OSError,"StaticListHolder::ListExtract:Failed freeing memory");
            ret = False;
        }

        if (msecTimeout != TTUnProtected)
            mux.FastUnLock();
        return ret;
    }

    /** @brief Reads a value without affecting the list.
     * @param element is a pointer to the data to read.
     * @param position is the position of the requested element.
     * @return false if semaphore lock fails or if the position is out of bounds, true otherwise. */
    bool ListPeek(intptr *element = NULL, int position = SLH_EndOfList) {
        if (msecTimeout != TTUnProtected) {
            if (!mux.FastLock(msecTimeout)) {
//                CStaticAssertErrorCondition(Timeout,"StaticListHolder::ListPeek:access Timeout( %i ) ",msecTimeout.msecTimeout);
                return False;
            }
        }

        if (position == SLH_EndOfList)
            position = numberOfElements - 1;

        bool ret = False;
        if ((position > numberOfElements) || (position < 0)) {
//            CStaticAssertErrorCondition(ParametersError,"StaticListHolder::ListPeek:poistion outside range: %i [0 %i)",position,numberOfElements);
        }
        else {
            ret = True;
            if (element)
                Copy(element, GetPointer(position));
        }

        if (msecTimeout != TTUnProtected)
            mux.FastUnLock();
        return ret;
    }

    /** @brief Removes an element from the list using a copy of the element as a search key.
     * @param element is a pointer to a buffer of data with the same value of the element to delete.
     * @return false if semaphore lock fails or if the element is not in the list, true otherwise. */
    bool ListDelete(const intptr *element) {
        if (msecTimeout != TTUnProtected) {
            if (!mux.FastLock(msecTimeout)) {
//                CStaticAssertErrorCondition(Timeout,"StaticListHolder::ListDelete:access Timeout( %i ) ",msecTimeout.msecTimeout);
                return False;
            }
        }

        int position = Find(element);

        bool ret = False;
        if (position >= 0) {
            ret = True;
            LeftShiftListTo(position);

            if (!DecreaseListSize()) {
//                CStaticAssertErrorCondition(OSError,"StaticListHolder::ListDelete:Failed freeing memory");
                ret = False;
            }
        }

        if (msecTimeout != TTUnProtected)
            mux.FastUnLock();
        return ret;
    }

    /** @brief Finds at what index the specified data is located. 
     * @param element is a pointer to an element with the same value of the element to search.
     * @return the position of the requested element or -1 in case of it is not found or the semaphore lock fails. */
    int ListFind(const intptr *element) {
        if (msecTimeout != TTUnProtected) {
            if (!mux.FastLock(msecTimeout)) {
//                CStaticAssertErrorCondition(Timeout,"StaticListHolder::ListFind:access Timeout( %i ) ",msecTimeout.msecTimeout);
                return -1;
            }
        }

        int position = Find(element);

        if (msecTimeout != TTUnProtected)
            mux.FastUnLock();
        return position;
    }

    /** @brief Add a the top of the list. */
    inline void ListInsert(intptr *element) {
        ListAdd(element, 0);
    }

    /** @brief Removes at the specified position.
     * @param position is the index of the element to be removed.
     * @return false if semaphore lock fails or if the position is out of bounds, true otherwise. */
    inline bool ListDelete(int position) {
        return (ListExtract(NULL, position));
    }

};

#endif

