/**
 * @file ReferencesExample1.cpp
 * @brief Source file for class ReferencesExample1
 * @date 14/03/2018
 * @author Andre' Neto
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

 * @details This source file contains the definition of all the methods for
 * the class ReferencesExample1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ClassRegistryDatabase.h"
#include "ErrorLoggerExample.h"
#include "Object.h"
#include "Reference.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {
/**
 * @brief A simple MARTe::Object class will be automatically registered into the ClassRegistryDatabase.
 */
class ControllerEx1: public MARTe::Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
ControllerEx1    () {
        gain = 0u;
    }

    /**
     * A property.
     */
    MARTe::uint32 gain;
};

CLASS_REGISTER(ControllerEx1, "")
}

int main(int argc, char **argv) {
    using namespace MARTe;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    CCString className = "ControllerEx1";
    //Automatically generate a new object instance based on the class name and on the correct Heap
    Reference ref1(className, GlobalObjectsDatabase::Instance()->GetStandardHeap());
    Reference ref2(className, GlobalObjectsDatabase::Instance()->GetStandardHeap());
    Reference ref3;

    //ref3 is not initialised and thus should not be valid
    if (ref1.IsValid() && ref2.IsValid() && !ref3.IsValid()) {
        ref1->SetName("ControllerInstance1");
        ref2->SetName("ControllerInstance2");
        //ref1 and ref2 are referencing two different object instances of the same class
        Object *referencedObj1 = ref1.operator ->();
        Object *referencedObj2 = ref2.operator ->();

        if (referencedObj1 != referencedObj2) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "As expected ref1 and ref2 are NOT pointing at the same instance (%s != %s).", ref1->GetName(), ref2->GetName());
        }

        ref3 = ref1;
        //ref3 is now valid and should point at the same object as ref1
        Object *referencedObj3 = ref3.operator ->();
        if (referencedObj1 == referencedObj3) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "As expected ref1 and ref3 are pointing at the same instance (%s == %s).", ref1->GetName(), ref3->GetName());
        }
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "One of the references is not correct.");
    }

    return 0;
}

