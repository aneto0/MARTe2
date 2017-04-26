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
 *
 * @details This source file contains the definition of all the methods for
 * the class ReferenceContainer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <typeinfo>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainer.h"
#include "ReferenceContainerNode.h"
#include "ReferenceContainerFilterReferences.h"
#include "ReferenceT.h"
#include "ErrorManagement.h"
#include "StringHelper.h"
#include "ReferenceContainerFilterObjectName.h"
#include "GlobalObjectsDatabase.h"
#include "DynamicCString.h"
#include "StaticCString.h"
#include "AnyType.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ReferenceContainer::ReferenceContainer() :
        Object() {
    mux.Create();
    muxTimeout = TTInfiniteWait;
}

ReferenceContainer::ReferenceContainer(ReferenceContainer &copy) :
        Object(copy) {
    SetTimeout(copy.GetTimeout());
    uint32 nChildren = copy.Size();
    for (uint32 i = 0u; i < nChildren; i++) {
        Reference toInsert = copy.Get(i);
        if (!Insert(toInsert)) {
            //TODO
        }
    }
}

ReferenceContainer& ReferenceContainer::operator =(ReferenceContainer &copy) {
    SetTimeout(copy.GetTimeout());
    if (this != &copy) {
        uint32 nChildren = copy.Size();
        for (uint32 i = 0u; i < nChildren; i++) {
            Reference toInsert = copy.Get(i);
            if (!Insert(toInsert)) {
                //TODO
            }
        }
    }
    return *this;
}

/*lint -e{929} -e{925} the current implementation of the ReferenceContainer requires pointer to pointer casting*/
Reference ReferenceContainer::Get(const uint32 idx) {
    Reference ref;
    if (Lock()) {
        if (idx < list.ListSize()) {
            ReferenceContainerNode *node = (list.ListPeek(idx));
            if (node != NULL) {
                ref = node->GetReference();
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::Warning, "ReferenceContainer: input greater than the list size.");
        }
    }
    UnLock();
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

}

void ReferenceContainer::CleanUp() {

    uint32 numberOfElements;
    if (!Lock()) {
        REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed FastLock()");
    }
    numberOfElements = list.ListSize() + purgeList.ListSize();
    UnLock();

    //flat recursion due to avoid stack waste!!
    for (uint32 i = 0u; i < numberOfElements; i++) {
        if (!Lock()) {
            REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed FastLock()");
        }
        if (purgeList.ListSize() >= numberOfElements) {
            UnLock();
            break;
        }
        //extract the element from the list
        ReferenceContainerNode *node = list.ListExtract(0u);
        if (node != NULL) {
            purgeList.ListInsert(node);
        }
        UnLock();
    }

    for (uint32 i = 0u; i < numberOfElements; i++) {
        if (!Lock()) {
            REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed FastLock()");
        }
        if (purgeList.ListSize() == 0u) {
            UnLock();
            break;
        }

        ReferenceContainerNode * node = purgeList.ListExtract(0u);
        UnLock();

        ReferenceT<ReferenceContainer> element;
        if (node != NULL) {
            element = node->GetReference();
        }

        if (element.IsValid()) {
            element->CleanUp();
        }
        //extract and delete the element from the list
        if (node != NULL) {
            delete node;
        }
    }
}

/*lint -e{593} .Justification: The node (newItem) will be deleted by the destructor. */
bool ReferenceContainer::Insert(Reference ref,  const int32 &position) {
    bool ok = (Lock());
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
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed FastLock()");
    }
    UnLock();
    return ok;
}

bool ReferenceContainer::Insert(CCString const path,  Reference ref) {
    bool ok = ref.IsValid();
    if (ok) {
        if (path.GetSize() == 0u) {
            ok = Insert(ref);
        }
        else {
            bool created = false;
            ReferenceContainer* currentNode = this;

            DynamicCString token;
            DynamicCString nextToken;

            CCString toTokenize(path);
            toTokenize = StringHelper::Tokenize(toTokenize, token,CCString("."), CCString("."));

            while ((token[0] != '\0') && (ok)) {
                ok = (token.GetSize() > 0u);
                if (ok) {
                    //Check if a node with this name already exists
                    bool found = false;
                    Reference foundReference;
                    uint32 i;
                    for (i = 0u; (i < currentNode->Size()) && (!found); i++) {
                        foundReference = currentNode->Get(i);
                        found = (StringHelper::Compare(foundReference->GetName(), token.GetList()) == 0);
                    }
                    // take the next token

                    toTokenize = StringHelper::Tokenize(toTokenize, nextToken,CCString("."), CCString("."));

                    if (found) {
                        currentNode = dynamic_cast<ReferenceContainer*>(foundReference.operator->());
                        // if it is a leaf exit (and return false)
                        if (currentNode == NULL) {
                            ok = false;
                        }
                    }
                    else {
                        // insert the reference
                        if (nextToken[0] == '\0') {
                            ref->SetName(token.GetList());
                            created = currentNode->Insert(ref);
                        }
                        // create a node
                        else {
                            ReferenceT<ReferenceContainer> container(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                            container->SetName(token.GetList());
                            ok = currentNode->Insert(container);
                            if (ok) {
                                currentNode = container.operator->();
                            }
                        }
                    }
                    if (ok) {
                        token.Truncate(0);
                        ok = token.AppendN(CCString (nextToken.GetList()));
                    }
                }
            }

            if (ok) {
                ok = created;
            }
        }
    }
    return ok;

}

bool ReferenceContainer::Delete(Reference ref) {
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::REMOVE, ref);
    ReferenceContainer result;
    //Locking is already done inside the Find
    Find(result, filter);
    return (result.Size() > 0u);
}

