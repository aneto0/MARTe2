/**
 * @file ObjectBuilderT.h
 * @brief Header file for class ObjectBuilderT
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

 * @details This header file contains the declaration of the class ObjectBuilderT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OBJECTBUILDERT_H_
#define OBJECTBUILDERT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ObjectBuilder.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Template of ObjectBuilder implementing a default memory allocation for
 * the new instance of a MARTe::Object descendant.
 * @tparam T is a descendant of MARTe::Object.
 */
template<typename T>
class ObjectBuilderT: public ObjectBuilder {

public:
    /**
     * @brief Constructor. Registers itself in the ClassRegistryItem if the registered class.
     */
    ObjectBuilderT();
    /**
     * @brief Destructor.
     */
    virtual ~ObjectBuilderT();
    /**
     * @brief Creates a new instance of the template type inheriting from MARTe::Object.
     * @param[in] heap is the heap where the memory for the new instance must be allocated.
     * @return a NULL pointer to the new created instance.
     */
    Object *Build(HeapI* const heap) const;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
ObjectBuilderT<T>::ObjectBuilderT() {
    T::GetClassRegistryItem_Static()->SetObjectBuilder(this);
}

template<typename T>
ObjectBuilderT<T>::~ObjectBuilderT() {
}

template<typename T>
Object *ObjectBuilderT<T>::Build(HeapI* const heap) const {
    T *p = new (heap) T();
    return p;
}

}

#endif /* OBJECTBUILDERT_H_ */

