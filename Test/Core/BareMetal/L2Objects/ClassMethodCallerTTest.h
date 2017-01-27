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

    /**
     * @brief Tests the Call method with one StreamI read-only parameter
     */
    bool TestCall_OneStreamIParameterReadOnly();

    /**
     * @brief Tests the Call method with one StreamI read/write parameter
     */
    bool TestCall_OneStreamIParameterReadWrite();

    /**
     * @brief Tests the Call method with four parameters on all the possible combinations
     */
    bool TestCall_FourParameters_C_C_C_C();
    bool TestCall_FourParameters_C_C_C_R();
    bool TestCall_FourParameters_C_C_C_W();
    bool TestCall_FourParameters_C_C_R_C();
    bool TestCall_FourParameters_C_C_R_R();
    bool TestCall_FourParameters_C_C_R_W();
    bool TestCall_FourParameters_C_C_W_C();
    bool TestCall_FourParameters_C_C_W_R();
    bool TestCall_FourParameters_C_C_W_W();
    bool TestCall_FourParameters_C_R_C_C();
    bool TestCall_FourParameters_C_R_C_R();
    bool TestCall_FourParameters_C_R_C_W();
    bool TestCall_FourParameters_C_R_R_C();
    bool TestCall_FourParameters_C_R_R_R();
    bool TestCall_FourParameters_C_R_R_W();
    bool TestCall_FourParameters_C_R_W_C();
    bool TestCall_FourParameters_C_R_W_R();
    bool TestCall_FourParameters_C_R_W_W();
    bool TestCall_FourParameters_C_W_C_C();
    bool TestCall_FourParameters_C_W_C_R();
    bool TestCall_FourParameters_C_W_C_W();
    bool TestCall_FourParameters_C_W_R_C();
    bool TestCall_FourParameters_C_W_R_R();
    bool TestCall_FourParameters_C_W_R_W();
    bool TestCall_FourParameters_C_W_W_C();
    bool TestCall_FourParameters_C_W_W_R();
    bool TestCall_FourParameters_C_W_W_W();
    bool TestCall_FourParameters_R_C_C_C();
    bool TestCall_FourParameters_R_C_C_R();
    bool TestCall_FourParameters_R_C_C_W();
    bool TestCall_FourParameters_R_C_R_C();
    bool TestCall_FourParameters_R_C_R_R();
    bool TestCall_FourParameters_R_C_R_W();
    bool TestCall_FourParameters_R_C_W_C();
    bool TestCall_FourParameters_R_C_W_R();
    bool TestCall_FourParameters_R_C_W_W();
    bool TestCall_FourParameters_R_R_C_C();
    bool TestCall_FourParameters_R_R_C_R();
    bool TestCall_FourParameters_R_R_C_W();
    bool TestCall_FourParameters_R_R_R_C();
    bool TestCall_FourParameters_R_R_R_R();
    bool TestCall_FourParameters_R_R_R_W();
    bool TestCall_FourParameters_R_R_W_C();
    bool TestCall_FourParameters_R_R_W_R();
    bool TestCall_FourParameters_R_R_W_W();
    bool TestCall_FourParameters_R_W_C_C();
    bool TestCall_FourParameters_R_W_C_R();
    bool TestCall_FourParameters_R_W_C_W();
    bool TestCall_FourParameters_R_W_R_C();
    bool TestCall_FourParameters_R_W_R_R();
    bool TestCall_FourParameters_R_W_R_W();
    bool TestCall_FourParameters_R_W_W_C();
    bool TestCall_FourParameters_R_W_W_R();
    bool TestCall_FourParameters_R_W_W_W();
    bool TestCall_FourParameters_W_C_C_C();
    bool TestCall_FourParameters_W_C_C_R();
    bool TestCall_FourParameters_W_C_C_W();
    bool TestCall_FourParameters_W_C_R_C();
    bool TestCall_FourParameters_W_C_R_R();
    bool TestCall_FourParameters_W_C_R_W();
    bool TestCall_FourParameters_W_C_W_C();
    bool TestCall_FourParameters_W_C_W_R();
    bool TestCall_FourParameters_W_C_W_W();
    bool TestCall_FourParameters_W_R_C_C();
    bool TestCall_FourParameters_W_R_C_R();
    bool TestCall_FourParameters_W_R_C_W();
    bool TestCall_FourParameters_W_R_R_C();
    bool TestCall_FourParameters_W_R_R_R();
    bool TestCall_FourParameters_W_R_R_W();
    bool TestCall_FourParameters_W_R_W_C();
    bool TestCall_FourParameters_W_R_W_R();
    bool TestCall_FourParameters_W_R_W_W();
    bool TestCall_FourParameters_W_W_C_C();
    bool TestCall_FourParameters_W_W_C_R();
    bool TestCall_FourParameters_W_W_C_W();
    bool TestCall_FourParameters_W_W_R_C();
    bool TestCall_FourParameters_W_W_R_R();
    bool TestCall_FourParameters_W_W_R_W();
    bool TestCall_FourParameters_W_W_W_C();
    bool TestCall_FourParameters_W_W_W_R();
    bool TestCall_FourParameters_W_W_W_W();

    /**
     * @brief Tests the Call method with four parameters with an error on the first parameter.
     */
    bool TestCall_FourParameters_ErrorParameter1();

    /**
     * @brief Tests the Call method with four parameters with an error on the second parameter.
     */
    bool TestCall_FourParameters_ErrorParameter2();

    /**
     * @brief Tests the Call method with four parameters with an error on the third parameter.
     */
    bool TestCall_FourParameters_ErrorParameter3();

    /**
     * @brief Tests the Call method with four parameters with an error on the fourth parameter.
     */
    bool TestCall_FourParameters_ErrorParameter4();

    /**
     * @brief Tests the Call method with four parameters with a return error.
     */
    bool TestCall_FourParameters_ErrorParameterReturn();

    /**
     * @brief Tests the Call method with four parameters through a ReferenceContainer.
     */
    bool TestCall_FourParameters_ReferenceContainer();

    /**
     * @brief Tests the Call method with three parameters on all the possible combinations
     */
    bool TestCall_ThreeParameters_C_C_C();
    bool TestCall_ThreeParameters_C_C_R();
    bool TestCall_ThreeParameters_C_C_W();
    bool TestCall_ThreeParameters_C_R_C();
    bool TestCall_ThreeParameters_C_R_R();
    bool TestCall_ThreeParameters_C_R_W();
    bool TestCall_ThreeParameters_C_W_C();
    bool TestCall_ThreeParameters_C_W_R();
    bool TestCall_ThreeParameters_C_W_W();
    bool TestCall_ThreeParameters_R_C_C();
    bool TestCall_ThreeParameters_R_C_R();
    bool TestCall_ThreeParameters_R_C_W();
    bool TestCall_ThreeParameters_R_R_C();
    bool TestCall_ThreeParameters_R_R_R();
    bool TestCall_ThreeParameters_R_R_W();
    bool TestCall_ThreeParameters_R_W_C();
    bool TestCall_ThreeParameters_R_W_R();
    bool TestCall_ThreeParameters_R_W_W();
    bool TestCall_ThreeParameters_W_C_C();
    bool TestCall_ThreeParameters_W_C_R();
    bool TestCall_ThreeParameters_W_C_W();
    bool TestCall_ThreeParameters_W_R_C();
    bool TestCall_ThreeParameters_W_R_R();
    bool TestCall_ThreeParameters_W_R_W();
    bool TestCall_ThreeParameters_W_W_C();
    bool TestCall_ThreeParameters_W_W_R();
    bool TestCall_ThreeParameters_W_W_W();


    /**
     * @brief Tests the Call method with three parameters with an error on the first parameter.
     */
    bool TestCall_ThreeParameters_ErrorParameter1();

    /**
     * @brief Tests the Call method with three parameters with an error on the second parameter.
     */
    bool TestCall_ThreeParameters_ErrorParameter2();

    /**
     * @brief Tests the Call method with three parameters with an error on the third parameter.
     */
    bool TestCall_ThreeParameters_ErrorParameter3();

    /**
     * @brief Tests the Call method with three parameters with a return error.
     */
    bool TestCall_ThreeParameters_ErrorParameterReturn();

    /**
     * @brief Tests the Call method with three parameters through a ReferenceContainer.
     */
    bool TestCall_ThreeParameters_ReferenceContainer();

    /**
     * @brief Tests the Call method with two parameters on all the possible combinations
     */
    bool TestCall_TwoParameters_C_C();
    bool TestCall_TwoParameters_C_R();
    bool TestCall_TwoParameters_C_W();
    bool TestCall_TwoParameters_R_C();
    bool TestCall_TwoParameters_R_R();
    bool TestCall_TwoParameters_R_W();
    bool TestCall_TwoParameters_W_C();
    bool TestCall_TwoParameters_W_R();
    bool TestCall_TwoParameters_W_W();

    /**
     * @brief Tests the Call method with two parameters with an error on the first parameter.
     */
    bool TestCall_TwoParameters_ErrorParameter1();

    /**
     * @brief Tests the Call method with two parameters with an error on the second parameter.
     */
    bool TestCall_TwoParameters_ErrorParameter2();

    /**
     * @brief Tests the Call method with two parameters with a return error.
     */
    bool TestCall_TwoParameters_ErrorParameterReturn();

    /**
     * @brief Tests the Call method with two parameters through a ReferenceContainer.
     */
    bool TestCall_TwoParameters_ReferenceContainer();

    /**
     * @brief Tests the Call method with one parameters on all the possible combinations
     */
    bool TestCall_OneParameter_C();
    bool TestCall_OneParameter_R();
    bool TestCall_OneParameter_W();

    /**
     * @brief Tests the Call method with one parameters with an error on the first parameter.
     */
    bool TestCall_OneParameter_ErrorParameter1();

    /**
     * @brief Tests the Call method with one parameters with a return error.
     */
    bool TestCall_OneParameter_ErrorParameterReturn();

    /**
     * @brief Tests the Call method with one parameters through a ReferenceContainer.
     */
    bool TestCall_OneParameter_ReferenceContainer();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSMETHODCALLERTTEST_H_ */

