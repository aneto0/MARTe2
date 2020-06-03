/**
 * @file BinaryTree.h
 * @brief Header file for class BinaryTree
 * @date 14/03/2019
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

 * @details This header file contains the declaration of the class BinaryTree
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BINARYTREE_H_
#define BINARYTREE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StaticList.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Implementation of a simple binary tree
 * @details For each element to be added to the list, an integer key is computed using the provided template hash function (see HashFunction).
 * The element and the computed key are inserted keeping the list sorted by keys. This allows to speed up the search operation to log(N) using
 * the binary search.
 */
template<typename T, typename HashObject>
class BinaryTree {
public:

    /**
     * @brief Constructor
     */
    BinaryTree();

    /**
     * @brief Destructor
     */
    virtual ~BinaryTree();

    /**
     * @brief Inserts an element in the list
     * @param[in] id is the element identifier
     * @param [in] value is the element value
     */
    uint32 Insert(const char8 * const id, const T value);

    /**
     * @brief Removes an element from the list
     * @param[in] id is the element identifier
     * @return true if the element is in the list, false otherwise
     */
    bool Remove(const char8 * const id);

    /**
     * @brief Searches an element in the list
     * @param[in] id the element identifier
     * @param[out] index the index of the element in the list if found.
     * @return true if the element is in the list, false otherwise.
     */
    bool Search(const char8 * const id, uint32 &index);

    /**
     * @brief Retrieves the required element value.
     * @param[in] position the position of the element in the list
     * @return the value of the element at \a position  within the list
     */
    T operator[](const uint32 position);

    /**
     * @brief Retrieves the required element value.
     * @param[in] id the element identifier
     * @return the value of the required element
     */
    T operator[](const char8 * const id);

    /**
     * @brief Empties the list
     */
    void Reset();

    /**
     * @brief Retrieves the size
     * @return the number of elements in the list
     */
    uint32 GetSize();

private:

    /**
     * The hash function.
     */
    HashObject hashFun;

    /**
     * The key list
     */
    StaticList<uint32> myKeyList;

    /**
     * The element value list
     */
    StaticList<T> myList;

    /**
     * Implements the binary search.
     * @param key the key to search
     * @param index the retrieved index.
     * @return true if the key is found.
     */
    bool BinarySearch(const uint32 key, uint32 &index);
};

template<typename T, typename HashObject>
BinaryTree<T, HashObject>::BinaryTree() {

}

template<typename T, typename HashObject>
BinaryTree<T, HashObject>::~BinaryTree() {

}

template<typename T, typename HashObject>
uint32 BinaryTree<T, HashObject>::Insert(const char8 * const id, const T value) {
    uint32 key = hashFun.Compute(id, 0u);
    uint32 index = 0u;
    bool ret = !BinarySearch(key, index);
    if (ret) {
        ret = myKeyList.Insert(index, key);
    }
    if (ret) {
        ret = myList.Insert(index, value);
    }

    return (ret) ? (index) : (0xFFFFFFFFu);

}

template<typename T, typename HashObject>
bool BinaryTree<T, HashObject>::Remove(const char8 * const id) {
    uint32 key = hashFun.Compute(id, 0u);

    uint32 index = 0u;
    bool ret = BinarySearch(key, index);
    if (ret) {
        myKeyList.Remove(index);
        myList.Remove(index);
    }
    return ret;
}

template<typename T, typename HashObject>
bool BinaryTree<T, HashObject>::Search(const char8 * const id, uint32 &index) {

    uint32 key = hashFun.Compute(id, 0u);
    return BinarySearch(key, index);
}

template<typename T, typename HashObject>
T BinaryTree<T, HashObject>::operator[](const uint32 position) {
    return myList[position];
}

template<typename T, typename HashObject>
T BinaryTree<T, HashObject>::operator[](const char8 * const id) {
    uint32 key = hashFun.Compute(id, 0u);

    uint32 index = 0u;
    if (!BinarySearch(key, index)) {
        myKeyList.Insert(index, key);
        T x;
        myList.Insert(index, x);
    }
    return myList[index];

}

template<typename T, typename HashObject>
void BinaryTree<T, HashObject>::Reset() {
    while (myList.GetSize() > 0u) {
        myList.Remove(0u);
    }
    while (myKeyList.GetSize() > 0u) {
        myKeyList.Remove(0u);
    }
}

template<typename T, typename HashObject>
uint32 BinaryTree<T, HashObject>::GetSize() {
    return myList.GetSize();
}

template<typename T, typename HashObject>
bool BinaryTree<T, HashObject>::BinarySearch(const uint32 key, uint32 &index) {

    uint32 range_1 = myList.GetSize();
    uint32 range = (range_1 / 2u);
    index = range;
    bool ret = false;
    while (range_1 > 0u) {
        if (key == myKeyList[index]) {
            ret = true;
            break;
        }
        else if (key < myKeyList[index]) {
            uint32 rem = (range % 2u);
            range_1 = range;
            if (rem == 1u) {
                index--;
            }
            range = (range_1 / 2u);
            index -= range;
        }
        else if (key > myKeyList[index]) {
            //this means that they are equal
            uint32 rem_1 = (range_1 % 2u);
            range_1 = range;
            if (rem_1 == 0u) {
                range_1--;
                //one less
            }
            range = (range_1 / 2u);
            index += (range + 1u);
        }
        else {

        }
    }
    return ret;
}

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BINARYTREE_H_ */
