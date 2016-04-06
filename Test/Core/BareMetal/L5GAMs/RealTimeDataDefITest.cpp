/**
 * @file RealTimeDataDefITest.cpp
 * @brief Source file for class RealTimeDataDefITest
 * @date 04/03/2016
 * @author Giuseppe Ferrò
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
 * the class RealTimeDataDefITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataDefITest.h"
#include "ConfigurationDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool RealTimeDataDefITest::TestConstructor() {
    RealTimeSampledDataDef def;

    if (StringHelper::Compare(def.GetType(), "") != 0) {
        return false;
    }

    return (StringHelper::Compare(def.GetPath(), "") == 0);
}

bool RealTimeDataDefITest::TestInitialise() {
    RealTimeSampledDataDef def;

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

bool RealTimeDataDefITest::TestGetType() {
    RealTimeSampledDataDef def;

    ConfigurationDatabase cdb;
    cdb.Write("Type", "uint32");

    if (!def.Initialise(cdb)) {
        return false;
    }

    return (StringHelper::Compare(def.GetType(), "uint32") == 0);

}

bool RealTimeDataDefITest::TestGetPath() {
    RealTimeSampledDataDef def;

    ConfigurationDatabase cdb;
    cdb.Write("Path", "DDB1.PID1.Kp");

    if (!def.Initialise(cdb)) {
        return false;
    }

    return (StringHelper::Compare(def.GetPath(), "DDB1.PID1.Kp") == 0);
}


bool RealTimeDataDefITest::TestSetPath(){
    RealTimeSampledDataDef def;

    ConfigurationDatabase cdb;
    cdb.Write("Path", "DDB1.PID1.Kp");

    if (!def.Initialise(cdb)) {
        return false;
    }

    if(StringHelper::Compare(def.GetPath(), "DDB1.PID1.Kp") != 0){
        return false;
    }

    def.SetPath("MyPath");
    return (StringHelper::Compare(def.GetPath(), "MyPath") == 0);
}


bool RealTimeDataDefITest::TestGetDefaultValue() {
    RealTimeSampledDataDef def;

    ConfigurationDatabase cdb;
    cdb.Write("Default", "123");

    if (!def.Initialise(cdb)) {
        return false;
    }

    return (StringHelper::Compare(def.GetDefaultValue(), "123") == 0);
}

bool RealTimeDataDefITest::TestNumberOfDimensions(const char8 * type,
                                                  const char8 * modifiers,
                                                  uint8 ret) {
    RealTimeSampledDataDef def;

    ConfigurationDatabase cdb;
    cdb.Write("Type", type);
    cdb.Write("Path", "DDB1.PID1.Kp");
    cdb.Write("Modifiers", modifiers);
    if (!def.Initialise(cdb)) {
        return false;
    }
    return def.GetNumberOfDimensions() == ret;
}

bool RealTimeDataDefITest::TestNumberOfElements(const char8 * type,
                                                const char8 * modifiers,
                                                uint32 ret[3]) {
    RealTimeSampledDataDef def;

    ConfigurationDatabase cdb;
    cdb.Write("Type", type);
    cdb.Write("Path", "DDB1.PID1.Kp");
    cdb.Write("Modifiers", modifiers);
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
