/**
 * @file GAM.cpp
 * @brief Source file for class GAM
 * @date 07/04/2016
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
 * the class GAM (public, protected, and private). Be aware that some 
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
#include "GAMSignalsContainer.h"
#include "DataSourceContainer.h"
#include "AdvancedErrorManagement.h"
#include "DataSourceBrokerI.h"
#include "DataSource.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static const uint32 stateNamesGranularity = 4u;
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAM::GAM() :
        ReferenceContainer() {
    localData = NULL_PTR(StructuredDataI*);
    numberOfSupportedStates = 0u;
    supportedStates = NULL_PTR(StreamString *);
    supportedThreads = NULL_PTR(StreamString *);
    group = NULL_PTR(GAMGroup*);
    application = NULL_PTR(RealTimeApplication *);
    inputReaders = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    outputWriters = ReferenceT<BrokerContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
}

/*lint -e{1551} no exception should be thrown*/
GAM::~GAM() {
    if (supportedStates != NULL) {
        delete[] supportedStates;
    }

    if (supportedThreads != NULL) {
        delete[] supportedThreads;
    }

    application=NULL_PTR(RealTimeApplication *);
    group = NULL_PTR(GAMGroup*);
}

/*void GAM::SetUp() {
 // initialises the local status
 }*/

/*
 virtual void Execute(){
 // execution routine
 */

bool GAM::ConfigureFunction() {
    // use for each of them before RealTimeDataContainer::MergeWithLocal(localData)
    // and merge the result with the existing one
    bool ret = true;
    if (localData != NULL) {
        uint32 numberOfElements = Size();
        for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {

            ReferenceT<GAMSignalsContainer> def = Get(i);

            // must be all GAMSignalsContainer ??

            if (def.IsValid()) {
                StreamString defName = "+";
                defName+=def->GetName();
                if (localData->MoveRelative(defName.Buffer())) {
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

bool GAM::ConfigureDataSource() {

    bool ret = true;
    if (GetNumberOfSupportedStates() > 0u) {
        ret = (application != NULL);
        if (ret) {
            ReferenceT<DataSourceContainer> dataContainer = application->GetDataSourceContainer();
            ret = dataContainer.IsValid();
            if (ret) {
                ret = dataContainer->AddDataDefinition(ReferenceT<GAM>(this));
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
        REPORT_ERROR(ErrorManagement::Warning, "GAM %s never called in states or threads");
    }
    return ret;
}

bool GAM::Initialise(StructuredDataI & data) {

    bool ret = ReferenceContainer::Initialise(data);
    ret = inputReaders.IsValid();
    if (ret) {
        StreamString irName = GetName();
        irName += "_InputReader";
        inputReaders->SetName(irName.Buffer());
        inputReaders->SetAsInput(true);
    }

    if (ret) {
        ret = outputWriters.IsValid();
        if (ret) {
            StreamString owName = GetName();
            owName += "_OutputWriter";
            inputReaders->SetName(owName.Buffer());
            outputWriters->SetAsInput(false);
        }
        else {
            //TODO
        }
    }
    else {
        //TODO
    }

    if (ret) {
        // implemented in the derived classes
        // get the local cdb
        SetUp();
        // merge definitions
        ret = ConfigureFunction();
    }
    return ret;
}

void GAM::SetApplication(RealTimeApplication &rtApp) {
    if (application == NULL) {
        application = &rtApp;
    }
    inputReaders->SetApplication(rtApp);
    outputWriters->SetApplication(rtApp);
}

void GAM::SetGAMGroup(ReferenceT<GAMGroup> gamGroup) {
    if (group == NULL) {
        group = gamGroup.operator->();
    }
}

bool GAM::AddState(const char8 * const stateName,
                   const char8 * const threadName) {

    bool ret = true;
    bool found = false;
    for (uint32 i = 0u; (i < numberOfSupportedStates) && (!found); i++) {
        /*lint -e{613} never enter here if supportedStates is NULL (because numberOfSupportedStates == 0) */
        found = (supportedStates[i] == stateName);

        if (found) {
            ret = (supportedThreads[i] == threadName);
            if (!ret) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The GAM %s cannot be in both %s and %s threads into the state %s", GetName(),
                                        supportedThreads[i].Buffer(), threadName, stateName)
                //TODO same gam in two different threads!
            }
        }

    }
    if (!found) {
        if ((numberOfSupportedStates % stateNamesGranularity) == 0u) {
            uint32 newSize = numberOfSupportedStates + stateNamesGranularity;
            StreamString *tempStates = new StreamString[newSize];
            StreamString *tempThreads = new StreamString[newSize];
            if (supportedStates != NULL) {
                for (uint32 i = 0u; i < numberOfSupportedStates; i++) {
                    tempStates[i] = supportedStates[i];
                    tempThreads[i] = supportedThreads[i];
                }
                delete[] supportedStates;
                delete[] supportedThreads;
            }
            supportedStates = tempStates;
            supportedThreads = tempThreads;

        }
        /*lint -e{613} the memory of supportedStates is already allocated (numberOfSupportedStates == 0) */
        supportedStates[numberOfSupportedStates] = stateName;
        supportedThreads[numberOfSupportedStates] = threadName;
        numberOfSupportedStates++;
    }
    return ret;
}

StreamString * GAM::GetSupportedStates() {
    return supportedStates;
}

StreamString * GAM::GetSupportedThreads() {
    return supportedThreads;
}

uint32 GAM::GetNumberOfSupportedStates() const {
    return numberOfSupportedStates;

}

/*void GAM::SetUp() {
 // initialises the local status
 }*/

/*
 virtual void Execute(){
 // execution routine
 */

bool GAM::ConfigureDataSourceLinks() {
    bool ret = true;
    uint32 numberOfElements = Size();

    for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
        ReferenceT<GAMSignalsContainer> defContainer = Get(i);
        if (defContainer.IsValid()) {
            uint32 numberOfDefs = defContainer->Size();
            for (uint32 j = 0u; (j < numberOfDefs) && (ret); j++) {
                ReferenceT<GAMSignalI> def = defContainer->Get(j);
                ret = def.IsValid();
                if (ret) {

                    if (defContainer->IsInput()) {
                        ret = (inputReaders->AddSignal(def));
                        if (!ret) {
                            // TODO definition not compatible with
                            // its data source !!!

                        }
                    }
                    if (defContainer->IsOutput()) {
                        ret = (outputWriters->AddSignal(def));
                        if (!ret) {
                            // TODO definition not compatible with
                            // its data source !!!

                        }
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "The GAMSignalsContainer must contain GAMSignalI objects");
                }
            }
        }
    }

    if (ret) {
        ret = inputReaders->Finalise();
        if (!ret) {
            //TODO Failed readers finalisation
        }
    }
    if (ret) {
        ret = outputWriters->Finalise();
        if (!ret) {
            //TODO Failed readers finalisation
        }
    }

    if (ret) {
        // not both sync!
        ret = !((inputReaders->IsSync()) && (outputWriters->IsSync()));
    }

    return ret;
}

bool GAM::IsSync() {
    return (inputReaders->IsSync() || outputWriters->IsSync());
}

Reference GAM::GetInputReader() {
    return inputReaders;
}

Reference GAM::GetOutputWriter() {
    return outputWriters;
}

RealTimeApplication *GAM::GetApplication() {
    return application;
}

}
