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

#ifndef SOURCE_CORE_L1OBJECTS_OBJECT_H_
#define SOURCE_CORE_L1OBJECTS_OBJECT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include "StructuredData.h"
#include "Heap.h"
#include "Introspection.h"
#include "ClassProperties.h"
#include "ClassRegistryItem.h"

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
    virtual void GetClassPropertiesCopy(ClassProperties &destination) const;                                           \
    /*                                                                                                                 \
     * @brief Allocates a new instance of the class type in the provided heap. Note that this                          \
     * is automatically called by the compiler (given that we use placement new).                                      \
     * Note that the selected heap might be different for each type of class.                                          \
     * @param[in, out] destination the destination where to copy the class properties to.                              \
     */                                                                                                                \
    static void * operator new(const size_t size, Heap &heap);                                                         \
    /*                                                                                                                 \
     * @brief Delete the object.                                                                                       \
     * @details Will delegate the deleting of the object to the correct heap. Note that the delete function            \
     * cannot call non-static members and as a consequence the heap variable must have global                          \
     * scope in the unit file (but is not exported) (see CLASS_REGISTER).                                              \
     * @param p the pointer to the object to be deleted.                                                               \
     */                                                                                                                \
    static void operator delete(void *p);

/**
 * This macro has to be inserted in every unit file.
 * The definition of the  _## name ## ClassRegistryItem variable will
 * instantiate a new ClassRegistryItem for every unit file compiled in the application.
 * Upon instantiation each ClassRegistryItem will automatically add itself to the ClassRegistryDatabase.
 * When required the ClassRegistryItem will instantiate new objects by asking the ClassRegistryItem
 * for the build function related to the class it is representing.
 * Note that new and delete are static functions and as a consequence they cannot call member
 * functions in the class (or its derived classes). Because their implementation is specific to the final
 * class they had to be written as part of the macro as well.
 */
#define CLASS_REGISTER(name,ver)                                                                                       \
    /*                                                                                                                 \
     * The heap which is used to instantiate objects from this class type. Only one heap can be set                    \
     * pre class type                                                                                                  \
     */                                                                                                                \
    static Heap name ## Heap_;                                                                                         \
    /*                                                                                                                 \
     * Forward declaration of function which allows to build a new instance of the object                              \
     * e.g. Object *MyClassTypeBuildFn_(const Heap &h);                                                                \
     */                                                                                                                \
    Object * name ## BuildFn_(const Heap &h);                                                                          \
    /*                                                                                                                 \
     * Class properties of this class type. One instance per class type automatically instantiated at the start        \
     * of an application or loading of a loadable library.                                                             \
     * e.g. static ClassProperties MyClassTypeClassProperties_( "MyClassType" , "1.0");                                \
     */                                                                                                                \
    static ClassProperties name ## ClassProperties_( #name , ver);                                                     \
    /*                                                                                                                 \
     * Class registry item of this class type. One instance per class type automatically instantiated at the start     \
     * of an application or loading of a loadable library. It will automatically add the class type to the             \
     * ClassRegistryDatabase.                                                                                          \
     * e.g. static ClassRegistryItem MyClassTypeClassRegistryItem_( MyClassTypeClassProperties_, &MyClassTypeBuildFn_);\
     */                                                                                                                \
    static ClassRegistryItem name ## ClassRegistryItem_( name ## ClassProperties_, & name ## BuildFn_);                \
    /*                                                                                                                 \
     * @brief Function called when a new instance of this class type is to be instantiated in the provided heap.       \
     * @param[in] h the heap where the object will be instantiated.                                                    \
     * @return a new instance of the object from the class type.                                                       \
     * e.g. Object *MyClassTypeBuildFn_( const Heap &h);                                                               \
     */                                                                                                                \
    Object * name ## BuildFn_(const Heap &h){                                                                          \
        static bool heapAlreadySet = false;                                                                            \
        if (!heapAlreadySet) {                                                                                         \
            name ## Heap_= h;                                                                                          \
            heapAlreadySet = true;                                                                                     \
        }                                                                                                              \
        name *p = new (name ## Heap_) name ();                                                                         \
        return p;                                                                                                      \
    }                                                                                                                  \
    /*                                                                                                                 \
     * e.g. MyClassType *MyClassType::GetClassPropertiesCopy( ClassProperties &destination) const;                     \
     */                                                                                                                \
    void name::GetClassPropertiesCopy(ClassProperties &destination) const {                                            \
        const ClassProperties *properties = name ## ClassRegistryItem_.GetClassProperties();                           \
        destination = *properties;                                                                                     \
    }                                                                                                                  \
    /*                                                                                                                 \
     * e.g. void *MyClassType::operator new(const size_t size, Heap &heap);                                            \
     */                                                                                                                \
    void * name::operator new(const size_t size, Heap &heap) {                                                         \
        void *obj = heap.Malloc(static_cast<uint32>(size));                                                            \
        name ## ClassRegistryItem_.IncrementNumberOfInstances();                                                       \
        return obj;                                                                                                    \
    }                                                                                                                  \
    /*                                                                                                                 \
     * e.g. void *MyClassType::operator delete(void *p);                                                               \
     */                                                                                                                \
    void name::operator delete(void *p) {                                                                              \
        name ## Heap_.Free(p);                                                                                         \
        name ## ClassRegistryItem_.DecrementNumberOfInstances();                                                       \
    }
/*lint -restore */

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
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
class Object {
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
     * @param data the input initialisation data.
     * @return true if all the input \a data is valid and can be successfully assigned
     * to the Object member variables.
     */
    virtual bool Initialise(const StructuredData &data);

    /**
     * @brief Returns a copy to the object introspection properties.
     * @destination Copies the object introspection properties to the \a destination.
     */
    void GetIntrospectionCopy(Introspection &destination) const;

    /**
     * @brief Returns the number of references.
     * @return the number of references pointing to this object.
     */
    uint32 NumberOfReferences() const;

private:

    /**
     * @brief Decrements the number of references to this object.
     * @details Only accessible to the Reference class.
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
     * Disallow the usage of new.
     */
    static void *operator new(size_t size) throw ();

    /**
     * Object introspection properties.
     */
    Introspection introspection;

    /**
     * The number of references to this object.
     */
    volatile uint32 referenceCounter;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L1OBJECTS_OBJECT_H_ */

