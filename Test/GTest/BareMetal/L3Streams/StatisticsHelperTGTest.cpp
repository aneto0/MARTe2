/**
 * @file StatisticsHelperTGTest.cpp
 * @brief Source file for class StatisticsHelperTGTest
 * @date 21/04/2017
 * @author Andre' Torres
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
 * the class StatisticsHelperTGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
#include "StatisticsHelperTTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/* uint8 */

TEST(StatisticsHelperTGTest,TestConstructor_uint8_32) {
    StatisticsHelperTTest<uint8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(32));
}

TEST(StatisticsHelperTGTest,TestConstructor_uint8_1) {
    StatisticsHelperTTest<uint8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(1));
}

TEST(StatisticsHelperTGTest,TestReset_uint8) {
    StatisticsHelperTTest<uint8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testReset(32));
}

TEST(StatisticsHelperTGTest,TestPushSample_uint8) {
    StatisticsHelperTTest<uint8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testPushSample(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_1_uint8) {
    StatisticsHelperTTest<uint8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_1(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_2_uint8) {
    StatisticsHelperTTest<uint8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_2(32));
}

TEST(StatisticsHelperTGTest,TestGetSize_uint8) {
    StatisticsHelperTTest<uint8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSize(32));
}

TEST(StatisticsHelperTGTest,TestGetCounter_uint8) {
    StatisticsHelperTTest<uint8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetCounter(32));
}

TEST(StatisticsHelperTGTest,TestGetAvg_0_uint8) {
    StatisticsHelperTTest<uint8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,0));
}

TEST(StatisticsHelperTGTest,TestGetAvg_1_uint8) {
    StatisticsHelperTTest<uint8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,1));
}

TEST(StatisticsHelperTGTest,TestGetMax_uint8) {
    StatisticsHelperTTest<uint8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMax(32));
}

TEST(StatisticsHelperTGTest,TestGetMin_uint8) {
    StatisticsHelperTTest<uint8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMin(32));
}

TEST(StatisticsHelperTGTest,TestGetSum_uint8) {
    StatisticsHelperTTest<uint8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSum(32));
}

TEST(StatisticsHelperTGTest,TestGetStd_uint8) {
    StatisticsHelperTTest<uint8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetStd());
}

/* uint16 */

TEST(StatisticsHelperTGTest,TestConstructor_uint16_32) {
    StatisticsHelperTTest<uint16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(32));
}

TEST(StatisticsHelperTGTest,TestConstructor_uint16_1) {
    StatisticsHelperTTest<uint16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(1));
}

TEST(StatisticsHelperTGTest,TestReset_uint16) {
    StatisticsHelperTTest<uint16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testReset(32));
}

TEST(StatisticsHelperTGTest,TestPushSample_uint16) {
    StatisticsHelperTTest<uint16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testPushSample(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_1_uint16) {
    StatisticsHelperTTest<uint16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_1(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_2_uint16) {
    StatisticsHelperTTest<uint16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_2(32));
}

TEST(StatisticsHelperTGTest,TestGetSize_uint16) {
    StatisticsHelperTTest<uint16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSize(32));
}

TEST(StatisticsHelperTGTest,TestGetCounter_uint16) {
    StatisticsHelperTTest<uint16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetCounter(32));
}

TEST(StatisticsHelperTGTest,TestGetAvg_0_uint16) {
    StatisticsHelperTTest<uint16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,0));
}

TEST(StatisticsHelperTGTest,TestGetAvg_1_uint16) {
    StatisticsHelperTTest<uint16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,1));
}

TEST(StatisticsHelperTGTest,TestGetMax_uint16) {
    StatisticsHelperTTest<uint16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMax(32));
}

TEST(StatisticsHelperTGTest,TestGetMin_uint16) {
    StatisticsHelperTTest<uint16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMin(32));
}

TEST(StatisticsHelperTGTest,TestGetSum_uint16) {
    StatisticsHelperTTest<uint16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSum(32));
}

TEST(StatisticsHelperTGTest,TestGetStd_uint16) {
    StatisticsHelperTTest<uint16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetStd());
}

/* uint32 */

TEST(StatisticsHelperTGTest,TestConstructor_uint32_32) {
    StatisticsHelperTTest<uint32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(32));
}

TEST(StatisticsHelperTGTest,TestConstructor_uint32_1) {
    StatisticsHelperTTest<uint32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(1));
}

TEST(StatisticsHelperTGTest,TestReset_uint32) {
    StatisticsHelperTTest<uint32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testReset(32));
}

