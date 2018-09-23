/**
 * @file StreamMemoryReference.h
 * @brief Header file for class StreamMemoryReference
 * @date 06/10/2015
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

 * @details This header file contains the declaration of the class StreamMemoryReference
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STREAMMEMORYREFERENCE_H_
#define STREAMMEMORYREFERENCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "BufferedStreamI.h"
#include "IOBuffer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A basic implementation of a stream which allows reading and writing
 * to an underlying bounded and preallocated memory location.
 */
class DLL_API StreamMemoryReference: public BufferedStreamI {

public:

    /**
     * @brief Default constructor
     * @pre
     *   true
     * @post
     *   not CanRead() &&
     *   not CanWrite() &&
     *   CanSeek() &&
     *   Position() == 0 &&
     *   Size() == 0
     */
    StreamMemoryReference();

    /**
     * @brief Constructor from a preallocated read/write buffer
     * @details This constructor binds the object to a memory area in
     * read/write mode. It also assumes that the area of memory is empty
     * and therefore the stream's size is 0.
     * @param[in] bufferIn is the char8 pointer of the preallocated read/write memory.
     * @param[in] bufferSize is the usable size of the buffer.
     * @pre
     *   bufferIn != NULL
     *   bufferSize > 0
     * @post
     *   CanRead() &&
     *   CanWrite() &&
     *   CanSeek() &&
     *   Position() == 0 &&
     *   Size() == 0
     * @warning The buffer pointer is an input parameter to the constructor,
     * but the pointee will be accessed for reading and writing by the other
     * methods of the class.
     */
    StreamMemoryReference(char8 * const bufferIn,
                          const uint32 bufferSize);

    /**
     * @brief Constructor from a preallocated read only buffer
     * @details This constructor binds the object to a memory area in
     * read only mode. It also assumes that the area of memory is full
     * and therefore the stream's size is bufferSize.
     * @param[in] bufferIn is the const char8 pointer to a preallocated read only memory.
     * @param[in] bufferSize is the usable size of the buffer.
     * @pre
     *   bufferIn != NULL
     *   bufferSize > 0
     * @post
     *   CanRead() &&
     *   not CanWrite() &&
     *   CanSeek() &&
     *   Position() == 0 &&
     *   Size() == bufferSize
     * @warning The buffer pointer is an input parameter to the constructor,
     * but the pointee will be accessed (read only) by the other methods of
     * the class.
     */
    StreamMemoryReference(const char8 * const bufferIn,
                          const uint32 bufferSize);

    /**
     * @brief Destructor
     */
    virtual ~StreamMemoryReference();

    /**
     *  @brief Automatic cast to AnyType as a const char8 passing Buffer() return value.
     */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2]. Justification: The StreamMemoryReference is considered as a string and not as a standard stream*/
    operator AnyType() const;

    /*-----------------------------------------------------------------------*/

    /**
     * @brief Reads data into a buffer.
     * @param[out] output is the buffer where data must be written to.
     * @param[in,out] size is the number of bytes to copy. This value will be
     * updated with the bytes actually read.
     * @return false if errors on copying data
     */
    virtual bool Read(char8 * const output,
                      uint32 & size);

    /**
     * @brief Write from a buffer to the string.
     * @param[in] input is the buffer with the data to be written from.
     * @param[in,out] size is the number of bytes to copy. This value will be
     * updated with the bytes actually written.
     * @return false if errors on copying data
     */
    virtual bool Write(const char8 * const input,
                       uint32 & size);

    /**
     * @brief Default implementation for buffered streams: calls StreamI::Read(*)
     * with infinite timeout.
     * @see StreamI::Read(*).
     */
    virtual bool Read(char8 * const output,
                      uint32 & size,
                      const MilliSeconds &timeout);

    /**
     * @brief Default implementation for buffered streams: calls StreamI::Write(*)
     * with infinite timeout.
     * @see StreamI::Write(*)
     */
    virtual bool Write(const char8 * const input,
                       uint32 & size,
                       const MilliSeconds &timeout);

    /**
     * @brief Queries if the stream is writable.
     * @return true if the stream is writable.
     */
    virtual bool CanWrite() const;

    /**
     * @brief Queries if the stream is readable.
     * @return true if the stream is readable.
     */
    virtual bool CanRead() const;

    /**
     * @brief Queries if seek operations can be performed on the stream.
     * @return true.
     */
    virtual bool CanSeek() const;

    /**
     * @brief Gets the size of the stream.
     * @details For this object the size is always minor than the buffer
     * dimension passed in the constructor.
     * @return the current stream size.
     */
    virtual uint64 Size() const ;

    /**
     * @brief Moves within the stream to an absolute location.
     * @param[in] pos is the desired absolute position.
     * @return false in case of cursor out of ranges or other errors.
     */
    virtual bool Seek(const uint64 pos);

    /**
     * @brief Moves within the stream relative to current location.
     * @details Checks that the final position is >= 0 and <= UsedSize, then moves the cursor.\n
     * -If the final position is < 0 moves the cursor at the beginning.\n
     * -If the final position is > UsedSize moves the cursor at the end.\n
     * @param[in] deltaPos is the gap from the current position.
     * @return false if the position falls out of bounds.or if \a deltaPos is outside the int32
     * range of values (supported by IOBuffer::RelativeSeek(*)).
     * @pre
     *   (deltaPos <= MAX_INT32) && (deltaPos >= MIN_INT32)
     */
    virtual bool RelativeSeek(const int64 deltaPos);

    /**
     * @brief Gets the current position.
     * @return the current position.
     */
    virtual uint64 Position() const ;

    /**
     * @brief Sets the used size.
     * @details Setting the size manually you can read until that position.
     * @param[in] size is the desired stream size.
     * @return true.
     */
    virtual bool SetSize(const uint64 size);

    /*-----------------------------------------------------------------------*/

    /**
     * @brief Gets a pointer to the beginning of the internal buffer with read
     * only access.
     * @return The pointer to the internal buffer.
     */
    inline const char8 *Buffer() const;

    /**
     * @brief Gets a pointer to the beginning of the internal buffer with read
     * write access.
     * @return The pointer to the internal buffer.
     */
    inline char8 *BufferReference();

    /**
     * @brief Gets a pointer to the tail of the internal buffer with read only
     * access.
     * @param[in] ix the offset from the end of buffer.
     * @return pointer to the tail of the intenal buffer.
     * @pre ix >= 0 && ix < Size().
     */
    inline const char8 *Tail(const uint32 ix) const;

protected:

    /**
     * @brief Gets the read buffer.
     * @return a pointer to the read buffer.
     */
    virtual IOBuffer *GetReadBuffer();

    /**
     * @brief Gets the write buffer.
     * @return a pointer to the write buffer.
     */
    virtual IOBuffer *GetWriteBuffer();

private:

    /**
     * Simply a normal IOBuffer. Nothing is overloaded.
     * It is allocated by IOBuffer::SetBufferReferencedMemory or IOBuffer::SetBufferReadOnlyReferencedMemory
     * @see IOBuffer.cpp for the naked implementation of this buffer.
     */
    IOBuffer buffer;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

const char8 *StreamMemoryReference::Buffer() const {
    return buffer.Buffer();
}

char8 *StreamMemoryReference::BufferReference() {
    return buffer.BufferReference();
}

const char8 *StreamMemoryReference::Tail(const uint32 ix) const {
    const char8* result;
    bool ok = (ix <= (buffer.UsedSize() - 1u));
    if (ok) {
        const char8* bufref = buffer.Buffer();
        result = &(bufref[(buffer.UsedSize() - ix) - 1u]);
    }
    else {
        result = static_cast<const char8 *>(NULL);
    }
    return result;
}

}

#endif /* STREAMMEMORYREFERENCE_H_ */

