/**
 * @file ClassMethodCallerT.h
 * @brief Header file for class ClassMethodCallerT
 * @date 13/06/2016
 * @author Giuseppe Ferrò
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * argTypeou may not use this work except in compliance with the Licence.
 * argTypeou may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANargType KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.
 *
 * @details This header file contains the declaration of the class ClassMethodCallerT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSMETHODCALLERT_H_
#define CLASSMETHODCALLERT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ClassMethodCaller.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * These constants specify if the value of the template parameters below are being by reference and
 *  thus should be updated when the Call function returns.
 */
static const uint32 PARAM1_IS_REFERENCE = 0x1000u;
static const uint32 PARAM2_IS_REFERENCE = 0x0100u;
static const uint32 PARAM3_IS_REFERENCE = 0x0010u;
static const uint32 PARAM4_IS_REFERENCE = 0x0001u;

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
 * + ClassMethodCallerT<ClassA, int, float, char, short> mc(&ClassA::MethodX);
 *
 * @tparam className is the class owning the target method.
 * @tparam MethodPointer is the pointer type of the target method.
 * @tparam argType1 is the type of the input parameter to be passed to the target method.
 * @tparam argType2 is the type of the input parameter to be passed to the target method.
 * @tparam argType3 is the type of the input parameter to be passed to the target method.
 * @tparam argType4 is the type of the input parameter to be passed to the target method.
 * @warning no pointers are allowed as input parameters.
 */
template<class className, typename MethodPointer, typename argType1, typename argType2, typename argType3, typename argType4>
class ClassMethodCallerT: public ClassMethodCaller {
public:

    /**
     * @brief Constructor from a class method with four input parameters.
     * @param[in] method is a pointer to the class method with four input parameters.
     * @param[in] maskIn a bit-field specifying which of the parameters are passed as an output parameter (i.e. has a reference).
     */
    ClassMethodCallerT(MethodPointer method, uint32 maskIn);

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT();

    /**
     * @brief See ClassMethodCaller.
     * @details The parameters StructuredDataI is expected to have valid field nodes named: param1, param2, param3 and param4; which contain the parameter values.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StructuredDataI &parameters);

    /**
     * @brief See ClassMethodCaller.
     * @details The parameters ReferenceContainer is expected to have a valid StructuredDataI on its first entry.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters);

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;

    /**
     * Read/Write mask
     */
    uint32 mask;
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
 * + ClassMethodCallerT<ClassA, int32, float32, char8> mc(&ClassA::MethodX);
 *
 * @tparam className is the class owning the target method.
 * @tparam MethodPointer is the pointer type of the target method.
 * @tparam argType1 is the type of the input parameter to be passed to the target method.
 * @tparam argType2 is the type of the input parameter to be passed to the target method.
 * @tparam argType3 is the type of the input parameter to be passed to the target method.
 * @warning no pointers are allowed as input parameters.
 */
template<class className, typename MethodPointer, typename argType1, typename argType2, typename argType3>
class ClassMethodCallerT<className, MethodPointer, argType1, argType2, argType3, void> : public ClassMethodCaller {
public:

    /**
     * @brief Constructor from a class method with three input parameters.
     * @param[in] method is a pointer to the class method with three input parameters.
     * @param[in] maskIn a bit-field specifying which of the parameters are passed as an output parameter (i.e. has a reference).
     */
    ClassMethodCallerT(MethodPointer method, uint32 maskIn);

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT();

    /**
     * @brief See ClassMethodCaller.
     * @details The parameters StructuredDataI is expected to have valid field nodes named: param1, param2 and param3; which contain the parameter values.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StructuredDataI &parameters);

    /**
     * @brief See ClassMethodCaller.
     * @details The parameters ReferenceContainer is expected to have a valid StructuredDataI on its first entry.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters);

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;

    /**
     * Read/write mask
     */
    uint32 mask;

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
 * + ClassMethodCallerT<ClassA, int32, float32> mc(&ClassA::MethodX);
 *
 * @tparam className is the class owning the target method.
 * @tparam MethodPointer is the pointer type of the target method.
 * @tparam argType1 is the type of the input parameter to be passed to the target method.
 * @tparam argType2 is the type of the input parameter to be passed to the target method.
 * @warning no pointers are allowed as input parameters.
 */
template<class className, typename MethodPointer, typename argType1, typename argType2>
class ClassMethodCallerT<className, MethodPointer, argType1, argType2, void, void> : public ClassMethodCaller {
public:

