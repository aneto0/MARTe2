/**
 * @file ObjectBrower.cpp
 * @brief Source file for class ObjectBrower
 * @date 29/10/2018
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
 * the class ObjectBrower (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "JsonPrinter.h"
#include "ObjectBrowser.h"
#include "ObjectRegistryDatabase.h"
#include "ProtocolI.h"
#include "ReferenceContainerFilterNameAndType.h"
#include "StreamStructuredData.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

ObjectBrowser::ObjectBrowser() :
        ReferenceContainer() {
}

ObjectBrowser::~ObjectBrowser() {
    root = Reference();
}

void ObjectBrowser::Purge(ReferenceContainer &purgeList) {
    root = Reference();
    ReferenceContainer::Purge(purgeList);
}

bool ObjectBrowser::Initialise(StructuredDataI &data) {
    bool ok = ReferenceContainer::Initialise(data);

    StreamString rootName;
    if (ok) {
        ok = data.Read("Root", rootName);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "A Root shall be specified");
        }
    }
    if (ok) {
        if (rootName.Size() == 1u) {
            if (rootName[0] == '/') {
                root = ObjectRegistryDatabase::Instance();
            }
            else if (rootName[0] == '.') {
                root = this;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Unknown Root [%c]", rootName[0]);
            }
        }
        else {
            root = ObjectRegistryDatabase::Instance()->Find(rootName.Buffer());
            ok = root.IsValid();
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Invalid Root [%s]", rootName.Buffer());
            }
        }

    }

    return ok;
}

bool ObjectBrowser::GetAsStructuredData(StreamStructuredDataI &data, ProtocolI &protocol) {
    Reference target = FindTarget(protocol);
    bool ok = target.IsValid();
    if (ok) {
        ok = GetTargetAsStructuredData(data, protocol, target);
    }
    return ok;
}

bool ObjectBrowser::GetTargetAsStructuredData(StreamStructuredDataI &data, ProtocolI &protocol, Reference &target) {
    ReferenceT<DataExportI> targetDataExportI = target;
    bool ok = true;
    bool isSelf = (target == this);
    if ((targetDataExportI.IsValid()) && (!isSelf)) {
        ok = targetDataExportI->GetAsStructuredData(data, protocol);
    }
    //Not a DataExportI
    else {
        ReferenceT<ReferenceContainer> targetReferenceContainer = target;
        if (targetReferenceContainer.IsValid()) {
            //List all the objects
            uint32 n;
            uint32 nObjs = targetReferenceContainer->Size();
            for (n = 0u; (n < nObjs) && (ok); n++) {
                StreamString id;
                (void) id.Printf("%d", n);
                ReferenceT<Object> obj = targetReferenceContainer->Get(n);
                ok = data.CreateRelative(id.Buffer());
                if (ok) {
                    ok = data.Write("name", obj->GetName());
                }
                if (ok) {
                    ok = data.Write("class", obj->GetClassProperties()->GetName());
                }
                if (ok) {
                    ok = data.Write("version", obj->GetClassProperties()->GetVersion());
                }
                if (ok) {
                    ok = data.Write("size", obj->GetClassProperties()->GetSize());
                }
                if (ok) {
                    ok = data.Write("refcounter", obj->NumberOfReferences());
                }
                if (ok) {
                    ok = data.MoveToAncestor(1u);
                }
            }
        }
        else {
            //Just dump the object properties
            ReferenceT<Object> targetObject = target;
            ok = data.Write("name", targetObject->GetName());
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
        }
    }
    return ok;
}

Reference ObjectBrowser::FindTarget(ProtocolI &protocol) {
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
            ReferenceContainerFilterNameAndType<Object> filter(occurrences, mode, unmatchedPath.Buffer());
            ReferenceContainer results;
            rootReferenceContainer->Find(results, filter);
            if (results.Size() > 0ull) {
                uint32 last = static_cast<uint32>(results.Size()) - 1u;
                target = results.Get(last);
            }
            ok = target.IsValid();
            if (ok) {
                // save remainder of address
                uint32 remAddrIndex = filter.GetRemainedAddrIndex();
                ok = (remAddrIndex > unmatchedPath.Size());
                if (ok) {
                    StreamString unmatchedPathNext = &unmatchedPath.Buffer()[remAddrIndex];
                    if (unmatchedPathNext.Size() > 0LLU) {
                        uint32 newUrlLastCharIdx = (static_cast<uint32>(unmatchedPathNext.Size()) - 1u);
                        if (unmatchedPathNext.Buffer()[newUrlLastCharIdx] == '.') {
                            ok = (unmatchedPathNext.SetSize(static_cast<uint64>(newUrlLastCharIdx)));
                        }
                    }
                    protocol.SetUnmatchedId(unmatchedPathNext.Buffer());
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Target object [%s] not found", unmatchedPath.Buffer());
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "Object browsing not supported (it is not a reference container)");
        }
    }
    else {
        target = root;
    }
    return target;
}

bool ObjectBrowser::GetAsText(StreamI &stream, ProtocolI &protocol) {
    ReferenceT<DataExportI> target = FindTarget(protocol);
    bool ok = target.IsValid();
    if (ok) {
        ok = target->GetAsText(stream, protocol);
    }
    return ok;
}

CLASS_REGISTER(ObjectBrowser, "1.0")
}

