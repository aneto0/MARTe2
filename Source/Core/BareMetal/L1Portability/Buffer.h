/**
 * @file Buffer.h
 * @brief Header file for class Buffer
 * @date 04/04/2017
 * @author Andre' Torres
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

 * @details This header file contains the declaration of the class Buffer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BUFFER_H_
#define BUFFER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "GlobalObjectsDatabase.h"
#include "MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief The class provides a memory buffer management to store and
 * retrieve data at index.
 *
 * @todo Provide locking mechanism through atomic operations.
 */
class Buffer {
public:
    /**
     * @brief Constructor. Allocates memory from the standard heap.
     * @param[in] bufferSize is the size of the buffer to allocate memory.
     * @param[in] typeSize is the size of type of the buffer elements.
     */
    Buffer(const uint32 bufferSize, const uint32 typeSize);

    /**
     * @brief Destructor. Frees allocated memory.
     */
    virtual ~Buffer();

    /**
     * @brief Initialiser. Preloads array with default value.
     * @return if success.
     */
    virtual bool Initialise(const char8 * data);

    /**
     * @brief Accessor. Returns size of templated array.
     * @return size of array.
     */
    virtual uint32 GetSize() const;

    /**
     * @brief Accessor. Retrieves data at index.
     * @return if valid index.
     */
    virtual bool GetData(char8 * const & data, const uint32 index) const;

    /**
     * @brief Accessor. Inserts data at index.
     * @return if valid index.
     */
    virtual bool PutData(const char8 * data, const uint32 index);

private:
    /**
     * Size of the buffer
     */
    uint32 size;

    /**
     * Size of the templated type
     */
    uint32 sizeOf;

    /**
     * Actual list
     */
    char8 * buffer;

/*lint -e{1712} the implementation does not provide default constructor*/
};

}



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BUFFER_H_ */
	
