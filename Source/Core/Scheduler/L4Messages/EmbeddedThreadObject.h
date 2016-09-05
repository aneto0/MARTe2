/**
 * @file EmbeddedThreadObject.h
 * @brief Header file for class EmbeddedThreadObject
 * @date Sep 5, 2016
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

 * @details This header file contains the declaration of the class EmbeddedThreadObject
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4MESSAGES_EMBEDDEDTHREADOBJECT_H_
#define L4MESSAGES_EMBEDDEDTHREADOBJECT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EmbeddedThread.h"

namespace MARTe {
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class EmbeddedThreadObject: public EmbeddedThread{

public:

    CLASS_REGISTER_DECLARATION()


    /**
     * TODO
     */
    EmbeddedThreadObject(MethodBinderI &binder);

    /**
     * TODO
     */
    template <typename className>
    EmbeddedThreadObject(MethodBinderT<className> &binder);

    /**
     * TODO
     */
    virtual ~EmbeddedThreadObject();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


template <typename className>
EmbeddedThreadObject::EmbeddedThreadObject(MethodBinderT<className> &binder): EmbeddedThread(binder) {}

}

#endif /* L4MESSAGES_EMBEDDEDTHREADOBJECT_H_ */
	
