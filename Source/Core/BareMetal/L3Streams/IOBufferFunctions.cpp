/**
 * @file IOBufferFunctions.cpp
 * @brief Source file for class IOBufferFunctions
 * @date 02/10/2015
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
 * the class IOBufferFunctions (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "FormatDescriptor.h"
#include "BitSetToInteger.h"
#include "IOBuffer.h"
#include "BufferedStream.h"
#include "StringHelper.h"

namespace MARTe {

/*lint -e526 . Justification: The function is not defined here. */
extern bool IntegerToStream(IOBuffer &stream,
                            uint8 number,
                            FormatDescriptor format);

/*lint -e{526} . Justification: The function is not defined here. */
extern bool IntegerToStream(IOBuffer &stream,
                            int8 number,
                            FormatDescriptor format);

/*lint -e{526} . Justification: The function is not defined here. */
extern bool IntegerToStream(IOBuffer &stream,
                            uint16 number,
                            FormatDescriptor format);

/*lint -e{526} . Justification: The function is not defined here. */
extern bool IntegerToStream(IOBuffer &stream,
                            int16 number,
                            FormatDescriptor format);

/*lint -e{526} . Justification: The function is not defined here. */
extern bool IntegerToStream(IOBuffer &stream,
                            uint32 number,
                            FormatDescriptor format);

/*lint -e{526} . Justification: The function is not defined here. */
extern bool IntegerToStream(IOBuffer &stream,
                            int32 number,
                            FormatDescriptor format);

/*lint -e{526} . Justification: The function is not defined here. */
extern bool IntegerToStream(IOBuffer &stream,
                            uint64 number,
                            FormatDescriptor format);

/*lint -e{526} . Justification: The function is not defined here. */
extern bool IntegerToStream(IOBuffer &stream,
                            int64 number,
                            FormatDescriptor format);


/*lint -e{526} . Justification: The function is not defined here. */
extern bool BitSetToStream(IOBuffer &stream,
                           uint32 *numberAddress,
                           uint8 numberBitShift,
                           uint8 numberBitSize,
                           bool numberIsSigned,
                           FormatDescriptor format);


/*lint -e{526} . Justification: The function is not defined here. */
extern bool FloatToStream(IOBuffer &buffer,
                          float32 number,
                          FormatDescriptor format);


/*lint -e{526} . Justification: The function is not defined here. */
extern bool FloatToStream(IOBuffer &buffer,
                          float64 number,
                          FormatDescriptor format);


bool IOBuffer::GetTokenFromStream(char8 * const outputBuffer,
                                  const char8 * const terminator,
                                  uint32 outputBufferSize,
                                  char8 * const saveTerminator,
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

            if (saveTerminator != NULL) {
                saveTerminator[0] = '\0';
            }

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

                    if (saveTerminator != NULL) {
                        saveTerminator[0] = c;
                    }

                    quit=true;
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

                        if (saveTerminator != NULL) {
                            saveTerminator[0] = c;
                        }

                        quit=true;
                    }
                }
            }
        }
    }

    return ret;
}

bool IOBuffer::GetTokenFromStream(IOBuffer & outputBuffer,
                                  const char8 * const terminator,
                                  char8 * const saveTerminator,
                                  const char8 * skipCharacters) {

    if (skipCharacters == NULL) {
        skipCharacters = terminator;
    }

    bool ret=true;
    bool quit=false;
    uint32 tokenSize=0u;
    while(!quit) {
        char8 c;
        if (!GetC(c)) {

            if (saveTerminator != NULL) {
                saveTerminator[0] = '\0';
            }

            //
            if (tokenSize==0u) {
                ret=false;
                quit=true;
            }
            else {
                quit=true;
            }
        }
        else {

            bool isTerminator = (StringHelper::SearchChar(terminator,c)!=NULL);
            bool isSkip = (StringHelper::SearchChar(skipCharacters ,c)!=NULL);
            if ((isTerminator) || ( c=='\0' )) {
                // quit only if some data was read, otw just skip separator block
                if ((tokenSize != 0u) || (!isSkip)) {

                    if (saveTerminator != NULL) {
                        saveTerminator[0] = c;
                    }

                    quit= true;
                }
            }
            else {
                if (!isSkip) {
                    if(!outputBuffer.PutC(c)) {
                        ret=false;
                        quit=true;
                    }
                    tokenSize++;
                }
            }
        }
    }

    return ret;
}

