/**
 * @file IOBuffer.cpp
 * @brief Source file for class IOBuffer
 * @date 01/10/2015
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

 * @details This source file contains the definition of all the methods for
 * the class IOBuffer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "IOBuffer.h"
#include "AdvancedErrorManagement.h"
#include "StringHelper.h"
#include "FormatDescriptor.h"
#include "BitSetToInteger.h"
#include "BufferedStream.h"

namespace MARTe {

/*lint -e526 . Justification: The following functions are not defined here. */

// These functions are implemented in IOBufferIntegerPrint.cpp
extern bool IntegerToStream(IOBuffer &ioBuffer,
                            uint8 number,
                            const FormatDescriptor &format);

extern bool IntegerToStream(IOBuffer &ioBuffer,
                            int8 number,
                            const FormatDescriptor &format);

extern bool IntegerToStream(IOBuffer &ioBuffer,
                            uint16 number,
                            const FormatDescriptor &format);

extern bool IntegerToStream(IOBuffer &ioBuffer,
                            int16 number,
                            const FormatDescriptor &format);

extern bool IntegerToStream(IOBuffer &ioBuffer,
                            uint32 number,
                            const FormatDescriptor &format);

extern bool IntegerToStream(IOBuffer &ioBuffer,
                            int32 number,
                            const FormatDescriptor &format);

extern bool IntegerToStream(IOBuffer &ioBuffer,
                            uint64 number,
                            const FormatDescriptor &format);

extern bool IntegerToStream(IOBuffer &ioBuffer,
                            int64 number,
                            const FormatDescriptor &format);

extern bool BitSetToStream(IOBuffer &ioBuffer,
                           uint32 *numberAddress,
                           uint8 numberBitShift,
                           uint8 numberBitSize,
                           bool numberIsSigned,
                           const FormatDescriptor &format);

// These functions are implemented in IOBufferFloatPrint.cpp
extern bool FloatToStream(IOBuffer &buffer,
                          float32 number,
                          const FormatDescriptor &format);

extern bool FloatToStream(IOBuffer &buffer,
                          float64 number,
                          const FormatDescriptor &format);

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * @brief Print a const char8* string on a buffer.
 * @param[out] iobuff is the output stream buffer.
 * @param[in] string is the string to be printed.
 * @param[in] fd specifies the desired format for the string.
 * @return true if the string is printed correctly.
 */
static bool PrintCCString(IOBuffer & iobuff,
                          const char8 * string,
                          const FormatDescriptor &fd) {

    bool ret = (string != NULL);

    if (ret) {
        //get the string size
        uint32 stringSize = StringHelper::Length(string);
        uint32 paddingSize = 0u;

        //is the desired size is 0 print completely the string without padd.
        if (fd.size != 0u) {
            //clip the string size if the desired size is minor.
            if (stringSize > fd.size) {
                stringSize = fd.size;
            }

            //if padded and desired size is greater than the string size
            //the difference is the padding size.
            bool isPadded = fd.padded;
            if (isPadded) {
                if (stringSize < fd.size) {
                    paddingSize = fd.size - stringSize;
                }
            }
        }

        bool isLeftAligned = fd.leftAligned;
        bool isPaddingSize = (paddingSize > 0u);

        //if right aligned put the padding at the beginning
        if ((!isLeftAligned) && (isPaddingSize)) {
            for (uint32 i = 0u; i < paddingSize; i++) {
                if (!iobuff.PutC(' ')) {
                    ret = false;
                }
            }
        }

        //print the string on the buffer completely.
        if (!iobuff.WriteAll(string, stringSize)) {
            ret = false;
        }

        //if left aligned put the padding at the end
        if ((isLeftAligned) && (isPaddingSize)) {
            for (uint32 i = 0u; i < paddingSize; i++) {
                if (!iobuff.PutC(' ')) {
                    ret = false;
                }
            }
        }
    }

    return ret;
}

/**
 * @brief Prints the bytes contained on a stream to a buffer.
 * @param[out] iobuff is the output stream buffer.
 * @param[in] stream is the stream in input which contains data to be copied.
 * @param[in] fd specifies the desired printing format.
 * @return false in case of errors in read and write operations.
 */
