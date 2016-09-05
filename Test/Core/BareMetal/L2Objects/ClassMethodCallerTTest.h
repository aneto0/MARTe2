/**
 * @file ClassMethodCallerTTest.h
 * @brief Header file for class ClassMethodCallerTTest
 * @date 14/07/2016
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

 * @details This header file contains the declaration of the class ClassMethodCallerTTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSMETHODCALLERTTEST_H_
#define CLASSMETHODCALLERTTEST_H_

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
 * @brief Unit tests for the public methods of ClassMethodCallerT.
 */
class ClassMethodCallerTTest {
public:

    /**
     * @brief Default constructor.
     **/
    ClassMethodCallerTTest();

    /**
     * @brief Destructor.
     **/
    virtual ~ClassMethodCallerTTest();

    /**
     * @brief Tests the default constructor
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the Call method with no parameters
     */
    bool TestCall_VoidParameters();

    /**
     * @brief Tests the Call method with one read-only parameter
     */
    bool TestCall_OneBasicParameterReadOnly();

    /**
     * @brief Tests the Call method with one read/write parameter
     */
    bool TestCall_OneBasicParameterReadWrite();

    /**
     * @brief Tests the Call method with one StructuredDataI read-only parameter
     */
    bool TestCall_OneStructuredDataIParameterReadOnly();

    /**
     * @brief Tests the Call method with one StructuredDataI read/write parameter
     */
    bool TestCall_OneStructuredDataIParameterReadWrite();

    /**
     * @brief Tests the Call method with one read-only ReferenceContainer parameter
     */
    bool TestCall_OneReferenceContainerParameterReadOnly();

    /**
     * @brief Tests the Call method with one read/write ReferenceContainer parameters
     */
    bool TestCall_OneReferenceContainerParameterReadWrite();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSMETHODCALLERTTEST_H_ */

