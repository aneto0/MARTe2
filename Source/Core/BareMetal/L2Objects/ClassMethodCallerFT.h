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
template <class className, typename MethodPointer, typename argType1,typename argType2,typename argType3,typename argType4>
class XClassMethodCallerT: public XClassMethodCaller{
public:

    /**
     * @brief Type definition for the method pointer prototype
     */
//    typedef ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4);

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    XClassMethodCallerT(MethodPointer method,uint32 mask){
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
            parameters->Read("param1",param1);
            argType2 param2;
            parameters->Read("param2",param2);
            argType3 param3;
            parameters->Read("param3",param3);
            argType4 param4;
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
template <class className,typename MethodPointer, typename argType1,typename argType2,typename argType3>
class XClassMethodCallerT<className,MethodPointer,argType1,argType2,argType3,void>:public XClassMethodCaller{
public:

    /**
     * @brief Type definition for the method pointer prototype
     */
//    typedef ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3);


    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    XClassMethodCallerT(MethodPointer method,uint32 mask){
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
template <class className, typename MethodPointer, typename argType1,typename argType2>
class XClassMethodCallerT<className,MethodPointer,argType1,argType2,void,void>:public XClassMethodCaller{
public:

    /**
     * @brief Type definition for the method pointer prototype
     */
//    typedef ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2);


    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    XClassMethodCallerT(MethodPointer method,uint32 mask){
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
template <class className, typename MethodPointer, typename argType1>
class XClassMethodCallerT<className,MethodPointer,argType1,void,void,void>:public XClassMethodCaller{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
//    typedef ErrorManagement::ErrorType (className::*MethodPointer)(argType1);

    /**
     * @brief Constructor from a class method with one i            argType4 param4;
     * nput parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    XClassMethodCallerT(MethodPointer method,uint32 mask){
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
template <class className,typename MethodPointer>
class XClassMethodCallerT<className,MethodPointer,void,void,void,void>:public XClassMethodCaller{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
//    typedef ErrorManagement::ErrorType (className::*MethodPointer)();

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
    XClassMethodCallerT(MethodPointer method,uint32 mask){
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
template <class className,typename MethodPointer>
class XClassMethodCallerT<className,MethodPointer,StructuredDataI *,void,void,void>:public XClassMethodCaller{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
//    typedef ErrorManagement::ErrorType (className::*MethodPointer)(StructuredDataI *parameters);

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    XClassMethodCallerT(MethodPointer method,uint32 mask){
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
template <class className,typename MethodPointer>
class XClassMethodCallerT<className,MethodPointer,ReferenceContainer,void,void,void>:public XClassMethodCaller{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
//    typedef ErrorManagement::ErrorType (className::*MethodPointer)(ReferenceContainer &parameters);

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    XClassMethodCallerT(MethodPointer method,uint32 mask){
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
template <class className,typename MethodPointer>
class XClassMethodCallerT<className,MethodPointer,StreamI *,void,void,void>:public XClassMethodCaller{
public:
    /**
     * @brief Type definition for the method pointer prototype
     */
//    typedef ErrorManagement::ErrorType (className::*MethodPointer)(StreamI *);

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] f is a pointer to the class method.
     * @post f == GetMethodPointer()
     */
    XClassMethodCallerT(MethodPointer method,uint32 mask){
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


















/**
 * @brief Implementation of an object which stores and calls a class method.
 * @details The class allows to create an implicit class method caller whose
 * actual target method is inferred from the constructor (this class has 3
 * templated constructors, which allow to bind the each instance with a target
 * method). The target method can have a prototype without arguments or with
 * one argument at most (with by reference and by copy versions).
 */
class DLL_API XClassMethodInterfaceMapper {

public:

    /**
     * @brief Default constructor.
     */
    //XClassMethodInterfaceMapper();

    /**
     * @brief Constructor by a class method with no arguments.
     * @tparam C is the class name.
     */
//    template<typename C>
//    XClassMethodInterfaceMapper(bool (C::*f)());

    /**
     * @brief Constructor by a class method with one argument passed by copy.
     * @param[in] f is a pointer to the class method to be registered.
     * @tparam C is the class name.
     * @tparam T is the type name of the class method argument.
     */
//    template<typename C, typename T>
//    XClassMethodInterfaceMapper(bool (C::*f)(T));

    /**
     * @brief Constructor by a class method with one argument passed by
     * reference.
     * @param[in] f is a pointer to the class method to be registered.
     * @tparam C is the class name.
     * @tparam T is the type name of the class method argument.
     */
//    template<typename C, typename T>
//    XClassMethodInterfaceMapper(bool (C::*f)(T&));


// unsupported feature trapping

    template <class className, typename argType1>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *)){
    //    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template <class className, typename argType1,typename argType2>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2 *)){
    //    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template <class className, typename argType1,typename argType2>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *,argType2)){
    //    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template <class className, typename argType1,typename argType2,typename argType3>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3 *)){
    //    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template <class className, typename argType1,typename argType2,typename argType3>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2 *,argType3)){
    //    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template <class className, typename argType1,typename argType2,typename argType3>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *,argType2,argType3)){
    //    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4 *)){
    //    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3 *,argType4)){
    //    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2 *,argType3,argType4)){
    //    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *,argType2,argType3,argType4)){
    //    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    // 0 params

    template <class className>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(void)){
        caller = new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(void),void,void,void,void>(MethodPointer,0x0000);
    }

    // 1 params

    template <class className, typename argType1>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &)){
        caller = new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(const argType1 &),argType1,void,void,void>(MethodPointer,0x0000);
    }

    template <class className, typename argType1>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &)){
        caller = new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(argType1 &),argType1,void,void,void>(MethodPointer,0x1000);
    }

    template <class className, typename argType1>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1)){
        caller = new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(argType1),argType1,void,void,void>(MethodPointer,0x0000);
    }



    // 2 params

    template <class className, typename argType1,typename argType2>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,const argType2 &)){
            caller = XClassMethodCallerCreate2(MethodPointer,0x0000);
    }

    template <class className, typename argType1,typename argType2>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2 &)){
            caller = XClassMethodCallerCreate2(MethodPointer,0x0400);
    }

    template <class className, typename argType1,typename argType2>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2 )){
            caller = XClassMethodCallerCreate2(MethodPointer,0x0000);
    }


    template <class className, typename argType1,typename argType2>
    XClassMethodCaller &XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &,argType2),uint32 mask){
            return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(const argType1 &,argType2),argType1,argType2,void,void>(MethodPointer,mask);
    }

    template <class className, typename argType1,typename argType2>
    XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &,argType2),uint32 mask){
            return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(argType1 &,argType2),argType1,argType2,void,void>(MethodPointer,mask | 0x1000);
    }

    template <class className, typename argType1,typename argType2>
    XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2),uint32 mask){
            return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(argType1,argType2),argType1,argType2,void,void>(MethodPointer,mask);
    }

    // 3 params

    template <class className, typename argType1,typename argType2,typename argType3>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,const argType3 & )){
            caller = XClassMethodCallerCreate2(MethodPointer,0x0000);
    }

    template <class className, typename argType1,typename argType2,typename argType3>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3 &)){
            caller = XClassMethodCallerCreate2(MethodPointer,0x0010);
    }

    template <class className, typename argType1,typename argType2,typename argType3>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3)){
            caller = XClassMethodCallerCreate2(MethodPointer,0x0000);
    }


    template <class className, typename argType1,typename argType2,typename argType3>
    XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,const argType2 &,argType3),uint32 mask){
            return XClassMethodCallerCreate3(MethodPointer,mask);
    }

    template <class className, typename argType1,typename argType2,typename argType3>
    XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2 &,argType3),uint32 mask){
            return XClassMethodCallerCreate3(MethodPointer,mask | 0x0100);
    }

    template <class className, typename argType1,typename argType2,typename argType3>
    XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3),uint32 mask){
            return XClassMethodCallerCreate3(MethodPointer,mask);
    }


    template <class className, typename argType1,typename argType2,typename argType3>
    XClassMethodCaller *XClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &,argType2,argType3),uint32 mask){
            return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(const argType1 &,argType2,argType3),argType1,argType2,argType3,void>(MethodPointer,mask);
    }

    template <class className, typename argType1,typename argType2,typename argType3>
    XClassMethodCaller *XClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &,argType2,argType3),uint32 mask){
            return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(argType1 &,argType2,argType3),argType1,argType2,argType3,void>(MethodPointer,mask | 0x0010);
    }

    template <class className, typename argType1,typename argType2,typename argType3>
    XClassMethodCaller *XClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3),uint32 mask){
            return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(argType1,argType2,argType3),argType1,argType2,argType3,void>(MethodPointer,mask);
    }


    // 4 params

    template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,const argType4 &)){
            caller = XClassMethodCallerCreate2(MethodPointer,0x0000);
    }

    template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4 &)){
            caller = XClassMethodCallerCreate2(MethodPointer,0x0001);
    }

    template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
    XClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4)){
            caller = XClassMethodCallerCreate2(MethodPointer,0x0000);
    }


    template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
    XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,const argType3 &,argType4 ),uint32 mask){
            return XClassMethodCallerCreate3(MethodPointer,mask);
    }

    template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
    XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3 &,argType4),uint32 mask){
            return XClassMethodCallerCreate3(MethodPointer,mask | 0x0010);
    }

    template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
    XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4),uint32 mask){
            return XClassMethodCallerCreate3(MethodPointer,mask);
    }


    template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
    XClassMethodCaller *XClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,const argType2 &,argType3,argType4 ),uint32 mask){
            return XClassMethodCallerCreate4(MethodPointer,mask);
    }

    template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
    XClassMethodCaller *XClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2 &,argType3,argType4),uint32 mask){
            return XClassMethodCallerCreate4(MethodPointer,mask | 0x0100);
    }

    template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
    XClassMethodCaller *XClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4),uint32 mask){
            return XClassMethodCallerCreate4(MethodPointer,mask);
    }


    template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
    XClassMethodCaller *XClassMethodCallerCreate4(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,const argType2 &,argType3,argType4 ),uint32 mask){
            return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(const argType1 &,argType2,argType3,argType4),argType1,argType2,argType3,argType4>(MethodPointer,mask);
    }

    template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
    XClassMethodCaller *XClassMethodCallerCreate4(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2 &,argType3,argType4),uint32 mask){
        return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(argType1 &,argType2,argType3,argType4),argType1,argType2,argType3,argType4>(MethodPointer,mask | 0x1000);
    }

    template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
    XClassMethodCaller *XClassMethodCallerCreate4(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4),uint32 mask){
        return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(argType1,argType2,argType3,argType4),argType1,argType2,argType3,argType4>(MethodPointer,mask);
    }

    /**
     *
     */
    XClassMethodCaller *GetMethod(){ return caller; }


    /**
     * @brief Destructor
     */
    virtual ~XClassMethodInterfaceMapper();