    /**
     * @brief Constructor from a class method with two input parameters.
     * @param[in] method is a pointer to the class method with two input parameters.
     * @param[in] maskIn a bit-field specifying which of the parameters are passed as an output parameter (i.e. has a reference).
     */
    ClassMethodCallerT(MethodPointer method, uint32 maskIn);

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT();

    /**
     * @brief See ClassMethodCaller.
     * @details The parameters StructuredDataI is expected to have valid field nodes named: param1 and param2; which contain the parameter values.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StructuredDataI &parameters);

    /**
     * @brief See ClassMethodCaller.
     * @details The parameters ReferenceContainer is expected to have a valid StructuredDataI on its first entry.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters);

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;

    /**
     * Read/write mask
     */
    uint32 mask;
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
 * + ClassMethodCallerT<ClassA, int32> mc(&ClassA::MethodX);
 *
 * @tparam className is the class owning the target method.
 * @tparam MethodPointer is the pointer type of the target method.
 * @tparam argType1 is the type of the input parameter to be passed to the target method.
 * @warning no pointers are allowed as input parameters.
 */
template<class className, typename MethodPointer, typename argType1>
class ClassMethodCallerT<className, MethodPointer, argType1, void, void, void> : public ClassMethodCaller {
public:

    /**
     * @brief Constructor from a class method with one input parameter.
     * @param[in] method is a pointer to the class method with one input parameter.
     * @param[in] maskIn a bit-field specifying which of the parameters are passed as an output parameter (i.e. has a reference).
     */
    ClassMethodCallerT(MethodPointer method, uint32 maskIn);

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT();

    /**
     * @brief See ClassMethodCaller.
     * @details The parameters StructuredDataI is expected to have valid field nodes named: param1 and param2; which contain the parameter values.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StructuredDataI &parameters);

    /**
     * @brief See ClassMethodCaller.
     * @details The parameters ReferenceContainer is expected to have a valid StructuredDataI on its first entry.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters);

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;

    /**
     * Read/Write mask
     */
    uint32 mask;
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
 * + ClassMethodCallerT<ClassA> mc(&ClassA::MethodX);
 *
 * @tparam className is the class owning the target method.
 * @tparam MethodPointer is the pointer type of the target method.
 */
template<class className, typename MethodPointer>
class ClassMethodCallerT<className, MethodPointer, void, void, void, void> : public ClassMethodCaller {
public:

    /**
     * @brief Constructor from a class method with void parameters.
     * @param[in] method is a pointer to the class method with void parameters.
     * @param[in] mask not used.
     */
    ClassMethodCallerT(MethodPointer method, uint32 mask);

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT();

    /**
     * @brief See ClassMethodCaller.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StructuredDataI &parameters);

    /**
     * @brief See ClassMethodCaller.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters);

    /**
     * @brief See ClassMethodCaller.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StreamI &stream);

    /**
     * @brief See ClassMethodCaller.
     */
    virtual ErrorManagement::ErrorType Call(Object *object);

private:

    /**
     * Pointer to the class method.
     */
    MethodPointer pFun;
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
 * + ClassMethodCallerT<ClassA, StructuredDataI> mc(&ClassA::MethodX);
 *
 * @tparam className is the class owning the target method.
 * @tparam MethodPointer is the pointer type of the target method.
 */
template<class className, typename MethodPointer>
class ClassMethodCallerT<className, MethodPointer, StructuredDataI, void, void, void> : public ClassMethodCaller {
public:

