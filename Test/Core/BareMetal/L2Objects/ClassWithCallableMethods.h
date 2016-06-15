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

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class ClassWithCallableMethods: public MARTe::Object {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API ClassWithCallableMethods)

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
    bool MethodK(MARTe::ReferenceContainer& data);

    /**
     * consumes input from a ReferenceContainer
     */
    bool MethodX(MARTe::ReferenceContainer& data);
    bool MethodX();

    /**
     * produces output into a ReferenceContainer
     */
    bool MethodY(MARTe::ReferenceContainer& data);

    /**
     * consumes/produces from/into a ReferenceContainer
     */
    bool MethodZ(MARTe::ReferenceContainer& data);

    /**
     * consumes an integer
     */
    bool MethodM(int data);
};

CLASS_METHOD_REGISTER(ClassWithCallableMethods, &ClassWithCallableMethods::MethodK, &ClassWithCallableMethods::MethodX, &ClassWithCallableMethods::MethodY, &ClassWithCallableMethods::MethodZ)

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSWITHCALLABLEMETHODS_H_ */

