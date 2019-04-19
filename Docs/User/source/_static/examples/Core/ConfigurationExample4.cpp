/**
 * @file ConfigurationExample4.cpp
 * @brief Source file for class ConfigurationExample4
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
#include "ConfigurationDatabase.h"
#include "ErrorLoggerExample.h"
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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {

/**
 * Configuration structures
 */
struct Gains {
    MARTe::float32 gain1;
    MARTe::float32 gain2;
};

struct Waveforms {
    MARTe::float32 *times;
    MARTe::float32 *values;
};

/**
 * @brief A MARTe::Object that will be inserted (data-driven) inside the ControllerEx1.
 */
class ReferenceEx1: public MARTe::Object {
public:
    CLASS_REGISTER_DECLARATION()

ReferenceEx1    () {
        waveform.times = NULL;
        waveform.values = NULL;
    }

    virtual ~ReferenceEx1 () {
        if (waveform.times != NULL) {
            delete [] waveform.times;
        }
        if (waveform.values != NULL) {
            delete [] waveform.values;
        }
        if (GetName() != NULL) {
            REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "No more references pointing"
                    " at %s [%s]. The Object will be safely deleted.", GetName(), GetClassProperties()->GetName());
        }
    }

    /**
     * Read the waveform properties
     * Times  = {0 0.1 0.2 1}
     * Values = {1 2   3   4}
     */
    virtual bool Initialise(MARTe::StructuredDataI &data) {
        using namespace MARTe;
        bool ok = Object::Initialise(data);

        if (ok) {
            ok = ReadArray(data, "Times", waveform.times);
        }
        if (ok) {
            ok = ReadArray(data, "Values", waveform.values);
        }

        return ok;
    }

private:
    bool ReadArray(MARTe::StructuredDataI &data, const MARTe::char8 * const arrayName, MARTe::float32 *&dest) {
        using namespace MARTe;
        if (dest != NULL) {
            delete [] dest;
        }

        AnyType arrayDescription = data.GetType(arrayName);
        bool ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
        uint32 numberOfElements = 0u;
        if (ok) {
            numberOfElements = arrayDescription.GetNumberOfElements(0u);
            ok = (numberOfElements > 0u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "No elements defined in the array with name %s", arrayName);
            }
        }
        if (ok) {
            dest = new float32[numberOfElements];
            Vector<float32> readVector(dest, numberOfElements);
            ok = data.Read(arrayName, readVector);
            if (ok) {
                REPORT_ERROR(ErrorManagement::Information, "Array set to %f", readVector);
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not read the array with name %s", arrayName);
            }
        }
        return ok;
    }

    Waveforms waveform;
};
CLASS_REGISTER(ReferenceEx1, "")

/**
 * @brief A MARTe::Object class will be automatically registered into the ClassRegistryDatabase.
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
     * Read all the properties which are organised inside a tree
     * Gains = {
     *     Low = {
     *         Gain1 = -1.0;
     *         Gain2 = -3.0;
     *     }
     *     High = {
     *         Gain1 = 7.0;
     *         Gain2 = 9.0;
     *     }
     * }
     * +SlowReference = {
     *     Class = ReferenceEx1
     *     Times  = {0 0.1 0.2 1}
     *     Values = {1 2   3   4}
     * }
     * +FastReference = {
     *     Class = ReferenceEx1
     *     Times  = {0 0.1 0.2 1}
     *     Values = {1 4   8   12}
     * }
     */
    virtual bool Initialise(MARTe::StructuredDataI &data) {
        using namespace MARTe;
        bool ok = ReferenceContainer::Initialise(data);
        if (ok) {
            //Move in the tree
            ok = data.MoveRelative("Gains");
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Gains section");
            }
        }
        if (ok) {
            ok = data.MoveRelative("Low");
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Gains.Low section");
            }
        }
        if (ok) {
            ok = data.Read("Gain1", lowGains.gain1);
            if (ok) {
                REPORT_ERROR(ErrorManagement::Information, "Gains.Low.Gain1 = %f", lowGains.gain1);
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not read the Gain1");
            }
        }
        if (ok) {
            ok = data.Read("Gain2", lowGains.gain2);
            if (ok) {
                REPORT_ERROR(ErrorManagement::Information, "Gains.Low.Gain1 = %f", lowGains.gain2);
            }
            else {
            }
        }
        if (ok) {
            ok = data.MoveToAncestor(1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not move back to the Gains section");
            }
        }
        if (ok) {
            ok = data.MoveRelative("High");
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Gains.High section");
            }
        }
        if (ok) {
            ok = data.Read("Gain1", highGains.gain1);
            if (ok) {
                REPORT_ERROR(ErrorManagement::Information, "Gains.High.Gain1 = %f", highGains.gain1);
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not read the Gain1");
            }
        }
        if (ok) {
            ok = data.Read("Gain2", highGains.gain2);
            if (ok) {
                REPORT_ERROR(ErrorManagement::Information, "Gains.High.Gain1 = %f", highGains.gain2);
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not read the Gain2");
            }
        }

        if (ok) {
            //Check if the waveforms were declared
            slowWaveform = Get(0);
            ok = slowWaveform.IsValid();
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not find the SlowWaveform");
            }
        }

        if (ok) {
            //Check if the waveforms were declared
            fastWaveform = Get(1);
            ok = fastWaveform.IsValid();
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not find the FastWaveform");
            }
        }

        return ok;
    }

    /**
     * A list of properties.
     */
    Gains lowGains;
    Gains highGains;
    MARTe::ReferenceT<ReferenceEx1> slowWaveform;
    MARTe::ReferenceT<ReferenceEx1> fastWaveform;
};

CLASS_REGISTER(ControllerEx1, "")

}

int main(int argc, char **argv) {
    using namespace MARTe;
    using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    StreamString configurationCfg = ""
            "+ControllerInstance1 = {\n"
            "    Class = ControllerEx1\n"
            "    Gains = {\n"
            "        Low = {\n"
            "            Gain1 = -1.0\n"
            "            Gain2 = -3.0\n"
            "        }\n"
            "        High = {\n"
            "            Gain1 = 7.0\n"
            "            Gain2 = 9.0\n"
            "        }\n"
            "    }\n"
            "    +SlowWaveform = {\n"
            "        Class = ReferenceEx1\n"
            "        Times  = {0 0.1 0.2 1}\n"
            "        Values = {1 2   3   4}\n"
            "    }\n"
            "    +FastWaveform = {\n"
            "        Class = ReferenceEx1\n"
            "        Times  = {0 0.1 0.2 1}\n"
            "        Values = {1 4   6   8}\n"
            "    }\n"
            "}";

    REPORT_ERROR_STATIC(ErrorManagement::Information, "Loading CFG:\n%s",
                        configurationCfg.Buffer());
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
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError,
                            errPrint.Buffer());
    }

    if (ok) {
        REPORT_ERROR_STATIC(ErrorManagement::Information,
                            "Successfully loaded the configuration file");
    }

    return 0;
}

