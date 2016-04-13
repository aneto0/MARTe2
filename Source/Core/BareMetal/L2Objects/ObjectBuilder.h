/**
 * @file ObjectBuilder.h
 * @brief Header file for class ObjectBuilder
 * @date Apr 11, 2016
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
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class Object;

/**
 * TODO
 * */
class ObjectBuilder{
public:

    /**
     * TODO
     * */
    virtual Object *Build(HeapI* const heap) const = 0;

    /**
     * TODO
     * */
    virtual ~ObjectBuilder(){}
};

/**
 * TODO
 * */
template <class T>
class ObjectBuilderT: public ObjectBuilder{

public:
    /**
     * TODO
     * */
    Object *Build(HeapI* const heap) const {
        T *p = new (heap) T ();
        return p;
    }

    /**
     * TODO
     * */
    virtual ~ObjectBuilderT(){}

    /**
     * TODO
     * */
    ObjectBuilderT(){
        T::GetClassRegistryItem->SetObjectBuilder(this);
    }
};


}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/**
 * TODO
 */
#define ADD_OBJECTBUILDER(className) \
    static MARTe::ObjectBuilderT<className> className ## _ ## objectBuilder();


#endif /* OBJECTBUILDER_H_ */
	
