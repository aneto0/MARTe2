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

uint32 RealTimeApplication::index = 1u;

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

static void PrintDatabases(RealTimeApplicationConfigurationBuilder &rtAppBuilder) {
    return;
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
        //TODO REMOVE comment!
        //ret = rtAppBuilder.VerifyConsumersAndProducers();
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
        ret = rtAppBuilder.CalculateFunctionsMemory();
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.AssignFunctionsMemoryToDataSource();
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.AssignBrokersToFunctions();
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.PostConfigureDataSources();
        PrintDatabases(rtAppBuilder);
    }
    if (ret) {
        ret = rtAppBuilder.PostConfigureFunctions();
        PrintDatabases(rtAppBuilder);
    }
    //<<<<<<<<<<<<<<Mine

    if (ret) {
        rtAppBuilder.Copy(functionsDatabase, dataSourcesDatabase);
    }
    //TODO Add brokers to GAMs allocating GAM memory
    if (ret) {
        ret = AllocateGAMMemory();
    }
    if (ret) {
        ret = AllocateDataSourceMemory();
    }
    if (ret) {
        ret = AddBrokersToFunctions();
    }

    if (ret) {
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
    }
    /*   if (ret) {
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
     */

    //<<<<<<<<<<<<<<Mine
    return ret;
#if 0
    //Andre's stuff
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
        RealTimeStateInfo stateInfo;
        stateInfo.currentState = "";
        stateInfo.nextState = "State1";

        for (i = 0u; i < numberOfDataSources; i++) {
            ReferenceT<DataSourceI> dataSource = dataSourcesFound.Get(i);
            dataSource->PrepareNextState(stateInfo);
            printf("[%s]\n", dataSource->GetName());
            dataSource->ChangeState();

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

#endif

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

bool RealTimeApplication::ConfigureApplicationFromExternalSource(StructuredDataI &functionsDatabaseIn,
                                                                 StructuredDataI &dataSourcesDatabaseIn) {
    // no check..someone else did it
    // TODO Standard checks can be done (the Verifies)
    RealTimeApplicationConfigurationBuilder rtAppBuilder(this, "DDB1");

    functionsDatabaseIn.MoveToRoot();
    bool ret = functionsDatabaseIn.Copy(functionsDatabase);
    if (ret) {
        dataSourcesDatabaseIn.MoveToRoot();
        ret = dataSourcesDatabaseIn.Copy(dataSourcesDatabase);
    }
    if (ret) {
        ret = rtAppBuilder.Set(functionsDatabase, dataSourcesDatabase);
    }
    if (ret) {
        ret = rtAppBuilder.PostConfigureDataSources();
    }
    if (ret) {
        ret = rtAppBuilder.PostConfigureFunctions();
    }
    return ret;
}

bool RealTimeApplication::AllocateGAMMemory() {

    bool ret = functionsDatabase.MoveAbsolute("Functions");
    uint32 numberOfFunctions = functionsDatabase.GetNumberOfChildren();
    for (uint32 i = 0u; i < numberOfFunctions && ret; i++) {
        const char8 * functionId = functionsDatabase.GetChildName(i);
        ret = functionsDatabase.MoveRelative(functionId);
        if (ret) {
            StreamString fullGAMName = "Functions.";
            ret = functionsDatabase.Read("QualifiedName", fullGAMName);
            if (ret) {
                ReferenceT<GAM> gam = Find(fullGAMName.Buffer());
                ret = gam.IsValid();

                if (ret) {

                    ret = gam->AllocateInputSignalsMemory();
                    if (ret) {
                        ret = gam->AllocateOutputSignalsMemory();
                    }
                }
            }
        }
        if (ret) {
            ret = functionsDatabase.MoveToAncestor(1u);
        }
    }
    return ret;
}

bool RealTimeApplication::AllocateDataSourceMemory() {
    bool ret = dataSourcesDatabase.MoveAbsolute("Data");
    uint32 numberOfDs = dataSourcesDatabase.GetNumberOfChildren();
    for (uint32 i = 0u; i < numberOfDs && ret; i++) {
        const char8* dsId = dataSourcesDatabase.GetChildName(i);
        ret = dataSourcesDatabase.MoveRelative(dsId);
        if (ret) {
            StreamString fullDsName = "Data.";
            ret = dataSourcesDatabase.Read("QualifiedName", fullDsName);
            if (ret) {
                ReferenceT<DataSourceI> ds = Find(fullDsName.Buffer());
                ret = ds.IsValid();
                if (ret) {
                    ret = ds->AllocateMemory();
                }
            }
        }
        if (ret) {
            ret = dataSourcesDatabase.MoveToAncestor(1u);
        }
    }
    return ret;
}

bool RealTimeApplication::AddBrokersToFunctions() {
    //pre: called after ConfigureApplication(*)
    bool ret = dataSourcesDatabase.MoveAbsolute("Data");
    if (ret) {
        uint32 numberOfDataSources = dataSourcesDatabase.GetNumberOfChildren();
        for (uint32 i = 0u; i < numberOfDataSources && ret; i++) {
            const char8 * dataSourceId = dataSourcesDatabase.GetChildName(i);
            ret = dataSourcesDatabase.MoveRelative(dataSourceId);
            StreamString fullDataSourcePath = "Data.";
            if (ret) {
                ret = dataSourcesDatabase.Read("QualifiedName", fullDataSourcePath);
            }
            ReferenceT<DataSourceI> dataSource;
            if (ret) {
                dataSource = Find(fullDataSourcePath.Buffer());
                ret = dataSource.IsValid();
            }
            if (ret) {

                ret = dataSource->AddBrokers(InputSignals);
                if (ret) {
                    ret = dataSource->AddBrokers(OutputSignals);
                }

            }
            if (ret) {
                ret = dataSourcesDatabase.MoveToAncestor(1u);
            }
        }
    }
    return ret;
}

bool RealTimeApplication::PrepareNextState(const char8 * const nextStateName) {

    RealTimeStateInfo status;
    status.currentState = currentStateName.Buffer();
    status.nextState = nextStateName;

    StreamString nextStatePath = "States.";
    nextStatePath += nextStateName;

    ReferenceT<RealTimeState> nextState = Find(nextStatePath.Buffer());
    bool ret = nextState.IsValid();
    if (ret) {
        // change the context in gam groups if needed
        nextState->PrepareState(status);
    }
    if (ret) {
        ret = dataSourceContainer.IsValid();
        if (ret) {
            uint32 numberOfDataSources = dataSourceContainer->Size();
            for (uint32 i = 0u; (i < numberOfDataSources) && (ret); i++) {
                ReferenceT<DataSourceI> dataSource = dataSourceContainer->Get(i);
                ret = dataSource.IsValid();
                if (ret) {
                    // resets the default value in data sources if needed
                    ret = dataSource->PrepareNextState(status);
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::FatalError, "Failed PrepareNextState");
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "Invalid DataSource");
                }
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Data container not found");
        }
    }
    /*
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
     }*/

    return ret;
}

bool RealTimeApplication::StartExecution() {
    index = (index + 1u) % 2u;
    //TODO Start scheduler execution
    return true;
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
