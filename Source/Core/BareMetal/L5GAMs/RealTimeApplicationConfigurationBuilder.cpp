/**
 * @file RealTimeApplicationConfigurationBuilder.cpp
 * @brief Source file for class RealTimeApplicationConfigurationBuilder
 * @date 11/12/2019
 * @author Giuseppe Ferro
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
#include "Introspection.h"
#include "RealTimeApplicationConfigurationBuilder.h"
#include "RealTimeState.h"
#include "RealTimeThread.h"
#include "ReferenceContainerFilterReferences.h"
#include "ReferenceContainerFilterReferencesTemplate.h"
#include "StandardParser.h"
#include "StaticList.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

RealTimeApplicationConfigurationBuilder::RealTimeApplicationConfigurationBuilder() :
        Object() {
    realTimeApplication = NULL_PTR(RealTimeApplication*);
    initialiseAfterInitialisation = true;
}

RealTimeApplicationConfigurationBuilder::RealTimeApplicationConfigurationBuilder(RealTimeApplication &realTimeApplicationIn,
                                                                                 const char8 *const defaultDataSourceNameIn) :
        Object() {
    defaultDataSourceName = defaultDataSourceNameIn;
    realTimeApplication = &realTimeApplicationIn;
    initialiseAfterInitialisation = true;
}

RealTimeApplicationConfigurationBuilder::RealTimeApplicationConfigurationBuilder(ConfigurationDatabase &globalDatabaseIn,
                                                                                 const char8 *const defaultDataSourceNameIn) :
        Object() {
    defaultDataSourceName = defaultDataSourceNameIn;
    bool ret = globalDatabaseIn.Copy(globalDatabase);
    if (ret) {
        ret = globalDatabase.MoveToRoot();
    }
    if (!ret) {
        REPORT_ERROR_FULL(ErrorManagement::FatalError, "In RealTimeApplicationConfigurationBuilder failed to MoveToRoot");
    }
    realTimeApplication = NULL_PTR(RealTimeApplication*);
    initialiseAfterInitialisation = false;
}

void RealTimeApplicationConfigurationBuilder::SetParameters(RealTimeApplication &realTimeApplicationIn,
                                                            const char8 *const defaultDataSourceNameIn) {
    defaultDataSourceName = defaultDataSourceNameIn;
    realTimeApplication = &realTimeApplicationIn;
    initialiseAfterInitialisation = true;
}

bool RealTimeApplicationConfigurationBuilder::ConfigureAfterInitialisation() {
    initialiseAfterInitialisation = true;
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to InitialiseSignalsDatabase");
    bool ret = InitialiseSignalsDatabase();
    if (!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to InitialiseSignalsDatabase");
    }
    if (ret) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to FlattenSignalsDatabases");
        ret = FlattenSignalsDatabases();
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to FlattenSignalsDatabases");
        }
    }
    if (ret) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to ResolveStates");
        ret = ResolveStates();
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to ResolveStates");
        }
    }
    if (ret) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to ResolveDataSources");
        ret = ResolveDataSources();
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to ResolveDataSources");
        }
    }
    if (ret) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to VerifyDataSourcesSignals");
        ret = VerifyDataSourcesSignals();
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to VerifyDataSourcesSignals");
        }
    }
    if (ret) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to VerifyConsumersAndProducers");
        ret = VerifyConsumersAndProducers();
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to VerifyConsumersAndProducers");
        }
    }
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to CleanCaches");
    CleanCaches();
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::ConfigureBeforeInitialisation() {
    initialiseAfterInitialisation = false;
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to InitialiseSignalsDatabaseFromConfiguration");
    bool ret = InitialiseSignalsDatabaseFromConfiguration();
    if (!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to InitialiseSignalsDatabaseFromConfiguration");
    }
    if (ret) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to FlattenSignalsDatabases");
        ret = FlattenSignalsDatabases();
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to FlattenSignalsDatabases");
        }
    }
    if (ret) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to ResolveStatesFromConfiguration");
        ret = ResolveStatesFromConfiguration();
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to ResolveStatesFromConfiguration");
        }
    }
    if (ret) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to ResolveDataSources");
        ret = ResolveDataSources();
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to ResolveDataSources");
        }
    }
    if (ret) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to VerifyDataSourcesSignals");
        ret = VerifyDataSourcesSignals();
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to VerifyDataSourcesSignals");
        }
    }
    if (ret) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to VerifyConsumersAndProducers");
        ret = VerifyConsumersAndProducers();
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to VerifyConsumersAndProducers");
        }
    }
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Going to CleanCaches");
    CleanCaches();
    return ret;

}

bool RealTimeApplicationConfigurationBuilder::InitialiseSignalsDatabaseFromConfiguration() {
    StreamString path = "";
    uint32 index = 0u;
    bool ret = functionsDatabase.CreateAbsolute("Functions");
    if (ret) {
        ret = globalDatabase.MoveAbsolute("+Functions");
        if (!ret) {
            ret = globalDatabase.MoveAbsolute("$Functions");
        }
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "No Functions section found");
        }
    }
    if (ret) {
        //the global database must point to the application to be configured
        ret = SearchGAMs(globalDatabase, functionsDatabase, path, index, false);
    }
    if (ret) {
        ret = globalDatabase.MoveAbsolute("+Data");
        if (!ret) {
            ret = globalDatabase.MoveAbsolute("$Data");
        }
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "No Data section found");
        }
        if (ret) {
            ret = dataSourcesDatabase.CreateAbsolute("Data");
        }
    }
    if (ret) {
        path = "";
        index = 0u;
        uint32 timingDataSourceCounter = 0u;
        ret = SearchDataSources(globalDatabase, dataSourcesDatabase, path, index, timingDataSourceCounter);
        if (ret) {
            ret = (timingDataSourceCounter == 1u);
            if (timingDataSourceCounter > 1u) {
                REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Only one TimingDataSource per application allowed");
            }
            else {
                if (timingDataSourceCounter == 0u) {
                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Please specify a TimingDataSource to store GAMs relevant times");
                }
            }
        }

    }
    if (ret) {
        ret = functionsDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }
    return ret;
}

////////////////////////////////
////////////////////////////////
// InitialiseSignalsDatabase
////////////////////////////////
////////////////////////////////

//-Creates a Function and a Data ConfigurationDatabase with all the GAMs and DataSource signals as they are declared in the
//original ConfigurationDatabase.
//-QualifiedName is the name of the GAM path beginning from "Functions" or the name of the DataSource beginning from "Data"
//-The name of each GAM and of each DS node is an incremental id number
bool RealTimeApplicationConfigurationBuilder::InitialiseSignalsDatabase() {
    bool ret = (realTimeApplication != NULL);
    if (ret) {
        //Create the Functions master node
        ret = functionsDatabase.CreateAbsolute("Functions");
        if (ret) {
            ReferenceContainer gamsFound;
            //Look for all the GAMs inside the RealTimeApplication
            ReferenceContainerFilterReferencesTemplate<GAM> gamFilter(-1, ReferenceContainerFilterMode::RECURSIVE);

            //lint -e{613} realTimeApplication cannot be NULL. Value checked at the beginning of the function
            realTimeApplication->Find(gamsFound, gamFilter);
            uint32 numberOfGAMs = gamsFound.Size();
            uint32 i;
            for (i = 0u; (i < numberOfGAMs) && (ret); i++) {
                ConfigurationDatabase functionsDatabaseToModify = functionsDatabase;
                // build the GAM qualified name searching each GAM by path
                ReferenceT<GAM> gam = gamsFound.Get(i);
                StreamString qualifiedName = "";
                ReferenceContainerFilterReferences qualifiedNameFilter(1, ReferenceContainerFilterMode::PATH, gam);
                ReferenceContainer result;
                //lint -e{613} realTimeApplication cannot be NULL. Value checked at the beginning of the function
                realTimeApplication->Find(result, qualifiedNameFilter);
                //Store the fully qualified name (Functions in the sub-levels of the tree might have the same name)
                uint32 r;
                uint32 pathLength = result.Size();
                //r == 0 => Functions
                for (r = 1u; (r < pathLength) && ret; r++) {
                    Reference element = result.Get(r);
                    ReferenceT<GAM> gamElement = element;
                    if (gamElement.IsValid()) {
                        ret = (r == (pathLength - 1u));
                        if (!ret) {
                            REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Unsupported nested GAMs in path %s", qualifiedName.Buffer());
                        }
                    }
                    StreamString name = element->GetName();
                    if (r > 1u) {
                        ret = qualifiedName.Printf(".", voidAnyType);
                    }
                    if (ret) {
                        ret = qualifiedName.Printf("%s", name.Buffer());
                    }
                }
                //Having the fully qualified name add a new node to the Function node, where the name of the node is the index of the
                //Function (GAM) and the fully qualified name is stored as a property.
                StreamString functionN;
                if (ret) {
                    const uint32 constI = i;
                    ret = functionN.Printf("%d", constI);
                }
                if (ret) {
                    ret = functionsDatabaseToModify.CreateRelative(functionN.Buffer());
                }
                if (ret) {
                    ret = functionsDatabaseToModify.Write("QualifiedName", qualifiedName.Buffer());
                }
                if (ret) {
                    ret = functionsIndexesCache.Write(qualifiedName.Buffer(), functionN.Buffer());
                }
                if (ret) {
                    //functionsDatabaseToModify = functionsDatabase;
                    //Declare all the signals with which this GAM interacts
                    ret = gam->AddSignals(functionsDatabaseToModify);
                    if (ret) {
                        ret = functionsDatabaseToModify.MoveRelative("Signals");
                        if (ret) {
                            if (!functionsDatabaseToModify.MoveRelative("InputSignals")) {
                                ret = functionsDatabaseToModify.MoveRelative("OutputSignals");
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Specified GAM %s with no input nor output", qualifiedName.Buffer());
                                }
                            }
                        }
                    }
                }
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Failed to AddSignals for %s", gam->GetName());
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
            //lint -e{613} realTimeApplication cannot be NULL. Value checked at the beginning of the function
            realTimeApplication->Find(dataSourcesFound, dataSourceFilter);
            uint32 numberOfDataSources = dataSourcesFound.Size();
            uint32 isTimeStamp = 0u;
            uint32 i;
            StreamString timeStampDsName;
            for (i = 0u; (i < numberOfDataSources) && (ret); i++) {
                ReferenceT<DataSourceI> dataSource = dataSourcesFound.Get(i);

                StreamString qualifiedName = "";
                ReferenceContainerFilterReferences qualifiedNameFilter(1, ReferenceContainerFilterMode::PATH, dataSource);
                ReferenceContainer result;
                //lint -e{613} realTimeApplication cannot be NULL. Value checked at the beginning of the function
                realTimeApplication->Find(result, qualifiedNameFilter);
                //Store the fully qualified name (DataSources in sub-levels of the tree might have the same name)
                uint32 r;
                uint32 pathLength = result.Size();
                for (r = 1u; (r < pathLength) && (ret); r++) {
                    StreamString name = result.Get(r)->GetName();
                    if (r > 1u) {
                        ret = qualifiedName.Printf(".", voidAnyType);
                    }
                    if (ret) {
                        ret = qualifiedName.Printf("%s", name.Buffer());
                    }
                }
                //Having the fully qualified name add a new node to the Data node, where the name of the node is the index of the
                //DataSource and the fully qualified name is stored as a property.
                StreamString dataSourceN;
                if (ret) {
                    const uint32 constI = i;
                    ret = dataSourceN.Printf("%d", constI);
                }
                if (ret) {
                    ret = dataSourcesDatabase.CreateRelative(dataSourceN.Buffer());
                }
                if (ret) {
                    ret = dataSourcesDatabase.Write("QualifiedName", qualifiedName.Buffer());
                }
                if (ret) {
                    ret = dataSourcesIndexesCache.Write(qualifiedName.Buffer(), dataSourceN.Buffer());
                }
                if (ret) {
                    ret = dataSourcesSignalIndexCache.CreateAbsolute(dataSourceN.Buffer());
                }
                if (ret) {
                    //Declare all the signals with which this GAM interacts
                    ret = dataSource->AddSignals(dataSourcesDatabase);
                }
                const ClassProperties *prop = NULL_PTR(const ClassProperties *);
                if (ret) {
                    prop = dataSource->GetClassProperties();
                    ret = (prop != NULL);
                }
                if (ret) {
                    //lint -e{613} prop cannot be NULL as it is checked just before this line.
                    StreamString className = prop->GetName();
                    if (className == "TimingDataSource") {
                        isTimeStamp++;
                        timeStampDsName = dataSourceN;
                    }
                }
                if (ret) {
                    //Move to the next DataSource
                    ret = dataSourcesDatabase.MoveAbsolute("Data");
                }
            }
            if (ret) {
                ret = (isTimeStamp == 1u);
                if (isTimeStamp > 1u) {
                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Only one TimingDataSource per application allowed");
                }
                else if (isTimeStamp == 0u) {
                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Please specify a TimingDataSource to store GAMs relevant times");
                }
                else {
                    ret = dataSourcesDatabase.MoveRelative(timeStampDsName.Buffer());
                    if (ret) {
                        uint32 isTimingDataSource;
                        if (!dataSourcesDatabase.Read("IsTimingDataSource", isTimingDataSource)) {
                            isTimingDataSource = 1u;
                            ret = dataSourcesDatabase.Write("IsTimingDataSource", isTimingDataSource);
                        }
                    }
                }
            }
        }

        if (ret) {
            ret = dataSourcesDatabase.MoveToRoot();
        }

    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Invalid RealTimeApplication set");
    }

    return ret;
}

////////////////////////////////
////////////////////////////////
// FlattenSignalsDatabases
////////////////////////////////
////////////////////////////////
//-Flats all the signals as long their type are declared
bool RealTimeApplicationConfigurationBuilder::FlattenSignalsDatabases() {
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Caching introspection signals");
    bool ret = CacheAllSignalsIntrospections();
    if (ret) {
        ret = functionsDatabase.MoveAbsolute("Functions");
    }
    if (ret) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Flattening functions input signals");
        ret = FlattenSignalsDatabase(functionsDatabase, InputSignals);
    }
    if (ret) {
        ret = functionsDatabase.MoveAbsolute("Functions");
    }
    if (ret) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Flattening functions output signals");
        ret = FlattenSignalsDatabase(functionsDatabase, OutputSignals);
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveAbsolute("Data");
    }
    if (ret) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Flattening data sources signals");
        ret = FlattenSignalsDatabase(dataSourcesDatabase, None);
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveAbsolute("Data");
    }
    if (ret) {
        uint32 n;
        uint32 nOfDataSources = dataSourcesDatabase.GetNumberOfChildren();
        for (n = 0u; (n < nOfDataSources) && (ret); n++) {
            ret = dataSourcesDatabase.MoveToChild(n);
            if (ret) {
                ret = dataSourcesSignalIndexCache.MoveAbsolute(dataSourcesDatabase.GetName());
            }
            if (ret) {
                ret = dataSourcesDatabase.MoveRelative("Signals");
            }
            if (ret) {
                uint32 s;
                uint32 nSignals = dataSourcesDatabase.GetNumberOfChildren();
                for (s = 0u; (s < nSignals) && (ret); s++) {
                    ret = dataSourcesDatabase.MoveToChild(s);
                    StreamString qname;
                    if (ret) {
                        ret = dataSourcesDatabase.Read("QualifiedName", qname);
                    }
                    if (ret) {
                        const uint32 ss = s;
                        ret = dataSourcesSignalIndexCache.Write(qname.Buffer(), ss);
                    }
                    if (ret) {
                        ret = dataSourcesDatabase.MoveToAncestor(1u);
                    }
                }
            }
            if (ret) {
                ret = dataSourcesDatabase.MoveToAncestor(2u);
            }
        }
    }

    return ret;
}

bool RealTimeApplicationConfigurationBuilder::FlattenSignalsDatabase(ConfigurationDatabase &signalDatabase,
                                                                     const SignalDirection direction) {
    const char8 *signalDirection;
    if (direction == InputSignals) {
        signalDirection = "InputSignals";
    }
    else if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }
    else {
        signalDirection = "Signals";
    }

    bool isFunctionsDatabase = (&signalDatabase == &functionsDatabase);

    uint32 signalNumber;
    bool ret = true;
    //A signal container can be either a GAM or a DataSource
    uint32 numberOfSignalContainers = signalDatabase.GetNumberOfChildren();
    uint32 i;
    //For each signal container...
    ConfigurationDatabase originalSignalDatabase = signalDatabase;
    for (i = 0u; (i < numberOfSignalContainers) && (ret); i++) {
        signalDatabase = originalSignalDatabase;
        // move inside GAM or DataSource
        ret = signalDatabase.MoveToChild(i);
        if (ret) {
            // move inside "Signals" field
            ret = signalDatabase.MoveRelative("Signals");
        }
        bool exists = ret;
        if (ret) {
            if (isFunctionsDatabase) {
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
                //ReferenceT<ReferenceContainer> signalList = signalDatabase.GetCurrentNode();
                uint32 j = 0u;
                //...then for each signal...
                while ((j < numberOfSignals) && (ret)) {
                    //const char8 *signalName = signalList->Get(j)->GetName();
                    const char8 *signalName = signalDatabase.GetChildName(j);
                    if (StringHelper::Compare(signalName, "Locked") != 0) {
                        ConfigurationDatabase signalDatabaseBeforeFlatten = signalDatabase;
                        ret = signalDatabase.MoveToChild(j);
                        if (ret) {
                            //resolve the full signal properties (recursing if need to signal namespace and introspection)
                            ret = FlattenSignal(isFunctionsDatabase, signalName, signalLocalDatabase, signalNumber);
                        }
                        if (ret) {
                            //Move to the next Signal
                            signalDatabase = signalDatabaseBeforeFlatten;
                        }
                    }
                    else {
                        if (!isFunctionsDatabase) {
                            //this is valid only for datasources!!
                            uint32 locked;
                            if (signalDatabase.Read("Locked", locked)) {
                                ret = signalDatabase.Delete("Locked");
                                if (ret) {
                                    if (locked != 0u) {
                                        ret = signalDatabase.MoveToAncestor(1u);
                                        if (ret) {
                                            ret = signalDatabase.Write("Locked", locked);
                                        }
                                        if (ret) {
                                            ret = signalDatabase.MoveRelative("Signals");
                                        }
                                    }
                                }
                                numberOfSignals--;
                                j--;
                            }
                        }
                    }
                    j++;
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
                REPORT_ERROR(ErrorManagement::Debug, "Updating the signal database");
                ret = signalLocalDatabase.Copy(signalDatabase);
                REPORT_ERROR(ErrorManagement::Debug, "Finished updating the signal database");
            }
        }
    }

    return ret;
}

bool RealTimeApplicationConfigurationBuilder::FlattenSignal(const bool isFunctionsDatabase,
                                                            const char8 *const signalName,
                                                            ConfigurationDatabase &resolvedSignal,
                                                            uint32 &signalNumber) {
    bool ret = true;
    StreamString signalType;

    ConfigurationDatabase &signalDatabase = (isFunctionsDatabase) ? (functionsDatabase) : (dataSourcesDatabase);

    //A node does not have the field Type specified and must contain at least another node (namespace or signal)
    bool signalTypeDefined = signalDatabase.Read("Type", signalType);
    if (signalTypeDefined) {
        signalTypeDefined = (signalType.Size() > 0u);
    }
    //Check if it is a pure node (i.e. a nested namespace which contains other signals)
    bool foundANode = false;
    if (!signalTypeDefined) {
        uint32 numberOfElements = signalDatabase.GetNumberOfChildren();
        uint32 n;
        //ReferenceT<ReferenceContainer> elementsList = signalDatabase.GetCurrentNode();
        for (n = 0u; (n < numberOfElements); n++) {
            StreamString elementName = signalDatabase.GetChildName(n);
            //If this element is a node then recurse
            // "MemberAliases" and "Defaults" the only node can be found in a signal. Mark it as a keyword.
            //if (elementsList->Get(n)->IsReferenceContainer()) {
            ConfigurationDatabase signalDatabaseBeforeMove = signalDatabase;
            if (signalDatabase.MoveToChild(n)) {
                //ret = signalDatabase.MoveToChild(n);
                if (StringHelper::Compare(elementName.Buffer(), "MemberAliases") != 0) {
                    if (StringHelper::Compare(elementName.Buffer(), "Defaults") != 0) {
                        foundANode = true;
                        StreamString signalFullName;
                        if (ret) {
                            ret = signalFullName.Printf("%s.%s", signalName, elementName.Buffer());
                        }

                        if (ret) {
                            ret = FlattenSignal(isFunctionsDatabase, signalFullName.Buffer(), resolvedSignal, signalNumber);
                        }
                        if (ret) {
                            signalDatabase = signalDatabaseBeforeMove;
                        }
                    }
                }
            }
        }
    }
    //Namespace resolved. Working at signal level
    ConfigurationDatabase signalDatabaseBeforeChanges = signalDatabase;
    if (ret && (!foundANode)) {
        bool isStructuredData = false;
        StreamString dataSourceName;
        if (isFunctionsDatabase) {
            //This is a signal in the GAMs and the DataSource must be known now.
            if (!signalDatabase.Read("DataSource", dataSourceName)) {
                if (defaultDataSourceName.Size() > 0u) {
                    dataSourceName = defaultDataSourceName;
                    ret = signalDatabase.Write("DataSource", dataSourceName);
                }
                else {
                    ret = false;
                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "DataSource not specified for %s and DefaultDataSource not specified",
                                        signalName);
                }
            }
        }
        StreamString fullType;
        if ((ret) && (signalTypeDefined)) {
            TypeDescriptor signalTypeDescriptor = TypeDescriptor::GetTypeDescriptorFromTypeName(signalType.Buffer());
            //...check if it is a BasicType. If it is not, discombobulate it to its BasicTypes
            StreamString alias;
            if (!signalDatabase.Read("Alias", alias)) {
                alias = "";
            }

            {
                StreamString tempAlias = alias;
                if (tempAlias.Size() == 0u) {
                    tempAlias = signalName;
                }

                ret = tempAlias.Seek(0LLU);
                if (ret) {
                    while (tempAlias.SkipTokens(1u, ".")) {
                        fullType += "Node.";
                    }
                    fullType += signalType.Buffer();
                }
            }

            if (signalTypeDescriptor == InvalidType) {
                uint32 numberOfDimensions = 0u;
                uint32 numberOfElements = 1u;
                if (signalDatabase.Read("NumberOfDimensions", numberOfDimensions)) {
                    ret = (numberOfDimensions <= 3u);
                    if (!ret) {
                        REPORT_ERROR_STATIC(ErrorManagement::InitialisationError,
                                            "Invalid NumberOfDimensions for signal %s. Structured types only support NumberOfDimensions <= 1", signalName);
                    }
                }
                //TODO the NumberOfElements will have to be read as an array!
                if (signalDatabase.Read("NumberOfElements", numberOfElements)) {
                    if (numberOfElements > 1u) {
                        numberOfDimensions = 1u;
                    }
                }
                StreamString fsignalName = "";
                StreamString fAlias = "";
                uint32 nr;
                uint32 nc;
                uint32 nz;
                uint32 nOfRows = numberOfElements;
                uint32 nOfCols = 1u;            //TODO change when the NumberOfElements will be read as an array...
                uint32 nOfZ = 1u; //TODO change when the NumberOfElements will be read as an array...
                if (nOfRows == 0u) {
                    nOfRows = 1u;
                }
                /*if (nOfCols == 0u) { //TODO enable when the NumberOfElements will be read as an array...
                 nOfCols = 1u;
                 }*/
                /*if (nOfZ == 0u) { //TODO enable when the NumberOfElements will be read as an array...
                 nOfZ = 1u;
                 }*/
                /*lint -e{850} nr, nc and nz are not modified by the Print*/
                for (nr = 0u; (nr < nOfRows) && (ret); nr++) {
                    for (nc = 0u; (nc < nOfCols) && (ret); nc++) {
                        for (nz = 0u; (nz < nOfZ) && (ret); nz++) {
                            isStructuredData = true;
                            fsignalName = "";
                            fAlias = "";
                            if (numberOfDimensions <= 1u) {
                                if (numberOfElements > 1u) {
                                    (void) fsignalName.Printf("%s[%d]", signalName, nr);
                                    if (alias.Size() > 0u) {
                                        (void) fAlias.Printf("%s[%d]", alias.Buffer(), nr);
                                    }
                                }
                                else {
                                    fsignalName = signalName;
                                    fAlias = alias;
                                }
                            }
                            else if (numberOfDimensions == 2u) {
                                (void) fsignalName.Printf("%s[%d][%d]", signalName, nr, nc);
                                if (alias.Size() > 0u) {
                                    (void) fAlias.Printf("%s[%d][%d]", alias.Buffer(), nr, nc);
                                }
                            }
                            else if (numberOfDimensions == 3u) {
                                (void) fsignalName.Printf("%s[%d][%d][%d]", signalName, nr, nc, nz);
                                if (alias.Size() > 0u) {
                                    (void) fAlias.Printf("%s[%d][%d][%d]", alias.Buffer(), nr, nc, nz);
                                }
                            }
                            else { //NOOP
                            }
                            AnyType ranges = signalDatabase.GetType("Ranges");
                            AnyType samples = signalDatabase.GetType("Samples");
                            AnyType frequency = signalDatabase.GetType("Frequency");
                            AnyType trigger = signalDatabase.GetType("Trigger");
                            StreamString syncSignalName;
                            StreamString triggerSignalName;
                            bool syncSet = true;
                            bool triggerSet = true;
                            if (signalDatabase.Read("SyncSignal", syncSignalName)) {
                                ret = !frequency.IsVoid();
                                if (ret) {
                                    // both sync signal and frequency specified... check if the member will be found
                                    syncSet = false;
                                }
                                else {
                                    REPORT_ERROR_STATIC(
                                            ErrorManagement::InitialisationError,
                                            "Specified a synchronising signal %s with no synchronised frequency in structured %s. Please define the \"Frequency\" field",
                                            syncSignalName.Buffer(), fsignalName.Buffer());
                                }
                            }
                            else {
                                ret = frequency.IsVoid();
                                if (!ret) {
                                    REPORT_ERROR_STATIC(
                                            ErrorManagement::InitialisationError,
                                            "Specified a frequency with no synchronised signal in structured %s. Please define the \"SyncSignal\" field",
                                            fsignalName.Buffer());

                                }
                            }
                            if (signalDatabase.Read("TriggerSignal", triggerSignalName)) {
                                ret = !trigger.IsVoid();
                                if (ret) {
                                    // both sync signal and trigger specified... check if the member will be found
                                    triggerSet = false;
                                }
                                else {
                                    REPORT_ERROR_STATIC(
                                            ErrorManagement::InitialisationError,
                                            "Specified a TriggerSignal %s with no synchronised trigger in structured %s. Please define the \"Trigger = 1\" field",
                                            triggerSignalName.Buffer(), fsignalName.Buffer());
                                }
                            }
                            else {
                                ret = trigger.IsVoid();
                                if (!ret) {
                                    REPORT_ERROR_STATIC(
                                            ErrorManagement::InitialisationError,
                                            "Specified Trigger = 1 with no trigger signal in structured %s. Please define the \"TriggerSignal\" field",
                                            fsignalName.Buffer());

                                }
                            }

                            if (ret) {
                                REPORT_ERROR_STATIC(ErrorManagement::Debug, "Calling SignalIntrospectionToStructuredData for %s [%s]", fsignalName.Buffer(),
                                                    signalType.Buffer());
                                ret = SignalIntrospectionToStructuredData(signalDatabase, signalType.Buffer(), fsignalName.Buffer(), fAlias.Buffer(),
                                                                          dataSourceName.Buffer(), syncSignalName.Buffer(), triggerSignalName.Buffer(),
                                                                          fullType.Buffer(), ranges, samples, frequency, trigger, resolvedSignal, signalNumber,
                                                                          syncSet, triggerSet, isFunctionsDatabase);
                            }
                            if (ret) {
                                ret = syncSet;
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Invalid synchronising member specified in %s",
                                                        fsignalName.Buffer());
                                }
                            }
                            if (ret) {
                                ret = triggerSet;
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Invalid trigger member specified in %s", fsignalName.Buffer());
                                }
                            }
                        }
                    }
                }
                if (ret) {
                    //The original structured type is no longer needed
                    ret = signalDatabase.Delete("Type");
                }
            }
        }
        //BasicType. Write all the known signal properties
        // do this also if the type is undefined
        if ((ret) && (!isStructuredData)) {
            StreamString signalNumberStr;
            ret = signalNumberStr.Printf("Signals.%d", signalNumber);
            if (ret) {
                ret = resolvedSignal.CreateAbsolute(signalNumberStr.Buffer());
            }
            if (ret) {
                ret = resolvedSignal.Write("QualifiedName", signalName);
            }
            //Loop and copy all known properties at this time.
            const char8 *properties[] = { "Type", "NumberOfDimensions", "NumberOfElements", "Alias", "Ranges", "DataSource", "Samples", "Default", "Frequency",
                    "Trigger", NULL_PTR(char8*) };
            uint32 p = 0u;
            while ((properties[p] != NULL_PTR(char8*)) && (ret)) {
                AnyType element = signalDatabase.GetType(properties[p]);
                if (element.GetTypeDescriptor() != VoidType) {
                    ret = resolvedSignal.Write(properties[p], element);
                }
                p++;
            }
            signalNumber++;
            if (ret) {
                if (fullType.Size() > 0u) {
                    ret = resolvedSignal.Write("FullType", fullType.Buffer());
                }
            }
            if (ret) {
                //Move to the next Signal
                signalDatabase = signalDatabaseBeforeChanges;
            }
        }
    }
    return ret;
}

