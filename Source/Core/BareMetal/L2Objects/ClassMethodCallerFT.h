/**
 * @file XClassMethodCallerFT.h
 * @brief Header file for class XClassMethodCallerFT
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

 * @details This header file contains the declaration of the class XClassMethodCallerFT
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



class XClassMethodCaller{
public:
    virtual ~XClassMethodCaller(){}

    /**
     * @brief Calls the class method using a Stream as the source of the parameters
     * @param[in] object is the pointer to the object owning the method.
     * @param[in] parameters a reference to a Stream that will be used to fill the parameters to call the functions
     * @return
     * + ErrorManagement::parametersError if no match between parameters provided and the function call is possible
     * + ErrorManagement::unsupportedFeature if dynamic_cast to specialised class type is possible with provided argument object
     * + on success the error returned by the method called
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StreamI *stream){
        return ErrorManagement::parametersError;
    }

    /**
     * @brief Calls the class method by taking the arguments from StructuredDataI *parameters
     * @param[in] object is the pointer to the object owning the method.
     * @param[in] parameters a reference to a StructuredDataI object where to read/write parameters/results.
     * @return
     * + ErrorManagement::parametersError if no match between parameters provided and the function call is possible
     * + ErrorManagement::unsupportedFeature if dynamic_cast to specialised class type is possible with provided argument object
     * + on success the error returned by the method called
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StructuredDataI *parameters){
        return ErrorManagement::parametersError;
    }

    /**
     * @brief Calls the class method by taking the arguments from the ReferenceContainer parameters
     * @param[in] object is the pointer to the object owning the method.
     * @param[in] parameters a reference to a ReferenceContainer that will be used to fill the parameters to call the functions
     * @return
     * + ErrorManagement::parametersError if no match between parameters provided and the function call is possible
     * + ErrorManagement::unsupportedFeature if dynamic_cast to specialised class type is possible with provided argument object
     * + on success the error returned by the method called
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){
        return ErrorManagement::parametersError;
    }

    /**
     * @brief Calls the class method without parameters
     * @param[in] object is the pointer to the object owning the method.
     * @return
     * + ErrorManagement::parametersError if no match between parameters provided and the function call is possible
     * + ErrorManagement::unsupportedFeature if dynamic_cast to specialised class type is possible with provided argument object
     * + on success the error returned by the method called
     */
    virtual ErrorManagement::ErrorType Call(Object *object){
        return ErrorManagement::parametersError;
    }
};

/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with four arguments.
 *
 * @details This specialisation of the template generates a number of "Call"
 * methods for a variety of target methods .
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the arguments to be passed
 * to the target method.
 *
 * E.g.:
 * + XClassMethodCallerT<ClassA, int, float,char,short> mc(&ClassA::MethodX);
 *
 * @param className is the class owning the target method.
 * @param argType1 is the type of the input parameter to be passed to the target method 1.
 * @param argType2 is the type of the input parameter to be passed to the target method 2.
 * @param argType3 is the type of the input parameter to be passed to the target method 3.
 * @param argType4 is the type of the input parameter to be passed to the target method 4.
 * NOTE only basic types with no modifiers can be used here!!! no * and no &
 */
template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
class XClassMethodCallerT: public XClassMethodCaller{
public:

    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4);

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    XClassMethodCallerT(MethodPointer method){
        pFun = method;
    }

    /**
     * @brief Destructor.
     */
    virtual ~XClassMethodCallerT(){}

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StructuredDataI *parameters){
        ErrorManagement::ErrorType err(true);

        className* actual = dynamic_cast<className *>(object);
        err.unsupportedFeature = (actual == static_cast<className*>(0));

        if (err.ErrorsCleared()){
            argType1 param1;
            argType2 param2;
            argType3 param3;
            argType4 param4;
            parameters->Read("param1",param1);
            parameters->Read("param2",param2);
            parameters->Read("param3",param3);
            parameters->Read("param4",param4);
            err =  (actual->*pFun)(param1,param2,param3,param4);
        }
        return err;
    }

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){

        // no need to check validity of
        ReferenceT<StructuredDataI> param = parameters.Get(0);
        return Call (object, param.operator->());
    }

    //TODO create class to generate a StructuredDataI from a StreamI
    //TODO then Call the Call(StructuredDataI)

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;

};


