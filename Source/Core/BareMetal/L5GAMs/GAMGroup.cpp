/**
 * @file GAMGroup.cpp
 * @brief Source file for class GAMGroup
 * @date 24/02/2016
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

 * @details This source file contains the definition of all the methods for
 * the class GAMGroup (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMGroup.h"
#include "ReferenceT.h"
#include "GAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static const uint32 stateNamesGranularity = 8u;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMGroup::GAMGroup() {
    supportedStates = NULL_PTR(StreamString*);
    numberOfSupportedStates = 0u;
}

GAMGroup::~GAMGroup() {
    if (supportedStates != NULL) {
        delete[] supportedStates;
    }
}

/*
 void GAMGroup::SetUp() {
 // initialise the context here
 }

 */
/*
 void GAMGroup::PrepareNextState(const RealTimeStateInfo &status) {
 // Use the two buffers in GAMContext
 // preparing the next buffer for the next state
 }
 */

StreamString *GAMGroup::GetSupportedStates() const {
    return supportedStates;
}

uint32 GAMGroup::GetNumberOfSupportedStates() const {
    return numberOfSupportedStates;
}

void GAMGroup::AddState(const char8 * stateName) {

    bool found = false;
    for (uint32 i = 0u; (i < numberOfSupportedStates) && (!found); i++) {
        found = (supportedStates[i] == stateName);
    }

    if (!found) {
        if ((numberOfSupportedStates % stateNamesGranularity) == 0u) {
            uint32 newSize = numberOfSupportedStates + stateNamesGranularity;
            StreamString *temp = new StreamString[newSize];
            if (supportedStates != NULL) {
                for (uint32 i = 0u; i < numberOfSupportedStates; i++) {
                    temp[i] = supportedStates[i];
                }
                delete[] supportedStates;
            }
            supportedStates = temp;

        }
        supportedStates[numberOfSupportedStates] = stateName;
        numberOfSupportedStates++;
    }
}

bool GAMGroup::Initialise(StructuredDataI &data) {
    bool ret = ReferenceContainer::Initialise(data);

    if (ret) {
        // Setup the context
        SetUp();
    }
    return ret;
}

}