    /**
     * @brief Constructor from a class method with one input parameter of type StructuredDataI.
     * @param[in] method is a pointer to the class method with one input parameter of type StructuredDataI.
     * @param[in] maskIn not used.
     */
    ClassMethodCallerT(MethodPointer method, uint32 maskIn);

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT();

    /**
     * @brief See ClassMethodCaller.
     * @details Calls the registered function with the input StructuredDataI parameter.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StructuredDataI &parameters);

    /**
     * @brief See ClassMethodCaller.
     * @details The parameters ReferenceContainer is expected to have a valid StructuredDataI on its first entry.
     * This is then used as the input argument of the registered function.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters);

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
 * + ClassMethodCallerT<ClassA, ReferenceContainer> mc(&ClassA::MethodX);
 *
 * @tparam className is the class owning the target method.
 * @tparam MethodPointer is the pointer type of the target method.
 */
template<class className, typename MethodPointer>
class ClassMethodCallerT<className, MethodPointer, ReferenceContainer, void, void, void> : public ClassMethodCaller {
public:

    /**
     * @brief Constructor from a class method with one input parameter of type ReferenceContainer.
     * @param[in] method is a pointer to the class method with one input parameter of type ReferenceContainer.
     * @param[in] mask not used.
     */
    ClassMethodCallerT(MethodPointer method, uint32 mask);

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT();

    /**
     * @brief See ClassMethodCaller.
     * @details Adds the StructuredDataI parameter to a ReferenceContainer and calls the registered function with this container as an argument.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StructuredDataI &parameters);

    /**
     * @brief See ClassMethodCaller.
     * @details Calls the registered function with the input ReferenceContainer parameter.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters);

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
 * + ClassMethodCallerT<ClassA, StreamI> mc(&ClassA::MethodX);
 *
 * @tparam className is the class owning the target method.
 * @tparam MethodPointer is the pointer type of the target method.
 */
template<class className, typename MethodPointer>
class ClassMethodCallerT<className, MethodPointer, StreamI, void, void, void> : public ClassMethodCaller {
public:

    /**
     * @brief Constructor from a class method with one input parameter of type StreamI.
     * @param[in] method is a pointer to the class method with one input parameter of type StreamI.
     * @param[in] mask not used.
     */
    ClassMethodCallerT(MethodPointer method, uint32 mask);

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCallerT();

