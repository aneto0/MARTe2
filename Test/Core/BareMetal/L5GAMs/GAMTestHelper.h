/**
 * @file GAMTestHelper.h
 * @brief Header file for class GAMTestHelper
 * @date 01/mar/2016
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
class GAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

GAM1    ();

    virtual ~GAM1();

    virtual bool Initialise(StructuredDataI & data);

    virtual bool Execute();


    uint32 numberOfExecutions;
protected:
    virtual void SetUp();

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

    virtual bool PrepareNextState(const MARTe::RealTimeStateInfo&);

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

virtual bool PrepareNextState(const RealTimeStateInfo &status);

virtual bool ChangeState();

virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
        const char8 * const functionName,
        void* const gamMemPtr);

virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
        const char8 * const functionName,
        void* const gamMemPtr);

virtual bool Synchronise();
};

#if 0

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

struct ReadParam {
ReferenceT<MemoryMapInputReader> reader;
uint32 numberOfReads;
volatile int32 *spinlock;
bool retVal;
};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

struct WriteParam {
ReferenceT<MemoryMapOutputWriter> writer;
uint32 numberOfWrites;
volatile int32 *spinlock;
bool retVal;
};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

/**
 * @brief An introspectable structure
 */
struct TrackError {
uint32 Par1;
uint32 Par2;
};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

/**
 * @brief An introspectable structure
 */
struct ControlIn {
uint32 Par1;
uint32 Par2;
};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

/**
 * @brief An introspectable structure
 */
struct ControlNoise {
float32 noiseValue;
};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

/**
 * @brief An introspectable structure
 * with a vector member
 */
struct TrackErrorArray {
uint32 Pars[2];
};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

/**
 * @brief An introspectable structure
 * with a vector member
 */
struct ControlInArray {
uint32 Pars[2];
};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

/**
 * @brief An introspectable structure
 * with a matrix member
 */
struct TrackErrorMatrix {
uint32 Pars[3][2];
};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

/**
 * @brief An introspectable structure
 * with a matrix member
 */
struct ControlInMatrix {
uint32 Pars[3][2];
};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

/**
 * @brief Implementation of a GAM used in tests.
 */
class PIDGAM: public GAM {

public:
CLASS_REGISTER_DECLARATION()

PIDGAM();

/**
 * @brief Destructor. Frees the local cdb created internally
 */
virtual ~PIDGAM();

/**
 * @brief The execution routine.
 */
virtual void Execute(uint8 activeContextBuffer);
protected:

/**
 * @brief Creates the local cdb.
 */
virtual void SetUp();

private:

};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

/**
 * @brief Implementation of a GAMGroup used in tests.
 */
class PIDGAMGroup: public GAMGroup {
public:
CLASS_REGISTER_DECLARATION()

PIDGAMGroup();

/**
 * @brief Dummy implementation of the pure virtual PrepareNextState() function which change a variable.
 */
virtual void PrepareNextState(const RealTimeStateInfo &status);

/**
 * @brief Retrieves the variable changed by PrepareNextState().
 */
uint32 GetContext();

protected:

/**
 * @brief Initialises the internal variable.
 */
virtual void SetUp();

private:

/**
 * An internal variable.
 */
uint32 context;

};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

/**
 * @brief Implementation of a GAM used in tests.
 */
class PIDGAM2: public GAM {

public:
CLASS_REGISTER_DECLARATION()
PIDGAM2();
/**
 * @brief Empty
 */
~PIDGAM2();

/**
 * @brief The execution routine using vector variables.
 */
virtual void Execute(uint8 activeContextBuffer);
protected:

/**
 * @brief Empty.
 */
virtual void SetUp();

private:
};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

class PIDGAM3: public GAM {

public:
CLASS_REGISTER_DECLARATION()
PIDGAM3();

~PIDGAM3();

/**
 * @brief The execution routine using matrix variables.
 */
virtual void Execute(uint8 activeContextBuffer);
protected:

/**
 * @brief Empty
 */
virtual void SetUp();

private:
};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

