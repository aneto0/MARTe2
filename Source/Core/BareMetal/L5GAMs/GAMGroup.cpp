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

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "GAM.h"
#include "GAMGroup.h"
#include "ReferenceContainerFilterReferencesTemplate.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMGroup::GAMGroup() :
        ReferenceContainer() {
}

GAMGroup::~GAMGroup() {
}

bool GAMGroup::Initialise(StructuredDataI & data){
    bool ret=ReferenceContainer::Initialise(data);
    if(ret){
        //Look for all the GAMs inside the RealTimeApplication
        ReferenceContainerFilterReferencesTemplate<GAM> gamFilter(-1, ReferenceContainerFilterMode::RECURSIVE);
        Find(GAMs, gamFilter);
    }
    return ret;
}

bool GAMGroup::SetContext(ConstReference context) {
    uint32 numberOfGAMs = GAMs.Size();
    bool ret = true;
    for (uint32 i = 0u; (i < numberOfGAMs) && (ret); i++) {
        ReferenceT<GAM> gam = GAMs.Get(i);
        ret = gam.IsValid();
        if(ret){
            ret = gam->SetContext(context);
        }
    }
    return ret;
}

}
