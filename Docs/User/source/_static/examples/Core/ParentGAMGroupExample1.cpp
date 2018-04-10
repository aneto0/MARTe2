/**
 * @file ParentGAMGroupExample1.cpp
 * @brief Source file for class ParentGAMGroupExample1
 * @date 06/04/2018
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

 * @details This source file contains the definition of all the methods for
 * the class ParentGAMGroupExample1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ParentGAMGroupExample1.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ParentGAMGroupExample1::ParentGAMGroupExample1() {
}

ParentGAMGroupExample1::~ParentGAMGroupExample1() {

}

bool ParentGAMGroupExample1::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = GAMGroup::Initialise(data);
    matrixModelContext = MARTe::ReferenceT<GAMGroupSharedInfoExample1>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (ok) {
        ok = matrixModelContext.IsValid();
    }
    if (ok) {
        ok = matrixModelContext->Initialise(data);
    }
    return ok;
}

bool ParentGAMGroupExample1::PrepareNextState(const MARTe::char8*, const MARTe::char8*) {
    using namespace MARTe;
    bool ok = matrixModelContext.IsValid();
    if (ok) {
        ok = SetContext(matrixModelContext);
    }
    return ok;
}

CLASS_REGISTER(ParentGAMGroupExample1, "")
