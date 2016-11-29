/**
 * @file ClassMethodCaller.cpp
 * @brief Source file for class ClassMethodCaller
 * @date 13/06/2016
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
 * @details This source file contains the definition of all the methods for
 * the class ClassMethodCaller (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Object.h"
#include "ClassMethodCaller.h"
#include "ErrorManagement.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ClassMethodCaller::ClassMethodCaller() {

}

ClassMethodCaller::~ClassMethodCaller() {
}

/*lint -e{715} -e{952} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12] [MISRA C++ Rule 7-1-1]. This function is a default implementation which does nothing*/
ErrorManagement::ErrorType ClassMethodCaller::Call(Object *object, StreamI &stream){
    return ErrorManagement::ParametersError;
}

/*lint -e{715} -e{952} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12] [MISRA C++ Rule 7-1-1]. This function is a default implementation which does nothing*/
ErrorManagement::ErrorType ClassMethodCaller::Call(Object *object, StructuredDataI &parameters){
    return ErrorManagement::ParametersError;
}

///*lint -e{715} -e{952} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12] [MISRA C++ Rule 7-1-1]. This function is a default implementation which does nothing*/
//ErrorManagement::ErrorType ClassMethodCaller::Call(Object *object, ReferenceContainer &parameters){
//    return ErrorManagement::ParametersError;
//}


/*lint -e{715} -e{952} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12] [MISRA C++ Rule 7-1-1]. This function is a default implementation which does nothing*/
ErrorManagement::ErrorType ClassMethodCaller::Call(Object *object){
    return ErrorManagement::ParametersError;
}



ErrorManagement::ErrorType ClassMethodCaller::Parameters2ReferenceContainerCall(Object *object, StructuredDataI &parameters) {

    ReferenceContainer param1;

    Object * o = dynamic_cast<Object *>(&parameters);

    Reference ref(o);

    ErrorManagement::ErrorType err;
    if (ref.IsValid()) {
        param1.Insert(ref);
        err = Call(object, param1);
    }
    else {
        err = ErrorManagement::ParametersError;
    }

    return err;
}

ErrorManagement::ErrorType ClassMethodCaller::ReferenceContainer2ParametersCall(Object *object, ReferenceContainer &parameters) {
    ErrorManagement::ErrorType err(true);

    ReferenceT<StructuredDataI> sI = parameters.Get(0);

    err.parametersError = !sI.IsValid();
    if (err.ErrorsCleared()) {
        err = Call(object, *(sI.operator->()));
    }

    return err;
}

StreamI* ClassMethodCaller::StreamIFromReferenceContainer(ReferenceContainer &parameters){

    ReferenceT<StreamI> param = parameters.Get(0);

    return param.operator->();
}

}

