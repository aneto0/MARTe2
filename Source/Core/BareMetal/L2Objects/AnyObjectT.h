/**
 * @file AnyObjectT.h
 * @brief Header file for class AnyObjectT
 * @date Sep 2, 2020
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef ANYOBJECTT_H_
#define ANYOBJECTT_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Object.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief Create an Object derivative embedding the class T
 * @details This class allows to associate a name and a reference to any class T.
 * T must have a simple constructor with no parameters
 */
template <typename T>
class AnyObjectT: public Object, public T {

public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor. NOOP.
     */
    AnyObjectT();

    /**
     * @brief Default destructor.
     * @details Calls CleanUp()
     */
    virtual ~AnyObjectT();

    /*
     * @brief The main interface provided by an AnyObjectS is the ability to provide its data via an AnyType.
     * @return a valid AnyType that describes the content of this object and allows read only access to its content
     */
    virtual void ToAnyType(AnyType &at);

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <class T>
AnyObjectT<T>::AnyObjectT():Object(),T(){
};

template <class T>
AnyObjectT<T>::~AnyObjectT(){
//    printf("*");
};

template <typename T>
void AnyObjectT<T>::ToAnyType(AnyType &at){
    T *p = this;
    at = AnyType(*p);
}



TEMPLATE_CLASS_REGISTER(AnyObjectT<T>,"1.0",typename  T)



}


#endif /* ANYOBJECTT_H_ */
