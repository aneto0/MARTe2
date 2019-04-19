/**
 * @file HttpDirectoryResourceTest.h
 * @brief Header file for class HttpDirectoryResourceTest
 * @date 18/03/2019
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

 * @details This header file contains the declaration of the class HttpDirectoryResourceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPDIRECTORYRESOURCETEST_H_
#define TEST_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPDIRECTORYRESOURCETEST_H_

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
 * @brief Tests the HttpDirectoryResource public methods.
 */
class HttpDirectoryResourceTest {
public:
    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests that the Initialise method fails if no BaseDir is specified.
     */
    bool TestInitialise_False_No_BaseDir();

    /**
     * @brief Tests the GetAsStructuredData with a directory.
     */
    bool TestGetAsStructuredData_Directory();

    /**
     * @brief Tests the GetAsStructuredData with a file.
     */
    bool TestGetAsStructuredData_File();

    /**
     * @brief Tests the GetAsStructuredData to check the file existence.
     */
    bool TestGetAsText_File_Head();

    /**
     * @brief Tests the GetAsText.
     */
    bool TestGetAsText();

    /**
     * @brief Tests the GetAsText with all the mime-types.
     */
    bool TestGetAsText_MimeTypes();

    /**
     * @brief Tests the SetReplyNotFound
     */
    bool TestSetReplyNotFound();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPDIRECTORYRESOURCETEST_H_ */
