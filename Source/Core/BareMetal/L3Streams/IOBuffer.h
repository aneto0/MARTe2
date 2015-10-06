/**
 * @file IOBuffer.h
 * @brief Header file for class IOBuffer
 * @date 01/10/2015
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

 * @details This header file contains the declaration of the class IOBuffer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef IOBUFFER_H_
#define IOBUFFER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CharBuffer.h"
#include "HeapManager.h"
#include "MemoryOperationsHelper.h"
#include "TimeoutType.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The most generic buffer used by streams.
 *
 * @details This class inherits from CharBuffer which represents a naked memory buffer with a pointer
 * at the beginning. It adds a movable pointer across the memory area and attributes
 * to get the size of the filled area and the readable area of the buffer.
 *
 * @details This class implements basic methods for read, write and seek operations on the buffer which are virtual
 * to avoid overloading in children.
 *
 * @details The most important functions are the inline PutC and GetC which are used in Printf and GetToken
 * functions and are shared by all children (besides the implementation of NoMoreSpaceToWrite and
 * NoMoreDataToRead depends on the specific derived class implementation)
 */
class IOBuffer {

public:
    /**
     * @brief Synchronizes the stream position with this buffer position.
     * @details This implementation is basic and only returns false.
     *
     * In BufferedStreamIOBuffer flushes the write buffer after write operations or adjusts the
     * stream position (shifted after a refill because of a previous read operation).
     *
     * @return false in this implementation.
     */
    virtual bool Resync();

    /**
     * @brief Moves the cursor to an absolute position.
     * @details Adjusts amountLeft = maxUsableAmount - position\n
     * and sets positionPtr to Char::BufferReference() (the beginning of the buffer) + position.
     *
     * @param[in] position is the desired position in the filled memory.
     * @return false if position is greater than the size of the filled memory.
     */
    virtual bool Seek(uint32 position);

    /**
     * @brief Moves the cursor relatively from the current position.
     * @details Checks that the final position is >= 0 and <= UsedSize, then moves the cursor.\n
     * -If the final position is < 0 moves the cursor at the beginning.\n
     * -If the final position is > UsedSize moves the cursor at the end.\n
     *
     * @param[in] delta is the step from the current position.
     * @return false if the final calculated position falls out of ranges, true otherwise.
     */
    virtual bool RelativeSeek(int32 delta);

    /**
     * @brief Sets manually the size of the filled memory.
     * @details If the desired size is greater than maxUsableAmount it clips the desired size to maxUsableAmount.
     *
     * @param[in] size is the desired used size.
     * @return true.
     */
    virtual bool SetUsedSize(uint32 size);

    /*---------------------------------------------------------------------------*/

    /**
     * @brief Default constructor.
     */
    inline IOBuffer();

    /**
     * @brief Constructor with the granularity mask input.
     */
    inline IOBuffer(const uint32 allocationGranularity);

    /**
     * @brief Default destructor.
     */
    virtual ~IOBuffer();

    /**
     * @brief Allocates dynamically a memory portion on the heap.
     * @details
     * -If the new size (desiredSize-reservedSpaceAtEnd) is minor than the current used size, usedSize
     * is clipped and if the position was over, it becomes the end of the new  size.\n
     * -This function calls CharBuffer::SetBufferAllocationSize, then fixes maxUsableAmount to desiredSize-reservedSpaceAtEnd.
     *
     * @param[in] desiredSize is the desired size to be allocated.
     * @param[in] reservedSpaceAtEnd is the space allocated over the usable memory area (maxUsableAmount does not consider it).\n
     * It could be for example the zero terminator character at the end of a string.
     * @return false if the allocation fails, true otherwise.
     */
    virtual bool SetBufferHeapMemory(const uint32 desiredSize,
                                     const uint32 reservedSpaceAtEnd);

    /**
     * @brief Assigns a preallocated memory with read and write access.
     * @details Calls CharBuffer::SetBufferReference, then sets the buffer as empty and maxUsableAmount = (bufferSize - reservedSpaceAtEnd).
     *
     * @param[in] buffer is a pointer to a preallocated memory.
     * @param[in] bufferSize is the size of the memory.
     * @param[in] reservedSpaceAtEnd is the memory allocated over the usable memory area.
     * @return true.
     */
    virtual bool SetBufferReferencedMemory(char8 * const buffer,
                                           const uint32 bufferSize,
                                           const uint32 reservedSpaceAtEnd);

    /**
     * @brief Assigns a preallocated memory only with read access.
     * @details Calls CharBuffer::SetBufferReference, then sets the buffer as empty and maxUsableAmount = (bufferSize - reservedSpaceAtEnd).
     * if buffer is a const char pointer the flag readOnly is set true then also CharBuffer::CanWrite returns false.
     *
     * @param[in] buffer is a pointer to a preallocated memory.
     * @param[in] bufferSize is the size of the memory.
     * @param[in] reservedSpaceAtEnd is the memory allocated but not .
     * @return true.
     */
    virtual bool SetBufferReadOnlyReferencedMemory(const char8 * const buffer,
                                                   const uint32 bufferSize,
                                                   const uint32 reservedSpaceAtEnd);

