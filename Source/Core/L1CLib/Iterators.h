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
 * @brief Definition of iterator/filters classes
 */

#ifndef ITERATORS_H
#define ITERATORS_H

/** @brief Here there are prototipes for Iterator, SearchFilter and SortFilter objects.
  *
  * All member functions are virtual to allow to the user to implement the desired actions.
  *
  * These objects are very useful in for LinkedListableList objects (@see LinkedListableList.h)
  * where passing them to the list functions it's possible sort, search and manipulate
  * elements. */ 

#include "GeneralDefinitions.h"

/** the return codes when a Test2 is called in a SearchFilterT or
 when Do2 is called in IteratorT */
enum SFTestType {

    /** No meaning associated */
    SFTTNull = 0x0000,

    /** This is exactly what I was looking for! */
    SFTTFound = 0x1001,

    /** This is not what I was looking for */
    SFTTNotFound = 0x1000,

    /** When recursive search where a path has
     been specified is used This is not the right path */
    SFTTWrongPath = 0x1002,

    /** When recursing this is to notify of a node beginning of
     recursion (used only for Do2) */
    SFTTRecurse = 0x2000,

    /** When recursing this is to notify of the return
     to the previous level used also for Do2 */
    SFTTBack = 0x2001
};
// Iterators support

class LinkedListable;

/** @brief A class to build an iterator filter. */
class Iterator {
public:
    /** @brief The function performing the action linked to the iterator.
      * @param data is a pointer to a LinkedListable object. */
    virtual void Do(LinkedListable *data)=0;
};

/** @brief A class template to build an iterator filter. */
template<typename T>
class IteratorT {
public:
    /** @brief The function performing the action linked to the iterator.
      * @param data is a generic template. */
    virtual void Do(T data)=0;

    /** @brief A more specialised form of the Do function to be used on certain applications.
      * @param data is a generic template.
      * @param mode is a flag for special operatios. */
    virtual void Do2(T data, SFTestType mode = SFTTNull) {
        Do(data);
    }
};

/** @brief The type of a function to be iterated on a set.
  * @param data is a LinkedListable object pointer. */
typedef void (IteratorFn)(LinkedListable *data);

/** @brief A class to build search filters. */
class SearchFilter {
public:
    /** @brief The function that performs the search on a set of data.
      * @param data is a LinkedListable object pointer. */
    virtual bool Test(LinkedListable *data)=0;
};

/** @brief A class template to build an iterator filter. */
template<typename T>
class SearchFilterT {
public:
    /** @brief The function performing the action linked to the iterator. 
      * @param data is a generic template. */
    virtual bool Test(T data)=0;

    /** @brief A more specialised form of the Test function to be used on certain applications.
      * @param data is a generic template.
      * @param mode is a flag for special operations. */
    virtual SFTestType Test2(T data, SFTestType mode = SFTTNull) {
        if (Test(data))
            return SFTTFound;
        return SFTTNotFound;
    }
};

/** @brief The type of a function to be used to search on a set.
  * @param data is a LinkedListable object pointer. */
typedef bool (SearchFilterFn)(LinkedListable *data);

/**  @brief A class to build search filters. */
class SortFilter {
public:
    virtual ~SortFilter(){

    }
    /** @brief A function that can be used to compare two object.
      * @param data1 is the first LinkedListable object.
      * @param data2 is the second LinkedListable object.
      * @return a positive value if data1 is not ordered with data2, negative otherwise. */
    virtual int32 Compare(LinkedListable *data1, LinkedListable *data2)=0;
};

/** The type of a function to be used to sort a set.
 @param data1 is the first LinkedListable object pointer.
 @param data2 is the second LinkedListable object pointer.
 */
typedef int32 (SortFilterFn)(LinkedListable *data1, LinkedListable *data2);

/** just a rename of LinkedListable

 STACK  ->######    (Push)
 <-######    (Pop)
 ######
 <-|       (Peek)

 */
typedef LinkedListable Stackable;

/*****************************************************************************/

/** just a rename of LinkedListable

 QUEUE    ######<-  (Add)
 <-######    (Extract)
 */
typedef LinkedListable Queueable;

#endif

