/**
 * @file MemoryArea.h
 * @brief Header file for class MemoryArea
 * @date 23/02/2016
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

 * @details This header file contains the declaration of the class MemoryArea
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYAREA_H_
#define MEMORYAREA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HeapManager.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Memory allocated area that behaves similarly to a StaticList with variable elements size.
 * @details The MemoryArea class facilitates the automatic management (allocation and freeing) of chunks of memory.
 */
class DLL_API MemoryArea {

public:
    /**
     * @brief Constructor
     * @post
     *   GetMemoryStart() == NULL &&
     *   GetMemorySize() == 0
     */
    MemoryArea();

    /**
     * @brief Destructor. Frees the memory.
     * @post
     *   GetMemoryStart() == NULL &&
     *   GetMemorySize() == 0
     */
    ~MemoryArea();

    /**
     * @brief Initialises the memory area with a specific pre-allocated memory in input.
     * @param[in] initialMemory is the pointer a pre-allocated memory to be set
     * @param[in] initialSize is the size of \a initialMemory
     * @warning The memory pointed by \a initialMemory is not copied but only wrapped. Functions like
     * MemoryArea::Add(*) and MemoryArea::Free(*) can deallocate \a initialMemory. As a consequence
     * make sure that \a initialMemory is not used outside the scope of MemoryArea.
     * @return true if the memory area is empty, false if it is already initialised.
     * @pre
     *   GetMemoryStart() == NULL &&
     *   GetMemorySize() == 0
     */
    bool InitMemory(void * const initialMemory, const uint32 initialSize);


    /**
     * @brief Frees all the allocated memory.
     */
    void Free();

    /**
     * @brief Sets the heap name.
     * @param[in] name is the name of the heap where the memory has to be
     * allocated.
     */
    void SetHeapName(const char8 * const name);

    /**
     * @brief Creates a chunk of memory into this memory area and copies existing memory.
     * @param[in] source pointer to the memory which must be copied.
     * @param[in] memorySize is the size of the \a source in bytes.
     * @param[out] offset is the distance in bytes of the beginning of the
     * allocated memory chunk with respect to the begin of this memory area.
     * @return true if the memory is allocated without errors, false otherwise.
     * @pre
     *    Add(memorySize, offset) == true
     * @post
     *    GetMemorySize() = GetMemorySize()' + memorySize &&
     *    offset = GetMemorySize()'
     */
    bool Add(const void * const source,
             const uint32 memorySize,
             uint32 &offset);

    /**
     * @brief Creates a chunk on memory in this memory area.
     * @param[in] memorySize is the size to be allocated in bytes.
     * @param[out] offset is the distance in bytes of the beginning of the
     * allocated memory chunk with respect to the begin of this memory area.
     * @return true if the memory is allocated without errors, false otherwise.
     * @post
     *    GetMemorySize() = GetMemorySize()' + memorySize &&
     *    offset = GetMemorySize()'
     */
    bool Add(const uint32 memorySize,
             uint32 &offset);

    /**
     * @brief Retrieves the pointer to the begin of the memory area.
     * @return the pointer to the begin of the memory area.
     */
    void* GetMemoryStart();

    /**
     * @brief Retrieves the total size of the memory area.
     * @return the total allocated size.
     */
    uint32 GetMemorySize() const;

    /**
     * @brief Retrieves the pointer to the memory allocated
     * in a specific position.
     * @param[in] offset is the distance in byte of the required memory with respect
     * the begin of the memory area.
     * return &GetMemoryStart()[offset]
     */
    void *GetPointer(const uint32 offset);

private:

    /**
     * The memory area.
     */
    void* memory;

    /**
     * The name of the heap where the memory managed by this area belongs to.
     */
    char8 * heapName;

    /**
     * The size of the memory area.
     */
    uint32 size;
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYAREA_H_ */

