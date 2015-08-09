/**
 * @file ListTestHelper.h
 * @brief Header file for class ListTestHelper
 * @date 06/ago/2015
 * @author pc
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

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

extern uint32 nToSearch;

class IntegerList: public LinkedListable {
public:
    uint32 intNumber;
public:
    IntegerList() {
        intNumber = 0;
    }

    IntegerList(uint32 number) {
        intNumber = number;
    }

    ~IntegerList() {

    }
};

class SortDecrescent: public SortFilter {
public:
    SortDecrescent() {
    }
    int32 Compare(LinkedListable* element1,
                  LinkedListable* element2) {
        if (((IntegerList*) element1)->intNumber < ((IntegerList*) element2)->intNumber)
            return 1;
        else
            return -1;
    }
};

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

class IncrementIterator: public Iterator {
private:

public:

    void Do(LinkedListable *data) {
        ((IntegerList*) data)->intNumber++;
    }

};

static int32 DecrescentSortFn(LinkedListable *data1,
                              LinkedListable *data2) {
    if ((((IntegerList *) data1)->intNumber) < (((IntegerList *) data2)->intNumber))
        return 1;
    else
        return -1;
}

static bool SearchIntFn(LinkedListable* data) {
    return ((IntegerList*) (data))->intNumber == nToSearch;

}

static void IncrementNumFn(LinkedListable *data) {
    ((IntegerList *) data)->intNumber++;
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LISTTESTHELPER_H_ */

