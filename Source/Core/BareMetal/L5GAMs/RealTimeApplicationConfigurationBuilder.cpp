/**
 * @file RealTimeApplicationConfigurationBuilder.cpp
 * @brief Source file for class RealTimeApplicationConfigurationBuilder
 * @date 23/06/2016
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
 * the class RealTimeApplicationConfigurationBuilder (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "GAM.h"
#include "RealTimeApplicationConfigurationBuilder.h"
#include "RealTimeState.h"
#include "RealTimeThread.h"
#include "ReferenceContainerFilterReferences.h"
#include "ReferenceContainerFilterReferencesTemplate.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeApplicationConfigurationBuilder::RealTimeApplicationConfigurationBuilder(ReferenceT<ReferenceContainer> realTimeApplicationIn,
                                                                                 const char8 *defaultDataSourceNameIn) {
    defaultDataSourceName = defaultDataSourceNameIn;
    realTimeApplication = realTimeApplicationIn;
}

bool RealTimeApplicationConfigurationBuilder::InitialiseSignalsDatabase() {
    //Create the Functions master node
    bool ret = functionsDatabase.CreateAbsolute("Functions");
    if (ret) {
        ReferenceContainer gamsFound;
        //Look for all the GAMs inside the RealTimeApplication
        ReferenceContainerFilterReferencesTemplate<GAM> gamFilter(-1, ReferenceContainerFilterMode::RECURSIVE);

        realTimeApplication->Find(gamsFound, gamFilter);
        uint32 numberOfGAMs = gamsFound.Size();
        for (uint32 i = 0; (i < numberOfGAMs) && (ret); i++) {
            // build the GAM qualified name searching each GAM by path
            ReferenceT<GAM> gam = gamsFound.Get(i);
            StreamString qualifiedName = "";
            ReferenceContainerFilterReferences qualifiedNameFilter(1, ReferenceContainerFilterMode::PATH, gam);
            ReferenceContainer result;
            realTimeApplication->Find(result, qualifiedNameFilter);
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
    //Create the Data master node.
    if (ret) {
        ret = dataSourcesDatabase.CreateAbsolute("Data");
    }
    if (ret) {
        ReferenceContainer dataSourcesFound;
        //Look for all the DataSources
        ReferenceContainerFilterReferencesTemplate<DataSourceI> dataSourceFilter(-1, ReferenceContainerFilterMode::RECURSIVE);

        realTimeApplication->Find(dataSourcesFound, dataSourceFilter);
        uint32 numberOfDataSources = dataSourcesFound.Size();
        uint32 i = 0;
        for (i = 0; (i < numberOfDataSources) && (ret); i++) {
            ReferenceT<DataSourceI> dataSource = dataSourcesFound.Get(i);
            StreamString qualifiedName = "";
            ReferenceContainerFilterReferences qualifiedNameFilter(1, ReferenceContainerFilterMode::PATH, dataSource);
            ReferenceContainer result;
            realTimeApplication->Find(result, qualifiedNameFilter);
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

bool RealTimeApplicationConfigurationBuilder::FlattenSignalsDatabases() {
    bool ret = functionsDatabase.MoveAbsolute("Functions");

    if (ret) {
        ret = FlattenSignalsDatabase(functionsDatabase, InputSignals);
    }
    if (ret) {
        ret = functionsDatabase.MoveAbsolute("Functions");
    }
    if (ret) {
        ret = FlattenSignalsDatabase(functionsDatabase, OutputSignals);
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveAbsolute("Data");
    }
    if (ret) {
        ret = FlattenSignalsDatabase(dataSourcesDatabase, None);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::VerifyDataSourcesSignals() {
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
            for (uint32 s = 0u; (s < numberOfSignals) && (ret); s++) {
                StreamString signalId;
                signalId = dataSourcesDatabase.GetChildName(s);
                ret = dataSourcesDatabase.MoveRelative(signalId.Buffer());

                //The QualifiedName must be known
                StreamString signalName;
                if (ret) {
                    ret = dataSourcesDatabase.Read("QualifiedName", signalName);
                }
                if (ret) {
                    ret = (signalName.Size() > 0u);
                }
                else {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "QualifiedName was not defined for signal at position: %s in %s",
                                            signalId.Buffer(), dataSourceName.Buffer())
                }
                StreamString type;
                bool typeDef = dataSourcesDatabase.Read("Type", type);
                if (typeDef) {
                    typeDef = (type.Size() > 0u);
                }

                bool signalFound = false;
                if (ret) {
                    ret = dataSourcesDatabase.MoveToAncestor(1u);

                    // check if it is a partial definition. Delete if the type is not specified, error otherwise
                    for (uint32 k = 0u; (k < numberOfSignals) && (!signalFound) && (ret); k++) {
                        if (k != s) {
                            char8 terminator;
                            StreamString otherSNumber = dataSourcesDatabase.GetChildName(k);
                            ret = dataSourcesDatabase.MoveRelative(otherSNumber.Buffer());
                            StreamString otherSName;
                            //The QualifiedName must be known
                            if (ret) {
                                ret = dataSourcesDatabase.Read("QualifiedName", otherSName);
                                otherSName.Seek(0u);
                            }
                            if (ret) {
                                // ret becomes true if found a partial def
                                StreamString signalNameStr = signalName;
                                signalNameStr.Seek(0u);
                                StreamString token1;
                                StreamString token2;
                                signalFound = true;
                                while (signalNameStr.GetToken(token1, ".", terminator) && (signalFound)) {
                                    if (otherSName.GetToken(token2, ".", terminator)) {
                                        signalFound = (token1 == token2);
                                    }
                                    else {
                                        signalFound = false;
                                    }
                                    token1 = "";
                                    token2 = "";
                                }
                            }
                            ret = dataSourcesDatabase.MoveToAncestor(1u);
                        }
                    }
                }

                if (ret) {
                    if (signalFound) {
                        ret = !typeDef;
                        if (ret) {
                            ret = dataSourcesDatabase.Delete(signalId.Buffer());
                            //resume
                            if (ret) {
                                numberOfSignals = dataSourcesDatabase.GetNumberOfChildren();
                                s--;
                                continue;
                            }
                        }
                        else {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The signal %s in %s is the already defined as a node", signalName.Buffer(),
                                                    dataSourceName.Buffer())
                        }
                    }
                    else {
                        ret = typeDef;
                        if (ret) {
                            ret = dataSourcesDatabase.MoveRelative(signalId.Buffer());
                        }
                        else {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Undefined type for signal %s in %s", signalName.Buffer(),
                                                    dataSourceName.Buffer())

                        }
                    }
                }
                //If the number of dimensions is > 1 then the NumberOfElements must also be defined.
                uint32 numberOfDimensions = 0u;
                if (ret) {
                    if (!dataSourcesDatabase.Read("NumberOfDimensions", numberOfDimensions)) {
                        ret = dataSourcesDatabase.Write("NumberOfDimensions", 0u);
                    }
                }

                uint32 numberOfElements = 0u;
                if (ret) {
                    if (!dataSourcesDatabase.Read("NumberOfElements", numberOfElements)) {
                        if (numberOfDimensions > 0u) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "NumberOfElements is not defined for signal: %s in %s, assume it as 1",
                                                    signalName.Buffer(), dataSourceName.Buffer())
                        }
                        numberOfElements = 1u;
                        ret = dataSourcesDatabase.Write("NumberOfElements", numberOfElements);
                    }
                }

                //Compute the byte size
                TypeDescriptor signalTypeDescriptor;
                if (ret) {
                    signalTypeDescriptor = TypeDescriptor::GetTypeDescriptorFromTypeName(type.Buffer());
                    ret = (signalTypeDescriptor != InvalidType);
                }
                uint32 signalNumberOfBytes = 0u;
                if (ret) {
                    signalNumberOfBytes = numberOfElements * signalTypeDescriptor.numberOfBits / 8u;
                }
                if (ret) {
                    ret = dataSourcesDatabase.Write("ByteSize", signalNumberOfBytes);
                }

                if (ret) {
                    StreamString defaultVal;
                    if (!dataSourcesDatabase.Read("Default", defaultVal)) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Default value is not defined for signal: %s in %s, by default it will be zeroed",
                                                signalName.Buffer(), dataSourceName.Buffer())
                    }
                    // check validity of the default value
                    else {
                        //TODO
                        // parse the DefaultValue
                        // check its compatibility with the type
                        StreamString defValConfig = "Default=";
                        defValConfig += defaultVal;
                        defValConfig.Seek(0u);
                        ConfigurationDatabase cdb;
                        StandardParser parser(defValConfig, cdb);
                        void *ptr = HeapManager::Malloc(signalNumberOfBytes);
                        if (ptr != NULL) {
                            AnyType at = AnyType(signalTypeDescriptor, 0u, ptr);
                            uint32 usedDimensions = numberOfDimensions;
                            if (numberOfDimensions > 1u) {
                                usedDimensions = 1u;
                            }
                            at.SetNumberOfDimensions(usedDimensions);
                            at.SetNumberOfElements(0u, numberOfElements);

                            ret = parser.Parse();
                            if (ret) {
                                ret = cdb.Read("Default", at);
                            }
                            ret = HeapManager::Free(reinterpret_cast<void*&>(ptr));
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

bool RealTimeApplicationConfigurationBuilder::ResolveFunctionSignals() {
    bool ret = ResolveFunctionSignals(InputSignals);
    if (ret) {
        ret = ResolveFunctionSignals(OutputSignals);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::ResolveDataSources() {
    bool ret = ResolveDataSources(InputSignals);
    if (ret) {
        ret = ResolveDataSources(OutputSignals);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::VerifyFunctionSignals() {
    bool ret = VerifyFunctionSignals(InputSignals);
    if (ret) {
        ret = VerifyFunctionSignals(OutputSignals);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::ResolveStates() {
    bool ret = true;
    ReferenceContainer statesFound;
    //Look for all the RealTimeStates
    ReferenceContainerFilterReferencesTemplate<RealTimeState> stateFilter(-1, ReferenceContainerFilterMode::RECURSIVE);

    realTimeApplication->Find(statesFound, stateFilter);
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
        for (j = 0; (j < realTimeApplication->Size()) && (!found); j++) {
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
                        realTimeApplication->Find(result, qualifiedNameFilter);
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
                            // the same gam cannot be declared in two different threads!!
                            ret = (functionsDatabase.GetType(stateName).IsVoid());
                            if (ret) {
                                ret = functionsDatabase.Write(stateName, threadName);
                            }
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

bool RealTimeApplicationConfigurationBuilder::VerifyStates() {
    bool ret = functionsDatabase.MoveAbsolute("Functions");
    uint32 numberOfFunctions = functionsDatabase.GetNumberOfChildren();
    for (uint32 i = 0u; i < numberOfFunctions && (ret); i++) {
        ret = functionsDatabase.MoveRelative(functionsDatabase.GetChildName(i));
        if (ret) {
            ret = functionsDatabase.MoveRelative("States");
        }
        if (ret) {
            ret = functionsDatabase.GetNumberOfChildren() > 0u;
            if (!ret) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The GAM with id %s is never called", functionsDatabase.GetChildName(i))
            }
        }
        if (ret) {
            //return to Functions level
            ret = functionsDatabase.MoveToAncestor(2u);
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::ResolveConsumersAndProducers() {
    bool ret = ResolveConsumersAndProducers(true);
    if (ret) {
        ret = ResolveConsumersAndProducers(false);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::VerifyConsumersAndProducers() {

    bool ret = dataSourcesDatabase.MoveAbsolute("Data");
    uint32 numberOfDS = dataSourcesDatabase.GetNumberOfChildren();
    for (uint32 i = 0u; (i < numberOfDS) && (ret); i++) {

        ret = dataSourcesDatabase.MoveRelative(dataSourcesDatabase.GetChildName(i));
        if (ret) {
            ret = dataSourcesDatabase.MoveRelative("Signals");
        }
        if (ret) {
            uint32 numberOfSignals = dataSourcesDatabase.GetNumberOfChildren();
            for (uint32 j = 0u; (j < numberOfSignals) && (ret); j++) {
                ret = dataSourcesDatabase.MoveRelative(dataSourcesDatabase.GetChildName(j));
                if (ret) {
                    ret = dataSourcesDatabase.MoveRelative("States");
                }
                if (ret) {
                    uint32 numberOfStates = dataSourcesDatabase.GetNumberOfChildren();
                    for (uint32 k = 0u; (k < numberOfStates) && (ret); k++) {
                        ret = dataSourcesDatabase.MoveRelative(dataSourcesDatabase.GetChildName(k));
                        if (ret) {
                            // no Producers found... in this case the default value must be declared
                            AnyType prods = dataSourcesDatabase.GetType("Producers");
                            ret = (prods.GetNumberOfElements(0u) > 1u);
                            if (!ret) {
                                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "More than one producer of signal %s in state %s",
                                                        functionsDatabase.GetChildName(i))
                            }
                        }
                        if (ret) {
                            // to States
                            ret = dataSourcesDatabase.MoveToAncestor(1u);
                        }
                    }
                }
                if (ret) {
                    // to Signals
                    ret = dataSourcesDatabase.MoveToAncestor(2u);
                }
            }
        }
        if (ret) {
            // to Data
            ret = dataSourcesDatabase.MoveToAncestor(2u);
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::ResolveFunctionSignalsMemorySize() {
    bool ret = ResolveFunctionSignalsMemorySize(InputSignals);
    if (ret) {
        ret = ResolveFunctionSignalsMemorySize(OutputSignals);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::ResolveFunctionsMemory() {
    bool ret = ResolveFunctionsMemory(InputSignals);
    if (ret) {
        ret = ResolveFunctionsMemory(OutputSignals);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::AllocateFunctionsMemory() {
    bool ret = AllocateFunctionsMemory(InputSignals);
    if (ret) {
        ret = AllocateFunctionsMemory(OutputSignals);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::AssignFunctionsMemoryToDataSource() {
    bool ret = AssignFunctionsMemoryToDataSource(InputSignals);
    if (ret) {
        ret = AssignFunctionsMemoryToDataSource(OutputSignals);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::PostConfigureDataSources() {
    bool ret = dataSourcesDatabase.MoveToRoot();
    uint32 numberOfDataSources = 0u;
    if (ret) {
        ret = dataSourcesDatabase.MoveRelative("Data");
    }
    if (ret) {
        numberOfDataSources = dataSourcesDatabase.GetNumberOfChildren();
    }
    if (ret) {
        uint32 n;
        for (n = 0u; (n < numberOfDataSources) && (ret); n++) {
            StreamString qualifiedName;
            StreamString dataSourceIdx;
            dataSourceIdx.Printf("%d", n);
            ret = dataSourcesDatabase.MoveRelative(dataSourceIdx.Buffer());
            if (ret) {
                ret = dataSourcesDatabase.Read("QualifiedName", qualifiedName);
            }
            ReferenceT<DataSourceI> dataSource;
            if (ret) {
                dataSource = realTimeApplication->Find(qualifiedName.Buffer());
                ret = dataSource.IsValid();
            }
            if (ret) {
                ret = dataSource->SetConfiguredDatabase(dataSourcesDatabase);
            }
            if (ret) {
                ret = dataSource->AllocateMemory();
            }
            if (ret) {
                ret = dataSourcesDatabase.MoveToAncestor(1u);
            }
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::PostConfigureFunctions() {
    bool ret = functionsDatabase.MoveToRoot();
    uint32 numberOfFunctions = 0u;
    if (ret) {
        ret = functionsDatabase.MoveRelative("Functions");
    }
    if (ret) {
        numberOfFunctions = functionsDatabase.GetNumberOfChildren();
    }
    if (ret) {
        uint32 n;
        for (n = 0u; (n < numberOfFunctions) && (ret); n++) {
            StreamString qualifiedName;
            StreamString functionIdx;
            functionIdx.Printf("%d", n);
            ret = functionsDatabase.MoveRelative(functionIdx.Buffer());
            if (ret) {
                ret = functionsDatabase.Read("QualifiedName", qualifiedName);
            }
            ReferenceT<GAM> gam;
            if (ret) {
                gam = realTimeApplication->Find(qualifiedName.Buffer());
                ret = gam.IsValid();
            }
            if (ret) {
                ret = gam->SetConfiguredDatabase(functionsDatabase);
            }
            if (ret) {
                ret = functionsDatabase.MoveToAncestor(1u);
            }
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::AddBrokersToFunctions() {
    bool ret = AddBrokersToFunctions(InputSignals);
    if (ret) {
        ret = AddBrokersToFunctions(OutputSignals);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::Copy(ConfigurationDatabase &functionsDatabaseOut,
                                                   ConfigurationDatabase &dataSourcesDatabaseOut) {
    bool ret = functionsDatabase.MoveToRoot();
    if (ret) {
        ret = functionsDatabase.Copy(functionsDatabaseOut);
    }
    if (ret) {
        dataSourcesDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.Copy(dataSourcesDatabaseOut);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::FlattenSignalsDatabase(ConfigurationDatabase &signalDatabase,
                                                                     SignalDirection direction) {
    const char8 *signalDirection = "Signals";
    if (direction == InputSignals) {
        signalDirection = "InputSignals";
    }
    else if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }

    bool isDataSource = (&signalDatabase == &dataSourcesDatabase);
    uint32 signalNumber;
    bool ret = true;
    //A signal container can be either a GAM or a DataSource
    uint32 numberOfSignalContainers = signalDatabase.GetNumberOfChildren();
    uint32 i;
    //For each signal container...
    for (i = 0; (i < numberOfSignalContainers) && (ret); i++) {
        // move inside GAM or DataSource
        const char8 *containerId = signalDatabase.GetChildName(i);
        ret = signalDatabase.MoveRelative(containerId);
        if (ret) {
            // move inside "Signals" field
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

            // replace the signals in tree mode with the signals in flat mode
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
                //Copy into the signalDatabase the found elements.
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

bool RealTimeApplicationConfigurationBuilder::FlattenSignal(ConfigurationDatabase &signalDatabase,
                                                            const char8 * const signalName,
                                                            ConfigurationDatabase &resolvedSignal,
                                                            uint32 &signalNumber,
                                                            bool forceWriteDataSource) {
    bool ret = true;
    StreamString signalType;

    // a node does not have the field Type specified and must contain at least another node (namespace or signal)
    bool signalTypeDefined = signalDatabase.Read("Type", signalType);
    if (signalTypeDefined) {
        signalTypeDefined = (signalType.Size() > 0u);
    }
    //Check if it is a pure node (i.e. a nested namespace which contains other signals)
    bool isNode = false;
    if (!signalTypeDefined) {
        uint32 numberOfElements = signalDatabase.GetNumberOfChildren();
        uint32 n;
        for (n = 0; (n < numberOfElements) && (!isNode); n++) {
            StreamString elementName = signalDatabase.GetChildName(n);
            //If this element is a node then recurse
            if (signalDatabase.GetType(elementName.Buffer()).GetTypeDescriptor() == VoidType) {
                // "MemberAliases" is the only node can be found in a signal. Mark it as a keyword.
                isNode = StringHelper::Compare(elementName.Buffer(), "MemberAliases") != 0;
                if (isNode) {
                    if (signalDatabase.MoveRelative(elementName.Buffer())) {
                        StreamString signalFullName;
                        signalFullName.Printf("%s.%s", signalName, elementName.Buffer());
                        ret = FlattenSignal(signalDatabase, signalFullName.Buffer(), resolvedSignal, signalNumber, forceWriteDataSource);
                        signalDatabase.MoveToAncestor(1u);
                    }
                }
            }
        }
    }
    //Namespace resolved. Working at signal level
    if (!isNode) {
        bool isStructuredData = false;
        StreamString dataSourceName;
        if (forceWriteDataSource) {
            //This is a signal in the GAMs and the DataSource must be known now.
            if (!signalDatabase.Read("DataSource", dataSourceName)) {
                if (defaultDataSourceName.Size() > 0u) {
                    dataSourceName = defaultDataSourceName;
                    ret = signalDatabase.Write("DataSource", dataSourceName);
                }
                else {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "DataSource not specified for %s and DefaultDataSource not specified",
                                            signalName)
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

                    //TODO check here number of elements and number of dimensions
                    // and produce a warning

                    AnyType ranges = signalDatabase.GetType("Ranges");
                    AnyType timeCyclesSamples = signalDatabase.GetType("TimeCyclesSamples");
                    ret = SignalIntrospectionToStructuredData(signalDatabase, signalType.Buffer(), signalName, alias.Buffer(), dataSourceName.Buffer(), ranges,
                                                              timeCyclesSamples, resolvedSignal, signalNumber);
                }
                if (ret) {
                    //The original structured type is no longer needed
                    ret = signalDatabase.Delete("Type");
                }
            }
        }
        //BasicType. Write all the known signal properties
        // do this also if the type is undefined
        if (!isStructuredData) {
            StreamString property;
            StreamString signalNumberStr;
            signalNumberStr.Printf("Signals.%d", signalNumber);
            ret = resolvedSignal.CreateAbsolute(signalNumberStr.Buffer());
            if (ret) {
                ret = resolvedSignal.Write("QualifiedName", signalName);
            }
            //Loop and copy all known properties at this time.
            const char8 *properties[] = { "Type", "NumberOfDimensions", "NumberOfElements", "Alias", "Ranges", "DataSource", "TimeCyclesSamples", "Default",
                    NULL_PTR(char8 *) };
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

bool RealTimeApplicationConfigurationBuilder::SignalIntrospectionToStructuredData(ConfigurationDatabase &signalDatabase,
                                                                                  const char8 * const typeName,
                                                                                  const char8 * const signalName,
                                                                                  const char8 * const alias,
                                                                                  const char8 * const dataSourceName,
                                                                                  AnyType ranges,
                                                                                  AnyType timeCyclesSamples,
                                                                                  StructuredDataI & data,
                                                                                  uint32 &signalNumber) {
    //Try to find the registered type in the ClassRegistryDatabase
    const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(typeName);
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
        //For each of the structure members...
        for (i = 0u; (i < numberOfMembers) && ret; i++) {
            const IntrospectionEntry entry = intro->operator[](i);
            StreamString fullSignalName;
            fullSignalName.Printf("%s.%s", signalName, entry.GetMemberName());
            StreamString fullAliasName = "";
            if (StringHelper::Length(alias) > 0u) {
                //Recursively build the signal name by adding all the nested structure path
                fullAliasName.Printf("%s.%s", alias, entry.GetMemberName());
            }
            // loop in the aliases finding the match... if found replace
            if (signalDatabase.MoveRelative("MemberAliases")) {
                StreamString memberAlias;
                if (signalDatabase.Read(fullSignalName.Buffer(), memberAlias)) {
                    fullAliasName = memberAlias;
                }
                signalDatabase.MoveToAncestor(1u);
            }
            //If the member is still structured data, continue to discombobulate
            if (entry.GetMemberTypeDescriptor().isStructuredData) {

                ret = SignalIntrospectionToStructuredData(signalDatabase, entry.GetMemberTypeName(), fullSignalName.Buffer(), fullAliasName.Buffer(),
                                                          dataSourceName, ranges, timeCyclesSamples, data, signalNumber);
            }
            else {
                //Got to the final BasicType. Add a node with the signal number as the name...
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
                if (timeCyclesSamples.GetTypeDescriptor() != VoidType) {
                    ret = data.Write("TimeCyclesSamples", timeCyclesSamples);
                }
                if (ret) {
                    if (StringHelper::Length(dataSourceName) > 0u) {
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
                    uint32 totalElements = 1u;
                    for (uint32 j = 0; j <= entry.GetNumberOfDimensions(); j++) {
                        totalElements *= entry.GetNumberOfElements(j);
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

bool RealTimeApplicationConfigurationBuilder::ResolveFunctionSignals(SignalDirection direction) {
    const char8 *signalDirection = "InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }

    bool ret = functionsDatabase.MoveAbsolute("Functions");
    uint32 numberOfFunctions = 0u;
    if (ret) {
        numberOfFunctions = functionsDatabase.GetNumberOfChildren();
        //For every function
        for (uint32 i = 0; (i < numberOfFunctions) && (ret); i++) {
            StreamString functionName;
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
                //For every signal in this function
                for (uint32 s = 0u; (s < numberOfSignals) && (ret); s++) {
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
                        ret = ResolveFunctionSignal(signalName.Buffer(), functionName.Buffer(), dataSourceName.Buffer(), numberOfSignals);
                    }
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Signal %s not found in %s", signalName.Buffer(), dataSourceName.Buffer())
                    }
                    if (ret) {
                        //MoveToAncestor is done in ResolveFunctionSignal
                        //ret = functionsDatabase.MoveToAncestor(1u);
                        numberOfSignals = functionsDatabase.GetNumberOfChildren();
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

bool RealTimeApplicationConfigurationBuilder::ResolveDataSources(SignalDirection direction) {
    const char8 *signalDirection = "InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }
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
                // read the qualified name just for error reporting
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
                for (uint32 s = 0u; (s < numberOfSignals) && (ret); s++) {
                    StreamString signalId = functionsDatabase.GetChildName(s);
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
                        //Add the signal to the Data.dataSourceName node (if the Type is defined)
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

bool RealTimeApplicationConfigurationBuilder::FindDataSourceNumber(StreamString dataSourceName,
                                                                   StreamString &dataSourceNumber) {
    bool ret = dataSourcesDatabase.MoveAbsolute("Data");
    if (ret) {
        uint32 numberOfDataSources = dataSourcesDatabase.GetNumberOfChildren();
        bool done = false;
        for (uint32 n = 0u; (n < numberOfDataSources) && (ret) && (!done); n++) {
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

bool RealTimeApplicationConfigurationBuilder::AddSignalToDataSource(StreamString functionName,
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
    // if alias unspecified, the name of the dataSource is the GAM signal name
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

    if ((!signalAlreadyExists) && (ret)) {
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
        const char8 *properties[] = { "Type", "NumberOfDimensions", "NumberOfElements", "Default", NULL_PTR(char8 *) };
        uint32 p = 0u;
        while ((properties[p] != NULL_PTR(char8 *)) && (ret)) {
            AnyType elementSignalDatabase = functionsDatabase.GetType(properties[p]);
            AnyType elementDataSourceDatabase = dataSourcesDatabase.GetType(properties[p]);
            //Property already exists, check compatibility!
            if (elementSignalDatabase.GetTypeDescriptor() != VoidType) {
                if (elementDataSourceDatabase.GetTypeDescriptor() != VoidType) {
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

bool RealTimeApplicationConfigurationBuilder::ResolveFunctionSignal(const char8 * const signalName,
                                                                    const char8 * const functionName,
                                                                    const char8 * const dataSourceName,
                                                                    uint32 numberOfFunctionSignals) {

    uint32 numberOfSignalsInDataSource = dataSourcesDatabase.GetNumberOfChildren();
    uint32 createdSignals = 0u;
    StreamString type;
    bool typeDefined = functionsDatabase.Read("Type", type);
    bool atLeastOneSignalFound = false;
    bool ret = true;
    const char8 * signalToDeleteName = NULL_PTR(const char8 *);
    //If the Type is defined, stop as soon as the signal found. Otherwise loop for all the signals...
    //Note that signalName can be contained in signalNameInDataSource (e.g. looking for A.B and in the DataSource we have A.B.C and A.B.D,
    //means that the signals A.B.C and A.B.D will be added to the GAM is the Type was not defined)
    for (uint32 s = 0; (s < numberOfSignalsInDataSource) && (ret) && (!typeDefined || !atLeastOneSignalFound); s++) {
        StreamString dataSourceSignalId = dataSourcesDatabase.GetChildName(s);
        ret = dataSourcesDatabase.MoveRelative(dataSourceSignalId.Buffer());
        StreamString dataSourceSignalName;
        if (ret) {
            ret = dataSourcesDatabase.Read("QualifiedName", dataSourceSignalName);
        }
        dataSourceSignalName.Seek(0u);
        bool signalFound = false;
        StreamString signalNameStr = signalName;
        signalNameStr.Seek(0u);
        uint32 signalNameSize = signalNameStr.Size();
        // need to create sub fields
        bool createNestedSignal = (signalNameSize < dataSourceSignalName.Size());
        if (ret) {
            StreamString token1;
            StreamString token2;
            char8 terminator;
            signalFound = true;
            while (signalNameStr.GetToken(token1, ".", terminator) && (signalFound)) {
                if (dataSourceSignalName.GetToken(token2, ".", terminator)) {
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
                if (createNestedSignal) {
                    //Mark this signal to be deleted
                    if (!atLeastOneSignalFound) {
                        signalToDeleteName = functionsDatabase.GetName();
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
                        ret = functionsDatabase.Write("DataSource", dataSourceName);
                    }
                }
            }

            atLeastOneSignalFound = true;
            //Loop through all properties. If the property is not defined, get it from the DataSource
            const char8 *properties[] = { "Type", "NumberOfDimensions", "NumberOfElements", "Default", NULL_PTR(char8 *) };
            uint32 p = 0u;
            while ((properties[p] != NULL_PTR(char8 *)) && (ret)) {
                AnyType elementSignalDatabase = functionsDatabase.GetType(properties[p]);
                AnyType elementDataSourceDatabase = dataSourcesDatabase.GetType(properties[p]);
                if (elementSignalDatabase.GetTypeDescriptor() == VoidType) {
                    if (elementDataSourceDatabase.GetTypeDescriptor() != VoidType) {
                        ret = functionsDatabase.Write(properties[p], elementDataSourceDatabase);
                    }
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "%s was not defined for signal: %s in %s", properties[p], signalName,
                                                functionName)
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
                        fullPropertyName.Printf("%s.%s", signalName, properties[p]);
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Mismatch in signal with name: %s. %s asked for %s while %s asked for %s",
                                                signalName, functionName, sElementSignalDatabase.Buffer(), dataSourceName, sElementDataSourceDatabase.Buffer())
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

    if (ret) {
        ret = atLeastOneSignalFound;
    }
    // go up and delete the uncomplete signal if needed
    if (ret) {
        ret = functionsDatabase.MoveToAncestor(1u);
        if (ret) {
            if (signalToDeleteName != NULL) {
                ret = functionsDatabase.Delete(signalToDeleteName);
            }
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::VerifyFunctionSignals(SignalDirection direction) {
    const char8 *signalDirection = "InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }

    bool ret = functionsDatabase.MoveAbsolute("Functions");
    uint32 numberOfFunctions = 0u;
    StreamString functionName;
    if (ret) {
        numberOfFunctions = functionsDatabase.GetNumberOfChildren();
    }
    if (ret) {
        //For every function
        for (uint32 i = 0; (i < numberOfFunctions) && (ret); i++) {
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
                //For every signal in this function
                for (uint32 s = 0u; (s < numberOfSignals) && (ret); s++) {
                    StreamString signalId;
                    signalId = functionsDatabase.GetChildName(s);
                    ret = functionsDatabase.MoveRelative(signalId.Buffer());
                    //The QualifiedName must be known
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
                        ret = functionsDatabase.Read("NumberOfElements", numberOfElements);
                        if (!ret) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "NumberOfElements was not defined for signal: %s in %s",
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

bool RealTimeApplicationConfigurationBuilder::ResolveConsumersAndProducers(bool consumers) {
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

bool RealTimeApplicationConfigurationBuilder::ResolveFunctionSignalsMemorySize(SignalDirection direction) {
    const char8 *signalDirection = "InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }

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

bool RealTimeApplicationConfigurationBuilder::ResolveFunctionsMemory(SignalDirection direction) {
    const char8 *signalDirection = "InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }

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
                    uint32 numberOfTimeCyclesSamplesElements = 0u;
                    uint32 *offsetMatrixBackend = NULL_PTR(uint32 *);
                    uint32 *timeCyclesSamplesVectorBackend = NULL_PTR(uint32 *);
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

                    AnyType existentByteOffset = functionsDatabase.GetType("ByteOffset");
                    if (existentByteOffset.GetDataPointer() != NULL_PTR(void *)) {
                        numberOfOffsetElements = existentByteOffset.GetNumberOfElements(1u);
                        offsetMatrixBackend = new uint32[numberOfOffsetElements * 2u];
                        Matrix<uint32> offsetMat(offsetMatrixBackend, numberOfOffsetElements, 2u);
                        ret = functionsDatabase.Read("ByteOffset", offsetMat);
                    }
                    AnyType existentTimeCyclesSamples = functionsDatabase.GetType("TimeCyclesSamples");
                    if (existentTimeCyclesSamples.GetDataPointer() != NULL_PTR(void *)) {
                        numberOfTimeCyclesSamplesElements = existentTimeCyclesSamples.GetNumberOfElements(0u);
                        timeCyclesSamplesVectorBackend = new uint32[numberOfTimeCyclesSamplesElements];
                        Vector<uint32> timeCyclesSamplesVec(timeCyclesSamplesVectorBackend, numberOfTimeCyclesSamplesElements);
                        ret = functionsDatabase.Read("TimeCyclesSamples", timeCyclesSamplesVec);
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
                            if (timeCyclesSamplesVectorBackend != NULL_PTR(void *)) {
                                Vector<uint32> timeCyclesSamplesVec(timeCyclesSamplesVectorBackend, numberOfTimeCyclesSamplesElements);
                                ret = functionsDatabase.Write("TimeCyclesSamples", timeCyclesSamplesVec);
                                delete[] timeCyclesSamplesVectorBackend;
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

bool RealTimeApplicationConfigurationBuilder::AllocateFunctionsMemory(SignalDirection direction) {
    const char8 *signalDirection = "InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }

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
            //Allocate the memory
            ReferenceT<GAM> gam;
            if (ret) {
                gam = realTimeApplication->Find(functionName.Buffer());
            }
            if (ret) {
                ret = gam.IsValid();
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
                    //Allocate the memory
                    void *signalBlockMemory = NULL_PTR(void *);
                    if (ret) {
                        if (direction == InputSignals) {
                            signalBlockMemory = gam->AllocateInputSignalsMemory(byteSize);
                        }
                        else {
                            signalBlockMemory = gam->AllocateOutputSignalsMemory(byteSize);
                        }
                        ret = functionsDatabase.Write("Address", signalBlockMemory);
                    }
                    if (ret) {
                        //Move to SignalDirection level
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

bool RealTimeApplicationConfigurationBuilder::AssignFunctionsMemoryToDataSource(SignalDirection direction) {
    const char8 *signalDirection = "InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }

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
                    uint64 address = 0u;
                    StreamString dataSourceName;
                    StreamString dataSourceId = functionsDatabase.GetChildName(d);
                    ret = functionsDatabase.MoveRelative(dataSourceId.Buffer());
                    if (ret) {
                        ret = functionsDatabase.Read("DataSource", dataSourceName);
                    }
                    if (ret) {
                        ret = functionsDatabase.Read("ByteSize", byteSize);
                    }
                    if (ret) {
                        ret = functionsDatabase.Read("Address", address);
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
                        if (!dataSourcesDatabase.MoveRelative(signalDirection)) {
                            ret = dataSourcesDatabase.CreateRelative(signalDirection);
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
                    if (ret) {
                        ret = dataSourcesDatabase.Write("Address", reinterpret_cast<void *>(address));
                    }
                    if (ret) {
                        ret = dataSourcesDatabase.Write("ByteSize", byteSize);
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

bool RealTimeApplicationConfigurationBuilder::AddBrokersToFunctions(SignalDirection direction) {
    bool ret = dataSourcesDatabase.MoveToRoot();
    uint32 numberOfDataSources = 0u;
    if (ret) {
        ret = dataSourcesDatabase.MoveRelative("Data");
    }
    if (ret) {
        numberOfDataSources = dataSourcesDatabase.GetNumberOfChildren();
    }
    if (ret) {
        uint32 n;
        for (n = 0u; (n < numberOfDataSources) && (ret); n++) {
            StreamString qualifiedName;
            StreamString dataSourceIdx;
            dataSourceIdx.Printf("%d", n);
            ret = dataSourcesDatabase.MoveRelative(dataSourceIdx.Buffer());
            if (ret) {
                ret = dataSourcesDatabase.Read("QualifiedName", qualifiedName);
            }
            ReferenceT<DataSourceI> dataSource;
            if (ret) {
                dataSource = realTimeApplication->Find(qualifiedName.Buffer());
                ret = dataSource.IsValid();
            }
            uint32 numberOfFunctions = 0u;
            if (ret) {
                numberOfFunctions = dataSource->GetNumberOfFunctions();
            }

            uint32 k;
            for (k = 0u; (k < numberOfFunctions) && (ret); k++) {
                StreamString gamQualifiedName;
                ret = dataSource->GetFunctionName(k, gamQualifiedName);
                ReferenceT<GAM> gam;
                if (ret) {
                    gam = realTimeApplication->Find(gamQualifiedName.Buffer());
                    ret = gam.IsValid();
                }
                ReferenceT<BrokerI> broker;
                if (ret) {
                    if (direction == InputSignals) {
                        broker = dataSource->GetInputReader(gamQualifiedName.Buffer());
                    }
                    else {
                        broker = dataSource->GetOutputWriter(gamQualifiedName.Buffer());
                    }
                    ret = broker.IsValid();
                }
                if (ret) {
                    if (direction == InputSignals) {
                        gam->AddInputBroker(broker);
                    }
                    else {
                        gam->AddOutputBroker(broker);
                    }
                }
            }
            if (ret) {
                ret = dataSourcesDatabase.MoveToAncestor(1u);
            }
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::FindSignalName(StreamString signalName,
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

bool RealTimeApplicationConfigurationBuilder::FindFunctionNumber(StreamString functionName,
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

}

