/**
 * @file RealTimeApplication.cpp
 * @brief Source file for class RealTimeApplication
 * @date 19/02/2016
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
 * the class RealTimeApplication (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeApplication.h"
#include "RealTimeState.h"
#include "ReferenceContainerFilterReferences.h"
#include "ReferenceContainerFilterObjectName.h"
#include "ReferenceContainerFilterReferencesTemplate.h"
#include "DataSourceContainer.h"
#include "GAM.h"
#include "GAMSchedulerI.h"
#include "DataSourceContainer.h"
#include "AdvancedErrorManagement.h"
#include "Matrix.h"
#include "RealTimeApplicationConfigurationBuilder.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
#if 0
static bool ConfigureDataSourcePrivate(ReferenceT<ReferenceContainer> functions) {

    bool ret = functions.IsValid();

    if (ret) {
        uint32 numberOfFunctions = functions->Size();
        for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
            Reference genericFunction = functions->Get(i);

            ReferenceT<GAM> gam = genericFunction;
            // a GAM
            if (gam.IsValid()) {
                // call the single gam configuration
                ret = gam->ConfigureDataSource();
            }
            else {
                // a GAMGroup
                ReferenceT<GAMGroup> gamGroup = genericFunction;
                if (gamGroup.IsValid()) {
                    uint32 numberOfSubGams = gamGroup->Size();
                    for (uint32 j = 0u; (j < numberOfSubGams) && (ret); j++) {
                        ReferenceT<GAM> subGam = gamGroup->Get(j);
                        ret = subGam.IsValid();
                        if (ret) {
                            // call the single gam configuration
                            ret = subGam->ConfigureDataSource();
                        }
                    }
                }
                else {
                    // a ReferenceContainer
                    ReferenceT<ReferenceContainer> gamContainer = genericFunction;
                    ret = gamContainer.IsValid();
                    if (ret) {
                        // go recursively
                        ret = ConfigureDataSourcePrivate(gamContainer);
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "+Functions must contain GAM, GAMGroup or ReferenceContainer references");
                    }
                }
            }
        }
    }

    return ret;
}

static bool ConfigureDataSourceLinksPrivate(ReferenceT<ReferenceContainer> functions) {

    bool ret = functions.IsValid();

    if (ret) {
        uint32 numberOfFunctions = functions->Size();
        for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
            Reference genericFunction = functions->Get(i);

            ReferenceT<GAM> gam = genericFunction;
            // a GAM
            if (gam.IsValid()) {
                // call the single gam configuration
                ret = gam->ConfigureDataSourceLinks();
            }
            else {
                // a GAMGroup
                ReferenceT<GAMGroup> gamGroup = genericFunction;
                if (gamGroup.IsValid()) {
                    uint32 numberOfSubGams = gamGroup->Size();
                    for (uint32 j = 0u; (j < numberOfSubGams) && (ret); j++) {
                        ReferenceT<GAM> subGam = gamGroup->Get(j);
                        ret = subGam.IsValid();
                        if (ret) {
                            // call the single gam configuration
                            ret = subGam->ConfigureDataSourceLinks();
                        }
                    }
                }
                else {
                    // a ReferenceContainer
                    ReferenceT<ReferenceContainer> gamContainer = genericFunction;
                    ret = gamContainer.IsValid();
                    if (ret) {
                        // go recursively
                        ret = ConfigureDataSourceLinksPrivate(gamContainer);
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "+Functions must contain GAM, GAMGroup or ReferenceContainer references");
                    }
                }
            }
        }
    }

    return ret;
}
#endif
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeApplication::RealTimeApplication() :
        ReferenceContainer() {
#if 0
    activeBuffer = 1u;
#endif
}

RealTimeApplication::~RealTimeApplication() {

}
#if 0
bool RealTimeApplication::ConfigureArchitecture() {

    // there must be the container called "States"
    bool ret = statesContainer.IsValid();

    if (ret) {
        // States contains RealTimeState references
        // for each of them call Validate(*)
        uint32 numberOfStates = statesContainer->Size();
        for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
            ReferenceT<RealTimeState> state = statesContainer->Get(i);
            if (state.IsValid()) {
                // for each state call the configuration function
                ret = state->ConfigureArchitecture(*this);
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "+States container not found");
    }
    if (ret) {
        ReferenceT<GAMSchedulerI> scheduler = schedulerContainer;
        ret = (scheduler.IsValid());
        if (ret) {
            scheduler->SetApplication(*this);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "+Scheduler container not found");
        }
    }
    return ret;
}

bool RealTimeApplication::ConfigureDataSource() {

    bool ret = functionsContainer.IsValid();

    if (ret) {
        // configure
        ret = ConfigureDataSourcePrivate(functionsContainer);
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "+Functions container not found");
    }

    return ret;

}

bool RealTimeApplication::ValidateDataSource() {
    ReferenceT<DataSourceContainer> data = dataSourceContainer;

    bool ret = data.IsValid();
    // there must be the container called "States"

    if (ret) {
        // States contains RealTimeState references
        // for each of them call Validate(*)
        ret = data->Verify();
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "+Data container not found");
    }
    return ret;
}

bool RealTimeApplication::AllocateDataSource() {
    ReferenceT<DataSourceContainer> data = dataSourceContainer;

    bool ret = data.IsValid();
    if (ret) {
        ret = data->Allocate();
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "+Data container not found");
    }
    return ret;
}

bool RealTimeApplication::ConfigureDataSourceLinks() {
    // there must be the container called "States"
    bool ret = functionsContainer.IsValid();
    if (ret) {
        // configure
        ret = ConfigureDataSourceLinksPrivate(functionsContainer);
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "+Functions container not found");
    }

    return ret;
}

bool RealTimeApplication::ValidateDataSourceLinks() {
    // there must be the container called "States"
    bool ret = statesContainer.IsValid();
    if (ret) {
        uint32 numberOfStates = statesContainer->Size();
        for (uint32 i = 0u; (i < numberOfStates) && ret; i++) {
            ReferenceT<RealTimeState> state = statesContainer->Get(i);
            ret = state.IsValid();
            if (ret) {
                ret = state->ValidateDataSourceLinks();
            }
        }

    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "+States container not found");
    }

    return ret;
}

bool RealTimeApplication::PrepareNextState(const char8 * const nextStateName) {

    RealTimeStateInfo status;
    status.currentState = currentStateName.Buffer();
    status.nextState = nextStateName;
    status.activeBuffer = activeBuffer;

    StreamString nextStatePath = "States.";
    nextStatePath += nextStateName;

    ReferenceT<RealTimeState> nextState = Find(nextStatePath.Buffer());
    bool ret = nextState.IsValid();
    if (ret) {
        // change the context in gam groups if needed
        nextState->PrepareState(status);
    }
    if (ret) {
        ReferenceT<DataSourceContainer> dataSource = dataSourceContainer;
        ret = dataSource.IsValid();
        if (ret) {
            // resets the default value in data sources if needed
            ret = dataSource->PrepareNextState(status);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "+Data container not found");
        }
    }
    if (ret) {
        ReferenceT<GAMSchedulerI> scheduler = schedulerContainer;
        ret = scheduler.IsValid();
        // save the accelerator to the next group of threads to be executed
        if (ret) {
            ret = scheduler->PrepareNextState(status);
        }
        if (ret) {
            scheduler->ChangeState(activeBuffer);
        }
    }
    activeBuffer = (activeBuffer + 1u) % 2u;

    return ret;
}

bool RealTimeApplication::StopExecution() {
    ReferenceT<GAMSchedulerI> scheduler = schedulerContainer;
    bool ret = scheduler.IsValid();
    if (ret) {
        scheduler->StopExecution();
    }
    return ret;
}
#endif
#include <stdio.h>

bool RealTimeApplication::SignalIntrospectionToStructuredData(StreamString typeName,
                                                              StreamString signalName,
                                                              StreamString alias,
                                                              StreamString dataSourceName,
                                                              AnyType ranges,
                                                              StructuredDataI & data,
                                                              uint32 &signalNumber) {

    const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(typeName.Buffer());
    const Introspection *intro = NULL_PTR(Introspection *);
    bool ret = (item != NULL);

    if (ret) {
        /*lint -e{613} NULL pointer checking done before entering here */
        intro = item->GetIntrospection();
    }
    else {
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The type %s is not registered", typeName)
    }
    ret = (intro != NULL);
    if (ret) {
        uint32 numberOfMembers = intro->GetNumberOfMembers();
        uint32 i;
        //For each member...
        for (i = 0u; (i < numberOfMembers) && ret; i++) {
            const IntrospectionEntry entry = intro->operator[](i);
            StreamString fullSignalName;
            fullSignalName.Printf("%s.%s", signalName.Buffer(), entry.GetMemberName());
            StreamString fullAliasName = "";
            if (alias.Size() > 0u) {
                fullAliasName.Printf("%s.%s", alias.Buffer(), entry.GetMemberName());
            }
            //...if the member is still structured data, continue to discombobulate...
            if (entry.GetMemberTypeDescriptor().isStructuredData) {
                ret = SignalIntrospectionToStructuredData(entry.GetMemberTypeName(), fullSignalName, fullAliasName, dataSourceName, ranges, data, signalNumber);
            }
            else {
                //...add a node with the signal number as the name...
                StreamString signalNumberStr;
                signalNumberStr.Printf("Signals.%d", signalNumber);
                ret = data.CreateAbsolute(signalNumberStr.Buffer());
                //Finally got to the BasicType. Write all the properties
                if (ret) {
                    ret = data.Write("QualifiedName", fullSignalName.Buffer());
                }
                if (fullAliasName.Size() > 0u) {
                    ret = data.Write("Alias", fullAliasName.Buffer());
                }
                if (ranges.GetTypeDescriptor() != VoidType) {
                    ret = data.Write("Ranges", ranges);
                }
                if (ret) {
                    if (dataSourceName.Size() > 0u) {
                        ret = data.Write("DataSource", dataSourceName);
                    }
                }
                if (ret) {
                    ret = data.Write("Type", entry.GetMemberTypeName());
                }
                if (ret) {
                    ret = data.Write("NumberOfDimensions", entry.GetNumberOfDimensions());
                }
                if (ret) {
                    uint32 j = 0;
                    uint32 totalElements = 0u;
                    for (j = 0; j <= entry.GetNumberOfDimensions(); j++) {
                        totalElements += entry.GetNumberOfElements(j);
                    }

                    ret = data.Write("NumberOfElements", totalElements);
                }
                data.MoveToAncestor(1);
                signalNumber++;
            }
        }
    }
    return ret;
}

bool RealTimeApplication::LinkGAMToState(StreamString functionName,
                                         StreamString stateName,
                                         StreamString threadName) {
    //Check if the functionName is a GAMGroup
    bool ret = false;
    StreamString functionNodeName;
    functionNodeName.Printf("Functions.%s.%s", functionName, stateName);
    ret = functionsDatabase.Write(functionNodeName.Buffer(), threadName);
    return ret;
}

