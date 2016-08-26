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
#include "ReferenceContainer.h"
#include "ReferenceT.h"


namespace MARTe{

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class CallerClass{
public:
    virtual ~CallerClass(){}


    virtual ErrorManagement::ErrorType caller(Object *object, StreamI *stream){
        return ErrorManagement::parametersError;
    }

    virtual ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){
        return ErrorManagement::parametersError;
    }

    virtual ErrorManagement::ErrorType caller(Object *object, ReferenceContainer &parameters){
        return ErrorManagement::parametersError;
    }

    virtual ErrorManagement::ErrorType caller(Object *object){
        return ErrorManagement::parametersError;
    }


};

template <class T, typename argType1,typename argType2,typename argType3,typename argType4>
class CallerClassT: public CallerClass{
public:

    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef ErrorManagement::ErrorType (T::*MethodPointer)(argType1,argType2,argType3,argType4);

    virtual ~CallerClassT(){}

    CallerClassT(MethodPointer method){
        pfunc = method;
    }

    virtual ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){
        T* actual = dynamic_cast<T *>(object);
        argType1 param1;
        argType2 param2;
        argType3 param3;
        argType4 param4;
        parameters->Read("param1",param1);
        parameters->Read("param2",param2);
        parameters->Read("param3",param3);
        parameters->Read("param4",param4);
        return (actual->*pfunc)(param1,param2,param3,param4);
    }

    virtual ErrorManagement::ErrorType caller(Object *object, ReferenceContainer &parameters){

        Reference par = parameters.Get(0);

        ReferenceT<StructuredDataI> param = par;
        return caller (object, param.operator->());
    }

    //TODO create class to generate a StructuredDataI from a StreamI
    //TODO then call the caller(StructuredDataI)

private:
    MethodPointer pfunc;


};


template <class T, typename argType1,typename argType2,typename argType3>
class CallerClassT<T,argType1,argType2,argType3,void>:public CallerClass{
public:

    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef ErrorManagement::ErrorType (T::*MethodPointer)(argType1,argType2,argType3);


    CallerClassT(MethodPointer method){
        pfunc = method;
    }
    virtual ~CallerClassT(){}

    ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){
        T* actual = dynamic_cast<T *>(object);
        argType1 param1;
        argType2 param2;
        argType3 param3;
        parameters->Read("param1",param1);
        parameters->Read("param2",param2);
        parameters->Read("param3",param3);
        return (actual->*pfunc)(param1,param2,param3);
    }

    virtual ErrorManagement::ErrorType caller(Object *object, ReferenceContainer &parameters){

        Reference par = parameters.Get(0);

        ReferenceT<StructuredDataI> param = par;
        return caller (object, param.operator->());
    }

    //TODO create class to generate a StructuredDataI from a StreamI
    //TODO then call the caller(StructuredDataI)

private:

    MethodPointer pfunc;

};



template <class T, typename argType1,typename argType2>
class CallerClassT<T,argType1,argType2,void,void>:public CallerClass{
public:

    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef ErrorManagement::ErrorType (T::*MethodPointer)(argType1,argType2);


    CallerClassT(MethodPointer method){
        pfunc = method;
    }
    virtual ~CallerClassT(){}

    ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){
        T* actual = dynamic_cast<T *>(object);
        argType1 param1;
        argType2 param2;
        parameters->Read("param1",param1);
        parameters->Read("param2",param2);
        return (actual->*pfunc)(param1,param2);
    }

    virtual ErrorManagement::ErrorType caller(Object *object, ReferenceContainer &parameters){

        Reference par = parameters.Get(0);

        ReferenceT<StructuredDataI> param = par;
        return caller (object, param.operator->());
    }

    //TODO create class to generate a StructuredDataI from a StreamI
    //TODO then call the caller(StructuredDataI)

private:

    MethodPointer pfunc;
};


template <class T, typename argType1>
class CallerClassT<T,argType1,void,void,void>:public CallerClass{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef ErrorManagement::ErrorType (T::*MethodPointer)(argType1);


    CallerClassT(MethodPointer method){
        pfunc = method;
    }
    virtual ~CallerClassT(){}

    ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){
        T* actual = dynamic_cast<T *>(object);
        argType1 param1;
        parameters->Read("param1",param1);
        return (actual->*pfunc)(param1);
    }

    virtual ErrorManagement::ErrorType caller(Object *object, ReferenceContainer &parameters){

        Reference par = parameters.Get(0);

        ReferenceT<StructuredDataI> param = par;
        return caller (object, param.operator->());
    }

    //TODO create class to generate a StructuredDataI from a StreamI
    //TODO then call the caller(StructuredDataI)

