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
    granularity = 16u;
    size = 0u;
    maxSize = 0u;
    container = NULL_PTR(Reference *);
    muxTimeout = TTInfiniteWait;
}

ConfigurationDatabaseNode::~ConfigurationDatabaseNode() {
    binTree.Reset();
    if (container != NULL_PTR(Reference *)) {
        parent = Reference();
        delete [] container;
    }
}

void ConfigurationDatabaseNode::Purge() {
    if (Lock()) {
        if (container != NULL_PTR(Reference *)) {
            parent = Reference();
            delete [] container;
            container = NULL_PTR(Reference *);
            size = 0u;
            maxSize = 0u;
            binTree.Reset();
        }
    }
    UnLock();
}

void ConfigurationDatabaseNode::Purge(ReferenceContainer &purgeList) {
    if (Lock()) {
        if (container != NULL_PTR(Reference *)) {
            parent = Reference();
            uint32 n;
            for (n = 0u; n<size; n++) {
                purgeList.Insert(container[n]);
            }
            delete [] container;
            container = NULL_PTR(Reference *);
            size = 0u;
            maxSize = 0u;
            binTree.Reset();
        }
    }
    UnLock();
}

bool ConfigurationDatabaseNode::Insert(Reference ref) {
    bool ok = Lock();
    if (ok) {
        uint32 index = size;
        if (index >= maxSize) {
            if (size < granularity) {
                maxSize++;
            }
            else {
                maxSize += granularity;
            }
            Reference *oldContainer = container;
            container = new Reference[maxSize];
            uint32 n;
            //oldContainer == NULL => size == 0
            for (n=0u; n<size; n++) {
                container[n] = oldContainer[n];
            }
            if (oldContainer != NULL_PTR(Reference *)) {
                delete [] oldContainer;
            }
        }
        container[index] = ref;
        ok = (binTree.Insert(ref->GetName(), index) != 0xFFFFFFFFu);
    }
    if (ok) {
        size++;
    }
    UnLock();
    return ok;
}

uint32 ConfigurationDatabaseNode::Size() {
    uint32 ssize = 0u;
    if (Lock()) {
        ssize = size;
    }
    UnLock();
    return ssize;
}

Reference ConfigurationDatabaseNode::Get(uint32 idx) {
    Reference ref;
    if (Lock()) {
        if (idx < size) {
            ref = container[idx];
        }
    }
    UnLock();
    return ref;
}

Reference ConfigurationDatabaseNode::Find(const char8 * const path) {
    Reference ret;
    //find the first name
    StreamString token;
    StreamString toTokenize = path;
    (void) toTokenize.Seek(0LLU);
    char8 term;

    if (toTokenize.GetToken(token, ".", term)) {
        uint32 bindex;
        //binary search
        if (Lock()) {
            if (binTree.Search(token.Buffer(), bindex)) {
                uint32 index = binTree[bindex];
                if (index < size) {
                    if (term == '.') {
                        ReferenceT<ConfigurationDatabaseNode> containerI;
                        containerI = container[index];
                        UnLock();
                        if (containerI.IsValid()) {
                            StreamString next = &toTokenize.Buffer()[token.Size() + 1u];
                            if (next.Size() > 0u) {
                                //continue only for the path
                                ret = containerI->Find(next.Buffer());
                            }
                            else {
                                ret = containerI;
                            }
                        }
                    }
                    else {
                        ret = container[index];
                        UnLock();
                    }
                }
                else {
                    UnLock();
                }
            }
            else {
                UnLock();
            }
        }
    }

    return ret;
}

Reference ConfigurationDatabaseNode::FindLeaf(const char8 * const name) {
    Reference ret;
    uint32 bindex;
    //binary search
    if (Lock()) {
        if (binTree.Search(name, bindex)) {
            uint32 index = binTree[bindex];
            if (index < size) {
                ret = container[index];
            }
        }
    }
    UnLock();
    return ret;
}

bool ConfigurationDatabaseNode::Delete(Reference ref) {
    bool ok = Lock();
    if (ok) {
        ok = ref.IsValid();
        uint32 index;
        //binary search
        if (ok) {
            uint32 bindex;
            if (binTree.Search(ref->GetName(), bindex)) {
                index = binTree[bindex];
                ok = (index < size);
            }
        }
        if (ok) {
            size--;
            if (size < granularity) {
                maxSize--;
            }
            else {
                if ((size % granularity) == 0) {
                    maxSize -= granularity;
                }
            }
            if (size > 0u) {
                Reference *oldContainer = container;
                container = new Reference[maxSize];
                uint32 n;
                //oldContainer == NULL => size == 0
                for (n=0u; n<index; n++) {
                    container[n] = oldContainer[n];
                }
                for (n=index; n<size; n++) {
                    container[n] = oldContainer[n + 1];
                }
                if (oldContainer != NULL_PTR(Reference *)) {
                    delete [] oldContainer;
                }
            }
            else {
                delete [] container;
                container = NULL_PTR(Reference *);
            }
        }
        //Need to remap...
        if (ok) {
            binTree.Reset();
            uint32 i;
            for (i=0u; (i<size) && (ok); i++) {
                Reference eRef = container[i];
                ok = (binTree.Insert(eRef->GetName(), static_cast<uint32>(i)) != 0xFFFFFFFFu);
            }
        }
    }
    UnLock();
    return ok;
}

ReferenceT<ConfigurationDatabaseNode> ConfigurationDatabaseNode::GetParent() {
    return parent;
}

void ConfigurationDatabaseNode::SetParent(ReferenceT<ConfigurationDatabaseNode> parentIn) {
    parent = parentIn;
}

bool ConfigurationDatabaseNode::Lock() {
    return (mux.FastLock(muxTimeout) == ErrorManagement::NoError);
}

void ConfigurationDatabaseNode::UnLock() {
    mux.FastUnLock();
}

CLASS_REGISTER(ConfigurationDatabaseNode, "1.0")

}
