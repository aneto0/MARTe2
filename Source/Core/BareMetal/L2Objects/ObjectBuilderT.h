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
 *
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

#include "HeapI.h"
#include "ObjectBuilder.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief This class template is a helper for building specific versions of
 * object builders for specific classes (the class specified as a class
 * template parameter).
 * @brief Template of ObjectBuilder implementing a default memory allocation for
 * the new instance of a MARTe::Object descendant.
 * @tparam T is the typename of a class descendant of MARTe::Object, which has
 * been registered as a class into ClassRegistryDatabase, and that provides a
 * new operator compatible for working with HeapI as memory manager.
 */
template<typename T>
class ObjectBuilderT: public ObjectBuilder {
public:

    /**
     * @brief Default constructor.
     * @details Registers itself into the ClassRegistryItem of the type T.
     */
    ObjectBuilderT();

    /**
     * @brief Destructor.
     */
    virtual ~ObjectBuilderT();

    /**
     * @brief Creates a new instance of type T, which inherits from
     * MARTe::Object and whose memory is managed by a HeapI object.
     * @param[in] heap is the heap where the memory for the new
     * instance will be allocated.
     * @return a pointer to the new created instance.
     */
    virtual Object *Build(HeapI* const heap) const;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

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