TEST(StatisticsHelperTGTest,TestPushSample_uint32) {
    StatisticsHelperTTest<uint32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testPushSample(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_1_uint32) {
    StatisticsHelperTTest<uint32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_1(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_2_uint32) {
    StatisticsHelperTTest<uint32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_2(32));
}

TEST(StatisticsHelperTGTest,TestGetSize_uint32) {
    StatisticsHelperTTest<uint32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSize(32));
}

TEST(StatisticsHelperTGTest,TestGetCounter_uint32) {
    StatisticsHelperTTest<uint32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetCounter(32));
}

TEST(StatisticsHelperTGTest,TestGetAvg_0_uint32) {
    StatisticsHelperTTest<uint32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,0));
}

TEST(StatisticsHelperTGTest,TestGetAvg_1_uint32) {
    StatisticsHelperTTest<uint32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,1));
}

TEST(StatisticsHelperTGTest,TestGetMax_uint32) {
    StatisticsHelperTTest<uint32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMax(32));
}

TEST(StatisticsHelperTGTest,TestGetMin_uint32) {
    StatisticsHelperTTest<uint32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMin(32));
}

TEST(StatisticsHelperTGTest,TestGetSum_uint32) {
    StatisticsHelperTTest<uint32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSum(32));
}

TEST(StatisticsHelperTGTest,TestGetStd_uint32) {
    StatisticsHelperTTest<uint32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetStd());
}

/* uint64 */

TEST(StatisticsHelperTGTest,TestConstructor_uint64_32) {
    StatisticsHelperTTest<uint64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(32));
}

TEST(StatisticsHelperTGTest,TestConstructor_uint64_1) {
    StatisticsHelperTTest<uint64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(1));
}

TEST(StatisticsHelperTGTest,TestReset_uint64) {
    StatisticsHelperTTest<uint64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testReset(32));
}

TEST(StatisticsHelperTGTest,TestPushSample_uint64) {
    StatisticsHelperTTest<uint64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testPushSample(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_1_uint64) {
    StatisticsHelperTTest<uint64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_1(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_2_uint64) {
    StatisticsHelperTTest<uint64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_2(32));
}

TEST(StatisticsHelperTGTest,TestGetSize_uint64) {
    StatisticsHelperTTest<uint64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSize(32));
}

TEST(StatisticsHelperTGTest,TestGetCounter_uint64) {
    StatisticsHelperTTest<uint64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetCounter(32));
}

TEST(StatisticsHelperTGTest,TestGetAvg_0_uint64) {
    StatisticsHelperTTest<uint64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,0));
}

TEST(StatisticsHelperTGTest,TestGetAvg_1_uint64) {
    StatisticsHelperTTest<uint64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,1));
}

TEST(StatisticsHelperTGTest,TestGetMax_uint64) {
    StatisticsHelperTTest<uint64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMax(32));
}

TEST(StatisticsHelperTGTest,TestGetMin_uint64) {
    StatisticsHelperTTest<uint64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMin(32));
}

TEST(StatisticsHelperTGTest,TestGetSum_uint64) {
    StatisticsHelperTTest<uint64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSum(32));
}

TEST(StatisticsHelperTGTest,TestGetStd_uint64) {
    StatisticsHelperTTest<uint64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetStd());
}

/* int8 */

TEST(StatisticsHelperTGTest,TestConstructor_int8_32) {
    StatisticsHelperTTest<int8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(32));
}

TEST(StatisticsHelperTGTest,TestConstructor_int8_1) {
    StatisticsHelperTTest<int8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(1));
}

TEST(StatisticsHelperTGTest,TestReset_int8) {
    StatisticsHelperTTest<int8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testReset(32));
}

TEST(StatisticsHelperTGTest,TestPushSample_int8) {
    StatisticsHelperTTest<int8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testPushSample(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_1_int8) {
    StatisticsHelperTTest<int8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_1(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_2_int8) {
    StatisticsHelperTTest<int8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_2(32));
}

TEST(StatisticsHelperTGTest,TestGetSize_int8) {
    StatisticsHelperTTest<int8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSize(32));
}

TEST(StatisticsHelperTGTest,TestGetCounter_int8) {
    StatisticsHelperTTest<int8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetCounter(32));
}

TEST(StatisticsHelperTGTest,TestGetAvg_0_int8) {
    StatisticsHelperTTest<int8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,0));
}

TEST(StatisticsHelperTGTest,TestGetAvg_1_int8) {
    StatisticsHelperTTest<int8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,1));
}

TEST(StatisticsHelperTGTest,TestGetMax_int8) {
    StatisticsHelperTTest<int8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMax(32));
}

TEST(StatisticsHelperTGTest,TestGetMin_int8) {
    StatisticsHelperTTest<int8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMin(32));
}

