/**
 * @file BasicUARTTest.h
 * @brief Header file for class BasicUARTTest.
 * @date 17/08/2021
 * @author Andre Neto
 * @author Pedro Lourenco
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
 *
 * @details This header file contains the declaration of the class BasicUARTTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICUARTTEST_H_
#define BASICUARTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicUART.h"
#include "EventSem.h"
#include "StreamString.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Tests the BasicUART public methods.
 * @details If target serial port device (i.e. /dev/ttyUSB0) TX-RX pins are not
 * shorted, use the folling command 'socat' and leave it runing on a separate
 * terminal. The output should resemble: \n
 * [user@server home]# socat -d -d
 * pty,link=/dev/ttyLoopWr,raw,echo=0,group-late=$USER,mode=760
 * pty,link=/dev/ttyLoopRd,raw,echo=0,group-late=$USER,mode=760 \n
 * 2021/08/18 15:28:50 socat[21538] N PTY is /dev/pts/4 \n
 * 2021/08/18 15:28:50 socat[21538] N PTY is /dev/pts/5 \n
 * 2021/08/18 15:28:50 socat[21538] N starting data transfer loop with FDs
 * [5,5] and [7,7] \n
 * and use the '/dev/ttyLoopWr' and '/dev/ttyLoopRd' devices in the defines at
 * the top of 'BasicUARTTest.cpp'. Notice that (i) this command shall be
 * executed as 'sudo' (or 'root') and (ii) $USER is expected to return the name
 * of the user running the tests (i.e. accessing the devices). Otherwise,
 * change the 'group-late' and 'mode' arguments accordingly.
 */
class BasicUARTTest {
public:

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the SetSpeed method.
     */
    bool TestSetSpeed();

    /**
     * @brief Tests that the SetSpeed method fails if a wrong speed is set.
     */
    bool TestSetSpeed_False();

    /**
     * @brief Tests that the SetSpeed method fails if called after the serial
     * was opened.
     */
    bool TestSetSpeed_False_Open();

    /**
     * @brief Tests the Open method.
     */
    bool TestOpen();

    /**
     * @brief Tests that the Open method fails with a wrong fd name.
     */
    bool TestOpen_WrongName();

    /**
     * @brief Tests the Close method.
     */
    bool TestClose();

    /**
     * @brief Tests the Read method.
     */
    bool TestRead();

    /**
     * @brief Tests the Read with timeout method.
     */
    bool TestReadTimeout();

    /**
     * @brief Tests the WaitRead method.
     */
    bool TestWaitRead();

    /**
     * @brief Tests the Write method.
     */
    bool TestWrite();

    /**
     * @brief Tests the WaitWrite method.
     */
    bool TestWaitWrite();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICUARTTEST_H_ */
