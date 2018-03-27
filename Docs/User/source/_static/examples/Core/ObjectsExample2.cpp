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
 * @brief A simple MARTe::Object class will be automatically registered into the ClassRegistryDatabase.
 */
class ControllerEx1: public MARTe::Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
    ControllerEx1() {
        gain = 0xffu;
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
    using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    //Find the ControllerEx1 class
    ClassRegistryDatabase *crdSingleton = ClassRegistryDatabase::Instance();
    CCString classNameToSearch = "ControllerEx1";
    const ClassRegistryItem *classRegistryItem = crdSingleton->Find(classNameToSearch);
    if (classRegistryItem != NULL) {
        //Get the object builder (which knows how to build classes of this type).
        const ObjectBuilder *builder = classRegistryItem->GetObjectBuilder();
        if (builder != NULL) {
            //Build the class using the specified memory heap
            Object *obj = builder->Build(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            if (obj != NULL) {
                ControllerEx1 *obj2 = dynamic_cast<ControllerEx1 *>(obj);
                if (obj2 != NULL) {
                    obj2->SetName("ControllerInstance1");
                    REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully managed to create an instance of: %s and to set its name to %s", classNameToSearch.GetList(), obj2->GetName());
                }
            }
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Could not find the ObjectBuilder for the class with name: %s", classNameToSearch.GetList());
        }
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Could not find class with name: %s", classNameToSearch.GetList());
    }

    return 0;
}