private:

    MethodPointer pfunc;
};

template <class T>
class CallerClassT<T,void,void,void,void>:public CallerClass{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef ErrorManagement::ErrorType (T::*MethodPointer)();

private:

    MethodPointer pfunc;
public:

    CallerClassT(MethodPointer method){
        pfunc = method;
    }
    virtual ~CallerClassT(){}


    virtual ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){
        return caller(object);
    }

    virtual ErrorManagement::ErrorType caller(Object *object, ReferenceContainer &parameters){
        return caller(object);
    }

    virtual ErrorManagement::ErrorType caller(Object *object, StreamI *stream){
        return caller(object);
    }

    virtual ErrorManagement::ErrorType caller(Object *object){
        T* actual = dynamic_cast<T *>(object);
        return (actual->*pfunc)();
    }

};

template <class T>
class CallerClassT<T,StructuredDataI *,void,void,void>:public CallerClass{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef ErrorManagement::ErrorType (T::*MethodPointer)(StructuredDataI *parameters);

    CallerClassT(MethodPointer method){
        pfunc = method;
    }

    ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){
        T* actual = dynamic_cast<T *>(object);
        return (actual->*pfunc)(parameters);
    }

    virtual ErrorManagement::ErrorType caller(Object *object, ReferenceContainer &parameters){
        ReferenceT<StructuredDataI> sI = parameters.Get(0);
        return caller(object,sI.operator->());
    }

    //TODO create class to generate a StructuredDataI from a StreamI
    //TODO then call the caller(StructuredDataI)

private:

    MethodPointer pfunc;
};


template <class T>
class CallerClassT<T,ReferenceContainer &,void,void,void>:public CallerClass{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef ErrorManagement::ErrorType (T::*MethodPointer)(ReferenceContainer &parameters);

    CallerClassT(MethodPointer method){
        pfunc = method;
    }
    virtual ~CallerClassT(){}

    virtual ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){

        ReferenceContainer param1;

        Object * o = dynamic_cast<Object *>(parameters);

        Reference ref(o);

        param1.Insert(ref);

        return caller (object ,param1);
    }

    virtual ErrorManagement::ErrorType caller(Object *object, ReferenceContainer &parameters){
        T* actual = dynamic_cast<T *>(object);
        return (actual->*pfunc)(parameters);
    }

private:

    MethodPointer pfunc;

};

template <class T>
class CallerClassT<T,StreamI *,void,void,void>:public CallerClass{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef ErrorManagement::ErrorType (T::*MethodPointer)(StreamI *);

    CallerClassT(MethodPointer method){
        pfunc = method;
    }
    virtual ~CallerClassT(){}

    virtual ErrorManagement::ErrorType caller(Object *object, StructuredDataI *parameters){
        return ErrorManagement::parametersError;
    }

    ErrorManagement::ErrorType caller(Object *object, ReferenceContainer &parameters){
        T* actual = dynamic_cast<T *>(object);

        Reference par = parameters.Get(0);

        ReferenceT<StreamI> param = par;

        return (actual->*pfunc)(param.operator->());
    }
private:

    MethodPointer pfunc;
};


template <class T>
CallerClass *CallerClassCreate(ErrorManagement::ErrorType (T::*MethodPointer)()){
    CallerClass *p = new CallerClassT<T,void,void,void,void>(MethodPointer);
    return p;
}

template <class T, typename argType1>
CallerClass *CallerClassCreate(ErrorManagement::ErrorType (T::*MethodPointer)(argType1)){
        return new CallerClassT<T,argType1,void,void,void>(MethodPointer);
}

template <class T, typename argType1,typename argType2>
CallerClass *CallerClassCreate(ErrorManagement::ErrorType (T::*MethodPointer)(argType1,argType2)){
        return new CallerClassT<T,argType1,argType2,void,void>(MethodPointer);
}

template <class T, typename argType1,typename argType2,typename argType3>
CallerClass *CallerClassCreate(ErrorManagement::ErrorType (T::*MethodPointer)(argType1,argType2,argType3)){
        return new CallerClassT<T,argType1,argType2,argType3,void>(MethodPointer);
}

template <class T, typename argType1,typename argType2,typename argType3,typename argType4>
CallerClass *CallerClassCreate(ErrorManagement::ErrorType (T::*MethodPointer)(argType1,argType2,argType3,argType4)){
        return new CallerClassT<T,argType1,argType2,argType3,argType4>(MethodPointer);
}



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}
#endif /* L2OBJECTS_CLASSMETHODCALLERFT_H_ */
	
