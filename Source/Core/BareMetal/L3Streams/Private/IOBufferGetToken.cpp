/**
 * @file IOBufferGetToken.cpp
 * @brief Implementation of IOBuffer::GetToken
 * @date Aug 3, 2020
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

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#define DLL_API

#include "IOBufferPrivate.h"
#include "AdvancedErrorManagement.h"
#include "FormatDescriptor.h"

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                           Static implementations                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool IOBuffer::GetToken(IOBuffer &  inputBuffer,
                        CStringTool &outputBuffer,
                        CCString    terminator,
                        char8       &saveTerminator,
                        CCString    skipCharacters) {

    bool ret = true;
    bool quit = false;
// need space for trailing 0

    if (skipCharacters.GetSize() ==0 ) {
        skipCharacters = terminator;
    }

    uint32 tokenSize = 0u;
    while (!quit) {
        char8 c;
        if (!inputBuffer.GetC(c)) {

            // 0 terminated string
//            outputBuffer[tokenSize] = '\0';

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

            bool isTerminator = (terminator.In(c));
            bool isSkip = skipCharacters.In(c);

            if ((isTerminator) || (c == '\0')) {

                // quit only if some data was read, otw just skip separator block
                if ((tokenSize != 0u) || (!isSkip)) {
                    // 0 terminated string
  //                  outputBuffer[tokenSize] = '\0';

                    saveTerminator = c;

                    quit = true;
                }
            }
            //isTerminator is false here
            else {
                if (!isSkip) {

                    ErrorManagement::ErrorType ok = outputBuffer.Append(c);
                    if (!ok){
                        saveTerminator = c;
                        quit = true;
                        ret = false;
                    }
                    tokenSize++;
/*
                    if (tokenSize >= outputBufferSize) {
                        // 0 terminated string
                        outputBuffer[tokenSize] = '\0';

                        saveTerminator = c;

                        quit = true;
                    }
*/
                }
            }
        }
    }

    return ret;
}

bool IOBuffer::GetToken(
            IOBuffer &inputBuffer,
            IOBuffer & outputBuffer,
            CCString  terminator,
            char8 &saveTerminator,
            CCString skipCharacters){


    if (skipCharacters.IsNullPtr()) {
        skipCharacters = terminator;
    }

    bool ret = true;
    bool quit = false;
    uint32 tokenSize = 0u;
    while (!quit) {
        char8 c;
        if (!inputBuffer.GetC(c)) {

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

            bool isTerminator = (terminator.In(c));
            bool isSkip = skipCharacters.In(c);

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

bool IOBuffer::SkipTokens(IOBuffer &iob, uint32 count, CCString terminator){

    bool ret = true;
    uint32 tokenSize = 0u;
    while (count > 0u) {
        char8 c;
        if (!iob.GetC(c)) {
            ret = false;
            //quit
            count = 0u;
        }
        else {

            //
            if (terminator.In(c) || (c == '\0')) {
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


} //MARTe