////////////////////////////////
////////////////////////////////
// ResolveDataSources
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::ResolveDataSources() {

    bool ret = functionsDatabase.MoveAbsolute("Functions");

    if (ret) {
        ret = dataSourcesDatabase.MoveAbsolute("Data");
    }
    /*lint -e{850} i is not modified by the REPORT_ERROR*/
    if (ret) {
        //Go to each function
        uint32 numberOfFunctions = functionsDatabase.GetNumberOfChildren();
        uint32 i;
        ConfigurationDatabase functionsDatabaseBeforeMove = functionsDatabase;
        for (i = 0u; (i < numberOfFunctions) && (ret); i++) {

            functionsDatabase = functionsDatabaseBeforeMove;
            ret = functionsDatabase.MoveToChild(i);
            StreamString functionName;
            if (ret) {
                // read the qualified name just for error reporting
                ret = functionsDatabase.Read("QualifiedName", functionName);
            }
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Resolving for function %s [idx: %d]", functionName.Buffer(), i);
            StreamString functionId;
            if (ret) {
                functionId = functionsDatabase.GetName();
            }
            ConfigurationDatabase functionDatabaseFunction = functionsDatabase;

            if (ret) {
                ret = functionsDatabase.MoveRelative("Signals");
            }

            SignalDirection directions[] = { InputSignals, OutputSignals };
            for (uint32 j = 0u; j < 2u; j++) {
                bool exists = false;
                const char8 *signalDirection = NULL_PTR(const char8*);
                if (ret) {
                    signalDirection = "InputSignals";
                    if (directions[j] == OutputSignals) {
                        signalDirection = "OutputSignals";
                    }

                    exists = functionsDatabase.MoveRelative(signalDirection);
                }
                if (exists) {
                    uint32 totalByteSize = 0u;
                    uint32 allocatedByteSize = 0u;
                    //...and for each signal...
                    uint32 numberOfSignals = functionsDatabase.GetNumberOfChildren();
                    //ReferenceT<ReferenceContainer> signalsContainer = functionsDatabase.GetCurrentNode();
                    ConfigurationDatabase functionsDatabaseBeforeSignalMove = functionsDatabase;
                    REPORT_ERROR_STATIC(ErrorManagement::Information, "Resolving %d signals", numberOfSignals);
                    for (uint32 s = 0u; (s < numberOfSignals) && (ret); s++) {
                        functionsDatabase = functionsDatabaseBeforeSignalMove;
                        ret = functionsDatabase.MoveToChild(s);

                        StreamString signalId;
                        if (ret) {
                            signalId = functionsDatabase.GetName();
                        }
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
                            //it leaves you inside the signal
                            ret = FindDataSourceNumber(dataSourceName, dataSourceNumber);
                        }
                        if (ret) {
                            ret = functionsDatabase.Write("DataSourceNumber", dataSourceNumber);
                        }
                        if (ret) {
                            ret = dataSourcesDatabase.MoveAbsolute("Data");
                        }
                        if (ret) {
                            ret = dataSourcesDatabase.MoveRelative(dataSourceNumber.Buffer());
                        }
                        if (ret) {
                            cachedDatabase = dataSourcesDatabase;
                            if (!dataSourcesDatabase.MoveRelative("Signals")) {
                                ret = dataSourcesDatabase.CreateRelative("Signals");
                            }
                        }
                        if (ret) {
                            //Add the signal to the Data.dataSourceName node (if the Type is defined)
                            ret = AddSignalToDataSource(functionName, dataSourceName);
                            uint32 numberOfElements = 0u;
                            if (!functionsDatabase.Read("NumberOfElements", numberOfElements)) {
                                numberOfElements = 1u;
                                ret = functionsDatabase.Write("NumberOfElements", numberOfElements);
                            }
                            if (ret) {
                                uint32 numberOfDimensions = 0u;
                                if (!functionsDatabase.Read("NumberOfDimensions", numberOfDimensions)) {
                                    numberOfDimensions = 0u;
                                    ret = functionsDatabase.Write("NumberOfDimensions", numberOfDimensions);
                                }
                            }
                        }
                        if (ret) {
                            ConfigurationDatabase temp = functionDatabaseFunction;
                            if (temp.MoveRelative("States")) {
                                uint32 numberOfStates = temp.GetNumberOfChildren();
                                if (numberOfStates > 0u) {
                                    Vector<StreamString> states(numberOfStates);
                                    for (uint32 st = 0u; (st < numberOfStates) && (ret); st++) {
                                        StreamString stateName = temp.GetChildName(st);
                                        states[st] = stateName;
                                    }
                                    ret = ResolveConsumersAndProducers((directions[j] == InputSignals), states, functionId.Buffer(), functionName.Buffer(),
                                                                       signalId.Buffer());
                                }
                            }
                        }
                        if (ret) {
                            ret = ResolveFunctionSignalsMemorySize(functionName.Buffer());
                        }
                        if (ret) {
                            ret = ResolveFunctionsMemory(directions[j], functionDatabaseFunction, totalByteSize, allocatedByteSize, functionName.Buffer());
                        }
                        if (!ret) {
                            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to resolve for data source %s and function %s", dataSourceName.Buffer(),
                                                functionName.Buffer());
                        }
                    }
                    if (ret) {
                        functionsDatabase = functionsDatabaseBeforeSignalMove;
                        ret = functionsDatabase.Write("ByteSize", totalByteSize);
                    }
                }
                if (ret) {
                    functionsDatabase = functionDatabaseFunction;
                    ret = functionsDatabase.MoveRelative("Signals");
                }
            }
            if (ret) {
                functionsDatabase = functionDatabaseFunction;
                ret = AssignFunctionsMemoryToDataSource(InputSignals, functionName.Buffer());
                if (ret) {
                    ret = AssignFunctionsMemoryToDataSource(OutputSignals, functionName.Buffer());
                }
                if (ret) {
                    ret = functionsDatabase.MoveRelative("Signals");
                }
            }

        }
    }

    return ret;
}

