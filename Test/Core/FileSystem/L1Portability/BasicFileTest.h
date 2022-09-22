/**
 * @file BasicFileTest.h
 * @brief Header file for class TestBasicFile
 * @date 04/11/2015
 * @author Llorenc Capella
 * @author Ivan Herrero
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

 * @details This header file contains the declaration of the class TestBasicFile
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICFILETEST_H_
#define BASICFILETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "BasicFile.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;
class BasicFileTest {
public:
    BasicFileTest();
    virtual ~BasicFileTest();

    /**
     * @brief Test the BasicFile constructor and the postconditions.
     * @return True if all the postconditions are true.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Test the BasicFile copy constructor with an open file and the postconditions
     * @return True if all the postconditions are true.
     */
    bool TestCopyConstructor_withOpenFile();

    /**
     * @brief Test the BasicFile copy constructor with an closed file and the postconditions
     * @return True if all the postconditions are true.
     */
    bool TestCopyConstructor_withClosedFile();

    /**
     * @brief Test the BasicFile copy assignment operator with an open file.
     * @return True if all the postconditions are true.
     */
    bool TestAssignmentOperator_withOpenFile();

    /**
     * @brief Test the BasicFile copy assignment operator with a closed file.
     * @return True if all the postconditions are true.
     */
    bool TestAssignmentOperator_withClosedFile();

    /**
     * @brief Test the BasicFile copy assignment operator with a closed file.
     * @detail Use bf to open a file, copy bf into copybf, use the bf to write on a file and then read with copybf and compare the write with the write.
     * @return True if the action done by the copy is reflected in the file pointed by the original.
     */
    bool TestAssignmentOperator_checkPointsToSameFile();

    /**
     * @brief Test SetFlags() opening a file and then change the flags.
     * @return True if the flag is changed.
     */
    bool TestSetFlags();

    /**
     * @brief Test SetFlags() without opening a file.
     * @return True if SetFlags returns false.
     */
    bool TestSetFlags_Close();

    /**
     * @brief Test GetFlags().
     * @detail Open a file with an specific flags, then use GetFlags and compare the returned value with the input flags.
     * @return true if the GetFlags return the expected flags.
     */
    bool TestGetFlags();

    /**
     * @brief Test GetFlags().
     * @detail Open a file with defaultWFlags, then use GetFlags and compare the returned value with the input flags.
     * @return true if the GetFlags return the expected flags.
     */
    bool TestGetFlags_defaultWFlags();

    /**
     * @brief Test GetFlags().
     * @detail Open a file with defaultRFlags, then use GetFlags and compare the returned value with the input flags.
     * @return true if the GetFlags return the expected flags.
     */
    bool TestGetFlags_defaultRFlags();

    /**
     * @brief Test canRead() without opening a file.
     * @return True if !canRead().
     */
    bool TestCanRead_OnClosedFile();

    /**
     * @brief Test canRead() opening a file in read mode.
     * @return True if canRead().
     */
    bool TestCanRead_OnOpenedReadbleFile();

    /**
     * @brief Test canRead() opening a file in write mode.
     * @return True if !canRead().
     */
    bool TestCanRead_OnOpenWritableFile();

    /**
     * @brief Test canWrite() without opening a file.
     * @return True if !canWrite().
     */
    bool TestCanWrite_Close();

    /**
     * @brief Test canWrite() opening a file in read mode.
     * @return True if !canWrite().
     */
    bool TestCanWrite_OpenRead();

    /**
     * @brief Test canWrite() opening a file in write mode.
     * @return True if canWrite().
     */
    bool TestCanWrite_OpenWrite();

    /**
     * @brief Test CanSeek() without opening a file.
     * @return True if !CanSeek().
     */
    bool TestCanSeek_Close();

    /**
     * @brief Test CanSeek() opening a file.
     * @return True if CanSeek().
     */
    bool TestCanSeek_Open();

    /**
     * @brief Test Open with the defaultRWFlags
     * @return true if Open() and all the postconditions are true.
     */
    bool TestOpen();

    /**
     * @brief Test Open with the FLAG_CREAT_EXCLUSIVE
     * @return true if Open() and all the postconditions are true.
     */
    bool TestOpen_FLAG_CREAT_EXCLUSIVE();

    /**
     * @brief Test Open() with the defaultRWFlags twice
     * @return true if the first Open() is true and the second is also true.
     */
    bool TestOpen_twice();

    /**
     * @brief Test Open with the flags FLAG_CREAT | FLAG_CREAT_EXCLUSIVE
     * @return true if the flag is set FLAG_CREAT
     */
    bool TestOpen_IncompatibleFlags1();

    /**
     * @brief Test Open with the flags FLAG_TRUNC | FLAG_CREAT_EXCLUSIVE
     * @return true if the flag is set FLAG_TRUNC
     */
    bool TestOpen_IncompatibleFlags2();

    /**
     * @brief Test Open with the flags FLAG_APPEND | ACCESS_MODE_R
     * @return true if the flag is set ACCESS_MODE_R
     */
    bool TestOpen_IncompatibleFlags3();

    /**
     * @brief Test Open with the flags FLAG_TRUNC | ACCESS_MODE_R
     * @return true if the flag is set ACCESS_MODE_R
     */
    bool TestOpen_IncompatibleFlags4();

    /**
     * @brief Test IsOpen().
     * @brief Use IsOpen without opening a file and then repeat the same operation opening a file.
     * @return true if the first IsOpen() is false and the second true.
     */
    bool TestIsOpen();

    /**
     * @brief Test Close() when a file is or is not open.
     * @return True if close the file when the file is open and return false when the file is already close and all the postconditions are true.
     */
    bool TestClose();

    /**
     * @brief test Write() in normal conditions
     * @return True if BasicFile::Write() returns true and the postconditions are true.
     */
    bool TestWrite();

    /**
     * @brief Test Write() without opening a file.
     * @return True if Write() fails.
     */
    bool TestWrite_close();

    /**
     * @brief test Write() using timeout.
     * @return True if BasicFile::Write() returns true and the postconditions are true.
     */
    bool TestWrite_timeout();

    /**
     * @brief test Write() using timeout without opening a file.
     * @return True if BasicFile::Write() returns false.
     */
    bool TestWrite_timeout_close();

    /**
     * brief Test Read() in normal conditions.
     * @return True if the read succeed and postconditions are true.
     */
    bool TestRead();

    /**
     * brief Test Read() in normal conditions.
     * @return True if the read succeed and postconditions are true.
     */
    bool TestRead_close();

    /**
     * @brief Test Read() with timeout.
     * @return True if the read succeed and postconditions are true.
     */
    bool TestRead_timeout();

    /**
     * @brief Test Read() with timeout without opening a file.
     * @return True if the Read() fails.
     */
    bool TestRead_timeout_close();

    /**
     * @brief Test Size() when IsOpen().
     * @return True if the Size() returns the real size of the file.
     */
    bool TestSize();

    /**
     * @brief Test Size() when !IsOpen().
     * @return True if the Size() returns 0.
     */
    bool TestSize_close();

    /**
     * @brief Test Seek() with a position smaller than the Size() of the file.
     * @return true if Position() == position.
     */
    bool TestSeek_small();

    /**
     * @brief Test Seek() with a position larger than the Size() of the file.
     * @return true if Position() == Size().
     */
    bool TestSeek_large();

    /**
     * @brief Test Seek() without opening a file.
     * @return true if Seek() == false.
     */
    bool TestSeek_Close();

    /**
     * @brief Test RelativeSeek() with a delta position smaller than the Size() of the file.
     * @return true if Position() == (delta position + old position).
     */
    bool TestRelativeSeek_small();

    /**
     * @brief Test RelativeSeek() with a delta position larger than the Size() of the file.
     * @return true if Position() == Size().
     */
    bool TestRelativeSeek_larger();

    /**
     * @brief Test RelativeSeek() with a delta position which move the pointer below 0.
     * @return true if Position() == 0.
     */
    bool TestRelativeSeek_NegativePosition();

    /**
     * @brief Test RelativeSeek() without opening the file-
     * @return true if RelativeSeek() returns false.
     */
    bool TestRelativeSeek_Close();

    /**
     * @brief Test Position() in normal conditions
     * @return True if the Position() is the expected value.
     */
    bool TestPosition();

    /**
     * @brief Test SetSize() enlarging the size.
     * @return True if Size() == expected size && Position() does not change.
     */
    bool TestSetSize_enlarging();

    /**
     * @brief Test SetSize() reducing the size.
     * @return True if Size() == expected size && Position() == Size().
     */
    bool TestSetSize_reducing();

    /**
     * @brief Test SetSize() reducing the size.
     * @return True if Size() == expected size && Position() == Size().
     */
    bool TestGetPathName();

    /**
     * @brief Test GetReadHandle() reducing the size.
     * @return True if GetReadHandle() > 0.
     */
    bool TestGetReadHandle();

    /**
     * @brief Test GetWriteHandle().
     * @return True if GetWriteHandle() > 0.
     */
    bool TestGetWriteHandle();

private:
    /* bf is used in each function to test the behaviour of the class*/
    BasicFile bf;
    /*Is used to save and return the result of the test. If retVal is false the test fails */
    bool retVal;
    /*Indicates how much characters have to be read or write. size has to be smaller than the size of the str2 otherwise some test will fail.*/
    static const uint32 size;
    /*Default flags to perform some test. This flags have to create a file if does not exist and remove the contests of a file if it exists */
    uint32 defaultRWFlags;
    /*Default flags to perform some test. This flags have to create a file if does not exist and remove the contests of a file if it exists */
    uint32 defaultWFlags;
    /*Default flags to perform some test. This flags have to create a file if does not exist and remove the contests of a file if it exists */
    uint32 defaultRFlags;
    const char8 * nameFileTarget;
    const char8 * stringTarget;
    StreamString str1;
    StreamString str2;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /*BASICFILETEST_H_ */

