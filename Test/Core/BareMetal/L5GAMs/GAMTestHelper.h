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

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

struct TrackError {
    uint32 Par1;
    uint32 Par2;
};

struct ControlIn {
    uint32 Par1;
    uint32 Par2;
};

struct ControlNoise {
    float32 noiseValue;
};

class PIDGAM: public GAM {

public:
    CLASS_REGISTER_DECLARATION()

    ~PIDGAM();

    virtual void Execute(uint8 activeContextBuffer);
protected:
    virtual void SetUp();

private:
};

class PIDGAMGroup: public GAMGroup {
public:
    CLASS_REGISTER_DECLARATION()

    virtual void SetUp();
    virtual void PrepareNextState(const RealTimeStateInfo &status);

    uint32 GetContext();

private:
    uint32 context;

};

class PlantGAM: public GAM {

public:
    CLASS_REGISTER_DECLARATION()

    virtual void Execute(uint8 activeContextBuffer);
protected:
    virtual void SetUp();

};

static ConfigurationDatabase GetCDB() {
    ConfigurationDatabase cdb;

// application
    cdb.CreateAbsolute("$Application1");
    cdb.Write("Class", "RealTimeApplication");
//functions
    cdb.CreateAbsolute("$Application1.+Functions");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM1");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+GAM2");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1");
    cdb.Write("Class", "PIDGAMGroup");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1.+GAM3");
    cdb.Write("Class", "PIDGAM");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup1.+GAM4");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer");
    cdb.Write("Class", "ReferenceContainer");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Inputs");
    cdb.Write("Class", "RealTimeDataDefContainer");
    cdb.Write("IsInput", "true");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Inputs.+Error");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "TrackError");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Inputs.+Error.+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Default", "2");
    cdb.Write("Path", "+DDB1.PidError2");
    cdb.Write("IsFinal", "true");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs");
    cdb.Write("Class", "RealTimeDataDefContainer");
    cdb.Write("IsOutput", "true");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs.+Control");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs.+Control.+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "+DDB2.PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs.+Noise");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlNoise");
    cdb.Write("IsFinal", "true");
    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs.+Noise.+noiseValue");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "float32");
    cdb.Write("Default", "2");
    cdb.Write("Path", "+DDB2.PidNoise");
    cdb.Write("IsFinal", "true");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs");
    cdb.Write("IsOutput", "true");
    cdb.Write("IsFinal", "true");
    cdb.Write("Class", "RealTimeDataDefContainer");
    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM5.+Outputs.+Control");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "true");
    cdb.Write("Default", "1");
    cdb.Write("Path", "+DDB1.u1");

    cdb.CreateAbsolute("$Application1.+Functions.+GAMContainer.+GAM6");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2");
    cdb.Write("Class", "PIDGAMGroup");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM7");
    cdb.Write("Class", "PIDGAM");

    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM7.+Inputs");
    cdb.Write("Class", "RealTimeDataDefContainer");
    cdb.Write("IsInput", "true");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM7.+Inputs.+Error");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "TrackError");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM7.+Inputs.+Error.+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Default", "2");
    cdb.Write("Path", "+DDB3.PidError2");
    cdb.Write("IsFinal", "true");

    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM7.+Outputs");
    cdb.Write("Class", "RealTimeDataDefContainer");
    cdb.Write("IsOutput", "true");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM7.+Outputs.+Control");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "ControlIn");
    cdb.Write("IsFinal", "false");
    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM7.+Outputs.+Control.+Par2");
    cdb.Write("Class", "RealTimeGenericDataDef");
    cdb.Write("Type", "uint32");
    cdb.Write("Path", "+DDB4.PidControl2");
    cdb.Write("Default", "1");
    cdb.Write("IsFinal", "true");



    cdb.CreateAbsolute("$Application1.+Functions.+PIDGroup2.+GAM8");
    cdb.Write("Class", "PIDGAM");

// state 1
    cdb.CreateAbsolute("$Application1.+States");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateAbsolute("$Application1.+States.+State1");
    cdb.Write("Class", "RealTimeState");
    cdb.CreateAbsolute("$Application1.+States.+State1.+Threads");
    cdb.Write("Class", "ReferenceContainer");

// state 1 threads
    cdb.CreateAbsolute("$Application1.+States.+State1.+Threads.+Thread1");
    cdb.Write("Class", "RealTimeThread");
    const char8 *functionsT1[2] = { ":+Functions.+GAM1", ":+Functions.+GAM2" };
    cdb.Write("Functions", functionsT1);
    cdb.CreateAbsolute("$Application1.+States.+State1.+Threads.+Thread2");
    cdb.Write("Class", "RealTimeThread");
    const char8 *functionsT2[1] = { ":+Functions.+PIDGroup1" };
    cdb.Write("Functions", functionsT2);

// state 2
    cdb.CreateAbsolute("$Application1.+States.+State2");
    cdb.Write("Class", "RealTimeState");
    cdb.CreateAbsolute("$Application1.+States.+State2.+Threads");
    cdb.Write("Class", "ReferenceContainer");

// state 2 threads
    cdb.CreateAbsolute("$Application1.+States.+State2.+Threads.+Thread1");
    cdb.Write("Class", "RealTimeThread");
    const char8 *functionsT3[1] = { ":+Functions.+GAMContainer" };
    cdb.Write("Functions", functionsT3);
    cdb.CreateAbsolute("$Application1.+States.+State2.+Threads.+Thread2");
    cdb.Write("Class", "RealTimeThread");
    const char8 *functionsT4[2] = { ":+Functions.+PIDGroup2.+GAM7", ":+Functions.+PIDGroup2.+GAM8" };
    cdb.Write("Functions", functionsT4);

// data
    cdb.CreateAbsolute("$Application1.+Data");
    cdb.Write("Class", "RealTimeDataSourceDefContainer");
    cdb.Write("IsFinal", "false");
    cdb.MoveToRoot();
    return cdb;
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMTESTHELPER_H_ */

