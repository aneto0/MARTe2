/**
 * @file BufferedStreamGenerator.h
 * @brief Header file for class BufferedStreamGenerator
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

 * @details This header file contains the declaration of the class BufferedStreamGenerator
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BUFFEREDSTREAMGENERATOR_H_
#define BUFFEREDSTREAMGENERATOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "SingleBufferedStream.h"
#include "DoubleBufferedStream.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A class to add buffering support to OperatingSystemStream.
 *
 * @details The functions Read(*), Write(*), Seek(*), RelativeSeek(*), Position(*), Size(*), SetSize(*)
 * wrap the related functions inherited by the buffered stream in order to implement all advanced
 * read write operations defined in BufferedStreamI with buffering support.
 *
 * The functions CanRead(*), CanWrite(*) and CanSeek(*) wrap the operating system stream related functions
 * defining the permissions stream operations.
 *
 * All the other operating system stream functions will be left untouched allowing full access to the specific
 * operating system stream implementation.
 *
 * @tparam bufferedStream a class representing a buffered stream, i.e. a class
 * derived from the abstract class BufferedStreamI.
 * @tparam basicStream a class representing a basic stream, i.e. a class
 * derived from the abstract class StreamI.
 */
template<class bufferedStream, class basicStream>
class BufferedStreamGenerator: public bufferedStream, public basicStream {

public:
    /**
     * @brief Default constructor.
     * @post bufferedStream(), basicStream().
     */
    BufferedStreamGenerator();

    /**
     * @brief Constructor with timeout set.
     * @param[in] timeout The timeout used in all read-write operations.
     * @post bufferedStream(timeout), basicStream().
     */
    BufferedStreamGenerator(uint32 timeout);

    /**
     * @brief Default destructor.
     */
    virtual ~BufferedStreamGenerator();

    /**
     * @brief Wraps bufferedStream::Read(*).
     * @see BufferedStreamI::Read(*).
     */
    virtual bool Read(char8 * const output,
                      uint32 &size);

    /**
     * @brief Performs a buffered read operation with a specific timeout.
     * @param[out] output the buffer in output which will contain the read data.
     * @param[in,out] size the number of bytes to read.
     * @param[in] msecTimeout The timeout used in this operation.
     * @remark The specified timeout will be used only for this operation.
     * To change the timeout used in all read-write operations permanently use SetTimeout(*).
     * @see BufferedStreamI::Read(*).
     */
    virtual bool Read(char8 * const output,
                      uint32 &size,
                      const TimeoutType &msecTimeout);

    /**
     * @brief Wraps bufferedStream::Write(*)
     * @see BufferedStreamI::Write(*).
     */
    virtual bool Write(const char8 * const input,
                       uint32 &size);

    /**
     * @brief Performs a buffered write operation with a specific timeout.
     * @param[in] input the buffer in input which contains the data to be written.
     * @param[in,out] size the number of bytes to write.
     * @param[in] msecTimeout The timeout used in this operation.
     * @remark The specified timeout will be used only for this operation.
     * To change the timeout used in all read-write operations permanently use SetTimeout(*).
     * @see BufferedStreamI::Write(*).
     */
    virtual bool Write(const char8 * const input,
                       uint32 &size,
                       const TimeoutType &msecTimeout);

    /**
     * @brief Wraps bufferedStream::Size(*)
     * @see BufferedStreamI::Size(*).
     */
    virtual uint64 Size();

    /**
     * @brief Wraps bufferedStream::Seek(*)
     * BufferedStreamI::Seek(*).
     */
    virtual bool Seek(uint64 pos);

    /**
     * @brief Wraps bufferedStream::RelativeSeek(*).
     * BufferedStreamI::RelativeSeek(*).
     */
    virtual bool RelativeSeek(const int64 deltaPos);

    /**
     * @brief Wraps bufferedStream::Position(*).
     * BufferedStreamI::Position(*).
     */
    virtual uint64 Position();

    /**
     * @brief Wraps bufferedStream::SetSize(*).
     * BufferedStreamI::SetSize(*).
     */
    virtual bool SetSize(uint64 size);

    /**
     * @brief Wraps basicStream::CanWrite(*).
     * StreamI::CanWrite(*).
     */
    virtual bool CanWrite() const;

    /**
     * @brief Wraps basicStream::CanSeek(*).
     * StreamI::CanSeek(*).
     */
    virtual bool CanSeek() const;

    /**
     * @brief Wraps basicStream::CanRead(*).
     * StreamI::CanRead(*).
     */
    virtual bool CanRead() const;

protected:

    /**
     * @brief Wraps the basicStream::Size(*)
     */
    virtual uint64 OSSize();

    /**
     * @brief Wraps the basicStream::Seek(*)
     */
    virtual bool OSSeek(uint64 pos);

    /**
     * @brief Wraps the basicStream::RelativeSeek(*)
     */
    virtual bool OSRelativeSeek(int64 deltaPos);

    /**
     * @brief Wraps the basicStream::Position(*)
     */
    virtual uint64 OSPosition();

