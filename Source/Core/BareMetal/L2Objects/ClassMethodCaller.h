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
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class Object;
class ReferenceContainer;
class StreamI;

/**
 * @brief Allows to call registered methods on registered objects (see ClassMethodInterfaceMapper and CLASS_METHOD_REGISTER).
 * @details The actual implementations of this class are in ClassMethodCallerT.
 */
/*lint -e{9109} forward declaration required to be able to Call the Object *.*/
class ClassMethodCaller {
public:

    /**
     * @brief Default constructor. NOOP.
     */
    ClassMethodCaller();

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodCaller();

    /**
     * @brief Calls the class method using a Stream as the source of the parameters
     * @param[in] object is the pointer to the object owning the method.
     * @param[in] stream a reference to a Stream that will be used to fill the parameters to call the functions.
     * @return
     * + ErrorManagement::ParametersError if no match between parameters provided and the function call is possible
     * + ErrorManagement::UnsupportedFeature if dynamic_cast to specialised class type is possible with provided argument object
     * + on success the error returned by the method called
     */
    virtual ErrorManagement::ErrorType Call(Object *object,StreamI &stream);

    /**
     * @brief Calls the class method by taking the arguments from StructuredDataI *parameters
     * @details Default implementation: The parameters ReferenceContainer is expected to have a valid StructuredDataI on its first entry.
     * @details The call then recurses and calls Call(object,StructuredDataI)
     * @param[in] object is the pointer to the object owning the method.
     * @param[in] parameters a reference to a StructuredDataI object where to read/write parameters/results.
     * @return
     * + ErrorManagement::ParametersError if no match between parameters provided and the function call is possible
     * + ErrorManagement::UnsupportedFeature if dynamic_cast to specialised class type is possible with provided argument object
     * + on success the error returned by the method called
     */
    virtual ErrorManagement::ErrorType Call(Object *object,StructuredDataI &parameters);

    /**
     * @brief Calls the class method by taking the arguments from the ReferenceContainer parameters
     * @param[in] object is the pointer to the object owning the method.
     * @param[in] parameters a reference to a ReferenceContainer that will be used to fill the parameters to call the functions
     * @return
     * + ErrorManagement::ParametersError if no match between parameters provided and the function call is possible
     * + ErrorManagement::UnsupportedFeature if dynamic_cast to specialised class type is possible with provided argument object
     * + on success the error returned by the method called
     */
    virtual ErrorManagement::ErrorType Call(Object *object,ReferenceContainer &parameters);

    /**
     * @brief Calls the class method without parameters
     * @param[in] object is the pointer to the object owning the method.
     * @return
     * + ErrorManagement::ParametersError if no match between parameters provided and the function call is possible
     * + ErrorManagement::UnsupportedFeature if dynamic_cast to specialised class type is possible with provided argument object
     * + on success the error returned by the method called
     */
    virtual ErrorManagement::ErrorType Call(Object *object);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe
#endif /* CLASSMETHODCALLER_H_ */
