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
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class Object;
class ReferenceContainer;

/**
 * @brief Calls a class method
 * @details All the prototypes of the callable functions must be declared here.
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
     * @return ErrorManagement::FatalError if the registered function returns false, ErrorManagement::NoError if it returns true.
     */
    virtual ErrorManagement::ErrorType Call(Object * const context);

    /**
     * @brief Calls a class method with an integer in input.
     * @param[in] context is the objects owning the method.
     * @param[in, out] x is the integer in input.
     * @return ErrorManagement::FatalError if the registered function returns false, ErrorManagement::NoError if it returns true.
     */
    virtual ErrorManagement::ErrorType Call(Object * const context,
                            int32 &x);

    /**
     * @brief Calls a class method with a ReferenceContainer in input.
     * @param[in] context is the objects owning the method.
     * @param[in, out] x is the ReferenceContainer in input.
     * @return ErrorManagement::FatalError if the registered function returns false, ErrorManagement::NoError if it returns true.
     */
    virtual ErrorManagement::ErrorType Call(Object * const context,
                            ReferenceContainer & x);

};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSMETHODCALLER_H_ */

