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
     */
    template <class T>
    ClassMember(T & member,CCString nameIn);

    /**
     * @brief name of the method
     */
    CCString GetName();

    /**
     * @brief returns associated descriptor
     */
    VariableDescriptor *GetDescriptor();


private:
    /**
     * @brief The description of the member
     */
    VariableDescriptor vd;

    /**
     * @brief The name of the member
     */
    CCString name;
};




/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


template <class T>
ClassMember::ClassMember(T & member,CCString nameIn): vd(member){
    ClassRegistryItem * cri = ClassRegistryItem::Instance<T>();
    if (cri != NULL_PTR(ClassRegistryItem * )){
        cri->AddMember(this);
    }
    name = nameIn;
}

CCString ClassMember::GetName(){
    return name;
}

VariableDescriptor *ClassMember::GetDescriptor(){
    return &vd;
}


}
#endif /* L2OBJECTS_CLASSMEMBER_H_ */
	