bool RealTimeApplicationConfigurationBuilder::FindDataSourceNumber(StreamString dataSourceName,
                                                                   StreamString &dataSourceNumber) {
    bool ret = dataSourcesIndexesCache.Read(dataSourceName.Buffer(), dataSourceNumber);
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::AddSignalToDataSource(StreamString functionName,
                                                                    StreamString dataSourceName) {

    StreamString originalSignalName;

    bool ret = functionsDatabase.Read("QualifiedName", originalSignalName);
    StreamString signalName;
//Expand the aliasName in order to write the correct name in the dataSourcesDatabase
    StreamString aliasName;

    if (ret) {
        if (!functionsDatabase.Read("Alias", aliasName)) {
            aliasName = "";
        }
        if (aliasName.Size() > 0u) {
            signalName = aliasName;
        }
        // if alias unspecified, the name of the dataSource is the GAM signal name
        else {
            signalName = originalSignalName;
        }
    }

    StreamString fullType;
    if (ret) {
        if (!functionsDatabase.Read("FullType", fullType)) {
            fullType = "";
        }
    }

    StreamString dataSourceNumber;
    if (ret) {
        ret = FindDataSourceNumber(dataSourceName, dataSourceNumber);
    }
    bool isDsLocked = false;
    if (ret) {
        //check if the ds is locked
        dataSourcesDatabase = cachedDatabase;
        uint32 locked;
        if (dataSourcesDatabase.Read("Locked", locked)) {
            isDsLocked = (locked != 0u);
        }

        ret = dataSourcesDatabase.MoveRelative("Signals");
    }

    //Check if the signal already exists
    uint32 foundSignalId = 0u;
    uint32 numberOfSignals = dataSourcesDatabase.GetNumberOfChildren();
    bool signalAlreadyExists = false;
    if (ret) {
        ret = dataSourcesSignalIndexCache.MoveAbsolute(dataSourceNumber.Buffer());
    }
    if (ret) {
        signalAlreadyExists = dataSourcesSignalIndexCache.Read(signalName.Buffer(), foundSignalId);
    }
    if (ret) {
        ConfigurationDatabase dataSourcesDatabaseBeforeMove = dataSourcesDatabase;
        if (signalAlreadyExists) {
            dataSourcesDatabase = dataSourcesDatabaseBeforeMove;
            ret = dataSourcesDatabase.MoveToChild(foundSignalId);
            StreamString dataSourceSignalName;
            if (ret) {
                ret = dataSourcesDatabase.Read("QualifiedName", dataSourceSignalName);
            }
            StreamString otherFullType;

            if (dataSourcesDatabase.Read("FullType", otherFullType)) {
                bool sizeFullTypeOK = (fullType.Size() > 0u);
                bool sizeOtherFullTypeOK = (otherFullType.Size() > 0u);
                if (sizeFullTypeOK && sizeOtherFullTypeOK) {
                    ret = signalName.Seek(0LLU);
                    if (ret) {
                        ret = dataSourceSignalName.Seek(0LLU);
                    }
                    if (ret) {
                        ret = fullType.Seek(0LLU);
                    }
                    if (ret) {
                        ret = otherFullType.Seek(0LLU);
                    }
                    if (ret) {
                        ret = CheckTypeCompatibility(fullType, otherFullType, signalName, dataSourceSignalName);
                    }
                }
            }
            if (!ret) {
                REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "The data source assigned to the signal %s in %s is incompatible",
                                    originalSignalName.Buffer(), functionName.Buffer());
            }
        }
        if (ret) {
            if (signalAlreadyExists) {
                dataSourcesDatabase = dataSourcesDatabaseBeforeMove;
                ret = dataSourcesDatabase.MoveToChild(foundSignalId);
            }
        }
    }
    if ((!signalAlreadyExists) && (ret)) {
        //If the signal still does not exist in the dataSourcesDatabase create it.
        ret = (!isDsLocked);
        StreamString signalId;
        if (ret) {
            ret = signalId.Printf("%d", numberOfSignals);
        }
        if (ret) {
            ret = dataSourcesDatabase.CreateRelative(signalId.Buffer());
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Cannot add the signal %s in GAM %s because the related DataSource is locked",
                                originalSignalName.Buffer(), functionName.Buffer());
        }
        if (ret) {
            ret = dataSourcesDatabase.Write("QualifiedName", signalName.Buffer());
        }
        if (ret) {
            ret = dataSourcesSignalIndexCache.MoveAbsolute(dataSourceNumber.Buffer());
        }
        if (ret) {
            ret = dataSourcesSignalIndexCache.Write(signalName.Buffer(), numberOfSignals);
        }
    }
    if (ret) {
        //Loop through all properties.
        const char8 *properties[] = { "Type", "NumberOfDimensions", "NumberOfElements", "Default", "MemberSize", NULL_PTR(char8*) };
        uint32 p = 0u;
        while ((properties[p] != NULL_PTR(char8*)) && (ret)) {
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
                        ret = (StringHelper::Compare(sElementSignalDatabase.Buffer(), sElementDataSourceDatabase.Buffer()) == 0);
                    }
                    if (!ret) {
                        //Report mismatch!
                        StreamString fullPropertyName;
                        bool retPrintf = fullPropertyName.Printf("%s.%s", originalSignalName.Buffer(), properties[p]);
                        if (!retPrintf) {
                            fullPropertyName = "Unknown";
                        }
                        REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Mismatch in signal with name: %s: %s asked for %s while %s asked for %s",
                                            fullPropertyName.Buffer(), functionName.Buffer(), sElementSignalDatabase.Buffer(), dataSourceName.Buffer(),
                                            sElementDataSourceDatabase.Buffer());
                    }
                }
                else {
                    //This was a misinterpretation of the requirement. A Locked DataSource means that you cannot add a new signal not
                    //that you cannot complete the properties of an existent signal!
                    /*ret = (!isDsLocked);*/
                    /*if (ret) {*/
                    ret = dataSourcesDatabase.Write(properties[p], elementSignalDatabase);
                    /*}*/
                    /*else {
                     REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Cannot complete the signal %s in GAM %s because the related DataSource is locked", originalSignalName.Buffer(),
                     functionName.Buffer());
                     }*/
                }
            }
            else {
                if (elementDataSourceDatabase.GetTypeDescriptor() != VoidType) {
                    ret = functionsDatabase.Write(properties[p], elementDataSourceDatabase);
                }
            }
            p++;
        }

        StreamString dsSignalFullType;
        bool replace = (!dataSourcesDatabase.Read("FullType", dsSignalFullType));

        if ((!replace) && (ret)) {
            ret = dsSignalFullType.Seek(0LLU);
            if (fullType.Size() > 0u) {
                if (ret) {
                    ret = fullType.Seek(0LLU);
                }
                StreamString token1;
                StreamString token2;
                bool go = true;
                char8 terminator;
                while (go && ret) {
                    go = dsSignalFullType.GetToken(token1, ".", terminator);
                    if (go) {
                        ret = fullType.GetToken(token2, ".", terminator);
                        if (ret) {
                            bool token1Node = (token1 == "Node");
                            bool token2Node = (token2 == "Node");
                            replace = (token1Node) && (!token2Node);
                            go = (token1Node) && (token2Node);
                        }
                    }
                    token1 = "";
                    token2 = "";
                }
            }
        }
        if (replace && ret) {
            StreamString exists;
            if (dataSourcesDatabase.Read("FullType", exists)) {
                ret = dataSourcesDatabase.Delete("FullType");
            }
            if (fullType.Size() > 0u) {
                if (ret) {
                    ret = dataSourcesDatabase.Write("FullType", fullType.Buffer());
                }
            }
        }
    }
    return ret;
}

////////////////////////////////
////////////////////////////////
// VerifyDataSourcesSignals
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::VerifyDataSourcesSignals() {
    bool ret = dataSourcesDatabase.MoveAbsolute("Data");
    uint32 numberOfDataSources = 0u;
    if (ret) {
        numberOfDataSources = dataSourcesDatabase.GetNumberOfChildren();
    }
    if (ret) {
        uint32 i;
        ConfigurationDatabase dataSourcesDatabaseBeforeMove = dataSourcesDatabase;
        for (i = 0u; (i < numberOfDataSources) && (ret); i++) {
            StreamString dataSourceName;
            dataSourcesDatabase = dataSourcesDatabaseBeforeMove;
            ret = dataSourcesDatabase.MoveToChild(i);

            if (ret) {
                ret = dataSourcesDatabase.Read("QualifiedName", dataSourceName);
            }
            if (ret) {
                ret = dataSourcesDatabase.MoveRelative("Signals");
            }
            REPORT_ERROR(ErrorManagement::Information, "Verifying signals for %s", dataSourceName.Buffer());
            uint32 numberOfSignals = 0u;
            if (ret) {
                numberOfSignals = dataSourcesDatabase.GetNumberOfChildren();
            }
            uint32 s = 0u;
            ConfigurationDatabase dataSourcesDatabaseBeforeSignalMove = dataSourcesDatabase;
            while ((s < numberOfSignals) && (ret)) {
                dataSourcesDatabase = dataSourcesDatabaseBeforeSignalMove;
                ret = dataSourcesDatabase.MoveToChild(s);
                StreamString signalId = dataSourcesDatabase.GetName();

                //The QualifiedName must be known
                StreamString signalName;
                if (ret) {
                    ret = dataSourcesDatabase.Read("QualifiedName", signalName);
                    if (ret) {
                        ret = (signalName.Size() > 0u);
                    }
                    if (!ret) {
                        REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "QualifiedName was not defined for signal at position: %s in %s",
                                            signalId.Buffer(), dataSourceName.Buffer());
                    }
                }
                StreamString type;
                bool typeDef = dataSourcesDatabase.Read("Type", type);
                if (typeDef) {
                    typeDef = (type.Size() > 0u);
                }

                bool signalFound = false;
                if (ret) {
                    dataSourcesDatabase = dataSourcesDatabaseBeforeSignalMove;

                    //Do this only for structured types.
                    TypeDescriptor td = TypeDescriptor::GetTypeDescriptorFromTypeName(type.Buffer());
                    if (td == InvalidType) {
                        // check if it is a partial definition. Delete if the type is not specified, error otherwise (empty node)
                        ConfigurationDatabase dataSourcesDatabaseBeforeSignalMoveOther = dataSourcesDatabase;
                        for (uint32 k = 0u; (k < numberOfSignals) && (!signalFound) && (ret); k++) {
                            if (k != s) {
                                char8 terminator;
                                ret = dataSourcesDatabase.MoveToChild(k);
                                StreamString otherSName;
                                //The QualifiedName must be known
                                if (ret) {
                                    ret = dataSourcesDatabase.Read("QualifiedName", otherSName);
                                    if (ret) {
                                        ret = otherSName.Seek(0LLU);
                                    }
                                }
                                if (ret) {
                                    // ret becomes true if found a partial def
                                    StreamString signalNameStr = signalName;
                                    ret = signalNameStr.Seek(0LLU);
                                    StreamString token1;
                                    StreamString token2;
                                    signalFound = true;
                                    while (signalNameStr.GetToken(token1, ".", terminator) && (signalFound) && (ret)) {
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
                                if (ret) {
                                    dataSourcesDatabase = dataSourcesDatabaseBeforeSignalMoveOther;
                                }
                            }
                        }
                    }
                }

                bool validSignalFound = false;
                if (ret) {
                    if (signalFound) {
                        ret = !typeDef;
                        if (ret) {
                            ret = dataSourcesDatabase.Delete(signalId.Buffer());
                            //resume
                            if (ret) {
                                numberOfSignals = dataSourcesDatabase.GetNumberOfChildren();
                                s--;
                                validSignalFound = true;
                            }
                        }
                    }
                    else {
                        ret = typeDef;
                        if (ret) {
                            ret = dataSourcesDatabase.MoveRelative(signalId.Buffer());
                        }
                        else {
                            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Undefined type for signal %s in %s", signalName.Buffer(),
                                                dataSourceName.Buffer());

                        }
                    }
                }
                if (!validSignalFound) {
                    uint32 numberOfElements = 0u;
                    if (ret) {
                        if (!dataSourcesDatabase.Read("NumberOfElements", numberOfElements)) {
                            numberOfElements = 1u;
                            ret = dataSourcesDatabase.Write("NumberOfElements", numberOfElements);
                        }
                        else {
                            ret = numberOfElements > 0u;
                            if (!ret) {
                                REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "NumberOfElements of signal %s in %s cannot be zero",
                                                    signalName.Buffer(), dataSourceName.Buffer());

                            }
                        }
                    }

                    //If the number of dimensions is > 1 then the NumberOfElements must also be defined.
                    uint32 numberOfDimensions = 0u;
                    if (ret) {
                        if (!dataSourcesDatabase.Read("NumberOfDimensions", numberOfDimensions)) {
                            numberOfDimensions = 0u;
                            if (numberOfElements > 1u) {
                                REPORT_ERROR_STATIC(
                                        ErrorManagement::Warning,
                                        "NumberOfDimensions is not defined for signal: %s in %s with NumberOfElements > 1, assuming it as 1 (vector)",
                                        signalName.Buffer(), dataSourceName.Buffer());
                                numberOfDimensions = 1u;
                            }
                            ret = dataSourcesDatabase.Write("NumberOfDimensions", numberOfDimensions);
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
                        signalNumberOfBytes = ((numberOfElements * signalTypeDescriptor.numberOfBits) / 8u);
                        ret = dataSourcesDatabase.Write("ByteSize", signalNumberOfBytes);
                    }

                    if (ret) {
                        ret = WriteDefault(dataSourcesDatabase, signalName.Buffer(), dataSourceName.Buffer(), signalTypeDescriptor, numberOfDimensions,
                                           numberOfElements, signalNumberOfBytes);
                    }
                }
                s++;
            }
        }
    }
    return ret;
}

////////////////////////////////
////////////////////////////////
// ResolveFunctionSignals
////////////////////////////////
////////////////////////////////

////////////////////////////////
////////////////////////////////
// VerifyFunctionSignals
////////////////////////////////
////////////////////////////////

////////////////////////////////
////////////////////////////////
// ResolveStates
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::ResolveStates() {

    bool ret = (realTimeApplication != NULL);
    if (ret) {
        ret = ConfigureThreads();
        if (ret) {
            ReferenceContainer statesFound;
            //Look for all the RealTimeStates
            ReferenceContainerFilterReferencesTemplate<RealTimeState> stateFilter(-1, ReferenceContainerFilterMode::RECURSIVE);
            /*lint -e{613} NULL pointer checking done before entering here */
            realTimeApplication->Find(statesFound, stateFilter);
            uint32 numberOfStates = statesFound.Size();
            //Loop on each state...
            for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
                ReferenceT<RealTimeState> state = statesFound.Get(i);
                const char8 * stateName = state->GetName();
                REPORT_ERROR(ErrorManagement::Information, "Resolving state %s", stateName);
                //Look for a ReferenceContainer named Threads
                uint32 j;
                bool found = false;
                ReferenceT<ReferenceContainer> threadContainer;
                /*lint -e{613} NULL pointer checking done before entering here */
                for (j = 0u; (j < realTimeApplication->Size()) && (!found); j++) {
                    threadContainer = state->Get(j);
                    if (threadContainer.IsValid()) {
                        found = (StringHelper::Compare("Threads", threadContainer->GetName()) == 0);
                        REPORT_ERROR(ErrorManagement::Information, "Resolving thread container %s", threadContainer->GetName());
                    }
                }
                ret = found;
                if (ret) {
                    uint32 numberOfThreads = threadContainer->Size();
                    //...get the threads which belong to this state...
                    for (j = 0u; (j < numberOfThreads) && (ret); j++) {
                        uint32 syncSignals = 0u;
                        ReferenceT<RealTimeThread> thread = threadContainer->Get(j);
                        ret = thread.IsValid();
                        if (ret) {
                            const char8 * threadName = thread->GetName();
                            StreamString threadFullName = stateName;
                            threadFullName += ".";
                            threadFullName += threadName;
                            REPORT_ERROR(ErrorManagement::Information, "Resolving thread %s", threadFullName.Buffer());

                            ret = AddThreadCycleTime(threadFullName.Buffer());
                            ReferenceContainer gams;
                            if (ret) {
                                ret = thread->GetGAMs(gams);
                            }
                            uint32 numberOfGAMs = 0u;
                            if (ret) {
                                numberOfGAMs = thread->GetNumberOfGAMs();
                            }
                            uint32 z;
                            //...get the GAMs that are executed by this thread....
                            for (z = 0u; (z < numberOfGAMs) && (ret); z++) {
                                ReferenceT<GAM> gam = gams.Get(z);
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
                                        ret = qualifiedName.Printf(".", voidAnyType);
                                    }
                                    if (ret) {
                                        ret = qualifiedName.Printf("%s", name.Buffer());
                                    }
                                }
                                REPORT_ERROR(ErrorManagement::Information, "Resolving %s", qualifiedName.Buffer());
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
                                    syncSignals += GetNumberOfSyncSignals("Signals.InputSignals", ret);
                                    if (ret) {
                                        syncSignals += GetNumberOfSyncSignals("Signals.OutputSignals", ret);
                                    }
                                }
                                if (ret) {
                                    if (!functionsDatabase.MoveRelative("States")) {
                                        ret = functionsDatabase.CreateRelative("States");
                                    }
                                }
                                if (ret) {
                                    // the same gam cannot be declared in two different threads!!
                                    AnyType atStateThread = functionsDatabase.GetType(stateName);
                                    ret = (atStateThread.IsVoid());
                                    if (ret) {
                                        ret = functionsDatabase.Write(stateName, threadName);
                                    }
                                    else {
                                        if(realTimeApplication->CheckSameGamInMoreThreads()) {
                                            StreamString presentThread;
                                            ret = functionsDatabase.Read(stateName, presentThread);
                                            if (ret) {
                                                ret = (presentThread == threadName);
                                                if (!ret) {
                                                    StreamString gamName;
                                                    if (functionsDatabase.MoveToAncestor(1u)) {
                                                        if (!functionsDatabase.Read("QualifiedName", gamName)) {
                                                            gamName = "UnknownGAM";
                                                        }
                                                    }
                                                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "The GAM %s is declared in more than one thread in %s", gamName.Buffer(), stateName);
                                                }
                                            }
                                        }
                                        else {
                                            uint32 numberOfThreadsGAM = atStateThread.GetNumberOfElements(0u);
                                            Vector<StreamString> threadsGAM(numberOfThreadsGAM+1u);
                                            uint32 stateNDims=atStateThread.GetNumberOfDimensions();
                                            if(stateNDims == 0u) {
                                                StreamString curThreadsGAM;
                                                ret = functionsDatabase.Read(stateName, curThreadsGAM);
                                                threadsGAM[0u] = curThreadsGAM;
                                            }
                                            else {

                                                Vector<StreamString> curThreadsGAM(numberOfThreadsGAM);
                                                ret = functionsDatabase.Read(stateName, curThreadsGAM);
                                                for(uint32 n=0u; n<numberOfThreadsGAM; n++) {
                                                    threadsGAM[n] = curThreadsGAM[n];
                                                }
                                            }

                                            threadsGAM[numberOfThreadsGAM] = threadName;
                                            if (ret) {
                                                ret = functionsDatabase.Delete(stateName);
                                            }
                                            if(ret) {
                                                ret = functionsDatabase.Write(stateName, threadsGAM);
                                            }
                                        }
                                    }
                                }
                            }
                            if (ret) {
                                ret = (syncSignals <= 1u);
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "More than one synchronising signal found in %s.%s", stateName, threadName);
                                }
                            }
                        }
                    }
                }
                else {
                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "No Threads ReferenceContainer found in RealTimeState %s", stateName);
                }
            }
        }
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Invalid RealTimeApplication set");
    }

    return ret;
}

