/**
 * @file ConfigurationExample4.cpp
 * @brief Source file for class ConfigurationExample4
 * @date 08/04/2018
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
 * the class ConfigurationExample4 (public, protected, and private). Be aware that some
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
#include "ClassRegistryItemT.h"
#include "ConfigurationDatabase.h"
#include "ErrorLoggerExample.h"
#include "IntrospectionT.h"
#include "Matrix.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"
#include "Reference.h"
#include "ReferenceT.h"
#include "StandardParser.h"
#include "StreamString.h"
#include "Vector.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {

/**
 * Configuration structures
 */
struct Gain {
    MARTe::float32 gain1;
    MARTe::float32 gain2;
    MARTe::float32 gain3[6];
};
struct Gains {
    struct Gain lowGains;
    struct Gain highGains;
};

DECLARE_CLASS_MEMBER(Gain, gain1, float32, "", "");
DECLARE_CLASS_MEMBER(Gain, gain2, float32, "", "");
DECLARE_CLASS_MEMBER(Gain, gain3, float32, "[6]", "");

static const MARTe::IntrospectionEntry* GainStructEntries[] = { &Gain_gain1_introspectionEntry, &Gain_gain2_introspectionEntry, &Gain_gain3_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(Gain, GainStructEntries)

DECLARE_CLASS_MEMBER(Gains, lowGains, Gain, "", "");
DECLARE_CLASS_MEMBER(Gains, highGains, Gain, "", "");
static const MARTe::IntrospectionEntry* GainsStructEntries[] = { &Gains_lowGains_introspectionEntry, &Gains_highGains_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(Gains, GainsStructEntries)

/**
 * @brief A simple MARTe::Object class that will read directly read its configuration from a structure.
 */
class ControllerEx1: public MARTe::ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
ControllerEx1    () {

    }

    virtual ~ControllerEx1 () {
        if (GetName() != NULL) {
            REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "No more references pointing at %s [%s]. "
                    "The Object will be safely deleted.", GetName(), GetClassProperties()->GetName());
        }
    }

    /**
     * Read all the properties from the Gains struct (names must match the ones of the struct!
     * Gains1 = {
     *     lowGain = {
     *         Gain1 = -1.0
     *         Gain2 = -3.0
     *         Gain3 = {-9.0, -8.0, -7.0, -6.0, -5.0, -4.0}
     *     }
     *     High = {
     *         Gain1 = 7.0
     *         Gain2 = 9.0
     *     }
     * }
     * Gains2 = {
     *     lowGain = {
     *         Gain1 = -1.1
     *         Gain2 = -3.1
     *         Gain3 = {-9.1, -8.1, -7.1, -6.1, -5.1, -4.1}
     *     }
     *     High = {
     *         Gain1 = 7.1
     *         Gain2 = 9.1
     *         Gain3 = {9.1, 8.1, 7.1, 6.1, 5.1, 4.1}"
     *     }
     * }
     */
    virtual bool Initialise(MARTe::StructuredDataI &data) {
        using namespace MARTe;
        bool ok = ReferenceContainer::Initialise(data);
        ClassRegistryItem *gainsStructClassRegistryItem = NULL_PTR(ClassRegistryItem *);
        if (ok) {
            //Search for the registered structure
            gainsStructClassRegistryItem = ClassRegistryDatabase::Instance()->Find("Gains");
        }
        const ClassProperties *gainsStructClassProperties = NULL_PTR(ClassProperties *);
        if (ok) {
            gainsStructClassProperties = gainsStructClassRegistryItem->GetClassProperties();
        }
        ClassUID gainsStructClassUID;
        AnyType gainsAnyType;
        if (ok) {
            gainsStructClassUID = gainsStructClassProperties->GetUniqueIdentifier();
            //Encapsulate the AnyType

            TypeDescriptor gainsTypeDescriptor(false, gainsStructClassUID);
            gainsAnyType = AnyType(gainsTypeDescriptor, 0u, &gains1);
            ok = data.Read("Gains1", gainsAnyType);
            if (ok) {
                gainsAnyType = AnyType(gainsTypeDescriptor, 0u, &gains2);
                ok = data.Read("Gains2", gainsAnyType);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Could not read the Gains2 structure");
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not read the Gains1 structure");
            }
        }

        if (ok) {
            REPORT_ERROR(ErrorManagement::Information, "Gains 1 low gains");
            PrintGains(&gains1.lowGains);
            REPORT_ERROR(ErrorManagement::Information, "Gains 1 high gains");
            PrintGains(&gains1.highGains);
            REPORT_ERROR(ErrorManagement::Information, "Gains 2 low gains");
            PrintGains(&gains2.lowGains);
            REPORT_ERROR(ErrorManagement::Information, "Gains 2 high gains");
            PrintGains(&gains2.highGains);
        }

        return ok;
    }

private:
    void PrintGains(Gain *gainToPrint) {
        using namespace MARTe;
        REPORT_ERROR(ErrorManagement::Information, "Gain1 %f", gainToPrint->gain1);
        REPORT_ERROR(ErrorManagement::Information, "Gain2 %f", gainToPrint->gain2);
        REPORT_ERROR(ErrorManagement::Information, "Gain3 %f", gainToPrint->gain3);
    }

    /**
     * A list of properties.
     */
    Gains gains1;
    Gains gains2;
};

CLASS_REGISTER(ControllerEx1, "")

}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int main(int argc, char **argv) {
    using namespace MARTe;
    using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    StreamString configurationCfg = ""
            "+ControllerInstance1 = {\n"
            "    Class = ControllerEx1\n"
            "    Gains1 = {\n"
            "        lowGains = {\n"
            "            Gain1 = -1.0\n"
            "            Gain2 = -3.0\n"
            "            Gain3 = {-9.0, -8.0, -7.0, -6.0, -5.0, -4.0}\n"
            "        }\n"
            "        highGains = {\n"
            "            Gain1 = 7.0\n"
            "            Gain2 = 9.0\n"
            "            Gain3 = {9.0, 8.0, 7.0, 6.0, 5.0, 4.0}\n"
            "        }\n"
            "    }\n"
            "    Gains2 = {\n"
            "        lowGains = {\n"
            "            Gain1 = -1.0\n"
            "            Gain2 = -3.0\n"
            "            Gain3 = {-9.1, -8.1, -7.1, -6.1, -5.1, -4.1}\n"
            "        }\n"
            "        highGains = {\n"
            "            Gain1 = 7.0\n"
            "            Gain2 = 9.0\n"
            "            Gain3 = {9.1, 8.1, 7.1, 6.1, 5.1, 4.1}\n"
            "        }\n"
            "    }\n"
            "}";

    REPORT_ERROR_STATIC(ErrorManagement::Information, "Loading CFG:\n%s", configurationCfg.Buffer());
    ConfigurationDatabase cdb;
    StreamString err;
    //Force the string to be seeked to the beginning.
    configurationCfg.Seek(0LLU);
    StandardParser parser(configurationCfg, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        //After parsing the tree is pointing at the last leaf
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    else {
        StreamString errPrint;
        errPrint.Printf("Failed to parse %s", err.Buffer());
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, errPrint.Buffer());
    }

    if (ok) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully loaded the configuration file");
    }

    return 0;
}

