/**
 * @file Object.h
 * @brief Header file for class Object
 * @date 04/08/2015
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
 *
 * @details This header file contains the declaration of the class Object
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OBJECT_H_
#define OBJECT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "HeapI.h"
#include "HeapManager.h"
#include "ClassProperties.h"
#include "ClassRegistryItem.h"
#include "ErrorType.h"
#include "StringHelper.h"
#include "StructuredDataI.h"
#include "CLASSREGISTER.h"
/*---------------------------------------------------------------------------*/
/*                         Forward declarations                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
//class StructuredDataI;
class ReferenceContainer;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief The root object of the framework.
 *
 * @details The MARTe Object is a class which offers the following functionality:
 *  - Its life cycle is managed by a smart pointer mechanism (see Reference);
 *  - Can be automatically constructed in runtime by the class name of its derived class;
 *  - Provides a standard initialisation/construction interface;
 *  - Is introspectable and enables reflection of derived classes;
 *  - The allocation heap for the object can be selected by the end-user.
 */
/*lint -e{9109} forward declaration of this class is required in other modules*/
class DLL_API Object {
    /**
     * This allows the Reference class to be the only interface to manage the number of instances pointing to this object.
     */
    friend class Reference;
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor. Sets the number of references to zero.
     * @post
     *   GetName() == copy.GetName()
     *   NumberOfReference() == 0
     *   IsDomain() == false
     */
    Object ();

    /**
     * @brief Copy constructor.
     * @param[in] copy is the Object to be copied.
     * @post
     *   GetName() == copy.GetName()
     *   NumberOfReference() == 0
     *   IsDomain() == false
     */
    Object(const Object &copy);

    /**
     * @brief Virtual destructor. No operation.
     */
    virtual ~Object();

    /**
     * @brief Initialises the object against a structured list of elements.
     * @details Upon a successful instantiation of the Object the framework will
     * call the Initialise method. The Object instance is then responsible for
     * retrieving the initialisation data from the input data and of assigning
     * these value to its internal variables.
     * @param[in] data the input initialisation data.
     * @return true if all the input \a data is valid and can be successfully assigned
     * to the Object member variables.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Purge internal links to references that cannot be reached when all the
     * explicit references to this object have been removed.
     * @details The smart pointer mechanism guarantees that when there are no references pointing
     *  at a given object this is destroyed.
     * Nevertheless, this does not prevent developers from creating internal references, so that there will
     *  be at least one reference pointing at the Object and thus it never gets destroyed.
     * An example: A is a specialised ReferenceContainer and has a member b which is also a ReferenceContainer.
     *  b contains an element C (b.Get(0)) which in turn contains a reference to A. A will then always have at least one
     *  unreachable reference pointing as b can only be destroyed (thus calling Delete(0) when A is destroyed,
     *  but A cannot be destroyed because it has a reference pointing at it. Upon calling this method, A is expected to remove all the references
     *  from b so that no implicit links exist and the global Purge (see ReferenceContainer::Purge) can remove all
     *  the references from the application.
     * @param[in] purgeList a container with all the elements to be purged.
     */
    virtual void Purge(ReferenceContainer &purgeList);

    /**
     * @brief Extracts the data of the object and puts it into an object which
     * implements the StructuredDataI interface.
     *
     * @details The returned StructuredDataI object is a tree which contains
     * a single node which represents the object's data. This node is named
     * with the object's name and has a leaf for the object's class name, plus
     * a leaf for each one of the class's members. If any of the members is of
     * complex type, then a new node is created instead of a leaf and a subtree
     * is created. This happens recursively for all members.
     * Below is pseudogrammar representing the tree with a textual syntax,
     * where "+" means a node and "-" a leaf. Each node has a name, while each
     * leaf has a name and a value.
     *
     * root
     *  |+<object's name>
     *   |-"Class": <object's class name>
     *   (|-<simple member's name>: <simple member's value>)*
     *   (|+<complex member's name>
     *     |-"Class": <object's class name>
     *     (|-<simple submember's name>: <simple member's value>)*
     *     (|+<complex submember's name>
     *       |..)*)*
     *
     * @param[out] data The holder for the tree that contains the extracted
     * data of the object.
     */
    virtual bool ExportData(StructuredDataI & data);