    /*---------------------------------------------------------------------------*/

    /**
     * @brief The size of the allocated memory.
     * @return the size of the allocated memory.
     */
    inline uint32 BufferSize() const;

    /**
     * @brief Gets the size from the beginning to the end of memory (without considering the reserved space).
     * @return the usable buffer size.
     */
    inline uint32 MaxUsableAmount() const;

    /**
     * @brief Gets the size from the cursor to the end of memory (without reserved space).
     * @return the usable space remained from the current position.
     */
    inline uint32 AmountLeft() const;

    /**
     * @brief Gets the size from the cursor to the end of filled memory area.
     * @return the space remained from the current position to the end of the filled memory in the buffer.
     */
    inline uint32 UsedAmountLeft() const;

    /**
     * @brief Gets the cursor position.
     * @return the current position, namely the size from the beginning to the cursor. */
    inline uint32 Position() const;

    /**
     * @brief Gets the size of the filled memory area.
     * @return the size of the filled memory area.
     */
    inline uint32 UsedSize() const;

    /**
     * @brief Gets the pointer at the beginning of the buffer in read only mode.
     * @return the pointer at the beginning of the buffer. */
    inline const char8 *Buffer() const;

    /**
     * @brief Gets the pointer at the beginning of the buffer in read-write mode.
     * @return the pointer at the beginning of the buffer.
     */
    inline char8 *BufferReference() const;

    /*---------------------------------------------------------------------------*/

    /**
     * @brief Puts a character on the buffer.
     * @details This inline function is called by formatted print-like functions for buffered streams. The
     * implementation of NoMoreSpaceToWrite depends on children classes and it could be
     * for example a flush (BufferedStreamIOBuffer) or a new allocation (StreamStringIOBuffer).\n
     *
     * The value of undoLevel could be used to trigger the call to NoMoreSpaceToWrite when
     * the cursor is at a specific position.
     *
     * @param[in] c is the character to copy on this buffer.
     * @return false if there is no space to write or the buffer is not writable, true otherwise.
     */
    inline bool PutC(const char8 c);

    /**
     * @brief If possible remove the last character from the buffer.
     * @details Increments amountLeft and decrements the cursor.
     *
     * @return false if the position is at the beginning, true otherwise.
     */
    inline bool UnPutC();

    /**
     * @brief Get a character from the buffer.
     * @details This function calls NoMoreDataToRead which can for example
     * refill this buffer from the stream (BufferedStreamIOBuffer)
     * when the cursor is in a specific position defined by undoLevel.
     *
     * @param[out] c is the character in return.
     * @return false if the cursor is at the end of the filled memory, true otherwise.
     */
    inline bool GetC(char8 &c);

    /**
     * @brief If possible decrements the cursor.
     *
     * @details It decrements the cursor and increments amountLeft.
     *
     * @return false if the cursor is at the beginning, true otherwise.
     */
    inline bool UnGetC();

    /**
     * @brief Empties the buffer.
     *
     * @details Sets amountLeft and fill to maxUsableAmount. Then sets the buffer pointer at the beginning.
     */
    inline void Empty();

    /**
     * @brief Writes from an input buffer.
     *
     * @details The function does nothing if CharBuffer::CanWrite returns false,
     * otherwise copy size bytes from the input buffer, sets the cursor size
     * positions forward and adjusts fillLeft and amountLeft accordingly.\n
     * If size is greater than amountLeft is clipped.
     *
     * @param[in] buffer contains the data to be written write in this buffer.
<<<<<<< HEAD
     * @param[in,out] size is the number of bytes to be copied. This value will be clipped to the space left if needed.
=======
     * @param[in,out] size is the number of bytes to be copied.
>>>>>>> refs/remotes/origin/#238.1_Streams
     */
    virtual bool Write(const char8 * const buffer,
                       uint32 &size);

    /**
     * @brief Writes all the size in the argument from an input buffer.
     * @details This function calls Write and NoMoreSpaceToWrite until size is consumed.
     * @param[in] buffer contains data to be written in this buffer.
     * @param[in] size is the number of byte to be copied.
     */
    bool WriteAll(const char8 * buffer,
                  const uint32 &size);

    /**
     * @brief Reads from this buffer to an output buffer.
     * @param[out] buffer is the output buffer where data must be written.
     * @param[in] size is the number of bytes to be read.
     */
    bool Read(char8 * const buffer,
              uint32 &size);

protected:

    /**
     * @see NoMoreSpaceToWrite(const uint32)
     */
    virtual bool NoMoreSpaceToWrite();

    /**
     * @brief The routine executed in PutC when amountLeft is <= undoLevel, namely the cursor arrived to a specific position.
     * @details This basic implementation only returns false.\n
     *
     * In StreamStringIOBuffer undoLevel is zero, so when the cursor arrived at the end of the memory,
     * this function allocated a new portion of memory in the queue.\n
     *
     * In BufferedStreamIOBuffer undoLevel is zero, so when the cursor arrived at the end of the memory
     * this function flushes this buffer to the stream.\n
     *
     * @param[in] neededSize is the size of the memory to be allocated or flushed (not used at this implementation level).
     * @return false at this implementation level.
     */
    virtual bool NoMoreSpaceToWrite(const uint32 neededSize);

