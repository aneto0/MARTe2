/**
 * @file StructuredDataI.h
 * @brief Header file for class StructuredDataI
 * @date 04/08/2015
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

 * @details This header file contains the declaration of the class StructuredDataI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRUCTURED_DATA_I_H_
#define STRUCTURED_DATA_I_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


#include "CCString.h"
#include "ErrorManagement.h"
#include "TypeDescriptor.h"

/*---------------------------------------------------------------------------*/
/*                         Forward declarations                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class Reference;
class AnyType;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Interface definition for any database that can store and retrieve AnyType values.
 * @details Classes that implement this interface are capable of storing and retrieving,
 * against any given name, data that is represented as an AnyType.
 *
 * The root node always exits and is anonymous to the users of the interface. This implies that the following
 * function calls are valid: CreateAbsolute("A.B.C")  && CreateAbsolute("D.E.F") &&
 * MoveToRoot() && MoveAbsolute("A.B.C") && MoveAbsolute("D.E.F")
 * Whenever the variable named path of type CCString is used, a sequence of path steps can be specified with the syntax above.
 *
 * Irrespectively of the interface the implementation shall support the concepts of navigable nodes and leafs
 * and shall support the following features:
 *
 * - Nodes shall be identified by a name;
 * - Leafs shall be identified by a name;
 * - Each node may contain one or more nodes;
 * - Each node may contain one or more leafs;
 * - Node shall not directly contain AnyType values;
 * - A leaf shall contain a single AnyType value;
 * - Leafs shall not contain nodes or leafs;
 * - Successful write operations shall create a new leaf;
 * - A read operation shall be performed on an existing leaf;
 * - The database shall know at any time what is the current node (i.e. the node against which the latest Move or
 * Create operation was performed).
 */
/*lint -e{9109} forward declaration of this class is required in other modules*/
class DLL_API StructuredDataI {
public:

    /**
     * @brief Destructor.
     */
    virtual ~StructuredDataI();

    /**
     * @brief Copies the value of a node into any variable. Path is relative to the current node.
     * @param[in] path the path to the node\a value.
     * @param[out] value the variable where the value is to be stored to. The variable shall be of a type compatible with AnyType.
     * @return true if the value is read and converted successfully.
     */
    virtual ErrorManagement::ErrorType Read(CCString path,const AnyType &value) = 0;

    /**
     * @brief Copies the value of a node into an Object . Path is relative to the current node.
     * @param[in] path the path to the node\a value.
     * @param[in] borrow if false the object created can be freely used, otherwise it shall not be modified
     * @param[out] object is a NULL reference which will be set to point an Object whose AnyType is a correct representation of the data
     * @return true if the value is read and converted successfully.
     */
    virtual ErrorManagement::ErrorType Read(CCString path,Reference &object,bool borrow=true) = 0;

    /**
     * @brief returns information of the variable stored at the specified path
     * @see VariableDescriptor::GetVariableDimensions
     * @param[in] path the path to the node\a value.
     * @param[out] td see VariableDescriptor::GetVariableDimensions
     * @param[in,out] nOfDimensions see VariableDescriptor::GetVariableDimensions
     * @param[in] dimensionSizes see VariableDescriptor::GetVariableDimensions
     * @return see VariableDescriptor::GetVariableDimensions
     */
    virtual ErrorManagement::ErrorType GetVariableInformation(
    		CCString 			path,
    		TypeDescriptor &	td,
			uint32 &			nOfDimensions,
			uint32 *			dimensionSizes) const = 0;

    /**
     * @brief Automatic cast to AnyType.
     */
    operator AnyType();

/**
 * require write access!
 */
    /**
     * @brief Writes an AnyType against the provided \a name and adds it to the current node.
     * @details If the name already exists the value will be overridden.
     * @param[in] name the name of the leaf against which the AnyType will be stored.
     * @param[in] value the AnyType to store.
     * @return true if the AnyType is successfully stored.
     * @pre
     *   name != NULL &&
     *   StringHelper::Length(name) > 0
     */
    virtual ErrorManagement::ErrorType Write(CCString name, const AnyType &value) = 0;

