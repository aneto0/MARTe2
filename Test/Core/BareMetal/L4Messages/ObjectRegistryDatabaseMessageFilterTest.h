/**
 * @file ObjectRegistryDatabaseMessageFilterTest.h
 * @brief Header file for class ObjectRegistryDatabaseMessageFilterTest
 * @date 19/06/2018
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

 * @details This header file contains the declaration of the class ObjectRegistryDatabaseMessageFilterTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OBJECTREGISTYDATABASEMESSAGEFILTERTEST_H_
#define OBJECTREGISTYDATABASEMESSAGEFILTERTEST_H_

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
 * Tests the ObjectRegistryDatabaseMessageI public methods.
 */
class ObjectRegistryDatabaseMessageFilterTest {
public:

    /**
     * @brief Default constructor
     */
    ObjectRegistryDatabaseMessageFilterTest();

    /**
     * @brief Destructor
     */
    virtual ~ObjectRegistryDatabaseMessageFilterTest();

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Test the ConsumeMessage with a purge message function for a given node.
     */
    bool TestConsumeMessage_Purge();

    /**
     * @brief Test the ConsumeMessage with a purge message function for all nodes.
     */
    bool TestConsumeMessage_PurgeAll();

    /**
     * @brief Test the ConsumeMessage with a load message.
     */
    bool TestConsumeMessage_Load();

    /**
     * @brief Test the ConsumeMessage with a load message for a subtree.
     */
    bool TestConsumeMessage_LoadSubTree();

    /**
     * @brief Test that the ConsumeMessage fails with an invalid node name.
     */
    bool TestConsumeMessage_False_InvalidNode();

    /**
     * @brief Test that the ConsumeMessage fails with an invalid function name.
     */
    bool TestConsumeMessage_False_InvalidFunction();

    /**
     * @brief Test that the ConsumeMessage fails with an empty configuration.
     */
    bool TestConsumeMessage_Load_False_EmptyConfiguration();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OBJECTREGISTYDATABASEMESSAGEFILTERTEST_H_ */

