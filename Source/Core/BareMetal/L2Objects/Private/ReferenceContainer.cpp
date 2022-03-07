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
#include "CompositeErrorManagement.h"
#include "ReferenceContainerFilterObjectName.h"
#include "DynamicCString.h"
#include "AnyType.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
char8 ReferenceContainer::buildTokensList[REFERENCE_CONTAINER_NUMBER_OF_TOKENS] = { '+', '\0', '\0', '\0', '\0' };
char8 ReferenceContainer::domainTokensList[REFERENCE_CONTAINER_NUMBER_OF_TOKENS] = { '$', '\0', '\0', '\0', '\0' };
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ReferenceContainer::ReferenceContainer() :
        Object() {
    mux.Create();
    muxTimeout = MilliSeconds::Infinite;
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
	ErrorManagement::ErrorType ret;
    ret.parametersError = !ref.IsValid();
    if (ret) {
        if (path.GetSize() == 0u) {
            ret.fatalError = Insert(ref);
        }
        else {
            ReferenceContainer* currentNode = this;

            DynamicCString token;
            DynamicCString nextToken;

            CCString toTokenize(path);
            uint32 index;
            toTokenize = DynamicCString::Tokenize(toTokenize, token,index,CCString("."), CCString("."));

//            bool created = false;
            bool done = false;
            while ((token[0] != '\0') && (ret) && (!done)) {
            	ret.notCompleted = !(token.GetSize() > 0u);
                if (ret) {
                    //Check if a node with this name already exists
                    bool found = false;
                    Reference foundReference;
                    uint32 i;
                    for (i = 0u; (i < currentNode->Size()) && (!found); i++) {
                        foundReference = currentNode->Get(i);
                        found = (foundReference->GetName().IsSameAs(token.GetList()));
                    }
                    // take the next token : nextToken
                    uint32 index;
                    toTokenize = DynamicCString::Tokenize(toTokenize, nextToken,index,CCString("."), CCString("."));

                    if (found) {
                    	// if found, simply move to it
                        currentNode = dynamic_cast<ReferenceContainer*>(foundReference.operator->());

                        // if it is a leaf exit (and return false)
                        ret.invalidOperation = (currentNode == NULL) ;
                    }
                    else {
                        // insert the reference
                        if (nextToken.GetSize()== 0u) {
                            ref->SetName(token.GetList());
//                            created = currentNode->Insert(ref);
                            done = true; // !! LOOP good exit condition
                        }
                        // create a node
                        else {
                            ReferenceT<ReferenceContainer> container(HeapManager::standardHeapId);
                            container->SetName(token.GetList());
                            ret.fatalError = !currentNode->Insert(container);
                            if (ret) {
                                currentNode = container.operator->();
                            }
                        }
                    } //while

                    if (ret) {
                    	token = nextToken;
                    }
                }
            }
        }
    }
    return ret;

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
    return ref.IsValid() ? ref->IsReferenceContainer() : false;
}

