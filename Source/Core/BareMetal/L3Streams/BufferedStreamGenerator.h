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

namespace MARTe{

template<class bufferedStream, class basicStream>
class BufferedStreamGenerator: public bufferedStream, public basicStream {

public:
    BufferedStreamGenerator() :
            bufferedStream(),
            basicStream() {
    }

    BufferedStreamGenerator(uint32 timeout) :
            bufferedStream(timeout),
            basicStream() {
    }

    virtual ~BufferedStreamGenerator() {
    }

    /**
     * @brief Calls the buffered read.
     */
    virtual bool Read(char8 * const outBuffer,
                      uint32 &inSize) {
        return bufferedStream::Read(outBuffer, inSize);
    }

    /**
     * @brief Performs a read operation with the specified timeout.
     * To change the timeout permanently (also for printf, get token ecc) use SetTimeout.
     */
    virtual bool Read(char8 * const outBuffer,
                      uint32 &inSize,
                      TimeoutType msecTimeout) {
        TimeoutType prevTimeout = bufferedStream::GetTimeout();
        bufferedStream::SetTimeout(msecTimeout);
        bool ret = bufferedStream::Read(outBuffer, inSize);
        bufferedStream::SetTimeout(prevTimeout);
        return ret;
    }

    /**
     * @brief Calls the buffered write.
     */
    virtual bool Write(const char8 * const outBuffer,
                       uint32 &inSize) {
        return bufferedStream::Write(outBuffer, inSize);
    }

    /**
     * @brief Performs a write operation with the specified timeout.
     * To change the timeout permanently (also for printf, get token ecc) use SetTimeout.
     */
    virtual bool Write(const char8 * const outBuffer,
                       uint32 &inSize,
                       TimeoutType msecTimeout) {
        TimeoutType prevTimeout = bufferedStream::GetTimeout();
        bufferedStream::SetTimeout(msecTimeout);
        return bufferedStream::Write(outBuffer, inSize);
        bufferedStream::SetTimeout(prevTimeout);
        return ret;

    }

    /**
     * @brief Calls buffered size.
     */
    virtual uint64 Size() {
        return bufferedStream::Size();
    }

    /**
     * @brief Calls buffered seek.
     */
    virtual bool Seek(uint64 pos) {
        return DoubleBufferedStream::Seek(pos);
    }

    /**
     * @brief Calls buffered relative seek.
     */
    virtual bool RelativeSeek(const int32 deltaPos) {
        return bufferedStream::RelativeSeek(deltaPos);
    }

    /**
     * @brief Calls buffered position.
     */
    virtual uint64 Position() {
        return bufferedStream::Position();
    }

    /**
     * @brief Calls buffered set size.     */
    virtual bool SetSize(uint64 size) {
        return bufferedStream::SetSize(size);
    }

    /**
     * @brief Calls basic can write.
     */
    virtual bool CanWrite() const {
        return basicStream::CanWrite();
    }

    /**
     * @brief Calls basic can seek.
     */
    virtual bool CanSeek() const {
        return basicStream::CanSeek();
    }

    /**
     * @brief Calls basic can read.
     */
    virtual bool CanRead() const {
        return basicStream::CanRead();
    }

protected:
    uint64 OSSize() {
        return basicStream::Size();
    }

    bool OSSeek(uint64 seek) {
        return basicStream::Seek(seek);
    }

    bool OSRelativeSeek(int32 delta) {
        return basicStream::RelativeSeek(delta);
    }

    uint64 OSPosition() {
        return basicStream::Position();
    }

    bool OSSetSize(uint64 desSize) {
        return basicStream::SetSize(desSize);
    }

    bool OSRead(char8 * const outBuffer,
                uint32 &inSize) {
        return basicStream::Read(outBuffer, inSize, GetTimeout());
    }

    bool OSWrite(const char8 * const inBuffer,
                 uint32 &outSize) {

        return basicStream::Write(inBuffer, outSize, GetTimeout());
    }

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BUFFEREDSTREAMGENERATOR_H_ */

