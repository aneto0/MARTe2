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
 *
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
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class Object;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Implementation of an object which stores and calls a class method.
 * @details The class allows to create an implicit class method caller whose
 * actual target method is inferred from the constructor (this class has 3
 * templated constructors, which allow to bind the each instance with a target
 * method). The target method can have a prototype without arguments or with
 * one argument at most (with by reference and by copy versions).
 */
class DLL_API ClassMethodInterfaceMapper {

public:

    /**
     * @brief Default constructor.
     */
    ClassMethodInterfaceMapper();

    /**
     * @brief Constructor by a class method with no arguments.
     * @tparam C is the class name.
     */
    template<typename C>
    ClassMethodInterfaceMapper(bool (C::*f)());

    /**
     * @brief Constructor by a class method with one argument passed by copy.
     * @param[in] f is a pointer to the class method to be registered.
     * @tparam C is the class name.
     * @tparam T is the type name of the class method argument.
     */
    template<typename C, typename T>
    ClassMethodInterfaceMapper(bool (C::*f)(T));

    /**
     * @brief Constructor by a class method with one argument passed by
     * reference.
     * @param[in] f is a pointer to the class method to be registered.
     * @tparam C is the class name.
     * @tparam T is the type name of the class method argument.
     */
    template<typename C, typename T>
    ClassMethodInterfaceMapper(bool (C::*f)(T&));

    /**
     * @brief Destructor
     */
    virtual ~ClassMethodInterfaceMapper();

    /**
     * @brief Calls the function with no arguments.
     * @param[in] context is the object which must call the function.
     * @return
     * + ErrorManagement::UnsupportedFeature if no function has been registered
     * + ErrorManagement::FatalError if the class method returns false
     * + ErrorManagement::NoError if it returns true.
     */
    ErrorManagement::ErrorType Call(Object * const context);

    /**
     * @brief Calls the function with one argument.
     * @tparam T is the type name of the class method argument.
     * @param[in] context is the object which must call the function.
     * @param[in,out] ref is the class method argument.
     * @return
     * + ErrorManagement::UnsupportedFeature if no function has been registered
     * + ErrorManagement::FatalError if the class method returns false
     * + ErrorManagement::NoError if it returns true.
     */
    template<typename T>
    ErrorManagement::ErrorType Call(Object * const context,
                                    T ref);

private:

    /**
     * The class method caller
     */
    ClassMethodCaller *caller;

    /**
     * Specify if the method argument is passed by copy
     */
    bool byCopy;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename className>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(bool (className::*f)()) {
    caller = new ClassMethodCallerT<className>(f);
    byCopy = false;
}

template<typename className, typename T>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(bool (className::*f)(T)) {
    caller = new ClassMethodCallerT<className, T>(f);
    byCopy = true;
}

template<typename className, typename T>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(bool (className::*f)(T&)) {
    caller = new ClassMethodCallerT<className, T&>(f);
    byCopy = false;
}

template<typename T>
ErrorManagement::ErrorType ClassMethodInterfaceMapper::Call(Object *context,
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

}

#endif /* CLASSMETHODINTERFACEMAPPER_H_ */

