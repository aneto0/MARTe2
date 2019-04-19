/**
 * @file StreamStringIOBuffer.h
 * @brief Header file for class StreamStringIOBuffer
 * @date 26/10/2015
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

 * @details This header file contains the declaration of the class StreamStringIOBuffer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRINGIOBUFFER_H_
#define STRINGIOBUFFER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "HeapManager.h"
#include "CharBuffer.h"
#include "IOBuffer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The StreamString buffer.
 *
 * @details This class inherits from IOBuffer and specialize the function NoMoreSpaceToWrite accordingly to
 * the string requirements, namely allocating new space when the buffer is full in case of write operations.
 *
 * For memory allocations it adds one to the desired size passed by argument and sets reservedSpaceAtEnd = 1 for the
 * final '\0' character.
 */
class DLL_API StreamStringIOBuffer: public IOBuffer {

public:

    /**
     * @brief Default constructor
     * @pre true
     * @post
     *   Buffer() == NULL &&
     *   GetBufferSize() == 0u &&
     *   AmountLeft() == 0u &&
     *   MaxUsableAmount() == 0u &&
     *   UsedAmountLeft() == 0 &&
     *   Position() == 0u &&
     *   UsedSize() == 0 &&
     *   undoLevel == 0 ??
     *   AllocationGranularity() == 64u &&
     *   UndoLevel() == 0
     */
    StreamStringIOBuffer();

    /**
     * @brief Constructor specifying granularity
     * @pre true
     * @post
     *   Buffer() == NULL &&
     *   GetBufferSize() == 0u &&
     *   AmountLeft() == 0u &&
     *   MaxUsableAmount() == 0u &&
     *   UsedAmountLeft() == 0 &&
     *   Position() == 0u &&
     *   UsedSize() == 0 &&
     *   undoLevel == 0 ??
     *   AllocationGranularity() == granularity &&
     *   UndoLevel() == 0
     */
    StreamStringIOBuffer(const uint32 granularity);

    /**
     * @brief Destructor.
     */
    virtual ~StreamStringIOBuffer();

    /**
     * @brief Sets the size of the buffer to be desiredSize.
     * @details Truncates stringSize to desiredSize-1.\n
     * Calls IOBuffer::SetBufferHeapMemory with desiredSize+1 and reservedSpaceAtEnd=1.
     * @param[in] desiredSize is the desired size to allocate without considering the final '\0'.
     * @return false in case of errors in the allocation.
     */
    virtual bool SetBufferAllocationSize(const uint32 desiredSize);

    /**
     * @see IOBuffer::Write(const char8 * const, uint32&)
     */
    virtual bool Write(const char8 * const buffer,
                       uint32 &size);

    /**
     * @brief Adds the termination character at the end of the filled memory.
     */
    virtual void Terminate();

    /**
     * @brief Calls NoMoreSpaceToWrite(1U)
     * @see IOBuffer::NoMoreSpaceToWrite()
     */
    virtual bool NoMoreSpaceToWrite();

    /**
     * @see IOBuffer::NoMoreSpaceToWrite(const uint32)
     */
    virtual bool NoMoreSpaceToWrite(const uint32 neededSize);

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STRINGIOBUFFER_H_ */

