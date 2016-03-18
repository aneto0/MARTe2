/**
 * @file RealTimeDataDefITest.h
 * @brief Header file for class RealTimeDataDefITest
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

 * @details This header file contains the declaration of the class RealTimeDataDefITest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEDATADEFITEST_H_
#define REALTIMEDATADEFITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RealTimeSampledDataDef.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;


/**
 * @brief Tests all the RealTimeDataDefI functions.
 */
class RealTimeDataDefITest {

public:

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests if the function initialises correctly the definition from a StructuredDataI in input.
     */
    bool TestInitialise();

    /**
     * @brief Tests if the function returns the type name defined.
     */
    bool TestGetType();

    /**
     * @brief Tests if the function returns the defined variable path in the RealTimeDataSource.
     */
    bool TestGetPath();

    /**
     * @brief Tests if the function returns the variable number of dimensions defined.
     */
    bool TestNumberOfDimensions(const char8 * type,
                                const char8 * modifiers,
                                uint8 ret);

    /**
     * @brief Tests if the function returns the number of elements for each dimension specified.
     */
    bool TestNumberOfElements(const char8 * type,
                              const char8 * modifiers,
                              uint32 ret[3]);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEDATADEFITEST_H_ */