/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with three arguments.
 *
 * @details This specialisation of the template generates a number of "Call"
 * methods for a variety of target methods .
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the arguments to be passed
 * to the target method.
 *
 * E.g.:
 * + XClassMethodCallerT<ClassA, int, float,char,short> mc(&ClassA::MethodX);
 *
 * @param className is the class owning the target method.
 * @param argType1 is the type of the input parameter to be passed to the target method 1.
 * @param argType2 is the type of the input parameter to be passed to the target method 2.
 * @param argType3 is the type of the input parameter to be passed to the target method 3.
 * NOTE only basic types with no modifiers can be used here!!! no * and no &
 */
template <class className, typename argType1,typename argType2,typename argType3>
class XClassMethodCallerT<className,argType1,argType2,argType3,void>:public XClassMethodCaller{
public:

    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3);


    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    XClassMethodCallerT(MethodPointer method){
        pFun = method;
    }

    /**
     * @brief Destructor.
     */
    virtual ~XClassMethodCallerT(){}

    /**
     * @brief See ClassMethodCaller
     */
    ErrorManagement::ErrorType Call(Object *object, StructuredDataI *parameters){
        ErrorManagement::ErrorType err(true);

        className* actual = dynamic_cast<className *>(object);
        err.unsupportedFeature = (actual == static_cast<className*>(0));

        if (err.ErrorsCleared()){
            argType1 param1;
            argType2 param2;
            argType3 param3;
            parameters->Read("param1",param1);
            parameters->Read("param2",param2);
            parameters->Read("param3",param3);
            err =  (actual->*pFun)(param1,param2,param3);
        }
        return err;
    }

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){
        ReferenceT<StructuredDataI> param = parameters.Get(0);
        return Call (object, param.operator->());
    }

    //TODO create class to generate a StructuredDataI from a StreamI
    //TODO then Call the Call(StructuredDataI)

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;

};

/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with two arguments.
 *
 * @details This specialisation of the template generates a number of "Call"
 * methods for a variety of target methods .
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the arguments to be passed
 * to the target method.
 *
 * E.g.:
 * + XClassMethodCallerT<ClassA, int, float,char,short> mc(&ClassA::MethodX);
 *
 * @param className is the class owning the target method.
 * @param argType1 is the type of the input parameter to be passed to the target method 1.
 * @param argType2 is the type of the input parameter to be passed to the target method 2.
 * NOTE only basic types with no modifiers can be used here!!! no * and no &
 */
template <class className, typename argType1,typename argType2>
class XClassMethodCallerT<className,argType1,argType2,void,void>:public XClassMethodCaller{
public:

    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2);


    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    XClassMethodCallerT(MethodPointer method){
        pFun = method;
    }

    /**
     * @brief Destructor.
     */
    virtual ~XClassMethodCallerT(){}

    /**
     * @brief See ClassMethodCaller
     */
    ErrorManagement::ErrorType Call(Object *object, StructuredDataI *parameters){
        ErrorManagement::ErrorType err(true);

        className* actual = dynamic_cast<className *>(object);
        err.unsupportedFeature = (actual == static_cast<className*>(0));

        if (err.ErrorsCleared()){
            argType1 param1;
            argType2 param2;
            parameters->Read("param1",param1);
            parameters->Read("param2",param2);
            err =  (actual->*pFun)(param1,param2);
        }
        return err;
    }

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){
        ReferenceT<StructuredDataI> param = parameters.Get(0);
        return Call (object, param.operator->());
    }

    //TODO create class to generate a StructuredDataI from a StreamI
    //TODO then Call the Call(StructuredDataI)

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;
};