bool RealTimeApplication::AddSignalToDataSource(StreamString functionName,
                                                StreamString dataSourceName) {
    bool ret = true;
    StreamString signalName;
    //Expand the aliasName in order to write the correct name in the dataSourcesDatabase
    StreamString aliasName;
    if (!functionsDatabase.Read("Alias", aliasName)) {
        aliasName = "";
    }
    if (aliasName.Size() > 0u) {
        signalName = aliasName;
    }
    else {
        ret = functionsDatabase.Read("QualifiedName", signalName);
    }

    //Check if the signal already exists
    uint32 numberOfSignals = dataSourcesDatabase.GetNumberOfChildren();
    bool signalAlreadyExists = false;
    uint32 n = 0u;
    for (n = 0u; (n < numberOfSignals) && (ret) && (!signalAlreadyExists); n++) {
        StreamString signalId = dataSourcesDatabase.GetChildName(n);
        ret = dataSourcesDatabase.MoveRelative(signalId.Buffer());
        StreamString dataSourceSignalName;
        if (ret) {
            ret = dataSourcesDatabase.Read("QualifiedName", dataSourceSignalName);
        }
        if (ret) {
            signalAlreadyExists = (StringHelper::Compare(signalName.Buffer(), dataSourceSignalName.Buffer()) == 0u);
        }
        //Move to the next signal
        if (!signalAlreadyExists) {
            ret = dataSourcesDatabase.MoveToAncestor(1u);
        }
    }

    if (!signalAlreadyExists && (ret)) {
        //If the signal still does not exist in the dataSourcesDatabase create it.
        StreamString signalId;
        signalId.Printf("%d", numberOfSignals);
        ret = dataSourcesDatabase.CreateRelative(signalId.Buffer());
        if (ret) {
            ret = dataSourcesDatabase.Write("QualifiedName", signalName.Buffer());
        }
    }
    if (ret) {
        //Loop through all properties.
        const char8 *properties[] = { "Type", "NumberOfDimensions", "NumberOfElements", NULL_PTR(char8 *) };
        uint32 p = 0u;
        while ((properties[p] != NULL_PTR(char8 *)) && (ret)) {
            AnyType elementSignalDatabase = functionsDatabase.GetType(properties[p]);
            AnyType elementDataSourceDatabase = dataSourcesDatabase.GetType(properties[p]);
            if (elementSignalDatabase.GetTypeDescriptor() != VoidType) {
                if (elementDataSourceDatabase.GetTypeDescriptor() != VoidType) {
                    //Property already exists, check compatibility!
                    StreamString sElementSignalDatabase;
                    StreamString sElementDataSourceDatabase;
                    ret = functionsDatabase.Read(properties[p], sElementSignalDatabase);
                    if (ret) {
                        ret = dataSourcesDatabase.Read(properties[p], sElementDataSourceDatabase);
                    }
                    if (ret) {
                        ret = (StringHelper::Compare(sElementSignalDatabase.Buffer(), sElementDataSourceDatabase.Buffer()) == 0u);
                    }
                    if (!ret) {
                        //Report mismatch!
                        StreamString fullPropertyName;
                        fullPropertyName.Printf("%s.%s", signalName.Buffer(), properties[p]);
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Mismatch in signal with name: %s. %s asked for %s while %s asked for %s",
                                                signalName.Buffer(), functionName.Buffer(), sElementSignalDatabase.Buffer(), dataSourceName.Buffer(),
                                                sElementDataSourceDatabase.Buffer())
                    }
                }
                else {
                    ret = dataSourcesDatabase.Write(properties[p], elementSignalDatabase);
                }
            }
            p++;
        }
    }
    return ret;
}

bool RealTimeApplication::VerifyDataSourcesSignals() {
    bool ret = dataSourcesDatabase.MoveAbsolute("Data");
    uint32 numberOfDataSources = 0u;
    StreamString dataSourceName;
    if (ret) {
        numberOfDataSources = dataSourcesDatabase.GetNumberOfChildren();
    }
    if (ret) {
        uint32 i;
        for (i = 0; (i < numberOfDataSources) && (ret); i++) {
            StreamString dataSourceNumber = dataSourcesDatabase.GetChildName(i);
            ret = dataSourcesDatabase.MoveRelative(dataSourceNumber.Buffer());
            if (ret) {

                ret = dataSourcesDatabase.Read("QualifiedName", dataSourceName);
            }
            if (ret) {
                ret = dataSourcesDatabase.MoveRelative("Signals");
            }
            uint32 numberOfSignals = 0u;
            if (ret) {
                numberOfSignals = dataSourcesDatabase.GetNumberOfChildren();
            }
            uint32 s;
            for (s = 0u; (s < numberOfSignals) && (ret); s++) {
                StreamString signalId;
                signalId = dataSourcesDatabase.GetChildName(s);
                ret = dataSourcesDatabase.MoveRelative(signalId.Buffer());

                //The QualifiedName must also be known
                StreamString signalName;
                if (ret) {
                    ret = dataSourcesDatabase.Read("QualifiedName", signalName);
                }
                if (ret) {
                    ret = (signalName.Size() > 0u);
                }
                if (!ret) {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "QualifiedName was not defined for signal at position: %s in %s",
                                            signalId.Buffer(), dataSourceName.Buffer())
                }
                //At this state the Type must be defined
                StreamString type;
                if (ret) {
                    ret = dataSourcesDatabase.Read("Type", type);
                }
                if (ret) {
                    ret = (type.Size() > 0u);
                }
                if (!ret) {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Type was not defined for signal: %s in %s", signalName.Buffer(),
                                            dataSourceName.Buffer())
                }
                //If the number of dimensions is > 1 then the NumberOfElements must also be defined.
                uint32 numberOfDimensions = 0u;
                if (ret) {
                    if (!dataSourcesDatabase.Read("NumberOfDimensions", numberOfDimensions)) {
                        ret = dataSourcesDatabase.Write("NumberOfDimensions", 0u);
                    }
                }

                if (ret) {
                    uint32 numberOfElements = 0u;
                    if (!dataSourcesDatabase.Read("NumberOfElements", numberOfElements)) {
                        if (numberOfDimensions > 0u) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "NumberOfElements was not defined for signal: %s in %s",
                                                    signalName.Buffer(), dataSourceName.Buffer())
                        }
                        else {
                            ret = dataSourcesDatabase.Write("NumberOfElements", 1u);
                        }
                    }
                }

                if (ret) {
                    ret = dataSourcesDatabase.MoveToAncestor(1u);
                }
            }
            if (ret) {
                ret = dataSourcesDatabase.MoveToAncestor(2u);
            }
        }
    }
    return ret;
}

bool RealTimeApplication::ResolveFunctionSignal(StreamString signalName,
                                                StreamString functionName,
                                                StreamString dataSourceName,
                                                uint32 numberOfFunctionSignals) {

    uint32 numberOfSignalsInDataSource = dataSourcesDatabase.GetNumberOfChildren();
    uint32 createdSignals = 0u;
    uint32 s = 0u;
    StreamString type;
    bool typeDefined = functionsDatabase.Read("Type", type);
    bool atLeastOneSignalFound = false;
    bool ret = true;
    //If the Type is defined, stop as soon as the signal found. Otherwise loop for all the signals...
    //Note that signalNameInDataSource can be contained in signalName (e.g. looking for A.B and in the DataSource we have A.B.C and A.B.D,
    //which means that the signals A.B.C and A.B.D will be added to the GAM is the Type was not defined)
    for (s = 0; (s < numberOfSignalsInDataSource) && (ret) && (!typeDefined || !atLeastOneSignalFound); s++) {
        StreamString dataSourceSignalId = dataSourcesDatabase.GetChildName(s);
        ret = dataSourcesDatabase.MoveRelative(dataSourceSignalId.Buffer());
        StreamString dataSourceSignalName;
        if (ret) {
            ret = dataSourcesDatabase.Read("QualifiedName", dataSourceSignalName);
        }
        dataSourceSignalName.Seek(0u);
        signalName.Seek(0u);
        bool signalFound = false;
        bool createSignal = (signalName.Size() < dataSourceSignalName.Size());
        if (ret) {
            StreamString token1;
            StreamString token2;
            char8 terminator;
            while (signalName.GetToken(token1, ".", terminator)) {
                if (dataSourceSignalName.GetToken(token2, ".", terminator)) {
                    token1.Seek(0);
                    token2.Seek(0);
                    signalFound = (token1 == token2);
                }
                else {
                    signalFound = false;
                }
                token1 = "";
                token2 = "";
            }
        }
        if (signalFound) {
            if (!typeDefined) {
                if (createSignal) {
                    //Mark this signal to be deleted
                    if (!atLeastOneSignalFound) {
                        ret = functionsDatabase.Write("Delete", 1u);
                    }
                    if (ret) {
                        ret = functionsDatabase.MoveToAncestor(1u);
                    }
                    if (ret) {
                        StreamString newSignalId;
                        newSignalId.Printf("%d", (numberOfFunctionSignals + createdSignals));
                        createdSignals++;
                        ret = functionsDatabase.CreateRelative(newSignalId.Buffer());
                    }
                    if (ret) {
                        ret = functionsDatabase.Write("QualifiedName", dataSourceSignalName.Buffer());
                    }
                    if (ret) {
                        ret = functionsDatabase.Write("DataSource", dataSourceName.Buffer());
                    }
                }
            }
            atLeastOneSignalFound = true;
            //Loop through all properties. If the property is not defined, get it from the DataSource
            const char8 *properties[] = { "Type", "NumberOfDimensions", "NumberOfElements", NULL_PTR(char8 *) };
            uint32 p = 0u;
            while ((properties[p] != NULL_PTR(char8 *)) && (ret)) {
                AnyType elementSignalDatabase = functionsDatabase.GetType(properties[p]);
                AnyType elementDataSourceDatabase = dataSourcesDatabase.GetType(properties[p]);
                if (elementSignalDatabase.GetTypeDescriptor() == VoidType) {
                    if (elementDataSourceDatabase.GetTypeDescriptor() != VoidType) {
                        ret = functionsDatabase.Write(properties[p], elementDataSourceDatabase);
                    }
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "%s was not defined for signal: %s in %s", properties[p],
                                                signalName.Buffer(), functionName.Buffer())
                    }
                }
                //Check for compatibility
                else {
                    //Property already exists, check compatibility!
                    StreamString sElementSignalDatabase;
                    StreamString sElementDataSourceDatabase;
                    ret = functionsDatabase.Read(properties[p], sElementSignalDatabase);
                    if (ret) {
                        ret = dataSourcesDatabase.Read(properties[p], sElementDataSourceDatabase);
                    }
                    if (ret) {
                        ret = (StringHelper::Compare(sElementSignalDatabase.Buffer(), sElementDataSourceDatabase.Buffer()) == 0u);
                    }
                    if (!ret) {
                        //Report mismatch!
                        StreamString fullPropertyName;
                        fullPropertyName.Printf("%s.%s", signalName.Buffer(), properties[p]);
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Mismatch in signal with name: %s. %s asked for %s while %s asked for %s",
                                                signalName.Buffer(), functionName.Buffer(), sElementSignalDatabase.Buffer(), dataSourceName.Buffer(),
                                                sElementDataSourceDatabase.Buffer())
                    }
                }
                p++;
            }
        }
        if (ret) {
            //Next signal in dataSourcesDatabase
            ret = dataSourcesDatabase.MoveToAncestor(1u);
        }
    }

    if (!ret) {
        atLeastOneSignalFound = ret;
    }
    return atLeastOneSignalFound;
}

bool RealTimeApplication::ResolveFunctionSignalsMemorySize(const char *signalDirection) {
    bool ret = functionsDatabase.MoveAbsolute("Functions");
    uint32 numberOfFunctions = 0u;
    if (ret) {
        numberOfFunctions = functionsDatabase.GetNumberOfChildren();
    }
    if (ret) {
        uint32 i;
        //For every function
        for (i = 0; (i < numberOfFunctions) && (ret); i++) {
            StreamString functionNumber = functionsDatabase.GetChildName(i);
            ret = functionsDatabase.MoveRelative(functionNumber.Buffer());
            StreamString functionName;
            if (ret) {
                ret = functionsDatabase.Read("QualifiedName", functionName);
            }
            if (ret) {
                ret = functionsDatabase.MoveRelative("Signals");
            }
            bool exists = false;
            if (ret) {
                exists = functionsDatabase.MoveRelative(signalDirection);
            }
            if (exists) {
                uint32 numberOfSignals = 0u;
                if (ret) {
                    numberOfSignals = functionsDatabase.GetNumberOfChildren();
                }
                uint32 s;
                //For every signal in this function
                for (s = 0u; (s < numberOfSignals) && (ret); s++) {
                    uint32 signalNumberOfBytes = 0u;
                    uint32 numberOfElements = 0u;
                    TypeDescriptor signalTypeDescriptor;
                    StreamString signalId;
                    StreamString signalType;
                    StreamString signalName;
                    signalId = functionsDatabase.GetChildName(s);
                    ret = functionsDatabase.MoveRelative(signalId.Buffer());
                    if (ret) {
                        ret = functionsDatabase.Read("NumberOfElements", numberOfElements);
                    }
                    if (ret) {
                        ret = functionsDatabase.Read("QualifiedName", signalName);
                    }
                    if (ret) {
                        ret = functionsDatabase.Read("Type", signalType);
                    }
                    if (ret) {
                        //Get the signal type
                        signalTypeDescriptor = TypeDescriptor::GetTypeDescriptorFromTypeName(signalType.Buffer());
                    }
                    if (ret) {
                        ret = (signalTypeDescriptor != InvalidType);
                    }
                    uint32 *offsetMatrixBackend = NULL_PTR(uint32 *);
                    uint32 numberOfRanges = 0u;
                    if (ret) {
                        //If Ranges are defined, compute the total signal size.
                        AnyType ranges = functionsDatabase.GetType("Ranges");
                        if (ranges.GetTypeDescriptor() != VoidType) {
                            uint32 numberOfDimensions = ranges.GetNumberOfDimensions();
                            ret = (numberOfDimensions == 2u);
                            if (ret) {
                                numberOfRanges = ranges.GetNumberOfElements(1u);
                                if (numberOfRanges > 0u) {
                                    uint32 *rangesMatBackend = new uint32[numberOfRanges * 2u];
                                    offsetMatrixBackend = new uint32[numberOfRanges * 2u];
                                    //The offset matrix stores, for each range and in bytes, the starting offset and the size of the range to copy.
                                    Matrix<uint32> offsetMat(offsetMatrixBackend, numberOfRanges, 2u);
                                    //Read the Ranges matrix from the configuration data.
                                    Matrix<uint32> rangesMat(rangesMatBackend, numberOfRanges, 2u);
                                    ret = functionsDatabase.Read("Ranges", rangesMat);
                                    uint32 n;
                                    uint32 lastMaxIdx = 0u;
                                    for (n = 0u; (n < numberOfRanges) && (ret); n++) {
                                        uint32 minIdx = rangesMat[n][0];
                                        uint32 maxIdx = rangesMat[n][1];
                                        if (minIdx > maxIdx) {
                                            ret = false;
                                            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError,
                                                                    "Illegal Ranges for signal %s in %s: minimum index must be < maximum index",
                                                                    signalName.Buffer(), functionName.Buffer())
                                        }
                                        if (ret) {
                                            if (maxIdx >= numberOfElements) {
                                                ret = false;
                                                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError,
                                                                        "Illegal Ranges for signal %s in %s: maximum index must be < NumberOfElements",
                                                                        signalName.Buffer(), functionName.Buffer())
                                            }
                                        }
                                        if (ret) {
                                            if (n > 0u) {
                                                //Do not allow overlapping arrays!
                                                if (lastMaxIdx > minIdx) {
                                                    ret = false;
                                                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError,
                                                                            "Illegal Ranges for signal %s in %s: overlapping Ranges are not allowed",
                                                                            signalName.Buffer(), functionName.Buffer())
                                                }
                                            }
                                        }
                                        if (ret) {
                                            uint32 rangeByteSize = (maxIdx - minIdx + 1u) * signalTypeDescriptor.numberOfBits / 8u;
                                            signalNumberOfBytes += rangeByteSize;
                                            offsetMat[n][0u] = minIdx * signalTypeDescriptor.numberOfBits / 8u;
                                            offsetMat[n][1u] = rangeByteSize;
                                        }
                                        lastMaxIdx = maxIdx;
                                    }
                                    delete[] rangesMatBackend;
                                }
                            }
                            else {
                                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Illegal Ranges for signal %s in %s: NumberOfDimensions != 2",
                                                        signalName.Buffer(), functionName.Buffer())
                            }
                        }
                        else {
                            signalNumberOfBytes = numberOfElements * signalTypeDescriptor.numberOfBits / 8u;
                        }
                    }
                    if (ret) {
                        ret = functionsDatabase.Write("ByteSize", signalNumberOfBytes);
                    }
                    if (ret) {
                        if (offsetMatrixBackend != NULL_PTR(uint32 *)) {
                            Matrix<uint32> offsetMat(offsetMatrixBackend, numberOfRanges, 2u);
                            ret = functionsDatabase.Write("ByteOffset", offsetMat);
                            delete[] offsetMatrixBackend;
                        }
                    }
                    if (ret) {
                        //Move to next Signal in this signalDirection
                        ret = functionsDatabase.MoveToAncestor(1u);
                    }
                }
                if (ret) {
                    //Move to Signals level
                    ret = functionsDatabase.MoveToAncestor(1u);
                }
            }
            if (ret) {
                //Move to next Function
                ret = functionsDatabase.MoveToAncestor(2u);
            }
        }
    }
    return ret;
}

bool RealTimeApplication::ResolveFunctionSignals(const char *signalDirection) {
    bool ret = functionsDatabase.MoveAbsolute("Functions");
    uint32 numberOfFunctions = 0u;
    if (ret) {
        numberOfFunctions = functionsDatabase.GetNumberOfChildren();
    }
    if (ret) {
        uint32 i;
//For every function
        for (i = 0; (i < numberOfFunctions) && (ret); i++) {
            StreamString functionName;
            StreamString functionNumber = functionsDatabase.GetChildName(i);
            ret = functionsDatabase.MoveRelative(functionNumber.Buffer());
            if (ret) {
                ret = functionsDatabase.Read("QualifiedName", functionName);
            }
            printf("FunctionName = %s\n", functionName.Buffer());
            if (ret) {
                ret = functionsDatabase.MoveRelative("Signals");
            }
            bool exists = false;
            if (ret) {
                exists = functionsDatabase.MoveRelative(signalDirection);
            }
            if (exists) {
                uint32 numberOfSignals = 0u;
                if (ret) {
                    numberOfSignals = functionsDatabase.GetNumberOfChildren();
                }
                uint32 s;
                //For every signal in this function
                for (s = 0u; (s < numberOfSignals) && (ret); s++) {
                    StreamString signalId;
                    signalId = functionsDatabase.GetChildName(s);
                    ret = functionsDatabase.MoveRelative(signalId.Buffer());
                    StreamString signalName;
                    if (ret) {
                        //Expand the aliasName in order to write the correct name in the dataSourcesDatabase
                        StreamString aliasName;
                        if (!functionsDatabase.Read("Alias", aliasName)) {
                            aliasName = "";
                        }
                        if (aliasName.Size() > 0u) {
                            signalName = aliasName;
                        }
                        else {
                            ret = functionsDatabase.Read("QualifiedName", signalName);
                        }
                    }
                    //Complement the properties with the information from the DataSource
                    StreamString dataSourceName;
                    if (ret) {
                        ret = functionsDatabase.Read("DataSource", dataSourceName);
                    }
                    if (ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "DataSource was not defined for signal: %s in %s", signalName.Buffer(),
                                                functionName.Buffer())
                    }
                    //Move to the DataSource
                    StreamString dataSourceNumber;
                    if (ret) {
                        ret = FindDataSourceNumber(dataSourceName, dataSourceNumber);
                    }
                    if (ret) {
                        //Look for the signal in the dataSourcesDatabase
                        ret = dataSourcesDatabase.MoveAbsolute("Data");
                    }
                    if (ret) {
                        ret = dataSourcesDatabase.MoveRelative(dataSourceNumber.Buffer());
                    }
                    if (ret) {
                        ret = dataSourcesDatabase.MoveRelative("Signals");
                    }
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "DataSource %s not found for signal: %s in %s", dataSourceName.Buffer(),
                                                signalName.Buffer(), functionName.Buffer())
                    }
                    if (ret) {
                        ret = ResolveFunctionSignal(signalName, functionName, dataSourceName, numberOfSignals);
                    }
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Signal %s not found in %s", signalName.Buffer(), dataSourceName.Buffer())
                    }
                    if (ret) {
                        //Move to next Signal in this signalDirection
                        ret = functionsDatabase.MoveToAncestor(1u);
                    }
                }

                //Delete any "Empty" definitions, i.e. definition that did not had the Type defined and that
                //were completed with an introspection
                numberOfSignals = functionsDatabase.GetNumberOfChildren();
                for (s = 0u; (s < numberOfSignals) && (ret); s++) {
                    StreamString signalId;
                    signalId = functionsDatabase.GetChildName(s);
                    ret = functionsDatabase.MoveRelative(signalId.Buffer());
                    uint32 deleteSignal;
                    if (!functionsDatabase.Read("Delete", deleteSignal)) {
                        deleteSignal = 0u;
                    }
                    if (deleteSignal == 1u) {
                        ret = functionsDatabase.MoveToAncestor(1u);
                        if (ret) {
                            ret = functionsDatabase.Delete(signalId.Buffer());
                        }
                        if (ret) {
                            numberOfSignals = functionsDatabase.GetNumberOfChildren();
                            s = 0u;
                        }
                    }
                    else {
                        if (ret) {
                            //Move to next Signal in this signalDirection
                            ret = functionsDatabase.MoveToAncestor(1u);
                        }
                    }
                }

                if (ret) {
                    //Move to Signals level
                    ret = functionsDatabase.MoveToAncestor(1u);
                }
            }
            if (ret) {
                //Move to next Function
                ret = functionsDatabase.MoveToAncestor(2u);
            }
        }
    }
    return ret;
}