bool IOBuffer::SkipTokensInStream(uint32 count,
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
            if ((StringHelper::SearchChar(terminator, c) != NULL)||(c=='\0')) {
                // quit only if some data was read, otherwise just skip separator block
                if (tokenSize!=0u) {
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

/*
 * @brief Print a const char8* string on a stream buffer.
 * @param iobuff is the output stream buffer.
 * @param string is the string to be printed.
 * @param fd specifies the desired format for the string.
 * @return true if the string is printed correctly.
 *
 * This function calls IOBuffer::WriteAll to write the complete string size on the stream buffer.
 */
static inline
bool PrintCCString(IOBuffer & iobuff,
                   const char8 * string,
                   const FormatDescriptor &fd) {
    //if the string is null print NULL pointer on the stream.
    if (string == NULL) {
        string = "NULL pointer";
    }

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
        bool isPadded=fd.padded;
        if (isPadded) {
            if (stringSize < fd.size) {
                paddingSize = fd.size - stringSize;
            }
        }
    }

    bool ret = true;

    bool isLeftAligned=fd.leftAligned;
    bool isPaddingSize=(paddingSize > 0u);

    //if right aligned put the padding at the beginning
    if ((!isLeftAligned) && (isPaddingSize)) {
        for (uint32 i=0u;i < paddingSize;i++) {
            if(!iobuff.PutC(' ')) {
                //TODO
                ret=false;
            }
        }
    }

    //print the string on the buffer completely.
    if(!iobuff.WriteAll(string,stringSize)) {
        //TODO
        ret=false;
    }

    //if left aligned put the padding at the end
    if ((isLeftAligned) && (isPaddingSize)) {
        for (uint32 i=0u;i < paddingSize;i++) {
            if(!iobuff.PutC(' ')) {
                //TODO
                ret=false;
            }
        }
    }

    return ret;
}

/*
 * @brief Prints the bytes contained on a stream to a stream buffer.
 * @param iobuff is the output stream buffer.
 * @param stream is the stream in input which contains data to be copied.
 * @param fd specifies the desired printing format.
 * @return false in case of errors in read and write operations.
 *
 * This function calls the function StreamI::GetC which calls the specific stream Write function truth the virtual table.
 * Gets a character from the stream and use IOBuffer::PutC to write it on the output stream buffer doing this operation for
 * each character from the cursor to the end of the input stream.*/
static inline
bool PrintStream(IOBuffer & iobuff,
                 BufferedStream * const stream,
                 const FormatDescriptor &fd) {

    bool ret = true;
    //print NULL pointer if the input stream is null.
    if (stream != NULL) {

        //the input stream must be seekable, otherwise the cursor is always at the end.
        if (stream->CanSeek()) {

            //calculates the size from the cursor to the end of the filled memory in the input stream
            uint64 streamSize=stream->Size();
            uint64 streamPosition=stream->Position();
            uint32 streamSizeL = static_cast<uint32>(streamSize-streamPosition);
            uint32 paddingSize = 0u;

            if (fd.size != 0u) {
                //if the desired size is minor, clip the stream size.
                if (streamSizeL > fd.size) {
                    streamSizeL = fd.size;
                }

                bool isPadded=fd.padded;
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
                ret= PrintCCString(iobuff,"!! too big > 10000 characters!!",fd);
            }
            else {
                //if right aligned put the padding at the beginning
                if ((!fd.leftAligned) && (paddingSize > 0u)) {
                    for (uint32 i=0u;i < paddingSize;i++) {
                        if(!iobuff.PutC(' ')) {
                            ret=false;
                            //TODO
                        }
                    }
                }

                //write the stream input on the stream buffer output
                char8 c;
                while (streamSizeL > 0u) {
                    if (!stream->GetC(c)) {
                        ret=false;
                    }
                    if (!iobuff.PutC(c)) {
                        ret=false;
                    }
                    streamSizeL--;
                }

                if(ret) {

                    //if left aligned put the padding at the end.
                    if (fd.leftAligned && (paddingSize > 0u)) {
                        for (uint32 i=0u;i < paddingSize;i++) {
                            if(!iobuff.PutC(' ')) {
                                ret=false;
                                //TODO
                            }
                        }
                    }
                }
            }
        }
        else {
            ret= PrintCCString(iobuff,"!!stream !seek!!",fd);
        }
    }
    else {
        ret= PrintCCString(iobuff,"!!NULL pointer!!",fd);
    }
    return ret;
}

/*
 * @brief Prints a generic AnyType object on a stream buffer.
 * @param iobuff is the stream buffer output.
 * @param par is the generic object to be printed.
 * @param fd specifies the desired printing format.
 *
 * Looking at the AnyType structure fields, calls the right function
 * for the conversion to string and the print of each type.
 */
static
bool PrintToStream(IOBuffer & iobuff,
                   const AnyType & parIn,
                   const FormatDescriptor &fd) {

    bool ret = true;
    // void anytype
    AnyType par = parIn;
    void* dataPointer = par.GetDataPointer();
    if (dataPointer != NULL) {

        //if the element is structured, the print is not supported.
        bool isStructured=(par.GetTypeDescriptor()).isStructuredData;
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
                            uint8 *data = static_cast<uint8 *> (dataPointer);
                            ret = IntegerToStream(iobuff, *data, fd);
                        }
                        break;
                        case 16u: {
                            uint16 *data = static_cast<uint16 *> (dataPointer);
                            ret = IntegerToStream(iobuff, *data, fd);
                        }
                        break;
                        case 32u: {
                            uint32 *data = static_cast<uint32 *> (dataPointer);
                            ret = IntegerToStream(iobuff, *data, fd);
                        }
                        break;
                        case 64u: {
                            uint64 *data = static_cast<uint64 *> (dataPointer);
                            ret = IntegerToStream(iobuff, *data, fd);
                        }
                        break;
                        default: {
                            ret= false;
                        }
                    }
                }
                else {
                    // use native standard integer
                    uint32 *number = static_cast<uint32 *> (dataPointer);
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
                            int8 *data = static_cast<int8 *> (dataPointer);
                            ret = IntegerToStream(iobuff, *data, fd);
                        }
                        break;
                        case 16u: {
                            int16 *data = static_cast<int16 *> (dataPointer);
                            ret = IntegerToStream(iobuff, *data, fd);
                        }
                        break;
                        case 32u: {
                            int32 *data = static_cast<int32 *> (dataPointer);
                            ret = IntegerToStream(iobuff, *data, fd);
                        }
                        break;
                        case 64u: {
                            int64 *data = static_cast<int64 *> (dataPointer);
                            ret = IntegerToStream(iobuff, *data, fd);
                        }
                        break;
                        default: {
                            ret= false;
                        }
                    }
                }
                else {
                    // use native standard integer
                    uint32 *number = static_cast<uint32 *> (dataPointer);
                    uint8 nBits = static_cast<uint8>((par.GetTypeDescriptor()).numberOfBits);
                    // all the remaining cases here
                    ret = BitSetToStream(iobuff, number, par.GetBitAddress(), nBits, true, fd);
                }

            }
            if (((par.GetTypeDescriptor()).type) == Float) {
                //native float32 types. Float 128 bit is not supported.
                switch ((par.GetTypeDescriptor()).numberOfBits) {
                    case 32u: {
                        float32 *data = static_cast<float32 *> (dataPointer);
                        ret = FloatToStream(iobuff, *data, fd);
                    }
                    break;
                    case 64u: {
                        float64 *data = static_cast<float64 *> (dataPointer);
                        ret = FloatToStream(iobuff, *data, fd);
                    }
                    break;
                    case 128u: {
                        //REPORT_ERROR(UnsupportedError,"unsupported 128 bit float32")
                        ret= false;
                    }
                    break;
                    default: {
                        //REPORT_ERROR(ParametersError,"non standard float32 size")
                        ret= false;
                    }
                }
            }

            //pointer type.
            if (((par.GetTypeDescriptor()).type) == Pointer) {
                TypeDescriptor newTypeDes(par.GetTypeDescriptor().isConstant, UnsignedInteger,par.GetTypeDescriptor().numberOfBits);
                AnyType at(newTypeDes,par.GetBitAddress(),static_cast<void *>(&dataPointer));
                ret = PrintToStream(iobuff, at, fd);
            }
            //const char8* string type.
            //if in the format descriptor is specified the hex notation (%p or %x)
            //print the value of the pointer.
            if (((par.GetTypeDescriptor()).type) == CCString) {
                if (fd.binaryNotation == HexNotation) {
                    TypeDescriptor newTypeDes(par.GetTypeDescriptor().isConstant, UnsignedInteger,par.GetTypeDescriptor().numberOfBits);
                    AnyType at(newTypeDes,par.GetBitAddress(),static_cast<void *>(&dataPointer));
                    ret = PrintToStream(iobuff, at, fd);
                }
                else {
                    const char8 *string = static_cast<const char8 *> (dataPointer);
                    ret=PrintCCString(iobuff, string, fd);
                }
            }

            //general stream type.
            if (((par.GetTypeDescriptor()).type) == Stream) {
                BufferedStream * stream = static_cast<BufferedStream *>(dataPointer);
                ret=PrintStream(iobuff, stream, fd);

            }
        }
    }
    else {
        ret=false;
    }
    //REPORT_ERROR(UnsupportedError,"unsupported format")
    return ret;
}

