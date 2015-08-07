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
 * These macros are required to automatically register in the ClassRegistryDatabase
 * the information about all the classes that inherit from object.
 * These have to be defined as macros so that the new and delete functions are implemented
 * in the final class inhering from Object (i.e. they are specific to each implementation).
 */
/**
 * The function GetHiddenClassRegistryItem has to be virtual in order to guarantee that
 * there will be a virtual member in all inherited classes.
 * This macro has to be inserted in every class that inherits from Object.
 */
#define CLASS_REGISTER_DECLARATION()                                                          \
    virtual ClassRegistryItem *GetHiddenClassRegistryItem() const;                            \
    void GetClassPropertiesCopy(ClassProperties &destination) const;                          \
    void * operator new(size_t size, Heap &heap);                                             \
    void operator delete(void *p);

/**
 * This macro has to be inserted in every unit file.
 * The definition of the  _## name ## ClassRegistryItem variable will
 * instantiate a new ClassRegistryItem for every unit file compiled in the application.
 * Upon instantiation each ClassRegistryItem will automatically add itself to the ClassRegistryDatabase.
 * When required the ClassRegistryItem will instantiate new objects by asking the ClassRegistryItem
 * for the build function related to the class it is representing.
 * Note that new and delete are static functions and as a consequence they cannot call member
 * functions in the class (or its derived classes). Because their implementation is specific to the final
 * class they had to be written as part of macro as well.
 */
#define CLASS_REGISTER(name,ver)                                                              \
    Object * _ ## name ## BuildFn (Heap &heap);                                               \
    static ClassProperties _## name ## ClassProperties( #name ,ver);                          \
    static ClassRegistryItem _ ## name ## ClassRegistryItem( _ ## name ## ClassProperties, & _ ##  name ## BuildFn );  \
    Object * _ ## name ## BuildFn (Heap &heap){                                               \
        _ ## name ## ClassRegistryItem.SetHeap(heap);                                         \
        name *p = new (heap) name () ;                                                        \
        return p;                                                                             \
    }                                                                                         \
    ClassRegistryItem * name::GetHiddenClassRegistryItem() const {                            \
        return &_## name ## ClassRegistryItem;                                                \
    }                                                                                         \
    void name::GetClassPropertiesCopy(ClassProperties &destination) const {                   \
        destination = *GetHiddenClassRegistryItem()->GetClassProperties();                    \
    }                                                                                         \
    void * name::operator new(size_t size, Heap &heap) {                                      \
        void *obj = heap.Malloc(size);                                                        \
        _ ## name ## ClassRegistryItem.IncrementNumberOfInstances();                          \
        return obj;                                                                           \
    }                                                                                         \
    void name::operator delete(void *p) {                                                     \
        _ ## name ## ClassRegistryItem.GetHeap()->Free(p);                                    \
        _ ## name ## ClassRegistryItem.DecrementNumberOfInstances();                          \
    }


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief The root object of the framework.
 *
 * @details The MARTe Object is a class which offers the following functionality:
 *  - Its life cycle is managed by a smart pointer mechanism (@see Reference);
 *  - Can be automatically constructed in runtime by the class name of its derived class;
 *  - Provides a standard initialisation/construction interface;
 *  - Is introspectable and enables reflection of derived classes;
 *  - The allocation heap can be selected by the end-user.
 */
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
     * @return true if all the input data is valid and can be successfully assigned
     * to the member variables.
     */
    virtual bool Initialise(const StructuredData &data);

    /**
     * @brief Returns a copy to this object instance introspection properties.
     * @destination copies this object instance introspection properties to destination.
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
     * @return the new number of references.
     */
    uint32 DecrementReferences();

    /**
     * @brief Increments the number of references to this object.
     * @details Only accessible to the Reference class.
     * @return the new number of references.
     */
    uint32 IncrementReferences();

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
    void *operator new(size_t size) throw ();

    /**
     * Object introspection properties.
     */
    Introspection introspection;

    /**
     * The number of references to this object.
     */
    volatile int32 referenceCounter;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L1OBJECTS_OBJECT_H_ */

