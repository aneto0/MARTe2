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
#include "Introspection.h"
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

static bool SignalIntrospectionToStructuredData(ConfigurationDatabase &signalDatabase,
                                                const char8 * const typeName,
                                                const char8 * const signalName,
                                                const char8 * const alias,
                                                const char8 * const dataSourceName,
                                                const char8 * const syncSignalName,
                                                AnyType ranges,
                                                AnyType samples,
                                                AnyType frequency,
                                                StructuredDataI & data,
                                                uint32 &signalNumber,
                                                bool &syncSet) {
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
                                                          dataSourceName, syncSignalName, ranges, samples, frequency, data, signalNumber, syncSet);
            }
            else {
                if (ret) {
                    //Got to the final BasicType. Add a node with the signal number as the name...
                    StreamString signalNumberStr;
                    signalNumberStr.Printf("Signals.%d", signalNumber);
                    ret = data.CreateAbsolute(signalNumberStr.Buffer());
                }
                // loop in the default values and add it to the signal if found
                if (signalDatabase.MoveRelative("Defaults")) {
                    StreamString defaultValue;
                    if (signalDatabase.Read(fullSignalName.Buffer(), defaultValue)) {
                        ret = data.Write("Default", defaultValue.Buffer());
                    }
                    signalDatabase.MoveToAncestor(1u);
                }
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
                if (samples.GetTypeDescriptor() != VoidType) {
                    ret = data.Write("Samples", samples);
                }
                if (samples.GetTypeDescriptor() != VoidType) {
                    if (StringHelper::Compare(fullSignalName.Buffer(), syncSignalName) == 0) {
                        ret = data.Write("Frequency", frequency);
                        syncSet = true;
                    }
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

static uint32 GetNumberOfSyncSignals(ConfigurationDatabase &functionsDatabase,
                                     const char8 * const direction,
                                     bool &ok) {
    uint32 syncCounter = 0u;
    if (functionsDatabase.MoveRelative(direction)) {
        uint32 numberOfOutSignal = functionsDatabase.GetNumberOfChildren();
        for (uint32 i = 0u; (i < numberOfOutSignal) && (ok); i++) {
            StreamString signalId = functionsDatabase.GetChildName(i);
            ok = functionsDatabase.MoveRelative(signalId.Buffer());
            if (ok) {
                float32 frequency;
                if (functionsDatabase.Read("Frequency", frequency)) {
                    if (frequency >= 0.0) {
                        syncCounter++;
                    }
                }
                ok = functionsDatabase.MoveToAncestor(1u);
            }
        }
        functionsDatabase.MoveToAncestor(2u);
    }
    return syncCounter;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeApplicationConfigurationBuilder::RealTimeApplicationConfigurationBuilder(ReferenceT<ReferenceContainer> realTimeApplicationIn,
                                                                                 const char8 *defaultDataSourceNameIn) {
    defaultDataSourceName = defaultDataSourceNameIn;
    realTimeApplication = realTimeApplicationIn;
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


#if 0

bool RealTimeApplicationConfigurationBuilder::InitialiseFunctionDatabaseFromConfiguration(ConfigurationDatabase &originalConfig,
                                                                                          originalConfig,
                                                                                          StreamString & fullPath,
                                                                                          uint32 &index) {
    uint32 numberOfFunctions = originalConfig.GetNumberOfChildren();
    bool ret = true;
    bool isGam = false;
    StreamString currentStep = fullPath;

    for (uint32 i = 0u; i < numberOfFunctions && ret; i++) {

        const char8 *nodeName = originalConfig.GetChildName(i);
        if (nodeName[0] == '+' || nodeName == '$') {
            nodeName = &nodeName[0];
        }
        if (originalConfig.MoveRelative(nodeName)) {
            StreamString className;
            isGam = originalConfig.Read("Class", className);
            if (isGam) {
                isGam = className.Size() > 0u;
            }
            if (isGam) {
                //TODO Use typeid to check polimorphism with GAM

            }
            if (isGam) {
                StreamString functionN;
                functionN.Printf("%d", index);
                ret = functionsDatabase.CreateRelative(functionN.Buffer());
                if (ret) {
                    functionsDatabase.Write("QualifiedName", currentStep.Buffer());
                }
                if (ret) {
                    ret = originalConfig.MoveRelative("Signals");

                }
                if (ret) {
                    ret = originalConfig.Copy(functionsDatabase);
                }
                if (ret) {
                    ret = originalConfig.MoveToAncestor(1u);
                }
                if (ret) {
                    ret = functionsDatabase.MoveToAncestor(1u);
                }
                index++;
            }
            currentStep += nodeName;
            ret = InitialiseFunctionDatabaseFromConfiguration(originalConfig, currentStep);

            ret = originalConfig.MoveToAncestor(1u);
        }

    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::InitialiseDataDatabaseFromConfiguration(ConfigurationDatabase &originalConfig,
                                                                                      originalConfig,
                                                                                      StreamString & fullPath,
                                                                                      uint32 &index) {
    uint32 numberOfDataSources = originalConfig.GetNumberOfChildren();
    bool ret = true;
    bool isGam = false;
    StreamString currentStep = fullPath;

    for (uint32 i = 0u; i < numberOfDataSources && ret; i++) {

        const char8 *nodeName = originalConfig.GetChildName(i);
        if (nodeName[0] == '+' || nodeName == '$') {
            nodeName = &nodeName[0];
        }
        if (originalConfig.MoveRelative(nodeName)) {
            StreamString className;
            isGam = originalConfig.Read("Class", className);
            if (isGam) {
                isGam = className.Size() > 0u;
            }
            if (isGam) {
                //TODO Use typeid to check polimorphism with DataSourceI

            }
            if (isGam) {
                StreamString functionN;
                functionN.Printf("%d", index);
                ret = dataSourcesDatabase.CreateRelative(functionN.Buffer());
                if (ret) {
                    dataSourcesDatabase.Write("dataSourcesDatabase", currentStep.Buffer());
                }
                if (ret) {
                    ret = originalConfig.MoveRelative("Signals");
                }
                if (ret) {
                    ret = originalConfig.Copy(dataSourcesDatabase);
                }
                if (ret) {
                    ret = originalConfig.MoveToAncestor(1u);
                }
                if (ret) {

                    ret = dataSourcesDatabase.MoveToAncestor(1u);
                }
                index++;
            }
            currentStep += nodeName;
            ret = InitialiseFunctionDatabaseFromConfiguration(originalConfig, currentStep);

            ret = originalConfig.MoveToAncestor(1u);
        }

    }
    return ret;
}

//-Creates a Function and a Data ConfigurationDatabase with all the GAMs and DataSource signals as they are declared in the
//original ConfigurationDatabase.
//-QualifiedName is the name of the GAM path beginning from "Functions" or the name of the DataSource beginning from "Data"
//-The name of each GAM and of each DS node is an incremental id number
bool RealTimeApplicationConfigurationBuilder::InitialiseSignalsDatabaseFromConfiguration(ConfigurationDatabase &originalConfig,
                                                                                         const char8* appName) {

    StreamString functionPath = appName;
    functionPath += ".Functions";
    StreamString dataPath = appName;
    dataPath += ".Data";

    bool ret = originalConfig.MoveAbsolute(functionPath.Buffer());

    //Create the Functions master node
    if (ret) {
        ret = functionsDatabase.CreateAbsolute("Functions");
    }
    if (ret) {
        StreamString initPath;
        uint32 initIndex = 0u;
        ret = InitialiseFunctionDatabaseFromConfiguration(originalConfig, initPath, initIndex);
    }

    if (ret) {
        ret = originalConfig.MoveAbsolute(dataPath.Buffer());
    }

    if (ret) {
        StreamString initPath;
        uint32 initIndex = 0u;
        ret = InitialiseDataDatabaseFromConfiguration(originalConfig, initPath, initIndex);
    }

    if (ret) {
        ret = functionsDatabase.MoveToRoot();
    }

    if (ret) {
        ret = dataSourcesDatabase.MoveToRoot();
    }
    return ret;
}
#endif

////////////////////////////////
////////////////////////////////
// FlattenSignalsDatabases
////////////////////////////////
////////////////////////////////

//-Flats all the signals as long their type are declared
bool RealTimeApplicationConfigurationBuilder::FlattenSignalsDatabases() {
    bool ret = functionsDatabase.MoveAbsolute("Functions");

    if (ret) {
        ret = FlattenSignalsDatabase(true, InputSignals);
    }
    if (ret) {
        ret = functionsDatabase.MoveAbsolute("Functions");
    }
    if (ret) {
        ret = FlattenSignalsDatabase(true, OutputSignals);
    }
    if (ret) {
        ret = dataSourcesDatabase.MoveAbsolute("Data");
    }
    if (ret) {
        ret = FlattenSignalsDatabase(false, None);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::FlattenSignalsDatabase(bool isFunctionsDatabase,
                                                                     SignalDirection direction) {
    const char8 *signalDirection = "Signals";
    if (direction == InputSignals) {
        signalDirection = "InputSignals";
    }
    else if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }

    ConfigurationDatabase &signalDatabase = (isFunctionsDatabase) ? (functionsDatabase) : (dataSourcesDatabase);

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
                        ret = FlattenSignal(isFunctionsDatabase, signalName, signalLocalDatabase, signalNumber, !isDataSource);
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

bool RealTimeApplicationConfigurationBuilder::FlattenSignal(bool isFunctionsDatabase,
                                                            const char8 * const signalName,
                                                            ConfigurationDatabase &resolvedSignal,
                                                            uint32 &signalNumber,
                                                            bool forceWriteDataSource) {
    bool ret = true;
    StreamString signalType;

    ConfigurationDatabase &signalDatabase = (isFunctionsDatabase) ? (functionsDatabase) : (dataSourcesDatabase);

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
                        ret = FlattenSignal(isFunctionsDatabase, signalFullName.Buffer(), resolvedSignal, signalNumber, forceWriteDataSource);
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

                    //TODO Error if

                    AnyType ranges = signalDatabase.GetType("Ranges");
                    AnyType samples = signalDatabase.GetType("Samples");
                    AnyType frequency = signalDatabase.GetType("Frequency");
                    StreamString syncSignalName;
                    bool syncSet = true;
                    if (signalDatabase.Read("SyncSignal", syncSignalName)) {
                        if (frequency.IsVoid()) {
                            REPORT_ERROR_PARAMETERS(
                                    ErrorManagement::InitialisationError,
                                    "Specified a synchronising signal in %s with no synchronised frequency. Please define the \"Frequency\" field", signalName)
                        }
                        else {
                            // both syc signal and frequency specified... check if the member will be found
                            syncSet = false;
                        }
                    }

                    ret = SignalIntrospectionToStructuredData(signalDatabase, signalType.Buffer(), signalName, alias.Buffer(), dataSourceName.Buffer(),
                                                              syncSignalName.Buffer(), ranges, samples, frequency, resolvedSignal, signalNumber, syncSet);
                    if (ret) {
                        ret = syncSet;
                        if (!ret) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Invalid synchronising member specified in %s", signalName)

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
        if (!isStructuredData) {
            StreamString property;
            StreamString signalNumberStr;
            signalNumberStr.Printf("Signals.%d", signalNumber);
            ret = resolvedSignal.CreateAbsolute(signalNumberStr.Buffer());
            if (ret) {
                ret = resolvedSignal.Write("QualifiedName", signalName);
            }
            //Loop and copy all known properties at this time.
            const char8 *properties[] = { "Type", "NumberOfDimensions", "NumberOfElements", "Alias", "Ranges", "DataSource", "Samples", "Default", "Frequency",
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

////////////////////////////////
////////////////////////////////
// ResolveDataSources
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::ResolveDataSources() {
    bool ret = ResolveDataSources(InputSignals);
    if (ret) {
        ret = ResolveDataSources(OutputSignals);
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
        for (uint32 i = 0; (i < numberOfDataSources) && (ret); i++) {
            StreamString dataSourceName;

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
                    signalNumberOfBytes = (numberOfElements * signalTypeDescriptor.numberOfBits) / 8u;
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
                        // parse the DefaultValue
                        // check its compatibility with the type
                        StreamString defValConfig = "Default=";
                        defValConfig += defaultVal;
                        defValConfig.Seek(0u);
                        ConfigurationDatabase cdb;
                        StandardParser parser(defValConfig, cdb);
                        ret = parser.Parse();
                        if (ret) {
                            AnyType defVal = cdb.GetType("Default");
                            uint32 defValDims = defVal.GetNumberOfDimensions();
                            ret = (defValDims <= 1u);
                            uint32 usedDimensions = (numberOfDimensions > 0u) ? (1u) : (0u);
                            if (ret) {
                                ret = (defValDims == usedDimensions);
                                if (ret) {
                                    ret = (defVal.GetNumberOfElements(0u) == numberOfElements);
                                }
                            }
                            else {
                                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError,
                                                        "Default value of signal %s in %s must be defined as a vector for multi-dimensional variables",
                                                        signalName.Buffer(), dataSourceName.Buffer())
                            }
                            if (ret) {
                                void *ptr = HeapManager::Malloc(signalNumberOfBytes);
                                ret = (ptr != NULL);
                                if (ret) {
                                    AnyType at = AnyType(signalTypeDescriptor, 0u, ptr);

                                    at.SetNumberOfDimensions(usedDimensions);
                                    at.SetNumberOfElements(0u, numberOfElements);

                                    ret = parser.Parse();
                                    if (ret) {
                                        ret = cdb.Read("Default", at);
                                        if (ret) {
                                            ret = dataSourcesDatabase.Delete("Default");
                                        }
                                        if (ret) {
                                            // write the default value as a value and not a string anymore!
                                            ret = dataSourcesDatabase.Write("Default", at);
                                        }
                                    }
                                    ret = HeapManager::Free(reinterpret_cast<void*&>(ptr));
                                }
                            }
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

////////////////////////////////
////////////////////////////////
// ResolveFunctionSignals
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::ResolveFunctionSignals() {
    bool ret = ResolveFunctionSignals(InputSignals);
    if (ret) {
        ret = ResolveFunctionSignals(OutputSignals);
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
                uint32 initNumberOfSignals = numberOfSignals;
                //For every signal in this function
                for (uint32 s = 0u; (s < numberOfSignals) && (ret); s++) {
                    StreamString signalId;
                    signalId = functionsDatabase.GetChildName(s);
                    ret = functionsDatabase.MoveRelative(signalId.Buffer());
                    StreamString signalName = "";
                    StreamString aliasName = "";

                    if (ret) {
                        //Expand the aliasName in order to write the correct name in the dataSourcesDatabase
                        ret = functionsDatabase.Read("QualifiedName", signalName);
                        if (ret) {
                            if (!functionsDatabase.Read("Alias", aliasName)) {
                                aliasName = signalName.Buffer();
                            }
                            if (aliasName.Size() == 0u) {
                                aliasName = signalName.Buffer();
                            }
                        }
                        printf("\nConsider %s %s \n", aliasName.Buffer(), signalName.Buffer());
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
                    bool deleted = false;
                    if (ret) {

                        ret = ResolveFunctionSignal(signalName.Buffer(), aliasName.Buffer(), functionName.Buffer(), dataSourceName.Buffer(),
                                                    initNumberOfSignals, deleted);
                    }
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Signal %s not found in %s", signalName.Buffer(), dataSourceName.Buffer())
                    }
                    if (ret) {
                        if (deleted) {
                            numberOfSignals--;
                            s--;
                        }
                        //MoveToAncestor is done in ResolveFunctionSignal
                        //ret = functionsDatabase.MoveToAncestor(1u);
                        //numberOfSignals = functionsDatabase.GetNumberOfChildren();
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

bool RealTimeApplicationConfigurationBuilder::ResolveFunctionSignal(const char8 * const signalName,
                                                                    const char8 * const aliasName,
                                                                    const char8 * const functionName,
                                                                    const char8 * const dataSourceName,
                                                                    uint32 &numberOfFunctionSignals,
                                                                    bool &deleted) {

    deleted = false;
    uint32 numberOfSignalsInDataSource = dataSourcesDatabase.GetNumberOfChildren();
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
        StreamString aliasNameStr = aliasName;
        aliasNameStr.Seek(0u);
        uint32 aliasNameSize = aliasNameStr.Size();
        // need to create sub fields

        bool createNestedSignal = (aliasNameSize < dataSourceSignalName.Size());
        if (ret) {
            StreamString token1;
            StreamString token2;
            char8 terminator;
            signalFound = true;
            while (aliasNameStr.GetToken(token1, ".", terminator) && (signalFound)) {
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
                    StreamString newNodeName = signalName;
                    newNodeName += ".";
                    newNodeName += &(dataSourceSignalName.Buffer()[dataSourceSignalName.Position()]);
                    aliasNameStr += ".";
                    aliasNameStr += &(dataSourceSignalName.Buffer()[dataSourceSignalName.Position()]);

                    //Mark this signal to be deleted
                    if (!atLeastOneSignalFound) {
                        signalToDeleteName = functionsDatabase.GetName();

                    }
                    if (ret) {
                        ret = functionsDatabase.MoveToAncestor(1u);
                    }
                    if (ret) {
                        StreamString newSignalId;
                        numberOfFunctionSignals++;
                        newSignalId.Printf("%d", (numberOfFunctionSignals));
                        ret = functionsDatabase.CreateRelative(newSignalId.Buffer());
                    }

                    if (ret) {

                        ret = functionsDatabase.Write("QualifiedName", newNodeName.Buffer());
                    }
                    if (ret) {
                        ret = functionsDatabase.Write("DataSource", dataSourceName);
                    }
                    if (ret) {
                        ret = functionsDatabase.Write("Alias", aliasNameStr.Buffer());
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
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "%s was not defined for signal: %s in %s", properties[p], aliasName,
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
                        fullPropertyName.Printf("%s.%s", aliasName, properties[p]);
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Mismatch in signal with name: %s. %s asked for %s while %s asked for %s",
                                                aliasName, functionName, sElementSignalDatabase.Buffer(), dataSourceName, sElementDataSourceDatabase.Buffer())
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
                deleted=true;
            }
        }
    }
    return ret;
}

////////////////////////////////
////////////////////////////////
// VerifyFunctionSignals
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::VerifyFunctionSignals() {
    bool ret = VerifyFunctionSignals(InputSignals);
    if (ret) {
        ret = VerifyFunctionSignals(OutputSignals);
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
    if (ret) {
        numberOfFunctions = functionsDatabase.GetNumberOfChildren();
    }
    if (ret) {
        //For every function
        for (uint32 i = 0; (i < numberOfFunctions) && (ret); i++) {
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
                    if (ret) {
                        //Move to next Signal in this signalDirection
                        ret = functionsDatabase.MoveToAncestor(1u);
                    }
                }
            }
            //Move to next Function
            ret = functionsDatabase.MoveAbsolute("Functions");
        }
    }
    return ret;
}

////////////////////////////////
////////////////////////////////
// ResolveStates
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::ResolveStates() {

    //TODO Absolute path with respect to Functions without using realTimeApplication

    bool ret = true;
    ReferenceContainer statesFound;
    //Look for all the RealTimeStates
    ReferenceContainerFilterReferencesTemplate<RealTimeState> stateFilter(-1, ReferenceContainerFilterMode::RECURSIVE);

    realTimeApplication->Find(statesFound, stateFilter);
    uint32 numberOfStates = statesFound.Size();
    //Loop on each state...
    for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
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
                uint32 syncSignals = 0u;
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
                            syncSignals += GetNumberOfSyncSignals(functionsDatabase, "Signals.InputSignals", ret);
                            if (ret) {
                                syncSignals += GetNumberOfSyncSignals(functionsDatabase, "Signals.OutputSignals", ret);
                            }
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
                    if (ret) {
                        //TODO Always a synchronisation point???
                        ret = (syncSignals <= 1u);
                        if (!ret) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "More than one synchronising signal found in %s.%s", stateName,
                                                    threadName)
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

////////////////////////////////
////////////////////////////////
// VerifyStates
////////////////////////////////
////////////////////////////////

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

////////////////////////////////
////////////////////////////////
// ResolveConsumersAndProducers
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::ResolveConsumersAndProducers() {
    bool ret = ResolveConsumersAndProducers(true);
    if (ret) {
        ret = ResolveConsumersAndProducers(false);
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
                                        StreamString operationTypeGAM = "GAM";
                                        operationTypeGAM += operationType;
                                        StreamString operationTypeSignal = "Signal";
                                        operationTypeSignal += operationType;
                                        AnyType existentArray = dataSourcesDatabase.GetType(operationTypeGAM.Buffer());
                                        StreamString *newGAMArray = NULL_PTR(StreamString *);
                                        StreamString *newSignalArray = NULL_PTR(StreamString *);

                                        uint32 numberOfExistentElements = 0u;
                                        if (existentArray.GetDataPointer() != NULL_PTR(void *)) {
                                            numberOfExistentElements = existentArray.GetNumberOfElements(0u);
                                            newGAMArray = new StreamString[numberOfExistentElements + 1u];
                                            newSignalArray = new StreamString[numberOfExistentElements + 1u];
                                            Vector<StreamString> newGAMVector(newGAMArray, numberOfExistentElements);
                                            Vector<StreamString> newSignalVector(newSignalArray, numberOfExistentElements);
                                            ret = (dataSourcesDatabase.Read(operationTypeGAM.Buffer(), newGAMVector));
                                            if (ret) {
                                                ret = (dataSourcesDatabase.Read(operationTypeSignal.Buffer(), newSignalVector));
                                            }
                                        }
                                        else {
                                            newGAMArray = new StreamString[1u];
                                            newSignalArray = new StreamString[1u];
                                        }
                                        newGAMArray[numberOfExistentElements] = functionId.Buffer();    //functionName.Buffer();
                                        newSignalArray[numberOfExistentElements] = signalId.Buffer();
                                        Vector<StreamString> newGAMVector(newGAMArray, numberOfExistentElements + 1u);
                                        Vector<StreamString> newSignalVector(newSignalArray, numberOfExistentElements + 1u);

                                        if (existentArray.GetDataPointer() != NULL_PTR(void *)) {
                                            ret = dataSourcesDatabase.Delete(operationTypeGAM.Buffer());
                                            if (ret) {
                                                ret = dataSourcesDatabase.Delete(operationTypeSignal.Buffer());
                                            }
                                        }
                                        if (ret) {
                                            ret = (dataSourcesDatabase.Write(operationTypeGAM.Buffer(), newGAMVector));
                                            if (ret) {
                                                ret = (dataSourcesDatabase.Write(operationTypeSignal.Buffer(), newSignalVector));
                                            }
                                        }
                                        delete[] newGAMArray;
                                        delete[] newSignalArray;
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

////////////////////////////////
////////////////////////////////
// VerifyConsumersAndProducers
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::VerifyConsumersAndProducers() {

    bool ret = dataSourcesDatabase.MoveAbsolute("Data");
    uint32 numberOfDS = dataSourcesDatabase.GetNumberOfChildren();
    for (uint32 i = 0u; (i < numberOfDS) && (ret); i++) {
        StreamString dataSourceId = dataSourcesDatabase.GetChildName(i);

        ret = dataSourcesDatabase.MoveRelative(dataSourceId.Buffer());

        StreamString dataSourceName;
        if (ret) {
            ret = dataSourcesDatabase.Read("QualifiedName", dataSourceName);
        }
        if (ret) {
            ret = dataSourcesDatabase.MoveRelative("Signals");
        }
        if (ret) {
            uint32 numberOfSignals = dataSourcesDatabase.GetNumberOfChildren();
            for (uint32 j = 0u; (j < numberOfSignals) && (ret); j++) {
                StreamString dsSignalId = dataSourcesDatabase.GetChildName(j);
                ret = dataSourcesDatabase.MoveRelative(dsSignalId.Buffer());
                StreamString dsSignalName;
                if (ret) {
                    ret = dataSourcesDatabase.Read("QualifiedName", dsSignalName);
                }
                if (ret) {
                    ret = dataSourcesDatabase.MoveRelative("States");
                }
                if (ret) {
                    uint32 numberOfStates = dataSourcesDatabase.GetNumberOfChildren();
                    for (uint32 k = 0u; (k < numberOfStates) && (ret); k++) {

                        ret = dataSourcesDatabase.MoveRelative(dataSourcesDatabase.GetChildName(k));
                        if (ret) {
                            // no Producers found... in this case the default value must be declared
                            AnyType prods = dataSourcesDatabase.GetType("GAMProducers");
                            if (prods.GetNumberOfElements(0u) > 1u) {
                                printf("\nVerify CP for %s\n", dsSignalName.Buffer());
                                // Check the range overlap
                                ret = BuildProducersRanges();
                                if (!ret) {
                                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                                                            "Producers of %s in %s overlap write operations on the same memory area", dsSignalName.Buffer(),
                                                            dataSourceName.Buffer());
                                }
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
        StreamString ranges;
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
                            ret = MemoryOperationsHelper::Copy(temp, signalRanges, (minNumberOfSignalRanges * sizeof(uint32)));
                            minNumberOfSignalRanges = newSize;
                            delete[] signalRanges;
                            signalRanges = temp;
                        }
                        if (ret) {
                            signalRanges[reachSignalRanges] = rangesMatrix[k][0];
                            reachSignalRanges++;
                            signalRanges[reachSignalRanges] = rangesMatrix[k][1];
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

bool RealTimeApplicationConfigurationBuilder::CheckProducersRanges(uint32 *rangesArray,
                                                                   uint32 numberOfElements) {
    bool ret = (numberOfElements % 2u == 0u);
    for (uint32 i = 1u; (i < numberOfElements) && ret; i += 2u) {
        for (uint32 j = i + 2u; (j < numberOfElements) && (ret); j += 2u) {
            bool isFirstMin = (rangesArray[i - 1u] < rangesArray[j - 1u]) && (rangesArray[i] < rangesArray[j - 1]);
            bool isFirstMax = (rangesArray[j - 1u] < rangesArray[i - 1u]) && (rangesArray[j] < rangesArray[i - 1]);
            ret = (isFirstMin || isFirstMax);
        }
    }
    return ret;
}

////////////////////////////////
////////////////////////////////
// ResolveFunctionSignalsMemorySize
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::ResolveFunctionSignalsMemorySize() {
    bool ret = ResolveFunctionSignalsMemorySize(InputSignals);
    if (ret) {
        ret = ResolveFunctionSignalsMemorySize(OutputSignals);
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
        //For every function
        for (uint32 i = 0; (i < numberOfFunctions) && (ret); i++) {
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
                //For every signal in this function
                for (uint32 s = 0u; (s < numberOfSignals) && (ret); s++) {
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
                                    for (uint32 n = 0u; (n < numberOfRanges) && (ret); n++) {
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
                                            uint32 rangeByteSize = (((maxIdx - minIdx) + 1u) * signalTypeDescriptor.numberOfBits) / 8u;
                                            signalNumberOfBytes += rangeByteSize;
                                            offsetMat[n][0u] = (minIdx * signalTypeDescriptor.numberOfBits) / 8u;
                                            offsetMat[n][1u] = rangeByteSize;
                                        }
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
                            signalNumberOfBytes = (numberOfElements * signalTypeDescriptor.numberOfBits) / 8u;
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

////////////////////////////////
////////////////////////////////
// ResolveFunctionsMemory
////////////////////////////////
////////////////////////////////
bool RealTimeApplicationConfigurationBuilder::ResolveFunctionsMemory() {
    bool ret = ResolveFunctionsMemory(InputSignals);
    if (ret) {
        ret = ResolveFunctionsMemory(OutputSignals);
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
                    uint32 *offsetMatrixBackend = NULL_PTR(uint32 *);
                    uint32 samplesBackend = 1u;
                    uint32 byteSize = 0u;
                    float32 frequencyBackend = -1.0;

                    signalId = functionsDatabase.GetChildName(s);
                    ret = functionsDatabase.MoveRelative(signalId.Buffer());
                    if (ret) {
                        ret = functionsDatabase.Read("QualifiedName", signalName);
                    }
                    if (ret) {
                        if (!functionsDatabase.Read("Alias", alias)) {
                            alias = signalName;
                        }
                    }
                    if (ret) {
                        ret = functionsDatabase.Read("DataSource", dataSourceName);
                    }
                    if (ret) {
                        ret = functionsDatabase.Read("ByteSize", byteSize);
                    }

                    if (ret) {
                        AnyType existentByteOffset = functionsDatabase.GetType("ByteOffset");
                        if (existentByteOffset.GetDataPointer() != NULL_PTR(void *)) {
                            numberOfOffsetElements = existentByteOffset.GetNumberOfElements(1u);
                            // create a static matrix
                            offsetMatrixBackend = new uint32[numberOfOffsetElements * 2u];
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
                    }
                    if (ret) {
                        if (!functionsDatabase.Read("Frequency", frequencyBackend)) {
                            frequencyBackend = -1.0;
                        }
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
                        bool found = false;
                        StreamString dataSourceId;
                        for (uint32 d = 0u; (d < numberOfDataSources) && (ret) && (!found); d++) {
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
                        }/*
                         uint32 numberOfSignalsInDataSource = functionsDatabase.GetNumberOfChildren();
                         StreamString newSignalId;
                         if (ret) {
                         newSignalId.Printf("%d", signalId.Buffer());
                         }*/
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
                            if (offsetMatrixBackend != NULL_PTR(void *)) {
                                Matrix<uint32> offsetMat(offsetMatrixBackend, numberOfOffsetElements, 2u);
                                ret = functionsDatabase.Write("ByteOffset", offsetMat);
                                delete[] offsetMatrixBackend;
                            }
                        }
                        if (ret) {
                            ret = functionsDatabase.Write("Samples", samplesBackend);
                        }

                        if (ret) {
                            ret = functionsDatabase.Write("Frequency", frequencyBackend);
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
                            existentByteSize += byteSize * samplesBackend;
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

////////////////////////////////
////////////////////////////////
// CalculateFunctionsMemory
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::CalculateFunctionsMemory() {
    bool ret = CalculateFunctionsMemory(InputSignals);
    if (ret) {
        ret = CalculateFunctionsMemory(OutputSignals);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::CalculateFunctionsMemory(SignalDirection direction) {
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
            //Compute the total amount of memory
            if (ret) {
                ret = functionsDatabase.MoveRelative("Signals");
            }
            bool exists = false;
            if (ret) {
                exists = functionsDatabase.MoveRelative(signalDirection);
            }
            if (exists) {
                uint32 numberOfSignals = functionsDatabase.GetNumberOfChildren();
                uint32 s;
                uint32 totalByteSize = 0u;
                for (s = 0u; (s < numberOfSignals) && (ret); s++) {
                    ret = functionsDatabase.MoveRelative(functionsDatabase.GetChildName(s));
                    if (ret) {
                        uint32 byteSize = 0u;
                        ret = functionsDatabase.Read("ByteSize", byteSize);
                        totalByteSize += byteSize;
                    }
                    if (ret) {
                        functionsDatabase.MoveToAncestor(1u);
                    }
                }
                ret = functionsDatabase.Write("ByteSize", totalByteSize);

                //Move back to the Function level
                functionsDatabase.MoveToAncestor(2u);

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

                    uint32 allocatedBytes = 0u;
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
                        if (ret) {
                            ret = functionsDatabase.Write("GamMemoryOffset", allocatedBytes);
                            allocatedBytes += byteSize;
                        }
                        if (ret) {
                            //Move to SignalDirection level
                            ret = functionsDatabase.MoveToAncestor(1u);
                        }
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
////////////////////////////////
////////////////////////////////
// AssignFunctionsMemoryToDataSource
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::AssignFunctionsMemoryToDataSource() {
    bool ret = AssignFunctionsMemoryToDataSource(InputSignals);
    if (ret) {
        ret = AssignFunctionsMemoryToDataSource(OutputSignals);
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
                    uint32 gamMemoryOffset = 0u;
                    StreamString dataSourceName;
                    StreamString dataSourceId = functionsDatabase.GetChildName(d);
                    ret = functionsDatabase.MoveRelative(dataSourceId.Buffer());
                    if (ret) {
                        ret = functionsDatabase.Read("DataSource", dataSourceName);
                    }
                    if (ret) {
                        ret = functionsDatabase.Read("ByteSize", byteSize);
                    }/*
                     if (ret) {
                     ret = functionsDatabase.Read("Address", address);
                     }*/
                    if (ret) {
                        ret = functionsDatabase.Read("GamMemoryOffset", gamMemoryOffset);
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
                    }/*
                     if (ret) {
                     ret = dataSourcesDatabase.Write("Address", reinterpret_cast<void *>(address));
                     }*/
                    if (ret) {
                        ret = dataSourcesDatabase.Write("ByteSize", byteSize);
                    }
                    if (ret) {
                        ret = dataSourcesDatabase.Write("GamMemoryOffset", gamMemoryOffset);
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

////////////////////////////////
////////////////////////////////
// AssignBrokersToFunctions
////////////////////////////////
////////////////////////////////
bool RealTimeApplicationConfigurationBuilder::AssignBrokersToFunctions() {

    dataSourcesDatabase.MoveAbsolute("Data");
    uint32 numberOfDataSources = dataSourcesDatabase.GetNumberOfChildren();
    bool ret = true;
    for (uint32 i = 0u; (i < numberOfDataSources) && (ret); i++) {
        const char8 *dsId = dataSourcesDatabase.GetChildName(i);
        ret = dataSourcesDatabase.MoveRelative(dsId);

        StreamString dsName;
        if (ret) {
            ret = dataSourcesDatabase.Read("QualifiedName", dsName);
        }
        ReferenceT<DataSourceI> dataSource;
        if (ret) {
            StreamString fullDsPath = "Data.";
            fullDsPath += dsName;
            dataSource = realTimeApplication->Find(fullDsPath.Buffer());
            ret = dataSource.IsValid();
        }

        if (ret) {
            ret = dataSourcesDatabase.MoveRelative("Functions");
        }
        if (ret) {
            uint32 numberOfFunctions = dataSourcesDatabase.GetNumberOfChildren();
            for (uint32 j = 0u; (j < numberOfFunctions) && (ret); j++) {
                const char8 * functionId = dataSourcesDatabase.GetChildName(j);
                ret = dataSourcesDatabase.MoveRelative(functionId);
                if (ret) {
                    ret = AssignBrokersToSignals(InputSignals, dataSource);
                    if (ret) {
                        ret = AssignBrokersToSignals(OutputSignals, dataSource);
                    }
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
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::AssignBrokersToSignals(SignalDirection direction,
                                                                     ReferenceT<DataSourceI> dataSource) {
    const char8 * dirStr = "InputSignals";
    if (direction == OutputSignals) {
        dirStr = "OutputSignals";
    }

    bool ret = true;
    if (dataSourcesDatabase.MoveRelative(dirStr)) {
        uint32 numberOfSignals = dataSourcesDatabase.GetNumberOfChildren();
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            StreamString signalId = dataSourcesDatabase.GetChildName(i);
            if (dataSourcesDatabase.MoveRelative(signalId.Buffer())) {

                if (ret) {
                    // return the broker depending on the signal configuration
                    StreamString brokerClassName = dataSource->Negotiate(dataSourcesDatabase, direction);
                    ret = brokerClassName.Size() > 0u;
                    if (ret) {
                        ret = dataSourcesDatabase.Write("Broker", brokerClassName.Buffer());
                    }
                    else {
                        //TODO Failed negotiation !!!
                    }
                }
                if (ret) {
                    ret = dataSourcesDatabase.MoveToAncestor(1u);
                }
            }
        }
        if (ret) {
            ret = dataSourcesDatabase.MoveToAncestor(1u);
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
        for (n = 0u; (n < numberOfDataSources) && (ret); n++) {
            StreamString qualifiedName = "Data.";
            StreamString dataSourceIdx;
            dataSourceIdx.Printf("%d", n);
            ret = dataSourcesDatabase.MoveRelative(dataSourceIdx.Buffer());
            if (ret) {
                ret = dataSourcesDatabase.Read("QualifiedName", qualifiedName);
            }
            ReferenceT<DataSourceI> dataSource;
            if (ret) {/*
             StreamString fullDsName="Data.";
             fullDsName+=qualifiedName;*/
                dataSource = realTimeApplication->Find(qualifiedName.Buffer());
                ret = dataSource.IsValid();
            }
            if (ret) {
                ret = dataSource->SetConfiguredDatabase(dataSourcesDatabase);
            }
            /*  if (ret) {
             ret = dataSource->AllocateMemory();
             }*/
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
            StreamString qualifiedName = "Functions.";
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

bool RealTimeApplicationConfigurationBuilder::Set(ConfigurationDatabase &functionsDatabaseIn,
                                                  ConfigurationDatabase &dataSourcesDatabaseIn) {
    bool ret = functionsDatabase.MoveToRoot();
    if (ret) {
        ret = functionsDatabaseIn.Copy(functionsDatabase);
    }
    if (ret) {
        dataSourcesDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabaseIn.Copy(dataSourcesDatabase);
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

