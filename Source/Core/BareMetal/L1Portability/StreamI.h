/**
 * @file StreamI.h
 * @brief Header file for class StreamI
 * @date 05/10/2015
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

 * @details This header file contains the declaration of the class StreamI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STREAMI_H_
#define STREAMI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TimeoutType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Abstract super class for all streams.
 *
 * @details This class provides the common interface specification for all the
 * streams.
 *
 * The stream represents a sequence of bytes which can be mapped to arrays,
 * strings, files, sockets, and so on, but that it can be accessed uniformly
 * using a common read/write interface. A stream is also meant to be able to
 * have the notion of cursor, which can be positioned randomly on any byte of
 * the stream; size, which allows to know the actual size of the stream; and
 * timeout, which allows to define a time out for read and write operations,
 * so they will not last for ever in case of I/O problems.
 *
 * Attributes:
 * - Size: The size of the stream, i.e. the number of bytes that it holds.
 * - Position: The position of the cursor, i.e. the zero-based index of the
 *   byte where the next read/write operation will occur.
 *
 * Operations:
 * - Read
 * - Write
 * - Seek
 *
 * (*) The class supplies a standard implementation for these operations.
 *
 * @warning The generic capabilities for reading, writing, and seeking can not
 * assured to be available on all kind of mapped streams, so an additional set
 * of "CanXXX" methods will advise the user what can be done with the stream.
 *
 * - CanRead: Queries if the stream is readable, i.e. if the operation Read
 *   is implemented.
 * - CanWrite: Queries if the stream is writable, i.e. if the operation Write
 *   is implemented.
 * - CanSeek: Queries if the stream is seekable, i.e. if the operations Seek
 *   and RelativeSeek are implemented.
 *
 * @warning The size and position attributes can be meaningless in some kind
 * of mapped streams, so in these cases, size and position will be set to the
 * maximum value of uint64. This also implies that operations like read and
 * write, which increments position and/or size, will saturate them at the
 * maximum value on these cases.
 */
class DLL_API StreamI {

public:

    /**
     * Default constructor
     */
    StreamI() {
    }

    /**
     * @brief Default destructor
     */
    virtual ~StreamI() {

    }

    /**
     * @brief Queries if the stream is writable, i.e. if write operations can
     * be performed on the stream.
     * @return true if the stream is writable.
     */
    virtual bool CanWrite() const = 0;

    /**
     * @brief Queries if the stream is readable, i.e. if read operations can
     * be performed on the stream.
     * @return if the stream is readable.
     */
    virtual bool CanRead() const = 0;

    /**
     * @brief Queries if the stream is seekable, i.e. if seek operations can
     * be performed on the stream.
     * @return true if the stream is seekable.
     */
    virtual bool CanSeek() const = 0;

    /**
     * @brief Reads data from the stream.
     * @detail Reads up to \a size bytes into \a output. The actual read size
     * is returned in \a size.
     * @param[out] output the buffer where to write the data into.
     * @param[in,out] size the number of bytes to read. Upon return of the
     * function \a size contains the number of bytes actually read.
     * @pre CanRead()
     * @post Position() == this'old->Position() + size
     * @return true if \a size bytes are successfully read from the stream and
     * written into \a output.
     */
    virtual bool Read(char8 * const output,
            uint32 & size) = 0;

    /**
     * @brief Writes data into the stream.
     * @detail Writes up to \a size bytes into \a input. The actual written
     * size is returned in \a size.
     * @param[in] input the buffer where to read the data from.
     * @param[in,out] size the number of bytes to write. Upon return of the
     * function \a size contains the number of bytes actually written.
     * @pre CanWrite()
     * @post Position() == this'old->Position() + size &&
     *       this'old->Position() + size > Size() => Size() == Position()
     * @return true if \a size bytes are successfully read from \a input and
     * written into the stream.
     */
    virtual bool Write(const char8 * const input,
            uint32 & size) = 0;

    /**
     * @brief Reads data from the stream to a char8* buffer.
     * @details As much as size byte are read, actual read size is returned
     * in size. (unless complete = true)
     * timeout is how much the operation should last - no more - if not
     * any (all) data read then return false
     * timeout behaviour depends on class characteristics and sync mode.
     * return false implies failure to comply with minimum requirements:
     *   timeout and complete and data read  != size
     *   timeout and data read == 0
     *   error in the stream  ==> no point to try again
     *   parameters error, for instance buffer = NULL
     * Note: The behaviour depends on derived classes implementation.
     * @param[out] buffer is the buffer where data must be copied
     * from the stream.
     * @param[in,out] size is the desired number of bytes to read.
     * @param[in] timeout is the desired timeout.
     */
    virtual bool Read(char8 * const output,
            uint32 & size,
            const TimeoutType &timeout) = 0;

    /**
     * @brief Writes from a const char8* buffer to the stream.
     * @param[in] buffer contains the data which must be copied
     * on the stream.
     * @param[in,out] size is the desired number of bytes to write.
     * @param[in] timeout is the desired timeout.
     *
     * @details As much as size byte are written, actual written size is
     * returned in size.
     * timeout is how much the operation should last.
     * timeout behaviour depends on class characteristics and sync mode.
     * return false implies failure to comply with minimum requirements:
     *   timeout and complete and data written  != size
     *   timeout and data written == 0
     *   error in the stream ==> no point to try again
     *   parameters error, for instance buffer = NULL
     * Note: The behaviour depends by derived classes implementation.
     */
    virtual bool Write(const char8 * const input,
            uint32 & size,
            const TimeoutType &timeout) = 0;

    /**
     * @brief Gets the size of the stream.
     * @return the size of the stream.
     */
    virtual uint64 Size() = 0;

    /**
     * @brief Moves within the stream to an absolute location.
     * @param[in] pos the desired absolute position.
     * @return true if the stream is successfully moved to \a pos.
     * @pre CanSeek()
     * @post
     *   pos <= Size() => Position() == pos &&
     *   pos > Size() => Position() == Size()
     */
    virtual bool Seek(uint64 pos) = 0;

    /**
     * @brief Moves within the stream to a position that is relative to the
     * current location.
     * @details Checks that the final position is >= 0 and <= UsedSize, then
     * moves the cursor.\n
     * -If the final position is < 0 moves the cursor at the beginning.\n
     * -If the final position is > UsedSize moves the cursor at the end.\n
     * @param[in] deltaPos is the distance from the current position.
     * @return true if the stream is successfully moved to \a deltaPos.
     * @pre CanSeek()
     * @post
     *   pos + deltaPos < 0 => Position() == 0 &&
     *   pos + deltaPos <= Size() =>
     *   Position() == this'old->Position() + deltaPos &&
     *   pos + deltaPos > Size() => Position() == Size()
     * @warning (1) The deltaPos is a signed integer, so it will always have a
     * half addressable space with respect to its unsigned counterpart, i.e.
     * the Seek() method.
     */
    virtual bool RelativeSeek(const int64 deltaPos) = 0;

    /**
     * @brief Gets the current position.
     * @return the current position in the stream.
     */
    virtual uint64 Position() = 0;

    /**
     * @brief Clips the stream size.
     * @param size the new size of the stream.
     * @return true if the size of the stream is set to \a size.
     * @pre true
     * @post Size() == size
     */
    virtual bool SetSize(uint64 size) = 0;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BUFFEREDSTREAM_H_ */

