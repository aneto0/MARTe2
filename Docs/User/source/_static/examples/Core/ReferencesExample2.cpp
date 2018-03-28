/**
 * @file ReferencesExample2.cpp
 * @brief Source file for class ReferencesExample2
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
 * the class ReferencesExample2 (public, protected, and private). Be aware that some
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
 * @brief A simple MARTe::Object class will be automatically registered
 * into the ClassRegistryDatabase.
 */
class ControllerEx1: public MARTe::Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
    ControllerEx1() {
        gain = 0u;
    }

    virtual ~ControllerEx1() {
        using namespace MARTe;
        if (GetName() != NULL) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "No more "
            		"references pointing at %s [%s]. The Object will be "
            		"safely deleted.", GetName(), GetClassProperties()->GetName());
        }
    }

    /**
     * A property.
     */
    MARTe::uint32 gain;
};

CLASS_REGISTER(ControllerEx1, "")
}

void Function (MARTe::Reference ref) {
    using namespace MARTe;
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Number of references "
    		"pointing at %s is %d", ref->GetName(), ref.NumberOfReferences());

    CCString className = "ControllerEx1";
    //Automatically generate a new object instance based on the class name and on the correct Heap
    Reference ref2(className, GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ref2->SetName("ControllerInstance2(FunctionMember)");
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Number of references "
    		"pointing at %s is %d", ref2->GetName(), ref2.NumberOfReferences());
}

int main(int argc, char **argv) {
    using namespace MARTe;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    CCString className = "ControllerEx1";
    //Automatically generate a new object instance based on the class name and on the correct Heap
    Reference ref1(className, GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ref1->SetName("ControllerInstance1");
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Number of references "
    		"pointing at %s is %d", ref1->GetName(), ref1.NumberOfReferences());
    Reference ref2 = ref1;
    Reference ref3 = ref1;
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Number of references "
    		"pointing at %s is %d", ref1->GetName(), ref1.NumberOfReferences());
    Function(ref3);
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Number of references "
    		"pointing at %s is %d", ref1->GetName(), ref1.NumberOfReferences());

    return 0;
}