bool RealTimeApplication::VerifyFunctionSignals(const char *signalDirection) {
    bool ret = functionsDatabase.MoveAbsolute("Functions");
    uint32 numberOfFunctions = 0u;
    StreamString functionName;
    if (ret) {
        numberOfFunctions = functionsDatabase.GetNumberOfChildren();
    }
    if (ret) {
        uint32 i;
//For every function
        for (i = 0; (i < numberOfFunctions) && (ret); i++) {
            StreamString functionNumber = functionsDatabase.GetChildName(i);
            ret = functionsDatabase.MoveRelative(functionNumber.Buffer());
            if (ret) {
                ret = functionsDatabase.Read("QualifiedName", functionName);
            }
            if (ret) {
                ret = functionsDatabase.MoveRelative("Signals");
            }
            bool exists = false;
            if (ret) {
                exists = functionsDatabase.MoveRelative(signalDirection);
            }
            if (exists) {
                uint32 numberOfSignals = 0u;
                if (ret) {
                    numberOfSignals = functionsDatabase.GetNumberOfChildren();
                }
                uint32 s;
                //For every signal in this function
                for (s = 0u; (s < numberOfSignals) && (ret); s++) {
                    StreamString signalId;
                    signalId = functionsDatabase.GetChildName(s);
                    ret = functionsDatabase.MoveRelative(signalId.Buffer());
                    //The QualifiedName must also be known
                    StreamString signalName;
                    if (ret) {
                        ret = functionsDatabase.Read("QualifiedName", signalName);
                    }
                    if (ret) {
                        ret = (signalName.Size() > 0u);
                    }
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "QualifiedName was not defined for signal at position: %s in %s",
                                                signalId.Buffer(), functionName.Buffer())
                    }
                    //At this state the Type must be defined
                    StreamString type;
                    if (ret) {
                        ret = functionsDatabase.Read("Type", type);
                    }
                    if (ret) {
                        ret = (type.Size() > 0u);
                    }
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Type was not defined for signal: %s in %s", signalName.Buffer(),
                                                functionName.Buffer())
                    }
                    uint32 numberOfDimensions = 0u;
                    if (ret) {
                        ret = functionsDatabase.Read("NumberOfDimensions", numberOfDimensions);
                        if (!ret) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "NumberOfDimensions was not defined for signal: %s in %s",
                                                    signalName.Buffer(), functionName.Buffer())
                        }
                    }
                    uint32 numberOfElements = 0u;
                    if (ret) {
                        ret = functionsDatabase.Read("NumberOfDimensions", numberOfElements);
                        if (!ret) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "NumberOfDimensions was not defined for signal: %s in %s",
                                                    signalName.Buffer(), functionName.Buffer())
                        }
                    }
                }
                if (ret) {
                    //Move to next Signal in this signalDirection
                    ret = functionsDatabase.MoveToAncestor(1u);
                }
                if (ret) {
                    //Move to next Function level
                    ret = functionsDatabase.MoveToAncestor(3u);
                }
            }
            else {
                //Move to next Function
                ret = functionsDatabase.MoveToAncestor(1u);
            }
        }
    }
    return ret;
}

bool RealTimeApplication::FindDataSourceNumber(StreamString dataSourceName,
                                               StreamString &dataSourceNumber) {
    bool ret = dataSourcesDatabase.MoveAbsolute("Data");
    if (ret) {
        uint32 numberOfDataSources = dataSourcesDatabase.GetNumberOfChildren();
        uint32 n;
        bool done = false;
        for (n = 0u; (n < numberOfDataSources) && (ret) && (!done); n++) {
            dataSourceNumber = dataSourcesDatabase.GetChildName(n);
            ret = dataSourcesDatabase.MoveRelative(dataSourceNumber.Buffer());
            StreamString qualifiedName;
            if (ret) {
                ret = dataSourcesDatabase.Read("QualifiedName", qualifiedName);
            }
            if (ret) {
                done = (StringHelper::Compare(qualifiedName.Buffer(), dataSourceName.Buffer()) == 0u);
            }
            if (ret) {
                ret = dataSourcesDatabase.MoveToAncestor(1u);
            }
        }
        if (ret) {
            ret = done;
        }
    }
    return ret;
}

bool RealTimeApplication::FindFunctionNumber(StreamString functionName,
                                             StreamString &functionNumber) {
    bool ret = functionsDatabase.MoveAbsolute("Functions");
    if (ret) {
        uint32 numberOfFunctions = functionsDatabase.GetNumberOfChildren();
        uint32 n;
        bool done = false;
        for (n = 0u; (n < numberOfFunctions) && (ret) && (!done); n++) {
            functionNumber = functionsDatabase.GetChildName(n);
            ret = functionsDatabase.MoveRelative(functionNumber.Buffer());
            StreamString qualifiedName;
            if (ret) {
                ret = functionsDatabase.Read("QualifiedName", qualifiedName);
            }
            if (ret) {
                done = (StringHelper::Compare(qualifiedName.Buffer(), functionName.Buffer()) == 0u);
            }
            if (ret) {
                ret = functionsDatabase.MoveToAncestor(1u);
            }
        }
        if (ret) {
            ret = done;
        }
    }
    return ret;
}

bool RealTimeApplication::ResolveDataSources(const char *signalDirection) {

    bool ret = functionsDatabase.MoveAbsolute("Functions");

    if (ret) {
        ret = dataSourcesDatabase.MoveAbsolute("Data");
    }
    if (ret) {
        //Go to each function
        uint32 numberOfFunctions = functionsDatabase.GetNumberOfChildren();
        uint32 i;
        for (i = 0u; (i < numberOfFunctions) && (ret); i++) {
            StreamString functionNumber = functionsDatabase.GetChildName(i);
            ret = functionsDatabase.MoveRelative(functionNumber.Buffer());
            StreamString functionName;
            if (ret) {
                ret = functionsDatabase.Read("QualifiedName", functionName);
            }
            if (ret) {
                ret = functionsDatabase.MoveRelative("Signals");
            }
            bool exists = false;
            if (ret) {
                exists = functionsDatabase.MoveRelative(signalDirection);
            }
            if (exists) {
                //...and for each signal...
                uint32 numberOfSignals = functionsDatabase.GetNumberOfChildren();
                uint32 j;
                for (j = 0u; (j < numberOfSignals) && (ret); j++) {
                    StreamString signalId = functionsDatabase.GetChildName(j);
                    ret = functionsDatabase.MoveRelative(signalId.Buffer());
                    //...extract the DataSource name...
                    StreamString dataSourceName;
                    if (ret) {
                        if (!functionsDatabase.Read("DataSource", dataSourceName)) {
                            dataSourceName = defaultDataSourceName;
                        }
                    }
                    //Move to the DataSource
                    StreamString dataSourceNumber;
                    if (ret) {
                        ret = FindDataSourceNumber(dataSourceName, dataSourceNumber);
                    }
                    if (ret) {
                        ret = dataSourcesDatabase.MoveAbsolute("Data");
                    }
                    if (ret) {
                        ret = dataSourcesDatabase.MoveRelative(dataSourceNumber.Buffer());
                    }
                    if (ret) {
                        if (!dataSourcesDatabase.MoveRelative("Signals")) {
                            ret = dataSourcesDatabase.CreateRelative("Signals");
                        }
                    }
                    if (ret) {
                        //Add the signal to the Data.dataSourceName node (iff the Type is defined)
                        StreamString type;
                        if (functionsDatabase.Read("Type", type)) {
                            ret = AddSignalToDataSource(functionName, dataSourceName);
                        }
                    }
                    if (ret) {
                        //Move to the next Signal
                        ret = functionsDatabase.MoveToAncestor(1u);
                    }
                }
                //Move to the Signals level
                if (ret) {
                    ret = functionsDatabase.MoveToAncestor(1u);
                }
            }
            if (ret) {
                //Move to the Functions level
                ret = functionsDatabase.MoveToAncestor(2u);
            }

        }
    }

    return ret;
}

bool RealTimeApplication::ResolveStates() {
    bool ret = true;
    ReferenceContainer statesFound;
//Look for all the RealTimeStates
    ReferenceContainerFilterReferencesTemplate<RealTimeState> stateFilter(-1, ReferenceContainerFilterMode::RECURSIVE);

    Find(statesFound, stateFilter);
    uint32 numberOfStates = statesFound.Size();
    uint32 i;
//Loop on each state...
    for (i = 0u; (i < numberOfStates) && (ret); i++) {
        ReferenceT<RealTimeState> state = statesFound.Get(i);
        const char8 * stateName = state->GetName();
//Look for a ReferenceContainer named Threads
        uint32 j;
        bool found = false;
        ReferenceT<ReferenceContainer> threadContainer;
        for (j = 0; (j < Size()) && (!found); j++) {
            threadContainer = state->Get(j);
            if (threadContainer.IsValid()) {
                found = (StringHelper::Compare("Threads", threadContainer->GetName()) == 0u);
            }
        }
        ret = found;
        if (ret) {
            uint32 numberOfThreads = threadContainer->Size();
            //...get the threads which belong to this state...
            for (j = 0; (j < numberOfThreads) && (ret); j++) {
                ReferenceT<RealTimeThread> thread = threadContainer->Get(j);
                ret = thread.IsValid();
                if (ret) {
                    ret = thread->ConfigureArchitecture();
                }
                else {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Found an invalid RealTimeThread in RealTimeState %s", stateName)
                }
                if (ret) {
                    const char8 * threadName = thread->GetName();
                    ReferenceT<GAM> *gams = thread->GetGAMs();
                    uint32 numberOfGAMs = thread->GetNumberOfGAMs();
                    uint32 z;
                    //...get the GAMs that are executed by this thread....
                    for (z = 0; (z < numberOfGAMs) && (ret); z++) {
                        ReferenceT<GAM> gam = gams[z];
                        StreamString qualifiedName = "";
                        ReferenceContainerFilterReferences qualifiedNameFilter(1, ReferenceContainerFilterMode::PATH, gam);
                        ReferenceContainer result;
                        Find(result, qualifiedNameFilter);
                        //Store the fully qualified name (Functions in the sub-levels of the tree might have the same name)
                        uint32 r;
                        uint32 pathLength = result.Size();
                        //r == 0 => Functions
                        for (r = 1u; (r < pathLength); r++) {
                            StreamString name = result.Get(r)->GetName();
                            if (r > 1u) {
                                qualifiedName.Printf(".", voidAnyType);
                            }
                            qualifiedName.Printf("%s", name.Buffer());
                        }
                        StreamString functionNumber;
                        //Look for the FunctionNumber which has this qualified name
                        ret = FindFunctionNumber(qualifiedName, functionNumber);
                        if (ret) {
                            ret = functionsDatabase.MoveAbsolute("Functions");
                        }
                        if (ret) {
                            ret = functionsDatabase.MoveRelative(functionNumber.Buffer());
                        }
                        if (ret) {
                            if (!functionsDatabase.MoveRelative("States")) {
                                ret = functionsDatabase.CreateRelative("States");
                            }
                        }
                        if (ret) {
                            ret = functionsDatabase.Write(stateName, threadName);
                        }
                    }
                }
            }
        }
        else {
            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "No Threads ReferenceContainer found in RealTimeState %s", stateName)
        }
    }
    return ret;
}

