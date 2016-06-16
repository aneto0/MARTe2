/**
 * @file ClassMethodInterfaceMapper.h
 * @brief Header file for class ClassMethodInterfaceMapper
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

 * @details This header file contains the declaration of the class ClassMethodInterfaceMapper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSMETHODINTERFACEMAPPER_H_
#define CLASSMETHODINTERFACEMAPPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorType.h"
#include "ClassMethodCaller.h"
#include "ClassMethodCallerT.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class Object;

class ClassMethodInterfaceMapper {

public:

    /**
     * TODO
     * */
    ClassMethodInterfaceMapper();

    template<class C, typename T>
    ClassMethodInterfaceMapper(bool (C::*f)(T ref));

/*
    template<class C>
    ClassMethodInterfaceMapper(bool (C::*f)());
*/

/*
 * Can I add this??
 * template<class C, typename T>
    ClassMethodInterfaceMapper(bool (C::*f)(void));
*/

    template<typename T>
    ErrorManagement::ErrorType Call(Object *context,
                                    T ref);


    ErrorManagement::ErrorType Call(Object *context);
    /**
     * TODO
     * */
    virtual ~ClassMethodInterfaceMapper();
private:
    ClassMethodCaller *caller;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<class className, typename argType>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(bool (className::*f)(argType ref)) {
    caller = new ClassMethodCallerT<className, argType>(f);
}

/*
template<class className>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(bool (className::*f)(void)) {
    caller = new ClassMethodCallerT<className, void>(f);
}
*/


template<typename argType>
ErrorManagement::ErrorType ClassMethodInterfaceMapper::Call(Object *context,
                                                            argType ref) {
    ErrorManagement::ErrorType ret;
    ret.unsupportedFeature = true;
    if (caller != NULL ) ret = caller->Call(context,ref);
    return ret;
}

/*
ErrorManagement::ErrorType ClassMethodInterfaceMapper::Call(Object *context) {
    ErrorManagement::ErrorType ret;
    ret.unsupportedFeature = true;
    if (caller != NULL ) ret = caller->Call(context);
    return ret;
}
*/

}

#endif /* CLASSMETHODINTERFACEMAPPER_H_ */

