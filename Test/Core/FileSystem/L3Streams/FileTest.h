/**
 * @file FileTest.h
 * @brief Header file for class FileTest
 * @date 16/11/2016
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

 * @details This header file contains the declaration of the class FileTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_FILESYSTEM_L3STREAMS_FILETEST_H_
#define TEST_CORE_FILESYSTEM_L3STREAMS_FILETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "File.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the File public methods.
 * @details Detailed testing was already performed for the BasicFile and for the DoubleBufferedStream.
 * The main scope here is to verify that the template is resolving to the correct methods.
 */
class FileTest {
public:

    /**
     * @brief Constructor. Deletes all the previous test files.
     */
    FileTest();

    /**
     * @brief Tests that the class can be constructed. Returns true or compilation will fail while resolving the templates.
     */
    bool TestConstructor();

    /**
     * @brief Tests that a function which receives a StreamI as an input can be called. Returns true or compilation will fail while resolving the templates.
     */
    bool TestConstructor_Stream();

    /**
     * @brief Tests that a function which receives a BasicFile as an input can be called. Returns true or compilation will fail while resolving the templates.
     */
    bool TestConstructor_BasicFile();

    /**
     * @brief Tests the Read method.
     */
    bool TestRead();

    /**
     * @brief Tests the Read method with a timeout.
     */
    bool TestRead_Timeout();

    /**
     * @brief Tests the Write method.
     */
    bool TestWrite();

    /**
     * @brief Tests the Write method with a timeout.
     */
    bool TestWrite_Timeout();

    /**
     * @brief Tests the Size method.
     */
    bool TestSize();

    /**
     * @brief Tests the Seek method.
     */
    bool TestSeek();

    /**
     * @brief Tests the RelativeSeek method.
     */
    bool TestRelativeSeek();

    /**
     * @brief Tests the Position method.
     */
    bool TestPosition();

    /**
     * @brief Tests the SetSize method.
     */
    bool TestSetSize();

    /**
     * @brief Tests the CanWrite method.
     */
    bool TestCanWrite();

    /**
     * @brief Tests the CanSeek method.
     */
    bool TestCanSeek();

    /**
     * @brief Tests the CanRead method.
     */
    bool TestCanRead();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_FILESYSTEM_L3STREAMS_FILETEST_H_ */