TEST(StatisticsHelperTGTest,TestGetSum_int8) {
    StatisticsHelperTTest<int8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSum(32));
}

TEST(StatisticsHelperTGTest,TestGetStd_int8) {
    StatisticsHelperTTest<int8> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetStd());
}

/* int16 */

TEST(StatisticsHelperTGTest,TestConstructor_int16_32) {
    StatisticsHelperTTest<int16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(32));
}

TEST(StatisticsHelperTGTest,TestConstructor_int16_1) {
    StatisticsHelperTTest<int16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(1));
}

TEST(StatisticsHelperTGTest,TestReset_int16) {
    StatisticsHelperTTest<int16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testReset(32));
}

TEST(StatisticsHelperTGTest,TestPushSample_int16) {
    StatisticsHelperTTest<int16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testPushSample(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_1_int16) {
    StatisticsHelperTTest<int16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_1(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_2_int16) {
    StatisticsHelperTTest<int16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_2(32));
}

TEST(StatisticsHelperTGTest,TestGetSize_int16) {
    StatisticsHelperTTest<int16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSize(32));
}

TEST(StatisticsHelperTGTest,TestGetCounter_int16) {
    StatisticsHelperTTest<int16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetCounter(32));
}

TEST(StatisticsHelperTGTest,TestGetAvg_0_int16) {
    StatisticsHelperTTest<int16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,0));
}

TEST(StatisticsHelperTGTest,TestGetAvg_1_int16) {
    StatisticsHelperTTest<int16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,1));
}

TEST(StatisticsHelperTGTest,TestGetMax_int16) {
    StatisticsHelperTTest<int16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMax(32));
}

TEST(StatisticsHelperTGTest,TestGetMin_int16) {
    StatisticsHelperTTest<int16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMin(32));
}

TEST(StatisticsHelperTGTest,TestGetSum_int16) {
    StatisticsHelperTTest<int16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSum(32));
}

TEST(StatisticsHelperTGTest,TestGetStd_int16) {
    StatisticsHelperTTest<int16> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetStd());
}

/* int32 */

TEST(StatisticsHelperTGTest,TestConstructor_int32_32) {
    StatisticsHelperTTest<int32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(32));
}

TEST(StatisticsHelperTGTest,TestConstructor_int32_1) {
    StatisticsHelperTTest<int32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(1));
}

TEST(StatisticsHelperTGTest,TestReset_int32) {
    StatisticsHelperTTest<int32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testReset(32));
}

TEST(StatisticsHelperTGTest,TestPushSample_int32) {
    StatisticsHelperTTest<int32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testPushSample(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_1_int32) {
    StatisticsHelperTTest<int32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_1(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_2_int32) {
    StatisticsHelperTTest<int32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_2(32));
}

TEST(StatisticsHelperTGTest,TestGetSize_int32) {
    StatisticsHelperTTest<int32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSize(32));
}

TEST(StatisticsHelperTGTest,TestGetCounter_int32) {
    StatisticsHelperTTest<int32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetCounter(32));
}

TEST(StatisticsHelperTGTest,TestGetAvg_0_int32) {
    StatisticsHelperTTest<int32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,0));
}

TEST(StatisticsHelperTGTest,TestGetAvg_1_int32) {
    StatisticsHelperTTest<int32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,1));
}

TEST(StatisticsHelperTGTest,TestGetMax_int32) {
    StatisticsHelperTTest<int32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMax(32));
}

TEST(StatisticsHelperTGTest,TestGetMin_int32) {
    StatisticsHelperTTest<int32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMin(32));
}

TEST(StatisticsHelperTGTest,TestGetSum_int32) {
    StatisticsHelperTTest<int32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSum(32));
}

TEST(StatisticsHelperTGTest,TestGetStd_int32) {
    StatisticsHelperTTest<int32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetStd());
}

/* int64 */

TEST(StatisticsHelperTGTest,TestConstructor_int64_32) {
    StatisticsHelperTTest<int64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(32));
}

TEST(StatisticsHelperTGTest,TestConstructor_int64_1) {
    StatisticsHelperTTest<int64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(1));
}

TEST(StatisticsHelperTGTest,TestReset_int64) {
    StatisticsHelperTTest<int64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testReset(32));
}

TEST(StatisticsHelperTGTest,TestPushSample_int64) {
    StatisticsHelperTTest<int64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testPushSample(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_1_int64) {
    StatisticsHelperTTest<int64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_1(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_2_int64) {
    StatisticsHelperTTest<int64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_2(32));
}

TEST(StatisticsHelperTGTest,TestGetSize_int64) {
    StatisticsHelperTTest<int64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSize(32));
}

