/**
 * @file BasicRealTimeDataSourceDefTest.h
 * @brief Header file for class BasicRealTimeDataSourceDefTest
 * @date 22/mar/2016
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

 * @details This header file contains the declaration of the class BasicRealTimeDataSourceDefTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICREALTIMEDATASOURCEDEFTEST_H_
#define BASICREALTIMEDATASOURCEDEFTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#include "BasicRealTimeDataSourceDef.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the RealTimeDataSourceDef functions.
 */
class BasicRealTimeDataSourceDefTest {

public:

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();


    /**
     * @brief Tests if the function initialises correctly the object from StructuredDataI.
     */
    bool TestInitialise();

    /**
     * @brief Tests if the function adds a record with the state name in input and adds to it the
     * consumer gam in input.
     */
    bool TestAddConsumer();

    /**
     * @brief Tests if the function adds a record with the state name in input and adds to it the
     * producer gam in input.
     */
    bool TestAddProducer();

    /**
     * @brief Tests if the function returns the number of consumer gams in the state specified in input.
     */
    bool TestGetNumberOfConsumers();

    /**
     * @brief Tests if the function returns the number of producer gams in the state specified in input.
     */
    bool TestGetNumberOfProducers();

    /**
     * @brief Tests if the function returns true if there is no more than one producer gam for each state.
     */
    bool TestVerifyTrue();

    /**
     * @brief Tests if the function fails if there is more than one producer gam in at least one state.
     */
    bool TestVerifyFalse_MoreThanOneProducer();

    /**
     * @brief Tests if the function returns true if the variable is not consumed. In this case a warning will be generated.
     */
    bool TestVerifyNoConsumers();

    /**
     * @brief Tests if the function returns the variable type.
     */
    bool TestGetType();

    /**
     * @brief Tests if the function sets the variable type.
     */
    bool TestSetType();

    /**
     * @brief Tests if the function fails if a different variable type is already set.
     */
    bool TestSetType_WarningAlreadySet();

    /**
     * @brief Tests if the function allocates correctly the space for a basic type variable in the
     * RealTimeDataSource
     */
    bool TestAllocate_Basic();

    /**
     * @brief Tests if the function allocates correctly the space for a structured type variable in the
     * RealTimeDataSource
     */
    bool TestAllocate_Structured();

    /**
     * @brief Tests if the function allocates a type not introspectable.
     */
    bool TestAllocate_Unintrospectable();

    /**
     * @brief Tests if the function fails trying to allocate a type not registered.
     */
    bool TestAllocateFalse_Invalid();

    /**
     * @brief Tests if the function allocates a vector variable.
     */
    bool TestAllocateMultiDim_Vector();

    /**
     * @brief Tests if the function allocates a matrix variable.
     */
    bool TestAllocateMultiDim_Matrix();

    /**
     * @brief Tests if the function fails trying to allocate a multi-dimensional structured type.
     */
    bool TestAllocateFalse_MultiDimStructured();

    /**
     * @brief Tests if the function returns the pointer to the correct buffer.
     */
    bool TestGetDataSourcePointer();

    /**
     * @brief Tests if the function does not reset a basic variable if it is alive between consecutive states.
     */
    bool TestPrepareNextState_Basic_ContinueVar();

    /**
     * @brief Tests if the function resets a basic variable if it was unused in the previous state.
     */
    bool TestPrepareNextState_Basic_DeadVar();

    /**
     * @brief Tests if the function does not reset a structured variable if it is alive between consecutive states.
     */
    bool TestPrepareNextState_Structured_ContinueVar();

    /**
     * @brief Tests if the function resets a structured variable if it was unused in the previous state.
     */
    bool TestPrepareNextState_Structured_DeadVar();

    /**
     * @brief Tests if the function resets a multi-dimensional basic type.
     */
    bool TestPrepareNextStateMultiDimensional();

    /**
     * @brief Tests if the function sets correctly the default value.
     */
    bool TestSetDefaultValue();

    /**
     * @brief Tests if the function returns correctly the default value.
     */
    bool TestGetDefaultValue();

    /**
     * @brief Tests if the function sets correctly the variable number of elements for each dimension.
     */
    bool TestSetNumberOfElements();

    /**
     * @brief Tests if the function sets correctly the number of dimensions.
     */
    bool TestSetNumberOfDimensions();

    /**
     * @brief Tests if the function returns correctly the number of elements for each dimension.
     */
    bool TestGetNumberOfElements();

    /**
     * @brief Tests if the function returns corectly the number of dimensions.
     */
    bool TestGetNumberOfDimensions();

    /**
     * @brief Tests if the function performs correctly the conversion of the definition to StructuredDataI.
     */
    bool TestToStructuredData();

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICREALTIMEDATASOURCEDEFTEST_H_ */

