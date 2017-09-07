/**
 * @file IOBufferStringPrint.cpp
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


#include "IOBufferPrivate.h"
#include "CCString.h"

namespace MARTe{

/**
 * @brief Prints a CCString .
 * @param[out] iobuff is the output stream buffer.
 * @param[in] string is the string to be printed.
 * @param[in] fd specifies the desired format for the string.
 * @return true if the string is printed correctly.
 */
 bool PrintCCString(      IOBuffer & iobuff,
                          CCString const string,
                          const FormatDescriptor &fd,
                          bool addQuotesOnString) {

    bool ret = (!string.IsNullPtr());

    if (ret) {
        //get the string size
        uint32 stringSize = string.GetSize();
        uint32 paddingSize = 0u;
        uint32 desSize = fd.size;
        // consider the quotes
        uint32 gap = (addQuotesOnString) ? (2u) : (0u);

        //is the desired size is 0 print completely the string without padd.
        if (desSize > 0u) {
            //clip the string size if the desired size is minor.
            if (stringSize > desSize) {
                stringSize = desSize;
            }

            //if padded and desired size is greater than the string size
            //the difference is the padding size.
            bool isPadded = fd.padded;
            if (isPadded) {
                if (stringSize < desSize) {
                    paddingSize = (desSize - stringSize);
                }
            }
        }
        else {
            stringSize += gap;
            desSize = stringSize;
        }

        // check if there is enough space
        ret = (desSize >= gap);
        if (ret) {

            // remove the quotes from the padding if necessary
            bool isPaddingSize = (paddingSize > gap);

            // otherwise remove it from the string size
            if (!isPaddingSize) {
                stringSize -= (gap - paddingSize);
            }

            bool isLeftAligned = fd.leftAligned;

            //if right aligned put the padding at the beginning
            if ((!isLeftAligned) && (isPaddingSize)) {
                for (uint32 i = 0u; i < paddingSize; i++) {
                    if (!iobuff.PutC(' ')) {
                        ret = false;
                    }
                }
            }

            if (addQuotesOnString) {
                if (!iobuff.PutC('\"')) {
                    ret = false;
                }
            }

            //print the string on the buffer completely.
            if (!iobuff.WriteAll(string.GetList(), stringSize)) {
                ret = false;
            }

            if (addQuotesOnString) {
                if (!iobuff.PutC('\"')) {
                    ret = false;
                }
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
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "IOBuffer: Not Enough space for double quotes");
        }
    }

    return ret;
}

}// MARTe
