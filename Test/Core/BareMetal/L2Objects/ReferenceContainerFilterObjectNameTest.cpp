/**
 * @file ReferenceContainerFilterObjectNameTest.cpp
 * @brief Source file for class ReferenceContainerFilterObjectNameTest
 * @date 19/08/2015
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

 * @details This source file contains the definition of all the methods for
 * the class ReferenceContainerFilterObjectNameTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainerFilterObjectNameTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ReferenceContainerFilterObjectNameTest::TestDefaultConstructor() {

    ReferenceContainerFilterObjectName myFilterName;
    return (myFilterName.GetMode() == 0) && (myFilterName.IsFinished());
}

bool ReferenceContainerFilterObjectNameTest::TestFullConstructor(int32 occurrence,
                                                                 CCString toSearch) {

    CCString begin = toSearch;
    if (toSearch[0] == '.') {
        begin++;
    }

    bool invalid = (begin.FindPattern("..") != 0xFFFFFFFFu);
//    bool invalid = StringHelper::SearchString(begin, "..") != NULL;
//    uint32 length = begin.GetSize();
    bool moreThanOneNode = (!invalid) && (begin.FindPattern(".") != 0xFFFFFFFFu);
//    bool moreThanOneNode = (!invalid) && (StringHelper::SearchIndex(begin, ".") < ((int32) (StringHelper::Length(begin) - 1u)));

    ReferenceContainer fakeTree;

    if (invalid) {

        for (uint32 mode = 0u; mode < 8u; mode++) {

            ReferenceContainerFilterObjectName myFilterName = ReferenceContainerFilterObjectName(occurrence, mode, toSearch);

        }
        return true;

    }

    for (uint32 mode = 0u; mode < 8u; mode++) {

        ReferenceContainerFilterObjectName myFilterName = ReferenceContainerFilterObjectName(occurrence, mode, toSearch);

        // this should be impossible
        if ((moreThanOneNode) && (myFilterName.IsSearchAll())) {
            return false;
        }

        // if there are more than two nodes, these two conditions should be valid
        if (moreThanOneNode) {
            if (!myFilterName.IsStorePath()) {
                return false;
            }
            if (!myFilterName.IsRecursive()) {
                return false;
            }
        }

    }

    return true;

}

bool ReferenceContainerFilterObjectNameTest::TestCopyConstructor(int32 occurrence,
                                                                 CCString toSearch) {

    for (uint32 mode = 0; mode < 8; mode++) {
        // creates a filter
        ReferenceContainerFilterObjectName *myFilterName = new ReferenceContainerFilterObjectName(occurrence, mode, toSearch);

        Reference testRef = Reference("Object");
        testRef->SetName(toSearch);

        ReferenceContainer fakeTree;

        int32 cycles = (occurrence >= 0) ? occurrence : 500;

        for (int32 i = 0; i < cycles; i++) {
            myFilterName->Test(fakeTree, testRef);
        }

        if (occurrence >= 0) {
            if (!myFilterName->IsFinished()) {
                return false;
            }
        }
        //copy constructor
        ReferenceContainerFilterObjectName copy(*myFilterName);

        //reset after copy
        if (copy.IsFinished()) {
            return false;
        }

        for (int32 i = 0; i < cycles; i++) {
            copy.Test(fakeTree, testRef);
        }
        if (occurrence >= 0) {
            if (!copy.IsFinished()) {
                return false;
            }
        }

        if (myFilterName->GetMode() != copy.GetMode()) {
            return false;
        }

        delete myFilterName;

        copy.Reset();

        //the address is still valid
        if (!copy.Test(fakeTree, testRef)) {
            return false;
        }
    }
    return true;
}


bool ReferenceContainerFilterObjectNameTest::TestTest(ReferenceContainer &previouslyFound,
                                                      CCString toSearch,
                                                      bool expected) {


    if (toSearch[0] == '.') {
        toSearch++;
    }

    uint32 length = toSearch.GetSize();
    uint32 addressNumberNodes = 1;
    CCString lastNode;
    for (uint32 i = 0; i < length; i++) {
        if (toSearch[i] == '.') {
            //increment the number of nodes where a '.' is found
            addressNumberNodes++;
            lastNode = CCString(toSearch.GetList()+i);
        }
    }

    //const char8 *lastNode = StringHelper::SearchLastChar(toSearch, '.');
    //if (lastNode == NULL) {
    if (lastNode.GetSize() == 0){
        lastNode = toSearch;
    }
    else {
//        if (lastNode[0] != '\0') {
    		// skip the .
            lastNode++;
//        }
//        else {
//            return false;
//        }
    }

    Reference toBeSearched("Object");
    toBeSearched->SetName(lastNode);

    for (uint32 mode = 0u; mode < 8u; mode++) {

        ReferenceContainerFilterObjectName filter(1, mode, toSearch);

        if (filter.Test(previouslyFound, toBeSearched) != expected) {
            return false;
        }
    }

    return true;
}

bool ReferenceContainerFilterObjectNameTest::TestAssignOperator(int32 occurrence,CCString toSearch) {
    for (uint32 mode = 0; mode < 8; mode++) {
        // creates a filter
        ReferenceContainerFilterObjectName *myFilterName = new ReferenceContainerFilterObjectName(occurrence, mode, toSearch);

        Reference testRef = Reference("Object");
        testRef->SetName(toSearch);

        ReferenceContainer fakeTree;

        int32 cycles = (occurrence >= 0) ? occurrence : 500;

        for (int32 i = 0; i < cycles; i++) {
            myFilterName->Test(fakeTree, testRef);
        }

        if (occurrence >= 0) {
            if (!myFilterName->IsFinished()) {
                return false;
            }
        }
        //copy
        ReferenceContainerFilterObjectName copy(1, 0, "Hello");
        copy = (*myFilterName);

        //reset after copy
        if (copy.IsFinished()) {
            return false;
        }

        for (int32 i = 0; i < cycles; i++) {
            copy.Test(fakeTree, testRef);
        }
        if (occurrence >= 0) {
            if (!copy.IsFinished()) {
                return false;
            }
        }

        if (myFilterName->GetMode() != copy.GetMode()) {
            return false;
        }

        delete myFilterName;

        copy.Reset();

        //the address is still valid
        if (!copy.Test(fakeTree, testRef)) {
            return false;
        }
    }

    return true;
}
