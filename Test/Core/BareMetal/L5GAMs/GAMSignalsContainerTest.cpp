/**
 * @file GAMSignalsContainerTest.cpp
 * @brief Source file for class GAMSignalsContainerTest
 * @date 13/04/2016
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
 * the class GAMSignalsContainerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMSignalsContainerTest.h"
#include "ConfigurationDatabase.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool GAMSignalsContainerTest::TestConstructor() {
    GAMSignalsContainer def;
    return (!def.IsInput()) && (!def.IsOutput());
}

bool GAMSignalsContainerTest::TestInitialise() {
    GAMSignalsContainer def;

    ConfigurationDatabase cdb;
    cdb.Write("IsFinal", "true");
    cdb.Write("IsInput", "true");
    cdb.Write("IsOutput", "true");
    if (!def.Initialise(cdb)) {
        return false;
    }

    return (def.IsInput()) && (def.IsOutput());
}

bool GAMSignalsContainerTest::TestMergeWithLocal() {

    ConfigurationDatabase localData;

    localData.Write("Class", "GAMGenericSignalContainer");
    localData.Write("IsFinal", "false");
    localData.Write("IsInput", "true");
    localData.CreateAbsolute("+Control");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Control.+Par1");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");

    localData.CreateAbsolute("+Error");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "TrackError");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Error.+Par2");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Default", "2");
    localData.Write("Path", "DDB1.PidError2");
    localData.Write("IsFinal", "true");

    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "GAMGenericSignalContainer");
    cdb.Write("IsFinal", "false");
    cdb.Write("IsInput", "true");
    cdb.CreateAbsolute("+Control");
    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Control.+Par2");
    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");

    cdb.CreateAbsolute("+Error");
    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "TrackError");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Error.+Par1");
    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "uint32");
    cdb.Write("Default", "1");
    cdb.Write("Path", "DDB1.PidError1");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<GAMSignalsContainer> test = ReferenceT<GAMSignalsContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Inputs");
    if (!test->Initialise(cdb)) {
        return false;
    }

    if (!test->MergeWithLocal(localData)) {
        return false;
    }

    ConfigurationDatabase out;
    test->ToStructuredData(out);

    //out.MoveToRoot();
    StreamString display;

    display.Printf("%!", out);

    StreamString testOut = "+Inputs = {\n"
            "Class = \"GAMSignalsContainer\"\n"
            "IsInput = \"true\"\n"
            "+Control = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"ControlIn\"\n"
            "+Par2 = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"uint32\"\n"
            "Path = \"PidControl2\"\n"
            "Default = \"1\"\n"
            "}\n"
            "+Par1 = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"uint32\"\n"
            "Path = \"DDB2.PidControl1\"\n"
            "Default = \"1\"\n"
            "}\n"
            "}\n"
            "+Error = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"TrackError\"\n"
            "+Par1 = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"uint32\"\n"
            "Path = \"DDB1.PidError1\"\n"
            "Default = \"1\"\n"
            "}\n"
            "+Par2 = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"uint32\"\n"
            "Path = \"DDB1.PidError2\"\n"
            "Default = \"2\"\n"
            "}\n"
            "}\n"
            "}\n";

    printf("|%s|\n|%s|", display.Buffer(), testOut.Buffer());

    return testOut == display;

}

bool GAMSignalsContainerTest::TestMergeWithLocal_AddDefinition() {
    ConfigurationDatabase localData;

    localData.Write("Class", "GAMGenericSignalContainer");
    localData.Write("IsFinal", "false");
    localData.Write("IsInput", "true");
    localData.CreateAbsolute("+Control");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Control.+Par1");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");

    localData.CreateAbsolute("+Error");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "TrackError");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Error.+Par1");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Default", "1");
    localData.Write("Path", "DDB1.PidError1");
    localData.Write("IsFinal", "true");
    localData.CreateAbsolute("+Error.+Par2");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Default", "2");
    localData.Write("Path", "DDB1.PidError2");
    localData.Write("IsFinal", "true");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "GAMGenericSignalContainer");
    cdb.Write("IsFinal", "false");
    cdb.Write("IsInput", "true");
    cdb.CreateAbsolute("+Control");
    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Control.+Par2");
    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<GAMSignalsContainer> test = ReferenceT<GAMSignalsContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Inputs");
    if (!test->Initialise(cdb)) {
        return false;
    }
    if (!test->MergeWithLocal(localData)) {
        return false;
    }

    ConfigurationDatabase out;
    test->ToStructuredData(out);

    //out.MoveToRoot();
    StreamString display;

    display.Printf("%!", out);

    StreamString testOut = "+Inputs = {\n"
            "Class = \"GAMSignalsContainer\"\n"
            "IsInput = \"true\"\n"
            "+Control = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"ControlIn\"\n"
            "+Par2 = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"uint32\"\n"
            "Path = \"PidControl2\"\n"
            "Default = \"1\"\n"
            "}\n"
            "+Par1 = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"uint32\"\n"
            "Path = \"DDB2.PidControl1\"\n"
            "Default = \"1\"\n"
            "}\n"
            "}\n"
            "+Error = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"TrackError\"\n"
            "+Par1 = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"uint32\"\n"
            "Path = \"DDB1.PidError1\"\n"
            "Default = \"1\"\n"
            "}\n"
            "+Par2 = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"uint32\"\n"
            "Path = \"DDB1.PidError2\"\n"
            "Default = \"2\"\n"
            "}\n"
            "}\n"
            "}\n";

    printf("|%s|\n|%s|", display.Buffer(), testOut.Buffer());

    return testOut == display;
}

bool GAMSignalsContainerTest::TestMergeWithLocalFalse_GlobalFinal() {

    ConfigurationDatabase localData;

    localData.Write("Class", "GAMGenericSignalContainer");
    localData.Write("IsFinal", "false");
    localData.Write("IsInput", "true");
    localData.CreateAbsolute("+Control");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Control.+Par1");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");

    localData.CreateAbsolute("+Error");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "TrackError");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Error.+Par2");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Default", "2");
    localData.Write("Path", "DDB1.PidError2");
    localData.Write("IsFinal", "true");

    localData.CreateAbsolute("+Error.+Par1");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Default", "1");
    localData.Write("Path", "DDB1.PidError1");
    localData.Write("IsFinal", "true");

    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "GAMGenericSignalContainer");
    cdb.Write("IsFinal", "true");
    cdb.Write("IsInput", "true");
    cdb.CreateAbsolute("+Control");
    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Control.+Par2");
    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");

    ReferenceT<GAMSignalsContainer> test = ReferenceT<GAMSignalsContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Inputs");
    if (!test->Initialise(cdb)) {
        return false;
    }

    return (!test->MergeWithLocal(localData));
}

bool GAMSignalsContainerTest::TestMergeWithLocal_AddInput() {
    ConfigurationDatabase localData;

    localData.Write("Class", "GAMGenericSignalContainer");
    localData.Write("IsFinal", "false");
    localData.Write("IsInput", "true");
    localData.CreateAbsolute("+Control");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Control.+Par1");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");

    localData.CreateAbsolute("+Error");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "TrackError");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Error.+Par1");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Default", "1");
    localData.Write("Path", "DDB1.PidError1");
    localData.Write("IsFinal", "true");
    localData.CreateAbsolute("+Error.+Par2");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Default", "2");
    localData.Write("Path", "DDB1.PidError2");
    localData.Write("IsFinal", "true");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "GAMGenericSignalContainer");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Control");
    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Control.+Par2");
    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<GAMSignalsContainer> test = ReferenceT<GAMSignalsContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Inputs");
    if (!test->Initialise(cdb)) {
        return false;
    }
    if (!test->MergeWithLocal(localData)) {
        return false;
    }

    ConfigurationDatabase out;
    test->ToStructuredData(out);

    //out.MoveToRoot();
    StreamString display;

    display.Printf("%!", out);

    StreamString testOut = "+Inputs = {\n"
            "Class = \"GAMSignalsContainer\"\n"
            "IsInput = \"true\"\n"
            "+Control = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"ControlIn\"\n"
            "+Par2 = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"uint32\"\n"
            "Path = \"PidControl2\"\n"
            "Default = \"1\"\n"
            "}\n"
            "+Par1 = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"uint32\"\n"
            "Path = \"DDB2.PidControl1\"\n"
            "Default = \"1\"\n"
            "}\n"
            "}\n"
            "+Error = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"TrackError\"\n"
            "+Par1 = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"uint32\"\n"
            "Path = \"DDB1.PidError1\"\n"
            "Default = \"1\"\n"
            "}\n"
            "+Par2 = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"uint32\"\n"
            "Path = \"DDB1.PidError2\"\n"
            "Default = \"2\"\n"
            "}\n"
            "}\n"
            "}\n";

    printf("|%s|\n|%s|", display.Buffer(), testOut.Buffer());

    return testOut == display;
}

bool GAMSignalsContainerTest::TestMergeWithLocal_AddOutput() {
    ConfigurationDatabase localData;

    localData.Write("Class", "GAMGenericSignalContainer");
    localData.Write("IsFinal", "false");
    localData.Write("IsOutput", "true");
    localData.CreateAbsolute("+Control");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Control.+Par1");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");

    localData.CreateAbsolute("+Error");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "TrackError");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Error.+Par1");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Default", "1");
    localData.Write("Path", "DDB1.PidError1");
    localData.Write("IsFinal", "true");
    localData.CreateAbsolute("+Error.+Par2");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Default", "2");
    localData.Write("Path", "DDB1.PidError2");
    localData.Write("IsFinal", "true");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "GAMGenericSignalContainer");
    cdb.Write("IsFinal", "false");
    cdb.Write("IsInput", "true");
    cdb.CreateAbsolute("+Control");
    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Control.+Par2");
    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<GAMSignalsContainer> test = ReferenceT<GAMSignalsContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Inputs");
    if (!test->Initialise(cdb)) {
        return false;
    }

    if (!test->MergeWithLocal(localData)) {
        return false;
    }

    ConfigurationDatabase out;
    test->ToStructuredData(out);

    //out.MoveToRoot();
    StreamString display;

    display.Printf("%!", out);

    StreamString testOut = "+Inputs = {\n"
            "Class = \"GAMSignalsContainer\"\n"
            "IsInput = \"true\"\n"
            "IsOutput = \"true\"\n"
            "+Control = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"ControlIn\"\n"
            "+Par2 = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"uint32\"\n"
            "Path = \"PidControl2\"\n"
            "Default = \"1\"\n"
            "}\n"
            "+Par1 = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"uint32\"\n"
            "Path = \"DDB2.PidControl1\"\n"
            "Default = \"1\"\n"
            "}\n"
            "}\n"
            "+Error = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"TrackError\"\n"
            "+Par1 = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"uint32\"\n"
            "Path = \"DDB1.PidError1\"\n"
            "Default = \"1\"\n"
            "}\n"
            "+Par2 = {\n"
            "Class = \"GAMGenericSignal\"\n"
            "Type = \"uint32\"\n"
            "Path = \"DDB1.PidError2\"\n"
            "Default = \"2\"\n"
            "}\n"
            "}\n"
            "}\n";

    printf("|%s|\n|%s|", display.Buffer(), testOut.Buffer());

    return testOut == display;
}

bool GAMSignalsContainerTest::TestVerify() {
    ConfigurationDatabase localData;

    localData.Write("Class", "GAMGenericSignalContainer");
    localData.Write("IsFinal", "false");
    localData.Write("IsInput", "true");
    localData.CreateAbsolute("+Control");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Control.+Par1");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");

    localData.CreateAbsolute("+Error");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "TrackError");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Error.+Par1");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Default", "1");
    localData.Write("Path", "DDB1.PidError1");
    localData.Write("IsFinal", "true");
    localData.CreateAbsolute("+Error.+Par2");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Default", "2");
    localData.Write("Path", "DDB1.PidError2");
    localData.Write("IsFinal", "true");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "GAMGenericSignalContainer");
    cdb.Write("IsFinal", "false");
    cdb.Write("IsInput", "true");
    cdb.CreateAbsolute("+Control");
    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Control.+Par2");
    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<GAMSignalsContainer> test = ReferenceT<GAMSignalsContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Inputs");
    if (!test->Initialise(cdb)) {
        return false;
    }
    if (!test->MergeWithLocal(localData)) {
        return false;
    }

    return test->Verify();
}

bool GAMSignalsContainerTest::TestVerify_DefInvalid() {
    ConfigurationDatabase localData;

    localData.Write("Class", "GAMGenericSignalContainer");
    localData.Write("IsFinal", "false");
    localData.Write("IsInput", "true");
    localData.CreateAbsolute("+Control");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Control.+Par1");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");

    localData.CreateAbsolute("+Error");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "Invalid");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Error.+Par1");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Default", "1");
    localData.Write("Path", "DDB1.PidError1");
    localData.Write("IsFinal", "true");
    localData.CreateAbsolute("+Error.+Par2");
    localData.Write("Class", "GAMGenericSignal");
    localData.Write("Type", "uint32");
    localData.Write("Default", "2");
    localData.Write("Path", "DDB1.PidError2");
    localData.Write("IsFinal", "true");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "GAMGenericSignalContainer");
    cdb.Write("IsFinal", "false");
    cdb.Write("IsInput", "true");
    cdb.CreateAbsolute("+Control");
    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Control.+Par2");
    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<GAMSignalsContainer> test = ReferenceT<GAMSignalsContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Inputs");
    if (!test->Initialise(cdb)) {
        return false;
    }
    if (!test->MergeWithLocal(localData)) {
        return false;
    }

    return !test->Verify();
}

bool GAMSignalsContainerTest::TestIsInput() {
    GAMSignalsContainer def;
    if (def.IsInput()) {
        return false;
    }

    ConfigurationDatabase cdb;
    cdb.Write("IsInput", "true");

    if (!def.Initialise(cdb)) {
        return false;
    }

    return def.IsInput();
}

bool GAMSignalsContainerTest::TestIsOutput() {
    GAMSignalsContainer def;
    if (def.IsOutput()) {
        return false;
    }

    ConfigurationDatabase cdb;
    cdb.Write("IsOutput", "true");

    if (!def.Initialise(cdb)) {
        return false;
    }

    return def.IsOutput();
}

bool GAMSignalsContainerTest::TestToStructuredData() {

    return TestMergeWithLocal();

}
