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
    return !test.IsFinal();
}

bool RealTimeGenericDataDefTest::TestInitialise(StructuredDataI & data,
                                                bool expected) {

    RealTimeGenericDataDef test;
    return test.Initialise(data) == expected;
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

    printf("%s", display.Buffer());

    return true;
}
