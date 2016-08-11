/**
 * @file ClassMethodsRegistryItemTest.h
 * @brief Header file for class ClassMethodsRegistryItemTest
 * @date 23/05/2016
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

 * @details This header file contains the declaration of the class ClassMethodsRegistryItemTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSMETHODSREGISTRYITEMTEST_H_
#define CLASSMETHODSREGISTRYITEMTEST_H_

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
 * @brief Unit tests for the public methods of ClassMethodsRegistryItemTest.
 */
class ClassMethodsRegistryItemTest {
public:

    /**
     * @brief Default constructor.
     **/
    ClassMethodsRegistryItemTest();

    /**
     * @brief Destructor.
     **/
    virtual ~ClassMethodsRegistryItemTest();

    /**
     * @brief Tests the default constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests calling a function
     */
    bool TestCallFunction();

    /**
     * @brief Tests calling a function with macro support
     */
    bool TestCallFunction_WithMacroSupport();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSMETHODSREGISTRYITEMTEST_H_ */

