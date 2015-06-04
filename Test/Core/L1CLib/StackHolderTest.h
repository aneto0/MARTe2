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
 * @file StackHolderTest.h
 * @brief Tests the StackHolder and Iterators associated functions.
 *
 * The test consists in adding, remove and sort elements in a stack-list checking that operations are doing in LIFO mode.
 */

#ifndef STACKHOLDERTEST_H_
#define STACKHOLDERTEST_H_

#include "StackHolder.h"
#include "Iterators.h"


/** @brief Create a list of integers**/
class IntegerLinkedList: public LinkedListable {
public:
    uint32 intNumber;
public:
    IntegerLinkedList() {
        intNumber = 0;
    }

    ~IntegerLinkedList() {

    }
};

/** @brief Class used for tests on StackHolder functions. **/
class StackHolderTest {
public:

    StackHolderTest() {

    }
    virtual ~StackHolderTest() {

    }

    /**
     * @brief Tests many operations of the StackHolder class. It's also tested the working of the iterators functions with Fn suffix. 
     * @return true if all operations works correctly and the final list is sorted. **/
    bool TestAll();

    /**
     * @brief Tests adding, peeking and removing functions with NULL or index out of bounds as argument.
     * @return true if functions return as expected. **/
    bool TestNULLConditions();
};

#endif 
