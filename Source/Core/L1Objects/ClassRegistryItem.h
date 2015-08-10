/**
 * @file ClassRegistryItem.h
 * @brief Header file for class ClassRegistryItem
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

 * @details This header file contains the declaration of the class ClassRegistryItem
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_L1OBJECTS_CLASSREGISTRYITEM_H_
#define SOURCE_CORE_L1OBJECTS_CLASSREGISTRYITEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "../L0Portability/HeapInterface.h"
#include "LinkedListable.h"
#include "LoadableLibrary.h"
#include "ClassProperties.h"

/*lint -e{9141} forward declaration required. Cannot #include Object.h given that Object.h needs to know about ClassRegistryItem (for the registration macros)*/
class Object;
/*lint -e{9141} forward declaration required. Cannot #include Object.h given that Object.h needs to know about ClassRegistryItem (for the registration macros)*/
typedef Object *(ObjectBuildFn)(const HeapInterface &);
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Represents a framework base class (i.e. one that inherits from Object).
 * @details Most of the framework user classes inherit from Object. As a consequence,
 * they have the property of being automatically instantiated and managed by the framework.
 * Every class that inherits from Object will be described by a ClassRegistryItem and
 * automatically added to a ClassRegistryDatabase.
 */
/*lint -e{1790} for performance reasons it was decided to implement the usage of LinkedListable this way.
 * This guarantees that the movements in the list are always performed with the correct pointer (i.e. pointing to the base class).
 * Otherwise it would have required to use dynamic_cast which has a performance impact that we are not ready to give away here.*/
class ClassRegistryItem: public LinkedListable {
public:
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
     * @return the number of instantiated objects belonging of the class type represented by this registry item.
     */
    uint32 GetNumberOfInstances() const;

    /**
     * @brief Returns a copy to the class parameters.
     * @param[in, out] destination the class properties will be copied to this variable.
     */
    void GetClassPropertiesCopy(ClassProperties &destination) const;

    /**
     * @brief Returns a pointer to the class parameters.
     * @details The method GetClassPropertiesCopy should be used when possible. This pointer
     * will live as long as this instance of ClassRegistryItem exists.
     * @return a pointer to the class parameters represented by this registry item.
     */
    const ClassProperties *GetClassProperties() const;

    /**
     * @brief Sets the heap for object allocation.
     * @param[in] h the heap to allocate objects of the class type represented by this registry item.
     */
    void SetHeap(const HeapInterface& h);

    /**
     * @brief Returns a pointer to the library (dll).
     * @return a pointer to the library (dll) of the class type represented by this registry item.
     */
    const LoadableLibrary *GetLoadableLibrary() const ;

    /**
     * @brief Updates the pointer to the loadable library (dll).
     * @param[in] lLibrary the library (dll) holding the class type represented by this registry item.
     */
    void SetLoadableLibrary(const LoadableLibrary * const lLibrary);

    /**
     * @brief Returns a pointer to object build function.
     * @details Returns a pointer to a function that allows to instantiate a new object from
     * the class represented by this ClassProperties.
     * @return a pointer to function that allows to instantiate a new object.
     */
    const ObjectBuildFn *GetObjectBuildFunction() const;

    /**
     * @brief Delegates the deleting of the object to the correct heap.
     * @details It is expected that this function is only called by the Object macros upon deletion.
     * @param obj the object to be deleted.
     */
    void FreeObject(void *&obj);

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
     * The heap that is being used to instantiate objects of the class type represented by this registry item.
     */
    HeapInterface *heap;

    /**
     * Library (dll) holding the class type represented by this registry item.
     * This class is responsible for destroying the pointer at the end of its life-cycle.
     */
    const LoadableLibrary *loadableLibrary;

    /**
     * The object instantiation function.
     */
    const ObjectBuildFn *objectBuildFn;

    /**
     * @brief The default constructor is not supposed to be used
     */
    /*lint -e{1704} private constructor not to be called*/
    ClassRegistryItem();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L1OBJECTS_CLASSREGISTRYITEM_H_ */

