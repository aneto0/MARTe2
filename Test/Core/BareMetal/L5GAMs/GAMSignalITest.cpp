/**
 * @file GAMSignalITest.cpp
 * @brief Source file for class GAMSignalITest
 * @date 12/apr/2016
 * @author pc
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
 * the class GAMSignalITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMSignalITest.h"
#include "ConfigurationDatabase.h"
#include "GAMGenericSignal.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool GAMSignalITest::TestConstructor() {
    GAMGenericSignal def;

    if (StringHelper::Compare(def.GetType(), "") != 0) {
        return false;
    }

    return (StringHelper::Compare(def.GetPath(), "") == 0);
}

bool GAMSignalITest::TestInitialise() {
    GAMGenericSignal def;

    ConfigurationDatabase cdb;
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "DDB1.PID1.Kp");

    if (!def.Initialise(cdb)) {
        return false;
    }

    if (StringHelper::Compare(def.GetType(), "uint32") != 0) {
        return false;
    }

    return (StringHelper::Compare(def.GetPath(), "DDB1.PID1.Kp") == 0);
}

bool GAMSignalITest::TestGetType() {
    GAMGenericSignal def;

    ConfigurationDatabase cdb;
    cdb.Write("Type", "uint32");

    if (!def.Initialise(cdb)) {
        return false;
    }

    return (StringHelper::Compare(def.GetType(), "uint32") == 0);

}

bool GAMSignalITest::TestGetPath() {
    GAMGenericSignal def;

    ConfigurationDatabase cdb;
    cdb.Write("Path", "DDB1.PID1.Kp");

    if (!def.Initialise(cdb)) {
        return false;
    }

    return (StringHelper::Compare(def.GetPath(), "DDB1.PID1.Kp") == 0);
}

bool GAMSignalITest::TestSetPath() {
    GAMGenericSignal def;

    ConfigurationDatabase cdb;
    cdb.Write("Path", "DDB1.PID1.Kp");

    if (!def.Initialise(cdb)) {
        return false;
    }

    if (StringHelper::Compare(def.GetPath(), "DDB1.PID1.Kp") != 0) {
        return false;
    }

    def.SetPath("MyPath");
    return (StringHelper::Compare(def.GetPath(), "MyPath") == 0);
}

bool GAMSignalITest::TestGetDefaultValue() {
    GAMGenericSignal def;

    ConfigurationDatabase cdb;
    cdb.Write("Default", "123");

    if (!def.Initialise(cdb)) {
        return false;
    }

    return (StringHelper::Compare(def.GetDefaultValue(), "123") == 0);
}

bool GAMSignalITest::TestGetOperation() {
    GAMGenericSignal def;

    ConfigurationDatabase cdb;
    cdb.Write("Operation", "Write \"ahahah\"");

    if (!def.Initialise(cdb)) {
        return false;
    }

    return (StringHelper::Compare(def.GetOperation(), "Write \"ahahah\"") == 0);
}

bool GAMSignalITest::TestNumberOfDimensions(const char8 * type,
                                            const char8 * dimensions,
                                            uint8 ret) {
    GAMGenericSignal def;

    ConfigurationDatabase cdb;
    cdb.Write("Type", type);
    cdb.Write("Path", "DDB1.PID1.Kp");
    cdb.Write("Dimensions", dimensions);
    if (!def.Initialise(cdb)) {
        return false;
    }
    return def.GetNumberOfDimensions() == ret;
}

bool GAMSignalITest::TestNumberOfElements(const char8 * type,
                                          const char8 * dimensions,
                                          uint32 ret[3]) {
    GAMGenericSignal def;

    ConfigurationDatabase cdb;
    cdb.Write("Type", type);
    cdb.Write("Path", "DDB1.PID1.Kp");
    cdb.Write("Dimensions", dimensions);
    if (!def.Initialise(cdb)) {
        return false;
    }

    for (uint32 k = 0; k < 3; k++) {
        if (def.GetNumberOfElements(k) != ret[k]) {
            return false;
        }
    }
    return true;
}
