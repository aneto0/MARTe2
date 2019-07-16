/**
 * @file SaturatedIntegerTest.cpp
 * @brief Header file for class AnyType
 * @date 28 May 2019
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#include <stdio.h>

#include "SaturatedIntegerTest.h"
#include "TestSupport.h"

static const int FailIfNeg   = 0x08;
static const int FailIfPos   = 0x04;
static const int FailIfNever = 0x00;

//#define CheckFail(expect,map) ((expect && map) != 0)
//#define Pack(n) n
static inline int CheckFail(int expect,int behave){
//	printf("(%i,%i->%i)",expect,behave,(expect & behave));
	return ((expect & behave)!=0);
}


#define TESTCONVBLOCKTYPE(type,N,expect,map,line) \
TEST(SaturatedIntegerGTest,TestConvTo ## type ## __ ## line) {  \
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint64,type>(N)),CheckFail(expect,map[0]));\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint32,type>(N)),CheckFail(expect,map[1]));\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint16,type>(N)),CheckFail(expect,map[2]));\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint8 ,type>(N)),CheckFail(expect,map[3]));\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int64,type>(N)),CheckFail(expect,map[4]));\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int32,type>(N)),CheckFail(expect,map[5]));\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int16,type>(N)),CheckFail(expect,map[6]));\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int8 ,type>(N)),CheckFail(expect,map[7]));\
}

static const int MapU64[8] = {FailIfNever,FailIfNever,FailIfNever,FailIfNever,FailIfNeg,FailIfNeg,FailIfNeg,FailIfNeg};
#define TESTCONVBLOCK64U(N,expect,line) TESTCONVBLOCKTYPE(uint64,N,expect,MapU64,line)

static const int MapU32[8] = {FailIfPos,FailIfNever,FailIfNever,FailIfNever,FailIfNeg+FailIfPos,FailIfNeg,FailIfNeg,FailIfNeg};
#define TESTCONVBLOCK32U(N,expect,line) TESTCONVBLOCKTYPE(uint32,N,expect,MapU32,line)

static const int MapU16[8] = {FailIfPos,FailIfPos,FailIfNever,FailIfNever,FailIfNeg+FailIfPos,FailIfNeg+FailIfPos,FailIfNeg,FailIfNeg};
#define TESTCONVBLOCK16U(N,expect,line) TESTCONVBLOCKTYPE(uint16,N,expect,MapU16,line)

static const int MapU8[8] = {FailIfPos,FailIfPos,FailIfPos,FailIfNever,FailIfNeg+FailIfPos,FailIfNeg+FailIfPos,FailIfNeg+FailIfPos,FailIfNeg};
#define TESTCONVBLOCK8U(N,expect,line) TESTCONVBLOCKTYPE(uint8,N,expect,MapU8,line)

static const int Map8[8] = {FailIfPos,FailIfPos,FailIfPos,FailIfPos,FailIfNeg+FailIfPos,FailIfNeg+FailIfPos,FailIfNeg+FailIfPos,FailIfNever};
#define TESTCONVBLOCK8(N,expect,line) TESTCONVBLOCKTYPE(int8,N,expect,Map8,line)

static const int Map16[8] = {FailIfPos,FailIfPos,FailIfPos,FailIfNever,FailIfNeg+FailIfPos,FailIfNeg+FailIfPos,FailIfNever,FailIfNever};
#define TESTCONVBLOCK16(N,expect,line) TESTCONVBLOCKTYPE(int16,N,expect,Map16,line)

static const int Map32[8] = {FailIfPos,FailIfPos,FailIfNever,FailIfNever,FailIfNeg+FailIfPos,FailIfNever,FailIfNever,FailIfNever};
#define TESTCONVBLOCK32(N,expect,line) TESTCONVBLOCKTYPE(int32,N,expect,Map32,line)

static const int Map64[8] = {FailIfPos,FailIfNever,FailIfNever,FailIfNever,FailIfNever,FailIfNever,FailIfNever,FailIfNever};
#define TESTCONVBLOCK64(N,expect,line) TESTCONVBLOCKTYPE(int64,N,expect,Map64,line)


TESTCONVBLOCK8  (-129,FailIfNeg,__LINE__);
TESTCONVBLOCK8U (-129,FailIfNeg,__LINE__);
TESTCONVBLOCK16 (-129,FailIfNever,__LINE__);
TESTCONVBLOCK16U(-129,FailIfNeg,__LINE__);
TESTCONVBLOCK32 (-129,FailIfNever,__LINE__);
TESTCONVBLOCK32U(-129,FailIfNeg,__LINE__);
TESTCONVBLOCK64 (-129,FailIfNever,__LINE__);
TESTCONVBLOCK64U(-129,FailIfNeg,__LINE__);

TESTCONVBLOCK8  (-1,FailIfNever,__LINE__);
TESTCONVBLOCK8U (-1,FailIfNeg,__LINE__);
TESTCONVBLOCK16 (-1,FailIfNever,__LINE__);
TESTCONVBLOCK16U(-1,FailIfNeg,__LINE__);
TESTCONVBLOCK32 (-1,FailIfNever,__LINE__);
TESTCONVBLOCK32U(-1,FailIfNeg,__LINE__);
TESTCONVBLOCK64 (-1,FailIfNever,__LINE__);
TESTCONVBLOCK64U(-1,FailIfNeg,__LINE__);

TESTCONVBLOCK8  (120,FailIfNever,__LINE__);
TESTCONVBLOCK8U (120,FailIfNever,__LINE__);
TESTCONVBLOCK16 (120,FailIfNever,__LINE__);
TESTCONVBLOCK16U(120,FailIfNever,__LINE__);
TESTCONVBLOCK32 (120,FailIfNever,__LINE__);
TESTCONVBLOCK32U(120,FailIfNever,__LINE__);
TESTCONVBLOCK64 (120,FailIfNever,__LINE__);
TESTCONVBLOCK64U(120,FailIfNever,__LINE__);

TESTCONVBLOCK8  (240,FailIfPos,__LINE__);
TESTCONVBLOCK8U (240,FailIfNever,__LINE__);
TESTCONVBLOCK16 (240,FailIfNever,__LINE__);
TESTCONVBLOCK16U(240,FailIfNever,__LINE__);
TESTCONVBLOCK32 (240,FailIfNever,__LINE__);
TESTCONVBLOCK32U(240,FailIfNever,__LINE__);
TESTCONVBLOCK64 (240,FailIfNever,__LINE__);
TESTCONVBLOCK64U(240,FailIfNever,__LINE__);

TESTCONVBLOCK8  (32000,FailIfPos,__LINE__);
TESTCONVBLOCK8U (32000,FailIfPos,__LINE__);
TESTCONVBLOCK16 (32000,FailIfNever,__LINE__);
TESTCONVBLOCK16U(32000,FailIfNever,__LINE__);
TESTCONVBLOCK32 (32000,FailIfNever,__LINE__);
TESTCONVBLOCK32U(32000,FailIfNever,__LINE__);
TESTCONVBLOCK64 (32000,FailIfNever,__LINE__);
TESTCONVBLOCK64U(32000,FailIfNever,__LINE__);

TESTCONVBLOCK8  (64000,FailIfPos,__LINE__);
TESTCONVBLOCK8U (64000,FailIfPos,__LINE__);
TESTCONVBLOCK16 (64000,FailIfPos,__LINE__);
TESTCONVBLOCK16U(64000,FailIfNever,__LINE__);
TESTCONVBLOCK32 (64000,FailIfNever,__LINE__);
TESTCONVBLOCK32U(64000,FailIfNever,__LINE__);
TESTCONVBLOCK64 (64000,FailIfNever,__LINE__);
TESTCONVBLOCK64U(64000,FailIfNever,__LINE__);


TEST(SaturatedIntegerGTest,TestMathU64) {  \
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint64>(20000000000,10000000000)),4); \
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint64>(10000000000,20000000000)),3);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint64>(10000000000,9999999990)),0);\
}

TEST(SaturatedIntegerGTest,TestMathU32) {  \
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint32>(2400000000U,2000000000U)),2);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint32>(2000000001, 2000000000)),5);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint32>(65000,66000)),3);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint32>(130000,65000)),4);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint32>(65000,64000)),0);\
}

TEST(SaturatedIntegerGTest,TestMathU16) {  \
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint16>(32000,32001)),3);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint16>(32002,32000)),4);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint16>(32001,32000)),5);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint16>(6558,6558)),6);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint16>(3276,3275)),7);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint16>(3005,3000)),0);\
}

TEST(SaturatedIntegerGTest,TestMathU8) {  \
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint8> (122,120)),4);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint8> (120,121)),3);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint8> (121,120)),5);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint8> (33,30)),6);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint8> (28,25)),7);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint8> (24,20)),0);\
}

TEST(SaturatedIntegerGTest,TestMath64) {  \
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int64>(20000000000, 10000000000)),4);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int64>(20000000000U,19768070922U)),5);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int64>(20000000000U,19768070925U)),0);\
}

TEST(SaturatedIntegerGTest,TestMath32) {  \
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int32>(2000000000U,2000000000U)),2);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int32>(130000,65000)),4);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int32>(1000000001, 1000000000)),5);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int32>(220000000,220000001)),6);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int32>(200000000,200000001)),7);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int32>(65000,64000)),0);\
}

TEST(SaturatedIntegerGTest,TestMath16) {  \
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int16>(17001,17000)),2);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int16>(16002,16000)),4);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int16>(16001,16000)),5);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int16>(3276,3277)),6);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int16>(3275,3276)),7);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int16>(2905,2900)),0);\
}

TEST(SaturatedIntegerGTest,TestMath8) {  \
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int8> (66,66)),2);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int8> (62,60)),4);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int8> (61,60)),5);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int8> (17,15)),6);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int8> (11,12)),7);\
ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int8> (12,10)),0);\
}



#if 0


#define TESTCONVBLOCK64U(N,expect) \
TEST(SatiratedIntegerGTest,TestConvToU64) {  \
		ASSERT_BOOL(SaturatedIntegerTest::TestConversion<uint64,uint64>(N),false)\
		ASSERT_BOOL(SaturatedIntegerTest::TestConversion<uint32,uint64>(N),false)\
		ASSERT_BOOL(SaturatedIntegerTest::TestConversion<uint16,uint64>(N),false)\
		ASSERT_BOOL(SaturatedIntegerTest::TestConversion<uint8 ,uint64>(N),false)\
		ASSERT_BOOL(SaturatedIntegerTest::TestConversion< int64,uint64>(N),(expect=='N'))\
		ASSERT_BOOL(SaturatedIntegerTest::TestConversion< int32,uint64>(N),(expect=='N'))\
		ASSERT_BOOL(SaturatedIntegerTest::TestConversion< int16,uint64>(N),(expect=='N'))\
		ASSERT_BOOL(SaturatedIntegerTest::TestConversion< int8 ,uint64>(N),(expect=='N'))\
}
#define TESTCONVBLOCK32U(N,expect) \
		ASSERT_BOOL(SaturatedIntegerTest::TestConversion<uint64,uint32>(N),(expect=='P'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint32,uint32>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint16,uint32>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint8 ,uint32>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int64,uint32>(N)),((expect=='P')||(expect=='N')))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int32,uint32>(N)),(expect=='N'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int16,uint32>(N)),(expect=='N'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int8 ,uint32>(N)),(expect=='N'))\


#define TESTCONVBLOCK16U(N,expect) \
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint64,uint16>(N)),(expect=='P'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint32,uint16>(N)),(expect=='P'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint16,uint16>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint8 ,uint16>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int64,uint16>(N)),((expect=='P')||(expect=='N')))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int32,uint16>(N)),((expect=='P')||(expect=='N')))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int16,uint16>(N)),(expect=='N'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int8 ,uint16>(N)),(expect=='N'))\


#define TESTCONVBLOCK8U(N,expect) \
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint64,uint8 >(N)),(expect=='P'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint32,uint8 >(N)),(expect=='P'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint16,uint8 >(N)),(expect=='P'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint8 ,uint8 >(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int64,uint8 >(N)),((expect=='P')||(expect=='N')))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int32,uint8 >(N)),((expect=='P')||(expect=='N')))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int16,uint8 >(N)),((expect=='P')||(expect=='N')))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int8 ,uint8 >(N)),(expect=='N'))\

#define TESTCONVBLOCK8(N,expect) \
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint64, int8 >(N)),(expect=='P'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint32, int8 >(N)),(expect=='P'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint16, int8 >(N)),(expect=='P'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint8 , int8 >(N)),(expect=='P'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int64, int8 >(N)),((expect=='P')||(expect=='N')))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int32, int8 >(N)),((expect=='P')||(expect=='N')))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int16, int8 >(N)),((expect=='P')||(expect=='N')))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int8 , int8 >(N)),false)

#define TESTCONVBLOCK16(N,expect) \
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint64, int16>(N)),(expect=='P'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint32, int16>(N)),(expect=='P'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint16, int16>(N)),(expect=='P'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint8 , int16>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int64, int16>(N)),((expect=='P')||(expect=='N')))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int32, int16>(N)),((expect=='P')||(expect=='N')))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int16, int16>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int8 , int16>(N)),false)

#define TESTCONVBLOCK32(N,expect) \
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint64, int32>(N)),(expect=='P'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint32, int32>(N)),(expect=='P'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint16, int32>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint8 , int32>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int64, int32>(N)),((expect=='P')||(expect=='N')))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int32, int32>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int16, int32>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int8 , int32>(N)),false)\

#define TESTCONVBLOCK64(N,expect) \
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint64, int64>(N)),(expect=='P'))\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint32, int64>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint16, int64>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion<uint8 , int64>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int64, int64>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int32, int64>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int16, int64>(N)),false)\
		ASSERT_BOOL((SaturatedIntegerTest::TestConversion< int8 , int64>(N)),false)\

/**
 * @brief Performs a full test of the SaturatedInteger type.
 * @param[in] testSet is a set of 1-64 bits. Each bit correspond to a subset to be executed
 * @return True if the selected tests are successful. False otherwise.
 */
