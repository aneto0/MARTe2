/**
 * @file ListTestHelper.h
 * @brief Header file for class ListTestHelper
 * @date 06/06/2015
 * @author Giuseppe Ferrò
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class ListTestHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LISTTESTHELPER_H_
#define LISTTESTHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Iterator.h"
#include "SearchFilter.h"
#include "SortFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief An list of integers used in tests
 */
class IntegerList: public LinkedListable {
public:
    /**
     * The element of the list
     */
    uint32 intNumber;
public:
    /**
     * @brief Default constructor.
     */
    IntegerList() {
        intNumber = 0;
    }

    /**
     * @brief Initializes the list element.
     * @param[in] number is the number attribute.
     */
    IntegerList(uint32 number) {
        intNumber = number;
    }

    /**
     * @brief Destructor
     */
    ~IntegerList() {

    }
};

/**
 * @brief A sort filter which sorts integers in decrescent order.
 */
class SortDecrescent: public SortFilter {
public:

    /**
     * @brief Constructor
     */
    SortDecrescent() {
    }

    /**
     * @brief The compare function.
     * @param[in] element1 is the first list element to be compared.
     * @param[in] element2 is the second list element to be compared.
     * @return 1 if the first element is minor than the second, -1 otherwise.
     */
    int32 Compare(LinkedListable* element1,
                  LinkedListable* element2) {
        if (((IntegerList*) element1)->intNumber < ((IntegerList*) element2)->intNumber)
            return 1;
        else
            return -1;
    }
};


/**
 * @brief A search filter of a specified integer element.
 */
class SearchInteger: public SearchFilter {
private:

    /**
     * The number to be searched in the list
     */
    uint32 searchIntNumber;

public:

    /**
     * @brief Constructor by element to be searched.
     */
    SearchInteger(uint32 intNum) {
        searchIntNumber = intNum;
    }

    /**
     * @brief A function used to change the number to be searched.
     */
    void ChangeSearchNumber(uint32 intNum) {
        searchIntNumber = intNum;
    }

    /**
     * @brief Test function.
     * @param[in] data is the element to be tested.
     * @return true if data is equal to the number to be searched, false otherwise.
     */
    bool Test(const LinkedListable *data) {
        return ((IntegerList*) (data))->intNumber == searchIntNumber;
    }
};


/**
 * @brief An Iterator which increment each integer number by one.
 */
class IncrementIterator: public Iterator {
private:

public:

    /**
     * @brief Do function.
     * @param[in] data is the element to be incremented.
     */
    void Do(LinkedListable *data) {
        ((IntegerList*) data)->intNumber++;
    }

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LISTTESTHELPER_H_ */

