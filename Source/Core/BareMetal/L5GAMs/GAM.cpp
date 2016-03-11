/**
 * @file GAM.cpp
 * @brief Source file for class GAM
 * @date 18/02/2016
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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAM.h"
#include "RealTimeDataDefContainer.h"
#include "RealTimeDataSource.h"
#include "RealTimeDataDefI.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static const uint32 stateNamesGranularity = 8u;
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAM::GAM() {
    localData = NULL_PTR(StructuredDataI*);
    numberOfSupportedStates = 0u;
    supportedStates = NULL_PTR(StreamString *);
    inputReader = ReferenceT<RealTimeDataSourceInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (inputReader.IsValid()) {
        inputReader->SetName("inputReader");
    }
    outputWriter = ReferenceT<RealTimeDataSourceOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (outputWriter.IsValid()) {
        outputWriter->SetName("outputWriter");
    }
}

GAM::~GAM() {
    if (supportedStates != NULL) {
        delete[] supportedStates;
    }
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

bool GAM::ConfigureDataSource() {

    bool ret = true;
    if (GetNumberOfSupportedStates() > 0u) {
        ret = application.IsValid();
        if (ret) {
            ReferenceT<RealTimeDataSource> dataContainer = application->Find("+Data");
            ret = (dataContainer.IsValid());
            if (ret) {
                ret = dataContainer->AddDataDefinition(ReferenceT<GAM>(this));
            }
            else {
                //TODO Data container not found
                printf("\nError, data container not found\n");
            }
        }
        else {
            //TODO application not set
            printf("\nError, application not set\n");
        }
    }
    else {
        //TODO Warning GAM never called
    }
    return ret;
}

bool GAM::Initialise(StructuredDataI & data) {

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

bool GAM::ConfigureDataSourceLinks() {
    // it is virtual... can be overriden if the data are static

    bool ret = (inputReader.IsValid() && outputWriter.IsValid());
    uint32 numberOfElements = Size();
    for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
        ReferenceT<RealTimeDataDefContainer> defContainer = Get(i);
        if (defContainer.IsValid()) {
            uint32 numberOfDefs = defContainer->Size();
            for (uint32 j = 0; (j < numberOfDefs) && (ret); j++) {
                ReferenceT<RealTimeDataDefI> def = defContainer->Get(j);
                if (def.IsValid()) {
                    if (defContainer->IsInput()) {
                        printf("\nAdd Variable in input\n");
                        ret = inputReader->AddVariable(def);
                    }
                    if (defContainer->IsOutput()) {
                        printf("\nAdd Variable in output\n");
                        ret = outputWriter->AddVariable(def);
                    }
                }
            }
        }
    }
    if (ret) {
        ret = inputReader->Finalise();
    }
    if (ret) {
        ret = outputWriter->Finalise();
    }

    return ret;
}

void GAM::SetApplication(ReferenceT<RealTimeApplication> rtApp) {
    if (!application.IsValid()) {
        application = rtApp;
        inputReader->SetApplication(rtApp);
        outputWriter->SetApplication(rtApp);
    }
}

void GAM::SetGAMGroup(ReferenceT<GAMGroup> gamGroup) {
    if (!group.IsValid()) {
        group = gamGroup;
    }
}

void GAM::AddState(const char8 *stateName) {

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

StreamString * GAM::GetSupportedStates() {
    return (group.IsValid()) ? (group->GetSupportedStates()) : (supportedStates);
}

uint32 GAM::GetNumberOfSupportedStates() {
    return (group.IsValid()) ? (group->GetNumberOfSupportedStates()) : (numberOfSupportedStates);

}

ReferenceT<RealTimeDataSourceInputReader> GAM::GetInputReader() {
    return inputReader;
}

ReferenceT<RealTimeDataSourceInputReader> GAM::GetOutputWriter() {
    return outputWriter;
}

}
