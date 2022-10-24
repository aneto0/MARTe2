/**
 * @file PrinterI.cpp
 * @brief Source file for class PrinterI
 * @date 07/09/2018
 * @author Giuseppe Ferro
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
 * the class PrinterI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "PrinterI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static bool PrinterIIsNodeAnArray(const char8 * const nodeName) {
    const char8 *nodeNameStr = StringHelper::SearchChar(nodeName, '[');
    return (nodeNameStr != NULL_PTR(const char8 *));
}

static bool PrinterIGetNodeArrayName(const char8 * const nodeName, StreamString &nodeArrayName) {
    StreamString temp = nodeName;
    char8 term;
    bool ok = temp.Seek(0LLU);
    if (ok) {  
        ok = temp.GetToken(nodeArrayName, "[", term);
    }
    return ok;
}

static uint32 PrinterIGetNodeArraySize(StructuredDataI &sdi, const char8 * const nodeArrayName) {
    uint32 size = sdi.GetNumberOfChildren();
    bool done = (size == 0u);
    while (!done) {
        StreamString fullName = "";
        (void) fullName.Printf("%s[%d]", nodeArrayName, size);
        done = sdi.MoveRelative(fullName.Buffer());
        if (done) {
            (void) sdi.MoveToAncestor(1u);
        }
        else {
            size--;
            done = (size == 0u);
        }
    }
    return (size + 1u);
}
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

PrinterI::PrinterI() {
    stream = NULL_PTR(BufferedStreamI*);
}

PrinterI::PrinterI(BufferedStreamI & streamIn) {
    stream = &streamIn;
}

PrinterI::~PrinterI() {
    stream = NULL_PTR(BufferedStreamI*);
}

void PrinterI::SetStream(BufferedStreamI & streamIn) {
    stream = &streamIn;
}

bool PrinterI::PrintBegin() {
    return true;
}

bool PrinterI::PrintEnd() {
    return true;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. blockName is not required by the default implementation.*/
bool PrinterI::PrintOpenArrayOfBlocks(const char8 * const blockName) {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. blockName is not required by the default implementation.*/
bool PrinterI::PrintCloseArrayOfBlocks(const char8 * const blockName) {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. blockName is not required by the default implementation.*/
bool PrinterI::PrintOpenInsideArrayOfBlocks(const char8 * const blockName) {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. blockName is not required by the default implementation.*/
bool PrinterI::PrintCloseInsideOfArrayOfBlocks(const char8 * const blockName) {
    return false;
}

bool PrinterI::PrintStructuredDataI(StructuredDataI &sdi, const bool collapseArraysOfNodes, const uint32 level) {
    bool ok = true;
    if (level == 0u) {
        ok = PrintBegin();
    }
    if (ok) {
        bool previousWasBlock = false;
        bool previousWasVariable = false;
        bool needsToCloseArrayOfNodes = false;
        uint32 numberOfArrayNodesToAdd = 0u;
        StreamString nodeNameNoArray;
        for (uint32 n=0u; n<sdi.GetNumberOfChildren(); n++) {
            const char8 * const nodeName = sdi.GetChildName(n);
            if (previousWasBlock) {
                ok = PrintBlockSeparator();
            }
            else if (previousWasVariable) {
                ok = PrintVariableSeparator();
            }
            else {
                //NOOP
            }
            if (sdi.MoveRelative(nodeName)) {
                previousWasBlock = true;
                previousWasVariable = false;
                if (collapseArraysOfNodes) {
                    if (numberOfArrayNodesToAdd > 0u) {
                        ok = PrintOpenInsideArrayOfBlocks(nodeNameNoArray.Buffer());
                        numberOfArrayNodesToAdd--;
                    }
                    else {
                        if (PrinterIIsNodeAnArray(nodeName)) {
                            nodeNameNoArray = "";
                            ok = PrinterIGetNodeArrayName(nodeName, nodeNameNoArray);
                            if (ok) {
                                ok = sdi.MoveToAncestor(1u);
                            }
                            if (ok) {
                                numberOfArrayNodesToAdd = PrinterIGetNodeArraySize(sdi, nodeNameNoArray.Buffer());
                            }
                        }
                        if (ok) {
                            if (numberOfArrayNodesToAdd > 0u) {
                                StreamString firstNodeName;
                                (void) firstNodeName.Printf("%s[0]", nodeNameNoArray.Buffer());
                                ok = sdi.MoveRelative(firstNodeName.Buffer());
                                if (ok) {
                                    ok = PrintOpenArrayOfBlocks(nodeNameNoArray.Buffer());
                                    needsToCloseArrayOfNodes = true;
                                }
                                if (ok) {
                                    ok = PrintOpenInsideArrayOfBlocks(nodeName);
                                    if (!ok) {
                                        REPORT_ERROR_STATIC_0(ErrorManagement::UnsupportedFeature, "Arrays of blocks are not supported by this Printer");
                                    }
                                }
                                if (ok) {
                                    numberOfArrayNodesToAdd--;
                                }
                            }
                            else {
                                if (ok) {
                                    ok = PrintOpenBlock(nodeName);
                                }
                            }
                        }
                    }
                }
                else {
                    if (ok) {
                        ok = PrintOpenBlock(nodeName);
                    }
                }
                if (ok) {
                    ok = PrintStructuredDataI(sdi, collapseArraysOfNodes, level + 1u);
                }
                if (ok) {
                    if (needsToCloseArrayOfNodes) {
                        ok = PrintCloseInsideOfArrayOfBlocks(nodeName);
                    }
                    else {
                        ok = PrintCloseBlock(nodeName);
                    }
                }
                if (ok) {
                    if (needsToCloseArrayOfNodes) {
                        if (numberOfArrayNodesToAdd == 0u) {
                            ok = PrintCloseArrayOfBlocks(nodeName);
                            needsToCloseArrayOfNodes = false;
                        }
                    }
                }
                if (ok) {
                    ok = sdi.MoveToAncestor(1u);
                }
            }
            else {
                previousWasBlock = false;
                previousWasVariable = true;
                if (ok) {
                    //use custom component to print
                    ok = PrintOpenAssignment(nodeName);
                }
                if (ok) {
                    ok = PrintVariable(sdi.GetType(nodeName));
                }
                if (ok) {
                    ok = PrintCloseAssignment(nodeName);
                }
            }
        }
    }
    if (ok) {
        if (level == 0u) {
            ok = PrintEnd();
        }
    }
    return ok;
}

}

