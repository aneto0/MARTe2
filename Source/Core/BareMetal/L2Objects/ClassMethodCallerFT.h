/**
 * @file ClassMethodCallerFT.h
 * @brief Header file for class ClassMethodCallerFT
 * @date Aug 25, 2016
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

 * @details This header file contains the declaration of the class ClassMethodCallerFT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L2OBJECTS_CLASSMETHODCALLERFT_H_
#define L2OBJECTS_CLASSMETHODCALLERFT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Object.h"
#include "ErrorType.h"
#include "StructuredDataI.h"
#include "AnyObject.h"

namespace MARTe{

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

template <class T, typename arg1,typename arg2,typename arg3,typename arg4>
class callerClassT{
    virtual ~callerClassT(){}
public:


    virtual ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){
        return ErrorManagement::parametersError;
    }

    virtual ErrorManagement::ErrorType caller(Object *object, ReferenceContainer &parameters){
        T* actual = dynamic_cast<T *>(object);

        Reference par = parameters.Get(0);

        ReferenceT<StructuredDataI> param = par;
        return caller (object, param.operator->());
    }

    virtual ErrorManagement::ErrorType caller(Object *object){
        return ErrorManagement::parametersError;
    }

};



template <class T, typename arg1,typename arg2,typename arg3,typename arg4>
class callerClassT<T,arg1,arg2,arg3,arg4>{
    ErrorManagement::ErrorType( T::*pfunc)(arg1 param1,arg2 param2,arg3 param3,arg4 param4);
public:

    callerClassT(ErrorManagement::ErrorType( T::*func)(arg1 param1,arg2 param2,arg3 param3,arg4 param4)){
        pfunc = func;
    }

    virtual ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){
        T* actual = dynamic_cast<T *>(object);
        arg1 param1;
        arg2 param2;
        arg3 param3;
        arg4 param4;
        parameters->Read("param1",AnyType(arg1));
        parameters->Read("param2",AnyType(arg2));
        parameters->Read("param3",AnyType(arg3));
        parameters->Read("param4",AnyType(arg4));
        return actual->pfunc(param1,param2,param3,param4);
    }

};

template <class T, typename arg1,typename arg2,typename arg3>
class callerClassT<T,arg1,arg2,arg3,void>{
    ErrorManagement::ErrorType( T::*pfunc)(arg1 param1,arg2 param2,arg3 param3);
public:

    callerClassT(ErrorManagement::ErrorType( T::*func)(arg1 param1,arg2 param2,arg3 param3)){
        pfunc = func;
    }

    ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){
        T* actual = dynamic_cast<T *>(object);
        arg1 param1;
        arg2 param2;
        arg3 param3;
        parameters->Read("param1",arg1);
        parameters->Read("param2",arg2);
        parameters->Read("param3",arg3);
        return actual->pfunc(param1,param2,param3);
    }
};



template <class T, typename arg1,typename arg2>
class callerClassT<T,arg1,arg2,void,void>{
    ErrorManagement::ErrorType( T::*pfunc)(arg1 param1,arg2 param2);
public:

    callerClassT(ErrorManagement::ErrorType( T::*func)(arg1 param1,arg2 param2)){
        pfunc = func;
    }

    ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){
        T* actual = dynamic_cast<T *>(object);
        arg1 param1;
        arg2 param2;
        parameters->Read("param1",arg1);
        parameters->Read("param2",arg2);
        return actual->pfunc(param1,param2);
    }
};


template <class T, typename arg1>
class callerClassT<T,arg1,void,void,void>{
    ErrorManagement::ErrorType( T::*pfunc)(arg1 param1);
public:

    callerClassT(ErrorManagement::ErrorType( T::*func)(arg1 param1)){
        pfunc = func;
    }

    ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){
        T* actual = dynamic_cast<T *>(object);
        arg1 param1;
        parameters->Read("param1",arg1);
        return actual->pfunc(param1);
    }
};

template <class T>
class callerClassT<T,void,void,void,void>{
    ErrorManagement::ErrorType( T::*pfunc)(void);
public:

    callerClassT(ErrorManagement::ErrorType( T::*func)(void)){
        pfunc = func;
    }

    virtual ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){
        return caller(object);
    }

    virtual ErrorManagement::ErrorType caller(Object *object, ReferenceContainer &parameters){
        return caller(object);
    }

    virtual ErrorManagement::ErrorType caller(Object *object){
        T* actual = dynamic_cast<T *>(object);
        return actual->pfunc();
    }

};

template <class T>
class callerClassT<T,StructuredDataI &,void,void,void>{
    ErrorManagement::ErrorType( T::*pfunc)(StructuredDataI &parameters);
public:

    callerClassT(ErrorManagement::ErrorType( T::*func)(void)){
        pfunc = func;
    }

    ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){
        T* actual = dynamic_cast<T *>(object);
        return actual->pfunc(*parameters);
    }
};


template <class T>
class callerClassT<T,ReferenceContainer &,void,void,void>{
    ErrorManagement::ErrorType( T::*pfunc)(ReferenceContainer &parameters);
public:

    callerClassT(ErrorManagement::ErrorType( T::*func)(ReferenceContainer &parameters)){
        pfunc = func;
    }

    virtual ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){

        ReferenceContainer param1;

        Reference ao(new AnyObject (*parameters));
        param1.Insert(ao);

        return caller (object ,param1);
    }

    virtual ErrorManagement::ErrorType caller(Object *object, ReferenceContainer &parameters){
        T* actual = dynamic_cast<T *>(object);
        return actual->pfunc(parameters);
    }

};

template <class T>
class callerClassT<T,StreamI *,void,void,void>{
    ErrorManagement::ErrorType( T::*pfunc)(StreamI *parameters);
public:

    callerClassT(ErrorManagement::ErrorType( T::*func)(StreamI *)){
        pfunc = func;
    }

    ErrorManagement::ErrorType caller(Object *object, ReferenceContainer &parameters){
        T* actual = dynamic_cast<T *>(object);

        Reference par = parameters.Get(0);

        ReferenceT<StreamI> param = par;

        return actual->pfunc(param);
    }
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}
#endif /* L2OBJECTS_CLASSMETHODCALLERFT_H_ */
	
