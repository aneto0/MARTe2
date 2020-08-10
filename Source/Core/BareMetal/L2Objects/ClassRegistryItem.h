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

#include <typeinfo>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CString.h"
#include "LoadableLibrary.h"
#include "TypeDescriptor.h"
#include "LinkedListHolderT.h"
#include "ClassRegistryIndex.h"
#include "ObjectBuilderT.h"
#include "FastPollingMutexSem.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class ClassMethodCaller;
class ObjectBuilder;
class ClassMember;
class VariableDescriptor;
class ClassMethodInterfaceMapper;
class Object;

/**
 * @brief Descriptor of framework base classes.
 * @details Most of the framework user classes inherit from Object. As a consequence,
 * they have the property of being automatically instantiated and managed by the framework.
 * Every class that inherits from Object will be described by a ClassRegistryItem and
 * automatically added to a ClassRegistryDatabase.
 * NOTE! ClassRegistryItemI must be the sole ancestor or else a mechanism to retrieve the root must be provided
 */
class DLL_API ClassRegistryItem: public ClassRegistryItemI {
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
    virtual const TypeDescriptor &  GetTypeDescriptor() const;

    /**
     * @brief  Get the name of the class (by default the same as returned by typeid.
     */
    virtual CCString                GetClassName() const;

    /**
     * @brief  The name of the class as returned by typeid.
     */
    virtual CCString                GetTypeidName() const;

    /**
     * @brief  The version of the class.
     */
    virtual CCString                GetClassVersion() const;

    /**
     * @brief  The size of the class.
     */
    virtual uint32                  GetSizeOfClass() const;

    /**
     * @brief  allows recovering the base pointer to ClassRegistryItem from this interface
     * without using dynamic_cast which might not works as this class is not known here
     */
    virtual ClassRegistryItem *		GetBasePtr() {
    	return this;
    }

    /**
     * @brief Gets the ClassMethodCaller associated to the method with name = methodName.
     * @param[in] methodName the name of the method.
     * @return the ClassMethodCaller associated to the method with name = methodName.
     */
    ClassMethodCaller *     FindMethod(CCString methodName) ;

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
    ClassMember const *     FindMember(CCString memberName) const;

    /**
     * @brief Gets the index-th ClassMember .
     * @param[in] index the ordinal number of the member.
     * @return the ClassMember description.
     */
    ClassMember const *     FindMember(uint32 index) const;

    /**
     * @brief Registers a member that can be later retrieved with FindMethod.
     * @param[in] the member to register. The pointer will be freed by this class.
     */
    void                    AddMember(ClassMember * const member);

    /**
     * @brief How many members are registered?
     * @return the number of registered members
     */
    inline uint32 			NumberOfMembers() const;

    /**
     * @brief Destructor.
     */
    virtual                 ~ClassRegistryItem();

    /**
     * @brief Allows obtaining per-class singleton ClassRegistryItem
     * @details The function will allocate the memory to hold the record and also,
     * if T descends from Object, will allocate a specialized ObjectBuilderT and add
     * it to the record.
     * Installing a further specialized ObjectBuilderT will correctly override the default
     * as the default will be installed first during the access to the record.
     */
//    template <class T>
//    static ClassRegistryItem *Instance();

    template <class T>
    static ClassRegistryItem *Instance(typename enable_if<isSameOrBaseOf(Object,T), T>::type *x=NULL);

    template <class T>
    static ClassRegistryItem *Instance(typename enable_if<!isSameOrBaseOf(Object,T), T>::type *x=NULL);

private:

    /**
     * @brief Constructor.
     */
    ClassRegistryItem(CCString typeidNameIn,uint32 sizeOfClassIn,const ObjectBuilder * const builder=NULL_PTR(const ObjectBuilder *));

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
     * The name of the class.
     */
    DynamicCString            className;

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

    /**
     *
     */
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
ClassRegistryItem *ClassRegistryItem::Instance(typename enable_if<isSameOrBaseOf(Object,T), T>::type *x){

	// flag used to implement a spinlock
	static int32 flag = 0;
	// local static object are constructed in a thread safe matter. So avoid them
	static ClassRegistryItem *instance = NULL_PTR(ClassRegistryItem *);
	// this is built only if T descends from Object
    static ObjectBuilderT<T> *builder = NULL_PTR(ObjectBuilderT<T>*);

	//spinlock
	FastPollingMutexSem mux(flag);
	while (mux.FastTryLock() && (instance == NULL_PTR(ClassRegistryItem *))){
		if (instance == NULL_PTR(ClassRegistryItem *)){
			builder = new ObjectBuilderT<T> (false);
			instance = new ClassRegistryItem(typeid(T).name(),sizeof(T),builder);
		}
	}
    return instance;
}

template <class T>
ClassRegistryItem *ClassRegistryItem::Instance(typename enable_if<!isSameOrBaseOf(Object,T), T>::type *x){

	// flag used to implement a spinlock
	static int32 flag = 0;
	// local static object are constructed in a thread safe matter. So avoid them
	static ClassRegistryItem *instance = NULL_PTR(ClassRegistryItem *);

	//spinlock
	FastPollingMutexSem mux(flag);
	while (mux.FastTryLock() && (instance == NULL_PTR(ClassRegistryItem *))){
		if (instance == NULL_PTR(ClassRegistryItem *)){
			instance = new ClassRegistryItem(typeid(T).name(),sizeof(T),NULL);
		}
	}
    return instance;
}

uint32 ClassRegistryItem::NumberOfMembers() const{
	return classMembers.ListSize();
}

}
#endif /* L2OBJECTS_CLASSREGISTRYITEM2_H_ */
	
