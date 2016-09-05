/**
 * @file ClassWithCallableMethods.h
 * @brief Header file for class ClassWithCallableMethods
 * @date 24/05/2016
 * @author Ivan Herrero
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

 * @details This header file contains the declaration of the class ClassWithCallableMethods
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSWITHCALLABLEMETHODS_H_
#define CLASSWITHCALLABLEMETHODS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ClassMethodInterfaceMapper.h"
#include "ErrorType.h"
#include "Object.h"
#include "ReferenceContainer.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Registered class with registered methods meant to be used in unit
 * tests of the calling mechanism.
 */
class ClassWithCallableMethods: public MARTe::Object {

public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief DEfault constructor
     */
ClassWithCallableMethods    ();

    /**
     * @brief Destructor
     */
    virtual ~ClassWithCallableMethods();

    /**
     * @brief Method that is not going to be registered.
     */
    bool NonRegisteredMethod(MARTe::ReferenceContainer& data);

    /**
     * @brief Method which simulates an error execution (i.e. returns false)
     */
    MARTe::ErrorManagement::ErrorType FaultyMethod(MARTe::ReferenceContainer& data);

    /**
     * @brief Method with no input parameters.
     */
    MARTe::ErrorManagement::ErrorType MethodWithVoidParameters(void);

    /**
     * @brief Method which consumes input from an integer by reference.
     */
    MARTe::ErrorManagement::ErrorType MethodWithConstInputInteger(const MARTe::int32 &data);

    /**
     * @brief Method which produces output into an integer passed by reference.
     */
    MARTe::ErrorManagement::ErrorType MethodWithOutputInteger(MARTe::int32& data);

    /**
     * @brief Method which produces output into an integer passed by reference.
     */
    MARTe::ErrorManagement::ErrorType MethodWithInputOutputInteger(MARTe::int32& data);

    /**
     * @brief Method which consumes input from an integer passed by copy.
     */
    MARTe::ErrorManagement::ErrorType MethodWithInputIntegerByCopy(MARTe::int32 data);

    /**
     * @brief Method which has a StructuredDataI has an input.
     */
    MARTe::ErrorManagement::ErrorType MethodWithConstInputStructuredDataI(const MARTe::StructuredDataI &data);

    /**
     * @brief Method which changes the input StructuredDataI.
     */
    MARTe::ErrorManagement::ErrorType MethodWithOutputStructuredDataI(MARTe::StructuredDataI &data);

    /**
     * @brief Method which consumes input from a ReferenceContainer passed by
     * copy.
     */
    MARTe::ErrorManagement::ErrorType MethodWithInputReferenceContainerByCopy(MARTe::ReferenceContainer data);

    /**
     * @brief Method which consumes input from a ReferenceContainer and that expects to have a StructuredDataI at its 0 position
     */
    MARTe::ErrorManagement::ErrorType MethodWithInputReferenceContainerAndStructuredDataIAt0(MARTe::ReferenceContainer &data);

    /**
     * @brief Method which adds an StructuredDataI to data at position 0
     */
    MARTe::ErrorManagement::ErrorType MethodWithOutputReferenceContainerAndStructuredDataIAt0(MARTe::ReferenceContainer &data);

    /**
     * @brief Method which consumes input from a ReferenceContainer passed
     * by reference.
     */
    MARTe::ErrorManagement::ErrorType MethodWithConstInputReferenceContainer(const MARTe::ReferenceContainer& data);

    /**
     * @brief Method which produces output into a ReferenceContainer passed
     * by reference.
     */
    MARTe::ErrorManagement::ErrorType MethodWithOutputReferenceContainer(MARTe::ReferenceContainer& data);

    /**
     * @brief Method which consumes/produces from/into a ReferenceContainer
     * passed by reference.
     */
    MARTe::ErrorManagement::ErrorType MethodWithInputOutputReferenceContainer(MARTe::ReferenceContainer& data);

    /**
     * @brief Gets a string with the name and signature of the last executed
     * method.
     */
    MARTe::StreamString GetLastMethodExecuted();

private:

    /**
     * The name of the last executed method.
     */
    MARTe::StreamString lastMethodExecuted;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSWITHCALLABLEMETHODS_H_ */

