/**
 * @file ReferenceContainer.cpp
 * @brief Source file for class ReferenceContainer
 * @date 12/08/2015
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
#include "ReferenceContainerNode.h"
#include "ReferenceContainerFilterReferences.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{


ReferenceContainer::ReferenceContainer() :
        Object() {
    mux.Create();
    muxTimeout = TTInfiniteWait;
}

/*lint -e{929} -e{925} the current implementation of the ReferenceContainer requires pointer to pointer casting*/
Reference ReferenceContainer::Get(const uint32 idx) {
    Reference ref;
    if (mux.FastLock(muxTimeout) == ErrorManagement::NoError) {
        if (idx < list.ListSize()) {
            ReferenceContainerNode *node = dynamic_cast<ReferenceContainerNode *>(list.ListPeek(idx));
            if (node != NULL) {
                ref = node->GetReference();
            }
        }
    }
    mux.FastUnLock();
    return ref;
}

TimeoutType ReferenceContainer::GetTimeout() const {
    return muxTimeout;
}

void ReferenceContainer::SetTimeout(const TimeoutType &timeout) {
    muxTimeout = timeout;
}

/*lint -e{1551} no exception should be thrown given that ReferenceContainer is
 * the sole owner of the list (LinkedListHolder)*/
ReferenceContainer::~ReferenceContainer() {
    LinkedListable *p = list.List();
    list.Reset();
    while (p != NULL) {
        LinkedListable *q = p;
        p = p->Next();
        delete q;
    }
}

bool ReferenceContainer::Insert(Reference ref,
                                const int32 &position) {
    bool ok = (mux.FastLock(muxTimeout) == ErrorManagement::NoError);
    if (ok) {
        ReferenceContainerNode *newItem = new ReferenceContainerNode();
        if (newItem->SetReference(ref)) {
            if (position == -1) {
                list.ListAdd(newItem);
            }
            else {
                list.ListInsert(newItem, static_cast<uint32>(position));
            }
        }
        else {
            delete newItem;
            ok = false;
        }
    }
    mux.FastUnLock();
    return ok;
}

bool ReferenceContainer::Delete(Reference ref) {
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::REMOVE, ref);
    ReferenceContainer result;
    //Locking is already done inside the Find
    Find(result, filter);
    return (result.Size() > 0u);
}

bool ReferenceContainer::IsContainer(const Reference &ref) const {
    ReferenceT<ReferenceContainer> test = ref;
    return test.IsValid();
}

/*lint -e{929} -e{925} the current implementation of the ReferenceContainer requires pointer to pointer casting*/
void ReferenceContainer::Find(ReferenceContainer &result,
                              ReferenceContainerFilter &filter) {
    int32 index = 0;
    bool ok = (mux.FastLock(muxTimeout) == ErrorManagement::NoError);
    if (ok && (list.ListSize() > 0u)) {
        if (filter.IsReverse()) {
            index = static_cast<int32>(list.ListSize()) - 1;
        }
        //The filter will be finished when the correct occurrence has been found (otherwise it will walk all the list)
        //lint -e{9007} no side-effects on the right of the && operator
        while ((!filter.IsFinished()) && ((filter.IsReverse() && (index > -1)) || ((!filter.IsReverse()) && (index < static_cast<int32>(list.ListSize()))))) {

            ReferenceContainerNode *currentNode = dynamic_cast<ReferenceContainerNode *>(list.ListPeek(static_cast<uint32>(index)));
            Reference currentNodeReference = currentNode->GetReference();

            //Check if the current node meets the filter criteria
            bool found = filter.Test(result, currentNodeReference);
            if (found) {
                //IsSearchAll() => all found nodes should be inserted in the output list
                //IsFinished() => that the desired occurrence of this object was found => add it to the output list
                /*lint -e{9007} filter.IsSearchAll() has no side effects*/
                if (filter.IsSearchAll() || filter.IsFinished()) {
                    if (result.Insert(currentNodeReference)) {
                        if (filter.IsRemove()) {
                            //Only delete the exact node index
                            if (list.ListDelete(currentNode)) {
                                //Given that the index will be incremented, but we have removed an element, the index should stay in the same position
                                if (!filter.IsReverse()) {
                                    index--;
                                }
                            }
                        }
                    }
                }
            }

            // no other stack waste!!
            if(filter.IsFinished()){
                break;
            }

            /*lint -e{9007} filter.IsRecursive() has no side effects*/
            if ((IsContainer(currentNodeReference)) && filter.IsRecursive()) {
                ok = true;
                if (filter.IsStorePath()) {
                    ok = result.Insert(currentNodeReference);
                }

                if (ok) {
                    ReferenceT<ReferenceContainer> currentNodeContainer = currentNodeReference;
                    uint32 sizeBeforeBranching = result.list.ListSize();
                    mux.FastUnLock();
                    currentNodeContainer->Find(result, filter);
                    if (mux.FastLock(muxTimeout) == ErrorManagement::NoError) {
                        //Something was found if the result size has changed
                        if (sizeBeforeBranching == result.list.ListSize()) {
                            //Nothing found. Remove the stored path (which led to nowhere).
                            if (filter.IsStorePath()) {
                                LinkedListable *node = result.list.ListExtract(result.list.ListSize() - 1u);
                                delete node;
                            }
                        }
                    }
                }
            }
            if (!filter.IsReverse()) {
                index++;
            }
            else {
                index--;
            }
        }
    }
    mux.FastUnLock();
}

uint32 ReferenceContainer::Size() {
    uint32 size = 0u;
    if (mux.FastLock(muxTimeout) == ErrorManagement::NoError) {
        size = list.ListSize();
    }
    mux.FastUnLock();
    return size;
}


CLASS_REGISTER(ReferenceContainer, "1.0")

}

