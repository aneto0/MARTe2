/**
 * @file ShiftTest.h
 * @brief Header file for class ShiftTest
 * @date 19/10/2015
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

 * @details This header file contains the declaration of the class ShiftTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SHIFTTEST_H_
#define SHIFTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "Shift.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class ShiftTest {

public:

    /**
     * @brief Tests the logical right shift in different scenarios and types.
     * @return true if the shifts give the expected results.
     */
    template<typename T>
    bool TestLogicalRightSaveShift(T number);

    /**
     * @brief Tests the logical left shift in different scenarios and types.
     * @return true if the shifts give the expected results.
     */
    template<typename T>
    bool TestLogicalLeftSaveShift(T number);

    /**
     * @brief Tests the mathematical right shift in different scenarios and types.
     * @return true if the shifts give the expected results.
     */
    template<typename T>
    bool TestMathematicRightSafeShift(T number);

    /**
     * @brief Tests the mathematical left shift in different scenarios and types.
     * @return true if the shifts give the expected results.
     */
    template<typename T>
    bool TestMathematicLeftSaveShift(T number);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
bool ShiftTest::TestLogicalRightSaveShift(T number) {
    // All sizes are in bits;
    uint8 sizeT = sizeof(T) * 8;
    uint8 shiftGreaterThanSize = sizeT + 10;
    uint8 shiftLessThanSize = 1;

    //shift greater than size
    if (Shift::LogicalRightSafeShift(number, shiftGreaterThanSize) != 0) {
        return false;
    }

    //shift less than size
    if (Shift::LogicalRightSafeShift(number, shiftLessThanSize) != ((number) >> shiftLessThanSize)) {
        //return false;
    }

    //shift the exact size
    if (Shift::LogicalRightSafeShift(number, sizeT) != 0) {
        return false;
    }
    return true;
}

template<typename T>
bool ShiftTest::TestLogicalLeftSaveShift(T number) {
    // All sizes are in bits;
    uint8 sizeT = sizeof(T) * 8;
    uint8 shiftGreaterThanSize = sizeT + 10;
    uint8 shiftLessThanSize = 1;

    //shift greater than size
    if (Shift::LogicalLeftSafeShift(number, shiftGreaterThanSize) != 0) {
        return false;
    }

    //shift less than size
    if (Shift::LogicalLeftSafeShift(number, shiftLessThanSize) != (number << shiftLessThanSize)) {
        return false;
    }

    //shift the exact size
    if (Shift::LogicalLeftSafeShift(number, sizeT) != 0) {
        return false;
    }

    return true;
}

template<typename T>
bool ShiftTest::TestMathematicRightSafeShift(T number) {
    // All sizes are in bits;
    uint8 sizeT = sizeof(T) * 8;
    uint8 shiftGreaterThanSize = sizeT + 10;
    uint8 shiftLessThanSize = 1;

    //shift greater than size
    if (Shift::MathematicRightSafeShift(number, shiftGreaterThanSize) != 0) {
        return false;
    }

    //shift less than size
    if (Shift::MathematicRightSafeShift(number, shiftLessThanSize) != number >> shiftLessThanSize) {
        return false;
    }

    //shift the exact size
    if (Shift::MathematicRightSafeShift(number, sizeT) != 0) {
        return false;
    }

    return true;
}

template<typename T>
bool ShiftTest::TestMathematicLeftSaveShift(T number) {
    // All sizes are in bits;
    uint8 sizeT = sizeof(T) * 8;
    uint8 shiftGreaterThanSize = sizeT + 10;
    uint8 shiftLessThanSize = 1;

    //shift greater than size
    if (Shift::MathematicLeftSafeShift(number, shiftGreaterThanSize) != 0) {
        return false;
    }

    //shift less than size
    if (Shift::MathematicLeftSafeShift(number, shiftLessThanSize) != number << shiftLessThanSize) {
        return false;
    }

    //shift the exact size
    if (Shift::MathematicLeftSafeShift(number, sizeT) != 0) {
        return false;
    }

    return true;
}

#endif /* SHIFTTEST_H_ */

