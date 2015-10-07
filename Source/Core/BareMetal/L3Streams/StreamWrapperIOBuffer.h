/**
 * @file StreamWrapperIOBuffer.h
 * @brief Header file for class StreamWrapperIOBuffer
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

 * @details This header file contains the declaration of the class StreamWrapperIOBuffer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STREAMWRAPPERIOBUFFER_H_
#define STREAMWRAPPERIOBUFFER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TimeoutType.h"
#include "IOBuffer.h"
#include "StreamI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A buffer associated to a generic stream.
 *
 * @details This type of buffer is used in Printf and GetToken functions when the streams does not provide
 * their own buffers. It could be used as a temporary buffer for unbuffered streams.
 *
 * @details The NoMoreDataToRead function is implemented as a refill from the stream.
 * The NoMoreSpaceToWrite is implemented as a flush on the stream.
 */
class StreamWrapperIOBuffer: public IOBuffer {

public:

    /**
     * @brief Default constructor
     * @pre true
     * @post
     *   Buffer() == NULL &&
     *   BufferSize() == 0u &&
     *   AmountLeft() == 0u &&
     *   MaxUsableAmount() == 0u &&
     *   UsedAmountLeft() == 0 &&
     *   Position() == 0u &&
     *   UsedSize() == 0 &&
     *   undoLevel == 0 ??
     *   AllocationGranularity() == 1u && &&
     *   UndoLevel() == 0
     *   GetStream() == NULL &&
     */
    StreamWrapperIOBuffer();

    /**
     * @brief Constructor for dynamic allocated buffer.
     * @param[in] s is the stream which uses this buffer.
     * @param[in] size is the size to allocate for this buffer.
     * @pre true
     * @post
     *   Buffer() == NULL &&
     *   BufferSize() == size &&
     *   AmountLeft() == 0u &&
     *   MaxUsableAmount() == 0u &&
     *   UsedAmountLeft() == 0 &&
     *   Position() == 0u &&
     *   UsedSize() == 0 &&
     *   undoLevel == 0 ??
     *   AllocationGranularity() == 1u && &&
     *   UndoLevel() == 0
     *   GetStream() == s &&
     */
    StreamWrapperIOBuffer(StreamI * const s,
                          const uint32 size);

    /**
     * @brief Constructor for memory referenced buffer.
     * @param[in] s is the stream which uses this buffer.
     * @param[in] buffer is the pointer to the preallocated memory.
     * @param[in] size is the size of the buffer.
     * @pre true
     * @post
     *   Buffer() == buffer &&
     *   BufferSize() == size &&
     *   AmountLeft() == 0u &&
     *   MaxUsableAmount() == 0u &&
     *   UsedAmountLeft() == 0 &&
     *   Position() == 0u &&
     *   UsedSize() == 0 &&
     *   undoLevel == 0 ??
     *   AllocationGranularity() == 1u && &&
     *   UndoLevel() == 0
     *   GetStream() == s
     */
    StreamWrapperIOBuffer(StreamI * const s,
                          char8 * const buffer,
                          const uint32 size);

    /**
     * @brief Adjusts the cursor position.
     * @return false if the stream seek fails.
     */
    virtual bool Resync();

    /**
     * Gets the stream pointer
     */
    inline const StreamI* GetStream() const;

protected:

    /**
     * @brief Refills the buffer reading from the stream.
     * @return false if the buffer is null or if the stream read fails.
     */
    virtual bool NoMoreDataToRead();

    /**
     * @brief Flushes the buffer on the stream.
     * @return false if the buffer is null or if the stream write fails.
     */
    virtual bool NoMoreSpaceToWrite();

private:

    /**
     * A pointer to the associated stream.
     */
    StreamI *stream;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

const StreamI* StreamWrapperIOBuffer::GetStream() const {
    return stream;
}

}

#endif /* STREAMWRAPPERIOBUFFER_H_ */

