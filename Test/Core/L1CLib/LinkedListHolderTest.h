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
 * @file LinkedListHolderTest.h
 * @brief Tests the LinkListHolder and Iterators associated functions.
 *
 * The test consists in adding, remove and sort elements in a list specified by LinkedListHolder class.
 */

#ifndef LINKEDLISTHOLDERTEST_H_
#define LINKEDLISTHOLDERTEST_H_

#include "LinkedListHolder.h"
#include "Iterators.h"

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

/** @brief Iterator which sum a constant to the list element. */
class SumIterator: public Iterator {
private:
    int32 toSum;

public:

    SumIterator(int32 number) {
        toSum = number;
    }
    void Do(LinkedListable *data) {
        IntegerList* dataInt = (IntegerList*) data;
        dataInt->intNumber += toSum;
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

/** @brief Create a searcher to find integers in the list. **/
class SearchGreatInteger: public SearchFilter {
private:
    uint32 searchIntNumber;

public:

    SearchGreatInteger(uint32 intNum) {
        searchIntNumber = intNum;
    }

    void ChangeSearchNumber(uint32 intNum) {
        searchIntNumber = intNum;
    }

    bool Test(LinkedListable *data) {
        return ((IntegerList*) (data))->intNumber > searchIntNumber;
    }
};

/** @brief Class used for tests on LinkedListHolder functions. **/
class LinkedListHolderTest {
public:

    LinkedListHolderTest() {

    }
    virtual ~LinkedListHolderTest() {

    }

    /**
     * @brief Inserts integers and lists of integers in different ways and sorts the final list in a decrescent order. 
     * @return true if all operations works correctly and the final list is sorted. **/
    bool TestInsertAndSorting();

    /** 
     * @brief Tests the add, extract, peek and delete functions. 
     * @return true if all operations works correctly. **/
    bool TestAddRemoveAndSearch();

    /** 
     * @brief Tests the behavior of many functions with NULL passed by argument.
     * @return true if all functions returns as expected (and without segmentation faults). **/
    bool TestNULLConditions();
};

#endif 

