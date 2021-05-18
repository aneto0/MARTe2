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

#include "AdvancedErrorManagement.h"
#include "AnyObject.h"
#include "ErrorType.h"
#include "ConfigurationDatabase.h"
#include "StreamString.h"
#include "TypeConversion.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ConfigurationDatabase::ConfigurationDatabase() :
        Object() {
    mux.Create();
    ReferenceT<ConfigurationDatabaseNode> rootContainer(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    rootNode = rootContainer;
    currentNode = rootNode;
}

/*lint -e{1551} by design memory if freed in the destructor.*/
ConfigurationDatabase::~ConfigurationDatabase() {
    currentNode = Reference();
    Purge();
}

ConfigurationDatabase::ConfigurationDatabase(const ConfigurationDatabase &toCopy) :
        Object(toCopy) {
    mux = toCopy.mux;
    rootNode = toCopy.rootNode;
    currentNode = toCopy.currentNode;
}

ConfigurationDatabase &ConfigurationDatabase::operator =(const ConfigurationDatabase &toCopy) {
    if (this != &toCopy) {
        currentNode = Reference();
        Purge();
        mux = toCopy.mux;
        rootNode = toCopy.rootNode;
        currentNode = toCopy.currentNode;
    }
    return *this;
}

void ConfigurationDatabase::Purge() const {
    //If the only references pointing at the rootNode are itself and eventually all its child nodes then it can be purged
    //Note that for every direct child of the rootNode a link to it (the parent) is created
    uint32 numberOfReferences = (rootNode.NumberOfReferences() - 1u);
    if (rootNode == currentNode) {
        //currentNode is pointing at rootNode
        numberOfReferences--;
    }
    if(numberOfReferences == rootNode->GetNumberOfNodes()) {
        rootNode->Purge();
    }
}

bool ConfigurationDatabase::Write(const char8 * const name, const AnyType &value) {

    bool ok = false;
    // call conversion Object-StructuredDataI or StructuredDataI-StructuredDataI
    bool isRegisteredObject = (value.GetTypeDescriptor().isStructuredData);
    bool isStructuredDataI = (value.GetTypeDescriptor() == StructuredDataInterfaceType);
    if ((isRegisteredObject) || (isStructuredDataI)) {
        ReferenceT<ConfigurationDatabaseNode> storeCurrentNode = currentNode;
        if (CreateRelative(name)) {
            ok = TypeConvert((*this).operator MARTe::AnyType(), value);
        }
        currentNode = storeCurrentNode;
    }
    else {
        ok = (StringHelper::Length(name) > 0u);
        if (ok) {
            AnyType existentType = GetType(name);
            if (existentType.GetTypeDescriptor() != voidAnyType.GetTypeDescriptor()) {
                ok = Delete(name);
            }
        }
        if (ok) {
            ReferenceT<AnyObject> objToWrite(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            ok = objToWrite.IsValid();
            if (ok) {
                ok = objToWrite->Serialise(value);
                if (ok) {
                    objToWrite->SetName(name);
                    ok = currentNode.IsValid();
                    if (ok) {
                        ok = currentNode->Insert(objToWrite);
                    }
                }
            }
        }
    }
    return ok;
}

AnyType ConfigurationDatabase::GetType(const char8 * const name) {
    AnyType retType;
    if (currentNode.IsValid()) {
        ReferenceT<AnyObject> objToRead = currentNode->FindLeaf(name);
        if (objToRead.IsValid()) {
            retType = objToRead->GetType();
        }
    }

    return retType;
}

bool ConfigurationDatabase::Copy(StructuredDataI &destination) {
    ReferenceT<ConfigurationDatabaseNode> foundNode;
    bool ok = true;
    for (uint32 i = 0u; (i < currentNode->Size()) && (ok); i++) {
        foundNode = currentNode->Get(i);
        if (foundNode.IsValid()) {
            if (!destination.CreateRelative(foundNode->GetName())) {
                ok = false;
            }
            if ((!MoveRelative(foundNode->GetName())) && ok) {
                ok = false;
            }
            if (ok) {
                // go recursively !
                ok = Copy(destination);
            }
            if ((!MoveToAncestor(1u)) && ok) {
                ok = false;
            }
            if ((!destination.MoveToAncestor(1u)) && ok) {
                ok = false;
            }
        }
        else {
            ReferenceT<AnyObject> foundLeaf = currentNode->Get(i);

            if (foundLeaf.IsValid()) {
                ok = destination.Write(foundLeaf->GetName(), foundLeaf->GetType());
            }
        }
    }
    return ok;
}

bool ConfigurationDatabase::Initialise(StructuredDataI &data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        ok = data.Copy(*this);
    }
    if (ok) {
        rootNode->SetName(data.GetName());
    }
    return ok;
}

bool ConfigurationDatabase::MoveToRoot() {
    bool ok = rootNode.IsValid();
    if (ok) {
        currentNode = rootNode;
    }
    return ok;
}

bool ConfigurationDatabase::Read(const char8 * const name, const AnyType &value) {

    bool ok = false;
    // call conversion Object-StructuredDataI or StructuredDataI-StructuredDataI
    bool isRegisteredObject = (value.GetTypeDescriptor().isStructuredData);
    bool isStructuredDataI = (value.GetTypeDescriptor() == StructuredDataInterfaceType);
    if ((isRegisteredObject) || (isStructuredDataI)) {
        ReferenceT<ConfigurationDatabaseNode> storeCurrentNode = currentNode;
        if (MoveRelative(name)) {
            ok = TypeConvert(value, (*this).operator MARTe::AnyType());
        }
        currentNode = storeCurrentNode;
    }
    else {
        ok = currentNode.IsValid();
        if (ok) {
            //Could have used the ReferenceContainerFilterObjectName but this way is faster given that no complex paths are involved
            ReferenceT<AnyObject> objToRead = currentNode->FindLeaf(name);
            ok = objToRead.IsValid();
            if (ok) {
                ok = TypeConvert(value, objToRead->GetType());
            }
        }
    }

    return ok;
}

bool ConfigurationDatabase::MoveAbsolute(const char8 * const path) {
    //Invalidate move to leafs
    ReferenceT<ConfigurationDatabaseNode> container = rootNode->Find(path);
    bool ok = container.IsValid();
    if (ok) {
        currentNode = container;
    }

    return ok;
}

bool ConfigurationDatabase::MoveRelative(const char8 * const path) {

    ReferenceT<ConfigurationDatabaseNode> container = currentNode->Find(path);
    bool ok = container.IsValid();
    if (ok) {
        currentNode = container;
    }

    return ok;
}

bool ConfigurationDatabase::MoveToChild(const uint32 childIdx) {
    bool ok = (childIdx < currentNode->Size());
    if (ok) {
        ReferenceT<ConfigurationDatabaseNode> temp = currentNode->Get(childIdx);
        ok = temp.IsValid();
        if (ok) {
            currentNode = temp;
        }
    }
    return ok;
}

bool ConfigurationDatabase::MoveToAncestor(const uint32 generations) {
    bool ok = (generations != 0u);
    if (ok) {
        ReferenceT<ConfigurationDatabaseNode> oldCurrent = currentNode;
        for (uint32 i = 0u; (i < generations) && (ok); i++) {
            ReferenceT<ConfigurationDatabaseNode> father = currentNode->GetParent();
            ok = father.IsValid();
            if (ok) {
                if (father == rootNode) {
                    ok = (generations == (i + 1u));
                }
                currentNode = father;
            }
        }

        if (!ok) {
            currentNode = oldCurrent;
        }
    }
    return ok;
}

bool ConfigurationDatabase::CreateNodes(const char8 * const path) {
    StreamString pathStr = path;
    bool ok = pathStr.Seek(0Lu);
    if (ok) {
        ok = (pathStr.Size() > 0u);
    }
    StreamString token;
    char8 c;
    bool created = false;
    ReferenceT<ConfigurationDatabaseNode> currentNodeOld = currentNode;

    while ((pathStr.GetToken(token, ".", c)) && (ok)) {
        ok = (token.Size() > 0u);
        if (ok) {
            //Check if a node with this name already exists
            Reference foundReference = currentNode->FindLeaf(token.Buffer());
            if (foundReference.IsValid()) {
                currentNode = foundReference;
            }
            else {
                ReferenceT<ConfigurationDatabaseNode> container(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                container->SetName(token.Buffer());
                ok = currentNode->Insert(container);
                if (ok) {
                    container->SetParent(currentNode);
                    currentNode = container;
                    created = true;
                }
            }
        }

        if (ok) {
            ok = token.Seek(0Lu);
            if (ok) {
                ok = token.SetSize(0Lu);
            }

        }
    }
    if (ok) {
        ok = created;
    }
    if (!ok) {
        currentNode = currentNodeOld;
    }
    return ok;
}

bool ConfigurationDatabase::CreateAbsolute(const char8 * const path) {
    currentNode = rootNode;
    return CreateNodes(path);
}

bool ConfigurationDatabase::CreateRelative(const char8 * const path) {
    return CreateNodes(path);
}

bool ConfigurationDatabase::Delete(const char8 * const name) {
    Reference foundReference = currentNode->FindLeaf(name);
    bool ok = (foundReference.IsValid());
    if (ok) {
        ok = currentNode->Delete(foundReference);
    }

    return ok;
}

bool ConfigurationDatabase::AddToCurrentNode(Reference node) {
    ReferenceT<ConfigurationDatabaseNode> nodeToAdd = node;
    bool ok = nodeToAdd.IsValid();
    if (ok) {
        ok = currentNode->Insert(nodeToAdd);
    }
    return ok;
}

const char8 *ConfigurationDatabase::GetName() {
    return (currentNode.IsValid()) ? (currentNode->GetName()) : (NULL_PTR(const char8*));
}

const char8 *ConfigurationDatabase::GetChildName(const uint32 index) {
    Reference foundReference = currentNode->Get(index);
    return (foundReference.IsValid()) ? (foundReference->GetName()) : (NULL_PTR(const char8*));
}

uint32 ConfigurationDatabase::GetNumberOfChildren() {
    uint32 size = 0u;
    if (currentNode.IsValid()) {
        size = currentNode->Size();
    }
    return size;
}

bool ConfigurationDatabase::Lock(const TimeoutType &timeout) {
    return (mux.FastLock(timeout) == ErrorManagement::NoError);
}

void ConfigurationDatabase::Unlock() {
    mux.FastUnLock();
}

/*Reference ConfigurationDatabase::GetCurrentNode() const {
    return currentNode;
}*/

void ConfigurationDatabase::SetCurrentNodeAsRootNode() {
    rootNode = currentNode;
}

CLASS_REGISTER(ConfigurationDatabase, "1.0")

}
