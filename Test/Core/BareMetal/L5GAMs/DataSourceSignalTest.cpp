/**
 * @file DataSourceSignalTest.cpp
 * @brief Source file for class DataSourceSignalTest
 * @date 13/apr/2016
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
 * the class DataSourceSignalTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DataSourceSignalTest.h"
#include "GAMTestHelper.h"
#include "DataSourceSignalRecord.h"
#include "GAMGenericSignal.h"
#include "RealTimeApplication.h"
#include "DataSourceContainer.h"
#include "MemoryMapInputReader.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool DataSourceSignalTest::TestConstructor() {

    GAMDataSource dsSignal;

    for (uint32 k = 0u; k < 3u; k++) {
        if (dsSignal.GetNumberOfElements() != 1u) {
            return false;
        }
    }

    if (dsSignal.GetNumberOfSamples() != 1) {
        return false;
    }

    return (dsSignal.GetDataSourcePointer(0) == NULL) && (dsSignal.GetDataSourcePointer(1) == NULL);

}

bool DataSourceSignalTest::TestInitialise() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "DataSourceSignal");
    cdb.Write("Samples", "10");
    cdb.Write("Type", "uint32");
    cdb.Write("Dimensions", "[1][3]");
    cdb.Write("Default", "{{1,2,3}}");

    GAMDataSource signal;

    if (!signal.Initialise(cdb)) {
        return false;
    }

    if (signal.GetNumberOfSamples() != 10) {
        return false;
    }

    if (StringHelper::Compare(signal.GetType(), "uint32") != 0) {
        return false;
    }

    if (signal.GetNumberOfElements() != 3) {
        return false;
    }

    return true;
}

bool DataSourceSignalTest::TestToStructuredData() {

    StreamString confString = "Class = \"DataSourceSignal\"\n"
            "Type = \"uint32\"\n"
            "Default = \"{{1,2,3}}\"\n"
            "Samples = +10\n"
            "Dimensions = \"[1][3]\"\n";

    confString.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(confString, cdb);
    if (!parser.Parse()) {
        return false;
    }

    GAMDataSource signal;
    signal.SetName("MySignal");

    if (!signal.Initialise(cdb)) {
        return false;
    }

    ConfigurationDatabase out;
    if (!signal.ToStructuredData(out)) {
        return false;
    }

    StreamString display;
    display.Printf("%!", out);

    StreamString test = "+MySignal = {\n";
    test += confString;
    test += "}\n";

    printf("\n|%s|\n|%s|\n", test.Buffer(), display.Buffer());
    return test == display;
}


bool DataSourceSignalTest::TestConfigure() {
    ReferenceT<GAMGenericSignal> def1 = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb1;

    cdb1.Write("Class", "GAMGenericSignal");
    cdb1.Write("Dimensions", "[2][3]");

    if (!def1->Initialise(cdb1)) {
        return false;
    }
    GAMDataSource dsSignal;
    if (!dsSignal.Configure(def1)) {
        return false;
    }

    if (dsSignal.GetNumberOfElements() != 6) {
        return false;
    }


    if (StringHelper::Compare(dsSignal.GetType(), "") != 0) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def2 = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb2;

    cdb2.Write("Class", "GAMGenericSignal");
    cdb2.Write("Dimensions", "[3][3]");
    cdb2.Write("Type", "uint32");

    if (!def2->Initialise(cdb2)) {
        return false;
    }
    if (!dsSignal.Configure(def2)) {
        return false;
    }

    if (dsSignal.GetNumberOfElements() != 9) {
        return false;
    }


    if (StringHelper::Compare(dsSignal.GetType(), "uint32") != 0) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def3 = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb3;

    cdb3.Write("Class", "GAMGenericSignal");
    cdb3.Write("Dimensions", "[3]");
    cdb3.Write("Type", "uint32");
    cdb3.Write("Default", "{1,2,3}");

    if (!def3->Initialise(cdb3)) {
        return false;
    }

    if (!dsSignal.Configure(def3)) {
        return false;
    }

    if (dsSignal.GetNumberOfElements() != 9) {
        return false;
    }


    if (StringHelper::Compare(dsSignal.GetType(), "uint32") != 0) {
        return false;
    }
    return true;
}

bool DataSourceSignalTest::TestConfigureFalse_AlreadyTypeSet() {
    ReferenceT<GAMGenericSignal> def1 = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb1;

    cdb1.Write("Class", "GAMGenericSignal");
    cdb1.Write("Dimensions", "[2][3]");
    cdb1.Write("Type", "float32");

    if (!def1->Initialise(cdb1)) {
        return false;
    }
    GAMDataSource dsSignal;
    if (!dsSignal.Configure(def1)) {
        return false;
    }

    if (dsSignal.GetNumberOfElements() != 6) {
        return false;
    }


    if (StringHelper::Compare(dsSignal.GetType(), "float32") != 0) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def2 = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb2;

    cdb2.Write("Class", "GAMGenericSignal");
    cdb2.Write("Dimensions", "[3][3]");
    cdb2.Write("Type", "uint32");

    if (!def2->Initialise(cdb2)) {
        return false;
    }
    return !dsSignal.Configure(def2);
}

bool DataSourceSignalTest::TestConfigureFalse_AlreadyDefaultSet() {
    ReferenceT<GAMGenericSignal> def1 = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb1;

    cdb1.Write("Class", "GAMGenericSignal");
    cdb1.Write("Dimensions", "[2][3]");
    cdb1.Write("Default", "a");

    if (!def1->Initialise(cdb1)) {
        return false;
    }
    GAMDataSource dsSignal;
    if (!dsSignal.Configure(def1)) {
        return false;
    }

    if (dsSignal.GetNumberOfElements() != 6) {
        return false;
    }


    ReferenceT<GAMGenericSignal> def2 = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb2;

    cdb2.Write("Class", "GAMGenericSignal");
    cdb2.Write("Dimensions", "[3][3]");
    cdb2.Write("Default", "b");

    if (!def2->Initialise(cdb2)) {
        return false;
    }
    return !dsSignal.Configure(def2);
}


bool DataSourceSignalTest::TestWriteStart() {
    GAMDataSource signal;
    signal.WriteStart();
    return true;
}

bool DataSourceSignalTest::TestReadStart() {
    GAMDataSource signal;
    signal.ReadStart();
    return true;
}

bool DataSourceSignalTest::TestWriteEnd() {
    GAMDataSource signal;
    signal.WriteEnd();
    return true;
}

bool DataSourceSignalTest::TestReadEnd() {
    GAMDataSource signal;
    signal.ReadEnd();
    return true;
}

bool DataSourceSignalTest::TestWaitOnEvent() {
    GAMDataSource signal;
    signal.WaitOnEvent();
    return true;

}

bool DataSourceSignalTest::TestAllocate() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "DataSourceSignal");
    cdb.Write("Samples", "10");
    cdb.Write("Type", "uint32");
    cdb.Write("Dimensions", "[1][3]");
    cdb.Write("Default", "{{1,2,3}}");

    GAMDataSource signal;

    if (!signal.Initialise(cdb)) {
        return false;
    }

    MemoryArea mem;
    if (!signal.Allocate(mem)) {
        return false;
    }

    // 2 buffers * 3 * 10 * sizeof(uint32)
    if (mem.GetMemorySize() != 240) {
        return false;
    }
    return true;
}

bool DataSourceSignalTest::TestAllocateStructure() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "DataSourceSignal");
    cdb.Write("Samples", "10");
    cdb.Write("Type", "ControlIn");
    cdb.Write("Default", "Par1=1 Par2=2");
    GAMDataSource signal;

    if (!signal.Initialise(cdb)) {
        return false;
    }

    MemoryArea mem;
    if (!signal.Allocate(mem)) {
        return false;
    }

    if (mem.GetMemorySize() != sizeof(ControlIn) * 10 * 2) {
        return false;
    }
    return true;
}

bool DataSourceSignalTest::TestAllocateStructureFalse_NotRegisteredType() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "DataSourceSignal");
    cdb.Write("Samples", "10");
    cdb.Write("Type", "Invalid");
    cdb.Write("Default", "Par1=1 Par2=2");
    GAMDataSource signal;

    if (!signal.Initialise(cdb)) {
        return false;
    }

    MemoryArea mem;
    return (!signal.Allocate(mem));
}

bool DataSourceSignalTest::TestAllocateStructureFalse_MultiDim() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "DataSourceSignal");
    cdb.Write("Samples", "10");
    cdb.Write("Type", "Invalid");
    cdb.Write("Dimensions", "[2][3]");
    cdb.Write("Default", "Par1=1 Par2=2");
    GAMDataSource signal;

    if (!signal.Initialise(cdb)) {
        return false;
    }

    MemoryArea mem;
    return (!signal.Allocate(mem));
}

bool DataSourceSignalTest::TestGetDataSourcePointer() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "DataSourceSignal");
    cdb.Write("Samples", "10");
    cdb.Write("Type", "uint32");
    cdb.Write("Dimensions", "[1][3]");
    cdb.Write("Default", "{{1,2,3}}");

    GAMDataSource signal;

    if (!signal.Initialise(cdb)) {
        return false;
    }

    MemoryArea mem;
    if (!signal.Allocate(mem)) {
        return false;
    }

    if (signal.GetDataSourcePointer(0) == NULL) {
        return false;
    }

    if (signal.GetDataSourcePointer(1) == NULL) {
        return false;
    }

    return (((char8*) *signal.GetDataSourcePointer(0)) + 120) == ((char8*) *signal.GetDataSourcePointer(1));

}

bool DataSourceSignalTest::TestPrepareNextState() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "DataSourceSignal");
    cdb.Write("Type", "uint32");
    cdb.Write("Default", "1");

    GAMDataSource signal;

    if (!signal.Initialise(cdb)) {
        return false;
    }

    MemoryArea mem;
    if (!signal.Allocate(mem)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->AddState("state1", "thread1");

    signal.AddConsumer(gam);

    RealTimeStateInfo info;
    info.currentState = "";
    info.nextState = "state1";
    info.activeBuffer = 1;
    if (!signal.PrepareNextState(info)) {
        return false;
    }

    uint32 * beg = (uint32*) mem.GetMemoryStart();

    return *beg == 1;
}

bool DataSourceSignalTest::TestPrepareNextStateStructure() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "DataSourceSignal");
    cdb.Write("Type", "ControlIn");
    cdb.Write("Default", "Par1=1 Par2=2");

    GAMDataSource signal;

    if (!signal.Initialise(cdb)) {
        return false;
    }

    MemoryArea mem;
    if (!signal.Allocate(mem)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->AddState("state1", "thread1");

    signal.AddConsumer(gam);

    RealTimeStateInfo info;
    info.currentState = "";
    info.nextState = "state1";
    info.activeBuffer = 1;
    if (!signal.PrepareNextState(info)) {
        return false;
    }

    ControlIn * beg = (ControlIn*) mem.GetMemoryStart();

    return (beg->Par1 == 1) && (beg->Par2 == 2);
}

bool DataSourceSignalTest::TestPrepareNextStateStructure_MultiSample() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "DataSourceSignal");
    cdb.Write("Type", "ControlIn");
    cdb.Write("Default", "Par1=1 Par2=2");
    cdb.Write("Samples", "10");

    GAMDataSource signal;

    if (!signal.Initialise(cdb)) {
        return false;
    }

    MemoryArea mem;
    if (!signal.Allocate(mem)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->AddState("state1", "thread1");

    signal.AddConsumer(gam);

    RealTimeStateInfo info;
    info.currentState = "";
    info.nextState = "state1";
    info.activeBuffer = 1;
    if (!signal.PrepareNextState(info)) {
        return false;
    }

    ControlIn * beg = (ControlIn*) mem.GetMemoryStart();

    for (uint32 i = 0; i < 10; i++) {
        if ((beg->Par1 != 1) || (beg->Par2 != 2)) {
            return false;
        }
        beg++;
    }
    return true;
}

bool DataSourceSignalTest::TestPrepareNextState_MultiDim_MultiSample() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "DataSourceSignal");
    cdb.Write("Samples", "10");
    cdb.Write("Type", "uint32");
    cdb.Write("Dimensions", "[1][3]");
    cdb.Write("Default", "{{1,2,3}}");

    GAMDataSource signal;

    if (!signal.Initialise(cdb)) {
        return false;
    }

    MemoryArea mem;
    if (!signal.Allocate(mem)) {
        return false;
    }

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->AddState("state1", "thread1");

    signal.AddConsumer(gam);

    RealTimeStateInfo info;
    info.currentState = "";
    info.nextState = "state1";
    info.activeBuffer = 1;
    if (!signal.PrepareNextState(info)) {
        return false;
    }

    uint32 * begMatrix = (uint32*) mem.GetMemoryStart();

    for (uint32 i = 0; i < 10; i += 3) {

        if (begMatrix[i] != 1) {
            printf("\n0 %d\n", begMatrix[i]);
            return false;
        }
        if (begMatrix[i + 1] != 2) {
            printf("\n1 %d\n", begMatrix[i + 1]);
            return false;
        }
        if (begMatrix[i + 2] != 3) {
            printf("\n2 %d\n", begMatrix[i + 2]);
            return false;
        }

    }

    return true;
}

bool DataSourceSignalTest::TestGetInputReader() {
    ReferenceT<GAMGenericSignal> gamSignal = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "DDB.var");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");

    if (!gamSignal->Initialise(cdb)) {
        return false;
    }

    ConfigurationDatabase dataCDB;
    dataCDB.CreateAbsolute("+Data");
    dataCDB.Write("Class", "DataSourceContainer");
    dataCDB.CreateAbsolute("+Data.+DDB");
    dataCDB.Write("Class", "DataSource");
    dataCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> app = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ;
    if (!app->Initialise(dataCDB)) {
        return false;
    }

    ReferenceT<GAMDataSource> dsSignal = ReferenceT<GAMDataSource>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    dsSignal->SetName("var");
    if (!dsSignal->Configure(gamSignal)) {
        return false;
    }

    ReferenceT<DataSourceContainer> data = app->Get(0);
    printf("\n%s\n", data->GetName());
    ((ReferenceT<DataSource> ) data->Get(0))->Insert(dsSignal);
    MemoryArea mem;
    if (!dsSignal->Allocate(mem)) {
        return false;
    }

    dsSignal->SetApplication(*app.operator ->());

    ReferenceT<MemoryMapInputReader> reader = dsSignal->GetInputReader(gamSignal);

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->AddState("state1", "thread1");
    dsSignal->AddConsumer(gam);

    RealTimeStateInfo info;
    info.activeBuffer = 1;
    info.currentState = "";
    info.nextState = "state1";

    if (!dsSignal->PrepareNextState(info)) {
        return false;
    }

    if (!reader->AddSignal(gamSignal)) {
        return false;
    }

    if (!reader->Finalise()) {
        return false;
    }

    reader->Read(0);
    uint32 * var = (uint32 *) reader->GetSignal(0);
    return *var == 1;
}

bool DataSourceSignalTest::TestGetOutputWriter() {
    ReferenceT<GAMGenericSignal> gamSignal = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "DDB.var");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");

    if (!gamSignal->Initialise(cdb)) {
        return false;
    }

    ConfigurationDatabase dataCDB;
    dataCDB.CreateAbsolute("+Data");
    dataCDB.Write("Class", "DataSourceContainer");
    dataCDB.CreateAbsolute("+Data.+DDB");
    dataCDB.Write("Class", "DataSource");
    dataCDB.MoveToRoot();

    ReferenceT<RealTimeApplication> app = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ;
    if (!app->Initialise(dataCDB)) {
        return false;
    }

    ReferenceT<GAMDataSource> dsSignal = ReferenceT<GAMDataSource>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    dsSignal->SetName("var");
    if (!dsSignal->Configure(gamSignal)) {
        return false;
    }

    ReferenceT<DataSourceContainer> data = app->Get(0);
    printf("\n%s\n", data->GetName());
    ((ReferenceT<DataSource> ) data->Get(0))->Insert(dsSignal);
    MemoryArea mem;
    if (!dsSignal->Allocate(mem)) {
        return false;
    }

    dsSignal->SetApplication(*app.operator ->());

    ReferenceT<MemoryMapOutputWriter> writer = dsSignal->GetOutputWriter(gamSignal);

    ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->AddState("state1", "thread1");
    dsSignal->AddConsumer(gam);

    if (!writer->AddSignal(gamSignal)) {
        return false;
    }

    if (!writer->Finalise()) {
        return false;
    }

    uint32 * var = (uint32 *) writer->GetSignal(0);
    *var = 1;
    writer->Write(0);

    ReferenceT<MemoryMapInputReader> reader = dsSignal->GetInputReader(gamSignal);

    if (!reader->AddSignal(gamSignal)) {
        return false;
    }

    if (!reader->Finalise()) {
        return false;
    }

    reader->Read(0);
    return *var == 1;
}

