/**
 * @file RealTimeDataSourceDefTest.cpp
 * @brief Source file for class RealTimeDataSourceDefTest
 * @date 04/03/2016
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
 * the class RealTimeDataSourceDefTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSourceDefTest.h"
#include "GAMTestHelper.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool RealTimeDataSourceDefTest::TestConstructor() {
    RealTimeDataSourceDef x;
    return (x.Size() == 0);
}

bool RealTimeDataSourceDefTest::TestAddConsumer() {

    RealTimeDataSourceDef def;
    const uint32 size = 32;

    for (uint32 i = 0u; i < size; i++) {
        ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        def.AddConsumer("state1", gam);
    }

    for (uint32 i = 0u; i < size; i++) {
        ReferenceT<PIDGAM> gam = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        def.AddConsumer("state2", gam);
    }

    if (def.Size() != 2) {
        return false;
    }

    if (def.GetNumberOfConsumers("state1") != size) {
        return false;
    }

    if (def.GetNumberOfConsumers("state2") != size) {
        return false;
    }
    return true;
}

bool RealTimeDataSourceDefTest::TestAddProducer() {
    RealTimeDataSourceDef def;
    const uint32 size = 32;

    for (uint32 i = 0u; i < size; i++) {
        ReferenceT<PIDGAM> gamS1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ReferenceT<PIDGAM> gamS2 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        def.AddProducer("state1", gamS1);
        def.AddProducer("state2", gamS2);
    }

    if (def.Size() != 2) {
        return false;
    }

    if (def.GetNumberOfProducers("state1") != size) {
        return false;
    }

    if (def.GetNumberOfProducers("state2") != size) {
        return false;
    }
    return true;
}

bool RealTimeDataSourceDefTest::TestGetNumberOfConsumers() {
    return TestAddConsumer();
}

bool RealTimeDataSourceDefTest::TestGetNumberOfProducers() {
    return TestAddProducer();
}

bool RealTimeDataSourceDefTest::TestVerifyTrue() {
    RealTimeDataSourceDef def;
    const uint32 size = 32;

    for (uint32 i = 0u; i < size; i++) {
        ReferenceT<PIDGAM> gamS1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ReferenceT<PIDGAM> gamS2 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        def.AddConsumer("state1", gamS1);
        def.AddConsumer("state2", gamS2);
    }
    ReferenceT<PIDGAM> prodS1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def.AddProducer("state1", prodS1);

    ReferenceT<PIDGAM> prodS2 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def.AddProducer("state2", prodS2);

    return def.Verify();
}

bool RealTimeDataSourceDefTest::TestVerifyFalse_MoreThanOneProducer() {
    RealTimeDataSourceDef def;
    const uint32 size = 32;

    for (uint32 i = 0u; i < size; i++) {
        ReferenceT<PIDGAM> gamS1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ReferenceT<PIDGAM> gamS2 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        def.AddConsumer("state1", gamS1);
        def.AddConsumer("state2", gamS2);
    }
    ReferenceT<PIDGAM> prodS1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def.AddProducer("state1", prodS1);

    ReferenceT<PIDGAM> prodS2 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<PIDGAM> prodX = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    def.AddProducer("state2", prodS2);
    def.AddProducer("state2", prodX);

    return !def.Verify();

}

bool RealTimeDataSourceDefTest::TestVerifyNoConsumers() {
    RealTimeDataSourceDef def;
    const uint32 size = 32;

    for (uint32 i = 0u; i < size; i++) {
        ReferenceT<PIDGAM> gamS1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        def.AddConsumer("state1", gamS1);
    }
    ReferenceT<PIDGAM> prodS1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    def.AddProducer("state1", prodS1);

    ReferenceT<PIDGAM> prodS2 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    def.AddProducer("state2", prodS2);

    return def.Verify();

}