bool RealTimeApplication::InitialiseSignalsDatabase() {
    bool ret = functionsDatabase.CreateAbsolute("Functions");
    if (ret) {
        ReferenceContainer gamsFound;
//Look for all the GAMs
        ReferenceContainerFilterReferencesTemplate<GAM> gamFilter(-1, ReferenceContainerFilterMode::RECURSIVE);

        Find(gamsFound, gamFilter);
        uint32 numberOfGAMs = gamsFound.Size();
        uint32 i = 0;
        for (i = 0; (i < numberOfGAMs) && (ret); i++) {
            ReferenceT<GAM> gam = gamsFound.Get(i);
            StreamString qualifiedName = "";
            ReferenceContainerFilterReferences qualifiedNameFilter(1, ReferenceContainerFilterMode::PATH, gam);
            ReferenceContainer result;
            Find(result, qualifiedNameFilter);
            //Store the fully qualified name (Functions in the sub-levels of the tree might have the same name)
            uint32 r;
            uint32 pathLength = result.Size();
            //r == 0 => Functions
            for (r = 1u; (r < pathLength); r++) {
                StreamString name = result.Get(r)->GetName();
                if (r > 1u) {
                    qualifiedName.Printf(".", voidAnyType);
                }
                qualifiedName.Printf("%s", name.Buffer());
            }
            //Having the fully qualified name add a new node to the Function node, where the name of the node is the index of the
            //Function (GAM) and the fully qualified name is stored as a property.
            StreamString functionN;
            functionN.Printf("%d", i);
            ret = functionsDatabase.CreateRelative(functionN.Buffer());
            if (ret) {
                functionsDatabase.Write("QualifiedName", qualifiedName.Buffer());
            }
            if (ret) {
                //Declare all the signals with which this GAM interacts
                ret = gam->AddSignals(functionsDatabase);
            }
            if (ret) {
                //Move to the next GAM
                ret = functionsDatabase.MoveToAncestor(1);
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Failed to AddSignals for %s", gam->GetName())
            }
        }
    }

    if (ret) {
        ret = functionsDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.CreateAbsolute("Data");
    }
    if (ret) {
        ReferenceContainer dataSourcesFound;
//Look for all the DataSources
        ReferenceContainerFilterReferencesTemplate<DataSourceI> dataSourceFilter(-1, ReferenceContainerFilterMode::RECURSIVE);

        Find(dataSourcesFound, dataSourceFilter);
        uint32 numberOfDataSources = dataSourcesFound.Size();
        uint32 i = 0;
        for (i = 0; (i < numberOfDataSources) && (ret); i++) {
            ReferenceT<DataSourceI> dataSource = dataSourcesFound.Get(i);
            StreamString qualifiedName = "";
            ReferenceContainerFilterReferences qualifiedNameFilter(1, ReferenceContainerFilterMode::PATH, dataSource);
            ReferenceContainer result;
            Find(result, qualifiedNameFilter);
            //Store the fully qualified name (DataSources in sub-levels of the tree might have the same name)
            uint32 r;
            uint32 pathLength = result.Size();
            for (r = 1u; (r < pathLength); r++) {
                StreamString name = result.Get(r)->GetName();
                if (r > 1u) {
                    qualifiedName.Printf(".", voidAnyType);
                }
                qualifiedName.Printf("%s", name.Buffer());
            }
            //Having the fully qualified name add a new node to the Data node, where the name of the node is the index of the
            //DataSource and the fully qualified name is stored as a property.
            StreamString dataSourceN;
            dataSourceN.Printf("%d", i);
            if (ret) {
                ret = dataSourcesDatabase.CreateRelative(dataSourceN.Buffer());
            }
            if (ret) {
                dataSourcesDatabase.Write("QualifiedName", qualifiedName.Buffer());
            }
            if (ret) {
                //Declare all the signals with which this GAM interacts
                ret = dataSource->AddSignals(dataSourcesDatabase);
            }
            if (ret) {
                //Move to the next DataSource
                ret = dataSourcesDatabase.MoveToAncestor(1);
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Failed to AddSignals for %s", dataSource->GetName())
            }
        }
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }

    return ret;
}

bool RealTimeApplication::FlattenSignal(ConfigurationDatabase &signalDatabase,
                                        StreamString signalName,
                                        ConfigurationDatabase &resolvedSignal,
                                        uint32 &signalNumber,
                                        bool forceWriteDataSource) {
    bool ret = true;
    bool signalTypeDefined = false;

    StreamString signalType;
    signalTypeDefined = signalDatabase.Read("Type", signalType);
    if (signalTypeDefined) {
        signalTypeDefined = (signalType.Size() > 0u);
    }
    //Check if it is a pure node (i.e. a namespace which contains other signals)
    bool isNode = false;
    if (!signalTypeDefined) {
        ret = true;
        uint32 numberOfElements = signalDatabase.GetNumberOfChildren();
        uint32 n;
        for (n = 0; (n < numberOfElements) && (!isNode); n++) {
            StreamString elementName = signalDatabase.GetChildName(n);
            //If this element is a node then recurse
            if (signalDatabase.GetType(elementName.Buffer()).GetTypeDescriptor() == VoidType) {
                isNode = true;
                StreamString signalFullName;
                signalFullName.Printf("%s.%s", signalName.Buffer(), elementName.Buffer());
                ret = FlattenSignal(signalDatabase, signalFullName.Buffer(), resolvedSignal, signalNumber, forceWriteDataSource);
            }
        }
    }
    if (!isNode) {
        bool isStructuredData = false;
        StreamString dataSourceName;
        if (forceWriteDataSource) {
            if (!signalDatabase.Read("DataSource", dataSourceName)) {
                ret = (defaultDataSourceName.Size() > 0u);
                if (ret) {
                    dataSourceName = defaultDataSourceName;
                    ret = signalDatabase.Write("DataSource", dataSourceName);
                }
                else {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "DataSource not specified for %s and DefaultDataSource not specified",
                                            signalName.Buffer())
                }
            }
        }
        if (signalTypeDefined) {
            TypeDescriptor signalTypeDescriptor = TypeDescriptor::GetTypeDescriptorFromTypeName(signalType.Buffer());
            //...check if it is a BasicType. If it is not, discombobulate it to its BasicTypes
            if (signalTypeDescriptor == InvalidType) {
                if (ret) {
                    isStructuredData = true;
                    StreamString alias;
                    if (!signalDatabase.Read("Alias", alias)) {
                        alias = "";
                    }

                    AnyType ranges = signalDatabase.GetType("Ranges");
                    ret = SignalIntrospectionToStructuredData(signalType, signalName, alias, dataSourceName, ranges, resolvedSignal, signalNumber);
                }
                if (ret) {
                    //The original structured type is no longer needed
                    ret = signalDatabase.Delete("Type");
                }
            }
        }
        //BasicType. Write all the known signal properties
        if (!isStructuredData) {
            StreamString property;
            StreamString signalNumberStr;
            signalNumberStr.Printf("Signals.%d", signalNumber);
            ret = resolvedSignal.CreateAbsolute(signalNumberStr.Buffer());
            if (ret) {
                ret = resolvedSignal.Write("QualifiedName", signalName);
            }
            //Loop through all properties. If the property is not defined, get it from the DataSource
            const char8 *properties[] = { "Type", "NumberOfDimensions", "NumberOfElements", "Alias", "Ranges", "DataSource", NULL_PTR(char8 *) };
            uint32 p = 0u;
            while ((properties[p] != NULL_PTR(char8 *)) && (ret)) {
                AnyType element = signalDatabase.GetType(properties[p]);
                if (element.GetTypeDescriptor() != VoidType) {
                    ret = resolvedSignal.Write(properties[p], element);
                }
                p++;
            }
            signalNumber++;
            //Move to the next Signal
            resolvedSignal.MoveToAncestor(1);
        }
    }
    return ret;
}

bool RealTimeApplication::FlattenSignalsDatabase(ConfigurationDatabase &signalDatabase,
                                                 const char8 *signalDirection) {
    bool isDataSource = (&signalDatabase == &dataSourcesDatabase);
    uint32 signalNumber;
    bool ret = true;
    //A signal container can be either a GAM or a DataSource
    uint32 numberOfSignalContainers = signalDatabase.GetNumberOfChildren();
    uint32 i;
    //For each signal container...
    for (i = 0; (i < numberOfSignalContainers) && (ret); i++) {
        const char8 *containerId = signalDatabase.GetChildName(i);
        ret = signalDatabase.MoveRelative(containerId);
        if (ret) {
            ret = signalDatabase.MoveRelative("Signals");
        }
        bool exists = ret;
        if (ret) {
            if (!isDataSource) {
                //...move to Signals.InputSignals, Signals.OutputSignals...
                exists = signalDatabase.MoveRelative(signalDirection);
            }
        }
        if (exists) {
            signalNumber = 0u;
            ConfigurationDatabase signalLocalDatabase;
            //Store all the flatten signals in a temporary tree with a root named Signals
            ret = signalLocalDatabase.CreateAbsolute("Signals");

            if (ret) {
                uint32 numberOfSignals = signalDatabase.GetNumberOfChildren();
                uint32 j = 0u;
                //...then for each signal...
                for (j = 0; (j < numberOfSignals) && (ret); j++) {
                    const char8 *signalName = signalDatabase.GetChildName(j);
                    ret = signalDatabase.MoveRelative(signalName);
                    if (ret) {
                        //resolve the full signal properties (recursing if need to signal namespace and introspection)
                        ret = FlattenSignal(signalDatabase, signalName, signalLocalDatabase, signalNumber, !isDataSource);
                    }
                    if (ret) {
                        //Move to the next Signal
                        ret = signalDatabase.MoveToAncestor(1u);
                    }
                }
            }
            if (ret) {
                ret = signalDatabase.MoveToAncestor(1u);
            }
            if (ret) {
                ret = signalDatabase.Delete(signalDirection);
            }
            if (ret) {
                ret = signalDatabase.CreateRelative(signalDirection);
            }
            if (ret) {
                ret = signalLocalDatabase.MoveAbsolute("Signals");
            }
            if (ret) {
                ret = signalLocalDatabase.Copy(signalDatabase);
            }
            if (ret) {
                //Move to the Signals level
                if (!isDataSource) {
                    ret = signalDatabase.MoveToAncestor(1u);
                }
            }
        }
        if (ret) {
            //Move to the next function
            ret = signalDatabase.MoveToAncestor(2u);
        }
    }

    return ret;
}

