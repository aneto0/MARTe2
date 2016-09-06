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
     * @brief Method which has a StreamI has an input.
     */
    MARTe::ErrorManagement::ErrorType MethodWithConstInputStreamI(const MARTe::StreamI &data);

    /**
     * @brief Method which changes the input StreamI.
     */
    MARTe::ErrorManagement::ErrorType MethodWithOutputStreamI(MARTe::StreamI &data);

    /**
     * @brief Method which changes the input StreamI.
     */
    MARTe::ErrorManagement::ErrorType MethodWithInputOutputStreamI(MARTe::StreamI &data);

    /**
     * @brief Methods with four parameters on all possible r/w combinations.
     */
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_C_C_C(const MARTe::uint32 &param1, const MARTe::float32 &param2, const MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_C_C_R(const MARTe::uint32 &param1, const MARTe::float32 &param2, const MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_C_C_W(const MARTe::uint32 &param1, const MARTe::float32 &param2, const MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_C_R_C(const MARTe::uint32 &param1, const MARTe::float32 &param2, MARTe::float64 param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_C_R_R(const MARTe::uint32 &param1, const MARTe::float32 &param2, MARTe::float64 param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_C_R_W(const MARTe::uint32 &param1, const MARTe::float32 &param2, MARTe::float64 param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_C_W_C(const MARTe::uint32 &param1, const MARTe::float32 &param2, MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_C_W_R(const MARTe::uint32 &param1, const MARTe::float32 &param2, MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_C_W_W(const MARTe::uint32 &param1, const MARTe::float32 &param2, MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_R_C_C(const MARTe::uint32 &param1, MARTe::float32 param2, const MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_R_C_R(const MARTe::uint32 &param1, MARTe::float32 param2, const MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_R_C_W(const MARTe::uint32 &param1, MARTe::float32 param2, const MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_R_R_C(const MARTe::uint32 &param1, MARTe::float32 param2, MARTe::float64 param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_R_R_R(const MARTe::uint32 &param1, MARTe::float32 param2, MARTe::float64 param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_R_R_W(const MARTe::uint32 &param1, MARTe::float32 param2, MARTe::float64 param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_R_W_C(const MARTe::uint32 &param1, MARTe::float32 param2, MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_R_W_R(const MARTe::uint32 &param1, MARTe::float32 param2, MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_R_W_W(const MARTe::uint32 &param1, MARTe::float32 param2, MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_W_C_C(const MARTe::uint32 &param1, MARTe::float32 &param2, const MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_W_C_R(const MARTe::uint32 &param1, MARTe::float32 &param2, const MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_W_C_W(const MARTe::uint32 &param1, MARTe::float32 &param2, const MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_W_R_C(const MARTe::uint32 &param1, MARTe::float32 &param2, MARTe::float64 param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_W_R_R(const MARTe::uint32 &param1, MARTe::float32 &param2, MARTe::float64 param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_W_R_W(const MARTe::uint32 &param1, MARTe::float32 &param2, MARTe::float64 param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_W_W_C(const MARTe::uint32 &param1, MARTe::float32 &param2, MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_W_W_R(const MARTe::uint32 &param1, MARTe::float32 &param2, MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_C_W_W_W(const MARTe::uint32 &param1, MARTe::float32 &param2, MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_C_C_C(MARTe::uint32 param1, const MARTe::float32 &param2, const MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_C_C_R(MARTe::uint32 param1, const MARTe::float32 &param2, const MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_C_C_W(MARTe::uint32 param1, const MARTe::float32 &param2, const MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_C_R_C(MARTe::uint32 param1, const MARTe::float32 &param2, MARTe::float64 param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_C_R_R(MARTe::uint32 param1, const MARTe::float32 &param2, MARTe::float64 param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_C_R_W(MARTe::uint32 param1, const MARTe::float32 &param2, MARTe::float64 param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_C_W_C(MARTe::uint32 param1, const MARTe::float32 &param2, MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_C_W_R(MARTe::uint32 param1, const MARTe::float32 &param2, MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_C_W_W(MARTe::uint32 param1, const MARTe::float32 &param2, MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_R_C_C(MARTe::uint32 param1, MARTe::float32 param2, const MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_R_C_R(MARTe::uint32 param1, MARTe::float32 param2, const MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_R_C_W(MARTe::uint32 param1, MARTe::float32 param2, const MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_R_R_C(MARTe::uint32 param1, MARTe::float32 param2, MARTe::float64 param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_R_R_R(MARTe::uint32 param1, MARTe::float32 param2, MARTe::float64 param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_R_R_W(MARTe::uint32 param1, MARTe::float32 param2, MARTe::float64 param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_R_W_C(MARTe::uint32 param1, MARTe::float32 param2, MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_R_W_R(MARTe::uint32 param1, MARTe::float32 param2, MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_R_W_W(MARTe::uint32 param1, MARTe::float32 param2, MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_W_C_C(MARTe::uint32 param1, MARTe::float32 &param2, const MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_W_C_R(MARTe::uint32 param1, MARTe::float32 &param2, const MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_W_C_W(MARTe::uint32 param1, MARTe::float32 &param2, const MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_W_R_C(MARTe::uint32 param1, MARTe::float32 &param2, MARTe::float64 param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_W_R_R(MARTe::uint32 param1, MARTe::float32 &param2, MARTe::float64 param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_W_R_W(MARTe::uint32 param1, MARTe::float32 &param2, MARTe::float64 param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_W_W_C(MARTe::uint32 param1, MARTe::float32 &param2, MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_W_W_R(MARTe::uint32 param1, MARTe::float32 &param2, MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_R_W_W_W(MARTe::uint32 param1, MARTe::float32 &param2, MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_C_C_C(MARTe::uint32 &param1, const MARTe::float32 &param2, const MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_C_C_R(MARTe::uint32 &param1, const MARTe::float32 &param2, const MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_C_C_W(MARTe::uint32 &param1, const MARTe::float32 &param2, const MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_C_R_C(MARTe::uint32 &param1, const MARTe::float32 &param2, MARTe::float64 param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_C_R_R(MARTe::uint32 &param1, const MARTe::float32 &param2, MARTe::float64 param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_C_R_W(MARTe::uint32 &param1, const MARTe::float32 &param2, MARTe::float64 param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_C_W_C(MARTe::uint32 &param1, const MARTe::float32 &param2, MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_C_W_R(MARTe::uint32 &param1, const MARTe::float32 &param2, MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_C_W_W(MARTe::uint32 &param1, const MARTe::float32 &param2, MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_R_C_C(MARTe::uint32 &param1, MARTe::float32 param2, const MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_R_C_R(MARTe::uint32 &param1, MARTe::float32 param2, const MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_R_C_W(MARTe::uint32 &param1, MARTe::float32 param2, const MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_R_R_C(MARTe::uint32 &param1, MARTe::float32 param2, MARTe::float64 param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_R_R_R(MARTe::uint32 &param1, MARTe::float32 param2, MARTe::float64 param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_R_R_W(MARTe::uint32 &param1, MARTe::float32 param2, MARTe::float64 param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_R_W_C(MARTe::uint32 &param1, MARTe::float32 param2, MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_R_W_R(MARTe::uint32 &param1, MARTe::float32 param2, MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_R_W_W(MARTe::uint32 &param1, MARTe::float32 param2, MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_W_C_C(MARTe::uint32 &param1, MARTe::float32 &param2, const MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_W_C_R(MARTe::uint32 &param1, MARTe::float32 &param2, const MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_W_C_W(MARTe::uint32 &param1, MARTe::float32 &param2, const MARTe::float64 &param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_W_R_C(MARTe::uint32 &param1, MARTe::float32 &param2, MARTe::float64 param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_W_R_R(MARTe::uint32 &param1, MARTe::float32 &param2, MARTe::float64 param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_W_R_W(MARTe::uint32 &param1, MARTe::float32 &param2, MARTe::float64 param3, MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_W_W_C(MARTe::uint32 &param1, MARTe::float32 &param2, MARTe::float64 &param3, const MARTe::StreamString &param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_W_W_R(MARTe::uint32 &param1, MARTe::float32 &param2, MARTe::float64 &param3, MARTe::StreamString param4);
    MARTe::ErrorManagement::ErrorType MethodWithFourParameters_W_W_W_W(MARTe::uint32 &param1, MARTe::float32 &param2, MARTe::float64 &param3, MARTe::StreamString &param4);

    /**
     * @brief Method with four parameters which returns an error.
     */
    MARTe::ErrorManagement::ErrorType MethodWithFourParametersReturnError(MARTe::uint32 &param1, MARTe::float32 &param2, MARTe::float64 &param3, MARTe::StreamString &param4);

    /**
     * @brief Methods with three parameters on all possible r/w combinations.
     */
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_C_C_C(const MARTe::uint32 & param1, const MARTe::float32 & param2, const MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_C_C_R(const MARTe::uint32 & param1, const MARTe::float32 & param2, MARTe::StreamString param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_C_C_W(const MARTe::uint32 & param1, const MARTe::float32 & param2, MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_C_R_C(const MARTe::uint32 & param1, MARTe::float32 param2, const MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_C_R_R(const MARTe::uint32 & param1, MARTe::float32 param2, MARTe::StreamString param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_C_R_W(const MARTe::uint32 & param1, MARTe::float32 param2, MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_C_W_C(const MARTe::uint32 & param1, MARTe::float32 & param2, const MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_C_W_R(const MARTe::uint32 & param1, MARTe::float32 & param2, MARTe::StreamString param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_C_W_W(const MARTe::uint32 & param1, MARTe::float32 & param2, MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_R_C_C(MARTe::uint32 param1, const MARTe::float32 & param2, const MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_R_C_R(MARTe::uint32 param1, const MARTe::float32 & param2, MARTe::StreamString param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_R_C_W(MARTe::uint32 param1, const MARTe::float32 & param2, MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_R_R_C(MARTe::uint32 param1, MARTe::float32 param2, const MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_R_R_R(MARTe::uint32 param1, MARTe::float32 param2, MARTe::StreamString param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_R_R_W(MARTe::uint32 param1, MARTe::float32 param2, MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_R_W_C(MARTe::uint32 param1, MARTe::float32 & param2, const MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_R_W_R(MARTe::uint32 param1, MARTe::float32 & param2, MARTe::StreamString param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_R_W_W(MARTe::uint32 param1, MARTe::float32 & param2, MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_W_C_C(MARTe::uint32 & param1, const MARTe::float32 & param2, const MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_W_C_R(MARTe::uint32 & param1, const MARTe::float32 & param2, MARTe::StreamString param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_W_C_W(MARTe::uint32 & param1, const MARTe::float32 & param2, MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_W_R_C(MARTe::uint32 & param1, MARTe::float32 param2, const MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_W_R_R(MARTe::uint32 & param1, MARTe::float32 param2, MARTe::StreamString param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_W_R_W(MARTe::uint32 & param1, MARTe::float32 param2, MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_W_W_C(MARTe::uint32 & param1, MARTe::float32 & param2, const MARTe::StreamString & param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_W_W_R(MARTe::uint32 & param1, MARTe::float32 & param2, MARTe::StreamString param3);
    MARTe::ErrorManagement::ErrorType MethodWithThreeParameters_W_W_W(MARTe::uint32 & param1, MARTe::float32 & param2, MARTe::StreamString & param3);

    /**
     * @brief Method with three parameters which returns an error.
     */
    MARTe::ErrorManagement::ErrorType MethodWithThreeParametersReturnError(MARTe::uint32 &param1, MARTe::float32 &param2, MARTe::StreamString &param3);

    /**
     * @brief Methods with two parameters on all possible r/w combinations.
     */
    MARTe::ErrorManagement::ErrorType MethodWithTwoParameters_C_C(const MARTe::uint32 & param1, const MARTe::StreamString & param2);
    MARTe::ErrorManagement::ErrorType MethodWithTwoParameters_C_R(const MARTe::uint32 & param1, MARTe::StreamString param2);
    MARTe::ErrorManagement::ErrorType MethodWithTwoParameters_C_W(const MARTe::uint32 & param1, MARTe::StreamString & param2);
    MARTe::ErrorManagement::ErrorType MethodWithTwoParameters_R_C(MARTe::uint32 param1, const MARTe::StreamString & param2);
    MARTe::ErrorManagement::ErrorType MethodWithTwoParameters_R_R(MARTe::uint32 param1, MARTe::StreamString param2);
    MARTe::ErrorManagement::ErrorType MethodWithTwoParameters_R_W(MARTe::uint32 param1, MARTe::StreamString & param2);
    MARTe::ErrorManagement::ErrorType MethodWithTwoParameters_W_C(MARTe::uint32 & param1, const MARTe::StreamString & param2);
    MARTe::ErrorManagement::ErrorType MethodWithTwoParameters_W_R(MARTe::uint32 & param1, MARTe::StreamString param2);
    MARTe::ErrorManagement::ErrorType MethodWithTwoParameters_W_W(MARTe::uint32 & param1, MARTe::StreamString & param2);

    /**
     * @brief Method with two parameters which returns an error.
     */
    MARTe::ErrorManagement::ErrorType MethodWithTwoParametersReturnError(MARTe::uint32 &param1, MARTe::StreamString &param2);

    /**
     * @brief Methods with one parameters on all possible r/w combinations.
     */
    MARTe::ErrorManagement::ErrorType MethodWithOneParameter_C(const MARTe::uint32 & param1);
    MARTe::ErrorManagement::ErrorType MethodWithOneParameter_R( MARTe::uint32 param1);
    MARTe::ErrorManagement::ErrorType MethodWithOneParameter_W( MARTe::uint32 & param1);

    /**
     * @brief Method with two parameters which returns an error.
     */
    MARTe::ErrorManagement::ErrorType MethodWithOneParameterReturnError(MARTe::uint32 &param1);

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