#if 0
    /**
     * @brief Calls the function with no arguments.
     * @param[in] context is the object which must call the function.
     * @return
     * + ErrorManagement::unsupportedFeature if no function has been registered
     * + ErrorManagement::fatalError if the class method returns false
     * + ErrorManagement::noError if it returns true.
     */
    ErrorManagement::ErrorType Call(Object * const context);

    /**
     * @brief Calls the function with one argument.
     * @tparam T is the type name of the class method argument.
     * @param[in] context is the object which must call the function.
     * @param[in,out] ref is the class method argument.
     * @return
     * + ErrorManagement::unsupportedFeature if no function has been registered
     * + ErrorManagement::fatalError if the class method returns false
     * + ErrorManagement::noError if it returns true.
     */
    template<typename T>
    ErrorManagement::ErrorType Call(Object * const context,
                                    T ref);

#endif
private:

    /**
     * The class method caller
     */
    XClassMethodCaller *caller;



};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
#if 0
template<typename className>
XClassMethodInterfaceMapper::XClassMethodInterfaceMapper(bool (className::*f)()) {
    caller = new XClassMethodCallerT<className>(f);
    byCopy = false;
}

template<typename className, typename T>
XClassMethodInterfaceMapper::XClassMethodInterfaceMapper(bool (className::*f)(T)) {
    caller = new XClassMethodCallerT<className, T>(f);
    byCopy = true;
}

template<typename className, typename T>
XClassMethodInterfaceMapper::XClassMethodInterfaceMapper(bool (className::*f)(T&)) {
    caller = new XClassMethodCallerT<className, T&>(f);
    byCopy = false;
}

template<typename T>
ErrorManagement::ErrorType XClassMethodInterfaceMapper::Call(Object *context,
                                                            T ref) {
    ErrorManagement::ErrorType ret;
    ret.unsupportedFeature = true;
    if (caller != NULL ) {
        if(byCopy) {
            ret = caller->Call(context,ref, byCopy);
        }
        else {
            ret = caller->Call(context,ref);
        }
    }
    return ret;
}

#endif



template <class className, typename argType1>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *)){
//    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
    return NULL;
}

template <class className, typename argType1,typename argType2>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2 *)){
//    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
    return NULL;
}

template <class className, typename argType1,typename argType2>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *,argType2)){
//    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
    return NULL;
}

template <class className, typename argType1,typename argType2,typename argType3>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3 *)){
//    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
    return NULL;
}

template <class className, typename argType1,typename argType2,typename argType3>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2 *,argType3)){
//    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
    return NULL;
}

template <class className, typename argType1,typename argType2,typename argType3>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *,argType2,argType3)){
//    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
    return NULL;
}

template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4 *)){
//    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
    return NULL;
}

