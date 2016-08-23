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

#ifndef CONFIGURATION_DATABASE_H_
#define CONFIGURATION_DATABASE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                         Forward declarations                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class Reference;
class StreamString;
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
 *
 * Irrespectively of the interface the implementation shall support the concepts of navigable nodes and leafs
 * and shall support the following features:
 *
 * - Nodes shall be identified by a name;
 * - Leafs shall be identified by a name;
 * - One node may contain one or more nodes;
 * - One node may contain one or more leafs;
 * - One node shall not contain any AnyType value;
 * - One leaf shall contain one, and only one, AnyType value;
 * - Leafs shall not contain nodes;
 * - A write operation shall create a new leaf;
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
     * @brief Reads a previously stored AnyType. The node with this name has to be a child of the current node.
     * @param[in] name the name of the leaf used to store the AnyType \a value.
     * @param[out] value the read AnyType will be stored in this parameter. If the AnyType
     * cannot be successfully read its value will be set to VoidType and the function will return false.
     * @return true if the AnyType is successfully read.
     * @pre
     *   GetType(name).GetTypeDescriptor() != VoidType
     */
    virtual bool Read(const char8 * const name,
                      const AnyType &value) = 0;

    /**
     * @brief Reads the value in input in the desired path. The path can be
     * specified in order to use the domain node concept to find the path where the data must
     * be read from.
     * @param[in] The path where \a value must be read from.
     * @param[in] value is the value to be read.
     * @return true if \a value is correct and \value is read correctly.
     */
    virtual bool AdvancedRead(const char8 * const path,
                              const char8 *attributes,
                              const AnyType &value)=0;

    /**
     * @brief Writes the value in input in the desired path. The path can be
     * specified in order to use the domain node concept to find the path where the data must
     * be written to.
     * @param[in] The path where \a value must be written to.
     * @param[in] value is the value to be written.
     * @return true if \a value is correct and \value is written correctly.
     */
    virtual bool AdvancedWrite(const char8 * const path,
                               const char8 *attributes,
                               const AnyType &value)=0;
    /**
     * @brief Gets the type of a previously stored AnyType.
     * @param[in] name the name of the leaf used to store the AnyType \a value.
     * @return the type of the stored AnyType or VoidType if this \a name does not exist.
     */
    virtual AnyType GetType(const char8 * const name) = 0;

    virtual uint8 GetNumberOfDimensions(const char8 * const name)=0;

    virtual uint32 GetNumberOfElements(const char8 * const name,
                                       uint8 dimension)=0;

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
    virtual bool Write(const char8 * const name,
                       const AnyType &value) = 0;

    /**
     * @brief Copies the content of the current node to the provided destination.
     * @details A deep copy of the contents is recursively performed.
     * @param[out] destination where the database will be copied to.
     * @return true if the copy is successful.
     */
    virtual bool Copy(StructuredDataI &destination) = 0;

    virtual bool Link(StructuredDataI &destination)=0;

    /**
     * @brief Adds a node to the current node.
     * @param[in] node a reference to the node to add.
     * @return true if the node is successfully added.
     * @post
     *   If successful: the current node will be node
     *   If unsuccessful: the current node will not be changed
     */
    virtual bool AddToCurrentNode(Reference node) = 0;

    /**
     * @brief Moves the current node to the root node.
     * @return true if the move is successful and the current node is now the root node.
     */
    virtual bool MoveToRoot() = 0;

    /**
     * @brief Moves to the generations-th node containing this node.
     * @param[in] generations number of parent nodes to climb.
     * @return true if the move is successful and the current node is now the parent node which is n-generations above.
     * @pre
     *   generations > 0
     */
    virtual bool MoveToAncestor(uint32 generations) = 0;

    /**
     * @brief Moves the current node to a new node address specified by an absolute path.
     * @param[in] path a path with the node address.
     * @return true if the move was successful and the current node is the node described by \a path. If unsuccessful the current node
     * is not changed.
     */
    virtual bool MoveAbsolute(const char8 * const path) = 0;

    /**
     * @brief Moves the current node to an address specified by a path relative to the current node address.
     * @param[in] path a path with the node address.
     * @return true if the move was successful and the current node is the node described by \a path. If unsuccessful the current node
     * is not changed.
     */
    virtual bool MoveRelative(const char8 * const path) = 0;

    /**
     * @brief Moves in the tree in absolute or relative mode using the concept of domains as start points (a domain is defined
     * when the first character of the node name is a '$' symbol).
     * @param[in] path is the address of the node in the tree. The syntax is
     * "A.B.C" where A, B and C must be replaced with the specific node names.
     * We admit the syntax "::A.B.C" where the ':' symbol set the search start point to the previous domain with
     * respect to the current node. If no ':' is found at the beginning of the path, the start point is the root.
     * @return the reference found at the provided \a path or an invalid reference in case of failure.
     */
    virtual bool AdvancedMove(const char8 * const path)=0;

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
    virtual bool CreateAbsolute(const char8 * const path) = 0;

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
    virtual bool CreateRelative(const char8 * const path) = 0;

    /**
     * @brief Deletes the node with \a name under the current node (and as a consequence all the nodes underneath).
     * @param[in] name the name of the node to be deleted.
     * @return true if the current node is successfully removed.
     */
    virtual bool Delete(const char8 * const name) = 0;

    /**
     * @brief Retrieves the name of the current node.
     * @return the name of the current node.
     */
    virtual const char8 *GetName()=0;

    /**
     * @brief Retrieves the name of the child in the specified index.
     * @param[in] index is the index of the current node child.
     * @return the name of the child in the specified index.
     */
    virtual const char8 *GetChildName(const uint32 index)=0;

    /**
     * @brief Retrieves the number of children of the current node.
     * @return the number of children of the current node.
     */
    virtual uint32 GetNumberOfChildren()=0;

    /**
     * @brief Automatic cast to AnyType.
     */
    operator AnyType();

    /**
     * @brief Retrieves the path of the current node with respect to the root.
     * @param[out] path the path of the current node with respect to the root.
     * @return true
     */
    virtual bool GetFullPath(StreamString &path) = 0;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

inline StructuredDataI::~StructuredDataI() {
}

inline StructuredDataI::operator AnyType() {
    AnyType anyTypeConversion(StructuredDataInterfaceType, 0u, this);
    return anyTypeConversion;
}

}

#endif /* CONFIGURATION_DATABASE_H_ */
