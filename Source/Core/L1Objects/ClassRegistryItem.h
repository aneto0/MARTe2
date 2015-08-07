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
#include "Heap.h"
#include "LinkedListable.h"
#include "LoadableLibrary.h"
#include "ClassProperties.h"

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
class ClassRegistryItem: public LinkedListable {
public:
    /**
     * @brief Assigns the input variables to the class members.
     * @param[in] clProperties the class properties associated with the class that is being registered.
     * @param[in] objBuildFn the function that allows to instantiate a new object from the class
     * represented by this ClassRegistryItem instance.
     */
    ClassRegistryItem(const ClassProperties &clProperties,
                      ObjectBuildFn *objBuildFn);

    /**
     * Destructor.
     * Responsible for destroying the assigned loadable library.
     */
    ~ClassRegistryItem();

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
    uint32 GetNumberOfInstances();

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
     * @brief Returns a reference to the object allocation heap.
     * @return the heap that was selected to allocate objects of the class type represented by this registry item.
     */
    Heap *GetHeap();

    /**
     * @brief Sets the heap for object allocation.
     * @param[in] h the heap to allocate objects of the class type represented by this registry item.
     */
    void SetHeap(const Heap& h);

    /**
     * @brief Returns a pointer to the library (dll).
     * @return a pointer to the library (dll) of the class type represented by this registry item.
     */
    const LoadableLibrary *GetLoadableLibrary() const ;

    /**
     * @brief Updates the pointer to the loadable library (dll).
     * @param[in] lLibrary the library (dll) holding the class type represented by this registry item.
     */
    void SetLoadableLibrary(const LoadableLibrary *lLibrary);

    /**
     * @brief Returns a pointer to object build function.
     * @details Returns a pointer to a function that allows to instantiate a new object from
     * the class represented by this ClassProperties.
     * @return a pointer to function that allows to instantiate a new object.
     */
    ObjectBuildFn *GetObjectBuildFunction() const;

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
    Heap heap;

    /**
     * Library (dll) holding the class type represented by this registry item.
     * This class is responsible for destroying the pointer at the end of its life-cycle.
     */
    const LoadableLibrary *loadableLibrary;

    /**
     * The object instantiation function.
     */
    ObjectBuildFn *objectBuildFn;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L1OBJECTS_CLASSREGISTRYITEM_H_ */