/**
 * Implementation of a GAM used in tests.
 */
class PlantGAM: public GAM {

public:
CLASS_REGISTER_DECLARATION()
PlantGAM();

/**
 * @brief The execution routine.
 */
virtual void Execute(uint8 activeContextBuffer);
protected:
/**
 * @brief Empty
 */
virtual void SetUp();

};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

/**
 * Implementation of a GAM used in tests.
 */
class DummyGAM: public GAM {

public:
CLASS_REGISTER_DECLARATION()
DummyGAM();

/**
 * @brief The execution routine.
 */
virtual void Execute(uint8 activeContextBuffer);

uint32 flag;
protected:
/**
 * @brief Empty
 */
virtual void SetUp();

};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

/**
 * @brief Returns a configuration database commonly used in tests.
 */
static const char8* cdbStr1 = ""
"$Application1 = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = PIDGAM"
"        }"
"        +GAM2 = {"
"            Class = PIDGAM"
"        }"
"        +PIDGroup1 = {"
"            Class = PIDGAMGroup"
"            +GAM3 = {"
"                Class = PIDGAM"
"            }"
"            +GAM4 = {"
"                Class = PIDGAM"
"            }"
"        }"
"        +GAMContainer = {"
"            Class = ReferenceContainer"
"            +GAM5 = {"
"                Class = PIDGAM"
"                +Inputs = {"
"                    Class = GAMSignalsContainer"
"                    IsInput = true"
"                    IsFinal = false"
"                    +Error = {"
"                        Class = GAMGenericSignal"
"                        Type = TrackError"
"                        IsFinal = false"
"                        +Par2 = {"
"                            Class = GAMGenericSignal"
"                            Type = uint32"
"                            Default = 2"
"                            Path = DDB1.PidError2"
"                            IsFinal = true"
"                        }"
"                    }"
"                }"
"                +Outputs = {"
"                    Class = GAMSignalsContainer"
"                    IsOutput = true"
"                    IsFinal = false"
"                    +Control = {"
"                        Class = GAMGenericSignal"
"                        Type = ControlIn"
"                        IsFinal = false"
"                        +Par2 = {"
"                            Class = GAMGenericSignal"
"                            Type = uint32"
"                            Path = DDB2.PidControl2"
"                            Default = 1"
"                            IsFinal = true"
"                        }"
"                    }"
"                    +Noise = {"
"                        Class = GAMGenericSignal"
"                        Type = ControlNoise"
"                        IsFinal = true"
"                        +noiseValue = {"
"                            Class = GAMGenericSignal"
"                            Type = float32"
"                            Default = 2"
"                            Path = DDB2.PidNoise"
"                            IsFinal = true"
"                        }"
"                    }"
"                }"
"            }"
"            +GAM6 = {"
"                Class = PIDGAM"
"            }"
"        }"
"        +PIDGroup2 = {"
"            Class = PIDGAMGroup"
"            +GAM7 = {"
"                Class = PIDGAM"
"                +Inputs = {"
"                    Class = GAMSignalsContainer"
"                    IsInput = true"
"                    IsFinal = false"
"                    +Error = {"
"                        Class = GAMGenericSignal"
"                        Type = TrackError"
"                        IsFinal = false"
"                        +Par2 = {"
"                            Class = GAMGenericSignal"
"                            Type = uint32"
"                            Default = 2"
"                            Path = DDB3.PidError2"
"                            IsFinal = true"
"                        }"
"                    }"
"                }"
"                +Outputs = {"
"                    Class = GAMSignalsContainer"
"                    IsOutput = true"
"                    IsFinal = false"
"                    +Control = {"
"                        Class = GAMGenericSignal"
"                        Type = ControlIn"
"                        IsFinal = false"
"                        +Par2 = {"
"                            Class = GAMGenericSignal"
"                            Type = uint32"
"                            Path = DDB4.PidControl2"
"                            Default = 1"
"                            IsFinal = true"
"                        }"
"                    }"
"                }"
"            }"
"            +GAM8 = {"
"                Class = PIDGAM"
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
"                    Functions = { :Functions.GAM1 :Functions.GAM2 }"
"                }"
"                +Thread2 = {"
"                    Class = RealTimeThread"
"                    Functions = { :Functions.PIDGroup1 }"
"                }"
"            }"
"        }"
"        +State2 = {"
"            Class = RealTimeState"
"            +Threads = {"
"                Class = ReferenceContainer"
"                +Thread1 = {"
"                    Class = RealTimeThread"
"                    Functions = { :Functions.GAMContainer }"
"                }"
"                +Thread2 = {"
"                    Class = RealTimeThread"
"                    Functions = { :Functions.PIDGroup2.GAM7 :Functions.PIDGroup2.GAM8 }"
"                }"
"            }"
"        }"
"    }"
"    +Data = {"
"        Class = DataSourceContainer"
"        +DDB1 = {"
"            Class = DataSource"
"        }"
"        +DDB2 = {"
"            Class = DataSource"
"        }"
"    }"
"    +Scheduler = {"
"        Class = DummyScheduler"
"    }"
"}";

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