bool ReferenceContainer::Delete(CCString const path) {
    ReferenceContainerFilterObjectName filter(1, ReferenceContainerFilterMode::REMOVE, path);
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
    bool ok = Lock();
    if (ok) {
        if (list.ListSize() > 0u) {
            if (filter.IsReverse()) {
                index = static_cast<int32>(list.ListSize()) - 1;
            }
            //The filter will be finished when the correct occurrence has been found (otherwise it will walk all the list)
            //lint -e{9007} no side-effects on the right of the && operator
            while ((!filter.IsFinished()) && ((filter.IsReverse() && (index > -1)) || ((!filter.IsReverse()) && (index < static_cast<int32>(list.ListSize()))))) {

                ReferenceContainerNode *currentNode = (list.ListPeek(static_cast<uint32>(index)));

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
                                else {
                                    REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed StaticList::Delete()");
                                }
                            }
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed StaticList::Insert()");
                        }
                    }
                }

                // no other stack waste!!
                if (filter.IsFinished()) {
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
                        UnLock();
                        currentNodeContainer->Find(result, filter);
                        if (Lock()) {
                            //Something was found if the result size has changed
                            if (sizeBeforeBranching == result.list.ListSize()) {
                                //Nothing found. Remove the stored path (which led to nowhere).
                                if (filter.IsStorePath()) {
                                    LinkedListable *node = result.list.ListExtract(result.list.ListSize() - 1u);
                                    delete node;
                                }
                            }
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed FastLock()");
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed StaticList::Insert()");
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
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed FastLock()");
    }

    UnLock();
}

Reference ReferenceContainer::Find(CCString const path,const bool recursive) {
    Reference ret;
    uint32 mode = ReferenceContainerFilterMode::SHALLOW;
    if (recursive) {
        mode = ReferenceContainerFilterMode::RECURSIVE;
    }
    ReferenceContainerFilterObjectName filter(1, mode, path);
    ReferenceContainer resultSingle;
    Find(resultSingle, filter);
    if (resultSingle.Size() > 0u) {
        ret = resultSingle.Get(resultSingle.Size() - 1u);
    }
    return ret;
}

uint32 ReferenceContainer::Size() {
    uint32 size = 0u;
    if (Lock()) {
        size = list.ListSize();
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed FastLock()");
    }
    UnLock();
    return size;
}

bool ReferenceContainer::Initialise(StructuredDataI &data) {

    // only one thread has to initialise.

    // Recursive initialization
    bool ok = true;
    uint32 numberOfChildren = data.GetNumberOfChildren();
    for (uint32 i = 0u; (i < numberOfChildren) && (ok); i++) {
        CCString childName = data.GetChildName(i);
        ok = (!childName.IsNullPtr());
        if (ok) {
            // case object
            if ((childName[0] == '+') || (childName[0] == '$')) {
                if (data.MoveRelative(childName)) {
                    Reference newObject;
                    ok = newObject.Initialise(data, false);
                    if (ok) {
                        ok = (newObject.IsValid());
                        if (ok) {
                            if (childName[0] == '$') {
                                newObject->SetDomain(true);
                            }
                            ok = ReferenceContainer::Insert(newObject);
                        }
                        if (ok) {
                            ok = data.MoveToAncestor(1u);
                        }
                    }
                    else {

// TODO use StaticCString
                        const uint32 maxSize = 64u;
                        char8 errorMsgBuffer[maxSize];
                        StaticCString<64> errorMsg(errorMsgBuffer);
//                        errorMsg[0] = '\0';
                        CCString msg = "Failed to Initialise object with name ";
                        bool ret = errorMsg.Append(msg);
//                        bool ret = StringHelper::Concatenate(&errorMsg[0], "Failed to Initialise object with name ");
//                        uint32 sizeLeft = 0u;
                        if (ret) {
                            ret = errorMsg.Append(childName);
//                            sizeLeft = maxSize - StringHelper::Length(&errorMsg[0]);
//                            ret = StringHelper::ConcatenateN(&errorMsg[0], childName, sizeLeft);
                        }
                        if (ret) {
                            REPORT_ERROR(ErrorManagement::FatalError, errorMsg.GetList());
                        }
                    }
                }
                else {
                    ok = false;
                }
            }
        }
    }
    return ok;
}
#if 0
bool ReferenceContainer::ExportData(StructuredDataI & data) {

    // no need to lock
    CCString objName = GetName();

    bool ret = true;
    {
        DynamicCString objNameToCreate;

        if (IsDomain()){
            ret = objNameToCreate.Append('$');
        } else {
            ret = objNameToCreate.Append('+');
        }

        if (ret){
            ret = objNameToCreate.AppendN(objName);
        }

        if (ret) {
            // moves to the node
            ret = data.CreateRelative(objNameToCreate.GetList());
        }
    }


    if (ret) {
        ClassRegistryItem *cri;

        cri = this->GetClassRegistryItem();
        ret = (cri != NULL);

        if (ret) {
            CCString cn = cri->GetClassName();
            ret = data.Write("Class", cn);

            uint32 numberOfChildren = Size();
            for (uint32 i = 0u; (i < numberOfChildren) && (ret); i++) {
                Reference child = Get(i);
                ret = child.IsValid();
                if (ret) {
                    ret = child->ExportData(data);
                }
            }
        }
    }

    if (ret){
        ret = data.MoveToAncestor(1u);
    }


    return ret;
}
#endif

bool ReferenceContainer::Lock() {
    return (mux.FastLock(muxTimeout) == ErrorManagement::NoError);
}

void ReferenceContainer::UnLock() {
    mux.FastUnLock();
}

CLASS_REGISTER(ReferenceContainer, "1.0")

}

