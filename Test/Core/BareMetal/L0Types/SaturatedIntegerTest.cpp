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

#include "SaturatedIntegerTest.h"
#include <stdio.h>

namespace SaturatedIntegerTest{

/**
 * @brief Copies a number of type bt1 to a Saturated Integer of type bt2 and vice versa.
 * Returns true if the number is unchanged. Fails also if the SaturatedInteger is Invalid
 */
template <typename bt1,typename bt2,typename bt3>
bool TestConversion(bt3 value,bool show){
	bool ret;
	bt1 valueS;
	// use this function to clip to within range of bt1
	SafeNumber2Number(value,valueS);
	// now test the conversion to bt2
	SaturatedInteger<bt2> saturatedValue(valueS);

	ret = saturatedValue.IsValid();
	if (show && !ret) printf("1 ");

	if (ret){
		bt1 value2;
		bt2 value3 = saturatedValue.GetData();
		ret = SafeNumber2Number(value3,value2);
		if (show && !ret) printf("2 ");

		if (ret){
			ret = (valueS == value2);
			if (show && !ret) printf("M ");
		}
	}
	return ret;
}

/**
 * @brief calculates (value1+value2)*(value1-value2) + (value1-value2)
 * Returns true if the number is unchanged. Fails also if the SaturatedInteger is Invalid
 */
template <typename bt1,typename bt2>
int TestMathOps(bt2 value1,bt2 value2){
	int retI=0;
	bool ret;

	SaturatedInteger<bt1>  value1S(value1);
	SaturatedInteger<bt1>  value2S(value2);
	SaturatedInteger<bt1>  temp(0);

	ret = value1S.IsValid() && value2S.IsValid() ;
	if (!ret) { retI = 1; }

	if (ret){
		// use operators +,-,*
		SaturatedInteger<bt1>  temp2 = (value1S + value2S);
		ret = temp2.IsValid();
		if (!ret) { retI = 2; }
		if (ret){
			SaturatedInteger<bt1>  temp3 = (value1S - value2S);
			ret = temp3.IsValid();
			if (!ret) { retI = 3; }

			if (ret){
				temp = temp2 * temp3;
				ret = temp.IsValid();
				if (!ret) { retI = 4; }

				if (ret){
					temp += value1S;
					ret = temp.IsValid();
					if (!ret) { retI = 5;}

					if (ret){
						value2S *=SaturatedInteger<bt1>(10);
						ret = value2S.IsValid();
						if (!ret) { retI = 6;}

						if (ret){
							temp -= value2S;
							ret = temp.IsValid();
							if (!ret) { retI = 7;}

						}
					}
				}
			}
		}
	}

	if (ret){
		bt1 result = (value1 + value2)*(value1 - value2) + value1 - value2*10;
		SaturatedInteger<bt1>  resultS(result);
		ret = (temp == resultS);
		if (!ret) { retI = 8;}
	}
	return retI;
}


#define CHECKEXEC(test_code,show,expect)\
		{\
			bool localRet = test_code;\
			if (show){\
				if (!expect){ \
					if (localRet){\
						printf("OK       :");\
					} else {\
						printf("**NO   :");\
					}\
				} else {\
					if (localRet){\
						printf("**OK=NO :");\
					} else {\
						printf("NO=OK  :");\
					}\
				}\
				printf ( #test_code "\n");\
			}\
			ret = ret && (localRet ^ expect);\
		}

#define CHECKEXECI(test_code,show,expect)\
		{\
			int localRet = test_code;\
			if (show){\
				printf("%i ",localRet);\
				if (expect==0){ \
					if (localRet==0){\
						printf("OK     :");\
					} else {\
						printf("**NO   :");\
					}\
				} else {\
					if (localRet==expect){\
						printf("%i=OK   :",expect);\
					} else {\
						printf("**NO!=%i:",expect);\
					}\
				}\
				printf ( #test_code "\n");\
			}\
			ret = ret && (localRet != expect);\
		}

#define TESTCONVBLOCK64U(N,expect) \
		CHECKEXEC((TestConversion<uint64,uint64>(N,show)),show,false)\
		CHECKEXEC((TestConversion<uint32,uint64>(N,show)),show,false)\
		CHECKEXEC((TestConversion<uint16,uint64>(N,show)),show,false)\
		CHECKEXEC((TestConversion<uint8 ,uint64>(N,show)),show,false)\
		CHECKEXEC((TestConversion< int64,uint64>(N,show)),show,(expect=='N'))\
		CHECKEXEC((TestConversion< int32,uint64>(N,show)),show,(expect=='N'))\
		CHECKEXEC((TestConversion< int16,uint64>(N,show)),show,(expect=='N'))\
		CHECKEXEC((TestConversion< int8 ,uint64>(N,show)),show,(expect=='N'))\

#define TESTCONVBLOCK32U(N,expect) \
		CHECKEXEC((TestConversion<uint64,uint32>(N,show)),show,(expect=='P'))\
		CHECKEXEC((TestConversion<uint32,uint32>(N,show)),show,false)\
		CHECKEXEC((TestConversion<uint16,uint32>(N,show)),show,false)\
		CHECKEXEC((TestConversion<uint8 ,uint32>(N,show)),show,false)\
		CHECKEXEC((TestConversion< int64,uint32>(N,show)),show,((expect=='P')||(expect=='N')))\
		CHECKEXEC((TestConversion< int32,uint32>(N,show)),show,(expect=='N'))\
		CHECKEXEC((TestConversion< int16,uint32>(N,show)),show,(expect=='N'))\
		CHECKEXEC((TestConversion< int8 ,uint32>(N,show)),show,(expect=='N'))\

#define TESTCONVBLOCK16U(N,expect) \
		CHECKEXEC((TestConversion<uint64,uint16>(N,show)),show,(expect=='P'))\
		CHECKEXEC((TestConversion<uint32,uint16>(N,show)),show,(expect=='P'))\
		CHECKEXEC((TestConversion<uint16,uint16>(N,show)),show,false)\
		CHECKEXEC((TestConversion<uint8 ,uint16>(N,show)),show,false)\
		CHECKEXEC((TestConversion< int64,uint16>(N,show)),show,((expect=='P')||(expect=='N')))\
		CHECKEXEC((TestConversion< int32,uint16>(N,show)),show,((expect=='P')||(expect=='N')))\
		CHECKEXEC((TestConversion< int16,uint16>(N,show)),show,(expect=='N'))\
		CHECKEXEC((TestConversion< int8 ,uint16>(N,show)),show,(expect=='N'))\

#define TESTCONVBLOCK8U(N,expect) \
		CHECKEXEC((TestConversion<uint64,uint8 >(N,show)),show,(expect=='P'))\
		CHECKEXEC((TestConversion<uint32,uint8 >(N,show)),show,(expect=='P'))\
		CHECKEXEC((TestConversion<uint16,uint8 >(N,show)),show,(expect=='P'))\
		CHECKEXEC((TestConversion<uint8 ,uint8 >(N,show)),show,false)\
		CHECKEXEC((TestConversion< int64,uint8 >(N,show)),show,((expect=='P')||(expect=='N')))\
		CHECKEXEC((TestConversion< int32,uint8 >(N,show)),show,((expect=='P')||(expect=='N')))\
		CHECKEXEC((TestConversion< int16,uint8 >(N,show)),show,((expect=='P')||(expect=='N')))\
		CHECKEXEC((TestConversion< int8 ,uint8 >(N,show)),show,(expect=='N'))\

#define TESTCONVBLOCK8(N,expect) \
		CHECKEXEC((TestConversion<uint64, int8 >(N,show)),show,(expect=='P'))\
		CHECKEXEC((TestConversion<uint32, int8 >(N,show)),show,(expect=='P'))\
		CHECKEXEC((TestConversion<uint16, int8 >(N,show)),show,(expect=='P'))\
		CHECKEXEC((TestConversion<uint8 , int8 >(N,show)),show,(expect=='P'))\
		CHECKEXEC((TestConversion< int64, int8 >(N,show)),show,((expect=='P')||(expect=='N')))\
		CHECKEXEC((TestConversion< int32, int8 >(N,show)),show,((expect=='P')||(expect=='N')))\
		CHECKEXEC((TestConversion< int16, int8 >(N,show)),show,((expect=='P')||(expect=='N')))\
		CHECKEXEC((TestConversion< int8 , int8 >(N,show)),show,false)

#define TESTCONVBLOCK16(N,expect) \
		CHECKEXEC((TestConversion<uint64, int16>(N,show)),show,(expect=='P'))\
		CHECKEXEC((TestConversion<uint32, int16>(N,show)),show,(expect=='P'))\
		CHECKEXEC((TestConversion<uint16, int16>(N,show)),show,(expect=='P'))\
		CHECKEXEC((TestConversion<uint8 , int16>(N,show)),show,false)\
		CHECKEXEC((TestConversion< int64, int16>(N,show)),show,((expect=='P')||(expect=='N')))\
		CHECKEXEC((TestConversion< int32, int16>(N,show)),show,((expect=='P')||(expect=='N')))\
		CHECKEXEC((TestConversion< int16, int16>(N,show)),show,false)\
		CHECKEXEC((TestConversion< int8 , int16>(N,show)),show,false)

#define TESTCONVBLOCK32(N,expect) \
		CHECKEXEC((TestConversion<uint64, int32>(N,show)),show,(expect=='P'))\
		CHECKEXEC((TestConversion<uint32, int32>(N,show)),show,(expect=='P'))\
		CHECKEXEC((TestConversion<uint16, int32>(N,show)),show,false)\
		CHECKEXEC((TestConversion<uint8 , int32>(N,show)),show,false)\
		CHECKEXEC((TestConversion< int64, int32>(N,show)),show,((expect=='P')||(expect=='N')))\
		CHECKEXEC((TestConversion< int32, int32>(N,show)),show,false)\
		CHECKEXEC((TestConversion< int16, int32>(N,show)),show,false)\
		CHECKEXEC((TestConversion< int8 , int32>(N,show)),show,false)\

#define TESTCONVBLOCK64(N,expect) \
		CHECKEXEC((TestConversion<uint64, int64>(N,show)),show,(expect=='P'))\
		CHECKEXEC((TestConversion<uint32, int64>(N,show)),show,false)\
		CHECKEXEC((TestConversion<uint16, int64>(N,show)),show,false)\
		CHECKEXEC((TestConversion<uint8 , int64>(N,show)),show,false)\
		CHECKEXEC((TestConversion< int64, int64>(N,show)),show,false)\
		CHECKEXEC((TestConversion< int32, int64>(N,show)),show,false)\
		CHECKEXEC((TestConversion< int16, int64>(N,show)),show,false)\
		CHECKEXEC((TestConversion< int8 , int64>(N,show)),show,false)\

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

	CHECKEXECI((TestMathOps<uint64>(20000000000,10000000000)),show,4);
	CHECKEXECI((TestMathOps<uint64>(10000000000,20000000000)),show,3);
	CHECKEXECI((TestMathOps<uint64>(10000000000,9999999990)),show,0);
	CHECKEXECI((TestMathOps<uint32>(2400000000U,2000000000U)),show,2);
	CHECKEXECI((TestMathOps<uint32>(2000000001, 2000000000)),show,5);
	CHECKEXECI((TestMathOps<uint32>(65000,66000)),show,3);
	CHECKEXECI((TestMathOps<uint32>(130000,65000)),show,4);
	CHECKEXECI((TestMathOps<uint32>(65000,64000)),show,0);
	CHECKEXECI((TestMathOps<uint16>(32000,32001)),show,3);
	CHECKEXECI((TestMathOps<uint16>(32002,32000)),show,4);
	CHECKEXECI((TestMathOps<uint16>(32001,32000)),show,5);
	CHECKEXECI((TestMathOps<uint16>(6558,6558)),show,6);
	CHECKEXECI((TestMathOps<uint16>(3276,3275)),show,7);
	CHECKEXECI((TestMathOps<uint16>(3005,3000)),show,0);
	CHECKEXECI((TestMathOps<uint8> (122,120)),show,4);
	CHECKEXECI((TestMathOps<uint8> (120,121)),show,3);
	CHECKEXECI((TestMathOps<uint8> (121,120)),show,5);
	CHECKEXECI((TestMathOps<uint8> (33,30)),show,6);
	CHECKEXECI((TestMathOps<uint8> (28,25)),show,7);
	CHECKEXECI((TestMathOps<uint8> (24,20)),show,0);
	CHECKEXECI((TestMathOps<int64>(20000000000, 10000000000)),show,4);
	CHECKEXECI((TestMathOps<int64>(20000000000U,19768070922U)),show,5);
	CHECKEXECI((TestMathOps<int64>(20000000000U,19768070925U)),show,0);
	CHECKEXECI((TestMathOps<int32>(2000000000U,2000000000U)),show,2);
	CHECKEXECI((TestMathOps<int32>(130000,65000)),show,4);
	CHECKEXECI((TestMathOps<int32>(1000000001, 1000000000)),show,5);
	CHECKEXECI((TestMathOps<int32>(220000000,220000001)),show,6);
	CHECKEXECI((TestMathOps<int32>(200000000,200000001)),show,7);
	CHECKEXECI((TestMathOps<int32>(65000,64000)),show,0);
	CHECKEXECI((TestMathOps<int16>(17001,17000)),show,2);
	CHECKEXECI((TestMathOps<int16>(16002,16000)),show,4);
	CHECKEXECI((TestMathOps<int16>(16001,16000)),show,5);
	CHECKEXECI((TestMathOps<int16>(3276,3277)),show,6);
	CHECKEXECI((TestMathOps<int16>(3275,3276)),show,7);
	CHECKEXECI((TestMathOps<int16>(2905,2900)),show,0);
	CHECKEXECI((TestMathOps<int8> (66,66)),show,2);
	CHECKEXECI((TestMathOps<int8> (62,60)),show,4);
	CHECKEXECI((TestMathOps<int8> (61,60)),show,5);
	CHECKEXECI((TestMathOps<int8> (17,15)),show,6);
	CHECKEXECI((TestMathOps<int8> (11,12)),show,7);
	CHECKEXECI((TestMathOps<int8> (12,10)),show,0);


	return ret;
};

}



