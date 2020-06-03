/**
 * @file ReferenceContainer.cpp
 * @brief Source file for class ReferenceContainer
 * @date 03/11/2018
 * @author Andre Neto
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
 * the class ReferenceContainer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "ReferenceT.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
bool ReferenceContainer::ExportData(StructuredDataI & data) {
    bool ret = Object::ExportData(data);
    if (ret) {
        uint32 numberOfChildren = Size();
        for (uint32 i = 0u; (i < numberOfChildren) && (ret); i++) {
            StreamString nname;
            ret = nname.Printf("%d", i);
            if (ret) {
                ret = data.CreateRelative(nname.Buffer());
            }
            Reference child;
            if (ret) {
                child = Get(i);
                ret = child.IsValid();
            }
            if (ret) {
                ReferenceT<ReferenceContainer> childRC = child;
                //Do not go recursive
                if (childRC.IsValid()) {
                    ret = child->Object::ExportData(data);
                    if (ret) {
                        ret = data.Write("IsContainer", 1);
                    }
                }
                else {
                    ret = child->ExportData(data);
                }
            }
            if (ret) {
                ret = data.MoveToAncestor(1u);
            }
        }
    }
    return ret;
}
}
