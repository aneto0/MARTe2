/**
 * @file AnyObjectS.h
 * @brief Header file for class AnyObjectS
 * @date 30/10/2017
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

 * @details This header file contains the declaration of the class AnyObjectS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */


#ifndef ANYOBJECT_S_H_
#define ANYOBJECT_S_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Object.h"
#include "MemoryOperators.h"
#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief An Object derivative either referring to or containing a generic variable described by an AnyType .
 * @details This class allows to associate a name and a reference to an AnyType.
 * It may holds and manage a memory space with a copy of the data of a generic variable or is a simply a reference to it
 */
template <unsigned int objectSize>
class AnyObjectS: public Object {

public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor. NOOP.
     */
	AnyObjectS();

    /**
     * @brief Default destructor.
     * @details Calls CleanUp()
     */
    virtual ~AnyObjectS();

    /**
     * @brief Allows to setup an Anytype of any type....
     * @param[in] pointer is the address of the memory to be copied (if NULL the memory will be left non-initialised)
     * @param[in] sizeToCopy is the amount of bytes to be copied
     * @param[in] descriptor is the variable descriptor to be used to describe this variable,
    */
	void Setup(uint32 sizeToCopy,const void *pointer,const VariableDescriptor &descriptor);

    /*
	 * @brief The main interface provided by an AnyObjectS is the ability to provide its data via an AnyType.
	 * @return a valid AnyType that describes the content of this object and allows read only access to its content
	 */
    virtual void ToAnyType(AnyType &at);

private:

    /**
     * @brief The Type and address of the data
     */
    VariableDescriptor vd;

    /**
     *
     */
    char8 data[objectSize];
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <unsigned int objectSize>
AnyObjectS<objectSize>::AnyObjectS():vd(InvalidType(0),""){
};

template <unsigned int objectSize>
AnyObjectS<objectSize>::~AnyObjectS(){

};

template <unsigned int objectSize>
void AnyObjectS<objectSize>::ToAnyType(AnyType &at){
	at = AnyType(vd,&data);
}

template <unsigned int objectSize>
void AnyObjectS<objectSize>::Setup(uint32 sizeToCopy,const void *pointer,const VariableDescriptor &descriptor){
	vd = descriptor;
	if (sizeToCopy > objectSize){
		sizeToCopy = objectSize;
	}
	if (pointer != NULL){
	    Memory::Copy(data,pointer,sizeToCopy);
	}
}


TEMPLATE_CLASS_REGISTER(AnyObjectS<objectSize>,"1.0",unsigned int objectSize)


}

#endif /* ANYOBJECT_H_ */
