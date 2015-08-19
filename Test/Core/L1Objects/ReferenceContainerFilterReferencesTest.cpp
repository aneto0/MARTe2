/**
 * @file ReferenceContainerFilterReferencesTest.cpp
 * @brief Source file for class ReferenceContainerFilterReferencesTest
 * @date 18/ago/2015
 * @author pc
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
 * the class ReferenceContainerFilterReferencesTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainerFilterReferencesTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ReferenceContainerFilterReferencesTest::TestDefaultConstructor() {

    //checks if the default values are set in the father class.
    ReferenceContainerFilterReferences myRefFilter;

    //checks if the mode is zero 8all flags false and if the number of occurrences is zero.
    return (myRefFilter.GetMode() == 0) && (myRefFilter.IsFinished());

}

bool ReferenceContainerFilterReferencesTest::TestFullConstructor() {

    return true;
}

bool ReferenceContainerFilterReferencesTest::TestTest() {
    return true;
}
