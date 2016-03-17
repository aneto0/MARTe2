/**
 * @file RealTimeDataDefI.cpp
 * @brief Source file for class RealTimeDataDefI
 * @date 25/02/2016
 * @author Giuseppe Ferrò
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
 * the class RealTimeDataDefI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataDefI.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
RealTimeDataDefI::RealTimeDataDefI() {
    numberOfDimensions = 0u;

    for (uint32 i = 0u; i < 3u; i++) {
        numberOfElements[i] = 1u;
    }

}

const char8 *RealTimeDataDefI::GetPath() {
    return path.Buffer();
}

const char8 *RealTimeDataDefI::GetType() {
    return type.Buffer();
}

const char8 *RealTimeDataDefI::GetDefaultValue() {
    return defaultValue.Buffer();
}

bool RealTimeDataDefI::Initialise(StructuredDataI &data) {

    bool ret = ReferenceContainer::Initialise(data);

    if (ret) {
        if (!data.Read("Path", path)) {
            //TODO Warning?
        }
        if (!data.Read("Type", type)) {
            //TODO Warning?
        }

        if (!data.Read("Default", defaultValue)) {
            //TODO Warning?
        }

        if (data.Read("Modifiers", modifiers)) {
            // use introspection entry to parse the modifiers
            IntrospectionEntry entry("", "", modifiers.Buffer(), "", 0u, 0u);
            numberOfDimensions = entry.GetNumberOfDimensions();
            for (uint32 i = 0u; i < 3u; i++) {
                numberOfElements[i] = entry.GetNumberOfElements(i);
            }
            if (entry.GetMemberPointerLevel() > 0u) {
                REPORT_ERROR(ErrorManagement::Warning, "Pointers not supported. The statement will be ignored");
            }
        }

    }
    return ret;
}

uint8 RealTimeDataDefI::GetNumberOfDimensions() const {
    return numberOfDimensions;

}

uint32 RealTimeDataDefI::GetNumberOfElements(uint32 dimension) {
    uint32 dimensionNumber = dimension;
    return (dimensionNumber < 3u) ? numberOfElements[dimensionNumber] : numberOfElements[2];
}

void RealTimeDataDefI::SetPath(const char8 * newPath) {
    if (path != "") {
        REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Path already set to %s", path.Buffer())
    }
    path = newPath;
}

}