    /**
     * @brief Writes an object into the current node of the database.
     * @details The implementation shall assume that the object does not need to be copied but can be referenced to.
     * @param[in] object the object to be added to the database. object->GetName() provides the name.
     * @return true if object is valid
     */
    virtual ErrorManagement::ErrorType Write(Reference object) = 0;

    /**
     * @brief Copies the content of the current node to the provided destination.
     * @details A deep copy of the contents is recursively performed.
     * @param[out] destination where the database will be copied to.
     * @return true if the copy is successful.
     * TODO clarify if this copy is from root or
     */
    virtual ErrorManagement::ErrorType Copy(StructuredDataI &destination) = 0;

    /**
     * Affect the current position
     */

    /**
     * @brief Moves the current node to the root node.
     * @return true if the move is successful and the current node is now the root node.
     */
    virtual ErrorManagement::ErrorType MoveToRoot() = 0;

    /**
     * @brief Moves to the generations-th node containing this node.
     * @param[in] generations number of parent nodes to climb.
     * @return true if the move is successful and the current node is now the parent node which is n-generations above.
     * @pre
     *   generations > 0
     */
    virtual ErrorManagement::ErrorType MoveToAncestor(uint32 generations) = 0;

    /**
     * @brief Moves the current node to a new node address specified by an absolute path.
     * @param[in] path a path with the node address.
     * @return true if the move was successful and the current node is the node described by \a path. If unsuccessful the current node
     * is not changed.
     */
    virtual ErrorManagement::ErrorType MoveAbsolute(CCString path) = 0;

    /**
     * @brief Moves the current node to an address specified by a path relative to the current node address.
     * @param[in] path a path with the node address.
     * @return true if the move was successful and the current node is the node described by \a path. If unsuccessful the current node
     * is not changed.
     */
    virtual ErrorManagement::ErrorType MoveRelative(CCString path) = 0;

    /**
     * @brief Moves the current node the child with idx \a childIdx.
     * @param[in] childIdx the index of the child where to move to.
     * @return true if the move was successful and the current node is the node at index \a .childIdx If unsuccessful the current node
     * is not changed.
     */
    virtual ErrorManagement::ErrorType MoveToChild(const uint32 childIdx) = 0;

    /**
     * Read path information
     */

    /**
     * @brief Retrieves the name of the current node.
     * @return the name of the current node.
     */
    virtual CCString GetName()=0;

    /**
     * @brief Retrieves the name of the child in the specified index.
     * @param[in] index is the index of the current node child.
     * @return the name of the child in the specified index.
     */
    virtual CCString GetChildName(const uint32 index)=0;

    /**
     * @brief Retrieves the number of children of the current node.
     * @return the number of children of the current node.
     */
    virtual uint32 GetNumberOfChildren()=0;


/**
 *  Affect path
 */

    /**
     * @brief Adds a node to the current node.
     * @param[in] node a reference to the node to add.
     * @return true if the node is successfully added.
     * @post
     *   If successful: the current node will be node
     *   If unsuccessful: the current node will not be changed
     */
    virtual ErrorManagement::ErrorType AddToCurrentNode(Reference node) = 0;

    /**
     * @brief Create a new series of nodes based on the provided absolute path.
     * @param[in] path the path of nodes to be created.
     * @return true if the nodes were successfully created and if the path does not already exist.
     * @pre
     *   MoveAbsolute(path) == false
     * @post
     *   If successful: the current node will be the last node specified in the path.
     *   If unsuccessful: the current node will not be changed.
     */
    virtual ErrorManagement::ErrorType CreateAbsolute(CCString path) = 0;

    /**
     * @brief Create a new series of nodes based on the provided relative path.
     * @param[in] path the path of nodes to be created, relative to the current node.
     * @return true if the nodes were successfully created and if the path does not already exist.
     * @pre
     *   MoveRelative(path) == false
     * @post
     *   If successful: the current node will be the last node specified in the path.
     *   If unsuccessful: the current node will not be changed.
     */
    virtual ErrorManagement::ErrorType CreateRelative(CCString path) = 0;

    /**
     * @brief Deletes the node with \a name under the current node (and as a consequence all the nodes underneath).
     * @param[in] name the name of the node to be deleted.
     * @return true if the current node is successfully removed.
     */
    virtual ErrorManagement::ErrorType Delete(CCString path) = 0;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/




#endif /* CONFIGURATION_DATABASE_H_ */
