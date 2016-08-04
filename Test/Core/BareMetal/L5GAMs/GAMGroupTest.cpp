/**
 * @file GAMGroupTest.cpp
 * @brief Source file for class GAMGroupTest
 * @date 07/03/2016
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
 * the class GAMGroupTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "GAMGroupTest.h"
#include "GAMSchedulerI.h"
#include "StandardParser.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief GAMSchedulerI empty implementation to support the GAMGroup tests
 */
class GAMGroupTestScheduler1: public GAMSchedulerI {
public:
    CLASS_REGISTER_DECLARATION()

GAMGroupTestScheduler1    ();

    virtual void StartExecution();

    virtual void StopExecution();
};

GAMGroupTestScheduler1::GAMGroupTestScheduler1() :
        GAMSchedulerI() {

}

void GAMGroupTestScheduler1::StartExecution() {

}

void GAMGroupTestScheduler1::StopExecution() {

}

CLASS_REGISTER(GAMGroupTestScheduler1, "1.0")

/**
 * @brief GAM empty implementation to support the GAMGroup tests
 */
class GAMGroupTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

GAMGroupTestGAM1    ();

    virtual bool Execute();

    virtual bool SetContext(ConstReference contextIn);

    virtual ConstReference GetContext();


    bool failContext;
private:
    ConstReference context;
};


GAMGroupTestGAM1::GAMGroupTestGAM1() :
        GAM() {
    failContext = false;
}

bool GAMGroupTestGAM1::Execute() {
    return true;
}

bool GAMGroupTestGAM1::SetContext(ConstReference contextIn) {
    context = contextIn;
    return !failContext;
}

ConstReference GAMGroupTestGAM1::GetContext() {
    return context;
}




CLASS_REGISTER(GAMGroupTestGAM1, "1.0")

/**
 * @brief GAMGroup empty implementation to support the GAMGroup tests
 */
class GAMGroupTestGAMGroup1: public GAMGroup {
public:
    CLASS_REGISTER_DECLARATION()

GAMGroupTestGAMGroup1    ();

    virtual bool PrepareNextState(const char8 * currentStateName,
                                  const char8 * nextStateName);

    bool SetContext(ConstReference context);

    uint32 GAMsContainerSize();

};

GAMGroupTestGAMGroup1::GAMGroupTestGAMGroup1() :
        GAMGroup() {
}

bool GAMGroupTestGAMGroup1::PrepareNextState(const char8 * currentStateName,
                                             const char8 * nextStateName) {
    return true;

}

bool GAMGroupTestGAMGroup1::SetContext(ConstReference context) {
    return GAMGroup::SetContext(context);
}


uint32 GAMGroupTestGAMGroup1::GAMsContainerSize(){
    return GAMs.Size();
}
CLASS_REGISTER(GAMGroupTestGAMGroup1, "1.0")

class GAMGroupTestContext1: public Object {
public:
    CLASS_REGISTER_DECLARATION()

GAMGroupTestContext1    ();

    int32 GetProperty1() const;

    float32 GetProperty2()const;

private:
    int32 propertyToBeShared1;

    float32 propertyToBeShared2;
};

GAMGroupTestContext1::GAMGroupTestContext1() :
        Object() {
    propertyToBeShared1 = 1;
    propertyToBeShared2 = 2;
}

int32 GAMGroupTestContext1::GetProperty1() const{
    return propertyToBeShared1;
}

float32 GAMGroupTestContext1::GetProperty2() const{
    return propertyToBeShared2;
}

CLASS_REGISTER(GAMGroupTestContext1, "1.0")
/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseGAMGroupEnviroment(const char8 * const config) {
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->CleanUp();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Application1");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    return ok;
}

