/**
 * @file IOBufferPrivate.h
 * @brief Header file for class AnyType
 * @date 7 Sep 2017
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

#ifndef IOBUFFERPRIVATE_H_
#define IOBUFFERPRIVATE_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "IOBuffer.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/*lint -e526 . Justification: The following functions are not defined here. */

// These functions are implemented in IOBufferIntegerPrint.cpp
bool IntegerToStream(IOBuffer &ioBuffer,
                            uint8 number,
                            const FormatDescriptor &format);

bool IntegerToStream(IOBuffer &ioBuffer,
                            int8 number,
                            const FormatDescriptor &format);

bool IntegerToStream(IOBuffer &ioBuffer,
                            uint16 number,
                            const FormatDescriptor &format);

bool IntegerToStream(IOBuffer &ioBuffer,
                            int16 number,
                            const FormatDescriptor &format);

bool IntegerToStream(IOBuffer &ioBuffer,
                            uint32 number,
                            const FormatDescriptor &format);

bool IntegerToStream(IOBuffer &ioBuffer,
                            int32 number,
                            const FormatDescriptor &format);

bool IntegerToStream(IOBuffer &ioBuffer,
                            uint64 number,
                            const FormatDescriptor &format);

bool IntegerToStream(IOBuffer &ioBuffer,
                            int64 number,
                            const FormatDescriptor &format);

bool BitSetToStream(IOBuffer &ioBuffer,
                           uint32 * const numberAddress,
                           const uint8 numberBitShift,
                           const uint8 numberBitSize,
                           const bool numberIsSigned,
                           const FormatDescriptor &format);


// to be revised  TODO
bool PointerToStream(IOBuffer &ioBuffer, const void *address);

// to be revised  TODO
bool IntToStream(IOBuffer &ioBuffer, int i);


// These functions are implemented in IOBufferFloatPrint.cpp
bool FloatToStream(IOBuffer &buffer,
                          float32 number,
                          const FormatDescriptor &format);

bool FloatToStream(IOBuffer &buffer,
                          float64 number,
                          const FormatDescriptor &format);

/**
 * @brief Prints a CCString .
 * @param[out] iobuff is the output stream buffer.
 * @param[in] string is the string to be printed.
 * @param[in] fd specifies the desired format for the string.
 * @return true if the string is printed correctly.
 */
bool PrintCCString(IOBuffer & iobuff,
                          CCString const string,
                          const FormatDescriptor &fd,
                          bool addQuotesOnString = false);

/**
 * @brief Prints the bytes contained on a stream to a buffer.
 * @param[out] iobuff is the output stream buffer.
 * @param[in] stream is the stream in input which contains data to be copied.
 * @param[in] fd specifies the desired printing format.
 * @return false in case of errors in read and write operations.
 */
bool PrintStream(IOBuffer & iobuff,
                        StreamI &stream,
                        const FormatDescriptor &fd,
                        bool addQuotesOnString = false);

/**
 * @brief Prints a StructuredDataInterface.
 * @param[out] iobuff is the output stream buffer.
 * @param[in] structuredData is the input.
 * @return false in case of error in the StructuredDataI functions, true otherwise.
 */

bool PrintStructuredDataInterface(IOBuffer &iobuff,
                                         StructuredDataI * const structuredData,
                                         uint32 nodeLevel = 0u);

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/




/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L3STREAMS_IOBUFFERPRIVATE_H_ */
