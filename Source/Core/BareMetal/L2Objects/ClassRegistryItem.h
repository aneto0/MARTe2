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
#include "LinkedListHolderT.h"
#include "LoadableLibrary.h"
#include "ClassProperties.h"
#include "FractionalInteger.h"
#include "ObjectBuilder.h"
#include "CString.h"
#include "ErrorType.h"
#include "ClassMethodsRegistryItem.h"
#include "LinkedListable.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {



class Introspection;
//class ClassMethodsRegistryItem;
class ReferenceContainer;

/*lint -e{9141} forward declaration required. Cannot #include Object.h given that Object.h needs to know about ClassRegistryItem (for the registration macros)*/
class Object;

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
     * todo
     */
    void SetIntrospection(Introspection *introspectionIn);

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
     * todo
     */
    void RegisterMethods(ClassMethodsRegistryItem *classMethodRecord);
    /**
     * todo
     */
    void SetObjectBuilder(ObjectBuilder *objectBuilderIn);

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
     * TODO
     */
    template <typename argType>
    ErrorManagement::ErrorType CallRegisteredMethod(Object *object,CCString methodName,argType parameters);

protected:
    /**
    // singleton approach - usable only by descendant methods
     * @brief Default constructor
     */
    ClassRegistryItem(ClassProperties &classProperties_in);

    /** singleton approach - usable only by descendant methods
     * common code
     * static ptr is specialised in the templetised descendant
     */
    static ClassRegistryItem *Instance(ClassRegistryItem *&instance,ClassProperties &classProperties_in);

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
     * TODO
     * */
    LinkedListHolderT<ClassMethodsRegistryItem, false> classMethods;

};



/**
 * TODO
 */
template <class T>
class DLL_API ClassRegistryItemT: public ClassRegistryItem{



public:
    /**
     * @brief Singleton access to the database.
     * @return a reference to the database.
     * TODO
     */
    static inline ClassRegistryItem *Instance(){
        static ClassRegistryItem *instance = NULL_PTR(ClassRegistryItem *);
        if (instance == NULL_PTR(ClassRegistryItem *)) {
            ClassRegistryItem::Instance(instance,T::classProperties);
            /* The next line, i.e. the instantiation of a static ObjectBuilderT
             * is necessary because it registers an ObjectBuilder object with a
             * default Build's implementation for the T class. Otherwise, it
             * will be a NULL pointer and a segmentation fault will eventually
             * arise when trying to use the object builder.
             */
            static ObjectBuilderT<T> defaultObjectBuilder;
        }
        return instance;
    }

    virtual ~ClassRegistryItemT() {

    }
private:
    /**
     * TODO
     */
    ClassRegistryItemT( ClassProperties &classProperties_in): ClassRegistryItem(classProperties_in){}

};




template <typename argType>
class DLL_API CallRegisteredMethodLauncher : public SearchFilterT<ClassMethodsRegistryItem > {
    CCString methodName;
    argType parameters;
    Object *object;
    ErrorManagement::ErrorType ret;
public:

    CallRegisteredMethodLauncher(Object *objectIn,CCString methodNameIn,argType parametersIn): parameters(parametersIn) {
        object = objectIn;
        methodName= methodNameIn;
    }

    virtual ~CallRegisteredMethodLauncher() {

    }

    virtual bool Test(ClassMethodsRegistryItem *data) {
        ret = data->CallFunction<argType>(object,methodName.GetList(),parameters);
        // the function has been found and called
        return !ret.unsupportedFeature;
    }

    ErrorManagement::ErrorType GetResults() {
        return ret;
    }

};

template <typename argType>
ErrorManagement::ErrorType ClassRegistryItem::CallRegisteredMethod(Object *object,
                                                                   CCString methodName,
                                                                   argType parameters) {
    ErrorManagement::ErrorType ret;

    if (object == NULL_PTR(Object*)) {
        ret.parametersError = true;
    }

    if (methodName.GetList() == NULL_PTR(char8*)) {
        ret.parametersError = true;
    }

    if (ret.NoError()) {
        // search in the list the first function returning without unsupported feature
        CallRegisteredMethodLauncher<argType> launcher(object, methodName, parameters);
        if (classMethods.ListSearch(&launcher)) {
            ret = launcher.GetResults();
        }
        else {
            ret.unsupportedFeature = true;
        }
    }
    return ret;
}


}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSREGISTRYITEM_H_ */

