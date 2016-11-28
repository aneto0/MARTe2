/**
 * @file ClassRegistryItem2.h
 * @brief Header file for class ClassRegistryItem2
 * @date Nov 18, 2016
 * @author fsartori
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

 * @details This header file contains the declaration of the class ClassRegistryItem2
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L2OBJECTS_CLASSREGISTRYITEM2_H_
#define L2OBJECTS_CLASSREGISTRYITEM2_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CString.h"
#include "LinkedListable.h"
#include "LoadableLibrary.h"
#include "TypeDescriptor.h"
//#include "Introspection.h"
#include "ClassMethodInterfaceMapper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class ClassMethodCaller;
class ObjectBuilder;
class ClassMember;
class VariableDescriptor;


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
     * @brief Add missing details.
     */
    void SetClassDetails(CCString classNameIn,CCString classVersionIn);
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
    const ObjectBuilder *   GetObjectBuilder() const;

    /**
     * @brief Sets the type descriptor for the class described by this ClassRegistryItem.
     * @return the TypeDescriptor.
     */
    TypeDescriptor          GetTypeDescriptor();

    /**
     * @brief  Get the name of the class (by default the same as returned by typeid.
     */
    CCString                GetClassName();

    /**
     * @brief  The name of the class as returned by typeid.
     */
    CCString                GetTypeidName();

    /**
     * @brief  The version of the class.
     */
    CCString                GetClassVersion();

    /**
     * @brief  The size of the class.
     */
    uint32                  GetSizeOfClass();

    /**
     * @brief Gets the ClassMethodCaller associated to the method with name = methodName.
     * @param[in] methodName the name of the method.
     * @return the ClassMethodCaller associated to the method with name = methodName.
     */
    ClassMethodCaller *     FindMethod(CCString methodName);

    /**
     * @brief Registers a method that can be later retrieved with FindMethod.
     * @param[in] method the method to register. The pointer will be freed by this class.
     */
    void                    AddMethod(ClassMethodInterfaceMapper * const method);

    /**
     * @brief Gets the ClassMember associated to the member with name = memberName.
     * @param[in] memberName the name of the member.
     * @return the ClassMember associated to the member with name = memberName.
     */
    VariableDescriptor *     FindMember(CCString memberName);

    /**
     * @brief Registers a member that can be later retrieved with FindMethod.
     * @param[in] the member to register. The pointer will be freed by this class.
     */
    void                    AddMember(ClassMember * const member);


    /**
     * @brief Destructor.
     */
    virtual               ~ClassRegistryItem();

    /**
     * @brief Allows obtaining per-class singleton ClassRegistryItem
     */
    template <class T>
    static ClassRegistryItem *Instance();

private:

    /**
     * @brief Constructor.
     */
    ClassRegistryItem(CCString typeidNameIn,uint32 sizeOfClassIn);

    /**
     * The number of instantiated objects of the class type represented by this registry item.
     */
    volatile int32            numberOfInstances;

    /**
     * Library (dll) holding the class type represented by this registry item.
     * This class is responsible for destroying the pointer at the end of its life-cycle.
     */
    const LoadableLibrary *   loadableLibrary;

    /**
     * The object factory .
     */
    const ObjectBuilder *     objectBuilder;

    /**
     * The introspection associated to the class.
     */
//    const Introspection *     introspection;

    /**
     * The name of the class.
     */
    CCString                  className;

    /**
     * The name of the class as returned by typeid.
     */
    CCString                  typeidName;

    /**
     * The version of the class.
     */
    CCString                  classVersion;

    /**
     * A unique identifier for this class
     */
    TypeDescriptor            typeDescriptor;

    ///
    uint32                    sizeOfClass;

    /**
     * A list of lists of registered class methods.
     */
    LinkedListHolderT<ClassMethodInterfaceMapper,true> classMethods;

    /**
     * A list of members of the class.
     */
    LinkedListHolderT<ClassMember,true> classMembers;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


template <class T>
 ClassRegistryItem *ClassRegistryItem::Instance(){
    /**
     * static variable. not automatic! persistent across calls
     * will be initialised with a pointer to the only valid instance of this
     */
    static ClassRegistryItem *instance = NULL_PTR(ClassRegistryItem *);

    /// first time will go inside here
    if (instance == NULL_PTR(ClassRegistryItem *)) {

        /// all common code here
        instance = new ClassRegistryItem(typeid(T).name(),sizeof(T) );

    }
    return instance;
}


}
#endif /* L2OBJECTS_CLASSREGISTRYITEM2_H_ */
	
