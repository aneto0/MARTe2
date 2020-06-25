/**
 * @file TypeCharacteristicsTest.h
 * @brief Header file for class TypeCharacteristicsTest
 * @date 02/09/2015
 * @author Llorenç Capellà
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

 * @details This header file contains the declaration of the class TypeCharacteristicsTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_L0TYPEDEV_TYPECHARACTERISTICSTEST_H_
#define TEST_CORE_L0TYPEDEV_TYPECHARACTERISTICSTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TypeCharacteristics.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class TypeCharacteristicsTest {
public:
    /**
     * TypeCharacterisitcs constructor.
     */
    TypeCharacteristicsTest();

    /**
     * @brief Test the function IsFloat with the type int8.
     * @return True if IsSigned returns false.
     */
    bool TestIsFloatInt8();

    /**
     * @brief Test the function IsSigned with the type float32.
     * @return True if IsSigned returns true.
     */
    bool TestIsFloatFloat32();

    /**
     * @brief Test the function IsSigned with the type float64.
     * @return True if IsSigned returns true.
     */
    bool TestIsFloatFloat64();

    /**
     * @brief Test the function IsSigned with the type int8.
     * @return True if IsSigned returns true.
     */
    bool TestIsSignedInt8();

    /**
     * @brief Test the function IsSigned with the type uint32.
     * @return True if IsSigned returns false.
     */
    bool TestIsSignedUInt32();

    /**
     * @brief Test the function MaxValue with the type int8.
     * @return True if MaxValue returns 127 (max value of int8).
     */
    bool TestMaxValueInt8();

    /**
     * @brief Test the MaxValue using int8 and BitSize = 2 as an input parameters of the function.
     * @return True if MaxValue returns 1 (which is the max value assuming two bits int).
     */
    bool TestMaxValueInt8BZ2();

    /**
     * @brief Test The function MaxValue using uint32.
     * @return true if MaxValue returns 4294967295 (which is the max value of an uint32).
     */
    bool TestMaxValueUInt32();

    /**
     * @brief Test the function MaxValue using int32 and bitsize = 2.
     * @return True if the MaxValue returns 3.
     */
    bool TestMaxValueUInt32BZ2();

    /**
     * @brief Test the MinValue function using an int8.
     * @returns True if MinValue returns -128 (which is the minimum possible representation with int8).
     */
    bool TestMinValueInt8();

    /**
     * @brief Test the MinValue function using int8 and BitSize = 2
     * @return True if MinValue returns -2.
     */
    bool TestMinValueInt8BZ2();

    /**
     * @brief Test MinValue function using uint32.
     * @return true if MinValue returns 0.
     */
    bool TestMinValueUInt32();

    /**
     * @brief Test MinValue function using uint32 and BitSize = 2.
     * @return true if MinValue returns 0.
     */
    bool TestMinValueUInt32BZ2();

    /**
     * @brief Test UsableBitSize using int8.
     * @return True if UsableBitSize return 7.
     */
    bool TestUsableBitSizeInt8();

    /**
     * @brief Test UsableBitSize using an int8 and BitSize = 2.
     * @return True if UsableBitSize returns 1.
     */
    bool TestUsableBitSizeInt8BZ();

    /**
     * @brief Test UsableBitSize using an uint32.
     * @return True if UsableBitSize returns 32.
     */
    bool TestUsableBitSizeUInt32();

    /**
     * @brief Test UsableBitSize using uint32 and BitSize = 2.
     * @return True if UsableBitSize returns 2.
     */
    bool TestUsableBitSizeUInt32BZ2();

    /**
     * @brief Test UsableBitSize using int8.
     * @return True if UsableBitSize return 6.
     */
    bool TestUsableNegativeBitSizeInt8();

    /**
     * @brief Test UsableBitSize using an uint32.
     * @return True if UsableBitSize returns 31.
     */
    bool TestUsableNegativeBitSizeInt32();

    /**
     * @brief Test UsableBitSize using an uint32.
     * @return True if UsableBitSize returns 0.
     */
    bool TestUsableNegativeBitSizeUInt32();

    /**
     * @brief Test UsableBitSize using a float32.
     * @return True if UsableBitSize returns FLT_MAX_EXP.
     */
    bool TestUsableNegativeBitSizeFloat32();

    /**
     * @brief Test SafeNumber2Number.
     * @tparam inputType Type tested as input.
     * @tparam outputType Type tested as output.
     * @param[in] input is the input value.
     * @param[in] expectedReturn is the expected return value from SafeNumber2Number.
     * @param[in] expectedOutput is the expected output from SafeNumber2Number.
     * @return True if SafeNumber2Number returns expectedReturn and output is expectedOutput.
     */
    template <typename inputType, typename outputType>
    bool TestSafeNumber2Number(inputType input, bool expectedReturn, outputType expectedOutput){
        outputType output;
        retVal = (MARTe::SafeNumber2Number<inputType,outputType>(input, output) == expectedReturn);
        retVal &= (output == expectedOutput);
        return retVal;
    }

    /**
     * @brief Test SaturableInteger function using a value in the range type.
     * @return True if SaturableInteger the input value.
     */
    bool TestSaturableIntegerNoChanges();

    /**
     * @brief Test SaturableInteger function using a value larger than the maximum value.
     * @details The output value range is equal than the input value range.
     * @return True if SaturableInteger returns 7 (the maximum possible value).
     */
    bool TestSaturableIntegerLargerThan();

    /**
     * @brief Test SaturableInteger function using a value larger than the maximum value.
     * @details The output value range is smaller than the input value range.
     * @return True if SaturableInteger returns 7 (the maximum possible value).
     */
    bool TestSaturableIntegerLargerThan2();

    /**
     * @brief Test SaturableInteger function using a value smaller than the minimum value.
     * @details The output value range is equal than the input value range.
     * @return True if SaturableInteger returns -4 (the minimum possible value).
     */
    bool TestSaturableIntegerLessThan();

    /**
     * @brief Test SaturableInteger function using a value smaller than the maximum value.
     * @details The output value range is smaller than the input value range.
     * @return True if SaturableInteger returns -4 (the minimum possible value).
     */
    bool TestSaturableIntegerLessThan2();

    /**
     * @brief Test that Epsilon() returns the correct machine epsilon value.
     */
    bool TestEpsilon();
    

private:

    /**
     * Returned value. It is used in all function to return the result of the test.
     */
    bool retVal;


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_L0TYPEDEV_TYPECHARACTERISTICSTEST_H_ */

