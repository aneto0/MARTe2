/**
 * @file PrintStructuredData.cpp
 * @brief Header file for class AnyType
 * @date Jul 9, 2020
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

#include "DynamicCString.h"
#include "StructuredDataI.h"
#include "IOBuffer.h"
#include "CompositeErrorManagement.h"


namespace MARTe{


struct StructuredDataPrintTemplate{

    /**
     *
     */
    CCString openMatrix;

    /**
     *
     */
    CCString closeMatrix;

    /**
     *
     */
    CCString scalarSeparator;

    /**
     *
     */
    CCString vectorSeparator;

    /**
     *
     */
    CCString variableSeparator;

    /**
     *
     */
    CCString blockSeparator;

    /**
     *
     */
    CCString openVector;

    /**
     *
     */
    CCString closeVector;

    /**
     *
     */
    CCString openBlockPre;

    /**
     *
     */
    CCString openBlockPost;

    /**
     *
     */
    CCString closeBlockPre;

    /**
     *
     */
    CCString closeBlockPost;

    /**
     *
     */
    CCString assignment;
};

const StructuredDataPrintTemplate standardStructuredDataPrintTemplate = {
        "{",
        "}",
        " ",
        "",
        "",
        "",
        "{",
        "}",
        "",
        " = {",
        "",
        "}",
        "="
};

const StructuredDataPrintTemplate xmlStructuredDataPrintTemplate = {
        "[",
        "]",
        ",",
        ",",
        ",",
        ",",
        "[",
        "]",
        "\"",
        "\": {",
        "}",
        "",
        "="
};






static void Indent(IOBuffer &iobuff,uint32 n){
    uint32 j;
    for (j = 0u; (j < (n*4)) ; j++) {
        AnyType noneType = voidAnyType;
        iobuff.PutC(' ');
    }
}
/**
 * @brief Prints a StructuredDataInterface.
 * @param[out] iobuff is the output stream buffer.
 * @param[in] structuredData is the input.
 * @return false in case of error in the StructuredDataI functions, true otherwise.
 */
static bool PrintStructuredDataInterface(IOBuffer &iobuff, StructuredDataI * const structuredData, const FormatDescriptor &fd, uint32 nodeLevel = 0u) {
    ErrorManagement::ErrorType ret;
    uint32 numberOfChildren = structuredData->GetNumberOfChildren();
    for (uint32 i = 0u; (i < numberOfChildren) && (ret); i++) {
        DynamicCString childName;
        ret = structuredData->GetChildName(i,childName);
        COMPOSITE_REPORT_ERROR(ret,"GetChildName ",i, " failed");

        TypeDescriptor td;
        uint32 dimensions[2];
        uint32 nDim = 2;
        if (ret){
            ret  = structuredData->GetVariableInformation(childName,td,nDim,dimensions);
            COMPOSITE_REPORT_ERROR(ret,"GetVariableInformation ",childName, " failed");
        }

        if (ret) {
            Indent(iobuff,nodeLevel);

            ret = PrintOpenAssignment(iobuff, childName, fd);
            if (ret) {
                AnyType noneType = voidAnyType;
                ret = (iobuff.PrintFormatted(" ", &noneType));
            }
            //AnyType printLeftSide[] = {childName, "= ", voidAnyType};
            //ret = (iobuff.PrintFormatted("%s %s", &printLeftSide[0]));
            if (ret) {
                AnyType printLeaf[] = { toPrint, voidAnyType };
                ret = (iobuff.PrintFormatted("%#!", &printLeaf[0]));
            }
            if (ret) {
                ret = PrintCloseAssignment(iobuff, childName, fd);
            }
            if (ret) {
                if (i < (numberOfChildren - 1u)) {
                    ret = PrintVariableSeparator(iobuff, fd);
                }
                if (ret) {
                    AnyType noneType = voidAnyType;
                    ret = (iobuff.PrintFormatted("\r\n", &noneType));
                }
            }
        }
        else {
            // a node
            if (structuredData->MoveRelative(childName)) {
                uint32 j;
                for (j = 0u; (j < nodeLevel) && (ret); j++) {
                    AnyType noneType = voidAnyType;
                    ret = (iobuff.PrintFormatted("    ", &noneType));
                }
                //AnyType printLeftSide[] = {childName, "= {", voidAnyType};
                //ret = (iobuff.PrintFormatted("%s %s\r\n", &printLeftSide[0]));
                ret = PrintOpenBlock(iobuff, childName, fd);
                if (ret) {
                    AnyType noneType = voidAnyType;
                    ret = (iobuff.PrintFormatted("\r\n", &noneType));
                }
                if (ret) {
                    nodeLevel++;
                    ret = PrintStructuredDataInterface(iobuff, structuredData, fd, nodeLevel);
                }
                if (ret) {
                    ret = (structuredData->MoveToAncestor(1u));
                    nodeLevel--;
                }
                if (ret) {
                    for (j = 0u; (j < nodeLevel) && (ret); j++) {
                        AnyType noneType = voidAnyType;
                        ret = (iobuff.PrintFormatted("    ", &noneType));
                    }
                    //AnyType printClose[] = {"}", voidAnyType};
                    //ret = (iobuff.PrintFormatted("%s\r\n", &printClose[0]));
                    ret = PrintCloseBlock(iobuff, childName, fd);
                    if (ret) {
                        if (i < (numberOfChildren - 1u)) {
                            ret = PrintBlockSeparator(iobuff, fd);
                        }
                        if (ret) {
                            AnyType noneType = voidAnyType;
                            ret = (iobuff.PrintFormatted("\r\n", &noneType));
                        }
                    }
                }
            }
        }
    }

    return ret;
}


}










} //MARTe