bool RealTimeApplication::FindSignalName(StreamString signalName,
                                         ConfigurationDatabase &database) {
    bool ret = true;
    bool found = false;
    uint32 numberOfSignalsInDatabase = database.GetNumberOfChildren();
    uint32 j;
    for (j = 0u; (j < numberOfSignalsInDatabase) && (ret) && (!found); j++) {
        StreamString nodeId = database.GetChildName(j);
        ret = database.MoveRelative(nodeId.Buffer());
        if (ret) {
            database.MoveRelative(nodeId.Buffer());
            StreamString databaseSignalName;
            ret = database.Read("QualifiedName", databaseSignalName);
            if (ret) {
                found = (StringHelper::Compare(signalName.Buffer(), databaseSignalName.Buffer()) == 0u);
            }
        }
        if (ret) {
            //Move to the next signal
            if (!found) {
                ret = database.MoveToAncestor(1u);
            }
            //If it was found leave the tree pointing to the signal
        }
    }
    if (ret) {
        ret = found;
    }
    return ret;
}

bool RealTimeApplication::ResolveConsumersAndProducers(bool consumers) {
    const char8 *signalDirection = "InputSignals";
    if (!consumers) {
        signalDirection = "OutputSignals";
    }
    const char8 *operationType = "Consumers";
    if (!consumers) {
        operationType = "Producers";
    }

    bool ret = functionsDatabase.MoveAbsolute("Functions");
    if (ret) {
        //For all the functions
        uint32 numberOfFunctions = functionsDatabase.GetNumberOfChildren();
        uint32 n;
        for (n = 0u; (n < numberOfFunctions) && (ret); n++) {
            //Move to the next function
            StreamString functionId;
            StreamString functionName;
            functionId.Printf("%d", n);
            ret = functionsDatabase.MoveRelative(functionId.Buffer());
            if (ret) {
                ret = functionsDatabase.Read("QualifiedName", functionName);
            }
            //For all the states
            if ((ret) && (functionsDatabase.MoveRelative("States"))) {
                uint32 numberOfStates = functionsDatabase.GetNumberOfChildren();
                if (numberOfStates > 0u) {
                    Vector<StreamString> states(numberOfStates);
                    uint32 s;
                    for (s = 0u; (s < numberOfStates) && (ret); s++) {
                        StreamString stateName = functionsDatabase.GetChildName(s);
                        states[s] = stateName;
                    }
                    if (ret) {
                        //Move to the function level
                        ret = functionsDatabase.MoveToAncestor(1u);
                    }
                    if (ret) {
                        ret = functionsDatabase.MoveRelative("Signals");
                    }
                    bool exists = false;
                    if (ret) {
                        exists = functionsDatabase.MoveRelative(signalDirection);
                    }
                    if (exists) {
                        uint32 numberOfSignals = functionsDatabase.GetNumberOfChildren();
                        //For all the signals in the function
                        uint32 i;
                        for (i = 0u; (i < numberOfSignals) && (ret); i++) {
                            StreamString signalId = functionsDatabase.GetChildName(i);
                            if (ret) {
                                ret = functionsDatabase.MoveRelative(signalId.Buffer());
                            }
                            if (ret) {
                                //Find the signal in the target DataSource
                                StreamString signalName;
                                StreamString alias;
                                StreamString dataSourceName;
                                ret = functionsDatabase.Read("QualifiedName", signalName);
                                if (ret) {
                                    if (functionsDatabase.Read("Alias", alias)) {
                                        signalName = alias;
                                    }
                                }
                                printf("%s\n", signalName.Buffer());
                                if (ret) {
                                    ret = functionsDatabase.Read("DataSource", dataSourceName);
                                }
                                //Move to the DataSource
                                StreamString dataSourceNumber;
                                if (ret) {
                                    ret = FindDataSourceNumber(dataSourceName, dataSourceNumber);
                                }
                                if (ret) {
                                    ret = dataSourcesDatabase.MoveAbsolute("Data");
                                }
                                if (ret) {
                                    ret = dataSourcesDatabase.MoveRelative(dataSourceNumber.Buffer());
                                }
                                if (ret) {
                                    ret = dataSourcesDatabase.MoveRelative("Signals");
                                }
                                if (ret) {
                                    //Try to find the signal in the DataSource
                                    ret = FindSignalName(signalName, dataSourcesDatabase);
                                }
                                if (ret) {
                                    if (!dataSourcesDatabase.MoveRelative("States")) {
                                        ret = dataSourcesDatabase.CreateRelative("States");
                                    }
                                }
                                //For all the states
                                for (s = 0u; (s < numberOfStates) && (ret); s++) {
                                    if (!dataSourcesDatabase.MoveRelative(states[s].Buffer())) {
                                        ret = dataSourcesDatabase.CreateRelative(states[s].Buffer());
                                    }
                                    if (ret) {
                                        //Check if the array Consumers (or Producers) already exists
                                        AnyType existentArray = dataSourcesDatabase.GetType(operationType);
                                        StreamString *newArray = NULL_PTR(StreamString *);
                                        uint32 numberOfExistentElements = 0u;
                                        if (existentArray.GetDataPointer() != NULL_PTR(void *)) {
                                            numberOfExistentElements = existentArray.GetNumberOfElements(0u);
                                            newArray = new StreamString[numberOfExistentElements + 1u];
                                            Vector<StreamString> newVector(newArray, numberOfExistentElements);
                                            ret = (dataSourcesDatabase.Read(operationType, newVector));
                                        }
                                        else {
                                            newArray = new StreamString[1u];
                                        }
                                        newArray[numberOfExistentElements] = functionName.Buffer();
                                        Vector<StreamString> newVector(newArray, numberOfExistentElements + 1u);
                                        if (existentArray.GetDataPointer() != NULL_PTR(void *)) {
                                            ret = dataSourcesDatabase.Delete(operationType);
                                        }
                                        if (ret) {
                                            ret = (dataSourcesDatabase.Write(operationType, newVector));
                                        }
                                        delete[] newArray;
                                    }
                                    //Move to the next state
                                    if (ret) {
                                        ret = dataSourcesDatabase.MoveToAncestor(1u);
                                    }
                                }
                                //Move to the States level
                                if (ret) {
                                    ret = dataSourcesDatabase.MoveToAncestor(1u);
                                }
                            }
                            //Move to the next signal
                            if (ret) {
                                ret = functionsDatabase.MoveToAncestor(1u);
                            }
                        }
                        if (ret) {
                            //Move to the Signals level
                            ret = functionsDatabase.MoveToAncestor(1u);
                        }
                    }
                    if (ret) {
                        //Move to the function level
                        ret = functionsDatabase.MoveToAncestor(1u);
                    }
                }
            }
            if (ret) {
                //Move to the next function
                ret = functionsDatabase.MoveToAncestor(1u);
            }
        }
    }

    return ret;
}

bool RealTimeApplication::ResolveFunctionsMemory(const char *signalDirection) {
    bool ret = functionsDatabase.MoveAbsolute("Functions");
    uint32 numberOfFunctions = 0u;
    if (ret) {
        numberOfFunctions = functionsDatabase.GetNumberOfChildren();
    }
    if (ret) {
        uint32 i;
        //For every function
        for (i = 0; (i < numberOfFunctions) && (ret); i++) {
            StreamString functionNumber = functionsDatabase.GetChildName(i);
            ret = functionsDatabase.MoveRelative(functionNumber.Buffer());
            StreamString functionName;
            if (ret) {
                ret = functionsDatabase.Read("QualifiedName", functionName);
            }
            if (ret) {
                ret = functionsDatabase.MoveRelative("Signals");
            }
            bool exists = false;
            if (ret) {
                exists = functionsDatabase.MoveRelative(signalDirection);
            }
            if (exists) {
                uint32 numberOfSignals = 0u;
                if (ret) {
                    numberOfSignals = functionsDatabase.GetNumberOfChildren();
                }
                uint32 s;
                //For every signal in this function
                for (s = 0u; (s < numberOfSignals) && (ret); s++) {
                    TypeDescriptor signalTypeDescriptor;
                    StreamString signalId;
                    StreamString signalName;
                    StreamString alias;
                    StreamString dataSourceName;
                    uint32 numberOfOffsetElements = 0u;
                    uint32 *offsetMatrixBackend = NULL_PTR(uint32 *);
                    uint32 byteSize = 0u;

                    signalId = functionsDatabase.GetChildName(s);
                    ret = functionsDatabase.MoveRelative(signalId.Buffer());
                    if (ret) {
                        ret = functionsDatabase.Read("QualifiedName", signalName);
                    }
                    if (ret) {
                        if (functionsDatabase.Read("Alias", alias)) {
                            signalName = alias;
                        }
                    }
                    if (ret) {
                        ret = functionsDatabase.Read("DataSource", dataSourceName);
                    }
                    if (ret) {
                        ret = functionsDatabase.Read("ByteSize", byteSize);
                    }

                    AnyType existentArray = functionsDatabase.GetType("ByteOffset");
                    if (existentArray.GetDataPointer() != NULL_PTR(void *)) {
                        numberOfOffsetElements = existentArray.GetNumberOfElements(1u);
                        offsetMatrixBackend = new uint32[numberOfOffsetElements * 2u];
                        Matrix<uint32> offsetMat(offsetMatrixBackend, numberOfOffsetElements, 2u);
                        ret = functionsDatabase.Read("ByteOffset", offsetMat);
                    }
                    //Move to the function level
                    if (ret) {
                        ret = functionsDatabase.MoveToAncestor(3u);
                    }
                    if (!functionsDatabase.MoveRelative("Memory")) {
                        ret = functionsDatabase.CreateRelative("Memory");
                    }
                    if (!functionsDatabase.MoveRelative(signalDirection)) {
                        ret = functionsDatabase.CreateRelative(signalDirection);
                    }
                    if (ret) {
                        //Check if a DataSource with this name already exists..
                        uint32 numberOfDataSources = functionsDatabase.GetNumberOfChildren();
                        uint32 d;
                        bool found = false;
                        StreamString dataSourceId;
                        for (d = 0u; (d < numberOfDataSources) && (ret) && (!found); d++) {
                            dataSourceId = functionsDatabase.GetChildName(d);
                            ret = functionsDatabase.MoveRelative(dataSourceId.Buffer());
                            StreamString thisDataSourceName;
                            if (ret) {
                                ret = functionsDatabase.Read("DataSource", thisDataSourceName);
                            }
                            if (ret) {
                                found = (StringHelper::Compare(thisDataSourceName.Buffer(), dataSourceName.Buffer()) == 0u);
                            }
                            //Move to the next DataSource
                            if (!found) {
                                functionsDatabase.MoveToAncestor(1u);
                            }
                        }
                        if (ret) {
                            if (!found) {
                                StreamString newDataSourceId;
                                newDataSourceId.Printf("%d", numberOfDataSources);
                                ret = functionsDatabase.CreateRelative(newDataSourceId.Buffer());
                                if (ret) {
                                    ret = functionsDatabase.Write("DataSource", dataSourceName.Buffer());
                                }
                            }
                        }
                        //Store the relevant signals belonging to this DataSource
                        if (!functionsDatabase.MoveRelative("Signals")) {
                            ret = functionsDatabase.CreateRelative("Signals");
                        }
                        uint32 numberOfSignalsInDataSource = functionsDatabase.GetNumberOfChildren();
                        StreamString newSignalId;
                        if (ret) {
                            newSignalId.Printf("%d", numberOfSignalsInDataSource);
                        }
                        if (ret) {
                            ret = functionsDatabase.CreateRelative(newSignalId.Buffer());
                        }
                        if (ret) {
                            ret = functionsDatabase.Write("QualifiedName", signalName.Buffer());
                        }
                        if (ret) {
                            if (offsetMatrixBackend != NULL_PTR(void *)) {
                                Matrix<uint32> offsetMat(offsetMatrixBackend, numberOfOffsetElements, 2u);
                                ret = functionsDatabase.Write("ByteOffset", offsetMat);
                                delete[] offsetMatrixBackend;
                            }
                        }
                        if (ret) {
                            ret = functionsDatabase.MoveToAncestor(2u);
                        }
                    }
                    if (ret) {
                        uint32 existentByteSize = 0u;
                        if (functionsDatabase.Read("ByteSize", existentByteSize)) {
                            ret = functionsDatabase.Delete("ByteSize");
                        }
                        if (ret) {
                            existentByteSize += byteSize;
                            ret = functionsDatabase.Write("ByteSize", existentByteSize);
                        }
                    }
                    //Move to this Function Level
                    if (ret) {
                        ret = functionsDatabase.MoveToAncestor(3u);
                    }
                    if (ret) {
                        ret = functionsDatabase.MoveRelative("Signals");
                    }
                    if (ret) {
                        ret = functionsDatabase.MoveRelative(signalDirection);
                    }
                }
                if (ret) {
                    //Move to Signals level
                    ret = functionsDatabase.MoveToAncestor(1u);
                }
            }
            if (ret) {
                //Move to next Function
                ret = functionsDatabase.MoveToAncestor(2u);
            }

        }
    }
    return ret;

}

