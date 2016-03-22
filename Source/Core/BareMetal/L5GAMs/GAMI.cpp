/**
 * @file GAMI.cpp
 * @brief Source file for class GAMI
 * @date 21/mar/2016
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
 * the class GAMI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMI.h"
#include "RealTimeDataDefContainer.h"
#include "RealTimeDataSourceContainer.h"
#include "RealTimeDataDefI.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static const uint32 stateNamesGranularity = 4u;
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMI::GAMI() :
        ReferenceContainer() {
    localData = NULL_PTR(StructuredDataI*);
    numberOfSupportedStates = 0u;
    supportedStates = NULL_PTR(StreamString *);
    group = NULL_PTR(GAMGroup*);
    application = NULL_PTR(RealTimeApplication *);
}

/*lint -e{1551} no exception should be thrown*/
GAMI::~GAMI() {
    if (supportedStates != NULL) {
        delete[] supportedStates;
    }

    application=NULL_PTR(RealTimeApplication *);
    group = NULL_PTR(GAMGroup*);
}

/*void GAMI::SetUp() {
 // initialises the local status
 }*/

/*
 virtual void Execute(){
 // execution routine
 */

bool GAMI::ConfigureFunction() {
    // use for each of them before RealTimeDataContainer::MergeWithLocal(localData)
    // and merge the result with the existing one
    bool ret = true;
    if (localData != NULL) {
        uint32 numberOfElements = Size();
        for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {

            ReferenceT<RealTimeDataDefContainer> def = Get(i);

            // must be all RealTimeDataDefContainer ??

            if (def.IsValid()) {
                const char8 * defName = def->GetName();
                if (localData->MoveRelative(defName)) {
                    // the partial definitions after this must become complete
                    ret = def->MergeWithLocal(*localData);
                    if (ret) {
                        ret = localData->MoveToAncestor(1u);
                    }
                }
                if (ret) {
                    // check if the definitions are meaningful
                    ret = def->Verify();
                }
            }
        }
    }
    return ret;
}

bool GAMI::ConfigureDataSource() {

    bool ret = true;
    if (GetNumberOfSupportedStates() > 0u) {
        ret = (application != NULL);
        if (ret) {
            ReferenceT<RealTimeDataSourceContainer> dataContainer;
            /*lint -e{613} NULL pointer checking done before entering here */
            uint32 numberOfAppItems = application->Size();
            ret = false;
            for (uint32 i = 0u; (i < numberOfAppItems) && (!ret); i++) {
                /*lint -e{613} NULL pointer checking done before entering here */
                Reference item = application->Get(i);
                if (item.IsValid()) {
                    if (StringHelper::Compare(item->GetName(), "+Data") == 0) {
                        dataContainer = item;
                        ret = dataContainer.IsValid();
                    }
                }
            }

            if (ret) {
                ret = dataContainer->AddDataDefinition(ReferenceT<GAMI>(this));
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "+Data container not found or invalid in the application");
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Application not set");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::Warning, "GAMI %s never called in states or threads");
    }
    return ret;
}

bool GAMI::Initialise(StructuredDataI & data) {

    bool ret = ReferenceContainer::Initialise(data);

    if (ret) {
        // implemented in the derived classes
        // get the local cdb
        SetUp();
        // merge definitions
        ret = ConfigureFunction();
    }
    return ret;
}

void GAMI::SetApplication(RealTimeApplication &rtApp) {
    if (application == NULL) {
        application = &rtApp;
    }
}

void GAMI::SetGAMGroup(ReferenceT<GAMGroup> gamGroup) {
    if (group == NULL) {
        group = gamGroup.operator->();
    }
}

void GAMI::AddState(const char8 * const stateName) {

    bool found = false;
    for (uint32 i = 0u; (i < numberOfSupportedStates) && (!found); i++) {
        /*lint -e{613} never enter here if supportedStates is NULL (because numberOfSupportedStates == 0) */
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
        /*lint -e{613} the memory of supportedStates is already allocated (numberOfSupportedStates == 0) */
        supportedStates[numberOfSupportedStates] = stateName;
        numberOfSupportedStates++;
    }

}

StreamString * GAMI::GetSupportedStates() {
    return (group != NULL) ? (group->GetSupportedStates()) : (supportedStates);
}

uint32 GAMI::GetNumberOfSupportedStates() const {
    return (group != NULL) ? (group->GetNumberOfSupportedStates()) : (numberOfSupportedStates);

}

bool GAMI::IsSync() const {
    return false;
}

}
