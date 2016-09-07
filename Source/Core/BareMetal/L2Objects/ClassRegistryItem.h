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
 *
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

#include "LinkedListHolderT.h"
#include "FractionalInteger.h"    //using ClassUID typedef
#include "CString.h"
#include "LinkedListable.h"

/*---------------------------------------------------------------------------*/
/*                         Forward declarations                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/*lint -e{9141} forward declaration required. Cannot #include Object.h given that Object.h needs to know about ClassRegistryItem (for the registration macros)*/
class Object;
//class ClassMethodsRegistryItem;
class ClassProperties;
class Introspection;
class LoadableLibrary;
class ObjectBuilder;
class ReferenceContainer;
class ClassMethodCaller;
class ClassMethodInterfaceMapper;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Descriptor of framework base classes.
 * @details Most of the framework user classes inherit from Object. As a consequence,
 * they have the property of being automatically instantiated and managed by the framework.
 * Every class that inherits from Object will be described by a ClassRegistryItem and
 * automatically added to a ClassRegistryDatabase.
 */
class DLL_API ClassRegistryItem: public LinkedListable {
public:

    /**
     * @brief Destructor. Responsible for destroying the assigned loadable library.
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
     * @brief Adds the introspection data.
     * @param[in] introspectioIn is the pointer to the object containing the informations
     * about the registered class attributes.
     */
    void SetIntrospection(const Introspection * const introspectionIn);

    /**
     * @brief Returns a pointer to the class introspection.
     * @return a pointer to the class introspection.
     */
    const Introspection * GetIntrospection() const;

    /**
     * @brief Updates the pointer to the loadable library (dll).
     * @param[in] loadLibrary the library (dll) holding the class type represented by this registry item.
     */
    void SetLoadableLibrary(const LoadableLibrary * const loadLibrary);

    /**
     * @brief Returns a pointer to the library (dll).
     * @return a pointer to the library (dll) of the class type represented by this registry item.
     */
    const LoadableLibrary *GetLoadableLibrary() const;

    /**
     * @brief Sets the object builder defining the way to allocate the memory when creating a new
     * instance of the registered class.
     * @param[in] objectBuilderIn is the object builder to be used to build new instances of the
     * registered class.
     */
    void SetObjectBuilder(const ObjectBuilder * const objectBuilderIn);

    /**
     * @brief Returns a pointer to object build function.
     * @details Returns a pointer to a function that allows to instantiate a new object from
     * the class represented by this registry item.
     * @return a pointer to the object factory.
     */
    const ObjectBuilder *GetObjectBuilder() const;

    /**
     * @brief Sets the unique identifier for the class described by this ClassRegistryItem.
     * @param[in] uid the new unique identifier to be set for the class described by this ClassRegistryItem.
     */
    void SetUniqueIdentifier(const ClassUID &uid);

    /**
     * @brief Gets the ClassMethodCaller associated to the method with name = methodName.
     * @param[in] methodName the name of the method.
     * @return the ClassMethodCaller associated to the method with name = methodName.
     */
    ClassMethodCaller *FindMethod(CCString methodName);

    /**
     * @brief Registers a method that can be later retrieved with FindMethod.
     * @param[in] method the method to register. The pointer will be freed by this class.
     */
    void AddMethod(ClassMethodInterfaceMapper *method);

protected:

    /**
     * @brief Default constructor. Singleton approach - usable only by descendant methods.
     * @param[in] classProperties_in class properties associated to this item.
     */
    ClassRegistryItem(ClassProperties &classProperties_in);

    /**
     * @brief Gets a pointer to this pseudo-singleton instance.
     * @param[in] instance if instance != NULL a new instance of ClassRegistryItem is created and assigned to instance.
     * This mechanism is used by the ClassRegistryItemT
     */
    static ClassRegistryItem *Instance(ClassRegistryItem *&instance, ClassProperties &classProperties_in);

private:

    /**
     * The properties of the class represented by this registry item.
     */
    ClassProperties &classProperties;

    /**
     * The number of instantiated objects of the class type represented by this registry item.
     */
    volatile int32 numberOfInstances;

    /**
     * Library (dll) holding the class type represented by this registry item.
     * This class is responsible for destroying the pointer at the end of its life-cycle.
     */
    const LoadableLibrary *loadableLibrary;

    /**
     * The object factory .
     */
    const ObjectBuilder *objectBuilder;

    /**
     * The introspection associated to the class.
     */
    const Introspection *introspection;

    /**
     * A list of lists of registered class methods.
     */
    LinkedListHolderT<ClassMethodInterfaceMapper, true> classMethods;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/






}

#endif /* CLASSREGISTRYITEM_H_ */