/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with one argument.
 *
 * @details This specialisation of the template generates a number of "Call"
 * methods for a variety of target methods .
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the arguments to be passed
 * to the target method.
 *
 * E.g.:
 * + XClassMethodCallerT<ClassA, int, float,char,short> mc(&ClassA::MethodX);
 *
 * @param className is the class owning the target method.
 * @param argType1 is the type of the input parameter to be passed to the target method 1.
 * NOTE only basic types with no modifiers can be used here!!! no * and no &
 */
template <class className, typename argType1>
class XClassMethodCallerT<className,argType1,void,void,void>:public XClassMethodCaller{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef ErrorManagement::ErrorType (className::*MethodPointer)(argType1);

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    XClassMethodCallerT(MethodPointer method){
        pFun = method;
    }

    /**
     * @brief Destructor.
     */
    virtual ~XClassMethodCallerT(){}

    /**
     * @brief See ClassMethodCaller
     */
    ErrorManagement::ErrorType Call(Object *object, StructuredDataI *parameters){
        ErrorManagement::ErrorType err(true);

        className* actual = dynamic_cast<className *>(object);
        err.unsupportedFeature = (actual == static_cast<className*>(0));

        if (err.ErrorsCleared()){
            argType1 param1;
            parameters->Read("param1",param1);
            err =  (actual->*pFun)(param1);
        }
        return err;
    }

    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){
        ReferenceT<StructuredDataI> param = parameters.Get(0);
        return Call (object, param.operator->());
    }

    //TODO create class to generate a StructuredDataI from a StreamI
    //TODO then Call the Call(StructuredDataI)

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;
};

/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with no arguments.
 *
 * @details This specialisation of the template generates a number of "Call"
 * methods for a variety of target methods .
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the arguments to be passed
 * to the target method.
 *
 * E.g.:
 * + XClassMethodCallerT<ClassA, int, float,char,short> mc(&ClassA::MethodX);
 *
 * @param className is the class owning the target method.
 * NOTE only basic types with no modifiers can be used here!!! no * and no &
 */
template <class className>
class XClassMethodCallerT<className,void,void,void,void>:public XClassMethodCaller{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef ErrorManagement::ErrorType (className::*MethodPointer)();

private:

    /**
     * Pointer to the class method
     */
    MethodPointer pFun;
public:

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    XClassMethodCallerT(MethodPointer method){
        pFun = method;
    }

    /**
     * @brief Destructor.
     */
    virtual ~XClassMethodCallerT(){}

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StructuredDataI *parameters){
        return Call(object);
    }

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){
        return Call(object);
    }

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StreamI *stream){
        return Call(object);
    }

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object){
        className* actual = dynamic_cast<className *>(object);
        return (actual->*pFun)();
    }

};

/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with one StructuredDataI *argument.
 *
 * @details This specialisation of the template generates a number of "Call"
 * methods for a variety of target methods .
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the arguments to be passed
 * to the target method.
 *
 * E.g.:
 * + XClassMethodCallerT<ClassA, int, float,char,short> mc(&ClassA::MethodX);
 *
 * @param className is the class owning the target method.
 */
template <class className>
class XClassMethodCallerT<className,StructuredDataI *,void,void,void>:public XClassMethodCaller{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef ErrorManagement::ErrorType (className::*MethodPointer)(StructuredDataI *parameters);

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    XClassMethodCallerT(MethodPointer method){
        pFun = method;
    }

    /**
     * @brief Destructor.
     */
    virtual ~XClassMethodCallerT(){}

