/**
 * @file RealTimeApplicationTest.cpp
 * @brief Source file for class RealTimeApplicationTest
 * @date 26/feb/2016
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
 * the class RealTimeApplicationTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeApplicationTest.h"
#include "RealTimeState.h"
#include "RealTimeThread.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeDataSourceDefContainer.h"
#include "GAMTestHelper.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeApplicationTest::RealTimeApplicationTest() {
    cdb=GetCDB();
    bool x = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    printf("\nINITIALIZATION = %d\n", x);
}

RealTimeApplicationTest::~RealTimeApplicationTest() {
    ObjectRegistryDatabase::Instance()->CleanUp();
}

bool RealTimeApplicationTest::TestConstructor() {
    return true;
}

bool RealTimeApplicationTest::TestConfigureArchitecture() {

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    ReferenceT<RealTimeApplication> application = god->Find("$Application1");
    if (!application.IsValid()) {
        return false;
    }

    application->ConfigureArchitecture();

    ReferenceT<RealTimeState> state1 = god->Find("$Application1.+States.+State1");
    if (!state1.IsValid()) {
        return false;
    }

    if (state1->GetNumberOfGAMGroups() != 1) {
        return false;
    }

    ReferenceT<RealTimeThread> thread1 = god->Find("$Application1.+States.+State1.+Threads.+Thread1");
    if (!thread1.IsValid()) {
        return false;
    }

    if (thread1->GetNumberOfGAMs() != 2) {
        printf("\n%d\n", thread1->GetNumberOfGAMs());
        return false;
    }

    ReferenceT<GAM> *gams = thread1->GetGAMs();
    ReferenceT<GAM> test1 = god->Find("$Application1.+Functions.+GAM1");
    ReferenceT<GAM> test2 = god->Find("$Application1.+Functions.+GAM2");

    if ((!test1.IsValid()) || (!test2.IsValid())) {
        return false;
    }
    if (gams[0] != test1) {
        return false;
    }
    if (gams[1] != test2) {
        return false;
    }

    StreamString output;
    ConfigurationDatabase outputCDB;
    god->ToStructuredData(outputCDB);
    output.Printf("%!", outputCDB);
    printf("\n%s\n", output.Buffer());

    return true;
}

bool RealTimeApplicationTest::TestConfigureArchitectureGAMGroup() {

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("$Application1");
    if (!application.IsValid()) {
        return false;
    }

    application->ConfigureArchitecture();

    ReferenceT<RealTimeState> state1 = god->Find("$Application1.+States.+State1");
    if (!state1.IsValid()) {
        return false;
    }

    if (state1->GetNumberOfGAMGroups() != 1) {
        return false;
    }

    ReferenceT<RealTimeThread> thread2 = god->Find("$Application1.+States.+State1.+Threads.+Thread2");
    if (!thread2.IsValid()) {
        return false;
    }

    if (thread2->GetNumberOfFunctions() != 1) {
        printf("\n%d\n", thread2->GetNumberOfFunctions());
        return false;
    }

    if (thread2->GetNumberOfGAMs() != 2) {
        printf("\n%d\n", thread2->GetNumberOfGAMs());
        return false;
    }

    ReferenceT<GAM> *gams = thread2->GetGAMs();
    ReferenceT<GAM> test1 = god->Find("$Application1.+Functions.+PIDGroup1.+GAM3");
    ReferenceT<GAM> test2 = god->Find("$Application1.+Functions.+PIDGroup1.+GAM4");

    if ((!test1.IsValid()) || (!test2.IsValid())) {
        return false;
    }
    if (gams[0] != test1) {
        return false;
    }
    if (gams[1] != test2) {
        return false;
    }

    StreamString output;
    ConfigurationDatabase outputCDB;
    god->ToStructuredData(outputCDB);
    output.Printf("%!", outputCDB);
    printf("\n%s\n", output.Buffer());

    return true;
}

bool RealTimeApplicationTest::TestConfigureDataSource() {

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("$Application1");
    if (!application.IsValid()) {
        return false;
    }

    if (!application->ConfigureArchitecture()) {
        return false;
    }

    if (!application->ConfigureDataSource()) {
        return false;
    }

    ReferenceT<RealTimeDataSourceDefContainer> data = god->Find("$Application1.+Data");
    if (!data.IsValid()) {
        return false;
    }

    StreamString output;
    ConfigurationDatabase outputCDB;
    god->ToStructuredData(outputCDB);
    output.Printf("%!", outputCDB);
    printf("\n%s\n", output.Buffer());

    printf("\n%d\n", data->Size());
    return data->Size() == 2;

    /*    ReferenceT<RealTimeState> state1 = god->Find("$Application1.+States.+State1");
     if (!state1.IsValid()) {
     return false;
     }

     if (state1->GetNumberOfElements() != 1) {
     return false;
     }

     ReferenceT<RealTimeThread> thread2 = god->Find("$Application1.+States.+State1.+Threads.+Thread2");
     if (!thread2.IsValid()) {
     return false;
     }

     if (thread2->GetNumberOfFunctions() != 1) {
     printf("\n%d\n", thread2->GetNumberOfFunctions());
     return false;
     }

     if (thread2->GetNumberOfGAMs() != 2) {
     printf("\n%d\n", thread2->GetNumberOfGAMs());
     return false;
     }

     ReferenceT<GAM> *gams = thread2->GetGAMs();
     ReferenceT<GAM> test1 = god->Find("$Application1.+Functions.+PIDGroup1.+GAM3");
     ReferenceT<GAM> test2 = god->Find("$Application1.+Functions.+PIDGroup1.+GAM4");

     if ((!test1.IsValid()) || (!test2.IsValid())) {
     return false;
     }
     if (gams[0] != test1) {
     return false;
     }
     if (gams[1] != test2) {
     return false;
     }
     */
    return true;

}

