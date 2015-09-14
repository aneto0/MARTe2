/**
 * @file ClassRegistryItem.h
 * @brief Header file for class ClassRegistryItem
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

 * @details This header file contains the declaration of the class ClassRegistryItem
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSREGISTRYITEM_H_
#define CLASSREGISTRYITEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HeapI.h"
#include "LinkedListable.h"
#include "LoadableLibrary.h"
#include "ClassProperties.h"

/*lint -e{9141} forward declaration required. Cannot #include Object.h given that Object.h needs to know about ClassRegistryItem (for the registration macros)*/
class Object;
/*lint -e{9141} forward declaration required. Cannot #include Object.h given that Object.h needs to know about ClassRegistryItem (for the registration macros)*/
/**
 * @brief Definition of the ObjectBuildFn function type.
 * @param[in] heap the desired heap memory area where the object will be created.
 * @return a pointer to the created object.
 */
typedef Object *(ObjectBuildFn)(HeapManager::HeapI* const);
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Descriptor of framework base classes.
 * @details Most of the framework user classes inherit from Object. As a consequence,
 * they have the property of being automatically instantiated and managed by the framework.
 * Every class that inherits from Object will be described by a ClassRegistryItem and
 * automatically added to a ClassRegistryDatabase.
 */
/* TODO CHECK for remove this lint comment section */
/*lint -e{1790} for performance reasons it was decided to implement the usage of LinkedListable this way.
 * This guarantees that the movements in the list are always performed with the correct pointer (i.e. pointing to the base class).
 * Otherwise it would have required to use dynamic_cast which has a performance impact that we are not ready to give away here.*/
class ClassRegistryItem {
public:
    /**
     * @brief Default constructor
     */
    ClassRegistryItem();

    /**
     * @brief Assigns the input variables to the class members.
     * @param[in] clProperties the class properties associated with the class that is being registered.
     * @param[in] objBuildFn the function that allows to instantiate a new object from the class
     * represented by this ClassRegistryItem instance.
     */
    ClassRegistryItem(const ClassProperties &clProperties, const ObjectBuildFn * const objBuildFn);

    /**
     * Destructor.
     * Responsible for destroying the assigned loadable library.
     */
    virtual ~ClassRegistryItem();

    /**
     * @brief Increments the number of instantiated objects of the class type represented by this registry item.
     */
    void IncrementNumberOfInstances();

    /**
     * @brief Decrements the number of instantiated objects of the class type represented by this registry item.
     */
    void DecrementNumberOfInstances();

    /**
     * @brief Returns the number of instantiated objects.
     * @return the number of instantiated objects of the class type represented by this registry item.
     */
    uint32 GetNumberOfInstances() const;

    /**
     * @brief Returns a copy to the class parameters.
     * @param[in, out] destination the class properties will be copied to this variable.
     */
    void GetClassPropertiesCopy(ClassProperties &destination) const;

    /**
     * @brief Returns a pointer of the class parameters.
     * @details The method GetClassPropertiesCopy() should be used when possible. This pointer
     * will live as long as this instance of ClassRegistryItem exists.
     * @return a pointer to the class parameters represented by this registry item.
     */
    const ClassProperties *GetClassProperties() const;

    /**
     * @brief Returns a pointer to the library (dll).
     * @return a pointer to the library (dll) of the class type represented by this registry item.
     */
    const LoadableLibrary *GetLoadableLibrary() const ;

    /**
     * @brief Updates the pointer to the loadable library (dll).
     * @param[in] loadLibrary the library (dll) holding the class type represented by this registry item.
     */
    void SetLoadableLibrary(const LoadableLibrary * const loadLibrary);

    /**
     * @brief Returns a pointer to object build function.
     * @details Returns a pointer to a function that allows to instantiate a new object from
     * the class represented by this registry item.
     * @return a pointer to the function that allows to instantiate a new object.
     */
    const ObjectBuildFn *GetObjectBuildFunction() const;

    /**
     * @brief Sets the unique identifier for the class described by this ClassRegistryItem.
     * @param uid the new unique identifier to be set for the class described by this ClassRegistryItem.
     */
    void SetUniqueIdentifier(const uint32 &uid);

private:
    /**
     * The properties of the class represented by this registry item.
     */
    ClassProperties classProperties;

    /**
     * The number of instantiated objects of the class type represented by this registry item.
     */
    uint32 numberOfInstances;

    /**
     * Library (dll) holding the class type represented by this registry item.
     * This class is responsible for destroying the pointer at the end of its life-cycle.
     */
    const LoadableLibrary *loadableLibrary;

    /**
     * The object instantiation function.
     */
    const ObjectBuildFn *objectBuildFn;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSREGISTRYITEM_H_ */

