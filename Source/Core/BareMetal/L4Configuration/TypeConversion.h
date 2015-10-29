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
    uint16 t1 = source.GetTypeDescriptor().all;
    uint16 t2 = destination.GetTypeDescriptor().all;
    printf("source = %u destination = %u\n", t1, t2);
    //TODO must check if the dimensions and the number of elements are the same...
    if (destination.GetTypeDescriptor() == source.GetTypeDescriptor()) {
        uint32 typeSize = source.GetTypeDescriptor().numberOfBits / 8u;
        uint32 nOfDimensions = source.GetNumberOfDimensions();
        uint32 copySize = 0u;
        if (nOfDimensions > 0) {
            uint32 i;
            for (i = 0u; i < nOfDimensions; i++) {
                printf("source.GetNumberOfElements(%d) = %d\n", i, source.GetNumberOfElements(i));
                copySize += source.GetNumberOfElements(i) * typeSize;
            }
        }
        else {
            copySize = typeSize;
        }
        printf("copySize = [%d]\n", copySize);
        if (!MemoryOperationsHelper::Copy(destination.GetDataPointer(), source.GetDataPointer(), copySize)) {
            ok = false;
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

