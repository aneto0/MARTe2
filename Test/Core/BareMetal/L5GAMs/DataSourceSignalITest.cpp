/**
 * @file DataSourceSignalITest.cpp
 * @brief Source file for class DataSourceSignalITest
 * @date 22/apr/2016
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
 * the class DataSourceSignalITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "DataSourceSignalITest.h"
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

bool DataSourceSignalITest::TestConstructor() {

    DataSourceSignal dsSignal;

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

bool DataSourceSignalITest::TestInitialise() {
    ConfigurationDatabase cdb;
    cdb.Write("Class", "DataSourceSignal");
    cdb.Write("Samples", "10");
    cdb.Write("Type", "uint32");
    cdb.Write("Dimensions", "[1][3]");
    cdb.Write("Default", "{{1,2,3}}");

    DataSourceSignal signal;

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


bool DataSourceSignalITest::TestAddConsumer() {
    ReferenceT<PIDGAM> gam1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam1->SetName("gam1");

    gam1->AddState("state1", "thread1");

    DataSourceSignal dsSignal;
    if (!dsSignal.AddConsumer(gam1)) {
        return false;
    }

    return (dsSignal.GetNumberOfConsumers("state1") == 1) && (((ReferenceT<DataSourceSignalRecord> ) dsSignal.Get(0))->GetConsumers()->Get(0) == gam1);
}

bool DataSourceSignalITest::TestAddProducer() {
    ReferenceT<PIDGAM> gam1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam1->SetName("gam1");

    gam1->AddState("state1", "thread1");

    DataSourceSignal dsSignal;
    if (!dsSignal.AddProducer(gam1)) {
        return false;
    }

    return (dsSignal.GetNumberOfProducers("state1") == 1) && (((ReferenceT<DataSourceSignalRecord> ) dsSignal.Get(0))->GetProducers()->Get(0) == gam1);
}


bool DataSourceSignalITest::TestGetNumberOfConsumers(uint32 nConsumers) {
    DataSourceSignal signal;
    for (uint32 i = 0u; i < nConsumers; i++) {
        ReferenceT<PIDGAM> gamS1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        gamS1->AddState("state1", "thread1");
        gamS1->AddState("state3", "thread1");

        ReferenceT<PIDGAM> gamS2 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        gamS2->AddState("state2", "thread1");
        gamS2->AddState("state3", "thread1");

        signal.AddConsumer(gamS1);
        signal.AddConsumer(gamS2);
    }

    if (signal.GetNumberOfConsumers("state1") != nConsumers) {
        return false;
    }

    if (signal.GetNumberOfConsumers("state2") != nConsumers) {
        return false;
    }

    return signal.GetNumberOfConsumers("state3") == 2 * nConsumers;

}

bool DataSourceSignalITest::TestGetNumberOfProducers(uint32 nProducers) {
    DataSourceSignal signal;
    for (uint32 i = 0u; i < nProducers; i++) {
        ReferenceT<PIDGAM> gamS1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        gamS1->AddState("state1", "thread1");
        gamS1->AddState("state3", "thread1");

        ReferenceT<PIDGAM> gamS2 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        gamS2->AddState("state2", "thread1");
        gamS2->AddState("state3", "thread1");

        signal.AddProducer(gamS1);
        signal.AddProducer(gamS2);
    }

    if (signal.GetNumberOfProducers("state1") != nProducers) {
        return false;
    }

    if (signal.GetNumberOfProducers("state2") != nProducers) {
        return false;
    }

    return signal.GetNumberOfProducers("state3") == 2 * nProducers;
}

bool DataSourceSignalITest::TestVerify() {
    DataSourceSignal signal;
    uint32 nConsumers = 32;
    for (uint32 i = 0u; i < nConsumers; i++) {
        ReferenceT<PIDGAM> gamS1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        gamS1->AddState("state1", "thread1");
        gamS1->AddState("state3", "thread1");

        ReferenceT<PIDGAM> gamS2 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        gamS2->AddState("state2", "thread1");
        gamS2->AddState("state3", "thread1");

        signal.AddConsumer(gamS1);
        signal.AddConsumer(gamS2);
    }

    ReferenceT<PIDGAM> gamP = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gamP->AddState("state1", "thread2");
    gamP->AddState("state2", "thread2");
    ReferenceT<PIDGAM> gamP2 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gamP2->AddState("state3", "thread2");
    signal.AddProducer(gamP);
    signal.AddProducer(gamP2);

    return (signal.Verify());
}

bool DataSourceSignalITest::TestVerifyFalse_TwoProducers() {
    DataSourceSignal signal;
    uint32 nConsumers = 32;
    for (uint32 i = 0u; i < nConsumers; i++) {
        ReferenceT<PIDGAM> gamS1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        gamS1->AddState("state1", "thread1");
        gamS1->AddState("state3", "thread1");

        ReferenceT<PIDGAM> gamS2 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        gamS2->AddState("state2", "thread1");
        gamS2->AddState("state3", "thread1");

        signal.AddConsumer(gamS1);
        signal.AddConsumer(gamS2);
    }

    ReferenceT<PIDGAM> gamP1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gamP1->AddState("state1", "thread2");
    gamP1->AddState("state2", "thread2");
    gamP1->AddState("state3", "thread2");
    ReferenceT<PIDGAM> gamP2 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gamP2->AddState("state1", "thread2");
    signal.AddProducer(gamP1);
    signal.AddProducer(gamP2);
    return !(signal.Verify());

}

bool DataSourceSignalITest::TestGetType() {

    ReferenceT<GAMGenericSignal> def = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;

    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Type", "uint32");

    if (!def->Initialise(cdb)) {
        return false;
    }

    DataSourceSignal dsSignal;
    dsSignal.Configure(def);

    return StringHelper::Compare("uint32", dsSignal.GetType()) == 0;
}

bool DataSourceSignalITest::TestGetNumberOfElements() {
    ReferenceT<GAMGenericSignal> def = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;

    cdb.Write("Class", "GAMGenericSignal");
    cdb.Write("Dimensions", "[2][3]");

    if (!def->Initialise(cdb)) {
        return false;
    }

    DataSourceSignal dsSignal;
    dsSignal.Configure(def);

    return dsSignal.GetNumberOfElements() == 6;
}

bool DataSourceSignalITest::TestGetNumberOfElements_DifferentSizes() {
    ReferenceT<GAMGenericSignal> def1 = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb1;

    cdb1.Write("Class", "GAMGenericSignal");
    cdb1.Write("Dimensions", "[2][3]");

    if (!def1->Initialise(cdb1)) {
        return false;
    }
    DataSourceSignal dsSignal;
    dsSignal.Configure(def1);
    if (dsSignal.GetNumberOfElements() != 6) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def2 = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb2;

    cdb2.Write("Class", "GAMGenericSignal");
    cdb2.Write("Dimensions", "[3][3]");

    if (!def2->Initialise(cdb2)) {
        return false;
    }
    dsSignal.Configure(def2);
    if (dsSignal.GetNumberOfElements() != 9) {
        return false;
    }
    ReferenceT<GAMGenericSignal> def3 = ReferenceT<GAMGenericSignal>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb3;

    cdb3.Write("Class", "GAMGenericSignal");
    cdb3.Write("Dimensions", "[3]");

    if (!def3->Initialise(cdb3)) {
        return false;
    }
    dsSignal.Configure(def3);

    return dsSignal.GetNumberOfElements() == 9;
}


bool DataSourceSignalITest::TestGetNumberOfSamples() {
    DataSourceSignal signal;
    return signal.GetNumberOfSamples() == 1;
}
