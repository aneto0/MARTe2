/**
 * @file ConfigurationExample7.cpp
 * @brief Source file for class ConfigurationExample7
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
 * the class ConfigurationExample7 (public, protected, and private). Be aware that some
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
struct __attribute__((__packed__)) GainFromIntroStructure {
    MARTe::float32 gain1;
    MARTe::float32 gain2;
    MARTe::float32 gain3[6];
};
struct __attribute__((__packed__)) GainsFromIntroStructure {
    struct GainFromIntroStructure lowGains[2];
    struct GainFromIntroStructure highGains;
};

/**
 * @brief A MARTe::Object class that will read directly read its configuration from a structure.
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
     *     lowGain[0] = {
     *         Gain1 = -1.0
     *         Gain2 = -3.0
     *         Gain3 = {-9.0, -8.0, -7.0, -6.0, -5.0, -4.0}
     *     }
     *     lowGain[1] = {
     *         Gain1 = -2.0
     *         Gain2 = -6.0
     *         Gain3 = {-18.0, -16.0, -14.0, -12.0, -10.0, -8.0}
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
            gainsStructClassRegistryItem = ClassRegistryDatabase::Instance()->Find("GainsFromIntroStructure");
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
            REPORT_ERROR(ErrorManagement::Information, "Gains 1 low[0] gains");
            PrintGains(&gains1.lowGains[0]);
            REPORT_ERROR(ErrorManagement::Information, "Gains 1 low[1] gains");
            PrintGains(&gains1.lowGains[1]);
            REPORT_ERROR(ErrorManagement::Information, "Gains 1 high gains");
            PrintGains(&gains1.highGains);
            REPORT_ERROR(ErrorManagement::Information, "Gains 2 low[0] gains");
            PrintGains(&gains2.lowGains[0]);
            REPORT_ERROR(ErrorManagement::Information, "Gains 2 low[1] gains");
            PrintGains(&gains2.lowGains[1]);
            REPORT_ERROR(ErrorManagement::Information, "Gains 2 high gains");
            PrintGains(&gains2.highGains);
        }

        return ok;
    }

private:
    void PrintGains(GainFromIntroStructure *gainToPrint) {
        using namespace MARTe;
        float32 gain1 = gainToPrint->gain1;
        float32 gain2 = gainToPrint->gain2;
        float32 gain3[6] = {gainToPrint->gain3[0], gainToPrint->gain3[1], gainToPrint->gain3[2], gainToPrint->gain3[3], gainToPrint->gain3[4], gainToPrint->gain3[5]};
        REPORT_ERROR(ErrorManagement::Information, "Gain1 %f", gain1);
        REPORT_ERROR(ErrorManagement::Information, "Gain2 %f", gain2);
        REPORT_ERROR(ErrorManagement::Information, "Gain3 %f", gain3);
    }

    /**
     * A list of properties.
     */
    GainsFromIntroStructure gains1;
    GainsFromIntroStructure gains2;
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
            "+Types = {\n"
            "    Class = ReferenceContainer\n"
            "    +GainFromIntroStructure = {\n"
            "        Class = IntrospectionStructure\n"
            "        gain1 = {\n"
            "            Type = float32\n"
            "            NumberOfElements = {1}\n"
            "        }\n"
            "        gain2 = {\n"
            "            Type = float32\n"
            "            NumberOfElements = {1}\n"
            "        }\n"
            "        gain3 = {\n"
            "            Type = float32\n"
            "            NumberOfElements = {6}\n"
            "        }\n"
            "    }\n"
            "    +GainsFromIntroStructure = {\n"
            "        Class = IntrospectionStructure\n"
            "        lowGains = {\n"
            "            Type = GainFromIntroStructure\n"
            "            NumberOfElements = {2}\n"
            "        }\n"
            "        highGains = {\n"
            "            Type = GainFromIntroStructure\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+ControllerInstance1 = {\n"
            "    Class = ControllerEx1\n"
            "    Gains1 = {\n"
            "        lowGains[0] = {\n"
            "            Gain1 = -1.0\n"
            "            Gain2 = -3.0\n"
            "            Gain3 = {-9.0, -8.0, -7.0, -6.0, -5.0, -4.0}\n"
            "        }\n"
            "        lowGains[1] = {\n"
            "            Gain1 = -2.0\n"
            "            Gain2 = -6.0\n"
            "            Gain3 = {-18.0, -16.0, -14.0, -12.0, -10.0, -8.0}\n"
            "        }\n"
            "        highGains = {\n"
            "            Gain1 = 7.0\n"
            "            Gain2 = 9.0\n"
            "            Gain3 = {9.0, 8.0, 7.0, 6.0, 5.0, 4.0}\n"
            "        }\n"
            "    }\n"
            "    Gains2 = {\n"
            "        lowGains[0] = {\n"
            "            Gain1 = -1.0\n"
            "            Gain2 = -3.0\n"
            "            Gain3 = {-9.1, -8.1, -7.1, -6.1, -5.1, -4.1}\n"
            "        }\n"
            "        lowGains[1] = {\n"
            "            Gain1 = -2.0\n"
            "            Gain2 = -6.0\n"
            "            Gain3 = {-18.2, -16.2, -14.2, -12.2, -10.2, -8.2}\n"
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

    //Write a structure to a ConfigurationDatabase
    ClassRegistryItem *gainsStructClassRegistryItem = NULL_PTR(ClassRegistryItem *);
    if (ok) {
        gainsStructClassRegistryItem = ClassRegistryDatabase::Instance()->Find("GainsFromIntroStructure");
    }
    AnyType gainsAnyType;
    const ClassProperties *gainsStructClassProperties = NULL_PTR(ClassProperties *);
    if (ok) {
        gainsStructClassProperties = gainsStructClassRegistryItem->GetClassProperties();
    }
    if (ok) {
        ConfigurationDatabase cdb;
        GainsFromIntroStructure gainsExample;
        gainsExample.lowGains[0].gain1 = 1;
        gainsExample.lowGains[0].gain2 = 2;
        gainsExample.lowGains[1].gain3[0] = -1;
        gainsExample.lowGains[1].gain3[5] = 1;
        gainsExample.highGains.gain1 = -1;
        gainsExample.highGains.gain2 = -2;
        gainsExample.highGains.gain3[0] = 1;
        gainsExample.highGains.gain3[5] = -1;

        ClassUID gainsStructClassUID = gainsStructClassProperties->GetUniqueIdentifier();
        //Encapsulate the AnyType

        TypeDescriptor gainsTypeDescriptor(false, gainsStructClassUID);
        gainsAnyType = AnyType(gainsTypeDescriptor, 0u, &gainsExample);
        ok = cdb.Write("DumpStruct", gainsAnyType);
        if (ok) {
            cdb.MoveToRoot();
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Wrote structure %!", cdb);
        }
    }
    return 0;
}

