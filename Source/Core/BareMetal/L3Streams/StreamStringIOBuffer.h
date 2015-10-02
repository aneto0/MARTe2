/**
 * @file StreamStringIOBuffer.h
 * @brief Header file for class StreamStringIOBuffer
 * @date 02/10/2015
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

#ifndef STREAMSTRINGIOBUFFER_H_
#define STREAMSTRINGIOBUFFER_H_

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
 * @details For memory allocations it adds one to the desired size passed by argument and sets reservedSpaceAtEnd = 1 for the
 * final \0 character.
 */
class StreamStringIOBuffer: public IOBuffer {

public:


    /**
     * @brief Default constructor
     */
    StreamStringIOBuffer();

    /**
     * @brief Default destructor.
     */
    virtual ~StreamStringIOBuffer();

    /**
     * @brief Sets the size of the buffer to be desiredSize or greater up next granularity.
     *
     * @details Truncates stringSize to desiredSize-1.\n
     * Calls IOBuffer::SetBufferHeapMemory with desiredSize+1 and reservedSpaceAtEnd=1.
     *
     * @param[in] desiredSize is the desired size to allocate without considering the final \0.
     * @param[in] allocationGranularityMask defines the desired granularity (see CharBuffer::SetBufferAllocationSize), default is granularity=64 bytes.
     * @return false in case of errors in the allocation.
     */
    virtual bool SetBufferAllocationSize(uint32 desiredSize,
                                         uint32 allocationGranularityMask = 0xFFFFFFC0);

public:
    /**
     * @brief If the buffer is full this function is called to allocate new memory.
     *
     * @details This function calls SetBufferAllocationSize passing neededSize.
     *
     * @param[in] neededSize is the desired size to allocate.
     * @param[in] msecTimeout is the timeout not used here.
     * @return false in case of errors in the memory allocation.
     */
    virtual bool NoMoreSpaceToWrite(uint32 neededSize = 1,
                                    TimeoutType msecTimeout = TTDefault);

    /**
     * @brief Copies buffer the end of writeBuffer.
     *
     * @details If the current buffer size is not enough a new portion of memory
     * will be dynamically allocated.
     *
     * @param[in] buffer contains data to be written.
     * @param[in] size is the desired number of bytes to copy.
     */
    virtual void Write(const char8 *buffer,
                       uint32 &size);

    /**
     * @brief Adds the termination character at the end of the filled memory.
     */
    virtual void Terminate();

};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STREAMSTRINGIOBUFFER_H_ */

