/**
 * @file ConfigurationDatabaseNode.cpp
 * @brief Source file for class ConfigurationDatabaseNode
 * @date 20/12/2019
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
 * the class ConfigurationDatabaseNode (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabaseNode.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ConfigurationDatabaseNode::ConfigurationDatabaseNode() :
        Object() {

}

ConfigurationDatabaseNode::~ConfigurationDatabaseNode() {
    binTree.Reset();
}

void ConfigurationDatabaseNode::Purge() {
    refContainer.Purge();
}

void ConfigurationDatabaseNode::Purge(ReferenceContainer &purgeList) {
    refContainer.Purge(purgeList);
}

bool ConfigurationDatabaseNode::Insert(Reference ref, const int32 &position) {
    uint32 index = refContainer.Size();
    bool ok = refContainer.Insert(ref, position);
    if (ok) {
        if (position == -1) {
            //add the index to the list to find it faster!
            ok = (binTree.Insert(ref->GetName(), index) != 0xFFFFFFFFu);
        }
        else {
            ok = (binTree.Insert(ref->GetName(), static_cast<uint32>(position)) != 0xFFFFFFFFu);
        }
    }
    return ok;
}

uint32 ConfigurationDatabaseNode::Size() {
    return refContainer.Size();
}

Reference ConfigurationDatabaseNode::Get(uint32 idx) {
    return refContainer.Get(idx);
}

Reference ConfigurationDatabaseNode::Find(const char8 * const path) {
    Reference ret;
    //find the first name
    StreamString token;
    StreamString toTokenize = path;
    (void) toTokenize.Seek(0LLU);
    char8 term;

    if (toTokenize.GetToken(token, ".", term)) {
        uint32 index;
        //binary search
        if (binTree.Search(token.Buffer(), index)) {
            if (term == '.') {
                ReferenceT<ConfigurationDatabaseNode> container = refContainer.Get(binTree[index]);
                if (container.IsValid()) {
                    StreamString next = &toTokenize.Buffer()[token.Size() + 1u];
                    if (next.Size() > 0u) {
                        //continue only for the path
                        ret = container->Find(next.Buffer());
                    }
                    else {
                        ret = container;
                    }
                }
            }
            else {
                ret = refContainer.Get(binTree[index]);
            }
        }
    }
    return ret;
}

Reference ConfigurationDatabaseNode::FindLeaf(const char8 * const name) {
    Reference ret;
    uint32 index;
    //binary search
    if (binTree.Search(name, index)) {
        ret = refContainer.Get(binTree[index]);
    }
    return ret;
}

bool ConfigurationDatabaseNode::Delete(Reference ref) {
    bool ok = ref.IsValid();
    if (ok) {
        ok = binTree.Remove(ref->GetName());
    }
    if (ok) {
        ok = refContainer.Delete(ref);
    }
    //Need to remap...
    uint32 size = refContainer.Size();
    binTree.Reset();
    uint32 i;
    for (i=0u; (i<size) && (ok); i++) {
        Reference eRef = refContainer.Get(i);
        ok = (binTree.Insert(eRef->GetName(), static_cast<uint32>(i)) != 0xFFFFFFFFu);
    }
    return ok;
}

ReferenceT<ConfigurationDatabaseNode> ConfigurationDatabaseNode::GetParent() {
    return parent;
}

void ConfigurationDatabaseNode::SetParent(ReferenceT<ConfigurationDatabaseNode> parentIn) {
    parent = parentIn;
}

CLASS_REGISTER(ConfigurationDatabaseNode, "1.0")

}
