/**
 * @file ConfigurationExample2.cpp
 * @brief Source file for class ConfigurationExample2
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
 * the class ConfigurationExample2 (public, protected, and private). Be aware that some
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
 * @brief A MARTe::Object class will be automatically registered into the ClassRegistryDatabase.
 */
class ControllerEx1: public MARTe::Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
ControllerEx1   () {
        slowWaveform.times = NULL;
        slowWaveform.values = NULL;
        fastWaveform.times = NULL;
        fastWaveform.values = NULL;

    }

    virtual ~ControllerEx1 () {
        if (slowWaveform.times != NULL) {
            delete [] slowWaveform.times;
        }
        if (slowWaveform.values != NULL) {
            delete [] slowWaveform.values;
        }
        if (fastWaveform.times != NULL) {
            delete [] fastWaveform.times;
        }
        if (fastWaveform.values != NULL) {
            delete [] fastWaveform.values;
        }
        if (GetName() != NULL) {
            REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "No more references pointing at "
                    "%s [%s]. The Object will be safely deleted.", GetName(), GetClassProperties()->GetName());
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
     * References = {
     *     Slow = {
     *         Waveform = {
     *             Times  = {0 0.1 0.2 1}
     *             Values = {1 2   3   4}
     *         }
     *     }
     *     Fast = {
     *         Waveform = {
     *             Times  = {0 0.1 0.2 1}
     *             Values = {1 2   3   4}
     *         }
     *     }
     * }
     */
    virtual bool Initialise(MARTe::StructuredDataI &data) {
        using namespace MARTe;
        bool ok = Object::Initialise(data);
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
            //Move to the ancestor
            ok = data.MoveToAncestor(2u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not move back to the References section");
            }
        }
        if (ok) {
            ok = data.MoveRelative("References.Slow.Waveform");
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the References.Slow.Waveform section");
            }
        }
        if (ok) {
            ok = ReadArray(data, "Times", slowWaveform.times);
        }
        if (ok) {
            ok = ReadArray(data, "Values", slowWaveform.values);
        }
        //Move back to the parent
        if (ok) {
            ok = data.MoveToAncestor(2u);
        }
        if (ok) {
            ok = data.MoveRelative("Fast.Waveform");
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the References.fast.Waveform section");
            }
        }
        if (ok) {
            ok = ReadArray(data, "Times", fastWaveform.times);
        }
        if (ok) {
            ok = ReadArray(data, "Values", fastWaveform.values);
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

    /**
     * A list of properties.
     */
    Gains lowGains;
    Gains highGains;
    Waveforms slowWaveform;
    Waveforms fastWaveform;
};

CLASS_REGISTER(ControllerEx1, "")

}

int main(int argc, char **argv) {
    using namespace MARTe;
    using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    CCString className1 = "ControllerEx1";

    float32 times1[] = { 0.0, 0.2, 1 };
    float32 values1[] = { 1.0, -1.0, 1.0 };
    float32 times2[] = { 0.0, 0.5, 1 };
    float32 values2[] = { 0.0, 1.0, 0.0 };

    ReferenceT<ControllerEx1> ref1(className1,
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    //Automatically generate a new object instance based on the class name and on the correct Heap and with the template reference.
    if (ref1.IsValid()) {
        ref1->SetName("ControllerInstance1");
        REPORT_ERROR_STATIC(ErrorManagement::Information,
                "Successfully created an instance of %s", className1.GetList());
        //Write a valid configuration.
        {
            ConfigurationDatabase cdb;
            //Create the structure nodes
            cdb.CreateAbsolute("Gains.Low");
            cdb.Write("Gain1", 1.0);
            cdb.Write("Gain2", -1.0);
            cdb.CreateAbsolute("Gains.High");
            cdb.Write("Gain1", 5.0);
            cdb.Write("Gain2", -5.0);
            cdb.CreateAbsolute("References.Slow.Waveform");
            cdb.Write("Times", times1);
            cdb.Write("Values", values1);
            cdb.CreateAbsolute("References.Fast.Waveform");
            cdb.Write("Times", times2);
            cdb.Write("Values", values2);
            //Move back again to the root of the tree
            cdb.MoveToRoot();

            //Print the configuration
            REPORT_ERROR_STATIC(ErrorManagement::Information,
                    "Going to load the following configuration\n%s", cdb);
            //Need to move back after the printf
            cdb.MoveToRoot();
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
            //Create the structure nodes with a typo: Gain (not Gains)
            cdb.CreateAbsolute("Gain.Low");
            cdb.Write("Gain1", 1.0);
            cdb.Write("Gain2", -1.0);
            cdb.CreateAbsolute("Gain.High");
            cdb.Write("Gain1", 5.0);
            cdb.Write("Gain2", -5.0);
            cdb.CreateAbsolute("References.Slow.Waveform");
            cdb.Write("Times", times1);
            cdb.Write("Values", values1);
            cdb.CreateAbsolute("References.Fast.Waveform");
            cdb.Write("Times", times2);
            cdb.Write("Values", values2);
            //Move back again to the root of the tree
            cdb.MoveToRoot();

            //Print the configuration
            REPORT_ERROR_STATIC(ErrorManagement::Information,
                    "Going to load the following configuration\n%s", cdb);

            if (!ref1->Initialise(cdb)) {
                REPORT_ERROR_STATIC(ErrorManagement::Information,
                        "As expected failed to configure instance of %s",
                        ref1->GetName());
            }
        }
    }

    return 0;
}

