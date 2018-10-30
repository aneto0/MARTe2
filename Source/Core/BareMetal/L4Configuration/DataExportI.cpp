/**
 * @file DataExportI.cpp
 * @brief Source file for class DataExportI
 * @date 14/09/2018
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
 * the class DataExportI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "DataExportI.h"
#include "ReferenceContainerFilterObjectName.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

DataExportI::DataExportI() {
}

DataExportI::~DataExportI() {
}

bool DataExportI::GetAsStructuredData(StreamStructuredDataI &data, ProtocolI &protocol) {
    ReferenceT<DataExportI> ref = this;
    return GetReferenceAsStructuredData(data, protocol, ref);
}

bool DataExportI::GetReferenceAsStructuredData(StreamStructuredDataI &data, ProtocolI &protocol, Reference ref) {
    ReferenceT<DataExportI> targetDataExportI = ref;
    bool ok = true;
    ReferenceT<DataExportI> thisRef = this;
    bool isSelf = (ref == thisRef);
    if ((targetDataExportI.IsValid()) && (!isSelf)) {
        ok = targetDataExportI->GetAsStructuredData(data, protocol);
    }
    //Not a DataExportI
    else {
        ReferenceT<ReferenceContainer> targetReferenceContainer = ref;
        if (targetReferenceContainer.IsValid()) {
            ok = GetObjectPropertiesAsStructuredData(data, protocol, targetReferenceContainer);
            //List all the objects
            uint32 n;
            uint32 nObjs = targetReferenceContainer->Size();
            for (n = 0u; (n < nObjs) && (ok); n++) {
                StreamString id;
                (void) id.Printf("%d", n);
                ReferenceT<Object> obj = targetReferenceContainer->Get(n);
                ok = data.CreateRelative(id.Buffer());
                if (ok) {
                    ok = GetObjectPropertiesAsStructuredData(data, protocol, obj);
                }
                if (ok) {
                    ok = data.MoveToAncestor(1u);
                }
            }
        }
        else {
            //Just dump the object properties
            ok = GetObjectPropertiesAsStructuredData(data, protocol, ref);
        }
    }
    return ok;
}

bool DataExportI::GetObjectPropertiesAsStructuredData(StreamStructuredDataI &data, ProtocolI &protocol, Reference targetObject) {
    bool ok = data.CreateRelative("props");
    if (ok) {
        data.Write("name", targetObject->GetName());
    }
    if (ok) {
        ok = data.Write("class", targetObject->GetClassProperties()->GetName());
    }
    if (ok) {
        ok = data.Write("version", targetObject->GetClassProperties()->GetVersion());
    }
    if (ok) {
        ok = data.Write("size", targetObject->GetClassProperties()->GetSize());
    }
    if (ok) {
        ok = data.Write("refcounter", targetObject->NumberOfReferences());
    }
    if (ok) {
        StreamString path;
        protocol.GetPath(path);
        ok = data.Write("path", path.Buffer());
    }
    if (ok) {
        ok = data.MoveToAncestor(1u);
    }
    return ok;

}

Reference DataExportI::FindReference(ProtocolI &protocol, Reference root) {
    StreamString unmatchedPath;
    protocol.GetUnmatchedId(unmatchedPath);
    Reference target;
    if (unmatchedPath.Size() > 0LLU) {
        ReferenceT<ReferenceContainer> rootReferenceContainer = root;
        bool ok = rootReferenceContainer.IsValid();
        if (ok) {
            // search for destination
            int32 occurrences = 1;
            uint32 mode = ReferenceContainerFilterMode::PATH;
            ReferenceContainerFilterObjectName filter(occurrences, mode, unmatchedPath.Buffer());
            ReferenceContainer results;
            rootReferenceContainer->Find(results, filter);
            if (results.Size() > 0ull) {
                uint32 last = static_cast<uint32>(results.Size()) - 1u;
                target = results.Get(last);
            }
            ok = target.IsValid();
            if (ok) {
                protocol.SetUnmatchedId("");
            }
            else {
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Target object [%s] not found", unmatchedPath.Buffer());
            }
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Object browsing not supported (it is not a reference container)");
        }
    }
    else {
        target = root;
    }
    return target;
}

}
