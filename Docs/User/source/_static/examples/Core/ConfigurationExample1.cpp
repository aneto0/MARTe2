/**
 * @file ConfigurationExample1.cpp
 * @brief Source file for class ConfigurationExample1
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
 * the class ConfigurationExample1 (public, protected, and private). Be aware that some
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
        mode = "";
        referencesArray = NULL;
        modesArray = NULL;
    }

    virtual ~ControllerEx1 () {
        using namespace MARTe;
        if (referencesArray != NULL) {
            delete [] referencesArray;
        }
        if (modesArray != NULL) {
            delete [] modesArray;
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
        if (ok) {
            ok = data.Read("Gain1", gain1);
            if (ok) {
                REPORT_ERROR(ErrorManagement::Information, "gain1 set to %d", gain1);
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "The Gain1 property shall be set");
            }
        }
        if (ok) {
            ok = data.Read("Gain2", gain2);
            if (ok) {
                REPORT_ERROR(ErrorManagement::Information, "gain2 set to %f", gain2);
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "The Gain2 property shall be set");
            }
        }

        if (ok) {
            mode = "";
            ok = data.Read("Mode", mode);
            if (ok) {
                REPORT_ERROR(ErrorManagement::Information, "mode set to %s", mode.Buffer());
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "The Mode property shall be set");
            }
        }

        if (ok) {
            AnyType arrayDescription = data.GetType("References");
            ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
            uint32 numberOfElements = 0u;
            if (ok) {
                numberOfElements = arrayDescription.GetNumberOfElements(0u);
                ok = (numberOfElements > 0u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "No elements defined in the array");
                }
            }
            if (ok) {
                //Reconfiguration...
                if (referencesArray != NULL) {
                    delete [] referencesArray;
                }
                referencesArray = new int32[numberOfElements];
                Vector<int32> readVector(referencesArray, numberOfElements);
                ok = data.Read("References", readVector);
                if (ok) {
                    REPORT_ERROR(ErrorManagement::Information, "referencesArray set to %d", readVector);
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Could not read the References");
                }
            }
        }

        if (ok) {
            AnyType arrayDescription = data.GetType("Modes");
            ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
            uint32 numberOfElements = 0u;
            if (ok) {
                numberOfElements = arrayDescription.GetNumberOfElements(0u);
                ok = (numberOfElements > 0u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "No elements defined in the array");
                }
            }
            if (ok) {
                //Reconfiguration...
                if (modesArray != NULL) {
                    delete [] modesArray;
                }
                modesArray = new StreamString[numberOfElements];
                Vector<StreamString> readVector(modesArray, numberOfElements);
                ok = data.Read("Modes", readVector);
                if (ok) {
                    uint32 i;
                    for (i=0u; i<numberOfElements; i++) {
                        REPORT_ERROR(ErrorManagement::Information, "modesArray[%d] set to %s", i,readVector[i].Buffer());
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Could not read the Modes");
                }
            }
        }

        if (ok) {
            AnyType arrayDescription = data.GetType("Model");
            ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
            uint32 numberOfDimensions = 0u;
            if (ok) {
                numberOfDimensions = arrayDescription.GetNumberOfDimensions();
                ok = (numberOfDimensions == 2u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Model is not a Matrix");
                }
            }
            uint32 numberOfRows = 0u;
            uint32 numberOfCols = 0u;
            if (ok) {
                numberOfRows = arrayDescription.GetNumberOfElements(1u);
                numberOfCols = arrayDescription.GetNumberOfElements(0u);
                ok = (numberOfRows > 0u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "No rows defined in the matrix");
                }
            }
            if (ok) {
                ok = (numberOfCols > 0u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "No columns defined in the matrix");
                }
            }
            if (ok) {
                Matrix<float32> modelMatrix(numberOfRows, numberOfCols);
                ok = data.Read("Model", modelMatrix);
                if (ok) {
                    REPORT_ERROR(ErrorManagement::Information, "modelMatrix set to %f", modelMatrix);
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Could not read the References");
                }
            }
        }

        return ok;
    }

    /**
     * A list of properties.
     */
    MARTe::uint32 gain1;
    MARTe::float32 gain2;
    MARTe::StreamString mode;
    MARTe::int32 *referencesArray;
    MARTe::StreamString *modesArray;
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
            cdb.Write("Gain2", 1.5);
            cdb.Write("Mode", "ASTRING");
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
        //Write an invalid configuration
        {
            ConfigurationDatabase cdb;
            cdb.Write("Gain1", 2);
            cdb.Write("Gain2", 1.5);
            cdb.Write("Mode", "ASTRING");
            cdb.Write("Modes", stringArr);
            cdb.Write("References", int32Arr);
            cdb.Write("Model", int32Arr);
            if (!ref1->Initialise(cdb)) {
                REPORT_ERROR_STATIC(ErrorManagement::Information,
                        "As expected failed to reconfigure instance of %s",
                        ref1->GetName());
            }
        }
    }

    return 0;
}

