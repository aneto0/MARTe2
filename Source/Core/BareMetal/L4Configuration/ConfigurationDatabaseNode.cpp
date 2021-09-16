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
#include "AdvancedErrorManagement.h"
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
    granularity = 2u;
    containerSize = 0u;
    numberOfNodes = 0u;
    maxSize = 0u;
    container = NULL_PTR(Reference *);
    muxTimeout = TTInfiniteWait;
}

/*lint -e{1579} .Justification: The destructor calls an external function. */
/*lint -e{1551} by design memory if freed in the destructor.*/
ConfigurationDatabaseNode::~ConfigurationDatabaseNode() {
    Purge();
}

void ConfigurationDatabaseNode::Purge() {
    bool ok = true;
    //Remove self recursions due to links to the ancestors
    parent = Reference();

    if (container != NULL_PTR(Reference *)) {
        for (uint32 n = 0u; (n<containerSize) && (ok); n++) {
            ReferenceT<ConfigurationDatabaseNode> rc = container[n];
            if (rc.IsValid()) {
                rc->Purge();
            }
        }
        delete [] container;
        container = NULL_PTR(Reference *);
    }
    containerSize = 0u;
    numberOfNodes = 0u;
    maxSize = 0u;
    binTree.Reset();
}

bool ConfigurationDatabaseNode::Insert(Reference ref) {
    bool ok = Lock();
    if (ok) {
        uint32 index = containerSize;
        if (index >= maxSize) {
            if (maxSize == 0u) {
                maxSize = 1u;
            }
            else {
                maxSize *= granularity;
            }
            Reference *oldContainer = container;
            container = new Reference[maxSize];
            uint32 n;
            //oldContainer == NULL => containerSize == 0
            for (n=0u; n<containerSize; n++) {
                container[n] = oldContainer[n];
                oldContainer[n] = Reference();
            }
            if (oldContainer != NULL_PTR(Reference *)) {
                delete [] oldContainer;
            }
        }
        container[index] = ref;
        ok = (binTree.Insert(ref->GetName(), index) != 0xFFFFFFFFu);
    }
    if (ok) {
        containerSize++;
        ReferenceT<ConfigurationDatabaseNode> node = ref;
        if (node.IsValid()) {
            numberOfNodes++;
        }
    }
    UnLock();
    return ok;
}

uint32 ConfigurationDatabaseNode::Size() {
    uint32 ssize = 0u;
    if (Lock()) {
        ssize = containerSize;
    }
    UnLock();
    return ssize;
}

Reference ConfigurationDatabaseNode::Get(const uint32 idx) {
    Reference ref;
    if (Lock()) {
        if (idx < containerSize) {
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
                if (index < containerSize) {
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
                    }
                }
            }
        }
        UnLock();
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
            if (index < containerSize) {
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
        uint32 index = 0u;
        //binary search
        if (ok) {
            uint32 bindex;
            if (binTree.Search(ref->GetName(), bindex)) {
                index = binTree[bindex];
                ok = (index < containerSize);
            }
        }
        if (ok) {
            containerSize--;
            if (containerSize < (maxSize / granularity)) {
                maxSize /= granularity;
                if (maxSize < containerSize) {
                    maxSize = containerSize;
                }
            }
            if (containerSize > 0u) {
                Reference *oldContainer = container;
                container = new Reference[maxSize];
                uint32 n;
                //oldContainer == NULL => containerSize == 0
                for (n=0u; n<index; n++) {
                    container[n] = oldContainer[n];
                }
                for (n=index; n<containerSize; n++) {
                    /*lint -e{679} no truncation*/
                    container[n] = oldContainer[n + 1u];
                }
                if (oldContainer != NULL_PTR(Reference *)) {
                    delete [] oldContainer;
                }
            }
            else {
                delete [] container;
                container = NULL_PTR(Reference *);
                maxSize = 0u;
            }
        }
        //Need to remap...
        if (ok) {
            binTree.Reset();
            uint32 i;
            for (i=0u; (i<containerSize) && (ok); i++) {
                /*lint -e{613} containerSize > 0 => container != NULL*/
                Reference eRef = container[i];
                ok = (binTree.Insert(eRef->GetName(), static_cast<uint32>(i)) != 0xFFFFFFFFu);
            }
        }
        if (ok) {
            //Break the reference to the parent
            ReferenceT<ConfigurationDatabaseNode> refCdbn = ref;
            if (refCdbn.IsValid()) {
                refCdbn->SetParent(Reference());
                refCdbn->Purge();
                numberOfNodes--;
            }
        }
    }
    UnLock();
    return ok;
}

ReferenceT<ConfigurationDatabaseNode> ConfigurationDatabaseNode::GetParent() const {
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

uint32 ConfigurationDatabaseNode::GetNumberOfNodes() {
    uint32 ssize = 0u;
    if (Lock()) {
        ssize = numberOfNodes;
    }
    UnLock();
    return ssize;
}

CLASS_REGISTER(ConfigurationDatabaseNode, "1.0")

}
