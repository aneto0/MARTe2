/**
 * @file ConfigurationDatabase.h
 * @brief Header file for class ConfigurationDatabase
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

 * @details This header file contains the declaration of the class ConfigurationDatabase
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONFIGURATIONDATABASE_H_
#define CONFIGURATIONDATABASE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StructuredDataI.h"
#include "ReferenceT.h"
#include "ReferenceContainer.h"
#include "ReferenceContainerFilterObjectName.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe { 

/**
 * @brief An implementation of StructuredDataI where node paths are identified and separated by dots.
 * @details Paths are constructed by concatenating node names with dots (e.g. A.B.C.D), where D is a leaf
 * and A, B and C are nodes.
 *
 * A shared semaphore that can be used by the users of a database instance to have concurrent access to the database.
 */
class DLL_API ConfigurationDatabase: public StructuredDataI, public Object {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * Initialises the shared mutex Semaphore.
     * @post
     *   MoveToRoot() == true
     */
    ConfigurationDatabase();

    /**
     * @brief Destructor.
     */
    virtual ~ConfigurationDatabase();

    /**
     * @brief Removes all the elements from the database.
     */
    void Purge();

    /**
     * @brief Locks the shared semaphore.
     * @param[in] timeout maximum time to wait for the semaphore to be unlocked.
     * @return true if the shared semaphore is successfully locked.
     */
    inline ErrorManagement::ErrorType Lock(const MilliSeconds &timeout);

    /**
     * @brief Unlocks the shared semaphore.
     * @return true if the shared semaphore is successfully unlocked.
     */
    inline void Unlock();

/// StructuredDataI Interface

    /**
     * @see StructuredDataI::Read
     * @details It is possible read directly a structure or a class if
     * the node with the \a name provided has a compatible structure.
     * In this case the class must be introspectable and registered into
     * the ConfigurationDatabase.
     */
    virtual ErrorManagement::ErrorType Read(CCString path,const AnyType &value);

    /**
     * @brief Reads a previously stored AnyType and creates an Object.
     * If borrowed the object is part of the structuredDataI internal implementation and therefore cannot be modified.
     * @param[in] path the relative path to the leaf used to store the AnyType \a value.
     * @param[in] borrow if false the object created can be freely used, otherwise it shall not be modified
     * @param[out] the object reference to point to the output object
     * @return no errors if the AnyType is successfully read. Actual errors are implementation dependent
     * @pre
     *   GetType(path).GetTypeDescriptor() != VoidType
     */
    virtual ErrorManagement::ErrorType Read(CCString path,Reference &object,bool borrow=true);

    /**
     * @brief ShortCut to ConfigurationDatabase::Read(path,false)
     * @see ConfigurationDatabase::Read()
     */
    inline ErrorManagement::ErrorType Borrow(CCString path,Reference &object);

    /**
     * @see StructuredDataI::GetVariableInformation
     * @see VariableDescriptor::GetVariableDimensions
     */
    virtual ErrorManagement::ErrorType GetVariableInformation(
    		CCString 			path,
    		TypeDescriptor &	td,
			uint32 &			nOfDimensions,
			uint32 *			dimensionSizes) const;
#if 0
    /**
     * @see StructuredDataI::GetVariableInformation
     * @see VariableDescriptor::GetVariableDimensions
     */
    virtual ErrorManagement::ErrorType GetVariableDescriptor(
    		CCString 			path,
    		VariableDescriptor &vd) const;
#endif

    /**
     * @see StructuredDataI::Write
     * @details It is possible write directly a structure or a class
     * if it is introspectable and registered into
     * the ConfigurationDatabase.
     */
    virtual ErrorManagement::ErrorType Write(CCString name, const AnyType &value);

    /**
     * @brief Writes an object into the current node of the database.
     * @details The implementation shall assume that the object does not need to be copied but can be referenced to.
     * @param[in] object the object to be added to the database. object->GetName() provides the name.
     * @return true if object is valid
     */
    virtual ErrorManagement::ErrorType Write(Reference object);

    /**
     * @see StructuredDataI::Copy
     */
    virtual ErrorManagement::ErrorType Copy(StructuredDataI &destination);

    /**
     * @see StructuredDataI::MoveToRoot
     */
    virtual ErrorManagement::ErrorType MoveToRoot();

    /**
     * @see StructuredDataI::MoveToAncestor
     */
    virtual ErrorManagement::ErrorType MoveToAncestor(const uint32 generations);

    /**
     * @see StructuredDataI::MoveAbsolute
     */
    virtual ErrorManagement::ErrorType MoveAbsolute(CCString path);

    /**
     * @see StructuredDataI::MoveRelative
     */
    virtual ErrorManagement::ErrorType MoveRelative(CCString path);

    /**
     * @see StructuredDataI::MoveToChild
     */
    virtual ErrorManagement::ErrorType MoveToChild(const uint32 childIdx);

    /**
     * @see StructuredDataI::CreateAbsolute
     */
    virtual ErrorManagement::ErrorType CreateAbsolute(CCString path);

    /**
     * @see StructuredDataI::CreateRelative
     */
    virtual ErrorManagement::ErrorType CreateRelative(CCString path);

    /**
     * @see StructuredDataI::AddToCurrentNode
     */
    virtual ErrorManagement::ErrorType AddToCurrentNode(Reference node);

    /**
     * @see StructuredDataI::Delete
     */
    virtual ErrorManagement::ErrorType Delete(CCString name);

    /**
     * @brief Sets the current node as the root node.
     */
    void SetCurrentNodeAsRootNode();

    /**
     * @brief StructuredDataI::GetName
     */
    virtual CCString GetName();

    /**
     * @brief StructuredDataI::GetChildName
     */
    virtual CCString GetChildName(const uint32 index);

    /**
     * @brief StructuredDataI::GetNumberOfChildren
     */
    virtual uint32 GetNumberOfChildren();


    // TODO. Test and Document! adds the possibility to use find, filters ecc ecc
    /*operator ReferenceT<ReferenceContainer>(){
     return currentNode;
     }*/
    //TODO
    ReferenceT<ReferenceContainer> GetCurrentNode() {
        return currentNode;
    }

    /**
     * @see ReferenceContainer::Purge()
     */
    virtual void Purge(ReferenceContainer &purgeList);

private:

    ErrorManagement::ErrorType FindRelative(CCString path,ReferenceT<ReferenceContainer> &node) const;

    /**
     * @brief Create nodes relative to the currentNode.
     * @param[in] path the path to be created.
     * @return true if the path creation is successful.
     */
    ErrorManagement::ErrorType CreateNodes(CCString path);

    /**
     * The current node to where the database is pointing.
     */
    ReferenceT<ReferenceContainer> currentNode;

    /**
     * The root node of the database.
     */
    ReferenceT<ReferenceContainer> rootNode;


    //TODO use a path instead of rootNode, currentNode

    /**
     * The shared mutex semaphore.
     */
    FastPollingMutexSem mux;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

ErrorManagement::ErrorType ConfigurationDatabase::Lock(const MilliSeconds &timeout) {
    return mux.FastLock(timeout);
}

void ConfigurationDatabase::Unlock() {
    mux.FastUnLock();
}

inline ErrorManagement::ErrorType ConfigurationDatabase::Borrow(CCString path,Reference &object){
	return Read(path,object,true);
}


}


#endif /* CONFIGURATIONDATABASE_H_ */

