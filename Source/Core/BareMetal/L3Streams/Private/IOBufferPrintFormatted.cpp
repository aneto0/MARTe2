/**
 * @file IOBufferPrintFormatted.cpp
 * @brief Implementation of IOBuffer::PrintFormatted
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

namespace MARTe{


/*---------------------------------------------------------------------------*/
/*                           Method implementations                          */
/*---------------------------------------------------------------------------*/

bool IOBuffer::PrintFormatted(IOBuffer &iob, CCString format, const AnyType pars[]){

    bool ret = true;
    bool quit = false;
    // indicates active parameter
    int32 parsIndex = 0;
    // checks silly parameter
    if (!(format.IsNullPtr())) {

        // loops through parameters
        while (!quit) {
            // scans for % and in the meantime prints what it encounters
            while ((format[0] != '\0') && (format[0] != '%')) {
                if (!iob.PutC(format[0])) {
                    ret = false;
                    quit = true;
                }
                format++;
            }
            if (ret) {

                // end of format
                if (format[0] == '\0') {
                    quit = true;
                }
                else {

                    // consume %
                    format++;

                    // if not end then %
                    // keep on parsing format to build a FormatDescriptor
                    FormatDescriptor fd;
                    if (!fd.InitialiseFromString(format)) {
                        ret = false;
                        quit = true;
                    }
                    else {
                        if (!PrintAnyType(iob, fd, pars[parsIndex])){
                            ret = false;
                            quit = true;
                        }
#if 0
                        // if void simply show type
                        if (pars[parsIndex].IsValid()) {
                            // use it to process parameters
                            if (!PrintAnyType(iob, fd, pars[parsIndex])){
                                ret = false;
                                quit = true;
                            }
                        } else {
                            if (!PrintAnyTypeInfo(iob, fd, pars[parsIndex])){
                                ret = false;
                                quit = true;
                            }
                        }
#endif
                        parsIndex++;
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


} //MARTe
