/**
 * @file IOBufferStreamPrint.cpp
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

#include "../L0Types/StreamI.h"
#include "IOBufferPrivate.h"
#include "CCString.h"


namespace MARTe{




/**
 * @brief Prints the bytes contained on a stream to a buffer.
 * @param[out] iobuff is the output stream buffer.
 * @param[in] stream is the stream in input which contains data to be copied.
 * @param[in] fd specifies the desired printing format.
 * @return false in case of errors in read and write operations.
 */
 bool PrintStream(IOBuffer & iobuff,
                        StreamI *stream,
                        const FormatDescriptor &fd,
                        bool addQuotesOnString) {

    bool ret = true;
    //print NULL pointer if the input stream is null.

    //the input stream must be seekable, otherwise the cursor is always at the end.
    if (stream->CanSeek()) {

        //calculates the size from the cursor to the end of the filled memory in the input stream
        uint64 streamSize = stream->Size();
        uint64 streamPosition = stream->Position();
        uint32 streamSizeL = static_cast<uint32>(streamSize - streamPosition);
        uint32 paddingSize = 0u;

        // consider the quotes
        uint32 gap = (addQuotesOnString) ? (2u) : (0u);

        uint32 desSize = fd.size;

        if (desSize != 0u) {
            //if the desired size is minor, clip the stream size.
            if (streamSizeL > desSize) {
                streamSizeL = desSize;
            }

            bool isPadded = fd.padded;
            if (isPadded) {
                //if the desired size is greater and padded is true
                //calculates the padding size as the difference.
                if (streamSizeL < desSize) {
                    paddingSize = desSize - streamSizeL;
                }
            }
        }
        else {
            streamSizeL += gap;
            desSize = streamSizeL;
        }
        //limit within 32 bit and further limit to 10000 chars
        if (streamSizeL > 10000u) {
            REPORT_ERROR(ErrorManagement::FatalError, "IOBuffer: Size too big");
            ret = PrintCCString(iobuff, "!! too big > 10000 characters!!", fd);
        }
        else {

            ret = (desSize >= gap);
            if (ret) {

                bool isPadding = (paddingSize > gap);
                if (!isPadding) {
                    streamSizeL -= (gap - paddingSize);
                }

                //if right aligned put the padding at the beginning
                if ((!fd.leftAligned) && (isPadding)) {
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
                    if (!stream->Read(&c, size)) {
                        ret = false;
                    }
                    if (!iobuff.PutC(c)) {
                        ret = false;
                    }
                    streamSizeL--;
                }

                if (ret) {

                    //if left aligned put the padding at the end.
                    if (fd.leftAligned && (isPadding)) {
                        for (uint32 i = 0u; i < paddingSize; i++) {
                            if (!iobuff.PutC(' ')) {
                                ret = false;
                            }
                        }
                    }
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "IOBuffer: Not Enough space for double quotes");
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "IOBuffer: The stream is not seekable");
        ret = PrintCCString(iobuff, "!!stream !seek!!", fd);
    }

    return ret;
}


} //MARTe