    /**
     * @brief The routine executed in GetC when amountLeft is <= undoLevel, namely the cursor arrived to a specific position.
     * @details This implementation is basic and only returns false.
     *
     * In BufferedStreamIOBuffer undoLevel is zero, so when the cursor arrives at the end of the memory
     * this function refills the buffer from the stream for a new read operation.
     *
     * @return false in this implementation.
     */
    virtual bool NoMoreDataToRead();

    /*---------------------------------------------------------------------------*/

    bool PrintFormattedToStream(const char8 * format,
                                const AnyType pars[]);

private:

    /**
     * The internal buffer.
     */
    CharBuffer internalBuffer;

    /**
     * The size of the usable memory in the buffer.
     * Usually it is equal to the CharBuffer::BufferSize
     * but for example in StreamString considering the final 0
     * the usable memory is CharBuffer::BufferSize - 1.
     */
    uint32 maxUsableAmount;

    /**
     * The remained size from the current position
     * to the end of the usable memory.
     */
    uint32 amountLeft;

    /**
     * The remained size from the filled memory to the end
     * of the usable memory.
     */
    uint32 fillLeft;

    /**
     * Allows to call NoMoreDataToRead or NoMoreSpaceToWrite when
     * the remained size from the cursor (amountLeft) is <= to it.
     */
    uint32 undoLevel;

    /**
     * The pointer to the current position in the buffer (cursor).
     */
    char8 *positionPtr;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

IOBuffer::IOBuffer() :
        internalBuffer() {
    amountLeft = 0u;
    maxUsableAmount = 0u;
    positionPtr = static_cast<char8 *>(NULL);
    fillLeft = 0u;
    undoLevel = 0u;
}

IOBuffer::IOBuffer(const uint32 allocationGranularity) :
        internalBuffer(allocationGranularity) {
    amountLeft = 0u;
    maxUsableAmount = 0u;
    positionPtr = static_cast<char8 *>(NULL);
    fillLeft = 0u;
    undoLevel = 0u;
}

uint32 IOBuffer::BufferSize() const {
    return internalBuffer.Size();
}

uint32 IOBuffer::MaxUsableAmount() const {
    return maxUsableAmount;
}

uint32 IOBuffer::AmountLeft() const {
    return amountLeft;
}

uint32 IOBuffer::UsedAmountLeft() const {
    return amountLeft - fillLeft;
}

uint32 IOBuffer::Position() const {
    return maxUsableAmount - amountLeft;
}

uint32 IOBuffer::UsedSize() const {
    return maxUsableAmount - fillLeft;
}

const char8 *IOBuffer::Buffer() const {
    return internalBuffer.Buffer();
}

char8 *IOBuffer::BufferReference() const {
    return internalBuffer.BufferReference();
}

void IOBuffer::Empty() {
    amountLeft = maxUsableAmount;   // Seek 0
    fillLeft = maxUsableAmount;   // SetSize 0
    positionPtr = const_cast<char8 *>(Buffer()); // seek 0
}

/*---------------------------------------------------------------------------*/

bool IOBuffer::PutC(const char8 c) {

    bool retval = true;
    // check if buffer needs updating and or saving
    if (amountLeft <= undoLevel) {
        if (!NoMoreSpaceToWrite()) {
            retval = false;
        }

        // check if we can continue or must abort
        if (amountLeft == 0u) {
            retval = false;
        }
    }

    // check later so that to give a chance to allocate memory
    // if that is the policy of this buffering scheme
    if (!internalBuffer.CanWrite()) {
        retval = false;
    }

    if (retval) {

        *positionPtr = c;

        positionPtr++;
        amountLeft--;
        if (fillLeft > amountLeft) {
            fillLeft = amountLeft;
        }
    }

    return retval;
}

bool IOBuffer::UnPutC() {

    // can I still do it?
    bool retval = (Position() > 0u);
    if (retval) {

        if (amountLeft == fillLeft) {
            fillLeft++;
        }
        positionPtr--;
        amountLeft++;
    }
    return retval;
}

bool IOBuffer::GetC(char8 &c) {

    bool retval = true;
    // check if buffer needs updating and or saving
    if (UsedAmountLeft() <= undoLevel) {
        if (!NoMoreDataToRead()) {
            retval = false;
        }

        if (UsedAmountLeft() == 0u) {
            retval = false;
        }
    }

    if (retval) {
        c = *positionPtr;

        positionPtr++;
        amountLeft--;

    }
    return retval;
}

bool IOBuffer::UnGetC() {
    bool retval = (Position() > 0u);

    // can I still do it?
    if (retval) {
        positionPtr--;
        amountLeft++;
    }

    return retval;
}

}

#endif /* IOBUFFER_H_ */

