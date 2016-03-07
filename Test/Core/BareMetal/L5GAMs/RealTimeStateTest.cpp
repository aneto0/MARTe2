/**
 * @file RealTimeStateTest.cpp
 * @brief Source file for class RealTimeStateTest
 * @date 07/mar/2016
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
 * the class RealTimeStateTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeStateTest.h"
#include "GAMTestHelper.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeStateTest::RealTimeStateTest() {
    cdb = GetCDB();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        printf("\n!!!\n");
    }

}

RealTimeStateTest::~RealTimeStateTest() {
    ObjectRegistryDatabase::Instance()->CleanUp();
}

bool RealTimeStateTest::TestConstructor() {

    RealTimeState test;
    if (test.GetNumberOfGAMGroups() != 0) {
        return false;
    }
    if (test.GetContextActiveBuffer() != 0) {
        return false;
    }

    if (test.GetGAMGroups() != NULL) {
        return false;
    }

    return true;
}

bool RealTimeStateTest::TestConfigureArchitecture() {

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State1");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (!state1->ConfigureArchitecture(*app.operator->())) {
        return false;
    }

    ReferenceT<PIDGAMGroup> group1 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+PIDGroup1");

    if (state1->GetNumberOfGAMGroups() != 1) {
        return false;
    }
    if (state1->GetGAMGroups()[0] != group1) {
        return false;
    }
    return true;

}


bool RealTimeStateTest::TestConfigureArchitecture_SingleInGAMGroup(){
    ReferenceT<RealTimeState> state2 = ObjectRegistryDatabase::Instance()->Find("$Application1.+States.+State2");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (!state2->ConfigureArchitecture(*app.operator->())) {
        return false;
    }

    ReferenceT<PIDGAMGroup> group2 = ObjectRegistryDatabase::Instance()->Find("$Application1.+Functions.+PIDGroup2");

    if (state2->GetNumberOfGAMGroups() != 1) {
        return false;
    }
    if (state2->GetGAMGroups()[0] != group2) {
        return false;
    }
    return true;

}