static const char8 *cdbStr2 = "\n"
"$Application1 = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = PIDGAM"
"        }"
"        +GAM2 = {"
"            Class = PIDGAM"
"        }"
"        +PIDGroup1 = {"
"            Class = PIDGAMGroup"
"            +GAM3 = {"
"                Class = PIDGAM"
"            }"
"            +GAM4 = {"
"                Class = PIDGAM"
"            }"
"        }"
"        +GAMContainer = {"
"            Class = ReferenceContainer"
"            +GAM5 = {"
"                Class = PIDGAM"
"                +Inputs = {"
"                    Class = GAMSignalsContainer"
"                    IsInput = true"
"                    IsFinal = false"
"                    +Error = {"
"                        Class = GAMGenericSignal"
"                        Type = TrackError"
"                        IsFinal = false"
"                        +Par2 = {"
"                            Class = GAMGenericSignal"
"                                Type = uint32"
"                                Default = 2"
"                                Path = DDB1.PidError2"
"                                IsFinal = true"
"                            }"
"                        }"
"                }"
"                +Outputs = {"
"                    Class = GAMSignalsContainer"
"                    IsOutput = true"
"                    IsFinal = false"
"                    +Control = {"
"                        Class = GAMGenericSignal"
"                        Type = ControlIn"
"                        IsFinal = false"
"                        +Par2 = {"
"                            Class = GAMGenericSignal"
"                            Type = uint32"
"                            Path = DDB2.PidControl2"
"                            Default = 2"
"                            IsFinal = true"
"                        }"
"                    }"
"                    +Noise = {"
"                        Class = GAMGenericSignal"
"                        Type = ControlNoise"
"                        IsFinal = true"
"                        +noiseValue = {"
"                            Class = GAMGenericSignal"
"                            Type = float32"
"                            Default = 2"
"                            Path = DDB2.PidNoise"
"                            IsFinal = true"
"                        }"
"                    }"
"                }"
"            }"
"            +GAM6 = {"
"                Class = PIDGAM"
"            }"
"        }"
"        +PIDGroup2 = {"
"            Class = PIDGAMGroup"
"            +GAM7 = {"
"                Class = PIDGAM"
"            }"
"            +GAM8 = {"
"                Class = PlantGAM"
"                +Inputs = {"
"                    Class = GAMSignalsContainer"
"                    IsInput = true"
"                    IsFinal = false"
"                    +Control = {"
"                        Class = GAMGenericSignal"
"                        Type = TrackError"
"                        IsFinal = true"
"                        +Par1 = {"
"                            Class = GAMGenericSignal"
"                            Type = uint32"
"                            Default = 1"
"                            Path = DDB2.PidControl1"
"                            IsFinal = true"
"                        }"
"                        +Par2 = {"
"                            Class = GAMGenericSignal"
"                            Type = uint32"
"                            Default = 2"
"                            Path = DDB2.PidControl2"
"                            IsFinal = true"
"                        }"
"                    }"
"                }"
"                +Outputs = {"
"                    Class = GAMSignalsContainer"
"                    IsOutput = true"
"                    IsFinal = true"
"                    +Error = {"
"                        Class = GAMGenericSignal"
"                        Type = TrackError"
"                        IsFinal = false"
"                        +Par1 = {"
"                            Class = GAMGenericSignal"
"                            Type = uint32"
"                            Default = 1"
"                            Path = DDB1.PidError1"
"                            IsFinal = true"
"                        }"
"                        +Par2 = {"
"                            Class = GAMGenericSignal"
"                            Type = uint32"
"                            Default = 2"
"                            Path = DDB1.PidError2"
"                            IsFinal = true"
"                        }"
"                    }"
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
"                    Functions = { :Functions.GAM1 :Functions.GAM2}"
"                }"
"                +Thread2 = {"
"                    Class = RealTimeThread"
"                    Functions = { :Functions.PIDGroup1 }"
"                }"
"            }"
"        }"
"        +State2 = {"
"            Class = RealTimeState"
"            +Threads = {"
"                Class = ReferenceContainer"
"                +Thread1 = {"
"                    Class = RealTimeThread"
"                    Functions = { :Functions.GAMContainer }"
"                }"
"                +Thread2 = {"
"                    Class = RealTimeThread"
"                    Functions = { :Functions.PIDGroup2.GAM7  :Functions.PIDGroup2.GAM8 }"
"                }"
"            }"
"        }"
"    }"
"    +Data = {"
"        Class = DataSourceContainer"
"        +DDB1 = {"
"            Class = DataSource"
"        }"
"        +DDB2 = {"
"            Class = DataSource"
"        }"
"    }"
"    +Scheduler = {"
"        Class = DummyScheduler"
"    }"
"}";

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

static const char8 *cdbStr3 = "\n"
"$Application1 = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = DummyGAM"
"            +Input = {"
"                Class = GAMSignalsContainer"
"                IsInput = true"
"                IsFinal = true"
"                +Counter = {"
"                    Class = GAMGenericSignal"
"                    Type = uint32"
"                    Default = 0"
"                    Path = DDB.Counter1"
"                    IsFinal = true"
"                }"
"            }"
"            +Output = {"
"                Class = GAMSignalsContainer"
"                IsOutput = true"
"                IsFinal = true"
"                +Counter = {"
"                    Class = GAMGenericSignal"
"                    Type = uint32"
"                    Default = 0"
"                    Path = DDB.Counter2"
"                    IsFinal = true"
"                }"
"            }"
"        }"
"        +GAM2 = {"
"            Class = DummyGAM"
"            +Input = {"
"                Class = GAMSignalsContainer"
"                IsInput = true"
"                IsFinal = true"
"                +Counter = {"
"                    Class = GAMGenericSignal"
"                    Type = uint32"
"                    Default = 0"
"                    Path = DDB.Counter2"
"                    IsFinal = true"
"                }"
"            }"
"            +Output = {"
"                Class = GAMSignalsContainer"
"                IsOutput = true"
"                IsFinal = true"
"                +Counter = {"
"                    Class = GAMGenericSignal"
"                        Type = uint32"
"                        Default = 0"
"                        Path = DDB.Counter1"
"                        IsFinal = true"
"                }"
"            }"
"        }"
"    }"
"    +States = {"
"        Class = ReferenceContainer"
"        +State1 = {"
"            Class = RealTimeState"
"            +Threads = {"
"            Class = ReferenceContainer"
"                +Thread1 = {"
"                    Class = RealTimeThread"
"                    Functions = { :Functions.GAM1 :Functions.GAM2 }"
"                }"
"            }"
"        }"
"    }"
"    +Data = {"
"        Class = DataSourceContainer"
"        +DDB = {"
"            Class = DataSource"
"        }"
"    }"
"    +Scheduler = {"
"        Class = DummyScheduler"
"    }"
"}";

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