//Standard valid config with GAMs, ReferenceContainers and GAMGroups
static const char8* config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = GAMGroupTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMGroup1 = {"
        "            Class = GAMGroupTestGAMGroup1"
        "            +GAM2 = {"
        "                Class = GAMGroupTestGAM1"
        "                InputSignals = {"
        "                   Signal0 = {"
        "                      Type = uint32"
        "                   }"
        "                }"
        "            }"
        "            +GAM3 = {"
        "                Class = GAMGroupTestGAM1"
        "                OutputSignals = {"
        "                   Signal2 = {"
        "                      Type = uint32"
        "                   }"
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
        "                    Functions = { GAM1 GAMGroup1 }"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        DefaultDataSource = DDB1"
        "        Class = ReferenceContainer"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMGroupTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool GAMGroupTest::TestConstructor() {
    GAMGroupTestGAMGroup1 test;
    return test.Size() == 0;
}


bool GAMGroupTest::TestInitialise(){
    bool ret = InitialiseGAMGroupEnviroment(config1);
    ReferenceT<GAMGroupTestGAMGroup1> gamGroup1;

    if (ret) {
        gamGroup1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMGroup1");
        ret = gamGroup1.IsValid();
    }
    return ret && (gamGroup1->GAMsContainerSize() == 2);
}

bool GAMGroupTest::TestSetContext() {
    bool ret = InitialiseGAMGroupEnviroment(config1);
    ReferenceT<GAMGroupTestGAM1> gam1;
    ReferenceT<GAMGroupTestGAM1> gam2;
    ReferenceT<GAMGroupTestGAM1> gam3;
    ReferenceT<GAMGroupTestGAMGroup1> gamGroup1;

    if (ret) {
        gam1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAM1");
        ret = gam1.IsValid();
    }
    if (ret) {
        gam2 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMGroup1.GAM2");
        ret = gam2.IsValid();
    }
    if (ret) {
        gam3 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMGroup1.GAM3");
        ret = gam3.IsValid();
    }
    if (ret) {
        gamGroup1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMGroup1");
        ret = gamGroup1.IsValid();
    }
    if (ret) {
        ret = (gamGroup1->Size() == 2);
    }
    ConstReferenceT(GAMGroupTestContext1) context("GAMGroupTestContext1");
    if (ret) {
        ret = context.IsValid();
    }
    if (ret) {
        ret = (gam1->GetContext() != context);
    }
    if (ret) {
        ret = (gam2->GetContext() != context);
    }
    if (ret) {
        ret = (gam3->GetContext() != context);
    }
    if (ret) {
        ret = (gamGroup1->SetContext(context));
    }
    if (ret) {
        ret = (gam1->GetContext() != context);
    }
    if (ret) {
        ret = (gam2->GetContext() == context);
    }
    if (ret) {
        ret = (gam3->GetContext() == context);
    }
    ConstReferenceT(GAMGroupTestContext1) thisGAMContext = context;
    if (ret) {
        ret = thisGAMContext.IsValid();
    }
    if (ret) {
        ret = (thisGAMContext->GetProperty1() == 1u);
    }
    if (ret) {
        ret = (thisGAMContext->GetProperty2() == 2);
    }
    return ret;
}

bool GAMGroupTest::TestSetContext_Failure() {
    bool ret = InitialiseGAMGroupEnviroment(config1);
    ReferenceT<GAMGroupTestGAM1> gam1;
    ReferenceT<GAMGroupTestGAM1> gam2;
    ReferenceT<GAMGroupTestGAM1> gam3;
    ReferenceT<GAMGroupTestGAMGroup1> gamGroup1;

    if (ret) {
        gam1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAM1");
        ret = gam1.IsValid();
    }
    if (ret) {
        gam2 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMGroup1.GAM2");
        ret = gam2.IsValid();
    }
    if (ret) {
        gam3 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMGroup1.GAM3");
        ret = gam3.IsValid();
    }
    if (ret) {
        gamGroup1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMGroup1");
        ret = gamGroup1.IsValid();
    }
    if (ret) {
        ret = (gamGroup1->Size() == 2);
    }
    ReferenceT<GAMGroupTestContext1> context("GAMGroupTestContext1");
    if (ret) {
        ret = context.IsValid();
    }
    if (ret) {
        ret = (gam1->GetContext() != context);
    }
    if (ret) {
        ret = (gam2->GetContext() != context);
    }
    if (ret) {
        ret = (gam3->GetContext() != context);
    }
    if (ret) {
        gam2->failContext = true;
        ret = !(gamGroup1->SetContext(context));
    }
    return ret;
}