/*lint -e{929} -e{925} the current implementation of the ReferenceContainer requires pointer to pointer casting*/
void ReferenceContainer::Find(ReferenceContainer &result,ReferenceContainerFilter &filter) {
    int32 index = 0;
    bool ok = Lock();
    if (ok) {
        if (list.ListSize() > 0u) {
            if (filter.IsReverse()) {
                index = static_cast<int32>(list.ListSize()) - 1;
            }

            ReferenceContainerNode *currentNode = (list.ListPeek(static_cast<uint32>(index)));

            //The filter will be finished when the correct occurrence has been found (otherwise it will walk all the list)
            //lint -e{9007} no side-effects on the right of the && operator
            while ((!filter.IsFinished()) && ((filter.IsReverse() && (index > -1)) || ((!filter.IsReverse()) && (index < static_cast<int32>(list.ListSize()))))) {

                Reference const & currentNodeReference = currentNode->GetReference();
                //Check if the current node meets the filter criteria
                bool found = filter.Test(result, currentNodeReference);
                if (found) {
                    //IsSearchAll() => all found nodes should be inserted in the output list
                    //IsFinished() => that the desired occurrence of this object was found => add it to the output list
                    /*lint -e{9007} filter.IsSearchAll() has no side effects*/
                    if (filter.IsSearchAll() || filter.IsFinished()) {
                        if (result.Insert(currentNodeReference)) {
/*
                        	{  // TODO remove
                        		Reference ref = result.Get(0);
                        		DynamicCString dcs;
                        		AnyType at;
                        		ref.ToAnyType(at);
                        		at.CopyTo(dcs);
                        		printf("%s=",ref->GetName().GetList());
                        		if (dcs.GetSize() > 0)  printf("%s",dcs.GetList());
                        		printf("\n");fflush(stdout);
                        	}
*/
                        	if (filter.IsRemove()) {
                                //Only delete the exact node index
                                if (list.ListDelete(currentNode)) {
                                    //Given that the index will be incremented, but we have removed an element, the index should stay in the same position
                                    if (!filter.IsReverse()) {
                                        index--;
                                        currentNode = (list.ListPeek(static_cast<uint32>(index)));
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
                    if (currentNode != NULL_PTR(ReferenceContainerNode *)) {
                        currentNode = dynamic_cast<ReferenceContainerNode *>(currentNode->Next());
                    }
                }
                else {
                    index--;
                    currentNode = (list.ListPeek(static_cast<uint32>(index)));
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
    ErrorManagement::ErrorType ret;

    uint32 numberOfChildren = data.GetNumberOfChildren();
    for (uint32 i = 0u; (i < numberOfChildren) && (ret); i++) {
        DynamicCString childName;
        ret = data.GetChildName(i,childName);
//        CCString childName = data.GetChildName(i);

        if (ret){
            ret.internalSetupError = (childName.GetSize()==0);
        }

        if (ret) {
            // case object
            //lint -e{9007} there are no side-effects on IsBuildToken or IsDomainToken, but these cannot be declared const.
            if ((IsBuildToken(childName[0])) || (IsDomainToken(childName[0]))) {
            	ret.internalStateError = !data.MoveRelative(childName);
                Reference newObject;
                if (ret) {
                    ret.fatalError = !newObject.Initialise(data, false);
                }
                if (ret) {
                    ret.fatalError = (!newObject.IsValid());
                }
                if (ret) {
                    if (IsDomainToken(childName[0])) {
                        newObject->SetDomain(true);
                    }
                    ret.fatalError = !ReferenceContainer::Insert(newObject);
                }
                if (ret) {
                	ret.fatalError = !data.MoveToAncestor(1u);
                }
            } //if

        } //if
    	COMPOSITE_REPORT_ERROR(ret, "Failed to Initialise object with name ",childName);

    } // for
    return ret;
}

bool ReferenceContainer::Lock() {
    return (mux.FastLock(muxTimeout) == ErrorManagement::ErrorType(ErrorManagement::NoError));
}

void ReferenceContainer::UnLock() {
    mux.FastUnLock();
}

void ReferenceContainer::Purge() {
    ReferenceContainer purgeList;
    Purge(purgeList);
}

void ReferenceContainer::Purge(ReferenceContainer &purgeList) {
    uint32 purgeStart = purgeList.Size();
    uint32 purgeEnd = purgeStart;
    uint32 numberOfElements = Size();

    bool ok = true;
    //flat recursion to avoid stack waste
    for (uint32 i = 0u; (i < numberOfElements) && (ok); i++) {
        //extract the element from the list
        Reference node = Get(0u);
        if (node.IsValid()) {
            ok = purgeList.Insert(node);
            if(ok) {
                ok = Delete(node);
            }
            purgeEnd++;
        }
    }

    //Recurse on all the sub nodes
    for (uint32 i = purgeStart; i < purgeEnd; i++) {
        Reference nodeObj = purgeList.Get(i);
        if (nodeObj.IsValid()) {
            nodeObj->Purge(purgeList);
        }
    }
}

bool ReferenceContainer::IsReferenceContainer() const {
    return true;
}

bool ReferenceContainer::AddToken(char8 * const tokenList, const char8 token) {
    uint32 i = 0u;
    bool exists = false;
    while ((!exists) && (i < REFERENCE_CONTAINER_NUMBER_OF_TOKENS) && (tokenList[i] != '\0')) {
        exists = (tokenList[i] == token);
        i++;
    }
    bool ok = exists;
    if (!ok) {
        ok = (i < REFERENCE_CONTAINER_NUMBER_OF_TOKENS);
        if (ok) {
            tokenList[i] = token;
        }
    }
    return ok;
}

void ReferenceContainer::RemoveToken(char8 * const tokenList, const char8 token) {
    uint32 i = 0u;
    while ((i < REFERENCE_CONTAINER_NUMBER_OF_TOKENS) && (tokenList[i] != token)) {
        i++;
    }
    while (i < (REFERENCE_CONTAINER_NUMBER_OF_TOKENS - 1u)) {
        tokenList[i] = tokenList[static_cast<uint8>(i + 1u)];
        i++;
    }
    tokenList[REFERENCE_CONTAINER_NUMBER_OF_TOKENS - 1u] = '\0';
}

bool ReferenceContainer::IsToken(const char8 * const tokenList, const char8 token) {
    uint32 i = 0u;
    bool ok = false;
    while ((i < REFERENCE_CONTAINER_NUMBER_OF_TOKENS) && (!ok) && (tokenList[i] != '\0')) {
        ok = (tokenList[i] == token);
        i++;
    }
    return ok;
}

MilliSeconds ReferenceContainer::GetTimeout() const {
    return muxTimeout;
}

void ReferenceContainer::SetTimeout(const MilliSeconds &timeout) {
    muxTimeout = timeout;
}


bool ReferenceContainer::AddBuildToken(const char8 token) {
    return AddToken(&buildTokensList[0], token);
}

void ReferenceContainer::RemoveBuildToken(const char8 token) {
    return RemoveToken(&buildTokensList[0], token);
}

bool ReferenceContainer::IsBuildToken(const char8 token) {
    return IsToken(&buildTokensList[0], token);
}

bool ReferenceContainer::AddDomainToken(const char8 token) {
    return AddToken(&domainTokensList[0], token);
}

void ReferenceContainer::RemoveDomainToken(const char8 token) {
    return RemoveToken(&domainTokensList[0], token);
}

bool ReferenceContainer::IsDomainToken(const char8 token) {
    return IsToken(&domainTokensList[0], token);
}

CLASS_REGISTER(ReferenceContainer, "1.0")


} //Marte




