/**
 * @file HttpObjectBrowserTest.h
 * @brief Header file for class HttpObjectBrowserTest
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

 * @details This header file contains the declaration of the class HttpObjectBrowserTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPOBJECTBROWSERTEST_H_
#define TEST_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPOBJECTBROWSERTEST_H_

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
 * @brief Tests the HttpObjectBrowser public methods.
 */
class HttpObjectBrowserTest {
public:
    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method with the ObjectRegistryDatabase as the Root.
     */
    bool TestInitialise_ObjectRegistryDatabase_Root();

    /**
     * @brief Tests the Initialise method with itself as the Root.
     */
    bool TestInitialise_Self_Root();

    /**
     * @brief Tests the Initialise method with another object as the Root.
     */
    bool TestInitialise_Other_Root();

    /**
     * @brief Tests that the Initialise method fails if no Root is specified.
     */
    bool TestInitialise_False_No_Root();

    /**
     * @brief Tests that the Initialise method fails if an invalid Root is specified and only one character is passed as the root path.
     */
    bool TestInitialise_False_Bad_Root_1();

    /**
     * @brief Tests that the Initialise method fails if an invalid Root is specified.
     */
    bool TestInitialise_False_Bad_Root_2();

    /**
     * @brief Tests the Initialise method with a valid Realm.
     */
    bool TestInitialise_Realm();

    /**
     * @brief Tests that the Initialise method fails with an invalid Realm.
     */
    bool TestInitialise_False_Realm();

    /**
     * @brief Tests the GetAsStructuredData with the target being the HttpObjectBrowser and the Root also being this HttpObjectBrowser.
     */
    bool TestGetAsStructuredData_This_Self();

    /**
     * @brief Tests the GetAsStructuredData with the target being the HttpObjectBrowser and the Root being the ObjectRegistryDatabase.
     */
    bool TestGetAsStructuredData_This_ObjectRegistryDatabase();

    /**
     * @brief Tests the GetAsStructuredData with the target being the HttpObjectBrowser and the Root being another object.
     */
    bool TestGetAsStructuredData_This_Other();

    /**
     * @brief Tests the GetAsStructuredData with the target not being an HttpObjectBrowser.
     */
    bool TestGetAsStructuredData_Other_Not_HttpObjectBrowser();

    /**
     * @brief Tests the GetAsStructuredData with a target having more information in the path than required.
     */
    bool TestGetAsStructuredData_Other_Not_HttpObjectBrowser_Long_Path();

    /**
     * @brief Tests the GetAsStructuredData with a non existent target.
     */
    bool TestGetAsStructuredData_InvalidTarget();

    /**
     * @brief Tests the GetAsText with the target being this HttpObjectBrowser.
     */
    bool TestGetAsText_This();

    /**
     * @brief Tests the GetAsText with the target being this HttpObjectBrowser but with an unknown file.
     */
    bool TestGetAsText_This_FileNotFound();

    /**
     * @brief Tests the GetAsText with the target being another HttpObjectBrowser.
     */
    bool TestGetAsText_Other();

    /**
     * @brief Tests the GetAsText with the target being another Object that is not an HttpObjectBrowser.
     */
    bool TestGetAsText_Other_Not_HttpDataExportI();

    /**
     * @brief Tests the GetAsText with an invalid object.
     */
    bool TestGetAsText_InvalidObject();

    /**
     * @brief Test the GetAsStructuredData with a realm which authorises the resource.
     */
    bool TestGetAsStructuredData_Realm();

    /**
     * @brief Test the GetAsText with a realm which authorises the resource.
     */
    bool TestGetAsText_Realm();

    /**
     * @brief Test the GetAsStructuredData with a realm which does not authorise the resource.
     */
    bool TestGetAsStructuredData_Realm_False();

    /**
     * @brief Test the GetAsText with a realm which does not authorise the resource.
     */
    bool TestGetAsText_Realm_False();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPOBJECTBROWSERTEST_H_ */
