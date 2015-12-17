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
#include "GeneralDefinitions.h"
#include "Introspection.h"
#include "ClassProperties.h"
#include "ClassRegistryItem.h"
#include "StructuredDataI.h"
#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                        Macro definitions                                  */
/*---------------------------------------------------------------------------*/
/**
 * These macros are required for the application to automatically register in the ClassRegistryDatabase
 * the information associated to every class that inherits from Object.
 * These have to be defined as macros given that the new and delete functions are specific for each
 * final class that inherits from Object (new and delete are static and thus cannot be virtual).
 */
/**
 * The function GetClassPropertiesCopy has to be virtual in order to guarantee that
 * there will be a virtual member in all inherited classes.
 * This macro has to be inserted in every class that inherits from Object.
 */
/*lint -save -e9026 -e9024 -e9023 -e9141 -estring(1960,"*6-2-1*")
 * 9026: function-like macro defined.
 * 9024: '#/##' operators used in macro.
 * 9023: (Multiple use of '#/##' operators in definition of macro) an exception to this rule.
 * is only applied in the definition of the ClassRegistryItem_.
 * 9141: Disable global declaration of symbols.
 * 1960: lint is confused with the placement new and reporting a false alarm.
 */
#define CLASS_REGISTER_DECLARATION()                                                                                   \
    /*                                                                                                                 \
     * @brief Returns a copy of the class properties associated with this class type.                                  \
     * @param[in, out] destination the destination where to copy the class properties to.                              \
     */                                                                                                                \
     virtual void GetClassPropertiesCopy(MARTe::ClassProperties &destination) const;                                           \
    /*                                                                                                                 \
     * @brief Returns the class properties associated with this class type.                                            \
     * @param[in, out] destination the destination where to copy the class properties to.                              \
     */                                                                                                                \
     virtual const MARTe::ClassProperties *GetClassProperties() const;                                                         \
    /*                                                                                                                 \
     * @brief Allocates a new instance of the class type in the provided heap. Note that this                          \
     * is automatically called by the compiler (given that we use placement new).                                      \
     * Note that the selected heap might be different for each type of class.                                          \
     * @param[in, out] destination the destination where to copy the class properties to.                              \
     */                                                                                                                \
     static void * operator new(const MARTe::osulong size, MARTe::HeapI *heap = static_cast<MARTe::HeapI *>(NULL));\
    /*                                                                                                                 \
     * @brief Delete the object.                                                                                       \
     * @details Will delegate the deleting of the object to the correct heap. Note that the delete function            \
     * cannot call non-static members and as a consequence the heap variable must have global                          \
     * scope in the unit file (but is not exported) (see CLASS_REGISTER).                                              \
     * @param[in] p the pointer to the object to be deleted.                                                           \
     */                                                                                                                \
      static void operator delete(void *p);

/**
 * This macro has to be inserted in every unit file.
 * The definition of the  _## className ## ClassRegistryItem variable will
 * instantiate a new ClassRegistryItem for every unit file compiled in the application.
 * Upon instantiation each ClassRegistryItem will automatically add itself to the ClassRegistryDatabase.
 * When required the ClassRegistryItem will instantiate new objects by asking the ClassRegistryItem
 * for the build function related to the class it is representing.
 * Note that new and delete are static functions and as a consequence they cannot call member
 * functions in the class (or its derived classes). Because their implementation is specific to the final
 * class they had to be written as part of the macro as well.
 */
#define CLASS_REGISTER(className,ver)                                                                                  \
    /*                                                                                                                 \
     * Forward declaration of function which allows to build a new instance of the object                              \
     * e.g. Object *MyClassTypeBuildFn_(const Heap &h);                                                                \
     */                                                                                                                \
    MARTe::Object * className ## BuildFn_(MARTe::HeapI* const heap);                                                    \
    /*                                                                                                                 \
     * Class properties of this class type. One instance per class type automatically instantiated at the start        \
     * of an application or loading of a loadable library.                                                             \
     * e.g. static ClassProperties MyClassTypeClassProperties_("MyClassType", typeid(MyClassType).name(), "1.0");      \
     */                                                                                                                \
    static MARTe::ClassProperties className ## ClassProperties_( #className , typeid(className).name(), ver);                 \
    /*                                                                                                                 \
     * Class registry item of this class type. One instance per class type automatically instantiated at the start     \
     * of an application or loading of a loadable library. It will automatically add the class type to the             \
     * ClassRegistryDatabase.                                                                                          \
     * e.g. static ClassRegistryItem MyClassTypeClassRegistryItem_( MyClassTypeClassProperties_, &MyClassTypeBuildFn_);\
     */                                                                                                                \
    static MARTe::ClassRegistryItem className ## ClassRegistryItem_( className ## ClassProperties_, & className ## BuildFn_); \
    /*                                                                                                                 \
     * @brief Function called when a new instance of this class type is to be instantiated in the provided heap.       \
     * @param[in] h the heap where the object will be instantiated.                                                    \
     * @return a new instance of the object from the class type.                                                       \
     * e.g. Object *MyClassTypeBuildFn_( const Heap &h);                                                               \
     */                                                                                                                \
    MARTe::Object * className ## BuildFn_(MARTe::HeapI* const heap){                                                    \
        className *p = new (heap) className ();                                                                        \
        return p;                                                                                                      \
    }                                                                                                                  \
    /*                                                                                                                 \
     * e.g. MyClassType *MyClassType::GetClassPropertiesCopy( ClassProperties &destination) const;                     \
     */                                                                                                                \
    void className::GetClassPropertiesCopy(MARTe::ClassProperties &destination) const {                                       \
        const MARTe::ClassProperties *properties = className ## ClassRegistryItem_.GetClassProperties();                      \
        destination = *properties;                                                                                     \
    }                                                                                                                  \
    /*                                                                                                                 \
     * e.g. MyClassType *MyClassType::GetClassPropertiesCopy( ClassProperties &destination) const;                     \
     */                                                                                                                \
    const MARTe::ClassProperties *className::GetClassProperties() const {                                                     \
        return className ## ClassRegistryItem_.GetClassProperties();                                                   \
    }                                                                                                                  \
    /*                                                                                                                 \
     * e.g. void *MyClassType::operator new(const size_t size, Heap &heap);                                            \
     */                                                                                                                \
    void * className::operator new(const size_t size, MARTe::HeapI* const heap) {                                             \
        void *obj = NULL_PTR(void *);                                                                                  \
        if (heap != NULL) {                                                                                            \
            obj = heap->Malloc(static_cast<uint32>(size));                                                             \
        } else {                                                                                                       \
            obj = MARTe::HeapManager::Malloc(static_cast<uint32>(size));                                                      \
        }                                                                                                              \
        className ## ClassRegistryItem_.IncrementNumberOfInstances();                                                  \
        return obj;                                                                                                    \
    }                                                                                                                  \
    /*                                                                                                                 \
     * e.g. void *MyClassType::operator delete(void *p);                                                               \
     */                                                                                                                \
    void className::operator delete(void *p) {                                                                         \
        bool ok = MARTe::HeapManager::Free(p);                                                                                \
        if(!ok){                                                                                                       \
            /* TODO error here */                                                                                      \
        }                                                                                                              \
        className ## ClassRegistryItem_.DecrementNumberOfInstances();                                                  \
    }
/*lint -restore */

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
/*lint -e{9109} Object is forward declared in ClassRegistryItem (in order to be able to have access the function pointer to
 * create new instances.*/
class DLL_API Object {
    /**
     * This allows the Reference class to be the only interface to manage the number of instances pointing to this object.
     */
    friend class Reference;
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor. Sets the number of references to zero.
     */
    Object();

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
    virtual bool Initialise(const StructuredDataI &data);

    /**
     * @brief Returns a copy to the object introspection properties.
     * @param[out] destination Copies the object introspection properties to the \a destination.
     */
    void GetIntrospectionCopy(Introspection &destination) const;

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
    static void *operator new(osulong size) throw ();

    /**
     * Object introspection properties.
     */
    Introspection introspection;

    /**
     * The number of references to this object.
     */
    volatile uint32 referenceCounter;

    /**
     * The name of this object.
     */
    char8 *name;
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OBJECT_H_ */

