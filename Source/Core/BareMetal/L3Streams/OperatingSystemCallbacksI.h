/**
 * @file OperatingSystemCallbacksI.h
 * @brief Header file for class OperatingSystemCallbacksI
 * @date 19/10/2015
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

 * @details This header file contains the declaration of the class OperatingSystemCallbacksI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OPERATINGSYSTEMCALLBACKSI_H_
#define OPERATINGSYSTEMCALLBACKSI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @details All modular Stream Buffering mechanisms: SingleBufferStream, DoubleBufferStream
 * derive from this interface. It is for the final classes, extensions of a Basic Stream to a buffered
 * stream to implement this interface. This is a copy of the StreamI interface to be used in a callback
 * fashion by the buffering mechanism. BufferedStreamGenerator automatises the creation of this compound class
 *
 */
class DLL_API OperatingSystemCallbacksI {

    friend class BufferedStreamIOBuffer;

public:

    virtual ~OperatingSystemCallbacksI() {
    }

protected:

    /**
     * @brief Writes without buffering.
     * @param[in] data the array of bytes to write.
     * @param[in,out] size as input is the number of bytes to write. In output the number of bytes actually written.
     * @return true if \a size bytes of data are successfully written within the specified \a timeout (see SetTimeout).
     */
    virtual bool OSWrite(const char8 * const data,
            uint32 & size)=0;

    /**
     * @brief Reads without buffering.
     * @param[out] data destination array where the read data will be put.
     * @param[in,out] size as input is the number of bytes to read. In output the number of bytes actually read.
     * @return true if \a size bytes of data are successfully read within the specified \a timeout (see SetTimeout).
     */
    virtual bool OSRead(char8 * const data,
            uint32 & size)=0;

    /**
     * @brief Retrieves the size of the low-level, unbuffered, stream implementation.
     * @return the size of the low-level stream.
     */
    virtual uint64 OSSize()const = 0;

    /**
     * @brief Moves within the low-level, unbuffered, stream implementation to an absolute location.
     * @param[in] pos the desired absolute position.
     * @return true if the stream is successfully moved to \a pos.
     */
    virtual bool OSSeek(uint64 pos) = 0;

    /**
     * @brief Moves within the low-level, unbuffered, stream to a position that is relative to the current location.
     * @param[in] deltaPos is the distance from the current position.
     * @return true if the stream is successfully moved to \a deltaPos.
     */
    virtual bool OSRelativeSeek(int64 deltaPos)=0;

    /**
     * @brief Gets the current position in the low-level, unbuffered, stream.
     * @return the current position in the low-level stream.
     */
    virtual uint64 OSPosition() const= 0;

    /**
     * @brief Clips the low-level, unbuffered, stream size.
     * @param size the new size of the low-level stream.
     * @return true if the size of the low-level stream is set to \a size.
     */
    virtual bool OSSetSize(uint64 size) = 0;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OPERATINGSYSTEMCALLBACKSI_H_ */

