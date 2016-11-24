/**
 * @file ClassRegistryItemT.h
 * @brief Header file for class ClassRegistryItemT
 * @date 17/06/2016
 * @author Giuseppe Ferrò
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
 * @details This header file contains the declaration of the class ClassRegistryItemT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

//#ifndef CLASSREGISTRYITEMT_H_
#if 0
#define CLASSREGISTRYITEMT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ObjectBuilderT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class ClassRegistryItem;

/**
 * @brief Template version of ClassRegistryItem.
 * @tparam T The type of the class to register. It is expected that will have
 * a public attribute named classProperties of type ClassProperties.
 * @tparam AddDefaultObjectBuilder A boolean which states if a default object
 * must be created.
 */
template<typename T>
class ClassRegistryItemT{

public:
    /**
     * @brief Singleton access to the database.
     * @return a reference to the database.
     * @only used for Object derived classes
     */
    static inline ClassRegistryItem *Instance();

private:
    /**
     * @brief Constructor.
     */
    ClassRegistryItemT(CCString typeIdNameIn,uint32 sizeOfClassIn);
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


template<typename T>
ClassRegistryItem *ClassRegistryItemT<T>::Instance() {
    /**
     * static variable. not automatic! persistent across calls
     * will be initialised with a pointer to the only valid instance of this
     */
    static ClassRegistryItem *instance = NULL_PTR(ClassRegistryItem *);

    /// first time will go inside here
    if (instance == NULL_PTR(ClassRegistryItem *)) {

        /// all common code here
        instance = ClassRegistryItem::CreateRegisterAndInitialiseInstance(T::classProperties);

    }
    return instance;
}

template<typename T>
ClassRegistryItem *ClassRegistryItemT<T>::GenericInstance(ClassProperties &classProperties_in) {
    /**
     * static variable. not automatic! persistent across calls
     * will be initialised with a pointer to the only valid instance of this
     */
    static ClassRegistryItem *instance = NULL_PTR(ClassRegistryItem *);

    /// first time will go inside here
    if (instance == NULL_PTR(ClassRegistryItem *)) {

        /// all common code here
        instance = ClassRegistryItem::CreateRegisterAndInitialiseInstance(T::classProperties);

    }
    return instance;
}


template<typename T>
ClassRegistryItemT<T>::~ClassRegistryItemT() {

}

#if 0
template<typename T, bool AddDefaultObjectBuilder>
ClassRegistryItem *ClassRegistryItemT<T, AddDefaultObjectBuilder>::Instance() {
    static ClassRegistryItem *instance = NULL_PTR(ClassRegistryItem *);
    if (instance == NULL_PTR(ClassRegistryItem *)) {
        ClassRegistryItem::Instance(instance, T::classProperties);
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

template<typename T>
ClassRegistryItem *ClassRegistryItemT<T, false>::Instance() {
    static ClassRegistryItem *instance = NULL_PTR(ClassRegistryItem *);
    if (instance == NULL_PTR(ClassRegistryItem *)) {
        ClassRegistryItem::Instance(instance, T::classProperties);
        /* The next line, i.e. the instantiation of a static ObjectBuilderT
         * is necessary to check if only the introspection has been registered
         * for a class that could not inherit from Object.
         */
        static ObjectBuilder invalidObjectBuilder;
        instance->SetObjectBuilder(&invalidObjectBuilder);
    }
    return instance;
}

template<typename T, bool AddDefaultObjectBuilder>
ClassRegistryItemT<T, AddDefaultObjectBuilder>::~ClassRegistryItemT() {

}

template<typename T, bool AddDefaultObjectBuilder>
ClassRegistryItemT<T, AddDefaultObjectBuilder>::ClassRegistryItemT(ClassProperties &classProperties_in) :
        ClassRegistryItem(classProperties_in) {
}

#endif

}

#endif /* CLASSREGISTRYITEMT_H_ */

