/**
 * @file LinkedListHolderTest.h
 * @brief Header file for class LinkedListHolderTest
 * @date 06/08/2015
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

 * @details This header file contains the declaration of the class LinkedListHolderTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LINKEDLISTHOLDERTEST_H_
#define LINKEDLISTHOLDERTEST_H_
/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "LinkedListHolder.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
class LinkedListHolderTest {

public:
    bool TestConstructor();

    bool TestDestructor();

    bool TestReset(uint32 nElements);

    bool TestCleanup(uint32 nElements);

    bool TestList(uint32 nElements);

    bool TestListSize(uint32 nElements);

    bool TestFastListInsertSingle(uint32 nElements);

    bool TestListInsert();

    bool TestListInsertSortedSorter();

    bool TestListInsertSortedFn();

    bool TestListInsertNullSorter();

    bool TestListInsertIndex(uint32 index, uint32 size);


/*
     bool TestListAdd();

     bool TestListAddL();

     bool TestListSearch();

     bool TestListSearch();

     bool TestListSearch();

     bool TestListExtract();

     bool TestListExtract();

     bool TestListExtract();

     bool TestListDelete();

     bool TestListDelete();

     bool TestListSafeDelete();

     bool TestListDelete();

     bool TestListBSort();

     bool TestListBSort();

     bool TestListPeek();

     bool TestListExtract();

     bool TestListIterate();

     bool TestListIterate();
     */

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LINKEDLISTHOLDERTEST_H_ */

