/**
 * @file StreamStructuredDataTest.h
 * @brief Header file for class StreamStructuredDataTest
 * @date 07/09/2018
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class StreamStructuredDataTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SENDERSTRUCTUREDDATATEST_H_
#define SENDERSTRUCTUREDDATATEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "JsonPrinter.h"
#include "Matrix.h"
#include "StreamStructuredData.h"
#include "StandardPrinter.h"
#include "XMLPrinter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief A structure used to test the StreamStructuredData methods
 */
struct StreamStructuredDataTestWriteStruct {
    AnyType *toWrite;
    const char8 ** varnames;
    StreamString desResult;
};

/**
 * @brief A structure used to test the StreamStructuredData methods
 */
struct StreamStructuredDataTestAddToCurrentNodeStruct {
    Reference toAdd;
    const char8 ** movements;
    bool *expected;
    StreamString desResult;
    uint32 nMoveUps;
    uint32 *index;
};


/**
 * @brief Tests the StreamStructuredData methods
 */
template<class Printer>
class StreamStructuredDataTest {
public:

    /**
     * @brief Constructor
     */
    StreamStructuredDataTest();

    /**
     * @brief Destructor
     */
    virtual ~StreamStructuredDataTest();

    /**
     * @brief Tests the Read method
     */
    bool TestRead();

    /**
     * @brief Tests the GetType method
     */
    bool TestGetType();

    /**
     * @brief Tests the Write method
     */
    bool TestWrite(StreamStructuredDataTestWriteStruct *table);

    /**
     * @brief Tests the Copy method
     */
    bool TestCopy();

    /**
     * @brief Tests the AddToCurrentNode method
     */
    bool TestAddToCurrentNode(StreamStructuredDataTestAddToCurrentNodeStruct *table);

    /**
     * @brief Tests the MoveToRoot method
     */
    bool TestMoveToRoot(StreamStructuredDataTestAddToCurrentNodeStruct *table);

    /**
     * @brief Tests the MoveToAncestor method
     */
    bool TestMoveToAncestor(StreamStructuredDataTestAddToCurrentNodeStruct *table);

    /**
     * @brief Tests the MoveToAncestor against the bug report on the Story 450
     */
    bool TestMoveToAncestorBug450();

    /**
     * @brief Tests the MoveAbsolute method
     */
    bool TestMoveAbsolute(StreamStructuredDataTestAddToCurrentNodeStruct *table);

    /**
     * @brief Tests the MoveRelative method
     */
    bool TestMoveRelative(StreamStructuredDataTestAddToCurrentNodeStruct *table);

    /**
     * @brief Tests the MoveToChild method
     */
    bool TestMoveToChild(StreamStructuredDataTestAddToCurrentNodeStruct *table);

    /**
     * @brief Tests the CreateAbsolute method
     */
    bool TestCreateAbsolute(StreamStructuredDataTestAddToCurrentNodeStruct *table);

    /**
     * @brief Tests the CreateRelative method
     */
    bool TestCreateRelative(StreamStructuredDataTestAddToCurrentNodeStruct *table);

    /**
     * @brief Tests the Delete method
     */
    bool TestDelete();

    /**
     * @brief Tests the GetName method
     */
    bool TestGetName(StreamStructuredDataTestAddToCurrentNodeStruct *table);

    /**
     * @brief Tests the GetChildName method
     */
    bool TestGetChildName(StreamStructuredDataTestAddToCurrentNodeStruct *table);

    /**
     * @brief Tests the GetNumberOfChildren method
     */
    bool TestGetNumberOfChildren(StreamStructuredDataTestAddToCurrentNodeStruct *table);

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<class Printer>
StreamStructuredDataTest<Printer>::StreamStructuredDataTest() {
}

template<class Printer>
StreamStructuredDataTest<Printer>::~StreamStructuredDataTest() {
}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestRead() {
    StreamString string;
    StreamStructuredData<StandardPrinter> test(string);

    uint32 x;
    return !test.Read("boh", x);
}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestGetType() {
    StreamString string;
    StreamStructuredData<StandardPrinter> test(string);

    return test.GetType("boh").IsVoid();

}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestWrite(StreamStructuredDataTestWriteStruct *table) {
    StreamString string;
    StreamStructuredData<Printer> test(string);

    bool ret = true;
    uint32 i = 0u;
    while ((!(table->toWrite[i].IsVoid())) && (ret)) {
        ret = test.Write(table->varnames[i], table->toWrite[i]);
        i++;
    }

    if (ret) {
        ret = string == table->desResult;
        printf("|%s||%s| %lld %lld\n", string.Buffer(), table->desResult.Buffer(), string.Size(), table->desResult.Size());

    }

    return ret;

}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestCopy() {
    StreamString string;
    StreamStructuredData<StandardPrinter> test(string);
    StreamStructuredData<StandardPrinter> copy(string);

    //copy not supported
    return !test.Copy(copy);
}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestAddToCurrentNode(StreamStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    StreamStructuredData<Printer> test(string);

    bool ret = test.AddToCurrentNode(table->toAdd);

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(test.MoveAbsolute(table->movements[i])==table->expected[i]);
        i++;
    }

