/**
 * @file RealTimeApplicationConfigurationBuilderExample1.cpp
 * @brief Source file for class RealTimeApplicationConfigurationBuilderExample1
 * @date 12/04/2018
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
 * the class ReferencesExample6 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ClassRegistryDatabase.h"
#include "ErrorLoggerExample.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplicationConfigurationBuilder.h"
#include "Reference.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"
#include "StandardParser.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int main(int argc, char **argv) {
    using namespace MARTe;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    StreamString configurationCfg = ""
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    DataSource = Timer\n"
            "                    Type = uint32\n"
            "                }\n"
            "                Time = {\n"
            "                    Frequency = 1\n"
            "                    DataSource = Timer\n"
            "                    Type = uint32\n"
            "                }\n"
            "           }\n"
            "           OutputSignals = {\n"
            "               Counter = {\n"
            "                   DataSource = DDB1\n"
            "                   Type = uint32\n"
            "               }\n"
            "               Time = {\n"
            "                   DataSource = DDB1\n"
            "                   Type = uint32\n"
            "               }\n"
            "           }\n"
            "       }\n"
            "       +GAMFixed1 = {\n"
            "           Class = FixedGAMExample1\n"
            "           Gain = 2\n"
            "           InputSignals = {\n"
            "               Counter = {\n"
            "                   DataSource = DDB1\n"
            "                   Type = uint32\n"
            "               }\n"
            "           }\n"
            "           OutputSignals = {\n"
            "               GainCounter = {\n"
            "                   DataSource = DDB1\n"
            "                   Type = uint32\n"
            "               }\n"
            "           }\n"
            "       }\n"
            "       +GAMDisplay = {\n"
            "           Class = IOGAM\n"
            "           InputSignals = {\n"
            "               Counter = {\n"
            "                   DataSource = DDB1\n"
            "                   Type = uint32\n"
            "               }\n"
            "               GainCounter = {\n"
            "                   DataSource = DDB1\n"
            "                   Type = uint32\n"
            "               }\n"
            "           }\n"
            "           OutputSignals = {\n"
            "               Counter = {\n"
            "                   DataSource = LoggerDataSource\n"
            "                   Type = uint32\n"
            "               }\n"
            "               GainCounter = {\n"
            "                   DataSource = LoggerDataSource\n"
            "                   Type = uint32\n"
            "               }\n"
            "           }\n"
            "       }\n"
            "   }\n"
            "   +Data = {\n"
            "       Class = ReferenceContainer\n"
            "       DefaultDataSource = DDB1\n"
            "       +DDB1 = {\n"
            "           Class = GAMDataSource\n"
            "       }\n"
            "       +LoggerDataSource = {\n"
            "           Class = LoggerDataSource\n"
            "       }\n"
            "       +Timings = {\n"
            "           Class = TimingDataSource\n"
            "       }\n"
            "       +Timer = {\n"
            "           Class = LinuxTimer\n"
            "           SleepNature = \"Default\"\n"
            "           Signals = {\n"
            "               Counter = {\n"
            "                   Type = uint32\n"
            "               }\n"
            "               Time = {\n"
            "                   Type = uint32\n"
            "               }\n"
            "           }\n"
            "       }\n"
            "   }\n"
            "   +States = {\n"
            "       Class = ReferenceContainer\n"
            "       +State1 = {\n"
            "           Class = RealTimeState\n"
            "           +Threads = {\n"
            "               Class = ReferenceContainer\n"
            "               +Thread1 = {\n"
            "                   Class = RealTimeThread\n"
            "                   CPUs = 0x1\n"
            "                   Functions = {GAMTimer GAMFixed1 GAMDisplay }\n"
            "               }\n"
            "           }\n"
            "       }\n"
            "   }\n"
            "   +Scheduler = {\n"
            "       Class = GAMScheduler\n"
            "       TimingDataSource = Timings\n"
            "   }\n";


    ConfigurationDatabase cdb;
    StreamString err;
    //Force the string to be seeked to the beginning.
    configurationCfg.Seek(0LLU);
    StandardParser parser(configurationCfg, cdb, &err);
    ConfigurationDatabase fundb;
    ConfigurationDatabase datadb;
    bool ok = parser.Parse();
    if (ok) {
        cdb.MoveToRoot();
        RealTimeApplicationConfigurationBuilder builder(cdb, "DDB1");
        if (ok) {
            ok = builder.ConfigureBeforeInitialisation();
        }
        if (ok) {
            ok = builder.Copy(fundb, datadb);
        }
    }
    if (ok) {
        fundb.MoveToRoot();
        StreamString fundbStr;
        fundbStr.Printf("%!", fundb);
        printf("\n\n\nFunctions\n\n\n%s\n", fundbStr.Buffer());
        datadb.MoveToRoot();
        StreamString datadbStr;
        datadbStr.Printf("%!", datadb);
        printf("\n\n\nFunctions\n\n\n%s\n", datadbStr.Buffer());
    }


    return 0;
}