    /**
     * @brief See ClassMethodCaller.
     * @details The parameters ReferenceContainer is expected to have a valid StreamI on its first entry.
     * This is then used as the input argument of the registered function.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, ReferenceContainer &parameters);

    /**
     * @brief See ClassMethodCaller.
     * @details Calls the registered function with the input StreamI parameter.
     */
    virtual ErrorManagement::ErrorType Call(Object *object, StreamI &stream);

private:
    /**
     * Pointer to the class method
     */
    MethodPointer pFun;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {



template<class className, typename MethodPointer, typename argType1, typename argType2, typename argType3, typename argType4>
ClassMethodCallerT<className, MethodPointer, argType1, argType2, argType3, argType4>::ClassMethodCallerT(MethodPointer method, uint32 maskIn){
    pFun = method;
    mask = maskIn;
}

template<class className, typename MethodPointer, typename argType1, typename argType2, typename argType3, typename argType4>
ClassMethodCallerT<className, MethodPointer, argType1, argType2, argType3, argType4>::~ClassMethodCallerT(){

}

template<class className, typename MethodPointer, typename argType1, typename argType2, typename argType3, typename argType4>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, argType1, argType2, argType3, argType4>::Call(Object *object, StructuredDataI &parameters) {
    ErrorManagement::ErrorType err(true);

    className* actual = dynamic_cast<className *>(object);
    err.unsupportedFeature = (actual == static_cast<className*>(0));

    argType1 param1;
    argType2 param2;
    argType3 param3;
    argType4 param4;
    if (err.ErrorsCleared()) {
        if (!parameters.Read("param1", param1)) {
            err.parametersError = true;
        }
    }
    if (err.ErrorsCleared()) {
        if (!parameters.Read("param2", param2)) {
            err.parametersError = true;
        }
    }
    if (err.ErrorsCleared()) {
        if (!parameters.Read("param3", param3)) {
            err.parametersError = true;
        }
    }
    if (err.ErrorsCleared()) {
        if (!parameters.Read("param4", param4)) {
            err.parametersError = true;
        }
    }
    if (err.ErrorsCleared()) {
        err = (actual->*pFun)(param1, param2, param3, param4);
    }
    if (err.ErrorsCleared()) {
        if ((mask & PARAM1_IS_REFERENCE) == PARAM1_IS_REFERENCE) {
            if (!parameters.Write("param1", param1)) {
                err.parametersError = true;
            }
        }
    }
    if (err.ErrorsCleared()) {
        if ((mask & PARAM2_IS_REFERENCE) == PARAM2_IS_REFERENCE) {
            if (!parameters.Write("param2", param2)) {
                err.parametersError = true;
            }
        }
    }
    if (err.ErrorsCleared()) {
        if ((mask & PARAM3_IS_REFERENCE) == PARAM3_IS_REFERENCE) {
            if (!parameters.Write("param3", param3)) {
                err.parametersError = true;
            }
        }
    }
    if (err.ErrorsCleared()) {
        if ((mask & PARAM4_IS_REFERENCE) == PARAM4_IS_REFERENCE) {
            if (!parameters.Write("param4", param4)) {
                err.parametersError = true;
            }
        }
    }

    return err;
}

template<class className, typename MethodPointer, typename argType1, typename argType2, typename argType3, typename argType4>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, argType1, argType2, argType3, argType4>::Call(Object *object, ReferenceContainer &parameters) {
    ErrorManagement::ErrorType err(true);

    ReferenceT<StructuredDataI> sI = parameters.Get(0);

    err.parametersError = !sI.IsValid();
    if (err.ErrorsCleared()) {
        err = Call(object, *(sI.operator->()));
    }

    return err;
}

template<class className, typename MethodPointer, typename argType1, typename argType2, typename argType3>
ClassMethodCallerT<className, MethodPointer, argType1, argType2, argType3, void>::ClassMethodCallerT(MethodPointer method, uint32 maskIn) {
    pFun = method;
    mask = maskIn;
}

template<class className, typename MethodPointer, typename argType1, typename argType2, typename argType3>
ClassMethodCallerT<className, MethodPointer, argType1, argType2, argType3, void>::~ClassMethodCallerT() {
}

template<class className, typename MethodPointer, typename argType1, typename argType2, typename argType3>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, argType1, argType2, argType3, void>::Call(Object *object, StructuredDataI &parameters) {
    ErrorManagement::ErrorType err(true);
    className* actual = dynamic_cast<className *>(object);
    err.unsupportedFeature = (actual == static_cast<className*>(0));

    argType1 param1;
    argType2 param2;
    argType3 param3;

    if (err.ErrorsCleared()) {
        if (!parameters.Read("param1", param1)) {
            err.parametersError = true;
        }
    }
    if (err.ErrorsCleared()) {
        if (!parameters.Read("param2", param2)) {
            err.parametersError = true;
        }
    }
    if (err.ErrorsCleared()) {
        if (!parameters.Read("param3", param3)) {
            err.parametersError = true;
        }
    }
    if (err.ErrorsCleared()) {
        err = (actual->*pFun)(param1, param2, param3);
    }
    if (err.ErrorsCleared()) {
        if ((mask & PARAM1_IS_REFERENCE) == PARAM1_IS_REFERENCE) {
            if (!parameters.Write("param1", param1)) {
                err.parametersError = true;
            }
        }
    }
    if (err.ErrorsCleared()) {
        if ((mask & PARAM2_IS_REFERENCE) == PARAM2_IS_REFERENCE) {
            if (!parameters.Write("param2", param2)) {
                err.parametersError = true;
            }
        }
    }
    if (err.ErrorsCleared()) {
        if ((mask & PARAM3_IS_REFERENCE) == PARAM3_IS_REFERENCE) {
            if (!parameters.Write("param3", param3)) {
                err.parametersError = true;
            }
        }
    }
    return err;
}

template<class className, typename MethodPointer, typename argType1, typename argType2, typename argType3>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, argType1, argType2, argType3, void>::Call(Object *object, ReferenceContainer &parameters) {
    ErrorManagement::ErrorType err(true);
    ReferenceT<StructuredDataI> sI = parameters.Get(0);

    err.parametersError = !sI.IsValid();
    if (err.ErrorsCleared()) {
        err = Call(object, *(sI.operator->()));
    }

    return err;
}

template<class className, typename MethodPointer, typename argType1, typename argType2>
ClassMethodCallerT<className, MethodPointer, argType1, argType2, void, void>::ClassMethodCallerT(MethodPointer method, uint32 maskIn) {
    pFun = method;
    mask = maskIn;
}

template<class className, typename MethodPointer, typename argType1, typename argType2>
ClassMethodCallerT<className, MethodPointer, argType1, argType2, void, void>::~ClassMethodCallerT() {
}

template<class className, typename MethodPointer, typename argType1, typename argType2>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, argType1, argType2, void, void>::Call(Object *object, StructuredDataI &parameters) {
    ErrorManagement::ErrorType err(true);

    className* actual = dynamic_cast<className *>(object);
    err.unsupportedFeature = (actual == static_cast<className*>(0));

    argType1 param1;
    argType2 param2;

    if (err.ErrorsCleared()) {
        if (!parameters.Read("param1", param1)) {
            err.parametersError = true;
        }
    }
    if (err.ErrorsCleared()) {
        if (!parameters.Read("param2", param2)) {
            err.parametersError = true;
        }
    }
    if (err.ErrorsCleared()) {
        err = (actual->*pFun)(param1, param2);
    }
    if (err.ErrorsCleared()) {
        if ((mask & PARAM1_IS_REFERENCE) == PARAM1_IS_REFERENCE) {
            if (!parameters.Write("param1", param1)) {
                err.parametersError = true;
            }
        }
    }
    if (err.ErrorsCleared()) {
        if ((mask & PARAM2_IS_REFERENCE) == PARAM2_IS_REFERENCE) {
            if (!parameters.Write("param2", param2)) {
                err.parametersError = true;
            }
        }
    }
    return err;
}

template<class className, typename MethodPointer, typename argType1, typename argType2>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, argType1, argType2, void, void>::Call(Object *object, ReferenceContainer &parameters) {
    ErrorManagement::ErrorType err(true);

    ReferenceT<StructuredDataI> sI = parameters.Get(0);

    err.parametersError = !sI.IsValid();
    if (err.ErrorsCleared()) {
        err = Call(object, *(sI.operator->()));
    }

    return err;
}


template<class className, typename MethodPointer, typename argType1>
ClassMethodCallerT<className, MethodPointer, argType1, void, void, void>::ClassMethodCallerT(MethodPointer method, uint32 maskIn) {
    pFun = method;
    mask = maskIn;
}

template<class className, typename MethodPointer, typename argType1>
ClassMethodCallerT<className, MethodPointer, argType1, void, void, void>::~ClassMethodCallerT() {
}

template<class className, typename MethodPointer, typename argType1>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, argType1, void, void, void>::Call(Object *object, StructuredDataI &parameters) {

    ErrorManagement::ErrorType err(true);

    className* actual = dynamic_cast<className *>(object);
    err.unsupportedFeature = (actual == static_cast<className*>(0));

    argType1 param1;
    if (err.ErrorsCleared()) {
        if (!parameters.Read("param1", param1)) {
            err.parametersError = true;
        }
    }
    if (err.ErrorsCleared()) {
        err = (actual->*pFun)(param1);
    }
    if (err.ErrorsCleared()) {
        if (mask != 0x0) {
            err = !parameters.Write("param1", param1);
        }
    }
    return err;
}

template<class className, typename MethodPointer, typename argType1>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, argType1, void, void, void>::Call(Object *object, ReferenceContainer &parameters) {

    ErrorManagement::ErrorType err;
    ReferenceT<StructuredDataI> param = parameters.Get(0);
    if (param.IsValid()) {
        err = Call(object, *(param.operator->()));
    }
    else {
        err = ErrorManagement::ParametersError;
    }
    return err;
}

template<class className, typename MethodPointer>
ClassMethodCallerT<className, MethodPointer, void, void, void, void>::ClassMethodCallerT(MethodPointer method, uint32 mask) {
    pFun = method;
}

template<class className, typename MethodPointer>
ClassMethodCallerT<className, MethodPointer, void, void, void, void>::~ClassMethodCallerT() {
}

template<class className, typename MethodPointer>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, void, void, void, void>::Call(Object *object, StructuredDataI &parameters) {
    return Call(object);
}

template<class className, typename MethodPointer>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, void, void, void, void>::Call(Object *object, ReferenceContainer &parameters) {
    return Call(object);
}

template<class className, typename MethodPointer>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, void, void, void, void>::Call(Object *object, StreamI &stream) {
    return Call(object);
}

template<class className, typename MethodPointer>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, void, void, void, void>::Call(Object *object) {
    className* actual = dynamic_cast<className *>(object);
    return (actual->*pFun)();
}

template<class className, typename MethodPointer>
ClassMethodCallerT<className, MethodPointer, StructuredDataI, void, void, void>::ClassMethodCallerT(MethodPointer method, uint32 maskIn) {
    pFun = method;
}

template<class className, typename MethodPointer>
ClassMethodCallerT<className, MethodPointer, StructuredDataI, void, void, void>::~ClassMethodCallerT() {
}

template<class className, typename MethodPointer>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, StructuredDataI, void, void, void>::Call(Object *object, StructuredDataI &parameters) {
    className* actual = dynamic_cast<className *>(object);
    return (actual->*pFun)(parameters);
}

template<class className, typename MethodPointer>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, StructuredDataI, void, void, void>::Call(Object *object, ReferenceContainer &parameters) {
    ErrorManagement::ErrorType err(true);

    ReferenceT<StructuredDataI> sI = parameters.Get(0);

    err.parametersError = !sI.IsValid();
    if (err.ErrorsCleared()) {
        err = Call(object, *(sI.operator->()));
    }

    return err;
}


template<class className, typename MethodPointer>
ClassMethodCallerT<className, MethodPointer, ReferenceContainer, void, void, void>::ClassMethodCallerT(MethodPointer method, uint32 mask) {
    pFun = method;
}

template<class className, typename MethodPointer>
ClassMethodCallerT<className, MethodPointer, ReferenceContainer, void, void, void>::~ClassMethodCallerT() {
}

template<class className, typename MethodPointer>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, ReferenceContainer, void, void, void>::Call(Object *object, StructuredDataI &parameters) {

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

template<class className, typename MethodPointer>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, ReferenceContainer, void, void, void>::Call(Object *object, ReferenceContainer &parameters) {
    className* actual = dynamic_cast<className *>(object);
    return (actual->*pFun)(parameters);
}

template<class className, typename MethodPointer>
ClassMethodCallerT<className, MethodPointer, StreamI, void, void, void>::ClassMethodCallerT(MethodPointer method, uint32 mask) {
    pFun = method;
}

template<class className, typename MethodPointer>
ClassMethodCallerT<className, MethodPointer, StreamI, void, void, void>::~ClassMethodCallerT() {
}

template<class className, typename MethodPointer>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, StreamI, void, void, void>::Call(Object *object, ReferenceContainer &parameters) {
    className* actual = dynamic_cast<className *>(object);

    ReferenceT<StreamI> param = parameters.Get(0);

    return (actual->*pFun)(*(param.operator->()));
}

template<class className, typename MethodPointer>
ErrorManagement::ErrorType ClassMethodCallerT<className, MethodPointer, StreamI, void, void, void>::Call(Object *object, StreamI &stream) {
    className* actual = dynamic_cast<className *>(object);

    return (actual->*pFun)(stream);
}

}

#endif /* CLASSMETHODCALLERT_H_ */

