/**
 * @file ConfigurationDatabase.cpp
 * @brief Source file for class ConfigurationDatabase
 * @date 27/10/2015
 * @author Andre' Neto
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
 * the class ConfigurationDatabase (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#define DLL_API

#include "CompositeErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "ReferenceContainerFilterObjectName.h"
#include "ReferenceContainerFilterReferences.h"
#include "GlobalObjectsDatabase.h"
#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
 
namespace MARTe {

ConfigurationDatabase::ConfigurationDatabase() : Object() {
    mux.Create();
    ReferenceT < ReferenceContainer > rootContainer(buildNow);
    rootNode = rootContainer;
    currentNode = rootNode;
}

ConfigurationDatabase::~ConfigurationDatabase() {
}

void ConfigurationDatabase::Purge() {
    currentNode = rootNode;
    rootNode->Purge();
}

void ConfigurationDatabase::Purge(ReferenceContainer &purgeList) {
    if (currentNode.IsValid()) {
        currentNode->Purge(purgeList);
    }
    if (rootNode.IsValid()) {
        rootNode->Purge(purgeList);
    }
}

//TODO clarify if object is copied or the reference is copied.
ErrorManagement::ErrorType ConfigurationDatabase::Write(Reference object){
	ErrorManagement::ErrorType ret;
	ret.parametersError = !object.IsValid();
	REPORT_ERROR(ret,"parameter is an invalid reference");

	if (ret){
		// result is not needed
		// if the objects are found then are deleted
		// if they are not found fine too
		currentNode->Delete(object->GetName());
		currentNode->Insert(object);
	}

	return ret;
}


ErrorManagement::ErrorType ConfigurationDatabase::Write(CCString name, const AnyType &value) {
	ErrorManagement::ErrorType ret;

	Reference ref(value);

	ret.fatalError = !ref.IsValid();
	REPORT_ERROR(ret,"cannot create reference");

	if (ret){
		ref->SetName(name);
	}

	if (ret){
		ret = Write(ref);
	}

	return ret;

}

#define SAVE_NODE_POSITION()    ReferenceT < ReferenceContainer > storeCurrentNode = currentNode;
#define RESTORE_NODE_POSITION() currentNode = storeCurrentNode;


ErrorManagement::ErrorType ConfigurationDatabase::Read(CCString path,Reference &object,bool borrow){
	ErrorManagement::ErrorType ret;

	SAVE_NODE_POSITION()

	ret = MoveRelative(path);
	COMPOSITE_REPORT_ERROR(ret,"Failed moving to ",path );

    if (ret) {
    	if (borrow){
    		object = currentNode;
    	} else {
    		AnyType at;
    		currentNode->ToAnyType(at);

    		ret = at.Clone(object);
    	}
    }

	RESTORE_NODE_POSITION()

	return ret;
}

ErrorManagement::ErrorType ConfigurationDatabase::Read(CCString path, const AnyType &value) {
	ErrorManagement::ErrorType ret;

	SAVE_NODE_POSITION()

	ret = MoveRelative(path);
	COMPOSITE_REPORT_ERROR(ret,"Failed moving to ",path );
	AnyType at;

    if (ret) {
   		currentNode->ToAnyType(at);
   		ret = at.CopyTo(value);
    }

	RESTORE_NODE_POSITION()

	return ret;
}


ErrorManagement::ErrorType  ConfigurationDatabase::Copy(StructuredDataI &destination) {
	ErrorManagement::ErrorType ret;

	ReferenceT < ReferenceContainer > foundNode;
    for (uint32 i = 0u; (i < currentNode->Size()) && (ret); i++) {

    	foundNode = currentNode->Get(i);
        if (foundNode.IsValid()) {
        	ret = destination.CreateRelative(foundNode->GetName());
        	COMPOSITE_REPORT_ERROR(ret,"Failed to Create node ",foundNode->GetName());

        	if (ret){
        		ret = MoveRelative(foundNode->GetName());
        		COMPOSITE_REPORT_ERROR(ret,"Failed to Move to ",foundNode->GetName());
        	}

        	if (ret) {
                // go recursively !
                ret = Copy(destination);
            	REPORT_ERROR(ret,"Failed to Copy recursively");
            }

        	if (ret){
        		ret = MoveToAncestor(1u);
            	REPORT_ERROR(ret,"Failed to move back one level");
        	}

        	if (ret){
        		ret = destination.MoveToAncestor(1u);
            	REPORT_ERROR(ret,"Failed to move back one level destination");
        	}
        }
        else {
            Reference foundLeaf = currentNode->Get(i);
            ret = destination.Write(foundLeaf->GetName(), foundLeaf);
        }
    }
    return ret;
}

ErrorManagement::ErrorType ConfigurationDatabase::MoveToRoot() {
	ErrorManagement::ErrorType ret;
	ret.fatalError = !rootNode.IsValid();

	if (ret) {
        currentNode = rootNode;
    }
    return ret;
}

ErrorManagement::ErrorType ConfigurationDatabase::MoveAbsolute(CCString path) {
	ErrorManagement::ErrorType ret;

    ReferenceContainerFilterObjectName filter(1, 0u, path);
    ReferenceContainer resultSingle;
    rootNode->Find(resultSingle, filter);
    ret.invalidOperation = (resultSingle.Size() == 0u);
    COMPOSITE_REPORT_ERROR(ret,"Cannot find absolute path ",path);

    if (ret) {
        //Invalidate move to leafs
        ReferenceT < ReferenceContainer > container = resultSingle.Get(resultSingle.Size() - 1u);
        ret = !container.IsValid();
        COMPOSITE_REPORT_ERROR(ret,"Absolute path ",path," not found or not a node");
        if (ret) {
            currentNode = container;
        }
    }

    return ret;
}

ErrorManagement::ErrorType ConfigurationDatabase::MoveRelative(CCString path) {
	ErrorManagement::ErrorType ret;

    ReferenceContainerFilterObjectName filter(1, ReferenceContainerFilterMode::SHALLOW, path);
    ReferenceContainer resultSingle;
    currentNode->Find(resultSingle, filter);

    ret.invalidOperation = (resultSingle.Size() == 0u);
    COMPOSITE_REPORT_ERROR(ret,"Cannot find relative path ",path);

    if (ret) {
        //Invalidate move to leafs
        ReferenceT < ReferenceContainer > container = resultSingle.Get(resultSingle.Size() - 1u);
        ret = !container.IsValid();
        COMPOSITE_REPORT_ERROR(ret,"Relative path ",path," not found or not a node");
        if (ret) {
            currentNode = container;
        }
    }

    return ret;
}

ErrorManagement::ErrorType  ConfigurationDatabase::MoveToChild(const uint32 childIdx) {
	ErrorManagement::ErrorType ret;

	ret.outOfRange = (childIdx >= currentNode->Size());
    if (ret) {
        currentNode = currentNode->Get(childIdx);
    }
    return ret;
}


ErrorManagement::ErrorType ConfigurationDatabase::MoveToAncestor(const uint32 generations) {
	ErrorManagement::ErrorType ret;

    if (generations > 0u) {
        ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH, currentNode);
        ReferenceContainer resultPath;
        rootNode->Find(resultPath, filter);

        ret.invalidOperation = (resultPath.Size() == 0u);
        REPORT_ERROR(ret,"Cannot find myself from root");

        if (ret) {
            int32 newPositionIdx = (static_cast<int32>(resultPath.Size()) - 1) - static_cast<int32>(generations);
            ret.fatalError= !(newPositionIdx >= -1);
            COMPOSITE_REPORT_ERROR(ret,"Moving back too many steps: ",generations);

            if (ret) {
                if (newPositionIdx == -1) {
                    currentNode = rootNode;
                }
                else {
                    currentNode = resultPath.Get(static_cast<uint32>(newPositionIdx));
                }
            }
        }
    }
    return ret;
}

ErrorManagement::ErrorType ConfigurationDatabase::CreateNodes(CCString path) {
	ErrorManagement::ErrorType ret;

    DynamicCString token;
    bool created = false;
    ReferenceT < ReferenceContainer > currentNodeOld = currentNode;

    path = StringHelper::Tokenize(path, token,".","");
    while ((token.GetSize() > 0) && ret){

        bool found = false;
        Reference foundReference;
        uint32 i;
        for (i = 0u; (i < currentNode->Size()) && (!found); i++) {
            foundReference = currentNode->Get(i);
            found = (token.isSameAs(foundReference->GetName().GetList()));
        }

        if (found) {
            currentNode = foundReference;
        } else {
        	ReferenceT < ReferenceContainer > container(buildNow);
        	ret.fatalError = !container.IsValid();
            REPORT_ERROR(ret,"container(buildNow) failed");

        	if (ret){
                container->SetName(token);
                ret.fatalError = !currentNode->Insert(container);
                REPORT_ERROR(ret,"Node->Insert() failed");
        	}

            if (ret) {
                currentNode = container;
                created = true;
            }
        }

    	if (ret && (path.GetSize()>0)){
            path = StringHelper::Tokenize(path, token,".","");
    	} else {
    		token = "";
    	}
    }

    if (ret){
        ret.notCompleted = !created;
        // not really an error so do not report
    }
    // on error switch back 
    // but is not doing cleanup
    if (!ret) {
        currentNode = currentNodeOld;
    }
    return ret;
}

ErrorManagement::ErrorType  ConfigurationDatabase::CreateAbsolute(CCString path) {
    currentNode = rootNode;
    return CreateNodes(path);
}

ErrorManagement::ErrorType  ConfigurationDatabase::CreateRelative(CCString path) {
    return CreateNodes(path);
}

ErrorManagement::ErrorType  ConfigurationDatabase::Delete(CCString name) {
	ErrorManagement::ErrorType ret;

	Reference foundReference;
    uint32 i;
    bool found = false;
    for (i = 0u; (i < currentNode->Size()) && (!found); i++) {
        foundReference = currentNode->Get(i);
        found = (name == foundReference->GetName());
    }

    if (found) {
        ret.fatalError = !currentNode->Delete(foundReference);
        REPORT_ERROR(ret,"Node.Delete() failed");
    }
    // not really an error. Not reporting
    ret.notCompleted = !found;

    return ret;
}

ErrorManagement::ErrorType  ConfigurationDatabase::AddToCurrentNode(Reference node) {
	ErrorManagement::ErrorType ret;

	ReferenceT < ReferenceContainer > nodeToAdd = node;

	ret.parametersError = !nodeToAdd.IsValid();
    REPORT_ERROR(ret,"node is not a valid ReferenceContainer");

    if (ret) {
        ret.fatalError = !currentNode->Insert(nodeToAdd);
        REPORT_ERROR(ret,"Node->Insert failed");
    }
    return ret;
}

CCString ConfigurationDatabase::GetName() {
    return (currentNode.IsValid()) ? (currentNode->GetName()) : (NULL_PTR(const char8*));
}

CCString ConfigurationDatabase::GetChildName(const uint32 index) {
    Reference foundReference = currentNode->Get(index);
    return (foundReference.IsValid()) ? (foundReference->GetName()) : (NULL_PTR(const char8*));
}

uint32 ConfigurationDatabase::GetNumberOfChildren() {
    return currentNode->Size();
}

void ConfigurationDatabase::SetCurrentNodeAsRootNode() {
    rootNode = currentNode;
}

CLASS_REGISTER(ConfigurationDatabase, "1.0")

}
