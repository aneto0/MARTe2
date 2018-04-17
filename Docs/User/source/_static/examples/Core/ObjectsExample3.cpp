/**
 * @file ObjectsExample2.cpp
 * @brief Source file for class ObjectsExample2
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
 * the class ObjectsExample2 (public, protected, and private). Be aware that some
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
#include "IntrospectionT.h"
#include "IntrospectionEntry.h"
#include "Object.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {
/**
 * @brief A MARTe::Object class will be automatically registered
 * into the ClassRegistryDatabase.
 */
class ControllerEx1: public MARTe::Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
ControllerEx1    () {
        gain = 0xffu;
    }

    /**
     * A property.
     */
    MARTe::uint32 gain;

    /**
     * Another property.
     */
    MARTe::float32 reference[6];
};

CLASS_REGISTER(ControllerEx1, "")
//Register the member gain
DECLARE_CLASS_MEMBER(ControllerEx1, gain, uint32, "", "");
DECLARE_CLASS_MEMBER(ControllerEx1, reference, float32, "[6]", "");
//The array members must follow the naming convention CLASSNAME_MEMBERNAME_introspectionEntry
static const MARTe::IntrospectionEntry* ControllerEx1ClassEntries[] = { &ControllerEx1_gain_introspectionEntry,
        &ControllerEx1_reference_introspectionEntry, 0 };
//Finally declare the class as introspectable
DECLARE_CLASS_INTROSPECTION(ControllerEx1, ControllerEx1ClassEntries)

//An introspectable structure
struct A {
    MARTe::float32 f1;
    MARTe::float32 f2;
};
struct B {
    struct A a1;
    struct A a2;
};
//The strategy is identical to the class registration
DECLARE_CLASS_MEMBER(A, f1, float32, "", "");
DECLARE_CLASS_MEMBER(A, f2, float32, "", "");
static const MARTe::IntrospectionEntry* AStructEntries[] = { &A_f1_introspectionEntry, &A_f2_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(A, AStructEntries)
DECLARE_CLASS_MEMBER(B, a1, A, "", "");
DECLARE_CLASS_MEMBER(B, a2, A, "", "");
static const MARTe::IntrospectionEntry* BStructEntries[] = { &B_a1_introspectionEntry, &B_a2_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(B, BStructEntries)

}

static void PrintIntrospection(const MARTe::char8 * const structOrClassToSearch) {
    using namespace MARTe;

    ClassRegistryDatabase *crdSingleton = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *classRegistryItem = crdSingleton->Find(structOrClassToSearch);
    if (classRegistryItem != NULL) {
        //Get the object builder (which knows how to build classes of this type).
        const Introspection *introspection = classRegistryItem->GetIntrospection();
        //Print all the available information.
        if (introspection != NULL) {
            uint32 numberOfMembers = introspection->GetNumberOfMembers();
            uint32 n;
            REPORT_ERROR_STATIC(ErrorManagement::Information, "[%s] number of members: %d:", structOrClassToSearch, numberOfMembers);
            for (n = 0u; n < numberOfMembers; n++) {
                const IntrospectionEntry entry = introspection->operator [](n);
                const char8 * const memberName = entry.GetMemberName();
                const char8 * const memberType = entry.GetMemberTypeName();
                const char8 * const memberModifiers = entry.GetMemberModifiers();
                const char8 * const memberAttributes = entry.GetMemberAttributes();
                REPORT_ERROR_STATIC(ErrorManagement::Information, "[%d] [name]: %s [type]: %s [mods]: %s [attrs]: %s", n, memberName, memberType, memberModifiers, memberAttributes);
            }
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "No introspection available for struct or class "
                    "with name: %s", structOrClassToSearch);
        }
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Could not find struct or class "
                "with name: %s", structOrClassToSearch);
    }
}

int main(int argc, char **argv) {
    using namespace MARTe;
    using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    PrintIntrospection("ControllerEx1");
    PrintIntrospection("A");
    PrintIntrospection("B");

    return 0;
}

