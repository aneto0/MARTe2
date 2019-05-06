/**
 * @file BinaryTree.h
 * @brief Header file for class BinaryTree
 * @date 14 mar 2019
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
#include "HashFunction.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<typename inputType, typename outputType>
class BinaryTree {
public:
    BinaryTree();
    virtual ~BinaryTree();

    uint32 Insert(inputType id,
                outputType value);

    bool Remove(inputType id);

    bool Search(inputType id,
                uint32 &index);

    outputType& GetValue(uint32 position);

    outputType& operator[](inputType id);

    void Reset();

    void operator =(outputType& value);

private:
    StaticList<outputType> myKeyList;
    StaticList<outputType> myList;

    bool BinarySearch(uint32 key,
                      uint32 &index);
};

template<typename inputType, typename outputType>
BinaryTree<inputType, outputType>::BinaryTree() {

}

template<typename inputType, typename outputType>
BinaryTree<inputType, outputType>::~BinaryTree() {

}

template<typename inputType, typename outputType>
uint32 BinaryTree<inputType, outputType>::Insert(inputType id,
                                               outputType value) {
    uint32 key = HashFunction::DJB(id);
    uint32 index = 0u;
    BinarySearch(key, index);

    bool ret = myKeyList.Insert(index, key);
    if (ret) {
        ret = myList.Insert(index, value);
    }
    return index;

}

template<typename inputType, typename outputType>
bool BinaryTree<inputType, outputType>::Remove(inputType id) {
    uint32 key = HashFunction::DJB(id);

    uint32 index = 0u;
    bool ret = BinarySearch(key, index);
    if (ret) {
        myKeyList.Remove(index);
        myList.Remove(index);
    }
    return ret;
}

template<typename inputType, typename outputType>
bool BinaryTree<inputType, outputType>::Search(inputType id,
                                               uint32 &index) {

    uint32 key = HashFunction::DJB(id);

    return BinarySearch(key, index);
}

template<typename inputType, typename outputType>
outputType& BinaryTree<inputType, outputType>::GetValue(uint32 position) {
    return myList[position];
}

template<typename inputType, typename outputType>
outputType& BinaryTree<inputType, outputType>::operator[](inputType id) {
    uint32 key = HashFunction::DJB(id);

    uint32 index = 0u;
    if(!BinarySearch(key, index)){
        myKeyList.Insert(index, key);
        outputType x;
        myList.Insert(index, x);
    }
    return myList[index];

}

template<typename inputType, typename outputType>
void BinaryTree<inputType, outputType>::Reset() {
    myKeyList.Reset();
    myList.Reset();

}

template<typename inputType, typename outputType>
bool BinaryTree<inputType, outputType>::BinarySearch(uint32 key,
                                                     uint32 &index) {

    uint32 range_1 = myList.GetSize();
    uint32 range = range_1 / 2;
    index = range;
    bool ret = false;
    //bool done = false;
    while (range_1 > 0) {
        if (key == myKeyList[index]) {
            ret = true;
            break;
        }
        else if (key < myKeyList[index]) {
            uint32 rem = range % 2;
            range_1 = range;
            if (rem == 1) {
                index--;
            }
            range = range_1 / 2;
            index -= range;
        }
        else if (key > myKeyList[index]) {
            //this means that they are equal
            uint32 rem_1 = range_1 % 2;
            range_1 = range;
            if (rem_1 == 0) {
                range_1--;
                //one less
            }
            range = range_1 / 2;
            index += (range + 1);
        }
    }
    return ret;
}



}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BINARYTREE_H_ */

