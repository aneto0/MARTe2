/**
 * @file ReferenceContainerFilterReferences.cpp
 * @brief Source file for class ReferenceContainerFilterReferences
 * @date 13/08/2015
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
 *
 * @details This source file contains the definition of all the methods for
 * the class ReferenceContainerFilterReferences (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainerFilterReferences.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ReferenceContainerFilterReferences::ReferenceContainerFilterReferences() :
        ReferenceContainerFilter() {

}

ReferenceContainerFilterReferences::ReferenceContainerFilterReferences(const int32 &occurrenceNumber,
                                                                       const uint32 &modeToSet,
                                                                       Reference refToSearch) :
        ReferenceContainerFilter(occurrenceNumber, modeToSet) {
    referenceToSearch = refToSearch;
}

/*lint -e{715} previously found not referenced because this function is an interface.*/
bool ReferenceContainerFilterReferences::Test(ReferenceContainer &previouslyFound,
                                              Reference const  &referenceToTest) {
    bool found = (referenceToSearch == referenceToTest);
    if (found) {
        IncrementFound();
    }
    return found;
}

}
