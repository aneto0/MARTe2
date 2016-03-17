/**
 * @file RealTimeSampledDataDefTest.h
 * @brief Header file for class RealTimeSampledDataDefTest
 * @date 04/02/2016
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

 * @details This header file contains the declaration of the class RealTimeSampledDataDefTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMESAMPLEDDATADEFTEST_H_
#define L5GAMS_REALTIMESAMPLEDDATADEFTEST_H_

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

class RealTimeSampledDataDefTest {

public:
    bool TestConstructor();

    bool TestInitialise(StructuredDataI & data);

    bool TestMergeWithLocal_AddSamples();

    bool TestMergeWithLocal_AddSamplesPerCycle();

    bool TestMergeWithLocal_AddPath();

    bool TestMergeWithLocal_AddType();

    bool TestMergeWithLocalFalse_FinalGlobal();

    bool TestMergeWithLocal_DifferentSamples();

    bool TestMergeWithLocal_DifferentSamplesPerCycle();

    bool TestMergeWithLocal_DifferentType();

    bool TestMergeWithLocal_DifferentPath();

    bool TestVerify();

    bool TestVerifyFalse_UnregisteredType();

    bool TestVerifyFalse_UnintrospectableType();

    bool TestVerifyFalse_EmptyType();

    bool TestGetSamples();

    bool TestGetSamplesPerCycle();

    bool TestToStructuredData();


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMESAMPLEDDATADEFTEST_H_ */

