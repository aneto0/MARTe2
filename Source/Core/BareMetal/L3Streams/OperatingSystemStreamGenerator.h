/**
 * @file OperatingSystemStreamGenerator.h
 * @brief Header file for class OperatingSystemStreamGenerator
 * @date 23/10/2015
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

 * @details This header file contains the declaration of the class OperatingSystemStreamGenerator
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OPERATINGSYSTEMSTREAMGENERATOR_H_
#define OPERATINGSYSTEMSTREAMGENERATOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * Wraps the Basic stream to inherits from StreamI.
 */
template<class basicStream>
class OperatingSystemStreamGenerator: public StreamI, public basicStream {

public:

    OperatingSystemStreamGenerator();

    virtual ~OperatingSystemStreamGenerator() {

    }

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
     * @detail Reads up to \a size bytes into \a output. The actual read size
     * is returned in \a size.
     * @param[out] output the buffer where to write the data into.
     * @param[in,out] size the number of bytes to read. Upon return of the
     * function \a size contains the number of bytes actually read.
     * @pre CanRead()
     * @post Position() == this'old->Position() + size
     * @return true if \a size bytes are successfully read from the stream and
     * written into \a output within the specified timeout (see SetTimeout).
     */
    virtual bool Read(char8 * const output,
                      uint32 & size);

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
     * written into the stream within the specified timeout (see SetTimeout).
     */
    virtual bool Write(const char8 * const input,
                       uint32 & size);

    /**
     * @brief Gets the size of the stream.
     * @return the size of the stream.
     */
    virtual uint64 Size();

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
     * @warning (1) The deltaPos is a signed integer, so it will always have a
     * half addressable space with respect to its unsigned counterpart, i.e.
     * the Seek() method.
     * @warning (2) Moreover, the deltaPos is a 32 bits integer, less than the
     * 64 bits integer used in Seek(), so it will have a shorter addressable
     * space, anyway.
     */
    virtual bool RelativeSeek(const int32 deltaPos);

    /**
     * @brief Gets the current position.
     * @return the current position in the stream.
     */
    virtual uint64 Position();

    /**
     * @brief Clips the stream size.
     * @param size the new size of the stream.
     * @return true if the size of the stream is set to \a size.
     * @pre true
     * @post Size() == size
     */
    virtual bool SetSize(uint64 size);

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
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<class basicStream>
OperatingSystemStreamGenerator<basicStream>::OperatingSystemStreamGenerator() :
        StreamI(),
        basicStream() {

}

template<class basicStream>
bool OperatingSystemStreamGenerator<basicStream>::CanWrite() const {
    return basicStream::CanWrite();
}

template<class basicStream>
bool OperatingSystemStreamGenerator<basicStream>::CanRead() const {
    return basicStream::CanRead();
}

template<class basicStream>
bool OperatingSystemStreamGenerator<basicStream>::CanSeek() const {
    return basicStream::CanSeek();
}

template<class basicStream>
bool OperatingSystemStreamGenerator<basicStream>::Read(char8 * const output,
                                                       uint32 & size) {
    return basicStream::Read(output, size, TTInfiniteWait);
}

template<class basicStream>
bool OperatingSystemStreamGenerator<basicStream>::Write(const char8 * const input,
                                                        uint32 & size) {
    return basicStream::Write(output, size, TTInfiniteWait);
}

template<class basicStream>
uint64 OperatingSystemStreamGenerator<basicStream>::Size() {
    return basicStream::Size();
}

template<class basicStream>
bool OperatingSystemStreamGenerator<basicStream>::Seek(uint64 pos) {
    return basicStream::Seek(pos);
}

template<class basicStream>
bool OperatingSystemStreamGenerator<basicStream>::RelativeSeek(const int32 deltaPos) {
    return basicStream::RelativeSeek(deltaPos);
}

template<class basicStream>
uint64 OperatingSystemStreamGenerator<basicStream>::Position() {
    return basicStream::Position();
}

template<class basicStream>
bool OperatingSystemStreamGenerator<basicStream>::SetSize(uint64 size) {
    return basicStream::SetSize();
}

template<class basicStream>
IOBuffer *OperatingSystemStreamGenerator<basicStream>::GetReadBuffer() {
    return static_cast<IOBuffer *>(NULL);
}

template<class basicStream>
IOBuffer *OperatingSystemStreamGenerator<basicStream>::GetWriteBuffer() {
    return static_cast<IOBuffer *>(NULL);
}


}
#endif /* OPERATINGSYSTEMSTREAMGENERATOR_H_ */

