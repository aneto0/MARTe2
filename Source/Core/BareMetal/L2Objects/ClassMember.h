/**
 * @file ClassMember.h
 * @brief Header file for class ClassMember
 * @date Nov 28, 2016
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

 * @details This header file contains the declaration of the class ClassMember
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L2OBJECTS_CLASSMEMBER_H_
#define L2OBJECTS_CLASSMEMBER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "LinkedListable.h"
#include "VariableDescriptor.h"
#include "ClassRegistryItem.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Used to describe a member of a class for introspection purposes
 */
class ClassMember: public LinkedListable{

public:
    /**
     * @brief templated constructor
     * offset is the offset of the member with respect to the start of the class memory
     */
    template <class T>
    ClassMember(T member,CCString nameIn,uint32 offsetIn);

    /**
     * @brief name of the method
     */
    inline CCString GetName() const;

    /**
     * @brief offset of the variable
     */
    inline uint32 GetOffset() const;

    /**
     * @brief returns associated descriptor
     */
    inline const VariableDescriptor &GetDescriptor() const;


private:
    /**
     * @brief The description of the member
     */
    VariableDescriptor vd;

    /**
     * @brief The name of the member
     */
    CCString name;

    /**
     *
     */
    uint32 offset;
};




/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <class T>
ClassMember::ClassMember(T member,CCString nameIn,uint32 offsetIn): vd(member){
    name = nameIn;
    offset = offsetIn;
}

CCString ClassMember::GetName() const{
    return name;
}

uint32 ClassMember::GetOffset() const{
    return offset;
}

const VariableDescriptor &ClassMember::GetDescriptor() const{
    return vd;
}


}
#endif /* L2OBJECTS_CLASSMEMBER_H_ */
	
