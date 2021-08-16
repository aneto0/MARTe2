/**
 * @file BasicUART.h
 * @brief Header file for class BasicUART.
 * @date 16/08/2021
 * @author Filippo Sartori
 * @author Pedro Lourenco
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
 *
 * @details This header file contains the declaration of the class BasicUART
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICUART_H_
#define BASICUART_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <cstring>
#include <string>
#include <sstream>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief UART implementation based on the Linux general terminal interface
 * that is provided to control asynchronous communications ports.
 * @details Some of termio parameters are hard-coded as follows:
 *     - eight data bits;
 *     - no parity bit;
 *     - one stop bit;
 *     - noncanonical input processing mode.
 */
class DLL_API BasicUART {
public:

    /**
     * @brief Default constructor.
     * @details FD_ZERO the file descriptors.
     */
    BasicUART();

    /**
     * @brief Destructor.
     * @details Calls Close().
     */
    virtual ~BasicUART();

    /**
     * @brief Sets the speed of the UART. Shall be called before the Open
     * method.
     * @param[in] serial the speed to set.
     * @return true if the speed was successfully updated.
     */
    bool SetSpeed(uint32 speed);

    /**
     * @brief Opens the UART described by the provided \a name.
     * @details The UART is open in read/write mode. Many of the UART
     * properties are hard-coded for the CRIO use-case.
     * @param[in] name the UART file descriptor location.
     * @pre
     *   SetSpeed
     */
    bool Open(const char8 *name);

    /**
     * @brief Closes the UART.
     * @pre
     *   Open
     */
    void Close();

    /**
     * @brief Reads \a size bytes into the \a buffer.
     * @param[in] buffer the memory where to write the read bytes.
     * @param[in,out] size the number of bytes to read and the number of bytes
     * that were actually read.
     * @return true if the Read operation completes successfully (and reads all
     * the requested bytes).
     * @pre
     *   Open
     */
    bool Read(char8 *buffer, uint32 &size);

    /**
     * @brief Reads \a size bytes into the \a buffer within \a timeoutUsec
     * micro-seconds.
     * @param[in] buffer the memory where to write the read bytes.
     * @param[in,out] size the number of bytes to read and the number of bytes
     * that were actually read.
     * @param[in] timeoutUsec the maximum time to complete the read operation.
     * @return true if the Read operation completes successfully within the
     * timeout (and reads all the requested bytes).
     * @pre
     *   Open
     */
    bool Read(char8 *buffer, uint32 &size, uint32 timeoutUsec);

    /**
     * @brief Waits \a timeoutUsec micro-seconds for data to be available for
     * reading in the UART.
     * @param[in] timeoutUsec the maximum time to wait for data to be
     * available.
     * @return true if data is available to be read within the timeout.
     */
    bool WaitRead(uint32 timeoutUsec);

    /**
     * @brief Writes \a size bytes from the \a buffer.
     * @param[in] buffer the memory where to read the bytes to write.
     * @param[in] size the number of bytes to write.
     * @param[in] timeoutUsec the maximum time to complete the read operation.
     * @return true if the Read operation completes successfully within the
     * timeout (and reads all the requested bytes).
     * @pre
     *   Open
     */
    bool Write(char8 *buffer, uint32 size);

    /**
     * @brief Waits \a timeoutUsec micro-seconds for the UART to be available
     * for writing data.
     * @param[in] timeoutUsec the maximum time to wait for the write to be
     * available.
     * @return true if the UART is available to write within the timeout.
     */
    bool WaitWrite(MARTe::uint32 timeoutUsec);

private:

    /**
     * The file descriptor associated to the Open operation.
     */
    int32 fileDescriptor;

    /**
     * The speed set with SetSpeed.
     */
    int32 speedCode;

    /**
     * The file descriptor array for the read in the select.
     */
    fd_set readFDS;

    /**
     * The file descriptor array ready to be read as returned by the select.
     */
    fd_set readFDS_done;

    /**
     * The file descriptor array for the write in the select.
     */
    fd_set writeFDS;

    /**
     * The file descriptor array ready to be written as returned by the select.
     */
    fd_set writeFDS_done;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICUART_H_ */