    if (ret) {
        ret = (string == table->desResult);
    }
    printf("|%s||%s|\n", string.Buffer(), table->desResult.Buffer());

    return ret;
}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestMoveToRoot(StreamStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    StreamStructuredData<Printer> test(string);

    bool ret = test.AddToCurrentNode(table->toAdd);

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(test.MoveAbsolute(table->movements[i])==table->expected[i]);
        i++;
    }

    if (ret) {
        ret = test.MoveToRoot();
    }

    if (ret) {
        ret = (string == table->desResult);
    }
    printf("|%s||%s|\n", string.Buffer(), table->desResult.Buffer());

    return ret;
}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestMoveToAncestor(StreamStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    StreamStructuredData<Printer> test(string);

    bool ret = test.AddToCurrentNode(table->toAdd);

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(test.MoveAbsolute(table->movements[i])==table->expected[i]);
        i++;
    }

    if (ret) {
        ret = test.MoveToAncestor(table->nMoveUps);
    }

    if (ret) {
        ret = (string == table->desResult);
    }
    printf("|%s||%s|\n", string.Buffer(), table->desResult.Buffer());

    return ret;
}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestMoveToAncestorBug450() {
    StreamString string;
    StreamStructuredData<Printer> test(string);
    bool ret = test.CreateAbsolute("A.B.C");
    ret &= test.MoveToAncestor(1u);
    ret &= test.MoveAbsolute("A");
    ret &= test.CreateRelative("D.E");
    ret &= test.MoveToAncestor(1u);
    ret &= test.CreateRelative("F");
    ret &= test.MoveToAncestor(2u);
    ret &= test.CreateAbsolute("A.G");
    ret &= test.MoveToAncestor(1u);
    ret &= test.CreateRelative("H");

    return ret;
}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestMoveAbsolute(StreamStructuredDataTestAddToCurrentNodeStruct *table) {
    return TestAddToCurrentNode(table);
}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestMoveRelative(StreamStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    StreamStructuredData<Printer> test(string);

    bool ret = test.AddToCurrentNode(table->toAdd);

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(test.MoveRelative(table->movements[i])==table->expected[i]);
        i++;
    }

    if (ret) {
        ret = (string == table->desResult);
    }
    printf("|%s||%s|\n", string.Buffer(), table->desResult.Buffer());

    return ret;
}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestMoveToChild(StreamStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    StreamStructuredData<Printer> test(string);

    bool ret = test.AddToCurrentNode(table->toAdd);

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(test.MoveToChild(table->index[i])==table->expected[i]);
        i++;
    }

    if (ret) {
        ret = (string == table->desResult);
    }
    printf("|%s||%s|\n", string.Buffer(), table->desResult.Buffer());

    return ret;
}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestCreateAbsolute(StreamStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    StreamStructuredData<Printer> test(string);

    bool ret = true;

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(test.CreateAbsolute(table->movements[i])==table->expected[i]);
        i++;
    }

    if (ret) {
        ret = (string == table->desResult);
    }
    printf("|%s||%s|\n", string.Buffer(), table->desResult.Buffer());

    return ret;
}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestCreateRelative(StreamStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    StreamStructuredData<Printer> test(string);

    bool ret = true;

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(test.CreateRelative(table->movements[i])==table->expected[i]);
        i++;
    }

    if (ret) {
        ret = (string == table->desResult);
    }
    printf("|%s||%s|\n", string.Buffer(), table->desResult.Buffer());

    return ret;
}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestDelete() {
    StreamString string;
    StreamStructuredData<Printer> test(string);

    return !test.Delete("boh");
}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestGetName(StreamStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    StreamStructuredData<Printer> test(string);

    bool ret = test.AddToCurrentNode(table->toAdd);

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(test.MoveToChild(table->index[i])==table->expected[i]);
        if(ret) {
            ret=(StringHelper::Compare(test.GetName(),table->movements[i])==0);
        }
        i++;
    }

    return ret;

}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestGetChildName(StreamStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    StreamStructuredData<Printer> test(string);

    bool ret = test.AddToCurrentNode(table->toAdd);

    test.MoveToChild(0);

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(StringHelper::Compare(test.GetChildName(table->index[i]),table->movements[i])==0);
        i++;
    }

    printf("|%s||%s|\n", string.Buffer(), table->desResult.Buffer());

    return ret;
}

template<class Printer>
bool StreamStructuredDataTest<Printer>::TestGetNumberOfChildren(StreamStructuredDataTestAddToCurrentNodeStruct *table) {

    StreamString string;
    StreamStructuredData<Printer> test(string);

    bool ret = test.AddToCurrentNode(table->toAdd);

    test.MoveToChild(0);

    if (ret) {
        ret = test.GetNumberOfChildren() == table->nMoveUps;
    }

    return ret;
}

#endif /* TEST_CORE_BAREMETAL_L3STREAMS_SENDERSTRUCTUREDDATATEST_H_ */