bool Test(uint64 testSelection,bool show){
	bool ret = true;

	TESTCONVBLOCK8  (-129,'N');
	TESTCONVBLOCK8U (-129,'N');
	TESTCONVBLOCK16 (-129,' ');
	TESTCONVBLOCK16U(-129,'N');
	TESTCONVBLOCK32 (-129,' ');
	TESTCONVBLOCK32U(-129,'N');
	TESTCONVBLOCK64 (-129,' ');
	TESTCONVBLOCK64U(-129,'N');

	TESTCONVBLOCK8  (-1,' ');
	TESTCONVBLOCK8U (-1,'N');
	TESTCONVBLOCK16 (-1,' ');
	TESTCONVBLOCK16U(-1,'N');
	TESTCONVBLOCK32 (-1,' ');
	TESTCONVBLOCK32U(-1,'N');
	TESTCONVBLOCK64 (-1,' ');
	TESTCONVBLOCK64U(-1,'N');

	TESTCONVBLOCK8  (120,' ');
	TESTCONVBLOCK8U (120,' ');
	TESTCONVBLOCK16 (120,' ');
	TESTCONVBLOCK16U(120,' ');
	TESTCONVBLOCK32 (120,' ');
	TESTCONVBLOCK32U(120,' ');
	TESTCONVBLOCK64 (120,' ');
	TESTCONVBLOCK64U(120,' ');

	TESTCONVBLOCK8  (240,'P');
	TESTCONVBLOCK8U (240,' ');
	TESTCONVBLOCK16 (240,' ');
	TESTCONVBLOCK16U(240,' ');
	TESTCONVBLOCK32 (240,' ');
	TESTCONVBLOCK32U(240,' ');
	TESTCONVBLOCK64 (240,' ');
	TESTCONVBLOCK64U(240,' ');

	TESTCONVBLOCK8  (32000,'P');
	TESTCONVBLOCK8U (32000,'P');
	TESTCONVBLOCK16 (32000,' ');
	TESTCONVBLOCK16U(32000,' ');
	TESTCONVBLOCK32 (32000,' ');
	TESTCONVBLOCK32U(32000,' ');
	TESTCONVBLOCK64 (32000,' ');
	TESTCONVBLOCK64U(32000,' ');

	TESTCONVBLOCK8  (64000,'P');
	TESTCONVBLOCK8U (64000,'P');
	TESTCONVBLOCK16 (64000,'P');
	TESTCONVBLOCK16U(64000,' ');
	TESTCONVBLOCK32 (64000,' ');
	TESTCONVBLOCK32U(64000,' ');
	TESTCONVBLOCK64 (64000,' ');
	TESTCONVBLOCK64U(64000,' ');

	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint64>(20000000000,10000000000)),4);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint64>(10000000000,20000000000)),3);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint64>(10000000000,9999999990)),0);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint32>(2400000000U,2000000000U)),2);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint32>(2000000001, 2000000000)),5);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint32>(65000,66000)),3);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint32>(130000,65000)),4);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint32>(65000,64000)),0);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint16>(32000,32001)),3);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint16>(32002,32000)),4);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint16>(32001,32000)),5);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint16>(6558,6558)),6);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint16>(3276,3275)),7);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint16>(3005,3000)),0);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint8> (122,120)),4);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint8> (120,121)),3);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint8> (121,120)),5);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint8> (33,30)),6);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint8> (28,25)),7);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<uint8> (24,20)),0);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int64>(20000000000, 10000000000)),4);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int64>(20000000000U,19768070922U)),5);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int64>(20000000000U,19768070925U)),0);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int32>(2000000000U,2000000000U)),2);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int32>(130000,65000)),4);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int32>(1000000001, 1000000000)),5);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int32>(220000000,220000001)),6);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int32>(200000000,200000001)),7);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int32>(65000,64000)),0);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int16>(17001,17000)),2);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int16>(16002,16000)),4);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int16>(16001,16000)),5);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int16>(3276,3277)),6);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int16>(3275,3276)),7);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int16>(2905,2900)),0);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int8> (66,66)),2);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int8> (62,60)),4);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int8> (61,60)),5);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int8> (17,15)),6);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int8> (11,12)),7);
	ASSERT_VALUE((SaturatedIntegerTest::TestMathOps<int8> (12,10)),0);


	return ret;
};

}

#endif


