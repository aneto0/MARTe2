/**
 * @file MemoryMapDataSourceBrokerTest.h
 * @brief Header file for class MemoryMapDataSourceBrokerTest
 * @date 18/04/2016
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

 * @details This header file contains the declaration of the class MemoryMapDataSourceBrokerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPDATASOURCEBROKERTEST_H_
#define MEMORYMAPDATASOURCEBROKERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "../../../../Source/Core/BareMetal/L5GAMs/MemoryMapBroker.h"
#include "ConfigurationDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the MemoryMapDataSourceBroker functions.
 */
class MemoryMapDataSourceBrokerTest {

public:

    /**
     * @brief Creates a configuration database used in tests.
     */
    MemoryMapDataSourceBrokerTest();

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests of the function sets correctly the application in the MemoryMapDataSourceBroker
     */
    bool TestSetApplication();

    /**
     * @brief Tests if the function allocates the space of a variable on the heap.
     */
    bool TestAddSignal_Allocation();

    /**
     * brief Tests if the function saves the pointer to the static preallocated variable.
     */
    bool TestAddSignal_Static();

    /**
     * @brief Tests if the function allocates the space for a basic type variable.
     */
    bool TestAddSignal_Allocation_BasicType();

    /**
     * @brief Tests if the function saves the pointer to the static preallocated basic type variable.
     */
    bool TestAddSignal_Static_BasicType();

    /**
     * @brief Tests if the function fails if the broker does not have the reference to the application.
     */
    bool TestAddSignalFalse_NoDataSourceSet();

    /**
     * @brief Tests if the function fails if an invalid definition reference is passed in input.
     */
    bool TestAddSignalFalse_InvalidDef();

    /**
     * @brief Tests if the function fails if no introspection is found for the definition in input.
     */
    bool TestAddSignalFalse_UnintrospectableType();

    /**
     * @brief Tests if the function fails if the definition provides an invalid path to the RealTimeDataSource.
     */
    bool TestAddSignalFalse_NotInDataSource();

    /**
     * @brief Tests if the function can add vector variables.
     */
    bool TestAddSignal_MultiDimensional_Static_Vector();

    /**
     * @brief Tests if the function allocates space for vector variables.
     */
    bool TestAddSignal_MultiDimensional_Allocation_Vector();

    /**
     * @brief Tests if the function saves the pointer for preallocated matrix variables.
     */
    bool TestAddSignal_MultiDimensional_Static_Matrix();

    /**
     * @brief Tests if the function allocates space for matrix variables.
     */
    bool TestAddSignal_MultiDimensional_Allocation_Matrix();

    /**
     * @brief Tests if the function fails passing a definition representing a multi-dimensional structure.
     */
    bool TestAddSignalFalse_UnsupportedMultidimStructured();

    bool TestAddSignal_ExistentDataSource();

    /**
     * @brief Tests if the function fails passing a definition representing a type not registered.
     */
    bool TestAddSignalFalse_UnregisteredType();

    /**
     * @brief Tests if the function fails passing a definition which does not match with its introspection.
     */
    bool TestAddSignalFalse_UnfoundMember();

    /**
     * @brief Tests if the function returns a pointer to the variable in the internal allocated memory.
     */
    bool TestGetSignal_Allocation();

    /**
     * @brief Tests if the function returns a pointer equal to the one to the preallocated variable memory.
     */
    bool TestGetSignal_Static();


    /**
     * @brief Tests if the function creates the arrays of pointers for IO operations.
     */
    bool TestFinalise();

    bool TestFailureOfSampleDefinitionInGeneric();

private:

    /**
     * A configuration database used in tests.
     */
    ConfigurationDatabase cdb;

    /**
     * A configuration database used in tests.
     */
    ConfigurationDatabase cdbVector;

    /**
     * A configuration database used in tests.
     */
    ConfigurationDatabase cdbMatrix;

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPDATASOURCEBROKERTEST_H_ */

