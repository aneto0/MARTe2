/**
 * @file ObjectRegistryDatabaseTest.h
 * @brief Header file for class ObjectRegistryDatabaseTest
 * @date 18/02/2016
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class ObjectRegistryDatabaseTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OBJECTREGISTRYDATABASETEST_H_
#define OBJECTREGISTRYDATABASETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ObjectRegistryDatabase.h"
#include "ConfigurationDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Class used for tests
 */
class PID: public Object {
    /**
     * @brief Initialises the gains from cdb
     */
    virtual bool Initialise(StructuredDataI &data);
public:
    /**
     * Proportional gain
     */
    uint32 Kp;
    /**
     * Integral gain
     */
    uint32 Ki;
    /**
     * Derivative gain
     */
    uint32 Kd;CLASS_REGISTER_DECLARATION()
    ;
};

/**
 * @brief Tests all the ObjectRegistryDatabase functions
 */
class ObjectRegistryDatabaseTest {

public:

    /**
     * @brief Constructor
     */
    ObjectRegistryDatabaseTest();

    /**
     * @brief Destructor
     */
    ~ObjectRegistryDatabaseTest();

    /**
     * @brief Tests if the function does not return null
     */
    bool TestInstance();

    /**
     * @brief Tests if the function behaves in the correct way searching absolute path (without passing reference start point in input)
     */
    bool TestFind();

    /**
     * @brief Tests the function behaves correctly searching the path relatively with respect to
     * a provided node.
     */
    bool TestFind_Relative();

    /**
     * @brief Tests if the function behaves in the correct way searching absolute path (passing reference start point in input)
     */
    bool TestFind_Absolute();

    /**
     * @brief Tests if the function considers the root as the start search point when the number
     * of back steps to the previous domain is too big.
     */
    bool TestFindTooManyBackSteps();

    /**
     * @brief Tests if the function returns "ObjectRegistryDatabase"
     */
    bool TestGetClassName();

    /**
     * @brief Tests if the function empties the database.
     */
    bool TestCleanUp();

private:

    /**
     * Configuration database used in tests.
     */
    ConfigurationDatabase cdb;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OBJECTREGISTRYDATABASETEST_H_ */

