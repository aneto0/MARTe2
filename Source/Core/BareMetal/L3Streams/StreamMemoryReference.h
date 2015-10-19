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
#include "StreamI.h"
#include "IOBuffer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A basic implementation of a stream which allows reading and writing
 * to an underlying memory location.
 */
class StreamMemoryReference: public StreamI {

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
     * @param[in] buffer is the char8 pointer of the preallocated read/write memory.
     * @param[in] bufferSize is the usable size of the buffer.
     * @warning The buffer pointer is an input parameter to the constructor,
     * but the pointee will be accessed for reading and writing by the other
     * methods of the class.
     * @pre
     *   bufferIn != NULL
     *   bufferSize > 0
     * @post
     *   CanRead() &&
     *   CanWrite() &&
     *   CanSeek() &&
     *   Position() == 0 &&
     *   Size() == 0
     */
    StreamMemoryReference(char8 * const bufferIn,
                          const uint32 bufferSize);

    /**
     * @brief Constructor from a preallocated read only buffer
     * @details This constructor binds the object to a memory area in
     * read only mode. It also assumes that the area of memory is full
     * and therefore the stream's size is bufferSize.
     * @param[in] buffer is the const char8 pointer to a preallocated read only memory.
     * @param[in] bufferSize is the usable size of the buffer.
     * @warning The buffer pointer is an input parameter to the constructor,
     * but the pointee will be accessed (read only) by the other methods of
     * the class.
     * @pre
     *   bufferIn != NULL
     *   bufferSize > 0
     * @post
     *   CanRead() &&
     *   not CanWrite() &&
     *   CanSeek() &&
     *   Position() == 0 &&
     *   Size() == bufferSize
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
     * @brief Gets the size of the stream.
     * @details For this object the size is always minor than the buffer
     * dimension passed in the constructor.
     * @return the current stream size.
     */
    virtual uint64 Size();

    /**
     * @brief Moves within the stream to an absolute location.
     * @param[in] pos is the desired absolute position.
     * @return false in case of cursor out of ranges or other errors.
     * @post
     *   pos <= Size() => Position() == pos
     *   pos > Size() => Position() == Size()
     */
    virtual bool Seek(const uint64 pos);

    /**
     * @brief Moves within the stream relative to current location.
     * @details Checks that the final position is >= 0 and <= UsedSize, then moves the cursor.\n
     * -If the final position is < 0 moves the cursor at the beginning.\n
     * -If the final position is > UsedSize moves the cursor at the end.\n
     * @param[in] deltaPos is the gap from the current position.
     * @return false if the position falls out of bounds.
     */
    virtual bool RelativeSeek(const int32 deltaPos);

    /**
     * @brief Gets the current position.
     * @return the current position.
     */
    virtual uint64 Position();

    /**
     * @brief Sets the used size.
     * @details Setting the size manually you can read until that position.
     * @param[in] size is the desired stream size.
     * @return true.
     */
    virtual bool SetSize(const uint64 size);

    /**
     * @brief Queries if seek operations can be performed on the stream.
     * @return true.
     */
    virtual bool CanSeek() const;

    /*-----------------------------------------------------------------------*/

    /**
     * @brief Writes without buffering.
     * @param[in] data the array of bytes to write.
     * @param[in,out] size as input is the number of bytes to write. In output the number of bytes actually written.
     * @return true if \a size bytes of data are successfully written within the specified \a timeout (see SetTimeout).
     */
    virtual bool UnbufferedWrite(const char8 * const data,
                                 uint32 & size);

    /**
     * @brief Reads without buffering.
     * @param[out] data destination array where the read data will be put.
     * @param[in,out] size as input is the number of bytes to read. In output the number of bytes actually read.
     * @return true if \a size bytes of data are successfully read within the specified \a timeout (see SetTimeout).
     */
    virtual bool UnbufferedRead(char8 * const data,
                                uint32 & size);

    /**
     * @brief Retrieves the size of the low-level, unbuffered, stream implementation.
     * @return the size of the low-level stream.
     */
    virtual uint64 UnbufferedSize();

    /**
     * @brief Moves within the low-level, unbuffered, stream implementation to an absolute location.
     * @param[in] pos the desired absolute position.
     * @return true if the stream is successfully moved to \a pos.
     * @post
     *   UnbufferedPosition() == pos
     */
    virtual bool UnbufferedSeek(uint64 pos);

    /**
     * @brief Moves within the low-level, unbuffered, stream to a position that is relative to the current location.
     * @param[in] deltaPos is the distance from the current position.
     * @return true if the stream is successfully moved to \a deltaPos.
     * @post
     *   UnbufferedPosition() == this'old->UnbufferedPosition() + deltaPos
     */
    virtual bool UnbufferedRelativeSeek(int32 deltaPos);

    /**
     * @brief Gets the current position in the low-level, unbuffered, stream.
     * @return the current position in the low-level stream.
     */
    virtual uint64 UnbufferedPosition();

    /**
     * @brief Clips the low-level, unbuffered, stream size.
     * @param size the new size of the low-level stream.
     * @return true if the size of the low-level stream is set to \a size.
     * @post
     *   UnbufferedSize() == size
     */
    virtual bool UnbufferedSetSize(uint64 size);

    /*---------------------------------------------------------------------------*/

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
    inline char8 *BufferReference() const;

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
    virtual IOBuffer *GetInputBuffer();

    /**
     * @brief Gets the write buffer.
     * @return a pointer to the write buffer.
     */
    virtual IOBuffer *GetOutputBuffer();

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

char8 *StreamMemoryReference::BufferReference() const {
    return buffer.BufferReference();
}

const char8 *StreamMemoryReference::Tail(const uint32 ix) const {
    bool ok = (ix <= (buffer.UsedSize() - 1u));

    return (ok) ? (&(buffer.BufferReference()[(buffer.UsedSize() - ix) - 1u])) : static_cast<const char8 *>(NULL);
}

}

#endif /* STREAMMEMORYREFERENCE_H_ */

