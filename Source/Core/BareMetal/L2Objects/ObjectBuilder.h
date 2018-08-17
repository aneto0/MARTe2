/**
 * @file ObjectBuilder.h
 * @brief Header file for class ObjectBuilder
 * @date 11/04/2016
 * @author Filippo Sartori
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
 * @details This header file contains the declaration of the class ObjectBuilder
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OBJECTBUILDER_H_
#define OBJECTBUILDER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "HeapI.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/*lint -e{9109} forward declaration of this class is required in other modules*/
class Object;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief This class represents an abstract object builder.
 *
 * @details An object builder is an object whose purpose is to build another
 * object. In order to build an object, an object builder needs:
 * + A HeapI object which will allocate the memory for the object to be built.
 * + An overloaded new operator for creating a new instance for a specific
 * class using the HeapI object.
 *
 * @remark This class must be considered as a pure abstract class, i.e. an
 * interface, although it does not declare any of its methods as pure virtual.
 * Instead, it implements the Build method forcing it to return a default
 * value (NULL). The reason to this is that derived classes are expected to
 * implement the Build method, while the abstract class can be used as an
 * invalid object builder (useful for setting a default builder).
 */
/*lint -e{9109} forward declaration of this class is required in other modules*/
class DLL_API ObjectBuilder {
public:

    /**
     * @brief Default constructor
     */
    ObjectBuilder();

    /**
     * @brief Destructor.
     */
    virtual ~ObjectBuilder();

    /**
     * @brief invalid object builder function.
     * @param[in] heap is the heap where the memory for the new instance must
     * be allocated.
     * @return a NULL pointer to MARTe::Object.
     */
    virtual Object *Build(HeapI* const heap) const;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

inline ObjectBuilder::ObjectBuilder() {
}

inline ObjectBuilder::~ObjectBuilder() {
}

inline Object *ObjectBuilder::Build(HeapI* const heap) const {
    return NULL_PTR(Object *);
}

}

#endif /* OBJECTBUILDER_H_ */
