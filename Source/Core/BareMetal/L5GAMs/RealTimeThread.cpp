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
        delete functions;
    }
}

bool RealTimeThread::Validate(RealTimeApplication &rtApp,
                              RealTimeState &rtState) {

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    // checks if the gam referenced declared here exist and support the state where this thread is declared into
    for (uint32 i = 0u; i < numberOfFunctions; i++) {
        // search
        ReferenceT<GAM> function = god->Find(functions[i].Buffer());
        if (function.IsValid()) {
            rtState.AddFunction(function);
        }
        else {
            //TODO warning the gam referenced does not exists
        }

    }

    return true;
}

bool RealTimeThread::Initialise(StructuredDataI & data) {
    bool ret = false;
    if (ReferenceContainer::Initialise(data)) {
        // set up the string array
        AnyType functionsArray = data.GetType("functions");
        if (functionsArray.GetDataPointer() != NULL) {
            numberOfFunctions=functionsArray.GetNumberOfElements(0u);
            functions=new StreamString[numberOfFunctions];

            Vector<StreamString> functionVector(functions, numberOfFunctions);

            if(data.Read("functions", functionVector)) {
                ret=true;
            }
        }
    }
    return ret;
}

}