    /**
     * @brief See ClassMethodCaller
     */
    ErrorManagement::ErrorType Call(Object *object, StructuredDataI *parameters){
        className* actual = dynamic_cast<className *>(object);
        return (actual->*pFun)(parameters);
    }

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){
        ReferenceT<StructuredDataI> sI = parameters.Get(0);
        return Call(object,sI.operator->());
    }

    //TODO create class to generate a StructuredDataI from a StreamI
    //TODO then Call the Call(StructuredDataI)

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;
};


/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with one ReferenceContainer &argument.
 *
 * @details This specialisation of the template generates a number of "Call"
 * methods for a variety of target methods .
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the arguments to be passed
 * to the target method.
 *
 * E.g.:
 * + XClassMethodCallerT<ClassA, int, float,char,short> mc(&ClassA::MethodX);
 *
 * @param className is the class owning the target method.
 */
template <class className>
class XClassMethodCallerT<className,ReferenceContainer &,void,void,void>:public XClassMethodCaller{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef ErrorManagement::ErrorType (className::*MethodPointer)(ReferenceContainer &parameters);

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    XClassMethodCallerT(MethodPointer method){
        pFun = method;
    }

    /**
     * @brief Destructor.
     */
    virtual ~XClassMethodCallerT(){}

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StructuredDataI *parameters){

        ReferenceContainer param1;

        Object * o = dynamic_cast<Object *>(parameters);

        Reference ref(o);

        param1.Insert(ref);

        return Call (object ,param1);
    }

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){
        className* actual = dynamic_cast<className *>(object);
        return (actual->*pFun)(parameters);
    }

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;

};

/**
 * @brief This class template is a helper for building specific versions of
 * class method callers for target methods with one StreamI *argument.
 *
 * @details This specialisation of the template generates a number of "Call"
 * methods for a variety of target methods .
 *
 * This specialisation expects the name of the target class as the first
 * class template parameter, followed by the type of the arguments to be passed
 * to the target method.
 *
 * E.g.:
 * + XClassMethodCallerT<ClassA, int, float,char,short> mc(&ClassA::MethodX);
 *
 * @param className is the class owning the target method.
 * NOTE only basic types with no modifiers can be used here!!! no * and no &
 */
template <class className>
class XClassMethodCallerT<className,StreamI *,void,void,void>:public XClassMethodCaller{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
    typedef ErrorManagement::ErrorType (className::*MethodPointer)(StreamI *);

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    XClassMethodCallerT(MethodPointer method){
        pFun = method;
    }

    /**
     * @brief Destructor.
     */
    virtual ~XClassMethodCallerT(){}

    /**
     * @brief See ClassMethodCaller
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StructuredDataI *parameters){
        return ErrorManagement::parametersError;
    }

    /**
     * @brief See ClassMethodCaller
     */
    ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters){
        className* actual = dynamic_cast<className *>(object);

        Reference par = parameters.Get(0);

        ReferenceT<StreamI> param = par;

        return (actual->*pFun)(param.operator->());
    }

    /**
     * @brief See ClassMethodCaller
     */
    ErrorManagement::ErrorType Call(Object *object, StreamI *stream){
        className* actual = dynamic_cast<className *>(object);

        return (actual->*pFun)(stream);
    }

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;
};


template <class className>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)()){
    XClassMethodCaller *p = new XClassMethodCallerT<className,void,void,void,void>(MethodPointer);
    return p;
}

template <class className, typename argType1>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1)){
        return new XClassMethodCallerT<className,argType1,void,void,void>(MethodPointer);
}

template <class className, typename argType1,typename argType2>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2)){
        return new XClassMethodCallerT<className,argType1,argType2,void,void>(MethodPointer);
}

template <class className, typename argType1,typename argType2,typename argType3>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3)){
        return new XClassMethodCallerT<className,argType1,argType2,argType3,void>(MethodPointer);
}

template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4)){
        return new XClassMethodCallerT<className,argType1,argType2,argType3,argType4>(MethodPointer);
}



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}
#endif /* L2OBJECTS_CLASSMETHODCALLERFT_H_ */
	