    /**
     * @brief Extracts the metadata of the object and puts it into an object
     * which implements the StructuredDataI interface.
     *
     * @details The returned StructuredDataI object is a tree which contains
     * a single node which represents the object's metadata.  This node is
     * named with the object's class name and has a sub node for each one of
     * the class's members. Those sub nodes, which represent each one a member,
     * have leaves for the metadata of the member, i.e. the type, the
     * modifiers, the attributes, the size, and the pointer to the member's
     * position. This happens recursively for all members, as deep as the
     * level parameter allows.
     * Below is pseudogrammar representing the tree with a textual syntax,
     * where "+" means a node and "-" a leaf. Each node has a name, while each
     * leaf has a name and a value.
     *
     * root
     *  |+<object's class name>
     *   (|+<member's name>
     *     |-"type": <type's value>
     *     |-"modifiers": <modifiers' value>
     *     |-"attributes": <attributes' value>
     *     |-"size": <size's value>
     *     |-"pointer": <object's address>+<member's offset>)*
     *
     * @param[out] data The holder for the tree that contains the extracted
     * metadata of the object.
     * @param[in] level The level of recursion, hence the depth of the tree.
     * Its values are between 0 and MAX_INT32. The -1 value means no limit,
     * so it will export recursively all the metadata.
     */
    virtual bool ExportMetadata(StructuredDataI & data,
            const int32 level = -1);

    /**
     * @brief Sets/unsets this object as a domain.
     * @details In a tree of objects, a domain object is a local root of a sub-tree.
     * Domain objects are identified with a prefix $ in their name.
     * @param[in] isDomainFlag true if the object is a domain.
     * @post
     *    IsDomain() == isDomainFlag
     */
    void SetDomain(const bool isDomainFlag);

    /**
     * @brief Returns true if this object is a domain.
     * @details The definition of a domain is related to the ObjectRegistryDatabase. A domain object can be used as a start research point
     * in ObjectRegistryDatabase::Find(*) to find objects in the ObjectRegistryDatabase.
     * @return true if this object is a domain.
     */
    bool IsDomain() const;

    /**
     * @brief Returns the number of references.
     * @return the number of references pointing to this object.
     */
    uint32 NumberOfReferences() const;

    /**
     * @brief Returns the object name.
     * @return the object name (which might be NULL).
     */
    const char8 * const GetName() const;

    /**
     * @brief Returns an object name which is guaranteed to be unique.
     * @details The object unique name is composed by the object memory
     *  address and by the object name as returned by GetName().
     *
     * If GetName() returns NULL the unique name will be the object memory address.
     * The format of the unique name is xMemoryAddress::Name. The leading zeros of the
     * memory address are discarded.
     * @param[in, out] destination the destination where to write the unique object to.
     * If enough space is available the string will be zero terminated.
     * @param[in] size the size of the \a destination input string.
     */
    void GetUniqueName(char8 * const destination,
            const uint32 &size) const;

    /**
     * @brief Sets the object name.
     * @details If a name had already been set the object name will be updated to this name.
     * @param newName the new name of the Object. A private copy of the \a name will be performed and managed by the Object.
     * @pre newName != NULL
     */
    void SetName(const char8 * const newName);

    /**
     * @brief Calls a registered method without arguments.
     * @param[in] methodName is the method name.
     * @return ErrorManagement::UnsupportedFeature if the \a methodName is not registered or if the prototype is not supported.
     * ErrorManagement::FatalError will be returned if the function returns false, ErrorManagement::NoError otherwise.
     */
    ErrorManagement::ErrorType CallRegisteredMethod(const CCString &methodName);

    /**
     * @brief Calls a registered method without arguments.
     * @param[in] methodName is the method name.
     * @param[in] parameters a reference to a ReferenceContainer that will be used to fill the parameters to call the functions
     * @return ErrorManagement::UnsupportedFeature if the \a methodName is not registered or if the prototype is not supported.
     * ErrorManagement::FatalError will be returned if the function returns false, ErrorManagement::NoError otherwise.
     */
    ErrorManagement::ErrorType CallRegisteredMethod(const CCString &methodName, ReferenceContainer &parameters);

