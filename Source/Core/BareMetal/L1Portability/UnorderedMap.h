/**
 * @file UnorderedMap.h
 * @brief Header file for class UnorderedMap
 * @date 08/09/2025
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class UnorderedMap
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HeapManager.h"
#include "LinkedListable.h"
#include "LinkedListHolderT.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#define INITIAL_CAPACITY 8
#define LOAD_FACTOR 0.75

namespace MARTe {

/**
 * Private class to store the map entries.
 */
template<typename T>
class UnorderedMapEntry : public LinkedListable {
public:
    /**
     * Sets the element hash to zero.
     */
    UnorderedMapEntry() {
        hash = 0LLU;
    }

    /**
     * NOOP
     */
    virtual ~UnorderedMapEntry() {
    }

    /**
     * The hash associated to the element.
     */
    uint64 hash;

    /**
     * The element value.
     */
    T value;
};

/**
 * @brief Implementation of an UnorderedMap aiming at log(1) insertion and search times.
 * @details The Insert and Search operations require that the element type be CopyAssignable (i.e., support operator=).
 */
template<typename T, typename HashObject, uint32 initialCapacity = 8, uint32 loadFactor = 75>
class UnorderedMap {
public:

    /**
     * @brief Constructor
     */
    UnorderedMap();

    /**
     * @brief Destructor
     */
    virtual ~UnorderedMap();

    /**
     * @brief Inserts an element in the map.
     * @param[in] id is the element identifier
     * @param [in, out] value is the element value. The element type must be CopyAssignable (i.e., support operator=).
     * @return true if the element was successfully added. 
     * If the element already exists the variable value will be update to its stored value and the function will return false.
     */
    bool Insert(const char8 * const id, T &value);

    /**
     * @brief Removes an element from the map.
     * @param[in] id is the element identifier.
     * @return true if the element is in the list, false otherwise.
     */
    bool Remove(const char8 * const id);

    /**
     * @brief Searches for the with the element with the input \a id in the map.
     * @param[in] id the element identifier
     * @param [out] value is the element value. The element type must be CopyAssignable (i.e., support operator=).
     * @return true if the element is in the list, false otherwise.
     */
    bool Search(const char8 * const id, T &value);

    /**
     * @brief Retrieves the required element value.
     * @param[in] id the element identifier.
     * @return the value of the required element.
     */
    T operator[](const char8 * const id);

    /**
     * @brief Empties the map.
     */
    void Reset();

    /**
     * @brief Retrieves the map size.
     * @return the number of elements in the map.
     */
    uint32 GetSize();

private:

    /**
     * Resizes the map when the ratio between the number of elements and the capacity is greater than the loadFactor.
     */
    void Resize();

    /**
     * The hash function.
     */
    HashObject hashFun;

    /**
     * The element value list.
     */
    LinkedListHolderT<UnorderedMapEntry<T>, true> *entries;

    /**
     * The map capacity.
     */
    uint32 capacity;

    /**
     * The number of elements currently in the map.
     */
    uint32 size;
};

template<typename T, typename HashObject, uint32 initialCapacity, uint32 loadFactor>
UnorderedMap<T, HashObject, initialCapacity, loadFactor>::UnorderedMap() {
    capacity = 0u;
    size = 0u;
    entries = static_cast<LinkedListHolderT<UnorderedMapEntry<T>, true> *>(0);
}

template<typename T, typename HashObject, uint32 initialCapacity, uint32 loadFactor>
UnorderedMap<T, HashObject, initialCapacity, loadFactor>::~UnorderedMap() {
    if (entries != static_cast<LinkedListHolderT<UnorderedMapEntry<T>, true> *>(0)) {
        delete [] entries;
    }
}

template<typename T, typename HashObject, uint32 initialCapacity, uint32 loadFactor>
bool UnorderedMap<T, HashObject, initialCapacity, loadFactor>::Insert(const char8 * const id, T &value) {
    if (capacity == 0u) {
        capacity = initialCapacity;
        entries = new LinkedListHolderT<UnorderedMapEntry<T>, true>[capacity];
    } 
    else {
        uint32 currentLoadFactor = size;
        currentLoadFactor *= 100; //Cannot store loadFactor as a float
        currentLoadFactor /= capacity;
        if (currentLoadFactor > loadFactor) {
            Resize();
        }
    }

    bool ret = true;
    uint64 hash = hashFun.Compute64(id, 0u);
    uint32 i = static_cast<uint32>(hash % capacity);
    uint32 listSize = entries[i].ListSize();
    uint32 j;
    for (j=0; j<listSize; j++) {
        UnorderedMapEntry<T> *entry = entries[i].ListPeek(j);
        if (entry->hash == hash) {
            value = entry->value;
            ret = false;
            break;
        }
    }
    if (ret) {
        UnorderedMapEntry<T> *newEntry = new UnorderedMapEntry<T>();
        newEntry->hash = hash;
        newEntry->value = value;
        entries[i].ListAdd(newEntry);
        size++;
    }
    return ret;
}

template<typename T, typename HashObject, uint32 initialCapacity, uint32 loadFactor>
bool UnorderedMap<T, HashObject, initialCapacity, loadFactor>::Remove(const char8 * const id) {
    bool ret = (capacity != 0u);
    if (ret) {
        ret = false;
        uint64 hash = hashFun.Compute64(id, 0u);
        uint32 i = static_cast<uint32>(hash % static_cast<uint64>(capacity));
        uint32 listSize = entries[i].ListSize();
        uint32 j;
        for (j=0; j<listSize; j++) {
            UnorderedMapEntry<T> *entry = entries[i].ListPeek(j);
            if(entry->hash == hash) {
                entries[i].ListDelete(entry);
                ret = true;
                size--;
                break;
            }
        }
    }
    return ret;
}

template<typename T, typename HashObject, uint32 initialCapacity, uint32 loadFactor>
bool UnorderedMap<T, HashObject, initialCapacity, loadFactor>::Search(const char8 * const id, T &value) {
    bool ret = (capacity != 0u);
    if (ret) {
        ret = false;
        uint64 hash = hashFun.Compute64(id, 0u);
        uint32 i = static_cast<uint32>(hash % static_cast<uint64>(capacity));
        uint32 listSize = entries[i].ListSize();
        uint32 j;
        for (j=0; j<listSize; j++) {
            UnorderedMapEntry<T> *entry = entries[i].ListPeek(j);
            if(entry->hash == hash) {
                value = entry->value;
                ret = true;
                break;
            }
        }
    }
    return ret;
}

template<typename T, typename HashObject, uint32 initialCapacity, uint32 loadFactor>
T UnorderedMap<T, HashObject, initialCapacity, loadFactor>::operator[](const char8 * const id) {
    T ret;
    (void) Search(id, ret);
    return ret;
}

template<typename T, typename HashObject, uint32 initialCapacity, uint32 loadFactor>
void UnorderedMap<T, HashObject, initialCapacity, loadFactor>::Reset() {
    uint32 i;
    for(i=0u; i<capacity; i++) {
        entries[i].CleanUp();
    }
    size = 0u;
}

template<typename T, typename HashObject, uint32 initialCapacity, uint32 loadFactor>
uint32 UnorderedMap<T, HashObject, initialCapacity, loadFactor>::GetSize() {
    return size;
}

template<typename T, typename HashObject, uint32 initialCapacity, uint32 loadFactor>
void UnorderedMap<T, HashObject, initialCapacity, loadFactor>::Resize() {

    uint32 oldCapacity = capacity;
    LinkedListHolderT<UnorderedMapEntry<T>, true> *oldEntries = entries;

    capacity *= 2;
    size = 0;
    entries = new LinkedListHolderT<UnorderedMapEntry<T>, true>[capacity];
    uint32 i;
    for(i=0; i<oldCapacity; ++i) {
        uint32 j;
        uint32 listSize = oldEntries[i].ListSize();
        for(j=0; j<listSize; j++) {
            UnorderedMapEntry<T> *oldEntry = oldEntries[i].ListPeek(j);
            UnorderedMapEntry<T> *newEntry = new UnorderedMapEntry<T>();
            newEntry->hash = oldEntry->hash;
            newEntry->value = oldEntry->value;
            uint32 h = static_cast<uint32>(newEntry->hash % static_cast<uint64>(capacity));
            entries[h].ListAdd(newEntry);
            size++;
        }
    }
    delete []oldEntries;
}

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HASHTABLE_H_ */
