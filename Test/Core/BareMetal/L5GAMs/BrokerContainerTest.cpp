/**
 * @file BrokerContainerTest.cpp
 * @brief Source file for class BrokerContainerTest
 * @date 21/04/2016
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
 * the class BrokerContainerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BrokerContainerTest.h"
#include "MemoryMapInputReader.h"
#include "MemoryMapOutputWriter.h"
#include "DataSourceSignal.h"
#include "GAMTestHelper.h"
#include "GAMGenericSignal.h"
#include "GAMSampledSignal.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

class DummyReader: public MemoryMapInputReader {
public:
    CLASS_REGISTER_DECLARATION()

    virtual bool AddSignal(Reference def,
                             void * const ptr = NULL_PTR(void*)) {

        ReferenceT<GAMSignalI> defIn = def;
        if (def.IsValid()) {
            if (StringHelper::Compare(defIn->GetType(), "uint32") == 0) {
                return false;
            }
            else {
                return MemoryMapInputReader::AddSignal(def, ptr);
            }
        }

        return false;

    }

    virtual bool Read(const uint8 activeDataSourceBuffer,
                      const TimeoutType &timeout = TTInfiniteWait) {
        printf("\ndummy read\n");
        return MemoryMapInputReader::Read(activeDataSourceBuffer, timeout);
    }

};
CLASS_REGISTER(DummyReader, "1.0")

class DummyWriter: public MemoryMapOutputWriter {

public:
    CLASS_REGISTER_DECLARATION()

    virtual bool AddSignal(Reference def,
                             void * const ptr = NULL_PTR(void*)) {

        ReferenceT<GAMSignalI> defIn = def;
        if (def.IsValid()) {
            if (StringHelper::Compare(defIn->GetType(), "uint32") == 0) {
                return false;
            }
            else {
                return MemoryMapOutputWriter::AddSignal(def, ptr);
            }
        }

        return false;

    }

    virtual bool Write(const uint8 activeDataSourceBuffer,
                       const TimeoutType &timeout = TTInfiniteWait) {
        printf("\ndummy write\n");
        return MemoryMapOutputWriter::Write(activeDataSourceBuffer, timeout);
    }

};
CLASS_REGISTER(DummyWriter, "1.0")

class DummyDSSignal: public DataSourceSignal {
public:
    CLASS_REGISTER_DECLARATION()

    virtual Reference GetInputReader(Reference defIn,
                                     void * varPtr = NULL_PTR(void*)) {

        Reference ret;
        ReferenceT<DummyReader> dummy = ReferenceT<DummyReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        dummy->SetApplication(*application);
        if (dummy->AddSignal(defIn, varPtr)) {
            return dummy;
        }
        return ret;
    }

    virtual Reference GetOutputWriter(Reference defIn,
                                      void * varPtr = NULL_PTR(void*)) {

        Reference ret;
        ReferenceT<DummyWriter> dummy = ReferenceT<DummyWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        dummy->SetApplication(*application);

        if (dummy->AddSignal(defIn, varPtr)) {
            return dummy;
        }
        return ret;
    }
};
CLASS_REGISTER(DummyDSSignal, "1.0")

bool BrokerContainerTest::TestConstructor() {
    BrokerContainer bro;
    if (bro.GetNumberOfSignals() != 0) {
        return false;
    }
    if (bro.GetSignal(0) != NULL) {
        return false;
    }
    if (bro.GetSignalNumberOfSamples(0) != 0) {
        return false;
    }
    return (!bro.IsSync());
}

bool BrokerContainerTest::TestAddSignal() {

    StreamString appCFG = ""
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n"
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +DDB = {\n"
            "        Class = DataSource\n"
            "        +DummySignal = {\n"
            "            Class = DummyDSSignal\n"
            "        }\n"
            "     }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }

    StreamString defCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = true\n"
            "    +Var1 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var2 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "}\n";

    defCFG.Seek(0);
    ConfigurationDatabase defCDB;

    StandardParser parserDef(defCFG, defCDB);
    if (!parserDef.Parse()) {
        return false;
    }

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(defCDB)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def1 = gam->Find("Inputs.Var1");
    ReferenceT<GAMGenericSignal> def2 = gam->Find("Inputs.Var2");

    ReferenceT<BrokerContainer> broker = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    if (!broker->AddSignal(def2)) {
        return false;
    }

    if (!broker->AddSignal(def1)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    return broker->Size() == 2;
}

bool BrokerContainerTest::TestAddSignal_SameBrokerSupported() {

    StreamString appCFG = ""
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n"
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +DDB = {\n"
            "        Class = DataSource\n"
            "        +DummySignal = {\n"
            "            Class = DummyDSSignal\n"
            "        }\n"
            "     }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }

    StreamString defCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = true\n"
            "    +Var1 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var2 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "}\n";

    defCFG.Seek(0);
    ConfigurationDatabase defCDB;

    StandardParser parserDef(defCFG, defCDB);
    if (!parserDef.Parse()) {
        return false;
    }

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(defCDB)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def1 = gam->Find("Inputs.Var1");
    ReferenceT<GAMGenericSignal> def2 = gam->Find("Inputs.Var2");

    ReferenceT<BrokerContainer> broker = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    if (!broker->AddSignal(def1)) {
        return false;
    }

    if (!broker->AddSignal(def2)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    return broker->Size() == 1;
}

bool BrokerContainerTest::TestAddSignalFalse_NotSupported() {

    StreamString appCFG = ""
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n"
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +DDB = {\n"
            "        Class = DataSource\n"
            "        +DummySignal = {\n"
            "            Class = DummyDSSignal\n"
            "        }\n"
            "     }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }

    StreamString defCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = true\n"
            "    +Var1 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var2 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "}\n";

    defCFG.Seek(0);
    ConfigurationDatabase defCDB;

    StandardParser parserDef(defCFG, defCDB);
    if (!parserDef.Parse()) {
        return false;
    }

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(defCDB)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def1 = gam->Find("Inputs.Var1");
    ReferenceT<GAMGenericSignal> def2 = gam->Find("Inputs.Var2");

    ReferenceT<BrokerContainer> broker = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    return (!broker->AddSignal(def2));
}

bool BrokerContainerTest::TestGetSignal() {

    StreamString appCFG = ""
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n"
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +DDB = {\n"
            "        Class = DataSource\n"
            "        +DummySignal = {\n"
            "            Class = DummyDSSignal\n"
            "        }\n"
            "     }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }

    StreamString defCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = true\n"
            "    +Var1 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var2 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var3 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       IsFinal = true\n"
            "    }\n"
            "}\n";

    defCFG.Seek(0);
    ConfigurationDatabase defCDB;

    StandardParser parserDef(defCFG, defCDB);
    if (!parserDef.Parse()) {
        return false;
    }

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(defCDB)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def1 = gam->Find("Inputs.Var1");
    ReferenceT<GAMGenericSignal> def2 = gam->Find("Inputs.Var2");
    ReferenceT<GAMGenericSignal> def3 = gam->Find("Inputs.Var3");

    ReferenceT<BrokerContainer> broker = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    float32 s1, s3;
    uint32 s2;

    // insert to the first container
    if (!broker->AddSignal(def2, &s1)) {
        return false;
    }

    // insert to the second container
    if (!broker->AddSignal(def1, &s2)) {
        return false;
    }

    // insert to the first again
    if (!broker->AddSignal(def3, &s3)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetSignal(0) != &s1) {
        return false;
    }

    if (broker->GetSignal(1) != &s2) {
        return false;
    }
    if (broker->GetSignal(2) != &s3) {
        return false;
    }

    return broker->Size() == 2;
}

bool BrokerContainerTest::TestGetSignalByName() {
    StreamString appCFG = ""
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n"
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +DDB = {\n"
            "        Class = DataSource\n"
            "        +DummySignal = {\n"
            "            Class = DummyDSSignal\n"
            "        }\n"
            "     }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }

    StreamString defCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = true\n"
            "    +Var1 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var2 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var3 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       IsFinal = true\n"
            "    }\n"
            "}\n";

    defCFG.Seek(0);
    ConfigurationDatabase defCDB;

    StandardParser parserDef(defCFG, defCDB);
    if (!parserDef.Parse()) {
        return false;
    }

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(defCDB)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def1 = gam->Find("Inputs.Var1");
    ReferenceT<GAMGenericSignal> def2 = gam->Find("Inputs.Var2");
    ReferenceT<GAMGenericSignal> def3 = gam->Find("Inputs.Var3");

    ReferenceT<BrokerContainer> broker = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    float32 s1, s3;
    uint32 s2;

    // insert to the first container
    if (!broker->AddSignal(def2, &s1)) {
        return false;
    }

    // insert to the second container
    if (!broker->AddSignal(def1, &s2)) {
        return false;
    }

    // insert to the first again
    if (!broker->AddSignal(def3, &s3)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    uint32 index;

    if (broker->GetSignalByName("Var1", index) != &s2) {
        return false;
    }

    if (index != 1) {
        return false;
    }

    if (broker->GetSignalByName("Var2", index) != &s1) {
        return false;
    }
    if (index != 0) {
        return false;
    }

    if (broker->GetSignalByName("Var3", index) != &s3) {
        return false;
    }

    if (index != 2) {
        return false;
    }

    return broker->Size() == 2;
}

bool BrokerContainerTest::TestGetSignalNumberOfSamples() {
    StreamString appCFG = ""
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n"
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +DDB = {\n"
            "        Class = DataSource\n"
            "        +DummySignal = {\n"
            "            Class = DummyDSSignal\n"
            "            Samples = 3\n"
            "        }\n"
            "     }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }

    StreamString defCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = true\n"
            "    +Var1 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var2 = {\n"
            "       Class = GAMSampledSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       IsFinal = true\n"
            "       Samples = \"{{0,0,1},{1,1,1}}\"\n" // first and last
            "    }\n"
            "    +Var3 = {\n"
            "       Class = GAMSampledSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       Samples = \"{{1,1,2}}\""// the middle one
            "       IsFinal = true\n"
            "    }\n"
            "}\n";

    defCFG.Seek(0);
    ConfigurationDatabase defCDB;

    StandardParser parserDef(defCFG, defCDB);
    if (!parserDef.Parse()) {
        return false;
    }

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(defCDB)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def1 = gam->Find("Inputs.Var1");
    ReferenceT<GAMSampledSignal> def2 = gam->Find("Inputs.Var2");
    ReferenceT<GAMSampledSignal> def3 = gam->Find("Inputs.Var3");

    ReferenceT<BrokerContainer> broker = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    float32 s1, s3;
    uint32 s2;

    // insert to the first container
    if (!broker->AddSignal(def2, &s1)) {
        return false;
    }

    // insert to the second container
    if (!broker->AddSignal(def1, &s2)) {
        return false;
    }

    // insert to the first again
    if (!broker->AddSignal(def3, &s3)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    printf("\n%d %d %d\n", broker->GetSignalNumberOfSamples(0), broker->GetSignalNumberOfSamples(1), broker->GetSignalNumberOfSamples(2));

    if (broker->GetSignalNumberOfSamples(0) != 2) {
        return false;
    }

    if (broker->GetSignalNumberOfSamples(1) != 1) {
        return false;
    }

    if (broker->GetSignalNumberOfSamples(2) != 1) {
        return false;
    }

    return broker->Size() == 2;
}

bool BrokerContainerTest::TestGetNumberOfSignals() {
    StreamString appCFG = ""
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n"
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +DDB = {\n"
            "        Class = DataSource\n"
            "        +DummySignal = {\n"
            "            Class = DummyDSSignal\n"
            "            Samples = 3\n"
            "        }\n"
            "     }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }

    StreamString defCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = true\n"
            "    +Var1 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var2 = {\n"
            "       Class = GAMSampledSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       IsFinal = true\n"
            "       Samples = \"{{0,0,1},{1,1,1}}\"\n" // first and last
            "    }\n"
            "    +Var3 = {\n"
            "       Class = GAMSampledSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       Samples = \"{{1,1,2}}\""// the middle one
            "       IsFinal = true\n"
            "    }\n"
            "    +Var4 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var5 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "}\n";

    defCFG.Seek(0);
    ConfigurationDatabase defCDB;

    StandardParser parserDef(defCFG, defCDB);
    if (!parserDef.Parse()) {
        return false;
    }

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(defCDB)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def1 = gam->Find("Inputs.Var1");
    ReferenceT<GAMSampledSignal> def2 = gam->Find("Inputs.Var2");
    ReferenceT<GAMSampledSignal> def3 = gam->Find("Inputs.Var3");

    ReferenceT<BrokerContainer> broker = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    float32 s1, s3;
    uint32 s2;

    // insert to the first container
    if (!broker->AddSignal(def2, &s1)) {
        return false;
    }

    // insert to the second container
    if (!broker->AddSignal(def1, &s2)) {
        return false;
    }

    // insert to the first again
    if (!broker->AddSignal(def3, &s3)) {
        return false;
    }

    ReferenceT<MemoryMapDataSourceBroker> tobeInserted = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    tobeInserted->SetName("toBeIns");
    tobeInserted->SetApplication(*rtapp.operator ->());

    ReferenceT<GAMGenericSignal> def4 = gam->Find("Inputs.Var4");
    ReferenceT<GAMGenericSignal> def5 = gam->Find("Inputs.Var5");

    if (!tobeInserted->AddSignal(def4)) {
        return false;
    }

    if (!tobeInserted->AddSignal(def5)) {
        return false;
    }

    if (!broker->InsertNewBroker(tobeInserted)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    return (broker->GetNumberOfSignals() == 5) && (broker->Size() == 3);
}

bool BrokerContainerTest::TestFinalise() {
    StreamString appCFG = ""
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n"
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +DDB = {\n"
            "        Class = DataSource\n"
            "        +DummySignal = {\n"
            "            Class = DummyDSSignal\n"
            "            Samples = 3\n"
            "        }\n"
            "     }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }

    StreamString defCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = true\n"
            "    +Var1 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var2 = {\n"
            "       Class = GAMSampledSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       IsFinal = true\n"
            "       Samples = \"{{0,0,1},{1,1,1}}\"\n" // first and last
            "       Cycles = 1"
            "    }\n"
            "    +Var3 = {\n"
            "       Class = GAMSampledSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       Samples = \"{{1,1,2}}\""// the middle one
            "       IsFinal = true\n"
            "    }\n"
            "    +Var4 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var5 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "}\n";

    defCFG.Seek(0);
    ConfigurationDatabase defCDB;

    StandardParser parserDef(defCFG, defCDB);
    if (!parserDef.Parse()) {
        return false;
    }

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(defCDB)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def1 = gam->Find("Inputs.Var1");
    ReferenceT<GAMSampledSignal> def2 = gam->Find("Inputs.Var2");
    ReferenceT<GAMSampledSignal> def3 = gam->Find("Inputs.Var3");

    ReferenceT<BrokerContainer> broker = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    float32 s1, s3;
    uint32 s2;

    // insert to the first container
    if (!broker->AddSignal(def2, &s1)) {
        return false;
    }

    // insert to the second container
    if (!broker->AddSignal(def1, &s2)) {
        return false;
    }

    // insert to the first again
    if (!broker->AddSignal(def3, &s3)) {
        return false;
    }

    ReferenceT<MemoryMapDataSourceBroker> tobeInserted = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    tobeInserted->SetName("toBeIns");
    tobeInserted->SetApplication(*rtapp.operator ->());

    ReferenceT<GAMGenericSignal> def4 = gam->Find("Inputs.Var4");
    ReferenceT<GAMGenericSignal> def5 = gam->Find("Inputs.Var5");

    if (!tobeInserted->AddSignal(def4)) {
        return false;
    }

    if (!tobeInserted->AddSignal(def5)) {
        return false;
    }

    if (!broker->InsertNewBroker(tobeInserted)) {
        return false;
    }

    for (uint32 i = 0u; i < broker->GetNumberOfSignals(); i++) {
        if (broker->GetSignal(i) != NULL) {
            return false;
        }
    }

    if (!broker->Finalise()) {
        return false;
    }

    for (uint32 i = 0u; i < broker->GetNumberOfSignals(); i++) {
        if (broker->GetSignal(i) == NULL) {
            return false;
        }
    }
    return broker->Size() == 3;
}

bool BrokerContainerTest::TestFinaliseFalse_MoreThanOneSync() {
    StreamString appCFG = ""
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n"
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +DDB = {\n"
            "        Class = DataSource\n"
            "        +DummySignal = {\n"
            "            Class = DummyDSSignal\n"
            "            Samples = 3\n"
            "        }\n"
            "     }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }

    StreamString defCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = true\n"
            "    +Var1 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var2 = {\n"
            "       Class = GAMSampledSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       IsFinal = true\n"
            "       Samples = \"{{0,0,1},{1,1,1}}\"\n" // first and last
            "       Cycles = 1\n"
            "    }\n"
            "    +Var3 = {\n"
            "       Class = GAMSampledSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       Samples = \"{{1,1,2}}\""// the middle one
            "       IsFinal = true\n"
            "       Cycles = 1\n"
            "    }\n"
            "    +Var4 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var5 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "}\n";

    defCFG.Seek(0);
    ConfigurationDatabase defCDB;

    StandardParser parserDef(defCFG, defCDB);
    if (!parserDef.Parse()) {
        return false;
    }

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(defCDB)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def1 = gam->Find("Inputs.Var1");
    ReferenceT<GAMSampledSignal> def2 = gam->Find("Inputs.Var2");
    ReferenceT<GAMSampledSignal> def3 = gam->Find("Inputs.Var3");

    ReferenceT<BrokerContainer> broker = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    float32 s1, s3;
    uint32 s2;

    // insert to the first container
    if (!broker->AddSignal(def2, &s1)) {
        return false;
    }

    // insert to the second container
    if (!broker->AddSignal(def1, &s2)) {
        return false;
    }

    // insert to the first again
    if (!broker->AddSignal(def3, &s3)) {
        return false;
    }

    ReferenceT<MemoryMapDataSourceBroker> tobeInserted = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    tobeInserted->SetName("toBeIns");
    tobeInserted->SetApplication(*rtapp.operator ->());

    ReferenceT<GAMGenericSignal> def4 = gam->Find("Inputs.Var4");
    ReferenceT<GAMGenericSignal> def5 = gam->Find("Inputs.Var5");

    if (!tobeInserted->AddSignal(def4)) {
        return false;
    }

    if (!tobeInserted->AddSignal(def5)) {
        return false;
    }

    if (!broker->InsertNewBroker(tobeInserted)) {
        return false;
    }

    return (!broker->Finalise());
}

bool BrokerContainerTest::TestInsertNewBroker() {

    StreamString appCFG = ""
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n"
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +DDB = {\n"
            "        Class = DataSource\n"
            "        +DummySignal = {\n"
            "            Class = DummyDSSignal\n"
            "            Samples = 3\n"
            "        }\n"
            "     }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }

    StreamString defCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = true\n"
            "    +Var1 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var2 = {\n"
            "       Class = GAMSampledSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       IsFinal = true\n"
            "       Samples = \"{{0,0,1},{1,1,1}}\"\n" // first and last
            "    }\n"
            "    +Var3 = {\n"
            "       Class = GAMSampledSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       Samples = \"{{1,1,2}}\""// the middle one
            "       IsFinal = true\n"
            "    }\n"
            "    +Var4 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var5 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "}\n";

    defCFG.Seek(0);
    ConfigurationDatabase defCDB;

    StandardParser parserDef(defCFG, defCDB);
    if (!parserDef.Parse()) {
        return false;
    }

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(defCDB)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def1 = gam->Find("Inputs.Var1");
    ReferenceT<GAMSampledSignal> def2 = gam->Find("Inputs.Var2");
    ReferenceT<GAMSampledSignal> def3 = gam->Find("Inputs.Var3");

    ReferenceT<BrokerContainer> broker = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    float32 s1, s5;
    uint32 s2, s3, s4;

    // insert to the first container
    if (!broker->AddSignal(def2, &s1)) {
        return false;
    }

    // insert to the second container
    if (!broker->AddSignal(def1, &s2)) {
        return false;
    }

    ReferenceT<MemoryMapDataSourceBroker> tobeInserted = ReferenceT<MemoryMapDataSourceBroker>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    tobeInserted->SetName("toBeIns");
    tobeInserted->SetApplication(*rtapp.operator ->());

    ReferenceT<GAMGenericSignal> def4 = gam->Find("Inputs.Var4");
    ReferenceT<GAMGenericSignal> def5 = gam->Find("Inputs.Var5");

    if (!tobeInserted->AddSignal(def4, &s3)) {
        return false;
    }

    if (!tobeInserted->AddSignal(def5, &s4)) {
        return false;
    }

    if (!broker->InsertNewBroker(tobeInserted)) {
        return false;
    }

    // insert to the first again
    if (!broker->AddSignal(def3, &s5)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetSignal(0) != &s1) {
        return false;
    }

    if (broker->GetSignal(1) != &s2) {
        return false;
    }

    if (broker->GetSignal(2) != &s3) {
        return false;
    }

    if (broker->GetSignal(3) != &s4) {
        return false;
    }

    if (broker->GetSignal(4) != &s5) {
        return false;
    }
    return broker->Size() == 3;
}

bool BrokerContainerTest::TestSetAsInput(bool flag) {
    StreamString appCFG = ""
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n"
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +DDB = {\n"
            "        Class = DataSource\n"
            "        +DummySignal = {\n"
            "            Class = DummyDSSignal\n"
            "        }\n"
            "     }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }

    StreamString defCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = true\n"
            "    +Var1 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var2 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var3 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       IsFinal = true\n"
            "    }\n"
            "}\n";

    defCFG.Seek(0);
    ConfigurationDatabase defCDB;

    StandardParser parserDef(defCFG, defCDB);
    if (!parserDef.Parse()) {
        return false;
    }

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(defCDB)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    rtapp->PrepareNextState("state1");

    ReferenceT<GAMGenericSignal> def1 = gam->Find("Inputs.Var1");
    ReferenceT<GAMGenericSignal> def2 = gam->Find("Inputs.Var2");
    ReferenceT<GAMGenericSignal> def3 = gam->Find("Inputs.Var3");

    ReferenceT<BrokerContainer> broker = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");

    broker->SetApplication(*rtapp.operator ->());

    broker->SetInput(flag);

    float32 s1, s3;
    uint32 s2;

    // insert to the first container
    if (!broker->AddSignal(def2, &s1)) {
        return false;
    }

    // insert to the second container
    if (!broker->AddSignal(def1, &s2)) {
        return false;
    }

    // insert to the first again
    if (!broker->AddSignal(def3, &s3)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (flag) {
        return (broker->Read(0)) && (!broker->Write(0));
    }
    else {
        return (!broker->Read(0)) && (broker->Write(0));
    }

    return true;
}

bool BrokerContainerTest::TestSetApplication() {
    StreamString appCFG = ""
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n"
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +DDB = {\n"
            "        Class = DataSource\n"
            "        +DummySignal = {\n"
            "            Class = DummyDSSignal\n"
            "        }\n"
            "     }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }

    StreamString defCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = true\n"
            "    +Var1 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 0\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var2 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var3 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal\n"
            "       Default = 1\n"
            "       IsFinal = true\n"
            "    }\n"
            "}\n";

    defCFG.Seek(0);
    ConfigurationDatabase defCDB;

    StandardParser parserDef(defCFG, defCDB);
    if (!parserDef.Parse()) {
        return false;
    }

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(defCDB)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    rtapp->PrepareNextState("state1");

    ReferenceT<GAMGenericSignal> def1 = gam->Find("Inputs.Var1");
    ReferenceT<GAMGenericSignal> def2 = gam->Find("Inputs.Var2");
    ReferenceT<GAMGenericSignal> def3 = gam->Find("Inputs.Var3");

    ReferenceT<BrokerContainer> broker = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");
    float32 s1, s3;
    uint32 s2;

    // insert to the first container
    if (broker->AddSignal(def2, &s1)) {
        return false;
    }

    broker->SetApplication(*rtapp.operator ->());

    // insert to the first container
    if (!broker->AddSignal(def2, &s1)) {
        return false;
    }

    // insert to the second container
    if (!broker->AddSignal(def1, &s2)) {
        return false;
    }

    // insert to the first again
    if (!broker->AddSignal(def3, &s3)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (broker->GetSignal(0) != &s1) {
        return false;
    }

    if (broker->GetSignal(1) != &s2) {
        return false;
    }

    if (broker->GetSignal(2) != &s3) {
        return false;
    }

    return true;
}

bool BrokerContainerTest::TestRead() {
    StreamString appCFG = ""
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n"
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +DDB = {\n"
            "        Class = DataSource\n"
            "        +DummySignal1 = {\n"
            "            Class = DummyDSSignal\n"
            "        }\n"
            "        +DummySignal2 = {\n"
            "            Class = DummyDSSignal\n"
            "        }\n"
            "     }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }

    StreamString defCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = true\n"
            "    +Var1 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       Default = 2\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var2 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal1\n"
            "       Default = 1.5\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var3 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal2\n"
            "       Default = 2.5\n"
            "       IsFinal = true\n"
            "    }\n"
            "}\n";

    defCFG.Seek(0);
    ConfigurationDatabase defCDB;

    StandardParser parserDef(defCFG, defCDB);
    if (!parserDef.Parse()) {
        return false;
    }

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(defCDB)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    rtapp->PrepareNextState("state1");

    ReferenceT<GAMGenericSignal> def1 = gam->Find("Inputs.Var1");
    ReferenceT<GAMGenericSignal> def2 = gam->Find("Inputs.Var2");
    ReferenceT<GAMGenericSignal> def3 = gam->Find("Inputs.Var3");

    ReferenceT<BrokerContainer> broker = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");
    broker->SetApplication(*rtapp.operator ->());

    float32 s1, s3;
    uint32 s2;

    // insert to the first container
    if (!broker->AddSignal(def2, &s1)) {
        return false;
    }

    // insert to the second container
    if (!broker->AddSignal(def1, &s2)) {
        return false;
    }

    // insert to the first again
    if (!broker->AddSignal(def3, &s3)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    if (!broker->Read(0)) {
        return false;
    }
    printf("\n%f %f %d\n", s1, s3, s2);

    if (s1 != 1.5) {
        return false;
    }

    if (s2 != 2) {
        return false;
    }

    if (s3 != 2.5) {
        return false;
    }

    return true;
}

bool BrokerContainerTest::TestWrite() {
    StreamString appCFG = ""
            "+States = {\n"
            "    Class = ReferenceContainer\n"
            "    +state1 = {\n"
            "        Class = RealTimeState\n"
            "    }\n"
            "}\n"
            "+Data = {\n"
            "    Class = DataSourceContainer\n"
            "    +DDB = {\n"
            "        Class = DataSource\n"
            "        +DummySignal1 = {\n"
            "            Class = DummyDSSignal\n"
            "        }\n"
            "        +DummySignal2 = {\n"
            "            Class = DummyDSSignal\n"
            "        }\n"
            "     }\n"
            "}\n";

    appCFG.Seek(0);
    ConfigurationDatabase appCDB;

    StandardParser parserApp(appCFG, appCDB);
    if (!parserApp.Parse()) {
        return false;
    }

    StreamString defCFG = ""
            "+Inputs = {\n"
            "    Class = GAMSignalsContainer\n"
            "    IsInput = true\n"
            "    IsFinal = true\n"
            "    +Var1 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = uint32\n"
            "       Path = DDB.Signal\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var2 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal1\n"
            "       IsFinal = true\n"
            "    }\n"
            "    +Var3 = {\n"
            "       Class = GAMGenericSignal\n"
            "       Type = float32\n"
            "       Path = DDB.DummySignal2\n"
            "       IsFinal = true\n"
            "    }\n"
            "}\n";

    defCFG.Seek(0);
    ConfigurationDatabase defCDB;

    StandardParser parserDef(defCFG, defCDB);
    if (!parserDef.Parse()) {
        return false;
    }

    ReferenceT<RealTimeApplication> rtapp = ReferenceT<RealTimeApplication>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!rtapp->Initialise(appCDB)) {
        return false;
    }

    ReferenceT<PlantGAM> gam = ReferenceT<PlantGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    gam->SetName("Plant");
    if (!gam->Initialise(defCDB)) {
        return false;
    }

    gam->SetApplication(*rtapp.operator ->());
    gam->AddState("state1", "thread1");
    if (!gam->ConfigureDataSource()) {
        return false;
    }

    if (!rtapp->ValidateDataSource()) {
        return false;
    }

    if (!rtapp->AllocateDataSource()) {
        return false;
    }

    ReferenceT<GAMGenericSignal> def1 = gam->Find("Inputs.Var1");
    ReferenceT<GAMGenericSignal> def2 = gam->Find("Inputs.Var2");
    ReferenceT<GAMGenericSignal> def3 = gam->Find("Inputs.Var3");

    ReferenceT<BrokerContainer> broker = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    broker->SetName("broker");
    broker->SetApplication(*rtapp.operator ->());
    broker->SetInput(false);

    float32 s1, s3;
    uint32 s2;

    // insert to the first container
    if (!broker->AddSignal(def2, &s1)) {
        return false;
    }

    // insert to the second container
    if (!broker->AddSignal(def1, &s2)) {
        return false;
    }

    // insert to the first again
    if (!broker->AddSignal(def3, &s3)) {
        return false;
    }

    if (!broker->Finalise()) {
        return false;
    }

    s1 = 1.5;
    s2 = 2;
    s3 = 2.5;
    if (!broker->Write(0)) {
        return false;
    }

    MemoryMapInputReader reader;
    reader.SetApplication(*rtapp.operator ->());
    if (!reader.AddSignal(def2)) {
        return false;
    }
    if (!reader.AddSignal(def1)) {
        return false;
    }

    if (!reader.AddSignal(def3)) {
        return false;
    }

    if (!reader.Finalise()) {
        return false;
    }

    if (!reader.Read(0)) {
        return false;
    }
    printf("\n%f %f %d\n", s1, s3, s2);

    if (*((float32*) reader.GetSignal(0)) != 1.5) {
        return false;
    }

    if (*((uint32*) reader.GetSignal(1)) != 2) {
        return false;
    }

    if (*((float32*) reader.GetSignal(2)) != 2.5) {
        return false;
    }

    return true;
}

