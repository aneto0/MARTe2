/**
 * @file GAM.cpp
 * @brief Source file for class GAM
 * @date 18/feb/2016
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
 * the class GAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAM.h"
#include "ObjectRegistryDatabase.h"
//#include "GAMBlock.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

GAM::GAM() {
    supportedStates = NULL_PTR(StreamString*);
    supportedContexts = NULL_PTR(ReferenceT<GAMContext>*);
    numberOfContexts=0u;
}

GAM::~GAM() {
    if (supportedStates != NULL) {
        delete supportedStates;
    }
    if(supportedContexts!=NULL) {
        delete supportedContexts;
    }
}

void GAM::SetUp() {
    for (uint32 i = 0u; i < Size(); i++) {
  //      ReferenceT<GAMBlock> block = Get(i);
  //      if (block.IsValid()) {
          //  block.SetUp();
          //  block.SetContext(supportedContexts[0u]);
        //}
    }
}

void GAM::ChangeState() {
    static uint32 contextNumber=0u;
    // circular
    if((contextNumber%numberOfContexts)==0u){
        contextNumber=0u;
    }
    for (uint32 i = 0u; i < Size(); i++) {
        //ReferenceT<GAMBlock> block = Get(i);
        //if (block.IsValid()) {
           // block.SetContext(supportedContexts[contextNumber]);
        //}
    }
}

bool GAM::Initialise(StructuredDataI &data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        AnyType statesAt = data.GetType("states");
        uint32 numberOfSupportedStates = statesAt.GetNumberOfElements(0u);
        supportedStates = new StreamString[numberOfSupportedStates];
        ret = (data.Read("states", supportedStates));
    }
    if (ret) {
        AnyType statesAt = data.GetType("contexts");
        uint32 numberOfSupportedContexts = statesAt.GetNumberOfElements(0u);
        StreamString *contextString;

        supportedContexts = new ReferenceT<GAMContext> [numberOfSupportedContexts];
        contextString = new StreamString[numberOfSupportedContexts];
        ret = (data.Read("states", supportedStates));

        // fill the supported context array
        for (uint32 i = 0u; (i < numberOfSupportedContexts) && (ret); i++) {
            ObjectRegistryDatabase* god = ObjectRegistryDatabase::Instance();
            ReferenceT<GAMContext> context = god->Find(contextString[i].Buffer());
            ret = context.IsValid();
            if (ret) {
                supportedContexts[i] = context;
                numberOfContexts++;
            }
        }
    }
    return ret;
}

}
