/**
 * @file ConfigurationExample8.cpp
 * @brief Source file for class ConfigurationExample8
 * @date 27/06/2022
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
 * the class ConfigurationExample8 (public, protected, and private). Be aware that some
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
#include "ConfigurationDatabase.h"
#include "ErrorLoggerExample.h"
#include "Matrix.h"
#include "Object.h"
#include "Reference.h"
#include "ReferenceT.h"
#include "StreamString.h"
#include "StructuredDataIHelper.h"
#include "Vector.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {
/**
 * @brief A MARTe::Object class will be automatically registered into the ClassRegistryDatabase.
 */
class ControllerEx1: public MARTe::Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
    ControllerEx1 () {
        gain1 = 0u;
        gain2 = 0.f;
        gain3 = -1.0f;
        controllerType = 0u;
        mode = "";
        referencesArray = NULL;
        modesArray = NULL;
        model = NULL;
        modelRows = 0u;
    }

    virtual ~ControllerEx1 () {
        using namespace MARTe;
        if (referencesArray != NULL) {
            delete [] referencesArray;
        }
        if (modesArray != NULL) {
            delete [] modesArray;
        }
        if (model != NULL) {
            for (uint32 i=0; i<modelRows; i++) {
                delete [] model[i];
            }
            delete [] model;
        }

        if (GetName() != NULL) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "No more references pointing at %s [%s]. "
                    "The Object will be safely deleted.", GetName(), GetClassProperties()->GetName());
        }
    }

    /**
     * Read all the properties
     */
    virtual bool Initialise(MARTe::StructuredDataI &data) {
        using namespace MARTe;
        bool ok = Object::Initialise(data);
        StructuredDataIHelper helper(data, this);
        if (ok) {
            ok = helper.Read("Gain1", gain1);
        }
        if (ok) {
            //Read with default
            ok = helper.Read("Gain2", gain2, 1.7);
        }
        if (ok) {
            ok = helper.Read("Mode", mode);
        }
        if (ok) {
            const char8 * controllerTypesStr[] = {"C1", "C2", "C3"};
            uint32 controllerTypes[] = {1, 2, 3};
            ok = helper.ReadEnum("ControllerType", controllerType, controllerTypesStr, controllerTypes);
        }

        if (ok) {
            uint32 nOfReferences = 0u;
            //Reconfiguration...
            if (referencesArray != NULL) {
                delete [] referencesArray;
                referencesArray = NULL;
            }
            ok = helper.ReadArray("References", referencesArray, nOfReferences);
        }

        if (ok) {
            uint32 nOfModes = 0u;
            //Reconfiguration...
            if (modesArray != NULL) {
                delete [] modesArray;
                modesArray = NULL;
            }
            ok = helper.ReadArray("Modes", modesArray, nOfModes);
        }

        if (ok) {
            //Reconfiguration...
            if (model != NULL) {
                for (uint32 i=0; i<modelRows; i++) {
                    delete [] model[i];
                }
                delete [] model;
                model = NULL;
            }

            uint32 modelCols = 0u;
            ok = helper.ReadMatrix("Model", model, modelRows, modelCols);
        }

        if (ok) {
            ok = helper.ReadValidated("Gain3", gain3, "(Gain3 > (float32)(-3.0)) && (Gain3 <= (float32)(0.0))");
        }

        return ok;
    }

    /**
     * A list of properties.
     */
    MARTe::uint32 gain1;
    MARTe::uint32 modelRows;
    MARTe::uint32 controllerType;
    MARTe::float32 gain2;
    MARTe::float32 gain3;
    MARTe::StreamString mode;
    MARTe::int32 *referencesArray;
    MARTe::StreamString *modesArray;
    MARTe::float32 **model;
};

CLASS_REGISTER(ControllerEx1, "")

}

int main(int argc, char **argv) {
    using namespace MARTe;
    using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    CCString className1 = "ControllerEx1";

    int32 int32Arr[] = { -1, 2, -3, 4, -5 };
    const char8 *stringArr[] = { "A", "BB", "CCC", "DDDD", "EEEEE" };
    float32 float32Mat[3][2] = { { -1.0, 2.3 }, { 4.7, -3.2 }, { -7.1, 5.6 } };

    ReferenceT<ControllerEx1> ref1(className1,
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    //Automatically generate a new object instance based on the class name and on the correct Heap
    //and with the template reference.
    if (ref1.IsValid()) {
        ref1->SetName("ControllerInstance1");
        REPORT_ERROR_STATIC(ErrorManagement::Information,
                "Successfully created an instance of %s", className1.GetList());
        //Write a valid configuration.
        {
            ConfigurationDatabase cdb;
            cdb.Write("Gain1", 2);
            cdb.Write("Gain3", -2.0);
            cdb.Write("Mode", "ASTRING");
            cdb.Write("ControllerType", "C3");
            cdb.Write("Modes", stringArr);
            cdb.Write("References", int32Arr);
            cdb.Write("Model", float32Mat);

            if (ref1->Initialise(cdb)) {
                REPORT_ERROR_STATIC(ErrorManagement::Information,
                        "Successfully configured instance of %s",
                        ref1->GetName());
            } else {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                        "Failed to configure instance of %s", ref1->GetName());
            }
        }
        //Write an invalid configuration - missing compulsory parameter
        {
            ConfigurationDatabase cdb;
            cdb.Write("Gain1", 2);
            cdb.Write("Gain2", 1.5);
            cdb.Write("Gain3", -2.0);
            cdb.Write("ControllerType", "C3");
            cdb.Write("Modes", stringArr);
            cdb.Write("References", int32Arr);
            cdb.Write("Model", float32Mat);
            if (!ref1->Initialise(cdb)) {
                REPORT_ERROR_STATIC(ErrorManagement::Information,
                        "As expected failed to reconfigure instance of %s",
                        ref1->GetName());
            }
        }
        //Write a configuration without a parameter that is out of range
        {
            ConfigurationDatabase cdb;
            cdb.Write("Gain1", 2);
            cdb.Write("Gain2", 1.5);
            cdb.Write("Gain3", 2.0);
            cdb.Write("ControllerType", "C3");
            cdb.Write("Mode", "ASTRING");
            cdb.Write("Modes", stringArr);
            cdb.Write("References", int32Arr);
            cdb.Write("Model", float32Mat);
            if (!ref1->Initialise(cdb)) {
                REPORT_ERROR_STATIC(ErrorManagement::Information,
                        "As expected failed to reconfigure instance of %s",
                        ref1->GetName());
            }
        }

    }

    return 0;
}

