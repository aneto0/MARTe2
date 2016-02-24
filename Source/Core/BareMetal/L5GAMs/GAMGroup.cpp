/**
 * @file GAMGroup.cpp
 * @brief Source file for class GAMGroup
 * @date 24/feb/2016
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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

GAMGroup::GAMGroup() {
    supportedStates = NULL_PTR(StreamString*);
}

GAMGroup::~GAMGroup() {
    if (supportedStates != NULL) {
        delete supportedStates;
    }
}

void GAMGroup::SetUp() {
    for (uint32 i = 0u; i < Size(); i++) {
        ReferenceT<GAM> gam = Get(i);
        if (gam.IsValid()) {
            gam->SetUp();
        }
    }
}

void GAMGroup::ChangeState() {
    // Use the two buffer in GAMContext
}

bool GAMGroup::Initialise(StructuredDataI &data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        AnyType statesAt = data.GetType("States");
        uint32 numberOfSupportedStates = statesAt.GetNumberOfElements(0u);
        supportedStates = new StreamString[numberOfSupportedStates];
        ret = (data.Read("states", supportedStates));
    }
    return ret;
}
}
