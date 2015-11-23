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
#include "TimeoutType.h"
#include "EventSem.h"

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
     * @brief Test Select::AddReadHandle
     * @return True if the Select::AddReadHandle(bf) returns true.
     */
    bool TestAddReadHandle();

    /**
     * @brief Test Select::AddReadHandle()
     * @brief add the same handle twice.
     * @return True if the Select::AddReadHandle(bf) returns true the first time and false the second attempt.
     */
    bool TestAddReadHandle_SameHandle();

    /**
     * @brief Test Select::AddReadHandle
     * @brief Add an invalid handle.
     * @return True if the Select::AddReadHandle(bf) returns false.
     */
    bool TestAddReadHandle_InvalidHandle();

    /**
     * @brief Select::TestAddReadHandle
     * @return True if the Select::AddWriteHandle(bc) returns true.
     */
    bool TestAddWriteHandle();

    /**
     * @brief Test Select::AddWriteHandle
     * @brief Add the same handle twice.
     * @return True if the Select::AddWriteHandle(bc) returns true the first time and false the second attempt.
     */
    bool TestAddWritedHandle_SameHandle();

    /**
     * @brief Select::TestAddWriteHandle
     * @brief Add an invalid handle.
     * @return True if the Select::AddWriteHandle(bf) returns false.
     */
    bool TestAddWritedHandle_InvalidHandle();

    /**
     * @brief Test Select::AddExceptionHandle using a valid handle
     * @return True if Select::AddExceptionHandle() return true.
     */
    bool TestAddExceptionHandle();

    /**
     * Test Select::AddExceptionHandle adding the same handle twice.
     * @return True if the first add succeeds and the second fails.
     */
    bool TestAddExceptionHandle_SameHandle();

    /**
     * Test Select::AddExceptionHandle using an invalid handle.
     * @return True if the add fails.
     */
    bool TestAddExceptionHandle_Invalidle();

    /**
     * @brief Test Select::RemoveReadHandle in normal conditions.
     * @details First add an element then try to remove it and check if it is really removed.
     * @return True if Select::RemoveReadHandle(bc) returns true and Select::IsSet(bc) returns false.
     */
    bool TestRemoveReadHandle();

    /**
     * @brief Test RemoveReadHandle() trying to remove the same handle twice
     * @details First add a valid handle then try to remove it twice.
     * @return True if the second remove fails.
     */
    bool TestRemoveReadHandle_SameHandle();

    /**
     * @brief Test RemoveReadHandle using an invalid handle.
     * @return True if Select::RemoveReadHandle(bc) returns false.
     */
    bool TestRemoveReadHandle_InvalidHandle();

    /**
     * @brief Test RemoveWriteHandle in normal conditions.
     * @details First add an element then try to remove it and check if it is really removed.
     * @return True if Select::RemoveWriteHandle(bc) returns true and Select::IsSet(bc) returns false.
     */
    bool TestRemoveWriteHandle();

    /**
     * @brief Test Select::RemoveWriteHandle() trying to remove the same handle twice
     * @details First add a valid handle then try to remove it twice.
     * @return True if the second remove fails.
     */
    bool TestRemoveWriteHandle_SameHandle();

    /**
     * @brief Test Select::RemoveWriteHandle using an invalid handle.
     * @return True if Select::RemoveWriteHandle(bc) returns false.
     */
    bool TestRemoveWriteHandle_InvalidHandle();

    /**
     * @brief Test Select::RemoveExceptionHandle using a valid handle.
     * @details First add an element then try to remove it and check if it is really removed.
     * @return True if Select::RemoveExceptionHandle(bc) returns true and Select::IsSet(bc) returns false.
     */
    bool TestRemoveExceptionHandle();

    /**
     * @brief Test Select::RemoveExceptionHandle() trying to remove the same handle twice
     * @details First add a valid handle then try to remove it twice.
     * @return True if the second remove fails.
     */
    bool TestRemoveExceptionHandle_SameHandle();

    /**
     * @brief Test Select::RemoveExceptionHandle using an invalid handle.
     * @return True if Select::RemoveExceptionHandle(bc) returns false.
     */
    bool TestRemoveExceptionHandle_InvalidHandle();

    /**
     * @brief Test Select::ClearAllHandle() using valid handles
     * @details add handles to read, write and exception modes, then clear all handle and check if they are removed
     * @return True if all handles are added and removed successfully.
     */
    bool TesClearAllHandle();

    /**
     * @brief Test Select::IsSet()
     * @details First check Select::IsSet() returns false, then add a handle and use Select::IsSet() again.
     * @return True if the first Select::IsSet() returns false and the second Select::IsSet() returns true.
     */
    bool TestIsSet();

    /**
     * @brief Test Select::WaitUntil()
     * @details add a read BasicConsole handle and wait until the timeout unblocks
     * @return True if the returned value is 0 (which means timeout is reached).
     */
    bool TestWaitUntil_waitTimeout();

    /**
     * @brief Test Select::WaitUntil()
     * @details add a read UPDSocked handle, lunched a thread which wait to write, Select::WaitUtil() on the socket to read and finally writes on the port.
     * @return True if the returned value of Select::WaitUntil is one and Select::IsSet() is true.
     */
    bool TestWaitUntil_waitRead();

    /**
     * @brief Test Select::WaitUntil()
     * @details add several read socket independent handles, lunched a thread which wait to write, Select::WaitUtil() on the socket to read and finally writes on the port.
     * @return True if the returned value of Select::WaitUntil is 1 and Select::IsSet() is true .
     */
    bool TestWaitUntil_severaDifferentWaitRead();

    /**
     * @brief Select::WaitUntil().
     * @details add several read socket independent handles, remove some of them, lunched a thread which wait to write, Select::WaitUtil() on the socket to read and finally writes on the port.
     * @return True if the returned value of Select::WaitUntil is 1 and Select::IsSet() is true.
     */
    bool TestWaitUntil_removeSomeWaitRead();

    /**
     * @brief Select::WaitUntil().
     * @details Select::WaitUtil() without adding any handle.
     * @return True if the returned value of Select::WaitUntil is 1 and Select::IsSet() is true.
     */
    bool TestWaitUntil_emptyList();

    const char8 *name;
    TimeoutType defaultTo;
    Select sel;
    BasicConsole bc;
    BasicFile bf;
    bool retVal;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /*SELECTTEST_H_ */