static bool PrintStream(IOBuffer & iobuff,
                        BufferedStream &stream,
                        const FormatDescriptor &fd) {

    bool ret = true;
    //print NULL pointer if the input stream is null.

    //the input stream must be seekable, otherwise the cursor is always at the end.
    if (stream.CanSeek()) {

        //calculates the size from the cursor to the end of the filled memory in the input stream
        uint64 streamSize = stream.Size();
        uint64 streamPosition = stream.Position();
        uint32 streamSizeL = static_cast<uint32>(streamSize - streamPosition);
        uint32 paddingSize = 0u;

        if (fd.size != 0u) {
            //if the desired size is minor, clip the stream size.
            if (streamSizeL > fd.size) {
                streamSizeL = fd.size;
            }

            bool isPadded = fd.padded;
            if (isPadded) {
                //if the desired size is greater and padded is true
                //calculates the padding size as the difference.
                if (streamSizeL < fd.size) {
                    paddingSize = fd.size - streamSizeL;
                }
            }
        }
        //limit within 32 bit and further limit to 10000 chars
        if (streamSizeL > 10000u) {
            ret = PrintCCString(iobuff, "!! too big > 10000 characters!!", fd);
        }
        else {
            //if right aligned put the padding at the beginning
            if ((!fd.leftAligned) && (paddingSize > 0u)) {
                for (uint32 i = 0u; i < paddingSize; i++) {
                    if (!iobuff.PutC(' ')) {
                        ret = false;
                    }
                }
            }

            //write the stream input on the stream buffer output
            char8 c;
            while (streamSizeL > 0u) {
                uint32 size = 1u;            
                if (!stream.Read(&c, size)) {
                    ret = false;
                }
                if (!iobuff.PutC(c)) {
                    ret = false;
                }
                streamSizeL--;
            }

            if (ret) {

                //if left aligned put the padding at the end.
                if (fd.leftAligned && (paddingSize > 0u)) {
                    for (uint32 i = 0u; i < paddingSize; i++) {
                        if (!iobuff.PutC(' ')) {
                            ret = false;
                        }
                    }
                }
            }
        }
    }
    else {
        ret = PrintCCString(iobuff, "!!stream !seek!!", fd);
    }

    return ret;
}

/**
 * @brief Prints a generic AnyType object on a buffer.
 * @param[out] iobuff is the stream buffer output.
 * @param[in] parIn is the generic object to be printed.
 * @param[in] fd specifies the desired printing format.
 */
static bool PrintToStream(IOBuffer & iobuff,
                          const AnyType & parIn,
                          const FormatDescriptor &fd) {

    bool ret = true;
// void anytype
    AnyType par = parIn;
    void* dataPointer = par.GetDataPointer();
    if (dataPointer != NULL) {

        //if the element is structured, the print is not supported.
        bool isStructured = (par.GetTypeDescriptor()).isStructuredData;
        if (isStructured) {
            //ErrorManagement::ReportError(UnsupportedError, "Streamable::Print StructuredData not supported");
            ret = false;
        }
        else {
            if (((par.GetTypeDescriptor()).type) == UnsignedInteger) {
                //native unsigned integer types.
                if (par.GetBitAddress() == 0u) {
                    switch ((par.GetTypeDescriptor()).numberOfBits) {
                        case 8u: {
                            uint8 *data = static_cast<uint8 *>(dataPointer);
                            ret = IntegerToStream(iobuff, *data, fd);
                        }
                        break;
                        case 16u: {
                            uint16 *data = static_cast<uint16 *>(dataPointer);
                            ret = IntegerToStream(iobuff, *data, fd);
                        }
                        break;
                        case 32u: {
                            uint32 *data = static_cast<uint32 *>(dataPointer);
                            ret = IntegerToStream(iobuff, *data, fd);
                        }
                        break;
                        case 64u: {
                            uint64 *data = static_cast<uint64 *>(dataPointer);
                            ret = IntegerToStream(iobuff, *data, fd);
                        }
                        break;
                        default: {
                            // use native standard integer
                            uint32 *number = static_cast<uint32 *>(dataPointer);
                            // all the remaining cases here
                            uint8 nBits = static_cast<uint8>((par.GetTypeDescriptor()).numberOfBits);
                            ret = BitSetToStream(iobuff, number, par.GetBitAddress(), nBits, false, fd);
                        }
                    }
                }
                else {
                    // use native standard integer
                    uint32 *number = static_cast<uint32 *>(dataPointer);
                    // all the remaining cases here
                    uint8 nBits = static_cast<uint8>((par.GetTypeDescriptor()).numberOfBits);
                    ret = BitSetToStream(iobuff, number, par.GetBitAddress(), nBits, false, fd);
                }

            }

            if (((par.GetTypeDescriptor()).type) == SignedInteger) {
                //native signed integer types.
                if (par.GetBitAddress() == 0u) {
                    switch ((par.GetTypeDescriptor()).numberOfBits) {
                        case 8u: {
                            int8 *data = static_cast<int8 *>(dataPointer);
                            ret = IntegerToStream(iobuff, *data, fd);
                        }
                        break;
                        case 16u: {
                            int16 *data = static_cast<int16 *>(dataPointer);
                            ret = IntegerToStream(iobuff, *data, fd);
                        }
                        break;
                        case 32u: {
                            int32 *data = static_cast<int32 *>(dataPointer);
                            ret = IntegerToStream(iobuff, *data, fd);
                        }
                        break;
                        case 64u: {
                            int64 *data = static_cast<int64 *>(dataPointer);
                            ret = IntegerToStream(iobuff, *data, fd);
                        }
                        break;
                        default: {
                            // use native standard integer
                            uint32 *number = static_cast<uint32 *>(dataPointer);
                            uint8 nBits = static_cast<uint8>((par.GetTypeDescriptor()).numberOfBits);
                            // all the remaining cases here
                            ret = BitSetToStream(iobuff, number, par.GetBitAddress(), nBits, true, fd);
                        }
                    }
                }
                else {
                    // use native standard integer
                    uint32 *number = static_cast<uint32 *>(dataPointer);
                    uint8 nBits = static_cast<uint8>((par.GetTypeDescriptor()).numberOfBits);
                    // all the remaining cases here
                    ret = BitSetToStream(iobuff, number, par.GetBitAddress(), nBits, true, fd);
                }

            }
            if (((par.GetTypeDescriptor()).type) == Float) {
                //native float32 types. Float 128 bit is not supported.
                switch ((par.GetTypeDescriptor()).numberOfBits) {
                    case 32u: {
                        float32 *data = static_cast<float32 *>(dataPointer);
                        ret = FloatToStream(iobuff, *data, fd);
                    }
                    break;
                    case 64u: {
                        float64 *data = static_cast<float64 *>(dataPointer);
                        ret = FloatToStream(iobuff, *data, fd);
                    }
                    break;
                    case 128u: {
                        //REPORT_ERROR(UnsupportedError,"unsupported 128 bit float32")
                        ret = false;
                    }
                    break;
                    default: {
                        //REPORT_ERROR(ParametersError,"non standard float32 size")
                        ret = false;
                    }
                }
            }

            //pointer type.
            if (((par.GetTypeDescriptor()).type) == Pointer) {
                TypeDescriptor newTypeDes(par.GetTypeDescriptor().isConstant, UnsignedInteger, par.GetTypeDescriptor().numberOfBits);
                AnyType at(newTypeDes, par.GetBitAddress(), static_cast<void *>(&dataPointer));
                ret = PrintToStream(iobuff, at, fd);
            }
            //const char8* string type.
            //if in the format descriptor is specified the hex notation (%p or %x)
            //print the value of the pointer.
            if (((par.GetTypeDescriptor()).type) == CCString) {
                if (fd.binaryNotation == HexNotation) {
                    TypeDescriptor newTypeDes(par.GetTypeDescriptor().isConstant, UnsignedInteger, par.GetTypeDescriptor().numberOfBits);
                    AnyType at(newTypeDes, par.GetBitAddress(), static_cast<void *>(&dataPointer));
                    ret = PrintToStream(iobuff, at, fd);
                }
                else {
                    const char8 *string = static_cast<const char8 *>(dataPointer);
                    ret = PrintCCString(iobuff, string, fd);
                }
            }

            //general stream type.
            if (((par.GetTypeDescriptor()).type) == Stream) {
                BufferedStream * stream = static_cast<BufferedStream *>(dataPointer);
                ret = PrintStream(iobuff, *stream, fd);

            }
        }
    }
    else {
        ret = false;
    }
//REPORT_ERROR(UnsupportedError,"unsupported format")
    return ret;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool IOBuffer::GetToken(char8 * const outputBuffer,
                                  const char8 * const terminator,
                                  uint32 outputBufferSize,
                                  char8 &saveTerminator,
                                  const char8 * skipCharacters) {

    bool ret = true;
    bool quit = false;
    // need space for trailing 0
    outputBufferSize--;

    if (skipCharacters == NULL) {
        skipCharacters = terminator;
    }

    uint32 tokenSize = 0u;
    while (!quit) {
        char8 c;
        if (!GetC(c)) {

            // 0 terminated string
            outputBuffer[tokenSize] = '\0';

            saveTerminator = '\0';

            //
            if (tokenSize == 0u) {
                ret = false;
                quit = true;
            }
            else {
                quit = true;
            }
        }
        else {

            bool isTerminator = (StringHelper::SearchChar(terminator, c) != NULL);
            bool isSkip = (StringHelper::SearchChar(skipCharacters, c) != NULL);
            if ((isTerminator) || (c == '\0')) {

                // quit only if some data was read, otw just skip separator block
                if ((tokenSize != 0u) || (!isSkip)) {
                    // 0 terminated string
                    outputBuffer[tokenSize] = '\0';

                    saveTerminator= c;

                    quit = true;
                }
            }
            //isTerminator is false here
            else {
                if (!isSkip) {

                    outputBuffer[tokenSize] = c;
                    tokenSize++;
                    if (tokenSize >= outputBufferSize) {
                        // 0 terminated string
                        outputBuffer[tokenSize] = '\0';

                        saveTerminator = c;

                        quit = true;
                    }
                }
            }
        }
    }

    return ret;
}

bool IOBuffer::GetToken(IOBuffer & outputBuffer,
                                  const char8 * const terminator,
                                  char8 &saveTerminator,
                                  const char8 * skipCharacters) {

    if (skipCharacters == NULL) {
        skipCharacters = terminator;
    }

    bool ret = true;
    bool quit = false;
    uint32 tokenSize = 0u;
    while (!quit) {
        char8 c;
        if (!GetC(c)) {

            saveTerminator = '\0';

            //
            if (tokenSize == 0u) {
                ret = false;
                quit = true;
            }
            else {
                quit = true;
            }
        }
        else {

            bool isTerminator = (StringHelper::SearchChar(terminator, c) != NULL);
            bool isSkip = (StringHelper::SearchChar(skipCharacters, c) != NULL);
            if ((isTerminator) || (c == '\0')) {
                // quit only if some data was read, otw just skip separator block
                if ((tokenSize != 0u) || (!isSkip)) {

                    saveTerminator = c;

                    quit = true;
                }
            }
            else {
                if (!isSkip) {
                    if (!outputBuffer.PutC(c)) {
                        ret = false;
                        quit = true;
                    }
                    tokenSize++;
                }
            }
        }
    }

    return ret;
}

bool IOBuffer::SkipTokens(uint32 count,
                                  const char8 * const terminator) {

    bool ret = true;
    uint32 tokenSize = 0u;
    while (count > 0u) {
        char8 c;
        if (!GetC(c)) {
            ret = false;
            //quit
            count = 0u;
        }
        else {
            //
            if ((StringHelper::SearchChar(terminator, c) != NULL) || (c == '\0')) {
                // quit only if some data was read, otherwise just skip separator block
                if (tokenSize != 0u) {
                    tokenSize = 0u;
                    count--;
                }
            }
            else {
                tokenSize++;
            }
        }
    }

    return ret;
}

bool IOBuffer::PrintFormatted(const char8 * format,
                                      const AnyType pars[]) {

    bool ret = true;
    bool quit = false;
    // indicates active parameter
    int32 parsIndex = 0;
    // checks silly parameter
    if (format != NULL) {

        // loops through parameters
        while (!quit) {
            // scans for % and in the meantime prints what it encounters
            while ((format[0] != '\0') && (format[0] != '%')) {
                if (!PutC(format[0])) {
                    ret = false;
                    quit = true;
                }
                format = &format[1];
            }
            if (ret) {

                // end of format
                if (format[0] == '\0') {
                    quit = true;
                }
                else {

                    // consume %
                    format = &format[1];

                    // if not end then %
                    // keep on parsing format to build a FormatDescriptor
                    FormatDescriptor fd;
                    if (!fd.InitialiseFromString(format)) {
                        ret = false;
                        quit = true;
                    }
                    else {

                        // if void simply skip and continue
                        if (!pars[parsIndex].IsVoid()) {
                            // use it to process parameters
                            if (!PrintToStream(*this, pars[parsIndex], fd)) {
                                ret = false;
                                quit = true;
                            }
                            parsIndex++;
                        }
                    }
                }
            }
        }
    }
    else {
        ret = false;
    }
    // never comes here!
    return ret;
}

bool IOBuffer::Seek(const uint32 position) {
    bool retval = (position <= UsedSize());

    if (retval) {
        amountLeft = MaxUsableAmount() - position;
        positionPtr = &((BufferReference())[position]);
    }
    return retval;
}

//position is set relative to current position
/*lint -e{9125} [MISRA C++ Rule 5-0-9]. Justification: the sign does not change because delta is negative (see the inline code).*/
bool IOBuffer::RelativeSeek(const int32 delta) {
    bool ret = true;
    if (delta >= 0) {
        uint32 gap = static_cast<uint32>(delta);
        uint32 actualLeft = amountLeft - fillLeft;
        //cannot seek beyond fillLeft
        if (gap > actualLeft) {
            //  saturate at the end
            gap = actualLeft;
            ret = false;
//REPORT_ERROR_PARAMETERS(ErrorType::ParametersError,"delta=%i at position %i moves out of range %i, moving to end of stream",delta,Position(),MaxUsableAmount())
        }
        amountLeft -= gap;
        positionPtr = &positionPtr[gap];
    }
    else {

        uint32 gap = static_cast<uint32>(-delta);
        // cannot seek below 0
        if (gap > Position()) {
            //  saturate at the beginning
            ret = false;
            gap = Position();

//REPORT_ERROR_PARAMETERS(ParametersError,"delta=%i at position %i moves out of range 0, moving to beginning of stream",delta,Position())
        }
        amountLeft += gap;
        positionPtr = &((BufferReference())[Position() - gap]);
    }

    return ret;
}

void IOBuffer::SetUsedSize(const uint32 desiredSize) {
    uint32 size;

    if (desiredSize > maxUsableAmount) {
        size = maxUsableAmount;
    }
    else {
        size = desiredSize;
    }

    fillLeft = maxUsableAmount - size;
}

IOBuffer::~IOBuffer() {
    positionPtr = static_cast<char8 *>(NULL);
}

bool IOBuffer::SetBufferHeapMemory(const uint32 desiredSize,
                                   const uint32 reservedSpaceAtEnd) {
    // save position
    uint32 position = Position();
    uint32 usedSize = UsedSize();

    // if the buffer was set on a memory reference begin again
    if ((!internalBuffer.IsAllocated()) && (positionPtr != static_cast<char8 *>(NULL))) {
        position = 0u;
        usedSize = 0u;
    }

    //special case: if we consider the difference
    //between two unsigned integers we can obtain bigger numbers (overflow).
    if (desiredSize < reservedSpaceAtEnd) {
        usedSize = 0u;
    }

    // truncating
    if ((desiredSize - reservedSpaceAtEnd) < usedSize) {
        usedSize = desiredSize - reservedSpaceAtEnd;
    }

    // saturate index
    if (position > usedSize) {
        position = usedSize;
    }

    bool ret = internalBuffer.SetBufferSize(desiredSize);

    positionPtr = BufferReference();

    maxUsableAmount = GetBufferSize();

    if (maxUsableAmount <= reservedSpaceAtEnd) {
        maxUsableAmount = 0u;
    }
    else {
        maxUsableAmount = GetBufferSize() - reservedSpaceAtEnd;
    }

    amountLeft = maxUsableAmount - position;
    fillLeft = maxUsableAmount - usedSize;

    positionPtr = &positionPtr[position];
    return ret;
}

void IOBuffer::SetBufferReferencedMemory(char8 * const buffer,
                                         const uint32 bufferSize,
                                         const uint32 reservedSpaceAtEnd) {
    internalBuffer.SetBufferReference(buffer, bufferSize);
    positionPtr = BufferReference();
    maxUsableAmount = GetBufferSize() - reservedSpaceAtEnd;
    Empty();
}

void IOBuffer::SetBufferReadOnlyReferencedMemory(const char8 * const buffer,
                                                 const uint32 bufferSize,
                                                 const uint32 reservedSpaceAtEnd) {
    internalBuffer.SetBufferReference(buffer, bufferSize);
    positionPtr = BufferReference();
    maxUsableAmount = GetBufferSize() - reservedSpaceAtEnd;
    Empty();
}

bool IOBuffer::NoMoreSpaceToWrite() {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11]. Justification: The default behavior does not need the input argument.*/
bool IOBuffer::NoMoreSpaceToWrite(const uint32 neededSize) {
    return NoMoreSpaceToWrite();
}

bool IOBuffer::NoMoreDataToRead() {
    return false;
}

bool IOBuffer::Resync() {
    return false;
}

bool IOBuffer::Write(const char8 * const buffer,
                     uint32 &size) {
    bool retval = internalBuffer.CanWrite();
    if (retval) {

        // clip to spaceLeft
        if (size > amountLeft) {
            size = amountLeft;
        }

        // fill the buffer with the remainder
        if (size > 0u) {
            if (!MemoryOperationsHelper::Copy(positionPtr, buffer, size)) {
                retval = false;
            }

            if (retval) {
                positionPtr = &positionPtr[size];
                amountLeft -= size;
                if (fillLeft > amountLeft) {
                    fillLeft = amountLeft;
                }
            }
        }
    }

    return retval;
}

bool IOBuffer::WriteAll(const char8 * buffer,
                        const uint32 &size) {

    bool retval = true;
    //size to be copied.
    uint32 leftSize = size;
    while (retval && (leftSize > 0u)) {
        // if the cursor is at the end call NoMoreSpaceToWrite
        // flushes the buffer or allocates new memory.
        if (amountLeft == 0u) {
            if (!NoMoreSpaceToWrite(leftSize)) {
                retval = false;
            }
            if (retval) {
                //Something wrong, no more available space, return false.
                if (amountLeft == 0u) {
                    retval = false;
                }
            }
        }

        if (retval) {
            uint32 toDo = leftSize;
            retval = Write(buffer, toDo);
            if (retval) {
                buffer = &buffer[toDo];
                //if all the size is copied leftSize becomes 0 and return true
                leftSize -= toDo;
            }
        }
    }
    return retval;
}

bool IOBuffer::Read(char8 * const buffer,
                    uint32 &size) {

    bool retval = true;
    uint32 maxSize = UsedAmountLeft();
    // clip to available space
    if (size > maxSize) {
        size = maxSize;
    }

    // fill the buffer with the remainder
    if (size > 0u) {
        if (!MemoryOperationsHelper::Copy(buffer, positionPtr, size)) {
            retval = false;

        }
        if (retval) {
            amountLeft -= size;
            positionPtr = &positionPtr[size];
        }
    }
    return retval;
}

}