template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3 *,argType4)){
//    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
    return NULL;
}

template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2 *,argType3,argType4)){
//    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
    return NULL;
}

template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *,argType2,argType3,argType4)){
//    static_assert(false, "Do not use XClassMethodCallerCreate with method having pointer parameters!");
    return NULL;
}

// 0 params

template <class className>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(void)){
    XClassMethodCaller *p = new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(void),void,void,void,void>(MethodPointer,0x0000);
    return p;
}

// 1 params

template <class className, typename argType1>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &)){
        return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(const argType1 &),argType1,void,void,void>(MethodPointer,0x0000);
}

template <class className, typename argType1>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &)){
        return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(argType1 &),argType1,void,void,void>(MethodPointer,0x1000);
}

template <class className, typename argType1>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1)){
        return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(argType1),argType1,void,void,void>(MethodPointer,0x0000);
}



// 2 params

template <class className, typename argType1,typename argType2>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,const argType2 &)){
        return XClassMethodCallerCreate2(MethodPointer,0x0000);
}

template <class className, typename argType1,typename argType2>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2 &)){
        return XClassMethodCallerCreate2(MethodPointer,0x0400);
}

template <class className, typename argType1,typename argType2>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2 )){
        return XClassMethodCallerCreate2(MethodPointer,0x0000);
}


template <class className, typename argType1,typename argType2>
XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &,argType2),uint32 mask){
        return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(const argType1 &,argType2),argType1,argType2,void,void>(MethodPointer,mask);
}

template <class className, typename argType1,typename argType2>
XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &,argType2),uint32 mask){
        return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(argType1 &,argType2),argType1,argType2,void,void>(MethodPointer,mask | 0x1000);
}

template <class className, typename argType1,typename argType2>
XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2),uint32 mask){
        return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(argType1,argType2),argType1,argType2,void,void>(MethodPointer,mask);
}

// 3 params

template <class className, typename argType1,typename argType2,typename argType3>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,const argType3 & )){
        return XClassMethodCallerCreate2(MethodPointer,0x0000);
}

template <class className, typename argType1,typename argType2,typename argType3>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3 &)){
        return XClassMethodCallerCreate2(MethodPointer,0x0010);
}

template <class className, typename argType1,typename argType2,typename argType3>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3)){
        return XClassMethodCallerCreate2(MethodPointer,0x0000);
}


template <class className, typename argType1,typename argType2,typename argType3>
XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,const argType2 &,argType3),uint32 mask){
        return XClassMethodCallerCreate3(MethodPointer,mask);
}

template <class className, typename argType1,typename argType2,typename argType3>
XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2 &,argType3),uint32 mask){
        return XClassMethodCallerCreate3(MethodPointer,mask | 0x0100);
}

template <class className, typename argType1,typename argType2,typename argType3>
XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3),uint32 mask){
        return XClassMethodCallerCreate3(MethodPointer,mask);
}


template <class className, typename argType1,typename argType2,typename argType3>
XClassMethodCaller *XClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &,argType2,argType3),uint32 mask){
        return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(const argType1 &,argType2,argType3),argType1,argType2,argType3,void>(MethodPointer,mask);
}

template <class className, typename argType1,typename argType2,typename argType3>
XClassMethodCaller *XClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &,argType2,argType3),uint32 mask){
        return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(argType1 &,argType2,argType3),argType1,argType2,argType3,void>(MethodPointer,mask | 0x0010);
}

template <class className, typename argType1,typename argType2,typename argType3>
XClassMethodCaller *XClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3),uint32 mask){
        return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(argType1,argType2,argType3),argType1,argType2,argType3,void>(MethodPointer,mask);
}


// 4 params

template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,const argType4 &)){
        return XClassMethodCallerCreate2(MethodPointer,0x0000);
}

template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4 &)){
        return XClassMethodCallerCreate2(MethodPointer,0x0001);
}

template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4)){
        return XClassMethodCallerCreate2(MethodPointer,0x0000);
}


template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,const argType3 &,argType4 ),uint32 mask){
        return XClassMethodCallerCreate3(MethodPointer,mask);
}

template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3 &,argType4),uint32 mask){
        return XClassMethodCallerCreate3(MethodPointer,mask | 0x0010);
}

template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4),uint32 mask){
        return XClassMethodCallerCreate3(MethodPointer,mask);
}


template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,const argType2 &,argType3,argType4 ),uint32 mask){
        return XClassMethodCallerCreate4(MethodPointer,mask);
}

template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2 &,argType3,argType4),uint32 mask){
        return XClassMethodCallerCreate4(MethodPointer,mask | 0x0100);
}

template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4),uint32 mask){
        return XClassMethodCallerCreate4(MethodPointer,mask);
}


template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate4(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,const argType2 &,argType3,argType4 ),uint32 mask){
        return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(const argType1 &,argType2,argType3,argType4),argType1,argType2,argType3,argType4>(MethodPointer,mask);
}

template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate4(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2 &,argType3,argType4),uint32 mask){
    return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(argType1 &,argType2,argType3,argType4),argType1,argType2,argType3,argType4>(MethodPointer,mask | 0x1000);
}

template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate4(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4),uint32 mask){
    return new XClassMethodCallerT<className,ErrorManagement::ErrorType (className::*)(argType1,argType2,argType3,argType4),argType1,argType2,argType3,argType4>(MethodPointer,mask);
}












#if 0
template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate(ErrorManagement::ErrorType (className::*MethodPointer)(argType1&,argType2,argType3,argType4)){
        return XClassMethodCallerCreate2(MethodPointer,0x1);
}
template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2,argType3,argType4),int flag){
        return XClassMethodCallerCreate3(MethodPointer,0x1);
}
template <class className, typename argType1,typename argType2,typename argType3,typename argType4>
XClassMethodCaller *XClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,argType2&,argType3,argType4),int flag){
        return XClassMethodCallerCreate3(MethodPointer,flag | 0x2);
}
#endif

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}
#endif /* L2OBJECTS_CLASSMETHODCALLERFT_H_ */
	
