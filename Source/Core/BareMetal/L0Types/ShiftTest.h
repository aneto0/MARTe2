/**
 * @file ShiftTest.h
 * @brief Header file for class ShiftTest
 * @date 19/ott/2015
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

class ShiftTest {

public:

    /** @brief Tests the logical right shift.
     * @return true if the shifts give the expected results. */
    bool TestLogicalRightShift();

    /** @brief Tests the logical left shift.
     * @return true if the shifts give the expected results. */
    bool TestLogicalLeftShift();

    /** @brief Tests the mathematic right shift.
     * @return true if the shifts give the expected results.*/
    bool TestMathematicRightShift();

    /**
     * @brief Tests the mathematic left shift.
     * @return true if the shifts give the expected results. */
    bool TestMathematicLeftShift();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SHIFTTEST_H_ */