/*
 * @brief The function called by all Printf operations.
 * @param iobuff is the stream buffer in the output.
 * @param format is a printf like string format.
 * @param pars is a list of AnyType elements to print.
 * @return the return of PrintToStream function.
 *
 * This function read the format, builds the related format descriptor and then
 * calls the PrintToStream function passing the next AnyType element in the list.*/
bool IOBuffer::PrintFormattedToStream(const char8 * format,
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
                    ret= false;
                    quit=true;
                }
                format=&format[1];
            }
            if(ret) {

                // end of format
                if (format[0] == '\0') {
                    quit=true;
                }
                else {

                    // consume %
                    format=&format[1];

                    // if not end then %
                    // keep on parsing format to build a FormatDescriptor
                    FormatDescriptor fd;
                    if (!fd.InitialiseFromString(format)) {
                        ret= false;
                        quit=true;
                    }
                    else {

                        // if void simply skip and continue
                        if (!pars[parsIndex].IsVoid()) {
                            // use it to process parameters
                            if (!PrintToStream(*this, pars[parsIndex], fd)) {
                                ret= false;
                                quit=true;
                            }
                            parsIndex++;
                        }
                    }
                }
            }
        }
    }
    else {
        ret=false;
    }
    // never comes here!
    return ret;
}

}