bool RealTimeApplication::AllocateFunctionsMemory(const char *signalDirection) {
    bool ret = functionsDatabase.MoveAbsolute("Functions");
    uint32 numberOfFunctions = 0u;
    if (ret) {
        numberOfFunctions = functionsDatabase.GetNumberOfChildren();
    }
    if (ret) {
        uint32 i;
        //For every function
        for (i = 0; (i < numberOfFunctions) && (ret); i++) {
            StreamString functionNumber = functionsDatabase.GetChildName(i);
            ret = functionsDatabase.MoveRelative(functionNumber.Buffer());
            StreamString functionName;
            if (ret) {
                ret = functionsDatabase.Read("QualifiedName", functionName);
            }
            if (ret) {
                ret = functionsDatabase.MoveRelative("Memory");
            }
            bool exists = false;
            if (ret) {
                exists = functionsDatabase.MoveRelative(signalDirection);
            }
            if (exists) {
                uint32 numberOfDataSources = 0u;
                if (ret) {
                    numberOfDataSources = functionsDatabase.GetNumberOfChildren();
                }
                uint32 d;
                //For every DataSource in this function
                for (d = 0u; (d < numberOfDataSources) && (ret); d++) {
                    uint32 byteSize = 0u;
                    StreamString dataSourceName;
                    StreamString dataSourceId = functionsDatabase.GetChildName(d);
                    ret = functionsDatabase.MoveRelative(dataSourceId.Buffer());
                    if (ret) {
                        ret = functionsDatabase.Read("DataSource", dataSourceName);
                    }
                    if (ret) {
                        ret = functionsDatabase.Read("ByteSize", byteSize);
                    }
                    //Find the DataSource
                    StreamString dataSourceIdInDataSourceDatabase;
                    if (ret) {
                        ret = FindDataSourceNumber(dataSourceName.Buffer(), dataSourceIdInDataSourceDatabase);
                    }
                    if (ret) {
                        ret = dataSourcesDatabase.MoveRelative(dataSourceIdInDataSourceDatabase.Buffer());
                    }
                    if (!dataSourcesDatabase.MoveRelative("Functions")) {
                        ret = dataSourcesDatabase.CreateRelative("Functions");
                    }
                    if (ret) {
                        //Check if this function name already exists
                        uint32 n;
                        uint32 numberOfFunctions = dataSourcesDatabase.GetNumberOfChildren();
                        StreamString functionId;
                        bool found = false;
                        for (n = 0u; (n < numberOfFunctions) && (ret) && (!found); n++) {
                            functionId = dataSourcesDatabase.GetChildName(n);
                            ret = dataSourcesDatabase.MoveRelative(functionId.Buffer());
                            StreamString thisFunctionName;
                            if (ret) {
                                ret = dataSourcesDatabase.Read("QualifiedName", thisFunctionName);
                            }
                            if (ret) {
                                found = (StringHelper::Compare(thisFunctionName.Buffer(), functionName.Buffer()) == 0u);
                            }
                            //Move to the next DataSource
                            if (!found) {
                                dataSourcesDatabase.MoveToAncestor(1u);
                            }
                        }
                        if (ret) {
                            if (!found) {
                                StreamString newFunctionId;
                                newFunctionId.Printf("%d", numberOfFunctions);
                                ret = dataSourcesDatabase.CreateRelative(newFunctionId.Buffer());
                                if (ret) {
                                    ret = dataSourcesDatabase.Write("QualifiedName", functionName.Buffer());
                                }
                            }
                        }
                    }
                    if (ret) {
                        ret = dataSourcesDatabase.Write("ByteSize", byteSize);
                    }
                    if (ret) {
                        if (!dataSourcesDatabase.MoveRelative("Signals")) {
                            ret = dataSourcesDatabase.CreateRelative("Signals");
                        }
                    }
                    if (ret) {
                        ret = functionsDatabase.MoveRelative("Signals");
                    }
                    if (ret) {
                        uint32 numberOfSignalsInFunction = functionsDatabase.GetNumberOfChildren();
                        uint32 numberOfSignalsAlreadyInDataSource = dataSourcesDatabase.GetNumberOfChildren();
                        uint32 n;
                        for (n = 0u; (n < numberOfSignalsInFunction) && (ret); n++) {
                            StreamString functionSignalId = functionsDatabase.GetChildName(n);
                            ret = functionsDatabase.MoveRelative(functionSignalId.Buffer());
                            StreamString newSignalId;
                            if (ret) {
                                newSignalId.Printf("%d", (n + numberOfSignalsAlreadyInDataSource));
                                ret = dataSourcesDatabase.CreateRelative(newSignalId.Buffer());
                            }
                            if (ret) {
                                ret = functionsDatabase.Copy(dataSourcesDatabase);
                            }

                            //Move to the next signal
                            if (ret) {
                                ret = functionsDatabase.MoveToAncestor(1u);
                            }
                            if (ret) {
                                ret = dataSourcesDatabase.MoveToAncestor(1u);
                            }
                        }
                    }
                    //Move back to the DataSource level
                    if (ret) {
                        ret = functionsDatabase.MoveToAncestor(1u);
                    }
                    //Move to the next DataSource
                    if (ret) {
                        ret = functionsDatabase.MoveToAncestor(1u);
                    }
                }
                if (ret) {
                    //Move to Memory level
                    ret = functionsDatabase.MoveToAncestor(1u);
                }
            }
            if (ret) {
                //Move to next Function
                ret = functionsDatabase.MoveToAncestor(2u);
            }

        }
    }
    return ret;

}

static void PrintDatabases(RealTimeApplicationConfigurationBuilder &rtAppBuilder) {
    static uint32 i = 0u;
    static uint32 j = 1u;
    ConfigurationDatabase fdb;
    ConfigurationDatabase dsdb;
    rtAppBuilder.Copy(fdb, dsdb);

    StreamString temp;
    temp.Printf("[%d]\n%s%s[%d]\n", i, fdb, dsdb, j);
    i += 2;
    j += 2;
    temp.Seek(0);
    printf("%s", temp.Buffer());
}