static const char8 *cdbStr4 = "\n"
"$Application1 = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = DummyGAM"
"            +Input = {"
"                Class = GAMSignalsContainer"
"                IsInput = true"
"                IsFinal = true"
"                +Counter = {"
"                    Class = GAMGenericSignal"
"                    Type = uint32"
"                    Default = 0"
"                    Path = DDB.Counter1"
"                    IsFinal = true"
"                }"
"                +AbsTime = {"
"                    Class = GAMGenericSignal"
"                    Type = uint64"
"                    Default = 0"
"                    Path = GAM_Times.GAM1.AbsoluteUsecTime"
"                    IsFinal = true"
"                }"
"                +RelTime = {"
"                    Class = GAMGenericSignal"
"                    Type = uint64"
"                    Default = 0"
"                    Path = GAM_Times.GAM1.RelativeUsecTime"
"                    IsFinal = true"
"                }"
"            }"
"            +Output = {"
"                Class = GAMSignalsContainer"
"                IsOutput = true"
"                IsFinal = true"
"                +Counter = {"
"                    Class = GAMGenericSignal"
"                    Type = uint32"
"                    Default = 0"
"                    Path = DDB.Counter2"
"                    IsFinal = true"
"                }"
"            }"

"        }"
"        +GAM2 = {"
"            Class = DummyGAM"
"            +Input = {"
"                Class = GAMSignalsContainer"
"                IsInput = true"
"                IsFinal = true"
"                +Counter = {"
"                    Class = GAMGenericSignal"
"                    Type = uint32"
"                    Default = 0"
"                    Path = DDB.Counter2"
"                    IsFinal = true"
"                }"
"                +AbsTime = {"
"                    Class = GAMGenericSignal"
"                    Type = uint64"
"                    Default = 0"
"                    Path = GAM_Times.GAM2.AbsoluteUsecTime"
"                    IsFinal = true"
"                }"
"                +RelTime = {"
"                    Class = GAMGenericSignal"
"                    Type = uint64"
"                    Default = 0"
"                    Path = GAM_Times.GAM2.RelativeUsecTime"
"                    IsFinal = true"
"                }"
"            }"
"            +Output = {"
"                Class = GAMSignalsContainer"
"                IsOutput = true"
"                IsFinal = true"
"                +Counter = {"
"                    Class = GAMGenericSignal"
"                    Type = uint32"
"                    Default = 0"
"                    Path = DDB.Counter1"
"                    IsFinal = true"
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
"                    Functions = { :Functions.GAM1 :Functions.GAM2 }"
"                }"
"            }"
"        }"
"    }"
"    +Data = {"
"        Class = DataSourceContainer"
"        +DDB = {"
"            Class = DataSource"
"        }"
"    }"
"    +Scheduler = {"
"        Class = DummyScheduler"
"    }"
"}";

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

