/**
 * @file GAMTestHelper.h
 * @brief Header file for class GAMTestHelper
 * @date 01/03/2016
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class GAMTestHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMTESTHELPER_H_
#define GAMTESTHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"
#include "ConfigurationDatabase.h"
#include "GAMSchedulerI.h"
#include "StandardParser.h"
#include "MemoryMapBroker.h"
#include "GAMGroup.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

static StreamString config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAM1"
        "            InputSignals = {"
        "                Signal1 = {"
        "                    DataSource = DDB1"
        "                    Type = TestStructA"
        "                    MemberAliases = {"
        "                        Signal1.a1 = E"
        "                    }"
        "                    Defaults = {"
        "                        Signal1.a1.b1 = 1"
        "                        Signal1.a2 = 0.5"
        "                    }"
        "                    Frequency = 1000"
        "                    SyncSignal = Signal1.a1.b1"
        "                    Samples = 2"
        "                }"
        "                Signal2 = {"
        "                    DataSource = DDB2"
        "                    Type = uint32"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 2"
        "                    Default = \"{1 2}\""
        "                    Samples = 10"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAM1"
        "            OutputSignals = {"
        "                Signal1 = {"
        "                    DataSource = DDB2.DDB1"
        "                }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAM1"
        "            OutputSignals = {"
        "                ToBoard1 = {"
        "                    Signal1 = {"
        "                        DataSource = DDB1"
        "                        Type = uint32"
        "                        Alias = SharedVar"
        "                        NumberOfDimensions = 2"
        "                        NumberOfElements = 32"
        "                        Ranges = {{0 0},{3 5}}"
        "                    }"
        "                }"
        "            }"
        "            +GAMA = {"
        "                Class = GAM1"
        "                OutputSignals = {"
        "                    Signal1 = {"
        "                        DataSource = DDB1"
        "                        Type = uint32"
        "                        Alias = SharedVar"
        "                        NumberOfDimensions = 2"
        "                        NumberOfElements = 32"
        "                        Ranges = {{1 2},{6 31}}"
        "                    }"
        "                }"
        "                InputSignals = {"
        "                    Signal2 = {"
        "                        DataSource = DDB2"
        "                        Alias = PredefinedSignal"
        "                    }"
        "                    Signal3 = {"
        "                        DataSource = DDB2"
        "                        Alias = PredefinedSignal.a1"
        "                    }"
        "                    Signal4 = {"
        "                        DataSource = DDB2"
        "                        Alias = PredefinedSignal.a2"
        "                    }"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +DDB1 = {"
        "            Class = DS1"
        "        }"
        "        +DDB2 = {"
        "            Class = DS1"
        "            +DDB1 = {"
        "                Class = DS1"
        "                Signals = {"
        "                    Signal1 = {"
        "                        Type = int32"
        "                        NumberOfDimensions = 1"
        "                        NumberOfElements = 32"
        "                    }"
        "                }"
        "            }"
        "            Signals = {"
        "                PredefinedSignal = {"
        "                    Type = TestStructA"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {:Functions.GAMA, :Functions.GAMB, :Functions.GAMC}"
        "                }"
        "                +Thread2 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {:Functions.GAMC.GAMA}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {:Functions.GAMA,:Functions.GAMC.GAMA}"
        "                }"
        "                +Thread2 = {"
        "                    Class = RealTimeThread"
        "                    Functions = { :Functions.GAMC, :Functions.GAMB}"
        "                }"
        "            }"
        "        }"
        "    }"
        "}";

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
class GAM1: public GAM, public StatefulI {
public:
    CLASS_REGISTER_DECLARATION()

GAM1    ();

    virtual ~GAM1();

    virtual bool Setup();

    virtual bool Initialise(StructuredDataI & data);

    virtual bool Execute();

    bool PrepareNextState(const char8 * currentStateName, const char8 * nextStateName);

    uint8 *GetInputMem();

    uint8 *GetOutputMem();

    uint32 numberOfExecutions;
    uint32 context;

};

class GAMGroup1: public GAMGroup {
public:
    CLASS_REGISTER_DECLARATION()
virtual bool PrepareNextState(const char8 * currentStateName,
                              const char8 * nextStateName);

protected:
    virtual void SetUp();

};

struct TestStructB {
    int32 b1;
    int32 b2;
};

struct TestStructA {
    TestStructB a1;
    float32 a2;
};

struct TestStructC {
    TestStructB c1;
    float32 c2[3];
    int32 c3[2][4];
};

struct TestStructD {
    TestStructB c1;
    float32 c2[3];
    int32 c3[2][4];
};



struct TestStructBB {
    int8 b1;
    int64 b2;
};

struct TestStructCC {
    TestStructBB c1;
    float32 c2[3];
    int32 c3[2][4];
};


struct TestStructE {
    TestStructC e1[2][1][2];
    int32 e2;
};

struct TestStructF {
    TestStructE f1[2];
    int32 f2;
};

struct TestStructG {
    TestStructF g1[2][2];
    int32 g2;
};

struct TestStructH {
    char8 str[64];
};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

namespace MARTe {
class DS1: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

DS1    ();

    virtual ~DS1();

    virtual bool Initialise(StructuredDataI & data);

    virtual uint32 GetCurrentBufferIndex();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    virtual bool AllocateMemory();

    virtual const char8 *GetBrokerName(StructuredDataI &data, const SignalDirection direction);

    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    virtual bool ChangeState();

    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8 *const functionName,
            void* const gamMemPtr);

    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
            const char8 * const functionName,
            void* const gamMemPtr);

    virtual bool Synchronise();
};
}

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
class Driver1: public DataSourceI {
public:
CLASS_REGISTER_DECLARATION()

Driver1();

virtual ~Driver1();

virtual bool Initialise(StructuredDataI & data);

virtual uint32 GetCurrentBufferIndex();

virtual uint32 GetNumberOfMemoryBuffers();

virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
        const uint32 bufferIdx,
        void *&signalAddress);

virtual bool AllocateMemory();

virtual const char8 *GetBrokerName(StructuredDataI &data, const SignalDirection direction);

virtual bool PrepareNextState(const char8 * const currentStateName,
                              const char8 * const nextStateName);
virtual bool ChangeState();

virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
        const char8 * const functionName,
        void* const gamMemPtr);

virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
        const char8 * const functionName,
        void* const gamMemPtr);

virtual bool Synchronise();
};


///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
class DefaultSchedulerForTests: public GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

DefaultSchedulerForTests    ();

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution();

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution();

    virtual void CustomPrepareNextState();


};

#endif /* GAMTESTHELPER_H_ */

