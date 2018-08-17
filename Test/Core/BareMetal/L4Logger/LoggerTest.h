/**
 * @file LoggerTest.h
 * @brief Header file for class LoggerTest
 * @date 07/11/2016
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class LoggerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LOGGERTEST_H_
#define LOGGERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * Tests the Logger public methods.
 */
class LoggerTest {
public:

    /**
     * @brief Tests the default constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the GetLogEntry method
     */
    bool TestGetLogEntry();

    /**
     * @brief Tests the ReturnPage method
     */
    bool TestReturnPage();

    /**
     * @brief Tests the GetPage method
     */
    bool TestGetPage();

    /**
     * @brief Tests the AddLogEntry method
     */
    bool TestAddLogEntry();

    /**
     * @brief Tests the GetNumberOfPages method
     */
    bool TestGetNumberOfPages();

    /**
     * @brief Tests the GetNumberOfLogs method
     */
    bool TestGetNumberOfLogs();

    /**
     * @brief Tests the LoggerErrorProcessFunction with no REPORT_ERROR parameters
     */
    bool TestLoggerErrorProcessFunction();

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LOGGERTEST_H_ */
	
