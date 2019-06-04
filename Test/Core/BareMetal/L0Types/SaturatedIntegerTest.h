/**
 * @file SaturatedIntegerTest.h
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

#ifndef TEST_CORE_BAREMETAL_L0TYPES_SATURATEDINTEGERTEST_H_
#define TEST_CORE_BAREMETAL_L0TYPES_SATURATEDINTEGERTEST_H_

#include "CompilerTypes.h"
#include "SaturatedInteger.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/



namespace SaturatedIntegerTest{

/**
 * @brief Copies a number of type bt1 to a Saturated Integer of type bt2 and vice versa.
 * Returns true if the number is unchanged. Fails also if the SaturatedInteger is Invalid
 */
template <typename bt1,typename bt2,typename bt3>
bool TestConversion(bt3 value){
	bool ret;
	bool show=true;
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


}

#endif /* TEST_CORE_BAREMETAL_L0TYPES_SATURATEDINTEGERTEST_H_ */
