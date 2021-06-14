/**
 * @file ClassMethodCallerGTest.cpp
 * @brief Source file for class ClassMethodCallerGTest
 * @date 27/06/2016
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

 * @details This source file contains the definition of all the methods for
 * the class ClassMethodCallerGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassMethodCallerTTest.h"
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestDefaultConstructor) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_VoidParameters) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_VoidParameters());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_OneBasicParameterReadOnly) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneBasicParameterReadOnly());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_OneBasicParameterReadWrite) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneBasicParameterReadWrite());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_OneStructuredDataIParameterReadOnly) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneStructuredDataIParameterReadOnly());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_OneStructuredDataIParameterReadWrite) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneStructuredDataIParameterReadWrite());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_OneReferenceContainerParameterReadOnly) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneReferenceContainerParameterReadOnly());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_OneReferenceContainerParameterReadWrite) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneReferenceContainerParameterReadWrite());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_OneStreamIParameterReadOnly) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneStreamIParameterReadOnly());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_OneStreamIParameterReadWrite) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneStreamIParameterReadWrite());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_C_C_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_C_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_C_C_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_C_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_C_C_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_C_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_C_R_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_C_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_C_R_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_C_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_C_R_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_C_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_C_W_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_C_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_C_W_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_C_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_C_W_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_C_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_R_C_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_R_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_R_C_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_R_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_R_C_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_R_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_R_R_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_R_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_R_R_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_R_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_R_R_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_R_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_R_W_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_R_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_R_W_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_R_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_R_W_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_R_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_W_C_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_W_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_W_C_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_W_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_W_C_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_W_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_W_R_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_W_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_W_R_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_W_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_W_R_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_W_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_W_W_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_W_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_W_W_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_W_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_C_W_W_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_C_W_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_C_C_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_C_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_C_C_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_C_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_C_C_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_C_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_C_R_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_C_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_C_R_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_C_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_C_R_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_C_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_C_W_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_C_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_C_W_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_C_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_C_W_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_C_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_R_C_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_R_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_R_C_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_R_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_R_C_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_R_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_R_R_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_R_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_R_R_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_R_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_R_R_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_R_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_R_W_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_R_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_R_W_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_R_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_R_W_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_R_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_W_C_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_W_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_W_C_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_W_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_W_C_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_W_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_W_R_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_W_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_W_R_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_W_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_W_R_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_W_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_W_W_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_W_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_W_W_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_W_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_R_W_W_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_R_W_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_C_C_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_C_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_C_C_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_C_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_C_C_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_C_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_C_R_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_C_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_C_R_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_C_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_C_R_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_C_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_C_W_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_C_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_C_W_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_C_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_C_W_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_C_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_R_C_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_R_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_R_C_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_R_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_R_C_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_R_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_R_R_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_R_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_R_R_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_R_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_R_R_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_R_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_R_W_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_R_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_R_W_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_R_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_R_W_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_R_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_W_C_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_W_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_W_C_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_W_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_W_C_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_W_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_W_R_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_W_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_W_R_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_W_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_W_R_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_W_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_W_W_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_W_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_W_W_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_W_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_W_W_W_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_W_W_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_ErrorParameter1) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_ErrorParameter1());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_ErrorParameter2) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_ErrorParameter2());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_ErrorParameter3) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_ErrorParameter3());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_ErrorParameter4) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_ErrorParameter4());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_ErrorParameterReturn) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_ErrorParameterReturn());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_FourParameters_ReferenceContainer) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_FourParameters_ReferenceContainer());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_C_C_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_C_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_C_C_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_C_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_C_C_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_C_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_C_R_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_C_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_C_R_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_C_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_C_R_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_C_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_C_W_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_C_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_C_W_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_C_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_C_W_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_C_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_R_C_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_R_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_R_C_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_R_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_R_C_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_R_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_R_R_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_R_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_R_R_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_R_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_R_R_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_R_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_R_W_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_R_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_R_W_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_R_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_R_W_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_R_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_W_C_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_W_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_W_C_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_W_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_W_C_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_W_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_W_R_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_W_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_W_R_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_W_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_W_R_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_W_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_W_W_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_W_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_W_W_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_W_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_W_W_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_W_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_ErrorParameter1) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_ErrorParameter1());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_ErrorParameter2) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_ErrorParameter2());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_ErrorParameter3) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_ErrorParameter3());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_ErrorParameterReturn) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_ErrorParameterReturn());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_ThreeParameters_ReferenceContainer) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_ThreeParameters_ReferenceContainer());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_TwoParameters_C_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_TwoParameters_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_TwoParameters_C_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_TwoParameters_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_TwoParameters_C_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_TwoParameters_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_TwoParameters_R_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_TwoParameters_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_TwoParameters_R_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_TwoParameters_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_TwoParameters_R_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_TwoParameters_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_TwoParameters_W_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_TwoParameters_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_TwoParameters_W_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_TwoParameters_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_TwoParameters_W_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_TwoParameters_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_TwoParameters_ErrorParameter1) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_TwoParameters_ErrorParameter1());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_TwoParameters_ErrorParameter2) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_TwoParameters_ErrorParameter2());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_TwoParameters_ErrorParameterReturn) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_TwoParameters_ErrorParameterReturn());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_TwoParameters_ReferenceContainer) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_TwoParameters_ReferenceContainer());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_OneParameter_C) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneParameter_C());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_OneParameter_R) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneParameter_R());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_OneParameter_W) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneParameter_W());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_OneParameter_ErrorParameter1) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneParameter_ErrorParameter1());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_OneParameter_ErrorParameterReturn) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneParameter_ErrorParameterReturn());
}

TEST(BareMetal_L2Objects_ClassMethodCallerTGTest,TestCall_OneParameter_ReferenceContainer) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneParameter_ReferenceContainer());
}
