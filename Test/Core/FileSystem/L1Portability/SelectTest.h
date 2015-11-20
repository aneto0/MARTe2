/**
 * @file SelectTest.h
 * @brief Header file for class SelectTest
 * @date 19/11/2015
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

 * @details This header file contains the declaration of the class SelectTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SELECTTEST_H_
#define SELECTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Select.h"
#include "BasicConsole.h"
#include "BasicFile.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
class SelectTest {
public:
    /**
     * @Brief Default constructor
     */
    SelectTest();
    virtual ~SelectTest() {

    }

    /**
     * @brief Test the constructor.
     * @details The preconditions of the constructor are difficult to test due to the attributes
     * are private. The test call Select::IsSet(bf).
     * @return True if Select::IsSet(bf) returns false.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Test AddReadHandle
     * @return True if the Select::AddReadHandle(bf) returns true.
     */
    bool TestAddReadHandle();

    /**
     * @brief Test AddReadHandle
     * @brief add the same handle twice.
     * @return True if the Select::AddReadHandle(bf) returns true the first time and false the second attempt.
     */
    bool TestAddReadHandle_SameHandle();

    /**
     * @brief Test AddReadHandle
     * @brief Add an invalid handle.
     * @return True if the Select::AddReadHandle(bf) returns false.
     */
    bool TestAddReadHandle_InvalidHandle();

    /**
     * @brief TestAddReadHandle
     * @return True if the Select::AddWriteHandle(bc) returns true.
     */
    bool TestAddWriteHandle();

    /**
     * @brief Test AddWriteHandle
     * @brief Add the same handle twice.
     * @return True if the Select::AddWriteHandle(bc) returns true the first time and false the second attempt.
     */
    bool TestAddWritedHandle_SameHandle();

    /**
     * @brief TestAddWriteHandle
     * @brief Add an invalid handle.
     * @return True if the Select::AddWriteHandle(bf) returns false.
     */
    bool TestAddWritedHandle_InvalidHandle();

    /**
     * @brief Test AddExceptionHandle using a valid handle
     * @return True if Select::AddExceptionHandle return true.
     */
    bool TestAddExceptionHandle();

    /**
     * TestAddExceptionHandle adding the same handle twice.
     * @return True if the first add succeeds and the second fails.
     */
    bool TestAddExceptionHandle_SameHandle();

    /**
     * Test AddExceptionHandle using an invalid handle.
     * @return True if the add fails.
     */
    bool TestAddExceptionHandle_Invaliddle();

    /**
     * @brief Test RemoveReadHandle in normal conditions.
     * @details First add an element then try to remove it and check if is really removed.
     * @return True if Select::RemoveReadHandle(bc) returns true and Select::IsSet(bc) returns false.
     */
    bool TestRemoveReadHandle();

    /**
     * @brief Test RemoveReadHandle using an invalid handle.
     * @return True if Select::RemoveReadHandle(bc) returns true and Select::IsSet(bc) returns false.
     */
    bool TestRemoveReadHandle_InvalidHandle();

private:
    Select sel;
    BasicConsole bc;
    BasicFile bf;
    bool retVal;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /*SELECTTEST_H_ */

