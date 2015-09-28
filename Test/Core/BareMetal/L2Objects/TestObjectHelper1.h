/**
 * @file TestObject1.h
 * @brief Header file for class TestObject1
 * @date 06/08/2015
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class TestObject1
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_L1OBJECTS_TESTOBJECTHELPER1_H_
#define TEST_CORE_L1OBJECTS_TESTOBJECTHELPER1_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Helper class to support the testing of references.
 */
class TestObjectHelper1 : public Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * Default constructor
     */
    TestObjectHelper1(){
        dummyVariable = 0;
    }

    virtual ~TestObjectHelper1(){
    }

    /**
     * @return the value of the dummy variable.
     */
    uint32 GetDummyVariable() const;

    /**
     * @brief Updates the value of the dummy variable.
     * @param dummyVariable the new value to set.
     */
    void SetDummyVariable(uint32 dummyVariable);

private:
    /**
     * Dummy variable
     */
    uint32 dummyVariable;
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_L1OBJECTS_TESTOBJECTHELPER1_H_ */
	
