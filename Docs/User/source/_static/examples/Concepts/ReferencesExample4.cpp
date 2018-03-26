/**
 * @file Playground.cpp
 * @brief Source file for class Playground
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
 * the class Playground (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ClassRegistryDatabase.h"
#include "Object.h"
#include "Reference.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

void ErrorProcessFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo, const char * const errorDescription) {
    using namespace MARTe;
    const char8 * RED = "\x1B[31m";
    const char8 * GRN = "\x1B[32m";
    const char8 * RST = "\x1B[0m";

    StreamString errorCodeStr;
    ErrorManagement::ErrorCodeToStream(errorInfo.header.errorType, errorCodeStr);
    if (errorInfo.header.errorType == ErrorManagement::Information) {
        printf(GRN);
    }
    else {
        printf(RED);
    }
    printf("[%s - %s:%d]: %s\n", errorCodeStr.Buffer(), errorInfo.fileName, errorInfo.header.lineNumber, errorDescription);
    printf(RST);
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {

class ControllerEx1: public MARTe::Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
ControllerEx1    () {
        gain = 0u;
    }

    virtual ~ControllerEx1() {
        using namespace MARTe;
        if (GetName() != NULL) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "No more references pointing at %s [%s]. The Object will be safely deleted.", GetName(), GetClassProperties()->GetName());
        }
    }

    virtual void AFunction () {
        using namespace MARTe;
        REPORT_ERROR_STATIC(ErrorManagement::Information, "AFunction called @ %s.", GetName());
    }

    /**
     * A property.
     */
    MARTe::uint32 gain;
};

CLASS_REGISTER(ControllerEx1, "")

class PIDEx1: public ControllerEx1 {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
PIDEx1    () {
        property3 = 0u;
    }

    virtual ~PIDEx1() {
        using namespace MARTe;
        REPORT_ERROR_STATIC(ErrorManagement::Information, "No more references pointing at %s [%s]. The Object will be safely deleted.", GetName(), GetClassProperties()->GetName());
    }

    virtual void AFunction () {
        using namespace MARTe;
        REPORT_ERROR_STATIC(ErrorManagement::Information, "AFunction called @ %s.", GetName());
    }
    /**
     * A property.
     */
    MARTe::uint32 property3;
};

CLASS_REGISTER(PIDEx1, "")
}

//Note that rc is passed by value
void ListReferenceContainerContents(MARTe::ReferenceContainer rc) {
    using namespace MARTe;

    uint32 i;
    uint32 size = rc.Size();
    for (i = 0u; i < size; i++) {
        Reference r = rc.Get(i);
        REPORT_ERROR_STATIC(ErrorManagement::Information, "@Function ReferenceContainer[%d] = %s", i, r->GetName());
    }
    Reference r = rc.Get(0);
    rc.Delete(r);
    size = rc.Size();
    for (i = 0u; i < size; i++) {
        Reference r = rc.Get(i);
        REPORT_ERROR_STATIC(ErrorManagement::Information, "@Function after Delete ReferenceContainer[%d] = %s", i, r->GetName());
    }
}

int main(int argc, char **argv) {
    using namespace MARTe;
    using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessFunction);

    CCString className1 = "ControllerEx1";
    CCString className2 = "PIDEx1";

    //Automatically generate a new object instance based on the class name and on the correct Heap and with the template reference.
    ReferenceT<ControllerEx1> ref1(className1, GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (ref1.IsValid()) {
        ref1->SetName("ControllerInstance1");
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully created an instance of %s", className1.GetList());
    }
    ReferenceT<ControllerEx1> ref2(className2, GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (ref2.IsValid()) {
        ref2->SetName("ControllerInstance2");
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully created an instance of %s", className1.GetList());
    }
    //This mechanism also works with compatible subclasses PIDEx1->ControllerEx1
    ReferenceT<PIDEx1> ref3(className2, GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (ref3.IsValid()) {
        ref3->SetName("PIDInstance1");
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully created an instance of %s", className2.GetList());
    }

    ReferenceContainer container;
    container.Insert(ref1);
    container.Insert(ref2);
    container.Insert(ref3);

    uint32 i;
    uint32 size = container.Size();
    for (i = 0u; i < size; i++) {
        Reference r = container.Get(i);
        REPORT_ERROR_STATIC(ErrorManagement::Information, "ReferenceContainer[%d] = %s", i, r->GetName());
    }
    ListReferenceContainerContents(container);
    size = container.Size();
    for (i = 0u; i < size; i++) {
        Reference r = container.Get(i);
        REPORT_ERROR_STATIC(ErrorManagement::Information, "After function call: ReferenceContainer[%d] = %s", i, r->GetName());
    }
    return 0;
}

