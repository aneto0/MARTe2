/**
 * @file RealTimeThread.cpp
 * @brief Source file for class RealTimeThread
 * @date 19/feb/2016
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
 * the class RealTimeThread (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeThread.h"
#include "Vector.h"
#include "ObjectRegistryDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

RealTimeThread::RealTimeThread() {
    functions = NULL_PTR(StreamString*);
    numberOfFunctions = 0u;
}

RealTimeThread::~RealTimeThread() {
    if (functions != NULL) {
        delete [] functions;
    }
}

bool RealTimeThread::Validate(RealTimeApplication &rtApp,
                              RealTimeState &rtState) {

    bool ret = true;
    for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
        ret = false;
        // search
        Reference functionGeneric = ObjectRegistryDatabase::Instance()->Find(functions[i].Buffer());
        if (functionGeneric.IsValid()) {
            // case GAMGroup (stateful gams)
            ReferenceT<GAMGroup> functionGAMGroup = functionGeneric;
            if (functionGAMGroup.IsValid()) {
                // add the GAMGroup to the RealTimeState accelerator array
                if (rtState.AddGAMGroup(functionGAMGroup)) {
                    uint32 nOfSubGAMs = functionGAMGroup->Size();
                    // add all the gams in the order of the configuration inside GAMGroup
                    for (uint32 j = 0u; j < nOfSubGAMs; j++) {
                        ReferenceT<GAM> subGam = functionGAMGroup->Get(j);
                        if (subGam.IsValid()) {
                            if (Insert(subGam)) {
                                ret = true;
                            }
                        }
                    }
                }
            }
            else {
                // case stateless GAM
                ReferenceT<GAM> functionGAM = functionGeneric;
                if (functionGAM.IsValid()) {
                    if (Insert(functionGAM)) {
                        ret = true;
                    }
                }
            }
        }
        else{
            //TODO error the gam referenced does not exists
        }
    }

    return ret;
}

bool RealTimeThread::Initialise(StructuredDataI & data) {
    bool ret = false;
    // set up the string array
    AnyType functionsArray = data.GetType("Functions");
    if (functionsArray.GetDataPointer() != NULL) {
        numberOfFunctions=functionsArray.GetNumberOfElements(0u);
        functions=new StreamString[numberOfFunctions];

        Vector<StreamString> functionVector(functions, numberOfFunctions);

        if(data.Read("Functions", functionVector)) {
            ret=true;
        }
    }
    return ret;
}

StreamString * RealTimeThread::GetFunctions() const {
    return functions;
}

uint32 RealTimeThread::GetNumberOfFunctions() const {
    return numberOfFunctions;
}

}