    /**
     * @brief Calls a registered method without arguments.
     * @param[in] methodName is the method name.
     * @param[in] parameters a reference to a StructuredDataI object where to read/write parameters/results.
     * @return ErrorManagement::UnsupportedFeature if the \a methodName is not registered or if the prototype is not supported.
     * ErrorManagement::FatalError will be returned if the function returns false, ErrorManagement::NoError otherwise.
     */
    ErrorManagement::ErrorType CallRegisteredMethod(const CCString &methodName, StructuredDataI &parameters);

    /**
     * @brief Calls a registered method without arguments.
     * @param[in] methodName is the method name.
     * @param[in] stream a reference to a Stream that will be used to fill the parameters to call the functions.
     * @return ErrorManagement::UnsupportedFeature if the \a methodName is not registered or if the prototype is not supported.
     * ErrorManagement::FatalError will be returned if the function returns false, ErrorManagement::NoError otherwise.
     */
    ErrorManagement::ErrorType CallRegisteredMethod(const CCString &methodName, StreamI &stream);

    /**
     * @brief Returns the class properties associated with this class type.
     * @return a pointer to the class properties object (which might be NULL).
     */
    const ClassProperties *GetClassProperties() const;

    /**
     * @brief Returns true if the Object is a ReferenceContainer.
     * @details This function was added for optimisation. The alternative of doing
     * ReferenceT<ReferenceContainer> test = rc; test.IsValid(), was to slow (as it is  
     * called millions of times).
     * @return true if the Object is a ReferenceContainer.
     */
    virtual bool IsReferenceContainer() const;

private:



    /**
     * @brief Decrements the number of references to this object.
     * @details Only accessible to the Reference class.
     * @return the number of references after the operation.
     */
    uint32 DecrementReferences();

    /**
     * @brief Increments the number of references to this object.
     * @details Only accessible to the Reference class.
     */
    void IncrementReferences();

    /**
     * @brief Clones the object.
     * @details To enable cloning of objects using references the final class must implement clone.
     * Only accessible to the Reference class.
     * @return a clone of this object.
     */
    virtual Object* Clone() const;

    /**
     * @brief Disallow the usage of new.
     * @param[in] size the size of the object.
     */
    static void *operator new(size_t size) throw ();

    /**
     * @brief Extracts the data of an input object and puts it into an object
     * which implements the StructuredDataI interface.
     * @details The returned StructuredDataI object is a tree which contains a
     * single node which represents the *ptr's data. This node is named with
     * the value of the objName parameter and has a leaf with the className
     * parameter as a value, plus a leaf for each one of the class's members.
     * If any of the members is of complex type, then a new node is created
     * instead of a leaf and a subtree is created. This happens recursively
     * for all members.
     * @param[in] ptr The pointer to the object whose data is expected to be
     * extracted.
     * @param[in] className The name of the class of the object pointed by ptr.
     * @param[out] data The holder for the tree that contains the extracted
     * data of the object.
     * @param[in] objName The name of the object, i.e. the root's name.
     */
    bool ConvertDataToStructuredData(void* const ptr,
            const char8* const className,
            StructuredDataI& data,
            const char8* const objName = NULL);

    /**
     * @brief Extracts the metadata of an input object and puts it into an
     * object which implements the StructuredDataI interface.
     * @details The returned StructuredDataI object is a tree which contains
     * a single node which represents the *ptr's metadata.  This node is
     * named with the className parameter and has a sub node for each one of
     * the class's members. Those sub nodes, which represent each one a member,
     * have leaves for the metadata of the member, i.e. the type, the
     * modifiers, the attributes, the size, and the pointer to the member's
     * position. This happens recursively for all members, as deep as the
     * recursionlevel parameter allows.
     * @param[in] ptr The pointer to the object whose metadata is expected to
     * be extracted.
     * @param[in] className The name of the class of the object pointed by ptr.
     * @param[out] data The holder for the tree that contains the extracted
     * metadata of the object.
     * @param[in] level The level of recursion, hence the depth of the tree.
     */
    bool ConvertMetadataToStructuredData(void * const ptr,
            const char8 * const className,
            StructuredDataI &data,
            const int32 recursionLevel = -1);

    /**
     * The number of references to this object.
     */
    volatile int32 referenceCounter;

    /**
     * The name of this object.
     */
    char8 *thisObjName;

    /**
     * Specifies if the object is a domain
     */
    bool isDomain;
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/








}

#endif /* OBJECT_H_ */

