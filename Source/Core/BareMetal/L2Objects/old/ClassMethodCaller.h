/**
 * @file ClassMethodCaller.h
 * @brief Header file for class ClassMethodCaller
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
 * @details This header file contains the declaration of the class ClassMethodCaller
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSMETHODCALLER_H_
#define CLASSMETHODCALLER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorType.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class Object;
class ReferenceContainer;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief This class represents an abstract class method caller.
 *
 * @details A class method caller is an object whose purpose is to call a
 * specific method of another object. In order to make a call, a class method
 * caller needs:
 * + A pointer to the context, also known as the target object.
 * + A pointer to the class method of the target object, also known as the
 * target method.
 * + A list of the arguments to be passed to the target method.
 *
 * This class is meant as a supplier of "Call" methods prototypes, defining
 * a closed list of callable methods for any context. These "Call" methods
 * always receive a context and the arguments to be passed to the target
 * method. If a target method does not match any of the defined "Call"
 * prototypes, then a new "Call" method must be added to the class. It must
 * be noted, too, that this class does not hold a pointer to the target
 * method, because this is left to the classes that will derive from it.
 *
 * @warning This class must be considered as a pure abstract class, i.e. an
 * interface, although it does not declare any of its methods as pure virtual.
 * Instead, it implements all the methods forcing them to return a default
 * value (UnsupportedFeature). The reason to this is that derived classes are
 * expected to implement only one of the "Call" methods, being this a way for
 * getting a default null implementation for the others.
 */
class DLL_API ClassMethodCaller {

public:

    /**
     * @brief Default Constructor.
     */
    ClassMethodCaller();

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCaller();

    /**
     * @brief Calls a class method without parameters.
     * @param[in] context is the pointer to the object owning the method.
     * @return
     * + ErrorManagement::fatalError if the registered function returns false
     * + ErrorManagement::noError if it returns true
     * + ErrorManagement::unsupportedFeature if the call is not registered
     * @warning The object pointed by context could be modified by the actual
     * class method called.
     */
    virtual ErrorManagement::ErrorType Call(Object* const context);

    /**
     * @brief Calls a class method with an integer in input passed by reference.
     * @param[in] context is the pointer to the object owning the method.
     * @param[in, out] x is the integer in input.
     * @return
     * + ErrorManagement::fatalError if the registered function returns false
     * + ErrorManagement::noError if it returns true
     * + ErrorManagement::unsupportedFeature if the call is not registered
     * @warning The object pointed by context could be modified by the actual
     * class method called.
     */
    virtual ErrorManagement::ErrorType Call(Object* const context,
                                            int32& x);

    /**
     * @brief Calls a class method with a ReferenceContainer in input passed by reference.
     * @param[in] context is the pointer to the object owning the method.
     * @param[in, out] x is the ReferenceContainer in input.
     * @return
     * + ErrorManagement::fatalError if the registered function returns false
     * + ErrorManagement::noError if it returns true
     * + ErrorManagement::unsupportedFeature if the call is not registered
     * @warning The object pointed by context could be modified by the actual
     * class method called.
     */
    virtual ErrorManagement::ErrorType Call(Object* const context,
                                            ReferenceContainer& x);

    /**
     * @brief Calls a class method with an integer in input passed by copy.
     * @param[in] context is the pointer to the object owning the method.
     * @param[in] x is the integer in input.
     * @param[in] byCopy states if the x argument is actually passed by copy
     * (meaningful when used from templates, helping on overloading resolution)
     * @return
     * + ErrorManagement::fatalError if the registered function returns false
     * + ErrorManagement::noError if it returns true
     * + ErrorManagement::unsupportedFeature if the call is not registered
     * @warning The object pointed by context could be modified by the actual
     * class method called.
     */
    virtual ErrorManagement::ErrorType Call(Object* const context,
                                            int32 x,
                                            bool byCopy);

    /**
     * @brief Calls a class method with a ReferenceContainer in input passed by copy.
     * @param[in] context is the pointer to the object owning the method.
     * @param[in] x is the ReferenceContainer in input.
     * @param[in] byCopy states if the x argument is actually passed by copy
     * (meaningful when used from templates, helping on overloading resolution)
     * @return
     * + ErrorManagement::fatalError if the registered function returns false
     * + ErrorManagement::noError if it returns true
     * + ErrorManagement::unsupportedFeature if the call is not registered
     * @warning The object pointed by context could be modified by the actual
     * class method called.
     */
    virtual ErrorManagement::ErrorType Call(Object * const context,
                                            ReferenceContainer x,
                                            bool byCopy);
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSMETHODCALLER_H_ */
