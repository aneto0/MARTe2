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

#include "Object.h"
#include "ReferenceContainer.h"
#include "ClassMethodInterfaceMapper.h"
#include "ClassMethodsRegistryItem.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class ClassWithCallableMethods: public MARTe::Object {

public:

    CLASS_REGISTER_DECLARATION()

    ClassWithCallableMethods();

    virtual ~ClassWithCallableMethods();

    /**
     *
     */
    bool NonRegisteredMethod(MARTe::ReferenceContainer& data);

    /**
     * Method which simulates an error execution (i.e. returns false)
     */
    bool FaultyMethod(MARTe::ReferenceContainer& data);

    /**
     *
     */
    bool OverloadedMethod();

    /**
     *
     */
    bool OverloadedMethod(int& data);

    /**
     *
     */
    bool OverloadedMethod(MARTe::ReferenceContainer& data);

    /**
     * consumes input from a ReferenceContainer
     */
    bool MethodWithInputReferenceContainer(MARTe::ReferenceContainer& data);

    /**
     * produces output into a ReferenceContainer
     */
    bool MethodWithOutputReferenceContainer(MARTe::ReferenceContainer& data);

    /**
     * consumes/produces from/into a ReferenceContainer
     */
    bool MethodWithInputOutputReferenceContainer(MARTe::ReferenceContainer& data);

    /**
     * consumes input from an integer
     */
    bool MethodWithInputInteger(int& data);

    /**
     * produces output into an integer
     */
    bool MethodWithOutputInteger(int& data);

    /**
     * consumes/produces from/into an integer
     */
    bool MethodWithInputOutputInteger(int& data);

    /**
     * consumes input from an integer
     */
    bool MethodWithInputIntegerByCopy(int data);

    /**
     * consumes input from a ReferenceContainer
     */
    bool MethodWithInputReferenceContainerByCopy(MARTe::ReferenceContainer data);

    /**
     * Returns a string with the name and signature of the last executed method
     */
    MARTe::StreamString GetLastMethodExecuted();

private:

    MARTe::StreamString lastMethodExecuted;
};

CLASS_METHOD_REGISTER(ClassWithCallableMethods, &ClassWithCallableMethods::MethodWithInputInteger, &ClassWithCallableMethods::MethodWithOutputInteger, &ClassWithCallableMethods::MethodWithInputOutputInteger, &ClassWithCallableMethods::FaultyMethod, &ClassWithCallableMethods::MethodWithInputReferenceContainer, &ClassWithCallableMethods::MethodWithOutputReferenceContainer, &ClassWithCallableMethods::MethodWithInputOutputReferenceContainer, &ClassWithCallableMethods::MethodWithInputIntegerByCopy, &ClassWithCallableMethods::MethodWithInputReferenceContainerByCopy, (bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods::OverloadedMethod, (bool (ClassWithCallableMethods::*)(int&))&ClassWithCallableMethods::OverloadedMethod, (bool (ClassWithCallableMethods::*)(MARTe::ReferenceContainer&))&ClassWithCallableMethods::OverloadedMethod)

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSWITHCALLABLEMETHODS_H_ */

