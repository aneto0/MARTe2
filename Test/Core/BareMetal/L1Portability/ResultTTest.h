/**
 * @file ResultTTest.h
 * @brief Header file for class ResultTTest
 * @date 02/12/2015
 * @author Giuseppe ferrò
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

 * @details This header file contains the declaration of the class ResultTTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TOKENTEST_H_
#define TOKENTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ResultT.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the ResultTTest functions
 */
class ResultTTest {
public:
    
    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();
    
    /**
     * @brief Tests the copy constructor
     */
    bool TestCopyConstructor();
   
    /**
     * @brief Tests the destructor
     */ 
    bool TestDestructor();

    /**
     * @brief Tests the SetError function
     */
    bool TestSetError();
    
    /**
     * @brief Tests the PrintError function
     */
    bool TestPrintError();

    /**
     * @brief Tests the GetValue function
     */
    bool TestGetValue();
    
    /**
     * @brief Tests the bool operator
     */
    bool TestBoolOperator();

    /**
     * @brief Tests the assignment operator with another T instance
     */
    bool TestAssignOperator_Type();
    
    /**
     * @brief Tests the assigment operator with another Result
     */
    bool TestAssignOperator_Result();

    /**
     * @brief tests the equal operator with another t instance
     */
    bool TestEqualOperator_Type();
 
    /**
     * @brief Tests the equal operator with another Result
     */
    bool TestEqualOperator_Result();

    /**
     * @brief tests the different operator with another t instance
     */
    bool TestDifferentOperator_Type();
    
    /**
     * @brief Tests the different operator with another Result
     */
    bool TestDifferentOperator_Result();


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TOKENTEST_H_ */

