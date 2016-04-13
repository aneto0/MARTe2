/**
 * @file RealTimeDataSourceContainerTest.h
 * @brief Header file for class RealTimeDataSourceContainerTest
 * @date 21/03/2016
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

 * @details This header file contains the declaration of the class RealTimeDataSourceContainerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEDATASOURCECONTAINERTEST_H_
#define REALTIMEDATASOURCECONTAINERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RealTimeDataSourceContainer.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class RealTimeDataSourceContainerTest {

public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests if the function intitialises correctly the data source from a StructuredDataI in input.
     */
    bool TestInitialise();

    /**
     * @brief Tests if the function creates the definitions without adding new nodes for final data source.
     */
    bool TestAddDataDefinition_Final();

    /**
     * @brief Tests if the function does not create a new definition if it still exists.
     */
    bool TestAddDataDefinition_AlreadyExistentLeaf();

    /**
     * @brief Tests if the function fails trying to generate a new definition node if the data source is final.
     */
    bool TestAddDataDefinitionFalse_Final();


    /**
     * @brief Tests if the function fails if the RealTimeDataDefContainer in the gam contains something different
     * from a RealTimeDataDefI
     */
    bool TestAddDataDefinitionFalse_NoDefinitionInGAM();

    /**
     * @brief Tests if the function fails if the gam contains something different from a RealTimeDataDefContainer
     */
    bool TestAddDataDefinitionFalse_NoDefinitionContainerInGAM();

    /**
     * @brief Tests if the function fails if the gam in input is invalid.
     */
    bool TestAddDataDefinitionFalse_InvalidGAM();

    /**
     * @brief Test if the function returns true if all the variables have at most one producer per state.
     */
    bool TestVerify();

    /**
     * @brief Test if the function returns false if there exists a definition with more than
     * one producer gam in at least one state where it will be used.
     */
    bool TestVerify_TwoProducers();

    /**
     * @brief Test if the function allocates correctly the memory for all the basic type variables.
     */
    bool TestAllocate();

    /**
     * @brief Tests if the function allocates correctly the memory also for some structured types.
     */
    bool TestAllocate_Structure();

    /**
     * @brief Tests if the function sets the default values for the variables if it is required by the change of state.
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the function with vector variables.
     */
    bool TestPrepareNextState_MultiDim_Vector();

    /**
     * @brief Tests the function with matrix variables.
     */
    bool TestPrepareNextState_MultiDim_Matrix();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEDATASOURCECONTAINERTEST_H_ */