bool RealTimeApplication::ConfigureApplication() {
    RealTimeApplicationConfigurationBuilder rtAppBuilder(this, "DDB1");
    bool ret = rtAppBuilder.InitialiseSignalsDatabase();
    if (ret) {
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.FlattenSignalsDatabases();
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.ResolveDataSources();
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.VerifyDataSourcesSignals();
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.ResolveFunctionSignals();
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.VerifyFunctionSignals();
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.ResolveStates();
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.ResolveConsumersAndProducers();
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.ResolveFunctionSignalsMemorySize();
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.ResolveFunctionsMemory();
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.AllocateFunctionsMemory();
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.AssignFunctionsMemoryToDataSource();
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.PostConfigureDataSources();
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.PostConfigureFunctions();
    }
    if (ret) {
        ret = rtAppBuilder.AddBrokersToFunctions();
    }
    if (ret) {
        ReferenceContainer dataSourcesFound;
        //Look for all the DataSources
        ReferenceContainerFilterReferencesTemplate<DataSourceI> dataSourceFilter(-1, ReferenceContainerFilterMode::RECURSIVE);
        Find(dataSourcesFound, dataSourceFilter);
        uint32 numberOfDataSources = dataSourcesFound.Size();
        uint32 i;
        for (i = 0u; i < numberOfDataSources; i++) {
            ReferenceT<DataSourceI> dataSource = dataSourcesFound.Get(i);
            printf("[%s]\n", dataSource->GetName());

            uint32 numberOfSignals = dataSource->GetNumberOfSignals();
            printf("Number of signals: %d\n", numberOfSignals);
            uint32 s;
            for (s = 0u; s < numberOfSignals; s++) {
                StreamString signalName;
                dataSource->GetSignalName(s, signalName);
                printf("  Signal: %s\n", signalName.Buffer());
                TypeDescriptor type = dataSource->GetSignalType(s);
                printf("    Type: %s\n", TypeDescriptor::GetTypeNameFromTypeDescriptor(type));
                uint32 n;
                dataSource->GetSignalNumberOfDimensions(s, n);
                printf("    NumberOfDimensions: %d\n", n);
                dataSource->GetSignalNumberElements(s, n);
                printf("    NumberOfElements: %d\n", n);
                dataSource->GetSignalByteSize(s, n);
                printf("    ByteSize: %d\n", n);
                uint32 numberOfStates;
                dataSource->GetSignalNumberOfStates(s, numberOfStates);
                printf("    Number of states: %d\n", numberOfStates);
                for (n = 0u; n < numberOfStates; n++) {
                    StreamString stateName;
                    dataSource->GetSignalStateName(s, n, stateName);
                    printf("      State: %s\n", stateName.Buffer());
                    uint32 numberOfConsumers;
                    dataSource->GetSignalNumberOfConsumers(s, stateName.Buffer(), numberOfConsumers);
                    printf("      Number of consumers: %d\n", numberOfConsumers);
                    uint32 c;
                    for (c = 0u; c < numberOfConsumers; c++) {
                        StreamString consumerName;
                        dataSource->GetSignalConsumerName(s, stateName.Buffer(), c, consumerName);
                        printf("        %s\n", consumerName.Buffer());
                    }
                    uint32 numberOfProducers;
                    dataSource->GetSignalNumberOfProducers(s, stateName.Buffer(), numberOfProducers);
                    printf("      Number of producers: %d\n", numberOfProducers);
                    uint32 p;
                    for (p = 0u; p < numberOfProducers; p++) {
                        StreamString producerName;
                        dataSource->GetSignalProducerName(s, stateName.Buffer(), p, producerName);
                        printf("        %s\n", producerName.Buffer());
                    }
                }
            }

            uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();
            printf("Number of functions: %d\n", numberOfFunctions);
            uint32 f;
            for (f = 0u; f < numberOfFunctions; f++) {
                StreamString functionName;
                dataSource->GetFunctionName(f, functionName);
                printf("  Function: %s\n", functionName.Buffer());
                printf("    InputSignals: \n");
                uint32 byteSize;
                dataSource->GetFunctionSignalsByteSize(InputSignals, f, byteSize);
                printf("      ByteSize: %d\n", byteSize);
                void *address;
                dataSource->GetFunctionSignalsAddress(InputSignals, f, address);
                printf("      Address: %p\n", address);
                uint32 numberOfFunctionSignals = 0u;
                dataSource->GetFunctionNumberOfSignals(InputSignals, f, numberOfFunctionSignals);
                for (s = 0u; s < numberOfFunctionSignals; s++) {
                    StreamString functionSignalName;
                    dataSource->GetFunctionSignalName(InputSignals, f, s, functionSignalName);
                    printf("      Signal: %s\n", functionSignalName.Buffer());
                    uint32 numberOfByteOffsets = 0u;
                    dataSource->GetFunctionSignalNumberOfByteOffsets(InputSignals, f, s, numberOfByteOffsets);
                    uint32 b;
                    for (b = 0u; b < numberOfByteOffsets; b++) {
                        uint32 start;
                        uint32 size;
                        dataSource->GetFunctionSignalByteOffsetInfo(InputSignals, f, s, b, start, size);
                        printf("        Offset[%d]: [%d %d]\n", b, start, size);
                    }
                    uint32 timeCycles;
                    uint32 timeSamples;
                    dataSource->GetFunctionSignalTimeCyclesInfo(InputSignals, f, s, timeCycles, timeSamples);
                    printf("        TimeCyclesSamples: [%d %d]\n", timeCycles, timeSamples);
                }
            }
        }
    }

    if (ret) {
        ReferenceContainer gamsFound;
        ReferenceContainerFilterReferencesTemplate<GAM> gamsFilter(-1, ReferenceContainerFilterMode::RECURSIVE);
        Find(gamsFound, gamsFilter);
        uint32 numberOfGAMs = gamsFound.Size();
        uint32 i;
        for (i = 0u; i < numberOfGAMs; i++) {
            ReferenceT<GAM> gam = gamsFound.Get(i);
            gam->Execute();
        }
        for (i = 0u; i < numberOfGAMs; i++) {
            ReferenceT<GAM> gam = gamsFound.Get(i);
            gam->Execute();
        }
    }
    return ret;
#if 0
//Create the Function and the Data nodes. Add all the signals by querying the GAMs and the DataSourceI
    bool ret = InitialiseSignalsDatabase();
    if (ret) {
        ret = functionsDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }
    StreamString temp;
    temp.Printf("[1]\n%s%s[2]\n", functionsDatabase, dataSourcesDatabase);
    temp.Seek(0);
    printf("%s", temp.Buffer());

//Expand all the introspections inside all the signals in the Function.FUNCTION_NAME.Signals and Data.DATA_SOURCE_NAME.Signals nodes.
    if (ret) {
        ret = functionsDatabase.MoveAbsolute("Functions");
    }
    if (ret) {
        ret = FlattenSignalsDatabase(functionsDatabase, "InputSignals");
    }
    if (ret) {
        ret = functionsDatabase.MoveAbsolute("Functions");
    }
    if (ret) {
        ret = FlattenSignalsDatabase(functionsDatabase, "OutputSignals");
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveAbsolute("Data");
    }
    if (ret) {
        ret = FlattenSignalsDatabase(dataSourcesDatabase, "Signals");
    }
    if (ret) {
        ret = functionsDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }
    temp = "";
    temp.Printf("[3]\n%s%s[4]\n", functionsDatabase, dataSourcesDatabase);
    temp.Seek(0);
    printf("%s", temp.Buffer());

//Add, or check compatibility, for all the Signals from Function.FUNCTION_NAME.Signals in the correct Data.DATA_SOURCE_NAME.
//Verify compatibility issues (i.e. FUNCTION_NAME asking for a given definition (e.g. Type) and DATA_SOURCE_NAME having another definition)
    if (ret) {
        ret = ResolveDataSources("InputSignals");
    }
    if (ret) {
        ret = ResolveDataSources("OutputSignals");
    }

    if (ret) {
        ret = functionsDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }
    temp = "";
    temp.Printf("[5]\n%s%s[6]\n", functionsDatabase, dataSourcesDatabase);
    temp.Seek(0);
    printf("%s", temp.Buffer());

//Verify that for every Signal in every DATA_SOURCE_NAME the Type is defined. Add the default values for NumberOfElements and NumberOfDimensions if needed
    if (ret) {
        ret = VerifyDataSourcesSignals();
    }

    if (ret) {
        ret = functionsDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }
    temp = "";
    temp.Printf("[7]\n%s%s[8]\n", functionsDatabase, dataSourcesDatabase);
    temp.Seek(0);
    printf("%s", temp.Buffer());

//For every Signal in every FUNCTION_NAME: if Type, NumberOfElements or NumberOfDimensions are not defined, try to get this property from the correspondent DATA_SOURCE_NAME (which might have to be guessed!)
    if (ret) {
        ret = ResolveFunctionSignals("InputSignals");
    }
    if (ret) {
        ret = ResolveFunctionSignals("OutputSignals");
    }

    if (ret) {
        ret = functionsDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }
    temp = "";
    temp.Printf("[9]\n%s%s[10]\n", functionsDatabase, dataSourcesDatabase);
    temp.Seek(0);
    printf("%s", temp.Buffer());

//At this stage all signal definition for Functions and DataSources is fully defined (i.e. the properties Type, NumberOfElements and NumberOfDimensions exist).
    if (ret) {
        ret = VerifyFunctionSignals("InputSignals");
    }
    if (ret) {
        ret = VerifyFunctionSignals("OutputSignals");
    }

    if (ret) {
        ret = functionsDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }
    temp = "";
    temp.Printf("[11]\n%s%s[12]\n", functionsDatabase, dataSourcesDatabase);
    temp.Seek(0);
    printf("%s", temp.Buffer());

//Add to Function.FUNCTION_NAME.States all the states where FUNCTION_NAME is executed
    if (ret) {
        ret = ResolveStates();
    }

    if (ret) {
        ret = functionsDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }
    temp = "";
    temp.Printf("[13]\n%s%s[14]\n", functionsDatabase, dataSourcesDatabase);
    temp.Seek(0);
    printf("%s", temp.Buffer());

//Add consumers and producers
    if (ret) {
        ret = ResolveConsumersAndProducers(true);
    }
    if (ret) {
        ret = ResolveConsumersAndProducers(false);
    }

    if (ret) {
        ret = functionsDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }
    temp = "";
    temp.Printf("[15]\n%s%s[16]\n", functionsDatabase, dataSourcesDatabase);
    temp.Seek(0);
    printf("%s", temp.Buffer());

//Compute memory sizes
    if (ret) {
        ret = ResolveFunctionSignalsMemorySize("InputSignals");
    }
    if (ret) {
        ret = ResolveFunctionSignalsMemorySize("OutputSignals");
    }

    if (ret) {
        ret = functionsDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }
    temp = "";
    temp.Printf("[17]\n%s%s[18]\n", functionsDatabase, dataSourcesDatabase);
    temp.Seek(0);
    printf("%s", temp.Buffer());

//Compute the memory required for each GAM in every DataSource
    if (ret) {
        ret = ResolveFunctionsMemory("InputSignals");
    }
    if (ret) {
        ret = ResolveFunctionsMemory("OutputSignals");
    }

    if (ret) {
        ret = functionsDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }
    temp = "";
    temp.Printf("[19]\n%s%s[20]\n", functionsDatabase, dataSourcesDatabase);
    temp.Seek(0);
    printf("%s", temp.Buffer());

    if (ret) {
        ret = functionsDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }

//Write the GAM memory information in each DataSource
    if (ret) {
        ret = AssignFunctionsMemoryToDataSource("InputSignals");
    }
    if (ret) {
        ret = AssignFunctionsMemoryToDataSource("OutputSignals");
    }

    if (ret) {
        ret = functionsDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }
    temp = "";
    temp.Printf("[21]\n%s%s[22]\n", functionsDatabase, dataSourcesDatabase);
    temp.Seek(0);
    printf("%s", temp.Buffer());

    if (ret) {
        ret = functionsDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }

    return ret;
#endif
}

bool RealTimeApplication::Initialise(StructuredDataI & data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (data.MoveRelative("+Data")) {
        if (!data.Read("DefaultDataSource", defaultDataSourceName)) {
            defaultDataSourceName = "";
        }
        data.MoveToAncestor(1u);
    }
    defaultDataSourceName.Seek(0u);
#if 0
    if (ret) {
//TODO Read the name of the first state
// do the PrepareNextState here.

        uint32 numberOfContainers = Size();
        ret = false;
        for (uint32 i = 0u; (i < numberOfContainers) && (!ret); i++) {
            Reference item = Get(i);
            if (item.IsValid()) {
                if (StringHelper::Compare(item->GetName(), "States") == 0) {
                    statesContainer = item;
                    ret = statesContainer.IsValid();
                }
            }
        }
        if (ret) {
            ret = false;
            for (uint32 i = 0u; (i < numberOfContainers) && (!ret); i++) {
                Reference container = Get(i);
                if (container.IsValid()) {
                    if (StringHelper::Compare(container->GetName(), "Data") == 0) {
                        dataSourceContainer = container;
                        ret = dataSourceContainer.IsValid();
                    }
                }
            }
        }
        if (ret) {
            ret = false;
            for (uint32 i = 0u; (i < numberOfContainers) && (!ret); i++) {
                Reference item = Get(i);
                if (item.IsValid()) {
                    if (StringHelper::Compare(item->GetName(), "Functions") == 0) {
                        functionsContainer = item;
                        ret = functionsContainer.IsValid();
                    }
                }
            }
        }
        if (ret) {
            ret = false;
            for (uint32 i = 0u; (i < numberOfContainers) && (!ret); i++) {
                Reference container = Get(i);
                if (container.IsValid()) {
                    if (StringHelper::Compare(container->GetName(), "Scheduler") == 0) {
                        schedulerContainer = container;
                        ret = schedulerContainer.IsValid();
                    }
                }
            }
        }
    }
#endif
    return ret;
}

#if 0
uint8 RealTimeApplication::GetActiveBuffer() const {
    return activeBuffer;
}

ReferenceT<ReferenceContainer> RealTimeApplication::GetDataSourceContainer() {
    return dataSourceContainer;
}
#endif

CLASS_REGISTER(RealTimeApplication, "1.0");
}