    /**
     * @brief Wraps the basicStream::SetSize(*)
     */
    virtual bool OSSetSize(uint64 size);

    /**
     * @brief Wraps the basicStream::Read(*) with the timeout set using
     * bufferedStream::SetTimeout(*)
     */
    virtual bool OSRead(char8 * const data,
                        uint32 &size);

    /**
     * @brief Wraps the basicStream::Write(*) with the timeout set using
     * bufferedStream::SetTimeout(*)
     */
    virtual bool OSWrite(const char8 * const data,
                         uint32 &size);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<class bufferedStream, class basicStream>
BufferedStreamGenerator<bufferedStream, basicStream>::BufferedStreamGenerator() :
bufferedStream(),
basicStream() {
}

template<class bufferedStream, class basicStream>
BufferedStreamGenerator<bufferedStream, basicStream>::BufferedStreamGenerator(uint32 timeout) :
        bufferedStream(timeout),
        basicStream(){
}

template<class bufferedStream, class basicStream>
BufferedStreamGenerator<bufferedStream, basicStream>::~BufferedStreamGenerator() {
}

template<class bufferedStream, class basicStream>
bool BufferedStreamGenerator<bufferedStream, basicStream>::Read(char8 * const output,
                                                                uint32 &size) {
    return bufferedStream::Read(output, size);
}

template<class bufferedStream, class basicStream>
bool BufferedStreamGenerator<bufferedStream, basicStream>::Read(char8 * const output,
                                                                uint32 &size,
                                                                const TimeoutType &msecTimeout) {
    return bufferedStream::Read(output, size, msecTimeout);

}

template<class bufferedStream, class basicStream>
bool BufferedStreamGenerator<bufferedStream, basicStream>::Write(const char8 * const input,
                                                                 uint32 &size) {
    return bufferedStream::Write(input, size);
}

template<class bufferedStream, class basicStream>
bool BufferedStreamGenerator<bufferedStream, basicStream>::Write(const char8 * const input,
                                                                 uint32 &size,
                                                                 const TimeoutType &msecTimeout) {
    return bufferedStream::Write(input, size, msecTimeout);

}

template<class bufferedStream, class basicStream>
uint64 BufferedStreamGenerator<bufferedStream, basicStream>::Size() {
    return bufferedStream::Size();
}

template<class bufferedStream, class basicStream>
bool BufferedStreamGenerator<bufferedStream, basicStream>::Seek(uint64 pos) {
    return bufferedStream::Seek(pos);
}

template<class bufferedStream, class basicStream>
bool BufferedStreamGenerator<bufferedStream, basicStream>::RelativeSeek(const int64 deltaPos) {
    return bufferedStream::RelativeSeek(deltaPos);
}

template<class bufferedStream, class basicStream>
uint64 BufferedStreamGenerator<bufferedStream, basicStream>::Position() {
    return bufferedStream::Position();
}

template<class bufferedStream, class basicStream>
bool BufferedStreamGenerator<bufferedStream, basicStream>::SetSize(uint64 size) {
    return bufferedStream::SetSize(size);
}

template<class bufferedStream, class basicStream>
bool BufferedStreamGenerator<bufferedStream, basicStream>::CanWrite() const {
    return basicStream::CanWrite();
}

template<class bufferedStream, class basicStream>
bool BufferedStreamGenerator<bufferedStream, basicStream>::CanSeek() const {
    return basicStream::CanSeek();
}

template<class bufferedStream, class basicStream>
bool BufferedStreamGenerator<bufferedStream, basicStream>::CanRead() const {
    return basicStream::CanRead();
}

template<class bufferedStream, class basicStream>
uint64 BufferedStreamGenerator<bufferedStream, basicStream>::OSSize() {
    return basicStream::Size();
}

template<class bufferedStream, class basicStream>
bool BufferedStreamGenerator<bufferedStream, basicStream>::OSSeek(uint64 pos) {
    return basicStream::Seek(pos);
}

template<class bufferedStream, class basicStream>
bool BufferedStreamGenerator<bufferedStream, basicStream>::OSRelativeSeek(int64 deltaPos) {
    return basicStream::RelativeSeek(deltaPos);
}

template<class bufferedStream, class basicStream>
uint64 BufferedStreamGenerator<bufferedStream, basicStream>::OSPosition() {
    return basicStream::Position();
}

template<class bufferedStream, class basicStream>
bool BufferedStreamGenerator<bufferedStream, basicStream>::OSSetSize(uint64 size) {
    return basicStream::SetSize(size);
}

template<class bufferedStream, class basicStream>
bool BufferedStreamGenerator<bufferedStream, basicStream>::OSRead(char8 * const data,
                                                                  uint32 &size) {
    return basicStream::Read(data, size, bufferedStream::GetTimeout());
}

template<class bufferedStream, class basicStream>
bool BufferedStreamGenerator<bufferedStream, basicStream>::OSWrite(const char8 * const data,
                                                                   uint32 &size) {

    return basicStream::Write(data, size, bufferedStream::GetTimeout());
}
}

#endif /* BUFFEREDSTREAMGENERATOR_H_ */

