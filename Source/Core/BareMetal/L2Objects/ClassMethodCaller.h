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
 * @brief Class method caller
 *
 * @details A class method caller is an object whose purpose is to call a
 * specific method of another object. In order to make a call, a class method
 * caller needs:
 * + A pointer to the target object, known as the context.
 * + A pointer to the class method of the target object.
 * + A list of the arguments to be passed to the target method.
 *
 * This class supplies a list of "Call" methods, which acts as a list of valid
 * prototypes for the accepted class methods on the target object. Each one of
 * these "Call" methods expects a target object (the context) and the arguments
 * to be passed to the class method of the target object. Note: The default
 * implementation of these methods return ErrorManagement::UnsupportedFeature.
 *
 * It is intended that each instance of the class is going to be linked to a
 * specific method of target object's class, but this is left to its derived
 * classes (e.g. passing to it through the constructor of a derived class).
 */
class ClassMethodCaller {

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
     * @param[in] context is the objects owning the method.
     * @return ErrorManagement::FatalError if the registered function returns false, ErrorManagement::NoError if it returns true, or ErrorManagement::UnsupportedFeature if the call is not registered.
     */
    virtual ErrorManagement::ErrorType Call(Object * const context);

    /**
     * @brief Calls a class method with an integer in input passed by reference.
     * @param[in] context is the objects owning the method.
     * @param[in, out] x is the integer in input.
     * @return ErrorManagement::FatalError if the registered function returns false, ErrorManagement::NoError if it returns true, or ErrorManagement::UnsupportedFeature if the call is not registered..
     */
    virtual ErrorManagement::ErrorType Call(Object * const context,
                                            int32 &x);

    /**
     * @brief Calls a class method with a ReferenceContainer in input passed by reference.
     * @param[in] context is the objects owning the method.
     * @param[in, out] x is the ReferenceContainer in input.
     * @return ErrorManagement::FatalError if the registered function returns false, ErrorManagement::NoError if it returns true, or ErrorManagement::UnsupportedFeature if the call is not registered..
     */
    virtual ErrorManagement::ErrorType Call(Object * const context,
                                            ReferenceContainer & x);

    /**
     * @brief Calls a class method with an integer in input passed by copy.
     * @param[in] context is the objects owning the method.
     * @param[in, out] x is the integer in input.
     * @return ErrorManagement::FatalError if the registered function returns false, ErrorManagement::NoError if it returns true, or ErrorManagement::UnsupportedFeature if the call is not registered..
     */
    virtual ErrorManagement::ErrorType Call(Object * const context,
                                            int32 x,
                                            bool byCopy);
    /**
     * @brief Calls a class method with a ReferenceContainer in input passed by copy.
     * @param[in] context is the objects owning the method.
     * @param[in, out] x is the ReferenceContainer in input.
     * @return ErrorManagement::FatalError if the registered function returns false, ErrorManagement::NoError if it returns true, or ErrorManagement::UnsupportedFeature if the call is not registered..
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
