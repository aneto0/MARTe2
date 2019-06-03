/**
 * @file VoidStream.h
 * @brief Header file for class AnyType
 * @date 3 Jun 2019
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef SOURCE_CORE_BAREMETAL_L0TYPES_VOIDSTREAM_H_
#define SOURCE_CORE_BAREMETAL_L0TYPES_VOIDSTREAM_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StreamI.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief a stream with no capabilities
 * Useful to inherit from and specialise
 */
class DLL_API VoidStream: public StreamI{
public:
	/**
	 * @brief construct this object by borrowing the CString tool.
	 */
	VoidStream();

	/**
	 * @brief construct this object by borrowing the CString tool.
	 */
	virtual ~VoidStream();

	/**
     * @brief Queries if the stream is writable, i.e. if write operations can
     * be performed on the stream.
     * @return true if the stream is writable.
     */
    virtual bool CanWrite() const;

    /**
     * @brief Queries if the stream is readable, i.e. if read operations can
     * be performed on the stream.
     * @return if the stream is readable.
     */
    virtual bool CanRead() const;

    /**
     * @brief Queries if the stream is seekable, i.e. if seek operations can
     * be performed on the stream.
     * @return true if the stream is seekable.
     */
    virtual bool CanSeek() const;

    /**
     * @brief Reads data from the stream.
     * @details Reads up to \a size bytes into \a output. The actual read size
     * is returned in \a size.
     * @param[out] output the buffer where to write the data into.
     * @param[in,out] size the number of bytes to read. Upon return of the
     * function \a size contains the number of bytes actually read.
     * @pre CanRead()
     * @post Position() == this'old->Position() + size
     * @return true if \a size bytes are successfully read from the stream and
     * written into \a output.
     */
    virtual bool Read(char8 * const output,uint32 & size);

    /**
     * @brief Writes data into the stream.
     * @details Writes up to \a size bytes into \a input. The actual written
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
    virtual bool Write(const char8 * const input, uint32 & size);

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
     * @param[out] output is the buffer where data must be copied
     * from the stream.
     * @param[in,out] size is the desired number of bytes to read.
     * @param[in] timeout is the desired timeout.
     */
    virtual bool Read(char8 * const output,
            uint32 & size,
            const MilliSeconds &timeout);

    /**
     * @brief Writes from a const char8* buffer to the stream.
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
     * @param[in] input contains the data which must be copied
     * on the stream.
     * @param[in,out] size is the desired number of bytes to write.
     * @param[in] timeout is the desired timeout.
     */
    virtual bool Write(const char8 * const input,
            uint32 & size,
            const MilliSeconds &timeout);

    /**
     * @brief Gets the size of the stream.
     * @return the size of the stream.
     */
    virtual uint64 Size() const;

    /**
     * @brief Moves within the stream to an absolute location.
     * @param[in] pos the desired absolute position.
     * @return true if the stream is successfully moved to \a pos.
     * @pre CanSeek()
     * @post
     *   pos <= Size() => Position() == pos &&
     *   pos > Size() => Position() == Size()
     */
    virtual bool Seek(uint64 pos);

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
     * @remark The deltaPos is a signed integer, so it will always have a
     * half addressable space with respect to its unsigned counterpart, i.e.
     * the Seek() method.
     */
    virtual bool RelativeSeek(const int64 deltaPos);

    /**
     * @brief Gets the current position.
     * @return the current position in the stream.
     */
    virtual uint64 Position() const;

    /**
     * @brief Clips the stream size.
     * @param size the new size of the stream.
     * @return true if the size of the stream is set to \a size.
     * @pre true
     * @post Size() == size
     */
    virtual bool SetSize(uint64 size);

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_VOIDSTREAM_H_ */
