/**
 * @file BufferedStreamIOBuffer.h
 * @brief Header file for class BufferedStreamIOBuffer
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

 * @details This header file contains the declaration of the class BufferedStreamIOBuffer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BUFFEREDSTREAMIOBUFFER_H_
#define BUFFEREDSTREAMIOBUFFER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BufferedStreamI.h"
#include "IOBuffer.h"
#include "OperatingSystemCallbacksI.h"
#include "StreamI.h"
#include "TimeoutType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief BufferedStreamIOBuffer class.
 *
 * @details This class inherits from IOBuffer specializing NoMoreSpaceToWrite and NoMoreDataToRead functions accordingly
 * to the buffered stream requirements. In particular NoMoreSpaceToWrite acts as a flush on the stream and
 * NoMoreDataToRead refills this buffer from the stream. The Resync function is also adjusts the stream position
 * after a buffered read or write operation.
 *
 * @remark Since this buffers has to read and write on the associated stream, the constructor needs a pointer to such stream.
 */
class DLL_API BufferedStreamIOBuffer: public IOBuffer {

public:

    /**
     * @brief Default constructor.
     * @pre true
     * @post
     *   Buffer() == NULL &&
     *   GetBufferSize() == 0u &&
     *   AmountLeft() == 0u &&
     *   MaxUsableAmount() == 0u &&
     *   UsedAmountLeft() == 0 &&
     *   Position() == 0u &&
     *   UsedSize() == 0 &&
     *   undoLevel == 0 &&
     *   AllocationGranularity() == 1u &&
     *   UndoLevel() == 0 &&
     *   GetStream() == NULL &&
     */
    BufferedStreamIOBuffer();

    /**
     * @brief Constructor.
     * @param[in] s is a pointer to the stream which uses this buffer.
     * @pre true
     * @post
     *   Buffer() == NULL &&
     *   GetBufferSize() == 0u &&
     *   AmountLeft() == 0u &&
     *   MaxUsableAmount() == 0u &&
     *   UsedAmountLeft() == 0 &&
     *   Position() == 0u &&
     *   UsedSize() == 0 &&
     *   undoLevel == 0 &&
     *   AllocationGranularity() == 1u &&
     *   UndoLevel() == 0 &&
     *   GetStream() == s &&
     */
    BufferedStreamIOBuffer(OperatingSystemCallbacksI * const s);

    /**
     * @brief Adjusts the position of the stream.
     * @details This function is called from the stream after a read operation because the position was shifted
     * forward (+bufferSize) due to the refill. Calls Seek moving the cursor back (-UsedAmountLeft).
     *
     * @return false if the stream seek fails.
     */
    virtual bool Resync();

    /**
     * @brief Sets the buffer size
     * @details Implies allocating or reallocating memory to the desired size.
     * @param[in] size is the desired size for the buffer.
     * @return false in case of allocation errors.
     */
    bool SetBufferSize(const uint32 size);

    /**
     * Gets the timeout for read and write operations.
     */
    inline TimeoutType GetTimeout() const;

    /**
     * Sets the timeout for read and write operations.
     */
    inline void SetTimeout(const TimeoutType &msecTimeout);

    /**
     * Gets the stream pointer
     */
    inline const OperatingSystemCallbacksI* GetStream() const;

    /**
     * @see IOBuffer::Flush
     * @details If the stream is not NULL it calls stream->Flush, otherwise it calls IOBuffer::NoMoreSpaceToWrite.
     * This implementation allows the buffer to access to the strean Flush function. In the stream Flush implementation
     * the user must use the buffer NoMoreSpaceToWrite function instead of the buffer Flush to not cause a infinite call loop.
     */
    /*lint -e{1735} this function has the same default parameter of its father.*/
    virtual bool Flush(const uint32 neededSize=0u);

    /**
     * @see IOBuffer::Refill
     * @details If the stream is not NULL it calls stream->Refill, otherwise it calls IOBuffer::NoMoreDataToRead.
     * This implementation allows the buffer to access to the strean Refill function. In the stream Refill implementation
     * the user must use the buffer NoMoreDataToRead function instead of the buffer Refill to not cause a infinite call loop.
     */
    virtual bool Refill();

    /**
     * @brief Refills the buffer reading from the stream.
     * @details Empties the buffer, calls StreamI::UnBufferedRead with size = MaxUsableAmount.
     * In case of stream read error empties the buffer and returns false.
     *
     * @return false if the buffer is null or in case of stream read error.
     */
    virtual bool NoMoreDataToRead();

    /**
     * @brief Flushes the buffer writing on the stream.
     * @return false if the buffer is null or in case of stream write error.
     */
    virtual bool NoMoreSpaceToWrite();

private:

    /**
     * The stream that uses this buffer.
     */
    OperatingSystemCallbacksI *stream;

    /**
     * Accelerator to the BufferedStreamI buffer.
     */
    BufferedStreamI *streamBuffered;

    /**
     * The timeout for read and write operations.
     */
    TimeoutType timeout;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


TimeoutType BufferedStreamIOBuffer::GetTimeout() const {
    return timeout;
}

const OperatingSystemCallbacksI* BufferedStreamIOBuffer::GetStream() const {
    return stream;
}

void BufferedStreamIOBuffer::SetTimeout(const TimeoutType &msecTimeout) {
    timeout = msecTimeout;
}

}
#endif /* BUFFEREDSTREAMIOBUFFER_H_ */