TEST(StatisticsHelperTGTest,TestGetCounter_int64) {
    StatisticsHelperTTest<int64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetCounter(32));
}

TEST(StatisticsHelperTGTest,TestGetAvg_0_int64) {
    StatisticsHelperTTest<int64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,0));
}

TEST(StatisticsHelperTGTest,TestGetAvg_1_int64) {
    StatisticsHelperTTest<int64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,1));
}

TEST(StatisticsHelperTGTest,TestGetMax_int64) {
    StatisticsHelperTTest<int64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMax(32));
}

TEST(StatisticsHelperTGTest,TestGetMin_int64) {
    StatisticsHelperTTest<int64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMin(32));
}

TEST(StatisticsHelperTGTest,TestGetSum_int64) {
    StatisticsHelperTTest<int64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSum(32));
}

TEST(StatisticsHelperTGTest,TestGetStd_int64) {
    StatisticsHelperTTest<int64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetStd());
}

/* float32 */

TEST(StatisticsHelperTGTest,TestConstructor_float32_32) {
    StatisticsHelperTTest<float32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(32));
}

TEST(StatisticsHelperTGTest,TestConstructor_float32_1) {
    StatisticsHelperTTest<float32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(1));
}

TEST(StatisticsHelperTGTest,TestReset_float32) {
    StatisticsHelperTTest<float32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testReset(32));
}

TEST(StatisticsHelperTGTest,TestPushSample_float32) {
    StatisticsHelperTTest<float32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testPushSample(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_1_float32) {
    StatisticsHelperTTest<float32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_1(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_2_float32) {
    StatisticsHelperTTest<float32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_2(32));
}

TEST(StatisticsHelperTGTest,TestGetSize_float32) {
    StatisticsHelperTTest<float32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSize(32));
}

TEST(StatisticsHelperTGTest,TestGetCounter_float32) {
    StatisticsHelperTTest<float32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetCounter(32));
}

TEST(StatisticsHelperTGTest,TestGetAvg_0_float32) {
    StatisticsHelperTTest<float32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,0));
}

TEST(StatisticsHelperTGTest,TestGetAvg_1_float32) {
    StatisticsHelperTTest<float32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,1));
}

TEST(StatisticsHelperTGTest,TestGetMax_float32) {
    StatisticsHelperTTest<float32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMax(32));
}

TEST(StatisticsHelperTGTest,TestGetMin_float32) {
    StatisticsHelperTTest<float32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMin(32));
}

TEST(StatisticsHelperTGTest,TestGetSum_float32) {
    StatisticsHelperTTest<float32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSum(32));
}

TEST(StatisticsHelperTGTest,TestGetStd_float32) {
    StatisticsHelperTTest<float32> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetStd());
}

/* float64 */

TEST(StatisticsHelperTGTest,TestConstructor_float64_32) {
    StatisticsHelperTTest<float64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(32));
}

TEST(StatisticsHelperTGTest,TestConstructor_float64_1) {
    StatisticsHelperTTest<float64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testConstructor(1));
}

TEST(StatisticsHelperTGTest,TestReset_float64) {
    StatisticsHelperTTest<float64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testReset(32));
}

TEST(StatisticsHelperTGTest,TestPushSample_float64) {
    StatisticsHelperTTest<float64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testPushSample(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_1_float64) {
    StatisticsHelperTTest<float64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_1(32));
}

TEST(StatisticsHelperTGTest,TestGetSample_2_float64) {
    StatisticsHelperTTest<float64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSample_2(32));
}

TEST(StatisticsHelperTGTest,TestGetSize_float64) {
    StatisticsHelperTTest<float64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSize(32));
}

TEST(StatisticsHelperTGTest,TestGetCounter_float64) {
    StatisticsHelperTTest<float64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetCounter(32));
}

TEST(StatisticsHelperTGTest,TestGetAvg_0_float64) {
    StatisticsHelperTTest<float64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,0));
}

TEST(StatisticsHelperTGTest,TestGetAvg_1_float64) {
    StatisticsHelperTTest<float64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetAvg(32,1));
}

TEST(StatisticsHelperTGTest,TestGetMax_float64) {
    StatisticsHelperTTest<float64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMax(32));
}

TEST(StatisticsHelperTGTest,TestGetMin_float64) {
    StatisticsHelperTTest<float64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetMin(32));
}

TEST(StatisticsHelperTGTest,TestGetSum_float64) {
    StatisticsHelperTTest<float64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetSum(32));
}

TEST(StatisticsHelperTGTest,TestGetStd_float64) {
    StatisticsHelperTTest<float64> statisticsHelperTTest;
    ASSERT_TRUE(statisticsHelperTTest.testGetStd());
}

