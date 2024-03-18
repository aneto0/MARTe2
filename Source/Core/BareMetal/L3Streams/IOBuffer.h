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
#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Class which represents a generic input output buffer intended to be
 * used by any kind of streams.
 *
 * @details This class represents a naked memory buffer with a pointer at the
 * beginning. It adds a movable pointer across the memory area and attributes
 * to get the size of the filled area and the readable area of the buffer.
 *
 * This class implements basic methods for read, write and seek operations
 * on the buffer which are virtual to avoid overloading in children.
 *
 * The most important functions are the inline PutC and GetC which are used
 * in Printf and GetToken functions and are shared by all children (besides
 * the implementation of NoMoreSpaceToWrite and NoMoreDataToRead depends on
 * the specific derived class implementation)
 */
class DLL_API IOBuffer {

public:

    /**
     * @brief Synchronizes the stream position with this buffer position.
     * @details This implementation is basic and only returns false.
     * In BufferedStreamIOBuffer flushes the write buffer after write
     * operations or adjusts the stream position (shifted after a refill
     * because of a previous read operation).
     * @return false in this implementation.
     */
    virtual bool Resync();

    /**
     * @brief Moves the cursor to an absolute position.
     * @details Adjusts AmountLeft() == MaxUsableAmount() - position\n
     *
     * @param[in] position is the desired position in the filled memory.
     * @return false if position is greater than the size of the filled memory.
     */
    virtual bool Seek(const uint32 position);

    /**
     * @brief Moves the cursor relatively from the current position.
     * @details Checks that the final position is >= 0 and <= UsedSize, then moves the cursor.\n
     * -If the final position is < 0 moves the cursor at the beginning.\n
     * -If the final position is > UsedSize moves the cursor at the end.\n
     *
     * @param[in] delta is the step from the current position.
     * @return false if the final calculated position falls out of ranges, true otherwise.
     */
    virtual bool RelativeSeek(const int32 delta);

    /**
     * @brief Sets manually the size of the filled memory.
     * @details If the desired size is greater than maxUsableAmount it clips the desired size to maxUsableAmount.
     *
     * @param[in] desiredSize is the desired used size.
     */
    virtual void SetUsedSize(const uint32 desiredSize);

    /*---------------------------------------------------------------------------*/

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
     *   AllocationGranularity() == 1u &&
     *   UndoLevel() == 0 &&
     *   not CanWrite()
     */
    inline IOBuffer();

    /**
     * @brief Constructor with the granularity mask input.
     * @pre true
     * @post
     *   Buffer() == NULL &&
     *   GetBufferSize() == 0u &&
     *   AmountLeft() == 0u &&
     *   MaxUsableAmount() == 0u &&
     *   UsedAmountLeft() == 0 &&
     *   Position() == 0u &&
     *   UsedSize() == 0 &&
     *   AllocationGranularity() == allocationGranularity &&
     *   UndoLevel() == newUndoLevel &&
     *   not CanWrite()
     */
    inline IOBuffer(const uint32 allocationGranularity,
            const uint32 newUndoLevel);

    /**
     * @brief Destructor.
     */
    virtual ~IOBuffer();

    /**
     * @brief Allocates dynamically a memory portion on the heap.
     * @details
     * -If the new size (desiredSize-reservedSpaceAtEnd) is minor than the
     * current used size, usedSize is clipped and if the position was over,
     * it becomes the end of the new size.\n
     * -This function fixes maxUsableAmount to desiredSize-reservedSpaceAtEnd.
     *
     * @param[in] desiredSize is the desired size to be allocated.
     * @param[in] reservedSpaceAtEnd is the space allocated over the usable
     * memory area (maxUsableAmount does not consider it).\n
     * It could be for example the zero terminator character at the end of a
     * string.
     * @return false if the allocation fails, true otherwise.
     */
    virtual bool SetBufferHeapMemory(const uint32 desiredSize,
            const uint32 reservedSpaceAtEnd);

    /**
     * @brief Assigns a preallocated memory with read and write access.
     * @details Sets the buffer as empty and maxUsableAmount = (bufferSize - reservedSpaceAtEnd).
     *
     * @param[in] buffer is a pointer to a preallocated memory.
     * @param[in] bufferSize is the size of the memory.
     * @param[in] reservedSpaceAtEnd is the memory allocated over the usable memory area.
     */
    virtual void SetBufferReferencedMemory(char8 * const buffer,
            const uint32 bufferSize,
            const uint32 reservedSpaceAtEnd);

    /**
     * @brief Assigns a preallocated memory only with read access.
     * @details Sets the buffer as empty and maxUsableAmount = (bufferSize - reservedSpaceAtEnd).
     *
     * @param[in] buffer is a pointer to a preallocated memory.
     * @param[in] bufferSize is the size of the memory.
     * @param[in] reservedSpaceAtEnd is the memory allocated but not .
     */
    virtual void SetBufferReadOnlyReferencedMemory(const char8 * const buffer,
            const uint32 bufferSize,
            const uint32 reservedSpaceAtEnd);

    /*---------------------------------------------------------------------------*/

    /**
     * @brief Gets the size of the allocated memory.
     * @return the size of the allocated memory.
     */
    inline uint32 GetBufferSize() const;

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
    inline char8 *BufferReference();

    /**
     * @brief Gets the allocation granularity
     */
    inline uint32 AllocationGranularity() const;

    /**
     * @brief Gets the UndoLevel
     */
    inline uint32 UndoLevel() const;

    /**
     * @brief Returns true if the buffer is writable, false otherwise.
     */
    inline bool CanWrite() const;

    /*---------------------------------------------------------------------------*/

    /**
     * @brief Puts a character on the buffer.
     * @details This function is called by formatted print-like functions for buffered streams. The
     * implementation of NoMoreSpaceToWrite depends on children classes and it could be
     * for example a flush (BufferedStreamIOBuffer) or a new allocation (StreamStringIOBuffer).\n
     *
     * The value of UndoLevel() could be used to trigger the call to NoMoreSpaceToWrite when
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
     * when the cursor is in a specific position defined by UndoLevel().
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
     * @details The function does nothing if CharBuffer::CanWrite returns false,
     * otherwise copy size bytes from the input buffer, sets the cursor size
     * positions forward and adjusts fillLeft and amountLeft accordingly.\n
     * If size is greater than amountLeft is clipped.
     * @param[in] buffer contains the data to be written in this buffer.
     * @param[in,out] size is the number of bytes to be copied. This value will
     * be clipped to the space left if needed.
     * @return false if errors copying data
     */
    virtual bool Write(const char8 * const buffer,
            uint32 &size);

    /**
     * @brief Writes all the size in the argument from an input buffer.
     * @details This function calls Write and NoMoreSpaceToWrite until size
     * is consumed.
     * @param[in] buffer contains data to be written in this buffer.
     * @param[in] size is the number of byte to be copied.
     */
    bool WriteAll(const char8 * buffer,
            const uint32 &size);

    /**
     * @brief Reads from this buffer to an output buffer.
     * @param[out] buffer is the output buffer where data must be written.
     * @param[in,out] size is the number of bytes to be read. This value will
     * be clipped to the space left if needed.
     * @return false if errors copying data
     */
    virtual bool Read(char8 * const buffer,
            uint32 &size);

    /*---------------------------------------------------------------------------*/

    /**
     * @brief The function called by all Printf operations.
     * @details This function read the format, builds the related format
     * descriptor and then calls the PrintToStream function passing the
     * next AnyType element in the list.
     * @param[in] format is a printf-like string format.
     * @param[in] pars is a list of AnyType elements to print.
     * @return false in case of errors.
     */
    bool PrintFormatted(const char8 * format,
            const AnyType pars[]);

    /**
     * @brief Reads a token from the buffer and writes it on an output buffer.
     * @details Extracts a token from the buffer into a string data until a
     * terminator or 0 is found. Skips all skip characters, if you want to
     * skip also terminator characters at the beginning add them to the skip
     * characters.
     * The terminator (just the first encountered) is consumed in the process
     * and saved in saveTerminator if provided skipCharacters=NULL is
     * equivalent to skipCharacters = terminator
     * A character can be found in the terminator or in the skipCharacters
     * list in both or in none
     * 0) none                 the character is copied.
     * 1) terminator           the character is not copied the string
     *                         is terminated.
     * 2) skip                 the character is not copied.
     * 3) skip + terminator    the character is not copied, the string
     *                         is terminated only if not empty.
     * @param[out] outputBuffer is the output buffer.
     * @param[in] terminator is a list of terminator characters.
     * @param[in] outputBufferSize is the size of the output buffer.
     * @param[out] saveTerminator is the found terminator in return.
     * @param[in] skipCharacters is a list of characters to be skipped.
     * @returns true if some data was read before any error or file
     * termination, false only on error and no data available.
     */
    bool GetToken(char8 * outputBuffer,
            const char8 * terminator,
            uint32 outputBufferSize,
            char8 &saveTerminator,
            const char8 * skipCharacters);

    /**
     * @brief Reads a token from the buffer and writes it on an output buffer.
     * @details Extracts a token from the buffer into a string data until a
     * terminator or 0 is found. Skips all skip characters and those that are
     * also terminators at the beginning.
     * The terminator (just the first encountered) is consumed in the process
     * and saved in saveTerminator if provided skipCharacters=NULL is
     * equivalent to skipCharacters = terminator {BUFFERED}
     * A character can be found in the terminator or in the skipCharacters
     * list  in both or in none
     * 0) none                 the character is copied.
     * 1) terminator           the character is not copied the string
     *                         is terminated.
     * 2) skip                 the character is not copied.
     * 3) skip + terminator    the character is not copied, the string
     *                         is terminated if not empty.
     * @param[out] outputBuffer is the output buffer.
     * @param[in] terminator is a list of terminator characters.
     * @param[out] saveTerminator is the found terminator in return.
     * @param[in] skipCharacters is a list of characters to be skipped.
     * @return false if no data read, true otherwise.
     * @return true if some data was read before any error or file termination.
     * false only on error and no data available.
     */
    bool GetToken(IOBuffer & outputBuffer,
            const char8 * terminator,
            char8 &saveTerminator,
            const char8 * skipCharacters);

    /**
     * @brief Skips a number of tokens on the buffer.
     * @param[in,out] count is the number of tokens to be skipped.
     * @param[in] terminator is a list of terminator characters for the tokenize operation.
     * @return false if the number of skipped tokens is minor than the desired.
     */
    bool SkipTokens(uint32 count,
            const char8 * terminator);

    /**
     * @brief User friendly function which simply calls NoMoreDataToRead.
     * @return whatever NoMoreDataToRead returns.
     */
    virtual bool Refill();

    /**
     * @brief User friendly function which simply calls NoMoreSpaceToWrite.
     * @return whatever NoMoreSpaceToWrite returns.
     */
    /*lint -e{1735} this function has the same default parameter of its father.*/
    virtual bool Flush(const uint32 neededSize=0u);

    /**
     * @see NoMoreSpaceToWrite(const uint32)
     */
    virtual bool NoMoreSpaceToWrite();

    /**
     * @brief The routine executed in PutC when amountLeft is <= UndoLevel(),
     * namely the cursor arrived to a specific position.
     * @details This basic implementation only returns false.\n
     *
     * In StreamStringIOBuffer UndoLevel() is zero, so when the cursor arrived
     * at the end of the memory, this function allocated a new portion of
     * memory in the queue.\n
     *
     * In BufferedStreamIOBuffer UndoLevel() is zero, so when the cursor
     * arrived at the end of the memory this function flushes this buffer
     * to the stream.\n
     *
     * @param[in] neededSize is the size of the memory to be allocated or
     * flushed (not used at this implementation level).
     * @return false at this implementation level.
     */
    virtual bool NoMoreSpaceToWrite(uint32 neededSize);

    /**
     * @brief The routine executed in GetC when amountLeft is <= UndoLevel(),
     * namely the cursor arrived to a specific position.
     * @details This implementation is basic and only returns false.
     *
     * In BufferedStreamIOBuffer UndoLevel() is zero, so when the cursor
     * arrives at the end of the memory this function refills the buffer
     * from the stream for a new read operation.
     *
     * @return false in this implementation.
     */
    virtual bool NoMoreDataToRead();

private:

    /**
     * The internal buffer.
     */
    CharBuffer internalBuffer;

    /**
     * The size of the usable memory in the buffer.
     * Usually it is equal to the CharBuffer::GetBufferSize
     * but for example in String considering the final 0
     * the usable memory is CharBuffer::GetBufferSize - 1.
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

IOBuffer::IOBuffer(const uint32 allocationGranularity,
                   const uint32 newUndoLevel) :
        internalBuffer(allocationGranularity) {
    amountLeft = 0u;
    maxUsableAmount = 0u;
    positionPtr = static_cast<char8 *>(NULL);
    fillLeft = 0u;
    undoLevel = newUndoLevel;
}

uint32 IOBuffer::GetBufferSize() const {
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

char8 *IOBuffer::BufferReference() {
    return internalBuffer.BufferReference();
}

uint32 IOBuffer::AllocationGranularity() const {
    return internalBuffer.AllocationGranularity();
}

uint32 IOBuffer::UndoLevel() const {
    return undoLevel;
}

void IOBuffer::Empty() {
    amountLeft = maxUsableAmount;   // Seek 0
    fillLeft = maxUsableAmount;   // SetSize 0
    positionPtr = const_cast<char8 *>(Buffer()); // seek 0
}

/*---------------------------------------------------------------------------*/

bool IOBuffer::PutC(const char8 c) {

    bool retval = (positionPtr != NULL);
    if(retval) {
        // check if buffer needs updating and or saving
        if (amountLeft <= undoLevel) {
            if (!Flush()) {
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

    }

    if (retval) {
        /*lint -e{613} . Justification: The NULL pointer condition is handled*/
        *positionPtr = c;

        /*lint -e{613} . Justification: The NULL pointer condition is handled*/
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
    bool retval = (Position() > 0u) && (positionPtr != NULL);
    if (retval) {

        if (amountLeft == fillLeft) {
            fillLeft++;
        }
        /*lint -e{613} . Justification: The NULL pointer condition is handled*/
        positionPtr--;
        amountLeft++;
    }
    return retval;
}

bool IOBuffer::GetC(char8 &c) {

    bool retval = (positionPtr != NULL);

    if(retval) {
        // check if buffer needs updating and or saving
        if (UsedAmountLeft() <= undoLevel) {
            if (!Refill()) {
                retval = false;
            }

            if (UsedAmountLeft() == 0u) {
                retval = false;
            }
        }

        if (retval) {
            /*lint -e{613} . Justification: The NULL pointer condition is handled*/
            c = *positionPtr;
            /*lint -e{613} . Justification: The NULL pointer condition is handled*/
            positionPtr++;
            amountLeft--;

        }
    }
    return retval;
}

bool IOBuffer::UnGetC() {
    bool retval = (Position() > 0u) && (positionPtr != NULL);

    // can I still do it?
    if (retval) {
        /*lint -e{613} . Justification: The NULL pointer condition is handled*/
        positionPtr--;
        amountLeft++;
    }

    return retval;
}

bool IOBuffer::CanWrite() const {
    return internalBuffer.CanWrite();
}

}

#endif /* IOBUFFER_H_ */