static const char8 *cdbStr5 = "\n"
"$Application1 = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = DummyGAM"
"            +Input = {"
"                Class = GAMSignalsContainer"
"                IsInput = true"
"                IsFinal = true"
"                +Counter = {"
"                    Class = GAMGenericSignal"
"                    Type = uint32"
"                    Default = 0"
"                    Path = DDB.Counter1"
"                    IsFinal = true"
"                }"
"            }"
"            +Output = {"
"                Class = GAMSignalsContainer"
"                IsOutput = true"
"                IsFinal = true"
"                +Counter = {"
"                    Class = GAMGenericSignal"
"                    Type = uint32"
"                    Default = 0"
"                    Path = DDB.Counter2"
"                    IsFinal = true"
"                }"
"            }"
"        }"
"        +GAM2 = {"
"            Class = DummyGAM"
"            +Input = {"
"                Class = GAMSignalsContainer"
"                IsInput = true"
"                IsFinal = true"
"                +Counter = {"
"                    Class = GAMGenericSignal"
"                    Type = uint32"
"                    Default = 0"
"                    Path = DDB.Counter2"
"                    IsFinal = true"
"                }"
"            }"
"            +Output = {"
"                Class = GAMSignalsContainer"
"                IsOutput = true"
"                IsFinal = true"
"                +Counter = {"
"                    Class = GAMGenericSignal"
"                        Type = uint32"
"                        Default = 0"
"                        Path = DDB.Counter1"
"                        IsFinal = true"
"                }"
"            }"
"        }"
"        +GAM3 = {"
"            Class = DummyGAM"
"            +Input = {"
"                Class = GAMSignalsContainer"
"                IsInput = true"
"                IsFinal = true"
"                +Counter = {"
"                    Class = GAMGenericSignal"
"                    Type = uint32"
"                    Default = 0"
"                    Path = DDB.Counter3"
"                    IsFinal = true"
"                }"
"            }"
"            +Output = {"
"                Class = GAMSignalsContainer"
"                IsOutput = true"
"                IsFinal = true"
"                +Counter = {"
"                    Class = GAMGenericSignal"
"                    Type = uint32"
"                    Default = 0"
"                    Path = DDB.Counter4"
"                    IsFinal = true"
"                }"
"            }"
"        }"
"        +GAM4 = {"
"            Class = DummyGAM"
"            +Input = {"
"                Class = GAMSignalsContainer"
"                IsInput = true"
"                IsFinal = true"
"                +Counter = {"
"                    Class = GAMGenericSignal"
"                    Type = uint32"
"                    Default = 0"
"                    Path = DDB.Counter4"
"                    IsFinal = true"
"                }"
"            }"
"            +Output = {"
"                Class = GAMSignalsContainer"
"                IsOutput = true"
"                IsFinal = true"
"                +Counter = {"
"                    Class = GAMGenericSignal"
"                        Type = uint32"
"                        Default = 0"
"                        Path = DDB.Counter3"
"                        IsFinal = true"
"                }"
"            }"
"        }"
"    }"
"    +States = {"
"        Class = ReferenceContainer"
"        +State1 = {"
"            Class = RealTimeState"
"            +Threads = {"
"            Class = ReferenceContainer"
"                +Thread1 = {"
"                    Class = RealTimeThread"
"                    Functions = { :Functions.GAM1 :Functions.GAM2 }"
"                }"
"                +Thread2 = {"
"                    Class = RealTimeThread"
"                    Functions = { :Functions.GAM3 :Functions.GAM4 }"
"                }"
"            }"
"        }"
"    }"
"    +Data = {"
"        Class = DataSourceContainer"
"        +DDB = {"
"            Class = DataSource"
"        }"
"    }"
"    +Scheduler = {"
"        Class = GAMScheduler"
"    }"
"}";

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

class SyncDataSourceSignal: public GAMDataSource {

public:
CLASS_REGISTER_DECLARATION()
SyncDataSourceSignal();
virtual ~SyncDataSourceSignal();

virtual void WriteEnd();

virtual bool WaitOnEvent(const TimeoutType &timeout = TTInfiniteWait);

virtual bool Configure(Reference gamSignalIn);

private:

// fake driver initialised from cdb

FastPollingEventSem internalSem;

};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

class WriteSyncDataSourceSignal: public GAMDataSource {

public:
CLASS_REGISTER_DECLARATION()
WriteSyncDataSourceSignal();
virtual ~WriteSyncDataSourceSignal();

virtual void ReadEnd();

virtual bool WaitOnEvent(const TimeoutType &timeout = TTInfiniteWait);

virtual bool Configure(Reference gamSignalIn);

private:

// fake driver initialised from cdb

FastPollingEventSem internalSem;

};

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

class DummyScheduler: public GAMSchedulerI {
public:

CLASS_REGISTER_DECLARATION()

DummyScheduler();

virtual void StartExecution(const uint32 activeBuffer);

virtual void StopExecution();

uint32 numberOfExecutions;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
#endif
#endif /* GAMTESTHELPER_H_ */

