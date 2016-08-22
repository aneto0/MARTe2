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

#include "AnyObject.h"
#include "TypeConversion.h"
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
class DLL_API ConfigurationDatabase: public StructuredDataI {

public:

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

<<<<<<< HEAD
    /**
     * @brief Destroys all the entries in the tree.
     */
=======

    //TODO test and document
>>>>>>> refs/remotes/origin/#44_GAM_Infrastructure
    void CleanUp();

    /**
     * @see StructuredDataI::Read
     * @details It is possible read directly a structure or a class if
     * the node with the \a name provided has a compatible structure.
     * In this case the class must be introspectable and registered into
     * the ConfigurationDatabase.
     */
    virtual bool Read(const char8 * const name,
                      const AnyType &value);

    /**
     * @brief Reads the value in input in the desired path. The path can be
     * specified in order to use the domain node concept to find the path where the data must
     * be read from.
     * @param[in] The path where \a value must be read from.
     * @param[in] value is the value to be read.
     * @return true if \a value is correct and \value is read correctly.
     */
    virtual bool AdvancedRead(const char8 * const path,
                              const AnyType &value);

    /**
     * @see StructuredDataI::GetType
     */
    virtual AnyType GetType(const char8 * const name);

    virtual uint8 GetNumberOfDimensions(const char8 * const name);

    virtual uint32 GetNumberOfElements(const char8 * const name,
                                       uint8 dimension);

    /**
     * @see StructuredDataI::Write
     * @details It is possible write directly a structure or a class
     * if it is introspectable and registered into
     * the ConfigurationDatabase.
     */
    virtual bool Write(const char8 * const name,
                       const AnyType &value);

    /**
     * @brief Writes the value in input in the desired path. The path can be
     * specified in order to use the domain node concept to find the path where the data must
     * be written to.
     * @param[in] The path where \a value must be written to.
     * @param[in] value is the value to be written.
     * @return true if \a value is correct and \value is written correctly.
     */
    virtual bool AdvancedWrite(const char8 * const path,
                               const AnyType &value);

    /**
     * @see StructuredDataI::Copy
     */
    virtual bool Copy(StructuredDataI &destination);

    virtual bool Link(StructuredDataI &destination);

    /**
     * @see StructuredDataI::MoveToRoot
     */
    virtual bool MoveToRoot();

    /**
     * @see StructuredDataI::MoveToAncestor
     */
    virtual bool MoveToAncestor(const uint32 generations);

    /**
     * @see StructuredDataI::MoveAbsolute
     */
    virtual bool MoveAbsolute(const char8 * const path);

    /**
     * @brief Moves in the tree in absolute or relative mode using the concept of domains as start points (a domain is defined
     * when the first character of the node name is a '$' symbol).
     * @param[in] path is the address of the node in the tree. The syntax is
     * "A.B.C" where A, B and C must be replaced with the specific node names.
     * We admit the syntax "::A.B.C" where the ':' symbol set the search start point to the previous domain with
     * respect to the current node. If no ':' is found at the beginning of the path, the start point is the root.
     * @return the reference found at the provided \a path or an invalid reference in case of failure.
     */
    virtual bool AdvancedMove(const char8 * const path);

    /**
     * @see StructuredDataI::MoveRelative
     */
    virtual bool MoveRelative(const char8 * const path);

    /**
     * @see StructuredDataI::CreateAbsolute
     */
    virtual bool CreateAbsolute(const char8 * const path);

    /**
     * @see StructuredDataI::CreateRelative
     */
    virtual bool CreateRelative(const char8 * const path);

    /**
     * @see StructuredDataI::AddToCurrentNode
     */
    virtual bool AddToCurrentNode(Reference node);

    /**
     * @see StructuredDataI::Delete
     */
    virtual bool Delete(const char8 * const name);

    /**
     * @brief StructuredDataI::GetName
     */
    virtual const char8 *GetName();

    /**
     * @brief StructuredDataI::GetChildName
     */
    virtual const char8 *GetChildName(const uint32 index);

    /**
     * @brief StructuredDataI::GetNumberOfChildren
     */
    virtual uint32 GetNumberOfChildren();

    /**
     * @brief Retrieves the path of the current node with respect to the root.
     * @param[out] path the path of the current node with respect to the root.
     * @return true
     */
    virtual bool GetFullPath(StreamString &path);

    /**
     * @brief Locks the shared semaphore.
     * @param[in] timeout maximum time to wait for the semaphore to be unlocked.
     * @return true if the shared semaphore is successfully locked.
     */
    bool Lock(const TimeoutType &timeout);

    /**
     * @brief Unlocks the shared semaphore.
     * @return true if the shared semaphore is successfully unlocked.
     */
    void Unlock();

    // TODO. Test and Document! adds the possibility to use find, filters ecc ecc
    operator ReferenceT<ReferenceContainer>(){
        return currentNode;
    }


private:

    /**
     * @brief Create nodes relative to the currentNode.
     * @param[in] path the path to be created.
     * @return true if the path creation is successful.
     */
    bool CreateNodes(const char8 * const path);

    /**
     * The current node to where the database is pointing.
     */
    ReferenceT<ReferenceContainer> currentNode;

    /**
     * The root node of the database.
     */
    ReferenceT<ReferenceContainer> rootNode;

    /**
     * The shared mutex semaphore.
     */
    FastPollingMutexSem mux;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONFIGURATIONDATABASE_H_ */

