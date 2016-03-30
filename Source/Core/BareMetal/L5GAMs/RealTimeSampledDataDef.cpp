/**
 * @file RealTimeSampledDataDef.cpp
 * @brief Source file for class RealTimeSampledDataDef
 * @date 25/feb/2016
 * @author pc
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
 * the class RealTimeSampledDataDef (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeSampledDataDef.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

RealTimeSampledDataDef::RealTimeSampledDataDef() :
        RealTimeDataDefI() {
    samples = 0;
    samplesPerCycle = 0;
    final = false;
}

bool RealTimeSampledDataDef::Verify() {

    bool ret = (type != "");
    if (ret) {
        // if a basic type return true
        if (TypeDescriptor::GetTypeDescriptorFromTypeName(type.Buffer()) == InvalidType) {
            // unsupported multi-dimensional structures
            ret = (numberOfDimensions == 0u);
            if (ret) {
                // structured type
                const ClassRegistryItem * item = ClassRegistryDatabase::Instance()->Find(type.Buffer());
                ret = (item != NULL);
                if (ret) {
                    /*lint -e{613} NULL pointer checking done before entering here */
                    const Introspection * intro = item->GetIntrospection();
                    ret = (intro != NULL);
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Type %s not introspectable", type.Buffer())
                    }
                }
                else {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Type %s not registered", type.Buffer())
                }
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Unsupported multi-dimensional structures for %s defined with type %s", GetName(),
                                        GetType())

            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "The type cannot be empty");
    }
    return ret;
}

bool RealTimeSampledDataDef::MergeWithLocal(StructuredDataI &localData) {
    bool ret = (!final);
    if (ret) {
        if (type == "") {
            if (!localData.Read("Type", type)) {
            }
        }

        if (path == "") {
            if (!localData.Read("Path", path)) {
            }
        }

        if (samples == 0) {
            if (!localData.Read("Samples", samples)) {
            }
        }

        if (modifiers == "") {
            if (localData.Read("Modifiers", modifiers)) {
                // use introspection entry to parse the modifiers
                IntrospectionEntry entry("", "", modifiers.Buffer(), "", 0u, 0u);
                numberOfDimensions = entry.GetNumberOfDimensions();
                for (uint32 i = 0u; i < 3u; i++) {
                    numberOfElements[i] = entry.GetNumberOfElements(i);
                }
            }
        }

        if (samplesPerCycle == 0) {
            if (!localData.Read("SamplesPerCycle", samplesPerCycle)) {
            }
        }
    }

    return ret;
}

bool RealTimeSampledDataDef::Initialise(StructuredDataI &data) {
    bool ret = RealTimeDataDefI::Initialise(data);
    if (ret) {
        if (data.Read("Samples", samples)) {

        }
        if (data.Read("SamplesPerCycle", samplesPerCycle)) {

        }

        StreamString isFinal;
        if (data.Read("IsFinal", isFinal)) {
            final = (isFinal == "true");
        }
    }
    return ret;
}

int32 RealTimeSampledDataDef::GetSamples() const {
    return samples;
}

int32 RealTimeSampledDataDef::GetSamplesPerCycle() const {
    return samplesPerCycle;
}

bool RealTimeSampledDataDef::ToStructuredData(StructuredDataI & data) {
    const char8 * objName = GetName();
    bool ret = (data.CreateRelative(objName));
    if (ret) {
        ret = data.Write("Class", "RealTimeSampledDataDef");
        if (ret) {
            if (type != "") {
                ret = data.Write("Type", type);
            }
        }
        if (ret) {
            if (path != "") {
                ret = data.Write("Path", path);
            }
        }
        if (ret) {
            if (numberOfDimensions > 0u) {
                ret = data.Write("NumberOfDimensions", numberOfDimensions);
                if (ret) {
                    ret = data.Write("NumberOfElements", numberOfElements);
                }
            }
        }
        if (ret) {
            ret = data.Write("Samples", samples);
        }
        if (ret) {
            ret = data.Write("SamplesPerCycle", samplesPerCycle);
        }

        if (!data.MoveToAncestor(1u)) {
            ret = false;
        }
    }
    return ret;
}

CLASS_REGISTER(RealTimeSampledDataDef, "1.0")

}
