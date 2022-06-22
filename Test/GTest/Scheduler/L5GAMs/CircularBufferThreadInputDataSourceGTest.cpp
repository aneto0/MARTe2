/**
 * @file CircularBufferThreadInputDataSourceGTest.cpp
 * @brief Source file for class CircularBufferThreadInputDataSourceGTest.
 * @date 12/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This source file contains the definition of all the methods for
 * the class CircularBufferThreadInputDataSourceGTest (public, protected, and
 * private). Be aware that some methods, such as those inline could be defined
 * on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CircularBufferThreadInputDataSourceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestConstructor) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestInitialise) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestInitialise_CpuMask) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_CpuMask());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestInitialise_PriorityLevel_Idle) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_PriorityLevel_Idle());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestInitialise_PriorityLevel_Unknown) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_PriorityLevel_Unknown());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestInitialise_PriorityLevel_RealTime) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_PriorityLevel_RealTime());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestInitialise_StackSize) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_StackSize());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestInitialise_SleepTime) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_SleepTime());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestInitialise_SignalDefinitionInterleaved) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_SignalDefinitionInterleaved());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSynchronise) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSynchronise_FullRolling) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise_FullRolling());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSynchronise_GetLatest) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise_GetLatest());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSynchronise_SleepTime) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise_SleepTime());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestGetBrokerName) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestGetInputBrokers) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestGetOutputBrokers) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_False_WrittenSignal) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_WrittenSignal());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_PacketMemberSizes) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_PacketMemberSizes());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_SignalDefinitionInterleaved) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_SignalDefinitionInterleaved());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_PacketMemberSizes_HeaderSize) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_PacketMemberSizes_HeaderSize());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_False_SignalDefinitionInterleaved_NOfSamples) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_SignalDefinitionInterleaved_NOfSamples());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_SignalDefinitionInterleaved_HeaderSize) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_SignalDefinitionInterleaved_HeaderSize());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_PacketMemberSizes_RecallAllocation) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_PacketMemberSizes_RecallAllocation());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_SignalDefinitionInterleaved_RecallAllocation) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_SignalDefinitionInterleaved_RecallAllocation());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_False_InvalidPacketMemberSize) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_InvalidPacketMemberSize());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_False_PacketMemberSizeGreater) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_PacketMemberSizeGreater());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestPrepareNextState) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestGetInputOffset) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestGetInputOffset());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,PrepareInputOffsets) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.PrepareInputOffsets());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestGetOutputOffset) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestGetOutputOffset());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestExecute) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestExecute_SameSignalDifferentModes) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_SameSignalDifferentModes());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestExecute_ErrorCheck) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_ErrorCheck());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestExecute_ErrorCheck_Overwrite) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_ErrorCheck_Overwrite());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestExecute_ErrorCheck_DriverRead) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_ErrorCheck_DriverRead());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestExecute_ErrorCheck_Both) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_ErrorCheck_Both());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_ErrorCheck_False_BadType) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_ErrorCheck_False_BadType());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_ErrorCheck_False_BadSize) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_ErrorCheck_False_BadSize());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestExecute_TimeStamp) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_TimeStamp());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestExecute_TimeStamp_NoRead) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_TimeStamp_NoRead());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestExecute_TimeStamp_FalseDriverRead) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_TimeStamp_FalseDriverRead());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_TimeStamp_False_BadSize) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_TimeStamp_False_BadSize());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_TimeStamp_False_BadType) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_TimeStamp_False_BadType());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestPurge) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestPurge());
}

TEST(Scheduler_L5GAMs_CircularBufferThreadInputDataSourceGTest,TestTerminateInputCopy) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestTerminateInputCopy());
}
