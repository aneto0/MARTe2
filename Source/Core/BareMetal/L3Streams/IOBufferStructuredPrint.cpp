/**
 * @file IOBufferStructuredPrint.cpp
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

#include "StructuredDataI.h"
#include "IOBufferPrivate.h"

namespace MARTe{

/**
 * @brief Prints a StructuredDataInterface.
 * @param[out] iobuff is the output stream buffer.
 * @param[in] structuredData is the input.
 * @return false in case of error in the StructuredDataI functions, true otherwise.
 */

static bool PrintStructuredDataInterface(IOBuffer &iobuff,
                                         StructuredDataI * const structuredData,
                                         uint32 nodeLevel) {
    bool ret = true;
    uint32 numberOfChildren = structuredData->GetNumberOfChildren();
    for (uint32 i = 0u; (i < numberOfChildren) && (ret); i++) {
        CCString childName = structuredData->GetChildName(i);

        AnyType toPrint = structuredData->GetType(childName);
        if (toPrint.GetVariablePointer() != NULL) {
            uint32 j;
            for (j = 0u; (j < nodeLevel) && (ret); j++) {
                AnyType noneType = voidAnyType;
                ret = (IOBuffer::PrintFormatted(iobuff,"    ", &noneType));
            }
            AnyType printLeftSide[] = {childName, "= ", voidAnyType};
            ret = (IOBuffer::PrintFormatted(iobuff,"%s %s", &printLeftSide[0]));
            if (ret) {
                AnyType printLeaf[] = {toPrint, voidAnyType};
                ret = (IOBuffer::PrintFormatted(iobuff,"%#!\r\n", &printLeaf[0]));
            }
        }
        else {
            // a node
            if (structuredData->MoveRelative(childName)) {
                uint32 j;
                for (j = 0u; (j < nodeLevel) && (ret); j++) {
                    AnyType noneType = voidAnyType;
                    ret = (IOBuffer::PrintFormatted(iobuff,"    ", &noneType));
                }
                AnyType printLeftSide[] = {childName, "= {", voidAnyType};
                ret = (IOBuffer::PrintFormatted(iobuff,"%s %s\r\n", &printLeftSide[0]));
                if (ret) {
                    nodeLevel++;
                    ret = PrintStructuredDataInterface(iobuff, structuredData, nodeLevel);
                }
                if (ret) {
                    ret = (structuredData->MoveToAncestor(1u));
                    nodeLevel--;
                }
                if (ret) {
                    for (j = 0u; (j < nodeLevel) && (ret); j++) {
                        AnyType noneType = voidAnyType;
                        ret = (IOBuffer::PrintFormatted(iobuff,"    ", &noneType));
                    }
                    AnyType printClose[] = {"}", voidAnyType};
                    ret = (IOBuffer::PrintFormatted(iobuff,"%s\r\n", &printClose[0]));
                }
            }
        }
    }

    return ret;
}


} //MARTe
