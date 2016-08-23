/**
 * @file GenericVoidMethodCallerT.h
 * @brief Header file for class GenericVoidMethodCallerT
 * @date Aug 23, 2016
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

 * @details This header file contains the declaration of the class GenericVoidMethodCallerT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L2MIXED_GENERICVOIDMETHODCALLERT_H_
#define L2MIXED_GENERICVOIDMETHODCALLERT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GenericVoidMethodCallerI.h"

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * TODO
 */
template <typename className>
class GenericVoidMethodCallerT: public GenericVoidMethodCallerI{

public:

    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef bool (className::*MethodPointer)(void);

    /**
     * TODO
     */
    GenericVoidMethodCallerT(className &o, MethodPointer f);

    /**
     * TODO
     */
    virtual ~GenericVoidMethodCallerT();

    /**
     * TODO
     */
    virtual ErrorManagement::ErrorType Call(){
        return object.function();
    }

private:

    /**
     * TODO
     */
    className &object;

    /**
     * TODO
     */
    MethodPointer function;
};




/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
template <typename className>
GenericVoidMethodCallerT<className>::GenericVoidMethodCallerT (className &o, MethodPointer f):object(o),function(f){
}

/**
 * TODO
 */
template <typename className>
virtual GenericVoidMethodCallerT<className>::~GenericVoidMethodCallerT(){
}

template <typename className>
virtual ErrorManagement::ErrorType GenericVoidMethodCallerT<className>::Call(){
    return object.function();
}



}

#endif /* L2MIXED_GENERICVOIDMETHODCALLERT_H_ */
	
