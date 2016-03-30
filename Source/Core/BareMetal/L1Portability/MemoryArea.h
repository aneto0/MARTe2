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
 * @brief Memory allocated area
 */
class DLL_API MemoryArea {

public:
    /**
     * @brief Constructor
     * @post
     *   GetMemoryStart() == NULL &&
     *   GetMemorySize() == 0;
     */
    MemoryArea();

    /**
     * @brief Destructor. Frees the memory.
     * @post
     *   GetMemoryStart() == NULL &&
     *   GetMemorySize() == 0;
     */
    ~MemoryArea();

    /**
     * @brief Frees all the allocated memory.
     */
    void Free();

    void SetHeapName(const char8 * heapName);

    /**
     * @brief Copies a chunk on memory in this memory area.
     * @param[in] element points to the memory which must be copied.
     * @param[in] memorySize is the size of the element in bytes.
     * @param[out] offset is the index of the allocated memory chunk with respect
     * to the begin of this memory area.
     * @return true if the memory will be allocated without errors, false otherwise.
     */
    bool Add(const void * const element,
             const uint32 memorySize,
             uint32 &offsetconst);

    /**
     * @brief Creates a chunk on memory in this memory area.
     * @param[in] memorySize is the size to be allocated in bytes.
     * @param[out] offset is the index of the allocated memory chunk with respect
     * to the begin of this memory area.
     * @return true if the memory will be allocated without errors, false otherwise.
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
     * @param[in] offset is the offset of the required memory with respect
     * the begin of the memory area.
     * return &GetMemoryStart()[offset]
     */
    void* GetPointer(const uint32 offset);

private:

    /**
     * The memory area.
     */
    void* memory;

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

