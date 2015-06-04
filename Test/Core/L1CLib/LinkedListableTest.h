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
 * $Id:  $
 *
 */
/**
 * @file LinkedListableTest.h
 * @brief Tests the LinkListable and Iterators associated functions.
 *
 * The test consists in adding, remove and sort elements in a list specified by LinkedListable class.
 */

#ifndef LINKEDLISTABLETEST_H_
#define LINKEDLISTABLETEST_H_

#include "LinkedListable.h"
#include "Iterators.h"
#include "Memory.h"


/** @brief Create a list of integers**/
class IntegerList: public LinkedListable {
public:
    uint32 intNumber;
public:
    IntegerList() {
        intNumber = 0;
    }

    ~IntegerList() {

    }
};


/** @brief Create a sorter which sorts integers in a decrescent order. **/
class SortDecrescent: public SortFilter {
public:
    SortDecrescent() {
    }
    int32 Compare(LinkedListable* element1, LinkedListable* element2) {
        if (((IntegerList*) element1)->intNumber
                < ((IntegerList*) element2)->intNumber)
            return 1;
        else
            return -1;
    }
};

/** @brief Create a searcher to find integer in the list. **/
class SearchInteger: public SearchFilter {
private:
    uint32 searchIntNumber;

public:

    SearchInteger(uint32 intNum) {
        searchIntNumber = intNum;
    }

    void ChangeSearchNumber(uint32 intNum) {
        searchIntNumber = intNum;
    }

    bool Test(LinkedListable *data) {
        return ((IntegerList*) (data))->intNumber == searchIntNumber;
    }
};


/** @brief Class used for tests on LinkedListable functions. **/
class LinkedListableTest {
public:

 


    /**
     * @brief Inserts integers and sorts them in decrescent order. Then another list is created and it is merged with the first list with the sorted insert. 
     * @return true if all operations works correctly and the final list is sorted. **/
    bool TestInsertAndSorting();

    /** @brief Tests the add, extract and peek functions. 
      * @return true if all operations works correctly. **/
    bool TestAddAndRemove();
};

#endif /* LINKEDLISTABLETEST_H_ */

