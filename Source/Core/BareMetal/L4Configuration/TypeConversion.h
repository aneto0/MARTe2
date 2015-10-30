/**
 * @file TypeConversion.h
 * @brief Header file for class TypeConversion
 * @date Oct 28, 2015
 * @author aneto
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

 * @details This header file contains the declaration of the class TypeConversion
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TYPECONVERSION_H_
#define TYPECONVERSION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "AnyType.h"
#include "StreamI.h"
#include "StringHelper.h"
#include "FormatDescriptor.h"
#include "MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static bool TypeConvert(const AnyType &destination,
                        const AnyType &source) {
    bool ok = true;
    uint32 nOfDimensions = source.GetNumberOfDimensions();
    uint32 copySize = 0u;
    uint32 totalElements = 1u;

    uint32 i;
    for (i = 0u; i < nOfDimensions; i++) {
        if (source.GetNumberOfElements(i) != 0u) {
            totalElements *= source.GetNumberOfElements(i);
        }
    }

    //TODO must check if the dimensions and the number of elements are the same in source and destination...
    if (destination.GetTypeDescriptor() == source.GetTypeDescriptor()) {
        uint32 typeSize = source.GetTypeDescriptor().numberOfBits / 8u;

        if (source.GetTypeDescriptor().type == Stream) {
            copySize = source.GetNumberOfElements(2u);
        }
        else if ((nOfDimensions == 0u) && (source.GetTypeDescriptor().type == CCString)) {
            copySize = (StringHelper::Length(static_cast<const char *>(source.GetDataPointer())) + 1u);
        }
        else {
            copySize = typeSize * totalElements;
        }

        if (source.GetTypeDescriptor().type == Stream) {
            StreamI *stream = static_cast<StreamI *>(destination.GetDataPointer());
            ok = stream->Seek(0u);
            if (ok) {
                stream->Write(static_cast<char8 *>(source.GetDataPointer()), copySize);
            }
        }
        //TODO should we support tables of strings?
        else if ((nOfDimensions == 1u) && (source.GetTypeDescriptor().type == CCString)) {
            uint32 j;
            char **destArray = static_cast<char **>(destination.GetDataPointer());
            const char **sourceArray = static_cast<const char **>(source.GetDataPointer());
            //TODO .GetNumberOfElements(0u) should walk all the dimension
            for (j = 0; j < source.GetNumberOfElements(0u); j++) {
                copySize = (StringHelper::Length(sourceArray[j]) + 1u);
                if (!MemoryOperationsHelper::Copy(static_cast<void *>(destArray[j]), static_cast<const void *>(sourceArray[j]), copySize)) {
                    //TODO
                }
            }
        }
        else {
            if (!MemoryOperationsHelper::Copy(destination.GetDataPointer(), source.GetDataPointer(), copySize)) {
                ok = false;
            }
        }
    }
    else {
        ok = false;
    }

    return ok;
}

static bool TypeConvert(AnyType &destination,
                        const AnyType& source,
                        const FormatDescriptor &conversionFormat) {
    return false;
}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TYPECONVERSION_H_ */