bool RealTimeApplicationConfigurationBuilder::AddStateToGAM(const char8 *const gamNameIn,
                                                            const char8 *const stateName,
                                                            const char8 *const threadName,
                                                            uint32 &syncSignals,
                                                            const bool checkSameGamInMoreThreads) {
    StreamString functionNumber;
    StreamString gamNameStr = gamNameIn;

    bool ret = FindFunctionNumber(gamNameStr, functionNumber);
    if (ret) {
        ret = functionsDatabase.MoveAbsolute("Functions");
    }
    if (ret) {
        ret = functionsDatabase.MoveRelative(functionNumber.Buffer());
    }
    if (ret) {
        syncSignals += GetNumberOfSyncSignals("Signals.InputSignals", ret);
        if (ret) {
            syncSignals += GetNumberOfSyncSignals("Signals.OutputSignals", ret);
        }
    }
    if (ret) {
        if (!functionsDatabase.MoveRelative("States")) {
            ret = functionsDatabase.CreateRelative("States");
        }
    }
    if (ret) {
        // the same gam cannot be declared in two different threads!!
        AnyType atStateThread = functionsDatabase.GetType(&stateName[1]);
        ret = (atStateThread.IsVoid());
        if (ret) {
            ret = functionsDatabase.Write(&stateName[1], &threadName[1]);
        }
        else {
            if (checkSameGamInMoreThreads) {
                StreamString presentThread;
                ret = functionsDatabase.Read(&stateName[1], presentThread);
                if (ret) {
                    ret = (presentThread == &threadName[1]);
                    if (!ret) {
                        StreamString gamName;
                        if (functionsDatabase.MoveToAncestor(1u)) {
                            if (!functionsDatabase.Read("QualifiedName", gamName)) {
                                gamName = "UnknownGAM";
                            }
                        }
                        REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "The GAM %s is declared in more than one thread in %s", gamName.Buffer(),
                                            stateName);
                    }
                }
            }
            else {
                uint32 numberOfThreadsGAM = atStateThread.GetNumberOfElements(0u);
                Vector<StreamString> threadsGAM(numberOfThreadsGAM + 1u);
                uint32 stateNDims = atStateThread.GetNumberOfDimensions();
                if (stateNDims == 0u) {
                    StreamString curThreadsGAM;
                    ret = functionsDatabase.Read(&stateName[1], curThreadsGAM);
                    threadsGAM[0u] = curThreadsGAM;
                }
                else {

                    Vector<StreamString> curThreadsGAM(numberOfThreadsGAM);
                    ret = functionsDatabase.Read(&stateName[1], curThreadsGAM);
                    for (uint32 n = 0u; n < numberOfThreadsGAM; n++) {
                        threadsGAM[n] = curThreadsGAM[n];
                    }
                }

                threadsGAM[numberOfThreadsGAM] = &threadName[1];
                if (ret) {
                    ret = functionsDatabase.Delete(&stateName[1]);
                }
                if (ret) {
                    ret = functionsDatabase.Write(&stateName[1], threadsGAM);
                }
            }
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::AddStateToFunction(ConfigurationDatabase &local,
                                                                 const char8 *const functionName,
                                                                 const char8 *const stateName,
                                                                 const char8 *const threadName,
                                                                 uint32 &syncSignals) {

    bool ret = local.MoveToRoot();
    bool checkSameGamInMoreThreads = true;
    if (ret) {
        uint8 checkSameGamInMoreThreadsT = 1u;
        if (!local.Read("CheckSameGamInMoreThreads", checkSameGamInMoreThreadsT)) {
            checkSameGamInMoreThreadsT = 1u;
        }
        checkSameGamInMoreThreads = (checkSameGamInMoreThreadsT > 0u);
    }
    StreamString functionPathStr = functionName;
    StreamString token = "+";
    char8 terminator;
    if (ret) {
        ret = local.MoveAbsolute("+Functions");
        if (!ret) {
            ret = local.MoveAbsolute("$Functions");
        }
    }

    if (ret) {
        ret = functionPathStr.Seek(0LLU);
    }
    while (functionPathStr.GetToken(token, ".", terminator) && (ret)) {
        ret = local.MoveRelative(token.Buffer());
        token = "+";
    }

    ConfigurationDatabase output;
    StreamString path = "";
    uint32 index = 0u;
    if (ret) {
        ret = SearchGAMs(local, output, path, index, false);
    }
    if (ret) {
        ret = output.MoveToRoot();
    }
    if (ret) {
        uint32 numberOfGAMs = output.GetNumberOfChildren();
        if (numberOfGAMs == 0u) {
            StreamString gamName = functionName;
            ret = AddStateToGAM(gamName.Buffer(), stateName, threadName, syncSignals, checkSameGamInMoreThreads);
        }
        else {
            uint32 i;
            //ReferenceT<ReferenceContainer> gamList = output.GetCurrentNode();
            ConfigurationDatabase outputBeforeMove = output;
            for (i = 0u; (i < numberOfGAMs) && (ret); i++) {
                output = outputBeforeMove;
                ret = output.MoveToChild(i);
                StreamString gamName = functionName;
                gamName += ".";
                if (ret) {
                    ret = output.Read("QualifiedName", gamName);
                }
                if (ret) {
                    ret = AddStateToGAM(gamName.Buffer(), stateName, threadName, syncSignals);
                }
            }
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::ResolveStatesFromConfiguration() {

    bool ret = globalDatabase.MoveToRoot();

    uint8 checkSameGamInMoreThreadsT = 1u;
    if (!globalDatabase.Read("CheckSameGamInMoreThreads", checkSameGamInMoreThreadsT)) {
        checkSameGamInMoreThreadsT = 1u;
    }

    if (ret) {
        ret = globalDatabase.MoveAbsolute("+Functions");
        if (!ret) {
            ret = globalDatabase.MoveAbsolute("$Functions");
        }
    }
    ConfigurationDatabase local;
    if (ret) {
        ret = local.Write("CheckSameGamInMoreThreads", checkSameGamInMoreThreadsT);
    }
    if (ret) {
        //ret = local.AddToCurrentNode(globalDatabase.GetCurrentNode());
        ret = local.CreateAbsolute(globalDatabase.GetName());
    }
    if (ret) {
        ret = globalDatabase.Copy(local);
    }
    if (ret) {
        ret = globalDatabase.MoveAbsolute("+States");
        if (!ret) {
            ret = globalDatabase.MoveAbsolute("$States");
        }
    }
    if (ret) {
        uint32 numberOfStates = globalDatabase.GetNumberOfChildren();
        uint32 i;
        for (i = 0u; (i < numberOfStates) && (ret); i++) {
            const char8 *stateName = globalDatabase.GetChildName(i);
            if (globalDatabase.MoveRelative(stateName)) {
                ret = globalDatabase.MoveRelative("+Threads");
                if (!ret) {
                    ret = globalDatabase.MoveAbsolute("$Threads");
                }
                if (ret) {
                    uint32 numberOfThreads = globalDatabase.GetNumberOfChildren();
                    uint32 j;
                    for (j = 0u; (j < numberOfThreads) && (ret); j++) {
                        const char8 *threadName = globalDatabase.GetChildName(j);
                        if (globalDatabase.MoveRelative(threadName)) {
                            StreamString threadFullName = &stateName[1];
                            threadFullName += ".";
                            threadFullName += &threadName[1];
                            ret = AddThreadCycleTime(threadFullName.Buffer());

                            AnyType at = globalDatabase.GetType("Functions");
                            if (ret) {
                                ret = !at.IsVoid();
                            }
                            if (ret) {
                                ret = (at.GetNumberOfDimensions() == 1u);
                                if (ret) {
                                    uint32 numberOfFunctions = at.GetNumberOfElements(0u);
                                    Vector<StreamString> functions(numberOfFunctions);
                                    ret = globalDatabase.Read("Functions", functions);
                                    uint32 syncSignals = 0u;
                                    uint32 k;
                                    for (k = 0u; (k < numberOfFunctions) && (ret); k++) {
                                        StreamString function = functions[k];
                                        ret = AddStateToFunction(local, function.Buffer(), stateName, threadName, syncSignals);
                                    }
                                    if (ret) {
                                        ret = (syncSignals <= 1u);
                                        if (!ret) {
                                            REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "More than one synchronising signal found in %s.%s",
                                                                &stateName[1], &threadName[1]);
                                        }
                                    }

                                }
                                else {
                                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "The Functions element must be defined as vector %s.%s",
                                                        &stateName[1], &threadName[1]);
                                }
                            }
                            else {
                                REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "The Functions element must be %s.%s", &stateName[1], &threadName[1]);
                            }
                            if (ret) {
                                ret = globalDatabase.MoveToAncestor(1u);
                            }
                        }

                    }
                    if (ret) {
                        ret = globalDatabase.MoveToAncestor(1u);
                    }
                }
                if (ret) {
                    ret = globalDatabase.MoveToAncestor(1u);
                }
            }

        }
    }

    return ret;
}

bool RealTimeApplicationConfigurationBuilder::AddThreadCycleTime(const char8 *const threadFullName) {
    bool ret = dataSourcesDatabase.MoveAbsolute("Data");
    uint32 numberOfDataSources = dataSourcesDatabase.GetNumberOfChildren();

    StreamString signalName = threadFullName;
    signalName += "_CycleTime";
    uint32 i;
    ConfigurationDatabase dataSourcesDatabaseBeforeMove = dataSourcesDatabase;
    for (i = 0u; (i < numberOfDataSources) && (ret); i++) {
        dataSourcesDatabase = dataSourcesDatabaseBeforeMove;
        ret = dataSourcesDatabase.MoveToChild(i);
        if (ret) {
            uint32 isTimingDataSource;
            if (dataSourcesDatabase.Read("IsTimingDataSource", isTimingDataSource)) {
                if (isTimingDataSource == 1u) {
                    ret = dataSourcesSignalIndexCache.MoveAbsolute(dataSourcesDatabase.GetName());
                    if (ret) {
                        ret = dataSourcesDatabase.MoveRelative("Signals");
                    }
                    if (ret) {
                        uint32 numberOfTimeSignals = dataSourcesDatabase.GetNumberOfChildren();
                        bool found = false;
                        uint32 j;
                        ConfigurationDatabase dataSourcesDatabaseBeforeTimeSignals = dataSourcesDatabase;
                        for (j = 0u; (j < numberOfTimeSignals) && (ret) && (!found); j++) {
                            ret = dataSourcesDatabase.MoveToChild(j);
                            StreamString qualifiedName;
                            if (ret) {
                                ret = dataSourcesDatabase.Read("QualifiedName", qualifiedName);
                            }
                            if (ret) {
                                found = (qualifiedName == signalName);
                                if (found) {
                                    ret = CheckTimeSignalInfo();
                                }
                            }
                            if (ret) {
                                dataSourcesDatabase = dataSourcesDatabaseBeforeTimeSignals;
                            }
                        }
                        if (ret && (!found)) {
                            uint32 numberOfSignals = dataSourcesDatabase.GetNumberOfChildren();
                            StreamString newSignalId;
                            ret = newSignalId.Printf("%d", numberOfSignals);
                            if (ret) {
                                ret = dataSourcesDatabase.CreateRelative(newSignalId.Buffer());
                            }
                            if (ret) {
                                ret = dataSourcesSignalIndexCache.Write(signalName.Buffer(), newSignalId.Buffer());
                            }
                            if (ret) {
                                ret = WriteTimeSignalInfo(signalName.Buffer());
                            }
                        }
                    }
                }
            }
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::WriteTimeSignalInfo(const char8 *const signalName) {
    bool ret = dataSourcesDatabase.Write("QualifiedName", signalName);
    if (ret) {
        ret = dataSourcesDatabase.Write("Type", "uint32");
    }
    if (ret) {
        uint32 numberOfDimensions = 0u;
        ret = dataSourcesDatabase.Write("NumberOfDimensions", numberOfDimensions);
    }
    if (ret) {
        uint32 numberOfElements = 1u;
        ret = dataSourcesDatabase.Write("NumberOfElements", numberOfElements);
    }
    if (ret) {
        uint32 byteSize = 4u;
        ret = dataSourcesDatabase.Write("ByteSize", byteSize);
    }
    if (ret) {
        StreamString signalNameStr = signalName;
        ret = signalNameStr.Seek(0LLU);
        StreamString fullType;
        while ((signalNameStr.SkipTokens(1u, ".")) && (ret)) {
            fullType += "Node.";
        }
        fullType += "uint32";
        ret = dataSourcesDatabase.Write("FullType", fullType.Buffer());
    }
    if (ret) {
        ret = dataSourcesDatabase.CreateRelative("States");
        if (ret) {
            ret = dataSourcesDatabase.MoveToAncestor(1u);
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::CheckTimeSignalInfo() {

    StreamString signalName;
    bool ret = dataSourcesDatabase.Read("QualifiedName", signalName);

    if (ret) {
        StreamString type;

        if (!dataSourcesDatabase.Read("Type", type)) {
            ret = dataSourcesDatabase.Write("Type", "uint32");
        }
        else {
            ret = (type == "uint32");
        }
    }
    if (ret) {
        StreamString fullTypeTest;
        ret = signalName.Seek(0LLU);
        while ((signalName.SkipTokens(1u, ".")) && (ret)) {
            fullTypeTest += "Node.";
        }
        fullTypeTest += "uint32";
        StreamString fullType;
        if (!dataSourcesDatabase.Read("FullType", fullType)) {
            ret = dataSourcesDatabase.Write("FullType", fullTypeTest.Buffer());
        }
        else {
            ret = (fullType == fullTypeTest);
        }
    }
    if (ret) {
        uint32 numberOfDimensions;
        if (!dataSourcesDatabase.Read("NumberOfDimensions", numberOfDimensions)) {
            numberOfDimensions = 0u;
            ret = dataSourcesDatabase.Write("NumberOfDimensions", numberOfDimensions);
        }
        else {
            ret = (numberOfDimensions == 0u);
        }
    }
    if (ret) {
        uint32 numberOfElements;
        if (!dataSourcesDatabase.Read("NumberOfElements", numberOfElements)) {
            numberOfElements = 1u;
            ret = dataSourcesDatabase.Write("NumberOfElements", numberOfElements);
        }
        else {
            ret = (numberOfElements == 1u);
        }
    }
    if (ret) {
        uint32 byteSize;
        if (!dataSourcesDatabase.Read("ByteSize", byteSize)) {
            byteSize = static_cast<uint32>(sizeof(uint32));
            ret = dataSourcesDatabase.Write("ByteSize", byteSize);
        }
        else {
            ret = (byteSize == sizeof(uint32));
        }
    }
    return ret;
}

////////////////////////////////
////////////////////////////////
// ResolveConsumersAndProducers
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::ResolveConsumersAndProducers(const bool consumers,
                                                                           Vector<StreamString> &states,
                                                                           const char8 *const functionId,
                                                                           const char8 *const functionName,
                                                                           const char8 *const signalId) {
    const char8 *operationType = "Consumers";
    if (!consumers) {
        operationType = "Producers";
    }
    //suppose to be already into the signal!!
    bool ret = true;
    if (!dataSourcesDatabase.MoveRelative("States")) {
        ret = dataSourcesDatabase.CreateRelative("States");
    }

    //For all the states
    ConfigurationDatabase dataSourcesDatabaseBeforeMove = dataSourcesDatabase;
    uint32 numberOfStates = states.GetNumberOfElements();
    for (uint32 s = 0u; (s < numberOfStates) && (ret); s++) {
        if (!dataSourcesDatabase.MoveRelative(states[s].Buffer())) {
            ret = dataSourcesDatabase.CreateRelative(states[s].Buffer());
        }
        if (ret) {
            //Check if the array Consumers (or Producers) already exists
            StreamString operationTypeGAM = "GAM";
            operationTypeGAM += operationType;
            StreamString operationTypeGAMNames = "GAMNames";
            operationTypeGAMNames += operationType;
            StreamString operationTypeSignal = "Signal";
            operationTypeSignal += operationType;
            AnyType existentArray = dataSourcesDatabase.GetType(operationTypeGAM.Buffer());
            StreamString *newGAMArray = NULL_PTR(StreamString*);
            StreamString *newGAMNamesArray = NULL_PTR(StreamString*);
            StreamString *newSignalArray = NULL_PTR(StreamString*);

            uint32 numberOfExistentElements = 0u;
            if (existentArray.GetDataPointer() != NULL_PTR(void*)) {
                numberOfExistentElements = existentArray.GetNumberOfElements(0u);
                const uint32 numberOfNewsElements = (numberOfExistentElements + 1u);
                newGAMArray = new StreamString[numberOfNewsElements];
                newGAMNamesArray = new StreamString[numberOfNewsElements];
                newSignalArray = new StreamString[numberOfNewsElements];
                Vector<StreamString> newGAMVector(newGAMArray, numberOfExistentElements);
                Vector<StreamString> newGAMNamesVector(newGAMNamesArray, numberOfExistentElements);
                Vector<StreamString> newSignalVector(newSignalArray, numberOfExistentElements);
                ret = (dataSourcesDatabase.Read(operationTypeGAM.Buffer(), newGAMVector));
                if (ret) {
                    ret = (dataSourcesDatabase.Read(operationTypeGAMNames.Buffer(), newGAMNamesVector));
                }
                if (ret) {
                    ret = (dataSourcesDatabase.Read(operationTypeSignal.Buffer(), newSignalVector));
                }
            }
            else {
                newGAMArray = new StreamString[1u];
                newGAMNamesArray = new StreamString[1u];
                newSignalArray = new StreamString[1u];
            }
            newGAMArray[numberOfExistentElements] = functionId;
            newGAMNamesArray[numberOfExistentElements] = functionName;
            newSignalArray[numberOfExistentElements] = signalId;
            Vector<StreamString> newGAMVector(newGAMArray, numberOfExistentElements + 1u);
            Vector<StreamString> newGAMNamesVector(newGAMNamesArray, numberOfExistentElements + 1u);
            Vector<StreamString> newSignalVector(newSignalArray, numberOfExistentElements + 1u);

            if (existentArray.GetDataPointer() != NULL_PTR(void*)) {
                ret = dataSourcesDatabase.Delete(operationTypeGAM.Buffer());
                if (ret) {
                    ret = dataSourcesDatabase.Delete(operationTypeGAMNames.Buffer());
                }
                if (ret) {
                    ret = dataSourcesDatabase.Delete(operationTypeSignal.Buffer());
                }
            }
            if (ret) {
                ret = (dataSourcesDatabase.Write(operationTypeGAM.Buffer(), newGAMVector));
                if (ret) {
                    ret = (dataSourcesDatabase.Write(operationTypeGAMNames.Buffer(), newGAMNamesVector));
                }
                if (ret) {
                    ret = (dataSourcesDatabase.Write(operationTypeSignal.Buffer(), newSignalVector));
                }
            }
            delete[] newGAMArray;
            delete[] newGAMNamesArray;
            delete[] newSignalArray;
        }
        //Move to the next state
        if (ret) {
            dataSourcesDatabase = dataSourcesDatabaseBeforeMove;
        }
    }

    if (ret) {
        ret = dataSourcesDatabase.MoveToAncestor(1u);
    }

    return ret;
}

////////////////////////////////
////////////////////////////////
// VerifyConsumersAndProducers
////////////////////////////////
////////////////////////////////
bool RealTimeApplicationConfigurationBuilder::VerifyConsumersAndProducers() {

    bool ret = true;
    bool checkMultipleProducersWrites = true;
    if (initialiseAfterInitialisation) {
        if (realTimeApplication != NULL_PTR(RealTimeApplication *)) {
            checkMultipleProducersWrites = realTimeApplication->CheckMultipleProducersWrites();
        }
    }
    else {
        ret = globalDatabase.MoveToRoot();
        if (ret) {
            uint8 checkMultipleProducersWritesT = 1u;
            if (!globalDatabase.Read("CheckMultipleProducersWrites", checkMultipleProducersWritesT)) {
                checkMultipleProducersWritesT = 1u;
            }
            checkMultipleProducersWrites = (checkMultipleProducersWritesT > 0u);
        }
    }

    if (ret) {
        ret = dataSourcesDatabase.MoveAbsolute("Data");
    }
    uint32 numberOfDS = dataSourcesDatabase.GetNumberOfChildren();
    ConfigurationDatabase dataSourcesDatabaseBeforeMove = dataSourcesDatabase;
    for (uint32 i = 0u; (i < numberOfDS) && (ret); i++) {
        bool isTimeDs = false;
        dataSourcesDatabase = dataSourcesDatabaseBeforeMove;
        ret = dataSourcesDatabase.MoveToChild(i);
        if (ret) {
            uint32 isTimingDataSource;
            if (dataSourcesDatabase.Read("IsTimingDataSource", isTimingDataSource)) {
                if (isTimingDataSource == 1u) {
                    isTimeDs = true;
                    ret = AddTimingSignals();
                }
            }
        }
        StreamString dataSourceName;
        if (ret) {
            ret = dataSourcesDatabase.Read("QualifiedName", dataSourceName);
        }
        if (ret) {
            ret = dataSourcesDatabase.MoveRelative("Signals");
        }
        REPORT_ERROR(ErrorManagement::Information, "Verifying consumers and producers for %s", dataSourceName.Buffer());
        if (ret) {
            uint32 numberOfSignals = dataSourcesDatabase.GetNumberOfChildren();
            ConfigurationDatabase dataSourcesDatabaseBeforeSignalMove = dataSourcesDatabase;
            for (uint32 j = 0u; (j < numberOfSignals) && (ret); j++) {
                dataSourcesDatabase = dataSourcesDatabaseBeforeSignalMove;
                ret = dataSourcesDatabase.MoveToChild(j);
                StreamString dsSignalName;
                if (ret) {
                    ret = dataSourcesDatabase.Read("QualifiedName", dsSignalName);
                }
                if (ret) {
                    if (!dataSourcesDatabase.MoveRelative("States")) {
                        ret = dataSourcesDatabase.CreateRelative("States");
                    }
                }
                if (ret) {
                    uint32 numberOfStates = dataSourcesDatabase.GetNumberOfChildren();
                    ConfigurationDatabase dataSourcesDatabaseBeforeState = dataSourcesDatabase;
                    for (uint32 k = 0u; (k < numberOfStates) && (ret); k++) {
                        dataSourcesDatabase = dataSourcesDatabaseBeforeState;
                        ret = dataSourcesDatabase.MoveToChild(k);
                        if (ret) {
                            // no Producers found... in this case the default value must be declared
                            AnyType prods = dataSourcesDatabase.GetType("GAMProducers");
                            if (isTimeDs) {
                                //the time data source must not have producers!!
                                ret = prods.IsVoid();
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "The time signal %s in %s cannot be produced", dsSignalName.Buffer(),
                                                        dataSourceName.Buffer());
                                }
                            }
                            else {
                                if (checkMultipleProducersWrites) {
                                    if (!prods.IsVoid()) {
                                        if (prods.GetNumberOfElements(0u) > 1u) {
                                            // Check the range overlap
                                            ret = BuildProducersRanges();
                                            if (!ret) {
                                                REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                                                                    "Producers of %s in %s overlap write operations on the same memory area",
                                                                    dsSignalName.Buffer(), dataSourceName.Buffer());
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::BuildProducersRanges() {

    AnyType prods = dataSourcesDatabase.GetType("GAMProducers");

    uint32 numberOfProducers = prods.GetNumberOfElements(0u);
    Vector<StreamString> producers(numberOfProducers);
    bool ret = dataSourcesDatabase.Read("GAMProducers", producers);
    Vector<StreamString> signalProducers(numberOfProducers);
    if (ret) {
        ret = dataSourcesDatabase.Read("SignalProducers", signalProducers);
    }

    uint32 minNumberOfSignalRanges = 2u * numberOfProducers;
    uint32 *signalRanges = new uint32[minNumberOfSignalRanges];
    if (ret) {
        ret = functionsDatabase.MoveAbsolute("Functions");
    }
    uint32 reachSignalRanges = 0u;
    for (uint32 i = 0u; (i < numberOfProducers) && (ret); i++) {
        ret = functionsDatabase.MoveRelative(producers[i].Buffer());
        if (ret) {
            StreamString signalPath = "Signals.OutputSignals.";
            signalPath += signalProducers[i];
            ret = functionsDatabase.MoveRelative(signalPath.Buffer());
            if (ret) {
                AnyType at = functionsDatabase.GetType("Ranges");
                ret = !at.IsVoid();
                if (ret) {
                    //has to be a matrix
                    ret = at.GetNumberOfDimensions() == 2u;
                }
                if (ret) {
                    uint32 rangeRows = at.GetNumberOfElements(1u);
                    uint32 rangeCols = at.GetNumberOfElements(0u);
                    Matrix<uint32> rangesMatrix = Matrix<uint32>(rangeRows, rangeCols);
                    ret = functionsDatabase.Read("Ranges", rangesMatrix);
                    for (uint32 k = 0u; (k < rangeRows) && (ret); k++) {
                        // allocate new memory if needed
                        if (reachSignalRanges >= (minNumberOfSignalRanges)) {
                            uint32 newSize = reachSignalRanges + 2u;
                            uint32 *temp = new uint32[newSize];
                            uint32 copySize = minNumberOfSignalRanges * static_cast<uint32>(sizeof(uint32));
                            ret = MemoryOperationsHelper::Copy(temp, signalRanges, copySize);
                            minNumberOfSignalRanges = newSize;
                            delete[] signalRanges;
                            signalRanges = temp;
                        }
                        if (ret) {
                            signalRanges[reachSignalRanges] = rangesMatrix(k, 0u);
                            reachSignalRanges++;
                            signalRanges[reachSignalRanges] = rangesMatrix(k, 1u);
                            reachSignalRanges++;
                        }
                    }
                }
            }
        }
        if (ret) {
            ret = functionsDatabase.MoveAbsolute("Functions");
        }
    }
    if (ret) {
        ret = CheckProducersRanges(signalRanges, reachSignalRanges);
    }

    delete[] signalRanges;

    return ret;
}

bool RealTimeApplicationConfigurationBuilder::CheckProducersRanges(const uint32 *const rangesArray,
                                                                   const uint32 numberOfElements) const {
    bool ret = ((numberOfElements % 2u) == 0u);
    for (uint32 i = 1u; (i < numberOfElements) && ret; i += 2u) {
        for (uint32 j = i + 2u; (j < numberOfElements) && (ret); j += 2u) {
            bool isFirstMin = (rangesArray[i - 1u] < rangesArray[j - 1u]) && (rangesArray[i] < rangesArray[j - 1u]);
            bool isFirstMax = (rangesArray[j - 1u] < rangesArray[i - 1u]) && (rangesArray[j] < rangesArray[i - 1u]);
            ret = (isFirstMin || isFirstMax);
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::AddTimingSignals() {
    bool ret = dataSourcesDatabase.MoveRelative("Signals");
    if (ret) {
        ret = functionsDatabase.MoveAbsolute("Functions");
    }
    if (ret) {
        uint32 numberOfFunctions = functionsDatabase.GetNumberOfChildren();
        uint32 i;
        ConfigurationDatabase functionsDatabaseBeforeMove = functionsDatabase;
        for (i = 0u; (i < numberOfFunctions) && (ret); i++) {
            functionsDatabase = functionsDatabaseBeforeMove;
            ret = functionsDatabase.MoveToChild(i);

            StreamString functionName;
            if (ret) {
                ret = functionsDatabase.Read("QualifiedName", functionName);
            }

            const char8 *postfix[] = { "_ReadTime", "_ExecTime", "_WriteTime", NULL_PTR(const char8*) };
            for (uint32 k = 0u; (postfix[k] != NULL) && ret; k++) {
                StreamString signalNameStr = functionName;
                signalNameStr += postfix[k];
                uint32 numberOfPresentSignals = dataSourcesDatabase.GetNumberOfChildren();
                bool found = false;
                uint32 j;
                ConfigurationDatabase dataSourcesDatabaseBeforeSignalMove = dataSourcesDatabase;
                for (j = 0u; (j < numberOfPresentSignals) && (ret) && (!found); j++) {
                    dataSourcesDatabase = dataSourcesDatabaseBeforeSignalMove;
                    ret = dataSourcesDatabase.MoveToChild(j);
                    StreamString toAdd;
                    if (ret) {
                        ret = dataSourcesDatabase.Read("QualifiedName", toAdd);
                    }
                    if (ret) {
                        found = (toAdd == signalNameStr);
                        if (found) {
                            ret = CheckTimeSignalInfo();
                        }
                    }
                }
                if (ret) {
                    dataSourcesDatabase = dataSourcesDatabaseBeforeSignalMove;
                }
                if ((!found) && (ret)) {
                    StreamString newSignalIdx;
                    uint32 nextIndex = dataSourcesDatabase.GetNumberOfChildren();
                    ret = newSignalIdx.Printf("%d", nextIndex);
                    ConfigurationDatabase dataSourcesDatabaseBeforeCreate = dataSourcesDatabase;
                    if (ret) {
                        ret = dataSourcesDatabase.CreateRelative(newSignalIdx.Buffer());
                    }
                    if (ret) {
                        ret = WriteTimeSignalInfo(signalNameStr.Buffer());
                    }
                    if (ret) {
                        dataSourcesDatabase = dataSourcesDatabaseBeforeCreate;
                    }
                }
            }
        }
        //Adding of the CurrentStateSignal
        if (ret) {
            StreamString currentStateIdx;
            uint32 currentStateNextIndex = dataSourcesDatabase.GetNumberOfChildren();
            ret = currentStateIdx.Printf("%d", currentStateNextIndex);
            ConfigurationDatabase dataSourcesDatabaseBeforeCreate;

            if (ret) {
                dataSourcesDatabaseBeforeCreate = dataSourcesDatabase;
                ret = dataSourcesDatabase.CreateRelative(currentStateIdx.Buffer());
            }
            if (ret) {
                ret = WriteTimeSignalInfo("CurrentState");
            }
            if (ret) {
                dataSourcesDatabase = dataSourcesDatabaseBeforeCreate;
            }
        }

        if (ret) {
            ret = dataSourcesDatabase.MoveToAncestor(1u);
        }

    }
    return ret;
}

////////////////////////////////
////////////////////////////////
// ResolveFunctionSignalsMemorySize
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::ResolveFunctionSignalsMemorySize(const char8 *const functionName) {

    uint32 signalNumberOfBytes = 0u;
    uint32 numberOfElements = 0u;
    TypeDescriptor signalTypeDescriptor;
    StreamString signalId;
    StreamString signalType;
    StreamString signalName;
    signalId = functionsDatabase.GetName();
    bool ret = functionsDatabase.Read("NumberOfElements", numberOfElements);
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

    uint32 *offsetMatrixBackend = NULL_PTR(uint32*);
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
                    const uint32 backendRangesSize = numberOfRanges * 2u;
                    uint32 *rangesMatBackend = new uint32[backendRangesSize];
                    offsetMatrixBackend = new uint32[backendRangesSize];
                    //The offset matrix stores, for each range and in bytes, the starting offset and the size of the range to copy.
                    Matrix<uint32> offsetMat(offsetMatrixBackend, numberOfRanges, 2u);
                    //Read the Ranges matrix from the configuration data.
                    Matrix<uint32> rangesMat(rangesMatBackend, numberOfRanges, 2u);
                    ret = functionsDatabase.Read("Ranges", rangesMat);
                    if (ret) {
                        for (uint32 n = 0u; (n < numberOfRanges) && (ret); n++) {
                            uint32 minIdx = rangesMat(n, 0u);
                            uint32 maxIdx = rangesMat(n, 1u);
                            if (minIdx > maxIdx) {
                                ret = false;
                                REPORT_ERROR_STATIC(ErrorManagement::InitialisationError,
                                                    "Illegal Ranges for signal %s in %s: minimum index must be <= maximum index", signalName.Buffer(),
                                                    functionName);
                            }
                            if (ret) {
                                if (maxIdx >= numberOfElements) {
                                    ret = false;
                                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError,
                                                        "Illegal Ranges for signal %s in %s: maximum index must be < NumberOfElements", signalName.Buffer(),
                                                        functionName);
                                }
                            }
                            if (ret) {
                                uint32 rangeByteSize = (((maxIdx - minIdx) + 1u) * signalTypeDescriptor.numberOfBits) / 8u;
                                signalNumberOfBytes += rangeByteSize;
                                offsetMat(n, 0u) = (minIdx * signalTypeDescriptor.numberOfBits) / 8u;
                                offsetMat(n, 1u) = rangeByteSize;
                            }
                        }
                    }
                    else {
                        REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Ranges must be a nx2 matrix for %s in %s", signalName.Buffer(),
                                            functionName);
                    }
                    delete[] rangesMatBackend;
                }
            }
        }
        else {
            signalNumberOfBytes = (numberOfElements * signalTypeDescriptor.numberOfBits) / 8u;
        }
    }

    if (ret) {
        uint32 elementOffset = 0u;

        if (functionsDatabase.Read("MemberSize", elementOffset)) {
            elementOffset = ((numberOfElements * (signalTypeDescriptor.numberOfBits)) / 8u) - elementOffset;
            //allocate memory without considering ranges because it is considered as a struct
            signalNumberOfBytes = (numberOfElements * signalTypeDescriptor.numberOfBits) / 8u;
        }
        else {
            elementOffset = 0u;
        }
        signalNumberOfBytes += elementOffset;
    }
    if (ret) {
        ret = functionsDatabase.Write("ByteSize", signalNumberOfBytes);
    }
    if (ret) {
        if (offsetMatrixBackend != NULL_PTR(uint32*)) {
            Matrix<uint32> offsetMat(offsetMatrixBackend, numberOfRanges, 2u);
            ret = functionsDatabase.Write("ByteOffset", offsetMat);
        }
    }
    if (offsetMatrixBackend != NULL_PTR(uint32*)) {
        delete[] offsetMatrixBackend;
    }
    return ret;
}

////////////////////////////////
////////////////////////////////
// ResolveFunctionsMemory
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::ResolveFunctionsMemory(const SignalDirection direction,
                                                                     const ConfigurationDatabase &functionsDatabaseAfterMove,
                                                                     uint32 &totalByteSize,
                                                                     uint32 &allocatedByteSize,
                                                                     const char8 *const functionName) {

    const char8 *signalDirection = "InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }

    StreamString signalId;
    StreamString signalName;
    StreamString alias;
    StreamString dataSourceName;
    uint32 numberOfOffsetElements = 0u;
    uint32 *offsetMatrixBackend = NULL_PTR(uint32*);
    uint32 samplesBackend = 1u;
    uint32 byteSize = 0u;
    float32 frequencyBackend = -1.0F;
    uint32 triggerBackend = 0u;

    ConfigurationDatabase beforeOperations = functionsDatabase;

    signalId = functionsDatabase.GetName();
    bool ret = functionsDatabase.Read("QualifiedName", signalName);
    if (ret) {
        if (!functionsDatabase.Read("Alias", alias)) {
            alias = signalName;
        }
    }

    if (ret) {
        ret = functionsDatabase.Read("DataSource", dataSourceName);
    }

    if (ret) {
        if (!functionsDatabase.Read("MemberSize", byteSize)) {
            ret = functionsDatabase.Read("ByteSize", byteSize);
        }
    }

    if (ret) {
        AnyType existentByteOffset = functionsDatabase.GetType("ByteOffset");
        if (existentByteOffset.GetDataPointer() != NULL_PTR(void*)) {
            numberOfOffsetElements = existentByteOffset.GetNumberOfElements(1u);
            // create a static matrix
            const uint32 offsetMatrixBackendNElements = numberOfOffsetElements * 2u;
            offsetMatrixBackend = new uint32[offsetMatrixBackendNElements];
            Matrix<uint32> offsetMat(offsetMatrixBackend, numberOfOffsetElements, 2u);
            ret = functionsDatabase.Read("ByteOffset", offsetMat);
        }
        else {
            numberOfOffsetElements = 1u;
            offsetMatrixBackend = new uint32[2u];
            offsetMatrixBackend[0] = 0u;
            offsetMatrixBackend[1] = byteSize;
        }

    }
    if (ret) {
        if (!functionsDatabase.Read("Samples", samplesBackend)) {
            samplesBackend = 1u;
        }
        else {
            ret = (samplesBackend > 0u);
            if (!ret) {
                REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Invalid Samples for signal %s in %s", signalName.Buffer(), functionName);
            }
        }
    }
    if (ret) {
        totalByteSize += (byteSize * samplesBackend);

        if (!functionsDatabase.Read("Frequency", frequencyBackend)) {
            frequencyBackend = -1.0F;
        }
        if (!functionsDatabase.Read("Trigger", triggerBackend)) {
            triggerBackend = 0u;
        }
        if (triggerBackend != 1u) {
            triggerBackend = 0u;
        }
    }
    //Move to the function level
    if (ret) {
        functionsDatabase = functionsDatabaseAfterMove;

        if (!functionsDatabase.MoveRelative("Memory")) {
            ret = functionsDatabase.CreateRelative("Memory");
        }
    }
    if (ret) {
        if (!functionsDatabase.MoveRelative(signalDirection)) {
            ret = functionsDatabase.CreateRelative(signalDirection);
        }
    }
    //Check if a DataSource with this name already exists..
    if (ret) {
        uint32 numberOfDataSources = functionsDatabase.GetNumberOfChildren();
        bool found = functionsMemoryIndexesCache.MoveAbsolute(functionName);
        if (found) {
            found = functionsMemoryIndexesCache.MoveRelative(signalDirection);
        }
        if (found) {
            StreamString dataSourceIdx;
            found = functionsMemoryIndexesCache.Read(dataSourceName.Buffer(), dataSourceIdx);
            if (found) {
                ret = functionsDatabase.MoveRelative(dataSourceIdx.Buffer());
            }
        }
        if (!found) {
            StreamString newDataSourceId;
            ret = newDataSourceId.Printf("%d", numberOfDataSources);
            if (ret) {
                ret = functionsDatabase.CreateRelative(newDataSourceId.Buffer());
            }
            if (ret) {
                ret = functionsDatabase.Write("DataSource", dataSourceName.Buffer());
            }
            if (ret) {
                if (!functionsMemoryIndexesCache.MoveAbsolute(functionName)) {
                    ret = functionsMemoryIndexesCache.CreateAbsolute(functionName);
                }
                if (!functionsMemoryIndexesCache.MoveRelative(signalDirection)) {
                    ret = functionsMemoryIndexesCache.CreateRelative(signalDirection);
                }
            }
            if (ret) {
                ret = functionsMemoryIndexesCache.Write(dataSourceName.Buffer(), newDataSourceId.Buffer());
            }
        }
    }
    uint32 existentByteSize = 0u;
    if (ret) {
        if (functionsDatabase.Read("ByteSize", existentByteSize)) {
            ret = functionsDatabase.Delete("ByteSize");
        }
    }

    ConfigurationDatabase beforeSignals = functionsDatabase;
    //Store the relevant signals belonging to this DataSource
    if (ret) {
        if (!functionsDatabase.MoveRelative("Signals")) {
            ret = functionsDatabase.CreateRelative("Signals");
        }
    }
    if (ret) {
        ret = functionsDatabase.CreateRelative(signalId.Buffer());
    }
    if (ret) {
        ret = functionsDatabase.Write("QualifiedName", signalName.Buffer());
    }
    if (ret) {
        ret = functionsDatabase.Write("Alias", alias.Buffer());
    }
    if (ret) {
        if (offsetMatrixBackend != NULL_PTR(void*)) {
            Matrix<uint32> offsetMat(offsetMatrixBackend, numberOfOffsetElements, 2u);
            ret = functionsDatabase.Write("ByteOffset", offsetMat);
        }
    }
    if (offsetMatrixBackend != NULL_PTR(void*)) {
        delete[] offsetMatrixBackend;
    }
    if (ret) {
        ret = functionsDatabase.Write("Samples", samplesBackend);
    }

    if (ret) {
        ret = functionsDatabase.Write("Frequency", frequencyBackend);
    }

    if (ret) {
        ret = functionsDatabase.Write("Trigger", triggerBackend);
    }

    if (ret) {
        ret = functionsDatabase.Write("GAMMemoryOffset", allocatedByteSize);
    }

    if (ret) {
        functionsDatabase = beforeSignals;
    }

    if (ret) {
        existentByteSize += byteSize * samplesBackend;
        allocatedByteSize += byteSize * samplesBackend;
        ret = functionsDatabase.Write("ByteSize", existentByteSize);
    }

    if (ret) {
        functionsDatabase = beforeOperations;
    }
    return ret;
}

////////////////////////////////
////////////////////////////////
// AssignFunctionsMemoryToDataSource
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::AssignFunctionsMemoryToDataSource(const SignalDirection direction,
                                                                                const char8 *const functionName) {
    const char8 *signalDirection = "InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }

    ConfigurationDatabase functionsDatabaseBeforeMove = functionsDatabase;
    bool ret = functionsDatabase.MoveRelative("Memory");
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
        ConfigurationDatabase functionsDatabaseBeforeMoveDataSource = functionsDatabase;
        for (d = 0u; (d < numberOfDataSources) && (ret); d++) {
            uint32 byteSize = 0u;
            StreamString dataSourceName;
            functionsDatabase = functionsDatabaseBeforeMoveDataSource;
            ret = functionsDatabase.MoveToChild(d);
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
            StreamString dsName;
            ReferenceT<DataSourceI> dataSource;
            if (ret) {
                ret = dataSourcesDatabase.MoveAbsolute("Data");
            }
            if (ret) {
                ret = dataSourcesDatabase.MoveRelative(dataSourceIdInDataSourceDatabase.Buffer());
            }
            if (ret) {
                ret = dataSourcesDatabase.Read("QualifiedName", dsName);
            }
            if (ret) {
                if (initialiseAfterInitialisation) {
                    StreamString fullDsPath = "Data.";
                    fullDsPath += dsName;
                    /*lint -e{613} NULL pointer checking done before entering here */
                    dataSource = realTimeApplication->Find(fullDsPath.Buffer());
                    ret = dataSource.IsValid();
                    if (!ret) {
                        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "%s is not a valid DataSourceI", fullDsPath.Buffer());
                    }
                }
            }
            if (ret) {
                if (!dataSourcesDatabase.MoveRelative("Functions")) {
                    ret = dataSourcesDatabase.CreateRelative("Functions");
                }
            }

            if (ret) {
                //Check if this function name already exists
                bool found = dataSourcesFunctionIndexesCache.MoveAbsolute(dsName.Buffer());
                StreamString functionIdIdx;
                if (found) {
                    found = dataSourcesFunctionIndexesCache.Read(functionName, functionIdIdx);
                }
                if (found) {
                    ret = dataSourcesDatabase.MoveRelative(functionIdIdx.Buffer());
                }
                if (!found) {
                    StreamString newFunctionId;
                    uint32 numberOfDataSourceFunctions = dataSourcesDatabase.GetNumberOfChildren();
                    ret = newFunctionId.Printf("%d", numberOfDataSourceFunctions);
                    if (ret) {
                        ret = dataSourcesDatabase.CreateRelative(newFunctionId.Buffer());
                    }
                    if (ret) {
                        ret = dataSourcesDatabase.Write("QualifiedName", functionName);
                    }
                    if (ret) {
                        if (!dataSourcesFunctionIndexesCache.MoveAbsolute(dsName.Buffer())) {
                            ret = dataSourcesFunctionIndexesCache.CreateAbsolute(dsName.Buffer());
                        }
                    }
                    if (ret) {
                        ret = dataSourcesFunctionIndexesCache.Write(functionName, newFunctionId.Buffer());
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
                ConfigurationDatabase functionsDatabaseBeforeSignalMove = functionsDatabase;
                for (n = 0u; (n < numberOfSignalsInFunction) && (ret); n++) {
                    functionsDatabase = functionsDatabaseBeforeSignalMove;
                    ret = functionsDatabase.MoveToChild(n);
                    StreamString newSignalId;
                    ConfigurationDatabase dataSourcesDatabaseBeforeNewSignal = dataSourcesDatabase;
                    if (ret) {
                        ret = newSignalId.Printf("%d", (n + numberOfSignalsAlreadyInDataSource));
                        if (ret) {
                            ret = dataSourcesDatabase.CreateRelative(newSignalId.Buffer());
                        }
                    }
                    if (ret) {
                        ret = functionsDatabase.Copy(dataSourcesDatabase);
                    }
                    if (initialiseAfterInitialisation) {

                        if (ret) {
                            StreamString brokerClassName = dataSource->GetBrokerName(dataSourcesDatabase, direction);

                            ret = brokerClassName.Size() > 0u;
                            if (ret) {
                                ret = dataSourcesDatabase.Write("Broker", brokerClassName.Buffer());
                            }
                            else {
                                StreamString signalName;
                                if (!dataSourcesDatabase.Read("QualifiedName", signalName)) {
                                    signalName = "UnknownSignal";
                                }
                                REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Unsupported broker for signal %s linked to %s", signalName.Buffer(),
                                                    signalName.Buffer(), dataSource->GetName());
                            }
                        }
                    }
                    //Move to the next signal
                    if (ret) {
                        dataSourcesDatabase = dataSourcesDatabaseBeforeNewSignal;
                    }
                }
            }
            if (ret) {
                ret = dataSourcesDatabase.Write("ByteSize", byteSize);
            }
        }
    }
    if (ret) {
        functionsDatabase = functionsDatabaseBeforeMove;
    }

    return ret;
}

////////////////////////////////
////////////////////////////////
// AssignBrokersToFunctions
////////////////////////////////
////////////////////////////////
bool RealTimeApplicationConfigurationBuilder::AssignBrokersToFunctions() {

    bool ret = dataSourcesDatabase.MoveAbsolute("Data");
    uint32 numberOfDataSources = dataSourcesDatabase.GetNumberOfChildren();
    if (ret) {
        ret = (realTimeApplication != NULL);
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Invalid RealTimeApplication set");
        }
    }

    ConfigurationDatabase dataSourcesDatabaseBeforeMove = dataSourcesDatabase;
    for (uint32 i = 0u; (i < numberOfDataSources) && (ret); i++) {
        dataSourcesDatabase = dataSourcesDatabaseBeforeMove;
        ret = dataSourcesDatabase.MoveToChild(i);
        StreamString dsName;
        if (ret) {
            ret = dataSourcesDatabase.Read("QualifiedName", dsName);
        }
        ReferenceT<DataSourceI> dataSource;
        if (ret) {
            StreamString fullDsPath = "Data.";
            fullDsPath += dsName;
            /*lint -e{613} NULL pointer checking done before entering here */
            dataSource = realTimeApplication->Find(fullDsPath.Buffer());
            ret = dataSource.IsValid();
            if (!ret) {
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "%s is not a valid DataSourceI", fullDsPath.Buffer());
            }
        }

        //Allow for empty DataSources...
        if (dataSourcesDatabase.MoveRelative("Functions")) {

            if (ret) {
                uint32 numberOfFunctions = dataSourcesDatabase.GetNumberOfChildren();
                ConfigurationDatabase dataSourcesDatabaseBeforeFunctionMove = dataSourcesDatabase;
                for (uint32 j = 0u; (j < numberOfFunctions) && (ret); j++) {
                    dataSourcesDatabase = dataSourcesDatabaseBeforeFunctionMove;
                    ret = dataSourcesDatabase.MoveToChild(j);
                    if (ret) {
                        ret = AssignBrokersToSignals(InputSignals, dataSource);
                        if (!ret) {
                            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to AssignBrokersToSignals(InputSignals) to DataSource %s",
                                                dataSource->GetName());
                        }
                        if (ret) {
                            ret = AssignBrokersToSignals(OutputSignals, dataSource);
                            if (!ret) {
                                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to AssignBrokersToSignals(OutputSignals) to DataSource %s",
                                                    dataSource->GetName());
                            }
                        }
                    }
                }
            }
        }
    }

    return ret;
}

bool RealTimeApplicationConfigurationBuilder::AssignBrokersToSignals(const SignalDirection direction,
                                                                     ReferenceT<DataSourceI> dataSource) {
    const char8 *dirStr = "InputSignals";
    if (direction == OutputSignals) {
        dirStr = "OutputSignals";
    }

    bool ret = true;
    ConfigurationDatabase dataSourcesDatabaseBeforeSignalMoveRelative = dataSourcesDatabase;
    if (dataSourcesDatabase.MoveRelative(dirStr)) {
        uint32 numberOfSignals = dataSourcesDatabase.GetNumberOfChildren();
        uint32 i;
        ConfigurationDatabase dataSourcesDatabaseBeforeSignalMove = dataSourcesDatabase;
        for (i = 0u; (i < numberOfSignals) && (ret); i++) {
            StreamString signalId = dataSourcesDatabase.GetChildName(i);
            if (dataSourcesDatabase.MoveRelative(signalId.Buffer())) {
                // return the broker depending on the signal configuration
                StreamString brokerClassName = dataSource->GetBrokerName(dataSourcesDatabase, direction);
                ret = brokerClassName.Size() > 0u;
                if (ret) {
                    ret = dataSourcesDatabase.Write("Broker", brokerClassName.Buffer());
                }
                else {
                    StreamString signalName;
                    if (!dataSourcesDatabase.Read("QualifiedName", signalName)) {
                        signalName = "UnknownSignal";
                    }
                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Unsupported broker for signal %s linked to %s", signalName.Buffer(),
                                        signalName.Buffer(), dataSource->GetName());
                }

                if (ret) {
                    dataSourcesDatabase = dataSourcesDatabaseBeforeSignalMove;
                }
            }
        }
        if (ret) {
            dataSourcesDatabase = dataSourcesDatabaseBeforeSignalMoveRelative;
        }
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
        ConfigurationDatabase dataSourcesDatabaseBeforeMove = dataSourcesDatabase;
        for (n = 0u; (n < numberOfDataSources) && (ret); n++) {
            StreamString qualifiedName = "Data.";
            StreamString dataSourceIdx;
            const uint32 constN = n;
            ret = dataSourceIdx.Printf("%d", constN);
            if (ret) {
                ret = dataSourcesDatabase.MoveRelative(dataSourceIdx.Buffer());
            }
            if (ret) {
                ret = dataSourcesDatabase.Read("QualifiedName", qualifiedName);
            }
            ReferenceT<DataSourceI> dataSource;
            if (ret) {
                /*lint -e{613} NULL pointer checking done before entering here */
                dataSource = realTimeApplication->Find(qualifiedName.Buffer());
                ret = dataSource.IsValid();
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "The DataSource %s is not valid", qualifiedName.Buffer());
                }
            }
            if (ret) {
                ret = dataSource->SetConfiguredDatabase(dataSourcesDatabase);
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "SetConfiguredDatabase failed for DataSource %s", qualifiedName.Buffer());
                }
            }
            if (ret) {
                dataSourcesDatabase = dataSourcesDatabaseBeforeMove;
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
        ConfigurationDatabase functionsDatabaseBeforeMove = functionsDatabase;
        for (n = 0u; (n < numberOfFunctions) && (ret); n++) {
            StreamString qualifiedName = "Functions.";
            StreamString functionIdx;
            const uint32 constN = n;
            ret = functionIdx.Printf("%d", constN);
            if (ret) {
                ret = functionsDatabase.MoveRelative(functionIdx.Buffer());
            }
            if (ret) {
                ret = functionsDatabase.Read("QualifiedName", qualifiedName);
            }
            ReferenceT<GAM> gam;
            if (ret) {
                /*lint -e{613} NULL pointer checking done before entering here */
                gam = realTimeApplication->Find(qualifiedName.Buffer());
                ret = gam.IsValid();
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "The GAM %s is not valid", qualifiedName.Buffer());
                }
            }
            if (ret) {
                ret = gam->SetConfiguredDatabase(functionsDatabase);
            }
            if (ret) {
                functionsDatabase = functionsDatabaseBeforeMove;
            }
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::Copy(ConfigurationDatabase &functionsDatabaseOut,
                                                   ConfigurationDatabase &dataSourcesDatabaseOut) {
    bool ret = functionsDatabase.MoveToRoot();
    if (ret) {
        functionsDatabaseOut = functionsDatabase;
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }
    if (ret) {
        dataSourcesDatabaseOut = dataSourcesDatabase;
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::Set(ConfigurationDatabase &functionsDatabaseIn,
                                                  ConfigurationDatabase &dataSourcesDatabaseIn) {
    functionsDatabase.Purge();
    bool ret = functionsDatabaseIn.Copy(functionsDatabase);
    if (ret) {
        dataSourcesDatabase.Purge();
        ret = dataSourcesDatabaseIn.Copy(dataSourcesDatabase);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::FindFunctionNumber(StreamString functionName,
                                                                 StreamString &functionNumber) {
    return functionsIndexesCache.Read(functionName.Buffer(), functionNumber);
}

bool RealTimeApplicationConfigurationBuilder::CheckTypeCompatibility(StreamString &fullType,
                                                                     StreamString &otherFullType,
                                                                     StreamString &signalName,
                                                                     StreamString &dataSourceSignalName) const {
    //If they are equal just move on with it.
    bool ret = (fullType == otherFullType);
    if (!ret) {
        ret = signalName.Seek(0LLU);
        if (ret) {
            ret = dataSourceSignalName.Seek(0LLU);
        }
        if (ret) {
            ret = fullType.Seek(0LLU);
        }
        if (ret) {
            ret = otherFullType.Seek(0LLU);
        }

        char8 terminator;
        StreamString token1;
        StreamString token2;

        bool go = true;
        while (go && ret) {
            go = signalName.GetToken(token1, ".", terminator);
            if (go) {
                go = dataSourceSignalName.GetToken(token2, ".", terminator);
                if (go) {
                    go = (token1 == token2);
                    token1 = "";
                    token2 = "";
                    if (go) {
                        ret = fullType.GetToken(token1, ".", terminator);
                        if (ret) {
                            ret = otherFullType.GetToken(token2, ".", terminator);
                        }
                        bool token1Node = (token1 == "Node");
                        if (!token1Node) {
                            if (ret) {
                                bool equalTokens = (token1 == token2);
                                bool token2Node = (token2 == "Node");
                                ret = (equalTokens) || (token2Node);
                            }
                        }
                    }
                }
            }
            token1 = "";
            token2 = "";
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::CacheSignalIntrospections(const char8 *const typeName,
                                                                        const char8 *const signalName,
                                                                        const char8 *const fullTypeName,
                                                                        ConfigurationDatabase &data,
                                                                        uint32 &signalNumber) {

    //Try to find the registered type in the ClassRegistryDatabase
    const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(typeName);
    const Introspection *intro = NULL_PTR(Introspection*);

    bool ret = (item != NULL_PTR(ClassRegistryItem*));

    if (ret) {
        /*lint -e{613} NULL pointer checking done before entering here */
        intro = item->GetIntrospection();
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "The type %s is not registered", typeName);
    }

    ret = (intro != NULL);
    if (ret) {
        uint32 numberOfMembers = 0u;
        if (intro != NULL_PTR(Introspection*)) {
            numberOfMembers = intro->GetNumberOfMembers();
        }
        uint32 i;
        //For each of the structure members...
        for (i = 0u; (i < numberOfMembers) && (ret); i++) {
            StreamString typeNameStr = fullTypeName;
            if (typeNameStr.Size() > 0u) {
                typeNameStr += ".";
            }
            //lint -e{613} intro cannot be NULL as it is checked above.
            const IntrospectionEntry entry = intro->operator[](i);
            StreamString fullSignalName = signalName;
            if (fullSignalName.Size() > 0u) {
                fullSignalName += ".";
            }
            fullSignalName += entry.GetMemberName();
            typeNameStr += entry.GetMemberTypeName();

            //If the member is still structured data, continue to discombobulate
            bool isStructured = entry.GetMemberTypeDescriptor().isStructuredData;
            if (isStructured) {
                uint8 nOfDimensions = entry.GetNumberOfDimensions();
                //TODO to be abstracted for any number of dimensions
                ret = (nOfDimensions < 4u);
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "The number of dimensions shall be < 3");
                }
                StreamString tempName = fullSignalName;
                uint32 nr;
                uint32 nc;
                uint32 nz;
                uint32 nOfRows = entry.GetNumberOfElements(0u);
                uint32 nOfCols = entry.GetNumberOfElements(1u);
                uint32 nOfZ = entry.GetNumberOfElements(2u);
                if (nOfRows == 0u) {
                    nOfRows = 1u;
                }
                if (nOfCols == 0u) {
                    nOfCols = 1u;
                }
                if (nOfZ == 0u) {
                    nOfZ = 1u;
                }

                /*lint -e{850} nr, nc and nz are not modified by the Print*/
                for (nr = 0u; (nr < nOfRows) && (ret); nr++) {
                    for (nc = 0u; (nc < nOfCols) && (ret); nc++) {
                        for (nz = 0u; (nz < nOfZ) && (ret); nz++) {
                            fullSignalName = "";
                            if (nOfDimensions <= 1u) {
                                if (nOfRows > 1u) {
                                    (void) fullSignalName.Printf("%s[%d]", tempName.Buffer(), nr);
                                }
                                else {
                                    fullSignalName = tempName;
                                }
                            }
                            else if (nOfDimensions == 2u) {
                                (void) fullSignalName.Printf("%s[%d][%d]", tempName.Buffer(), nr, nc);
                            }
                            else if (nOfDimensions == 3u) {
                                (void) fullSignalName.Printf("%s[%d][%d][%d]", tempName.Buffer(), nr, nc, nz);
                            }
                            else {    //NOOP
                            }
                            ret = CacheSignalIntrospections(entry.GetMemberTypeName(), fullSignalName.Buffer(), typeNameStr.Buffer(), data, signalNumber);
                        }
                    }
                }
            }
            else {
                //Got to the final BasicType. Add a node with the signal number as the name...
                StreamString signalNumberStr;
                ret = signalNumberStr.Printf("%d", signalNumber);
                if (ret) {
                    ret = data.CreateRelative(signalNumberStr.Buffer());
                }
                uint32 byteSize = 0u;
                if ((i + 1u) < numberOfMembers) {
                    if (intro != NULL_PTR(const Introspection*)) {
                        const IntrospectionEntry nextEntry = intro->operator[](i + 1u);
                        byteSize = nextEntry.GetMemberByteOffset() - entry.GetMemberByteOffset();
                    }
                }
                else {
                    if (intro != NULL_PTR(const Introspection*)) {
                        byteSize = intro->GetClassSize() - entry.GetMemberByteOffset();
                    }
                }
                //Finally got to the BasicType. Write all the properties
                if (ret) {
                    ret = data.Write("QualifiedName", fullSignalName.Buffer());
                }
                if (ret) {
                    ret = data.Write("Type", entry.GetMemberTypeName());
                }
                uint32 numberOfDimensions = 0u;
                if (ret) {
                    numberOfDimensions = entry.GetNumberOfDimensions();
                    ret = data.Write("NumberOfDimensions", numberOfDimensions);
                }
                uint32 totalElements = 1u;
                if (ret) {
                    uint8 j;
                    for (j = 0u; j <= entry.GetNumberOfDimensions(); j++) {
                        totalElements *= entry.GetNumberOfElements(j);
                    }

                    ret = data.Write("NumberOfElements", totalElements);
                }
                if (ret) {
                    ret = data.Write("FullType", typeNameStr.Buffer());
                }
                if (ret) {
                    ret = data.Write("MemberSize", byteSize);
                }
                if (ret) {
                    ret = data.MoveToAncestor(1u);
                }
                signalNumber++;
            }
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::CacheAllSignalsIntrospections() {
    bool ret = true;
    ClassRegistryDatabase *crd = ClassRegistryDatabase::Instance();
    if (crd != NULL_PTR(ClassRegistryDatabase*)) {
        uint32 nItems = crd->GetSize();
        for (uint32 n = 0u; (n < nItems) && (ret); n++) {
            const ClassRegistryItem *item = crd->Peek(n);
            if (item != NULL_PTR(const ClassRegistryItem*)) {
                const Introspection *intro = item->GetIntrospection();
                if (intro != NULL_PTR(const Introspection*)) {
                    StreamString typeName = item->GetClassProperties()->GetName();
                    uint32 signalNumber = 0u;
                    ret = cachedIntrospections.CreateAbsolute(typeName.Buffer());
                    /*lint -e{864} typeName is not modified by the REPORT_ERROR*/
                    if (ret) {
                        REPORT_ERROR(ErrorManagement::Information, "Caching signal type [%s]", typeName.Buffer());
                        ret = CacheSignalIntrospections(typeName.Buffer(), "", typeName.Buffer(), cachedIntrospections, signalNumber);
                    }
                }
            }
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::SignalIntrospectionToStructuredData(ConfigurationDatabase &signalDatabase,
                                                                                  const char8 *const typeName,
                                                                                  const char8 *const signalName,
                                                                                  const char8 *const alias,
                                                                                  const char8 *const dataSourceName,
                                                                                  const char8 *const syncSignalName,
                                                                                  const char8 *const triggerSignalName,
                                                                                  const char8 *const fullTypeName,
                                                                                  const AnyType &ranges,
                                                                                  const AnyType &samples,
                                                                                  const AnyType &frequency,
                                                                                  const AnyType &trigger,
                                                                                  ConfigurationDatabase &data,
                                                                                  uint32 &signalNumber,
                                                                                  bool &syncSet,
                                                                                  bool &triggerSet,
                                                                                  const bool isFunctionDatabase) {
    //Try to find the registered type in the ClassRegistryDatabase
    bool ret = cachedIntrospections.MoveAbsolute(typeName);
    uint32 numberOfSignals = cachedIntrospections.GetNumberOfChildren();
    uint32 n;
    REPORT_ERROR(ErrorManagement::Debug, "Going to introspect %d signals", numberOfSignals);
    for (n = 0u; (n < numberOfSignals) && (ret); n++) {
        ret = cachedIntrospections.MoveToChild(n);
        if (ret) {
            //Got to the final BasicType. Add a node with the signal number as the name...
            StreamString signalNumberStr;
            ret = signalNumberStr.Printf("Signals.%d", signalNumber);
            if (ret) {
                ret = data.CreateAbsolute(signalNumberStr.Buffer());
            }
        }
        StreamString cachedQualifiedName;
        StreamString cachedAliasName;
        StreamString cachedFullType;
        StreamString cachedType;
        StreamString fullSignalName;
        StreamString fullAliasName;
        if (ret) {
            ret = cachedIntrospections.Read("QualifiedName", cachedQualifiedName);
        }
        if (ret) {
            ret = cachedQualifiedName.Seek(0LLU);
        }
        char8 terminator;
        StreamString token;
        if (ret) {
            while (cachedQualifiedName.GetToken(token, ".", terminator)) {
                if (cachedAliasName.Size() > 0U) {
                    cachedAliasName += ".";
                }
                cachedAliasName += token.Buffer();
                token = "";
            }
        }
        if (ret) {
            ret = cachedIntrospections.Read("FullType", cachedFullType);
        }
        if (ret) {
            ret = cachedIntrospections.Read("Type", cachedType);
        }
        //Finally got to the BasicType. Write all the properties
        if (ret) {
            ret = fullSignalName.Printf("%s.%s", signalName, cachedQualifiedName.Buffer());
        }
        if (StringHelper::Length(dataSourceName) > 0u) {
            StreamString dataSourceNumber;
            ret = dataSourcesIndexesCache.Read(dataSourceName, dataSourceNumber);
            if (ret) {
                TypeDescriptor signalTypeDesc = dataSourcesSignalIndexCache.GetType(fullSignalName.Buffer()).GetTypeDescriptor();
                if (signalTypeDesc != voidAnyType.GetTypeDescriptor()) {
                    ret = dataSourcesSignalIndexCache.Write(fullSignalName.Buffer(), signalNumber);
                }
            }
        }
        if (ret) {
            ret = cachedIntrospections.Copy(data);
        }
        if (ret) {
            ret = cachedIntrospections.MoveToAncestor(1u);
        }
        //Must update the QualifiedName after the cachedIntrospections.Copy
        if (ret) {
            ret = data.Write("QualifiedName", fullSignalName.Buffer());
            fullAliasName = alias;
            if (fullAliasName.Size() > 0u) {
                if (cachedAliasName.Size() > 0u) {
                    fullAliasName += ".";
                    fullAliasName += cachedAliasName.Buffer();
                }
            }
        }
        ConfigurationDatabase signalDatabaseBeforeMove = signalDatabase;
        if (ret) {
            // loop in the default values and add it to the signal if found
            if (signalDatabase.MoveRelative("Defaults")) {
                AnyType defaultValueAnyType = signalDatabase.GetType(fullSignalName.Buffer());
                if (defaultValueAnyType.GetTypeDescriptor() != voidAnyType.GetTypeDescriptor()) {
                    ret = data.Write("Default", defaultValueAnyType);
                    REPORT_ERROR(ErrorManagement::Debug, "Writing Default value %! (from Defaults) for signal %s", defaultValueAnyType,
                                 fullSignalName.Buffer());
                }
            }
        }
        if (ret) {
            signalDatabase = signalDatabaseBeforeMove;
        }
        bool patchAlias = false;
        StreamString typeNameStr;
        StreamString memberAlias;
        StreamString memberAliasKey;
        if (signalDatabase.MoveRelative("MemberAliases")) {
            uint32 nn;
            for (nn = 0u; (nn < signalDatabase.GetNumberOfChildren()) && (!patchAlias); nn++) {
                memberAliasKey = signalDatabase.GetChildName(nn);
                //Check if the declared member alias is part of the full signal name. If so patch
                patchAlias = (fullSignalName.Locate(memberAliasKey.Buffer()) == 0);
            }
        }
        if (patchAlias) {
            //All this code is to support the alias of a substructure member. In the previous version this was being discovered every time a signal was added.
            //That had the big drawback that for large nested structures the same code had to be executed for every alias combination. This strategy allows
            //to recycle the cached structured types, at the expense of increased code complexity
            if (ret) {
                ret = signalDatabase.Read(memberAliasKey.Buffer(), memberAlias);
            }
            if (ret) {
                //The alias will be what is set in the MemberAliases in the configuration file
                fullAliasName = memberAlias.Buffer();
                (void) cachedFullType.Seek(0LLU);
                (void) cachedQualifiedName.Seek(0LLU);
                bool patch = false;
                //Reset the type so that is can be matched against the correct aliased signal
                typeNameStr = "";
                ret = memberAlias.Seek(0LLU);
                //If the alias is a path (e.g. A=B.C), add the keyword Node so that it can be later matched
                while (memberAlias.SkipTokens(1u, ".")) {
                    if (typeNameStr.Size() > 0LLU) {
                        typeNameStr += ".";
                    }
                    typeNameStr += "Node";
                }
                token = "";
                StreamString tokenA;
                //Get the full signal name and use only the unmatched part w.r.t. to the MemberAlias. E.g. If the full signal name was ADCs.ADC1.Signal3.c1.b1
                //and the name of the to be aliased (i.e. the memberAliasKey) was ADCs.ADC1.Signal3.c1, then the unmatched part would be b1.
                //As a consequence, the string b1 will have to be appended to the MemberAlias, so that if we had MemberAlias = ADC_Channel1, the alias name would be ADC_Channel1.b1,
                //since ADC_Channel1 must also be a struct (with a member b1). The same logic applies to the type below
                //1. Find out what part of the memberAliasKey does not match the fullSignalName
                (void) fullSignalName.Seek(0LLU);
                (void) memberAliasKey.Seek(0LLU);
                uint32 nOfTypeTokensToKeep = 0u;
                while (fullSignalName.GetToken(token, ".", terminator)) {
                    (void) (memberAliasKey.GetToken(tokenA, ".", terminator));
                    if (!patch) {
                        patch = (token != tokenA);
                    }
                    if (patch) {
                        nOfTypeTokensToKeep++;
                        if (token.Size() > 0U) {
                            fullAliasName += ".";
                            fullAliasName += token.Buffer();
                        }
                    }
                    token = "";
                    tokenA = "";
                }
                //Reconstruct the missing signal type by checking how many unmatched nodes were detected before. In the example above there was only one unmatched node named b1
                uint32 fullTypeNOfTokens = 0u;
                StreamString tokenT;
                (void) cachedFullType.Seek(0LLU);
                //Compute how many tokens are in the full type
                while (cachedFullType.GetToken(tokenT, ".", terminator)) {
                    fullTypeNOfTokens++;
                    tokenT = "";
                }
                //Only keep the unmatched last N tokens
                uint32 nOfTypeTokensToSkip = 0u;
                if (fullTypeNOfTokens > nOfTypeTokensToKeep) {
                    nOfTypeTokensToSkip = (fullTypeNOfTokens - nOfTypeTokensToKeep);
                }
                if (nOfTypeTokensToSkip > 0u) {
                    nOfTypeTokensToSkip--;
                }
                (void) cachedFullType.Seek(0LLU);
                for (uint32 m = 0u; m < nOfTypeTokensToSkip; m++) {
                    (void) cachedFullType.GetToken(tokenT, ".", terminator);
                    tokenT = "";
                }
                //Reoconstruct the signal type
                while (cachedFullType.GetToken(tokenT, ".", terminator)) {
                    if (typeNameStr.Size() > 0u) {
                        typeNameStr += ".";
                    }
                    typeNameStr += tokenT.Buffer();
                    tokenT = "";
                }
            }
        }
        else {
            typeNameStr = fullTypeName;
            StreamString subType;
            bool tokenize = true;
            (void) cachedFullType.Seek(0LLU);
            while (cachedFullType.GetToken(subType, ".", terminator)) {
                //Only add to the type name the unmatched part of the cached full signal type (e.g. A.B is inside Node.Node.A.B.C.D)
                if (tokenize) {
                    //Make sure that there is a complete match of the subtype, e.g. looking for BB inside A.CBB.D.BB should not held a match in CBB!
                    //First case subType equals the type, i.e. BB = BB
                    tokenize = (subType == cachedFullType);
                    if (!tokenize) {
                        //Second case the type starts with the subType BB.A.BC
                        StreamString subTypeDot = subType;
                        subTypeDot += ".";
                        uint32 len = static_cast<uint32>(subTypeDot.Size());
                        tokenize = (StringHelper::CompareN(cachedFullType.Buffer(), subTypeDot.Buffer(), len) == 0);
                    }
                    if (!tokenize) {
                        //Third case the type contains the subType A.BB.C.BC or ends with the subType A.C.BB
                        StreamString subTypeDot = ".";
                        subTypeDot += subType;
                        const char8 *foundStr = (StringHelper::SearchString(fullTypeName, subTypeDot.Buffer()));
                        tokenize = (foundStr != NULL_PTR(char8*));
                        if (tokenize) {
                            //Check if ends with . or \0
                            tokenize = (foundStr[subTypeDot.Size()] == '\0');
                            if (!tokenize) {
                                tokenize = (foundStr[subTypeDot.Size()] == '.');
                            }
                        }
                    }
                }
                if (!tokenize) {
                    typeNameStr += ".";
                    typeNameStr += subType.Buffer();
                }
                subType = "";
            }
        }
        if (ret) {
            ret = data.Write("FullType", typeNameStr.Buffer());
        }
        if (ret) {
            signalDatabase = signalDatabaseBeforeMove;
        }
        ConfigurationDatabase dataBeforeMove = data;
        if (ret) {
            if (isFunctionDatabase) {
                if (StringHelper::Length(dataSourceName) > 0u) {
                    ret = data.Write("DataSource", dataSourceName);
                }
                if (ret) {
                    if (fullAliasName.Size() > 0u) {
                        ret = data.Write("Alias", fullAliasName.Buffer());
                    }
                }
                if (ret) {
                    if (ranges.GetTypeDescriptor() != VoidType) {
                        ret = data.Write("Ranges", ranges);
                    }

                }
                if (ret) {
                    if (samples.GetTypeDescriptor() != VoidType) {
                        ret = data.Write("Samples", samples);
                    }
                }
                if (ret) {
                    if (frequency.GetTypeDescriptor() != VoidType) {
                        if (StringHelper::Compare(fullSignalName.Buffer(), syncSignalName) == 0) {
                            ret = data.Write("Frequency", frequency);
                            syncSet = true;
                        }
                    }
                }
                if (ret) {
                    if (trigger.GetTypeDescriptor() != VoidType) {
                        if (StringHelper::Compare(fullSignalName.Buffer(), triggerSignalName) == 0) {
                            ret = data.Write("Trigger", trigger);
                            triggerSet = true;
                        }
                    }
                }
            }
        }
        if (ret) {
            data = dataBeforeMove;
        }
        signalNumber++;
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::SearchGAMs(ConfigurationDatabase &inputDatabase,
                                                         ConfigurationDatabase &outputDatabase,
                                                         StreamString &fullPath,
                                                         uint32 &index,
                                                         bool found) {

    bool ret = true;
    StreamString className;
    bool isGam = inputDatabase.Read("Class", className);
    if (isGam) {
        isGam = className.Size() > 0u;
    }
    if (isGam) {
        isGam = (className != "ReferenceContainer");
    }
    if (isGam) {
        StreamString notGAM;
        if (inputDatabase.Read("NotGAM", notGAM)) {
            isGam = (notGAM != "true");
        }
    }
    if (isGam) {
        isGam = inputDatabase.MoveRelative("InputSignals");
        if (!isGam) {
            isGam = inputDatabase.MoveRelative("OutputSignals");
            if (!isGam) {
                ret = false;
                REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Specified GAM %s with no input nor output", fullPath.Buffer());
            }
        }
    }
    if (isGam) {
        isGam = inputDatabase.MoveToAncestor(1u);
    }
    if (isGam) {

        if (found) {
            ret = false;
            REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Unsupported nested GAMs in path %s", fullPath.Buffer());
        }
        else {
            found = true;
        }

        if (fullPath.Size() > 0u) {
            StreamString functionN;
            ret = functionN.Printf("%d", index);
            if (ret) {
                ret = outputDatabase.CreateRelative(functionN.Buffer());
            }
            if (ret) {
                ret = outputDatabase.Write("QualifiedName", fullPath.Buffer());
            }
            if (ret) {
                ret = functionsIndexesCache.Write(fullPath.Buffer(), functionN.Buffer());
            }
            if (ret) {
                ret = outputDatabase.CreateRelative("Signals");
                if (ret) {
                    if (inputDatabase.MoveRelative("InputSignals")) {
                        ret = outputDatabase.CreateRelative("InputSignals");
                        if (ret) {
                            ret = inputDatabase.Copy(outputDatabase);
                        }
                        if (ret) {
                            ret = inputDatabase.MoveToAncestor(1u);
                        }
                        if (ret) {
                            ret = outputDatabase.MoveToAncestor(1u);
                        }
                    }
                    if (inputDatabase.MoveRelative("OutputSignals")) {
                        ret = outputDatabase.CreateRelative("OutputSignals");
                        if (ret) {
                            ret = inputDatabase.Copy(outputDatabase);
                        }
                        if (ret) {
                            ret = inputDatabase.MoveToAncestor(1u);
                        }
                        if (ret) {
                            ret = outputDatabase.MoveToAncestor(1u);
                        }
                    }
                    if (ret) {
                        ret = outputDatabase.MoveToAncestor(1u);
                    }
                }
            }
            if (ret) {
                ret = outputDatabase.MoveToAncestor(1u);
            }
            index++;
        }

    }
    //don't go deep
    uint32 numberOfFunctions = inputDatabase.GetNumberOfChildren();

    for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
        const char8 *nodeName = inputDatabase.GetChildName(i);

        if (inputDatabase.MoveRelative(nodeName)) {
            if ((nodeName[0] == '+') || (nodeName[0] == '$')) {
                nodeName = &nodeName[1];
            }
            StreamString currentStep = fullPath;
            if (currentStep.Size() > 0u) {
                currentStep += ".";
            }
            currentStep += nodeName;

            // don't search gams into gams

            ret = SearchGAMs(inputDatabase, outputDatabase, currentStep, index, found);

            if (ret) {
                ret = inputDatabase.MoveToAncestor(1u);
            }
        }

    }

    return ret;
}

uint32 RealTimeApplicationConfigurationBuilder::GetNumberOfSyncSignals(const char8 *const direction,
                                                                       bool &ok) {
    uint32 syncCounter = 0u;
    ConfigurationDatabase functionsDatabaseOriginal = functionsDatabase;
    if (functionsDatabase.MoveRelative(direction)) {
        uint32 numberOfOutSignal = functionsDatabase.GetNumberOfChildren();
        ConfigurationDatabase functionsDatabaseBeforeMove = functionsDatabase;
        for (uint32 i = 0u; (i < numberOfOutSignal) && (ok); i++) {
            functionsDatabase = functionsDatabaseBeforeMove;
            ok = functionsDatabase.MoveToChild(i);
            if (ok) {
                float32 frequency;
                if (functionsDatabase.Read("Frequency", frequency)) {
                    if (frequency >= 0.0) {
                        syncCounter++;
                    }
                }
            }
        }
        if (ok) {
            functionsDatabase = functionsDatabaseOriginal;
        }
    }
    return syncCounter;
}

bool RealTimeApplicationConfigurationBuilder::SearchDataSources(ConfigurationDatabase &inputDatabase,
                                                                ConfigurationDatabase &outputDatabase,
                                                                StreamString &fullPath,
                                                                uint32 &index,
                                                                uint32 &timingDataSourceCounter) {
    bool ret = true;

    StreamString className;
    bool isDataSource = inputDatabase.Read("Class", className);
    if (isDataSource) {
        isDataSource = (fullPath.Size() > 0u);
    }
    if (isDataSource) {
        isDataSource = (className.Size() > 0u);
    }
    if (isDataSource) {
        isDataSource = (className != "ReferenceContainer");
    }
    if (isDataSource) {
        StreamString notDataSource;
        if (inputDatabase.Read("NotDataSource", notDataSource)) {
            isDataSource = (notDataSource != "true");
        }
    }
    if (isDataSource) {
        StreamString functionN;
        ret = functionN.Printf("%d", index);
        if (ret) {
            ret = outputDatabase.CreateRelative(functionN.Buffer());
        }
        if (ret) {
            if (className == "TimingDataSource") {
                uint32 isTimingDataSource;
                if (!outputDatabase.Read("IsTimingDataSource", isTimingDataSource)) {
                    isTimingDataSource = 1u;
                    ret = outputDatabase.Write("IsTimingDataSource", isTimingDataSource);
                }
                timingDataSourceCounter++;
            }
        }
        if (ret) {
            ret = outputDatabase.Write("QualifiedName", fullPath.Buffer());
        }
        if (ret) {
            ret = dataSourcesIndexesCache.Write(fullPath.Buffer(), functionN.Buffer());
        }
        if (ret) {
            ret = dataSourcesSignalIndexCache.CreateAbsolute(functionN.Buffer());
        }
        if (ret) {
            ret = outputDatabase.CreateRelative("Signals");
            if (ret) {
                if (inputDatabase.MoveRelative("Signals")) {
                    ret = inputDatabase.Copy(outputDatabase);
                    if (ret) {
                        ret = inputDatabase.Copy(dataSourcesSignalIndexCache);
                    }
                    if (ret) {
                        ret = inputDatabase.MoveToAncestor(1u);
                    }
                }
                if (ret) {
                    ret = outputDatabase.MoveToAncestor(1u);
                }
            }

        }
        if (ret) {
            ret = outputDatabase.MoveToAncestor(1u);
        }
        index++;
    }
    // go deep

    uint32 numberOfFunctions = inputDatabase.GetNumberOfChildren();

    for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {

        const char8 *nodeName = inputDatabase.GetChildName(i);
        if (inputDatabase.MoveRelative(nodeName)) {
            if ((nodeName[0] == '+') || (nodeName[0] == '$')) {
                nodeName = &nodeName[1];
            }

            StreamString currentStep = fullPath;
            if (currentStep.Size() > 0u) {
                currentStep += ".";
            }
            currentStep += nodeName;

            // don't search ds into ds

            ret = SearchDataSources(inputDatabase, outputDatabase, currentStep, index, timingDataSourceCounter);

            if (ret) {
                ret = inputDatabase.MoveToAncestor(1u);
            }
        }

    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::ConfigureThreads() const {

    ReferenceContainer statesContainer;
    bool ret = (realTimeApplication != NULL);
    if (ret) {
        /*lint -e{613} realTimeApplication != NULL is checked in the line above*/
        ret = realTimeApplication->GetStates(statesContainer);
    }

    if (ret) {
        // States contains RealTimeState references
        // for each of them call Validate(*)
        uint32 numberOfStates = statesContainer.Size();
        for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
            ReferenceT<RealTimeState> state = statesContainer.Get(i);
            if (state.IsValid()) {
                ReferenceT<ReferenceContainer> threadsContainer = state->Find("Threads");

                // for each state call the configuration function
                uint32 numberOfThreads = threadsContainer->Size();
                for (uint32 j = 0u; (j < numberOfThreads) && (ret); j++) {
                    ReferenceT<RealTimeThread> thread = threadsContainer->Get(j);
                    if (thread.IsValid()) {
                        ret = thread->ConfigureArchitecture();
                    }
                }
            }
        }
    }

    return ret;
}

void RealTimeApplicationConfigurationBuilder::CleanCaches() {
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Purging dataSourcesIndexesCache. Number of children:%d", dataSourcesSignalIndexCache.GetNumberOfChildren());
    dataSourcesIndexesCache.Purge();
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Purging functionsIndexesCache. Number of children:%d", functionsIndexesCache.GetNumberOfChildren());
    functionsIndexesCache.Purge();
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Purging dataSourcesSignalIndexCache. Number of children:%d",
                        dataSourcesSignalIndexCache.GetNumberOfChildren());
    dataSourcesSignalIndexCache.Purge();
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Purging dataSourcesFunctionIndexesCache. Number of children:%d",
                        dataSourcesFunctionIndexesCache.GetNumberOfChildren());
    dataSourcesFunctionIndexesCache.Purge();
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Purging functionsMemoryIndexesCache. Number of children:%d",
                        functionsMemoryIndexesCache.GetNumberOfChildren());
    functionsMemoryIndexesCache.Purge();
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Purged functionsMemoryIndexesCache. Number of children:%d", functionsMemoryIndexesCache.GetNumberOfChildren());
    cachedIntrospections.Purge();
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Purged cachedIntrospections. Number of children:%d", cachedIntrospections.GetNumberOfChildren());
}

bool RealTimeApplicationConfigurationBuilder::WriteDefault(StructuredDataI &sdi,
                                                           const char8 *const signalName,
                                                           const char8 *const dataSourceName,
                                                           const TypeDescriptor &signalTypeDescriptor,
                                                           const uint32 numberOfDimensions,
                                                           const uint32 numberOfElements,
                                                           const uint32 signalNumberOfBytes) const {
    bool ret = true;
    StreamString defaultVal;
    if (sdi.Read("Default", defaultVal)) {
        //Check if is a string stored in the StructuredDataI
        bool isString = (signalTypeDescriptor == Character8Bit);
        if (!isString) {
            isString = (signalTypeDescriptor == CharString);
        }
        //Given that the signal may be stored as a string in the cdb, it will have to be reparsed, otherwise
        //array signals like {1, 2, 3, 4} will be interpreted as a string (instead of an array of uint32)
        StreamString defValConfig = "Default=";
        if (isString) {
            //Force quotes around the string, otherwise we may get problems with spacs
            defValConfig += "\"";
        }
        defValConfig += defaultVal;
        if (isString) {
            defValConfig += "\"";
        }
        ret = defValConfig.Seek(0LLU);
        ConfigurationDatabase cdb;
        StandardParser parser(defValConfig, cdb);
        if (ret) {
            ret = parser.Parse();
        }

        //Get the type as it was directly stored
        AnyType defaultValueAnyType;
        if (ret) {
            defaultValueAnyType = cdb.GetType("Default");
        }
        uint32 defValDims = defaultValueAnyType.GetNumberOfDimensions();
        if (isString) {
            //Yet another exception for char8[]
            if (numberOfElements > 1u) {
                defValDims = 1u;
            }
        }

        //Check if the dimensions between the signal declared dimensions and the default value are compatible (e.g. do not accept to store an array default value in a scalar type)
        uint8 usedDimensions = (numberOfDimensions > 0u) ? (1u) : (0u);
        if (ret) {
            //Now check that the number of elements is compatible between the signal definitions and its default value
            uint32 elements0 = defaultValueAnyType.GetNumberOfElements(0u);
            //Another exception for the char8[]
            if (isString && (elements0 <= 1u)) {
                //Check that the string fits inside the char8[] size
                StreamString ss = reinterpret_cast<const char8*>(defaultValueAnyType.GetDataPointer());
                ret = (ss.Size() <= numberOfElements);
            }
            else {
                ret = (defValDims == usedDimensions);
                if (ret) {
                    ret = (elements0 == numberOfElements);
                }
                else if (numberOfElements == 1u) {
                    //To avoid the problem of 0 dimensions with 1 element vs 1 dimension with 1 element
                    ret = (elements0 == 1u);
                }
                else {
                    //NOOP
                }
            }
            if (!ret) {
                const char8 *const typeNameStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(signalTypeDescriptor);
                REPORT_ERROR_STATIC(ErrorManagement::InitialisationError,
                                    "Default value of signal %s of type %s in %s must be defined as a vector for multi-dimensional variables", signalName,
                                    typeNameStr, dataSourceName);
            }
        }
        //If everything is OK, construct a temporary AnyType with the correct characteristics and read the Default value to be stored in the target database (sdi)
        if (ret) {
            void *ptr = HeapManager::Malloc(signalNumberOfBytes);
            ret = (ptr != NULL_PTR(void*));
            if (ret) {
                AnyType at = AnyType(signalTypeDescriptor, 0u, ptr);
                at.SetNumberOfDimensions(static_cast<uint8>(defValDims));
                at.SetNumberOfElements(0u, numberOfElements);

                ret = cdb.Read("Default", at);
                if (ret) {
                    ret = sdi.Delete("Default");
                }
                if (ret) {
                    // write the default value as a value and not a string anymore!
                    ret = sdi.Write("Default", at);
                }
                (void) HeapManager::Free(reinterpret_cast<void*&>(ptr));
            }
        }
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Unsupported defined default value of signal %s in %s", signalName, dataSourceName);
        }
    }
    return ret;
}

/**Ignore doxygen false positive*/
CLASS_REGISTER(RealTimeApplicationConfigurationBuilder, "1.0");

}

