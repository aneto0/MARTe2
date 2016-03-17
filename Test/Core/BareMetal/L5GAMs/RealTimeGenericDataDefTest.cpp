/**
 * @file RealTimeGenericDataDefTest.cpp
 * @brief Source file for class RealTimeGenericDataDefTest
 * @date 03/mar/2016
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
 * the class RealTimeGenericDataDefTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeGenericDataDefTest.h"
#include "ConfigurationDatabase.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool RealTimeGenericDataDefTest::TestConstructor() {
    RealTimeGenericDataDef test;
    if (StringHelper::Compare(test.GetType(), "") != 0) {
        return false;
    }
    if (StringHelper::Compare(test.GetPath(), "") != 0) {
        return false;
    }
    return (StringHelper::Compare(test.GetDefaultValue(), "") == 0);
}

bool RealTimeGenericDataDefTest::TestInitialise() {

    RealTimeGenericDataDef test;
    ConfigurationDatabase data;
    data.Write("IsFinal", "true");
    data.Write("Default", "1");
    return test.Initialise(data);
}

bool RealTimeGenericDataDefTest::TestMergeWithLocal() {
    ConfigurationDatabase localData;

    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Par1");
    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Modifiers", "[2][3]");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
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

    StreamString testOut = "Control = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = ControlIn\n"
            "+Par2 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = PidControl2\n"
            "Default = 1\n"
            "NumberOfDimensions = 2\n"
            "NumberOfElements = { 3 2 1 } \n"
            "}\n"
            "+Par1 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = DDB2.PidControl1\n"
            "Default = 1\n"
            "}\n"
            "}\n";

    printf("|%s|\n|%s|", display.Buffer(), testOut.Buffer());

    return testOut == display;
}

bool RealTimeGenericDataDefTest::TestMergeWithLocalFalse_FinalGlobal() {
    ConfigurationDatabase localData;

    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Par1");
    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
    if (!test->Initialise(cdb)) {
        return false;
    }

    return (!test->MergeWithLocal(localData));
}

bool RealTimeGenericDataDefTest::TestMergeWithLocalFalse_BothFinal() {
    ConfigurationDatabase localData;

    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "true");
    localData.CreateAbsolute("+Par1");
    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
    if (!test->Initialise(cdb)) {
        return false;
    }

    return (!test->MergeWithLocal(localData));

}

bool RealTimeGenericDataDefTest::TestMergeWithLocal_AddType() {
    ConfigurationDatabase localData;

    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Par1");
    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par1");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Path", "DDB2.PidControl1");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "false");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
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

    StreamString testOut = "Control = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = ControlIn\n"
            "+Par2 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = PidControl2\n"
            "Default = 1\n"
            "}\n"
            "+Par1 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = DDB2.PidControl1\n"
            "Default = 1\n"
            "}\n"
            "}\n";

    printf("|%s|\n|%s|", display.Buffer(), testOut.Buffer());

    return testOut == display;

}

bool RealTimeGenericDataDefTest::TestMergeWithLocal_AddPath() {
    ConfigurationDatabase localData;

    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Par1");
    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par1");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Default", "1");
    cdb.Write("Type", "uint32");
    cdb.Write("IsFinal", "false");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
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

    StreamString testOut = "Control = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = ControlIn\n"
            "+Par2 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = PidControl2\n"
            "Default = 1\n"
            "}\n"
            "+Par1 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = DDB2.PidControl1\n"
            "Default = 1\n"
            "}\n"
            "}\n";

    printf("|%s|\n|%s|", display.Buffer(), testOut.Buffer());

    return testOut == display;
}

bool RealTimeGenericDataDefTest::TestMergeWithLocal_AddModifiers() {
    ConfigurationDatabase localData;

    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Par1");
    localData.Write("Modifiers", "[2][3]");
    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "false");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par1");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "DDB2.PidControl1");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "false");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
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

    StreamString testOut = "Control = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = ControlIn\n"
            "+Par2 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = PidControl2\n"
            "Default = 1\n"
            "}\n"
            "+Par1 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = DDB2.PidControl1\n"
            "Default = 1\n"
            "NumberOfDimensions = 2\n"
            "NumberOfElements = { 3 2 1 } \n"
            "}\n"
            "}\n";

    printf("|%s|\n|%s|", display.Buffer(), testOut.Buffer());

    return testOut == display;
}

bool RealTimeGenericDataDefTest::TestMergeWithLocal_AddDefaultValues() {
    ConfigurationDatabase localData;

    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Par1");
    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par1");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Path", "DDB2.PidControl1");
    cdb.Write("Type", "uint32");
    cdb.Write("IsFinal", "false");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
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

    StreamString testOut = "Control = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = ControlIn\n"
            "+Par2 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = PidControl2\n"
            "Default = 1\n"
            "}\n"
            "+Par1 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = DDB2.PidControl1\n"
            "Default = 1\n"
            "}\n"
            "}\n";

    printf("|%s|\n|%s|", display.Buffer(), testOut.Buffer());

    return testOut == display;
}

bool RealTimeGenericDataDefTest::TestMergeWithLocal_DifferentTypes() {
    ConfigurationDatabase localData;

    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Par1");
    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "float32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par1");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Default", "1");
    cdb.Write("Type", "uint32");
    cdb.Write("IsFinal", "false");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
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

    StreamString testOut = "Control = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = ControlIn\n"
            "+Par2 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = PidControl2\n"
            "Default = 1\n"
            "}\n"
            "+Par1 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = DDB2.PidControl1\n"
            "Default = 1\n"
            "}\n"
            "}\n";

    printf("|%s|\n|%s|", display.Buffer(), testOut.Buffer());

    return testOut == display;
}

bool RealTimeGenericDataDefTest::TestMergeWithLocal_DifferentPaths() {
    ConfigurationDatabase localData;

    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Par1");
    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB3.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par1");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Path", "DDB2.PidControl1");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "false");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
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

    StreamString testOut = "Control = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = ControlIn\n"
            "+Par2 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = PidControl2\n"
            "Default = 1\n"
            "}\n"
            "+Par1 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = DDB2.PidControl1\n"
            "Default = 1\n"
            "}\n"
            "}\n";

    printf("|%s|\n|%s|", display.Buffer(), testOut.Buffer());

    return testOut == display;

}

bool RealTimeGenericDataDefTest::TestMergeWithLocal_DifferentDefaultValues() {
    ConfigurationDatabase localData;

    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Par1");
    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "2");
    localData.Write("IsFinal", "true");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par1");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Path", "DDB2.PidControl1");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "false");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
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

    StreamString testOut = "Control = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = ControlIn\n"
            "+Par2 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = PidControl2\n"
            "Default = 1\n"
            "}\n"
            "+Par1 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = DDB2.PidControl1\n"
            "Default = 1\n"
            "}\n"
            "}\n";

    printf("|%s|\n|%s|", display.Buffer(), testOut.Buffer());

    return testOut == display;

}

bool RealTimeGenericDataDefTest::TestMergeWithLocal_DifferentModifiers() {
    ConfigurationDatabase localData;

    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Par1");
    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");
    localData.CreateAbsolute("+Par2");
    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "uint32");
    localData.Write("Path", "PidControl2");
    localData.Write("Modifiers", "[3]");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "false");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Modifiers", "[2][3]");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "false");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
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

    StreamString testOut = "Control = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = ControlIn\n"
            "+Par2 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = PidControl2\n"
            "Default = 1\n"
            "NumberOfDimensions = 2\n"
            "NumberOfElements = { 3 2 1 } \n"
            "}\n"
            "+Par1 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = DDB2.PidControl1\n"
            "Default = 1\n"
            "}\n"
            "}\n";

    printf("|%s|\n|%s|", display.Buffer(), testOut.Buffer());

    return testOut == display;
}

bool RealTimeGenericDataDefTest::TestGetDefaultValue() {
    ConfigurationDatabase cdb;
    const char8* defaultValue = "\"member_1=1\n"
            "member_2=2\n"
            "member_3=3\n"
            "member_4 = {\n"
            "a=3.2\n"
            "b=2.5\n"
            "}\n"
            "\"";

    cdb.Write("Default", defaultValue);

    RealTimeGenericDataDef test;
    test.Initialise(cdb);
    return StringHelper::Compare(defaultValue, test.GetDefaultValue()) == 0;
}

bool RealTimeGenericDataDefTest::TestToStructuredData() {
    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Modifiers","[2][2]");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
    if (!test->Initialise(cdb)) {
        return false;
    }

    ConfigurationDatabase out;
    if (!test->ToStructuredData(out)) {
        return false;
    }

    StreamString display;

    display.Printf("%!", out);

    StreamString testOut = "Control = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = ControlIn\n"
            "+Par2 = {\n"
            "Class = RealTimeGenericDataDef\n"
            "Type = uint32\n"
            "Path = PidControl2\n"
            "Default = 1\n"
            "NumberOfDimensions = 2\n"
            "NumberOfElements = { 2 2 1 } \n"
            "}\n"
            "}\n";

    printf("|%s|\n|%s|", display.Buffer(), testOut.Buffer());

    return testOut == display;

}

bool RealTimeGenericDataDefTest::TestVerify() {
    ConfigurationDatabase localData;

    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "ControlIn");
    localData.Write("IsFinal", "false");
    localData.CreateAbsolute("+Par1");
    localData.Write("Class", "RealTimeGenericDataDef");
    localData.Write("Type", "uint32");
    localData.Write("Path", "DDB2.PidControl1");
    localData.Write("Default", "1");
    localData.Write("IsFinal", "true");
    localData.MoveToRoot();

    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
    if (!test->Initialise(cdb)) {
        return false;
    }

    if (!test->MergeWithLocal(localData)) {
        return false;
    }

    return (test->Verify());
}

bool RealTimeGenericDataDefTest::TestVerifyFalse_UnregisteredType() {

    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "InvalidType");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Par1");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "DDB2.PidControl1");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");

    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
    if (!test->Initialise(cdb)) {
        return false;
    }

    return !(test->Verify());

}

bool RealTimeGenericDataDefTest::TestVerifyFalse_UnintrospectableType() {

    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "Object");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Par1");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "DDB2.PidControl1");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");

    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
    if (!test->Initialise(cdb)) {
        return false;
    }

    return !(test->Verify());
}

bool RealTimeGenericDataDefTest::TestVerifyFalse_InvalidMemberName() {
    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Par3");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "DDB2.PidControl1");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
    if (!test->Initialise(cdb)) {
        return false;
    }

    return !(test->Verify());

}

bool RealTimeGenericDataDefTest::TestVerifyFalse_InvalidMemberType() {
    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Par1");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "DDB2.PidControl1");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "float32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
    if (!test->Initialise(cdb)) {
        return false;
    }

    return !(test->Verify());

}

bool RealTimeGenericDataDefTest::TestVerifyFalse_InvalidNumberOfMembers() {
    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Par1");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "DDB2.PidControl1");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par3");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl3");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
    if (!test->Initialise(cdb)) {
        return false;
    }

    return !(test->Verify());

}

bool RealTimeGenericDataDefTest::TestVerifyFalse_EmptyType() {
    ConfigurationDatabase cdb;

    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("+Par1");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "DDB2.PidControl1");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");
    cdb.MoveToRoot();

    ReferenceT<RealTimeGenericDataDef> test = ReferenceT<RealTimeGenericDataDef>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->SetName("Control");
    if (!test->Initialise(cdb)) {
        return false;
    }

    return !(test->Verify());

}

