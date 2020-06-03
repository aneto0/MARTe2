/**
 * @file BinaryTreeTest.h
 * @brief Header file for class BinaryTreeTest
 * @date 19/08/2019
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

 * @details This header file contains the declaration of the class BinaryTreeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_BAREMETAL_L1PORTABILITY_BINARYTREETEST_H_
#define TEST_CORE_BAREMETAL_L1PORTABILITY_BINARYTREETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BinaryTree.h"
#include "Fnv1aHashFunction.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Test the BinaryTree public methods.
 */
template<typename T, typename HashFunction>
class BinaryTreeTest {
public:
    BinaryTreeTest();

    virtual ~BinaryTreeTest();

    bool TestConstructor();

    /**
     * @brief Tests the Insert method.
     */
    bool TestInsert(BinaryTree<T, HashFunction> &test, const char8 *id, T value);

    /**
     * @brief Tests the Remove method.
     */
    bool TestRemove(BinaryTree<T, HashFunction> &test, const char8 *id);

    /**
     * @brief Tests the Search method.
     */
    bool TestSearch(BinaryTree<T, HashFunction> &test, const char8 *id, T result);

    /**
     * @brief Tests the GetValue method.
     */
    bool TestGetValue(BinaryTree<T, HashFunction> &test, const char8 *id, T result);

    /**
     * @brief Tests the [] operator.
     */
    bool TestSquareOperator(BinaryTree<T, HashFunction> &test, const char8 *id, T result);

    /**
     * @brief Tests the Reset method.
     */
    bool TestReset(BinaryTree<T, HashFunction> &test);

    /**
     * @brief Tests the [] method with a non-existent id.
     */
    bool TestSquareOperatorNonExistendId(BinaryTree<T, HashFunction> &test);

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {
template<typename T, typename HashFunction>
BinaryTreeTest<T, HashFunction>::BinaryTreeTest() {
}
template<typename T, typename HashFunction>
BinaryTreeTest<T, HashFunction>::~BinaryTreeTest() {
}

template<typename T, typename HashFunction>
bool BinaryTreeTest<T, HashFunction>::TestConstructor() {
    BinaryTree<T, Fnv1aHashFunction> test;
    return true;
}

template<typename T, typename HashFunction>
bool BinaryTreeTest<T, HashFunction>::TestInsert(BinaryTree<T, HashFunction> &test, const char8 *id, T value) {
    uint32 size = test.GetSize();
    test.Insert(id, value);
    return test.GetSize() == (size + 1u);
}
template<typename T, typename HashFunction>
bool BinaryTreeTest<T, HashFunction>::TestRemove(BinaryTree<T, HashFunction> &test, const char8 *id) {
    uint32 size = test.GetSize();
    test.Remove(id);
    return test.GetSize() == (size - 1u);
}
template<typename T, typename HashFunction>
bool BinaryTreeTest<T, HashFunction>::TestSearch(BinaryTree<T, HashFunction> &test, const char8 *id, T result) {
    uint32 index = 0u;
    test.Search(id, index);
    return test[index] == result;
}
template<typename T, typename HashFunction>
bool BinaryTreeTest<T, HashFunction>::TestGetValue(BinaryTree<T, HashFunction> &test, const char8 *id, T result) {

    uint32 index = 0u;
    test.Search(id, index);
    return test[index] == result;
}
template<typename T, typename HashFunction>
bool BinaryTreeTest<T, HashFunction>::TestSquareOperator(BinaryTree<T, HashFunction> &test, const char8 *id, T result) {
    return (test[id] == result);
}

template<typename T, typename HashFunction>
bool BinaryTreeTest<T, HashFunction>::TestReset(BinaryTree<T, HashFunction> &test) {
    test.Reset();
    return (test.GetSize() == 0u);
}

template<typename T, typename HashFunction>
bool BinaryTreeTest<T, HashFunction>::TestSquareOperatorNonExistendId(BinaryTree<T, HashFunction> &test) {
    T t = test["NOTEXISTENT"];
    return (!t.IsValid());
}
}

#endif /* TEST_CORE_BAREMETAL_L1PORTABILITY_BINARYTREETEST_H_ */

