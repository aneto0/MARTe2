/**
 * @file BasicRealTimeDataSourceOutputWriterTest.h
 * @brief Header file for class BasicRealTimeDataSourceOutputWriterTest
 * @date 22/03/2016
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

 * @details This header file contains the declaration of the class BasicRealTimeDataSourceOutputWriterTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICREALTIMEDATASOURCEOUTPUTWRITERTEST_H_
#define BASICREALTIMEDATASOURCEOUTPUTWRITERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BasicRealTimeDataSourceOutputWriter.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the RealTimeDataSourceOutputWriter functions.
 */
class BasicRealTimeDataSourceOutputWriterTest {
public:

    /**
     * @brief Creates a configuration database used in tests.
     */
    BasicRealTimeDataSourceOutputWriterTest();

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the write to the RealTimeDataSource from a memory allocated internal memory.
     */
    bool TestWrite_Allocation();

    /**
     * @brief Tests the write to the RealTimeDataSource from a preallocated memory.
     */
    bool TestWrite_Static();

    /**
     * @brief Tests the function writing more variables at the same time.
     */
    bool TestWrite_MoreThanOneVariable();

    /**
     * @brief Tests the function with a vector variable.
     */
    bool TestWrite_MultiDim_Vector();

    /**
     * @brief Tests the function with a matrix variable.
     */
    bool TestWrite_MultiDim_Matrix();

private:

    /**
     * A configuration database used in tests.
     */
    ConfigurationDatabase cdb;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICREALTIMEDATASOURCEOUTPUTWRITERTEST_H_ */

