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
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

RealTimeApplicationConfigurationBuilder::RealTimeApplicationConfigurationBuilder(RealTimeApplication & realTimeApplicationIn,
                                                                                 const char8 * const defaultDataSourceNameIn) {
    defaultDataSourceName = defaultDataSourceNameIn;
    realTimeApplication = &realTimeApplicationIn;
}

RealTimeApplicationConfigurationBuilder::RealTimeApplicationConfigurationBuilder(ConfigurationDatabase &globalDatabaseIn,
                                                                                 const char8 * const defaultDataSourceNameIn) {
    defaultDataSourceName = defaultDataSourceNameIn;
    bool ret = globalDatabaseIn.Copy(globalDatabase);
    if (ret) {
        ret = globalDatabase.MoveToRoot();
    }
    if (!ret) {
        REPORT_ERROR_FULL(ErrorManagement::FatalError, "In RealTimeApplicationConfigurationBuilder failed to MoveToRoot");
    }
    realTimeApplication = NULL_PTR(RealTimeApplication*);
}

bool RealTimeApplicationConfigurationBuilder::ConfigureAfterInitialisation() {

    bool ret = InitialiseSignalsDatabase();
    if (ret) {
        ret = FlattenSignalsDatabases();
    }
    if (ret) {
        ret = ResolveDataSources();
    }
    if (ret) {
        ret = VerifyDataSourcesSignals();
    }
    if (ret) {
        ret = ResolveFunctionSignals();
    }
    if (ret) {
        ret = VerifyFunctionSignals();
    }
    if (ret) {
        ret = ResolveStates();
    }
    if (ret) {
        ret = ResolveConsumersAndProducers();
    }
    if (ret) {
        ret = VerifyConsumersAndProducers();
    }
    if (ret) {
        ret = ResolveFunctionSignalsMemorySize();
    }
    if (ret) {
        ret = ResolveFunctionsMemory();
    }
    if (ret) {
        ret = AssignFunctionsMemoryToDataSource();
    }
    if (ret) {
        ret = AssignBrokersToFunctions();
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::ConfigureBeforeInitialisation() {

    bool ret = InitialiseSignalsDatabaseFromConfiguration();
    if (ret) {
        ret = FlattenSignalsDatabases();
    }
    if (ret) {
        ret = ResolveDataSources();
    }
    if (ret) {
        ret = VerifyDataSourcesSignals();
    }
    if (ret) {
        ret = ResolveFunctionSignals();
    }
    if (ret) {
        ret = VerifyFunctionSignals();
    }
    if (ret) {
        ret = ResolveStatesFromConfiguration();
    }
    if (ret) {
        ret = ResolveConsumersAndProducers();
    }
    if (ret) {
        ret = VerifyConsumersAndProducers();
    }
    if (ret) {
        ret = ResolveFunctionSignalsMemorySize();
    }
    if (ret) {
        ret = ResolveFunctionsMemory();
    }
    if (ret) {
        ret = AssignFunctionsMemoryToDataSource();
    }
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
                REPORT_ERROR(ErrorManagement::InitialisationError, "Only one TimingDataSource per application allowed");
            }
            else {
                if (timingDataSourceCounter == 0u) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Please specify a TimingDataSource to store GAMs relevant times");
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
                            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Unsupported nested GAMs in path %s", qualifiedName.Buffer())
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
                    ret = functionsDatabase.CreateRelative(functionN.Buffer());
                }
                if (ret) {
                    ret = functionsDatabase.Write("QualifiedName", qualifiedName.Buffer());
                }
                if (ret) {
                    //Declare all the signals with which this GAM interacts
                    ret = gam->AddSignals(functionsDatabase);
                    if (ret) {
                        ret = functionsDatabase.MoveRelative("Signals");
                        if (ret) {
                            if (!functionsDatabase.MoveRelative("InputSignals")) {
                                ret = functionsDatabase.MoveRelative("OutputSignals");
                                if (!ret) {
                                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Specified GAM %s with no input nor output",
                                                            qualifiedName.Buffer())
                                }
                            }
                        }
                        if (ret) {
                            ret = functionsDatabase.MoveToAncestor(2u);
                        }
                    }
                }
                if (ret) {
                    //Move to the next GAM
                    ret = functionsDatabase.MoveToAncestor(1u);
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
                    //Declare all the signals with which this GAM interacts
                    ret = dataSource->AddSignals(dataSourcesDatabase);
                }
                const ClassProperties *prop = NULL;
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
                else {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Failed to AddSignals for %s", dataSource->GetName())
                }

            }
            if (ret) {
                ret = (isTimeStamp == 1u);
                if (isTimeStamp > 1u) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Only one TimingDataSource per application allowed");
                }
                else if (isTimeStamp == 0u) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Please specify a TimingDataSource to store GAMs relevant times");
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
        REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid RealTimeApplication set");
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
    for (i = 0u; (i < numberOfSignalContainers) && (ret); i++) {
        // move inside GAM or DataSource
        const char8 *containerId = signalDatabase.GetChildName(i);
        ret = signalDatabase.MoveRelative(containerId);
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
                uint32 j = 0u;
                //...then for each signal...
                //for (j = 0; (j < numberOfSignals) && (ret); j++) {
                while ((j < numberOfSignals) && (ret)) {
                    const char8 *signalName = signalDatabase.GetChildName(j);
                    if (StringHelper::Compare(signalName, "Locked") != 0) {
                        ret = signalDatabase.MoveRelative(signalName);
                        if (ret) {
                            //resolve the full signal properties (recursing if need to signal namespace and introspection)
                            ret = FlattenSignal(isFunctionsDatabase, signalName, signalLocalDatabase, signalNumber);
                        }
                        if (ret) {
                            //Move to the next Signal
                            ret = signalDatabase.MoveToAncestor(1u);
                        }
                    }
                    else {
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
                ret = signalLocalDatabase.Copy(signalDatabase);
            }
            if (ret) {
                //Move to the Signals level
                if (isFunctionsDatabase) {
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

bool RealTimeApplicationConfigurationBuilder::FlattenSignal(const bool isFunctionsDatabase,
                                                            const char8 * const signalName,
                                                            ConfigurationDatabase &resolvedSignal,
                                                            uint32 &signalNumber) {
    bool ret = true;
    StreamString signalType;

    ConfigurationDatabase &signalDatabase = (isFunctionsDatabase) ? (functionsDatabase) : (dataSourcesDatabase);

    // a node does not have the field Type specified and must contain at least another node (namespace or signal)
    bool signalTypeDefined = signalDatabase.Read("Type", signalType);
    if (signalTypeDefined) {
        signalTypeDefined = (signalType.Size() > 0u);
    }
    //Check if it is a pure node (i.e. a nested namespace which contains other signals)
    bool foundANode = false;
    if (!signalTypeDefined) {
        uint32 numberOfElements = signalDatabase.GetNumberOfChildren();
        uint32 n;
        for (n = 0u; (n < numberOfElements); n++) {
            StreamString elementName = signalDatabase.GetChildName(n);
            //If this element is a node then recurse
            // "MemberAliases" and "Defaults" the only node can be found in a signal. Mark it as a keyword.
            if (signalDatabase.MoveRelative(elementName.Buffer())) {
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
                            ret = signalDatabase.MoveToAncestor(1u);
                        }
                    }
                }
            }
        }
    }
    //Namespace resolved. Working at signal level
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
                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "DataSource not specified for %s and DefaultDataSource not specified",
                                            signalName)
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
                    ret = (numberOfDimensions == 0u);
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(
                                ErrorManagement::InitialisationError,
                                "Invalid NumberOfDimensions for signal %s. Structured types only support NumberOfDimensions = 0 (you may define arrays of basic types inside the structure)",
                                signalName)
                    }
                }
                if (signalDatabase.Read("NumberOfElements", numberOfElements)) {
                    ret = (numberOfElements == 1u);
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(
                                ErrorManagement::InitialisationError,
                                "Invalid NumberOfElements for signal %s. Structured types only support NumberOfElements = 1 (you may define arrays of basic types inside the structure)",
                                signalName)
                    }
                }

                if (ret) {
                    isStructuredData = true;

                    AnyType ranges = signalDatabase.GetType("Ranges");
                    AnyType samples = signalDatabase.GetType("Samples");
                    AnyType frequency = signalDatabase.GetType("Frequency");
                    StreamString syncSignalName;
                    bool syncSet = true;
                    if (signalDatabase.Read("SyncSignal", syncSignalName)) {
                        ret = !frequency.IsVoid();
                        if (ret) {
                            // both sync signal and frequency specified... check if the member will be found
                            syncSet = false;

                        }
                        else {
                            REPORT_ERROR_PARAMETERS(
                                    ErrorManagement::InitialisationError,
                                    "Specified a synchronising signal %s with no synchronised frequency in structured %s. Please define the \"Frequency\" field",
                                    syncSignalName.Buffer(), signalName)
                        }
                    }
                    else {
                        ret = frequency.IsVoid();
                        if (!ret) {
                            REPORT_ERROR_PARAMETERS(
                                    ErrorManagement::InitialisationError,
                                    "Specified a frequency with no synchronised signal in structured %s. Please define the \"SyncSignal\" field", signalName)

                        }
                    }

                    if (ret) {

                        ret = SignalIntrospectionToStructuredData(signalDatabase, signalType.Buffer(), signalName, alias.Buffer(), dataSourceName.Buffer(),
                                                                  syncSignalName.Buffer(), fullType.Buffer(), ranges, samples, frequency, resolvedSignal,
                                                                  signalNumber, syncSet, isFunctionsDatabase);
                    }
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
            if (ret) {
                if (fullType.Size() > 0u) {
                    ret = resolvedSignal.Write("FullType", fullType.Buffer());
                }
            }
            if (ret) {
                //Move to the next Signal
                ret = resolvedSignal.MoveToAncestor(1u);
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
    bool ret = ResolveDataSources(InputSignals);
    if (ret) {
        ret = ResolveDataSources(OutputSignals);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::ResolveDataSources(const SignalDirection direction) {
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
                        //StreamString type;
                        //if (functionsDatabase.Read("Type", type)) {
                        ret = AddSignalToDataSource(functionName, dataSourceName);
                        //}
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
                done = (StringHelper::Compare(qualifiedName.Buffer(), dataSourceName.Buffer()) == 0);
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

    bool isDsLocked = false;
    if (ret) {
        //check if the ds is locked
        ret = dataSourcesDatabase.MoveToAncestor(1u);
        if (ret) {
            uint32 locked;
            if (dataSourcesDatabase.Read("Locked", locked)) {
                isDsLocked = (locked != 0u);
            }
        }
        if (ret) {
            ret = dataSourcesDatabase.MoveRelative("Signals");
        }
    }

//Check if the signal already exists
    uint32 numberOfSignals = dataSourcesDatabase.GetNumberOfChildren();
    bool signalAlreadyExists = false;
    uint32 n;
    StreamString foundSignalId;
    for (n = 0u; (n < numberOfSignals) && (ret) && (!signalAlreadyExists); n++) {
        foundSignalId = dataSourcesDatabase.GetChildName(n);
        ret = dataSourcesDatabase.MoveRelative(foundSignalId.Buffer());
        StreamString dataSourceSignalName;
        if (ret) {
            ret = dataSourcesDatabase.Read("QualifiedName", dataSourceSignalName);
        }
        if (ret) {
            signalAlreadyExists = (StringHelper::Compare(signalName.Buffer(), dataSourceSignalName.Buffer()) == 0);
        }
        //Move to the next signal
        if (ret) {
            ret = dataSourcesDatabase.MoveToAncestor(1u);
        }
    }

    if (ret) {
        for (n = 0u; (n < numberOfSignals) && (ret); n++) {
            StreamString signalId = dataSourcesDatabase.GetChildName(n);
            ret = dataSourcesDatabase.MoveRelative(signalId.Buffer());
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

            if (ret) {
                ret = dataSourcesDatabase.MoveToAncestor(1u);
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "The data source assigned to the signal %s in %s is incompatible",
                                        originalSignalName.Buffer(), functionName.Buffer())
            }
        }
        if (signalAlreadyExists && ret) {
            ret = dataSourcesDatabase.MoveRelative(foundSignalId.Buffer());
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
            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Cannot add the signal %s in GAM %s because the related DataSource is locked",
                                    originalSignalName.Buffer(), functionName.Buffer())
        }
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
                        ret = (StringHelper::Compare(sElementSignalDatabase.Buffer(), sElementDataSourceDatabase.Buffer()) == 0);
                    }
                    if (!ret) {
                        //Report mismatch!
                        StreamString fullPropertyName;
                        ret = fullPropertyName.Printf("%s.%s", originalSignalName.Buffer(), properties[p]);
                        if (!ret) {
                            fullPropertyName = "Unknown";
                        }
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Mismatch in signal with name: %s: %s asked for %s while %s asked for %s",
                                                fullPropertyName.Buffer(), functionName.Buffer(), sElementSignalDatabase.Buffer(), dataSourceName.Buffer(),
                                                sElementDataSourceDatabase.Buffer())
                    }
                }
                else {
                    if (!isDsLocked) {
                        ret = dataSourcesDatabase.Write(properties[p], elementSignalDatabase);
                    }
                }
            }
            p++;
        }

        StreamString dsSignalFullType;
        bool replace = (!dataSourcesDatabase.Read("FullType", dsSignalFullType));

        if (!replace) {
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
        for (i = 0u; (i < numberOfDataSources) && (ret); i++) {
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
            uint32 s = 0u;
            while ((s < numberOfSignals) && (ret)) {
                StreamString signalId;
                signalId = dataSourcesDatabase.GetChildName(s);
                ret = dataSourcesDatabase.MoveRelative(signalId.Buffer());

                //The QualifiedName must be known
                StreamString signalName;
                if (ret) {
                    ret = dataSourcesDatabase.Read("QualifiedName", signalName);
                    if (ret) {
                        ret = (signalName.Size() > 0u);
                    }
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "QualifiedName was not defined for signal at position: %s in %s",
                                                signalId.Buffer(), dataSourceName.Buffer())
                    }
                }
                StreamString type;
                bool typeDef = dataSourcesDatabase.Read("Type", type);
                if (typeDef) {
                    typeDef = (type.Size() > 0u);
                }

                bool signalFound = false;
                if (ret) {
                    ret = dataSourcesDatabase.MoveToAncestor(1u);

                    // check if it is a partial definition. Delete if the type is not specified, error otherwise (empty node)
                    for (uint32 k = 0u; (k < numberOfSignals) && (!signalFound) && (ret); k++) {
                        if (k != s) {
                            char8 terminator;
                            StreamString otherSNumber = dataSourcesDatabase.GetChildName(k);
                            ret = dataSourcesDatabase.MoveRelative(otherSNumber.Buffer());
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
                                ret = dataSourcesDatabase.MoveToAncestor(1u);
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
                        else {
                            // This error is trapped in the Resolve
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The name %s in %s is defined as a node", signalName.Buffer(),
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
                                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "NumberOfElements of signal %s in %s cannot be zero",
                                                        signalName.Buffer(), dataSourceName.Buffer())

                            }
                        }
                    }

                    //If the number of dimensions is > 1 then the NumberOfElements must also be defined.
                    uint32 numberOfDimensions = 0u;
                    if (ret) {
                        if (!dataSourcesDatabase.Read("NumberOfDimensions", numberOfDimensions)) {
                            numberOfDimensions = 0u;
                            if (numberOfElements > 1u) {
                                REPORT_ERROR_PARAMETERS(
                                        ErrorManagement::Warning,
                                        "NumberOfDimensions is not defined for signal: %s in %s with NumberOfElements > 1, assuming it as 1 (vector)",
                                        signalName.Buffer(), dataSourceName.Buffer())
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
                            ret = defValConfig.Seek(0LLU);
                            ConfigurationDatabase cdb;
                            StandardParser parser(defValConfig, cdb);
                            if (ret) {
                                ret = parser.Parse();
                            }
                            if (ret) {
                                AnyType defVal = cdb.GetType("Default");
                                uint32 defValDims = defVal.GetNumberOfDimensions();
                                ret = (defValDims <= 1u);
                                uint8 usedDimensions = (numberOfDimensions > 0u) ? (1u) : (0u);
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
                                        if (ret) {
                                            ret = HeapManager::Free(reinterpret_cast<void*&>(ptr));
                                        }
                                    }
                                }
                                if (!ret) {
                                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Unsupported defined default value of signal %s in %s",
                                                            signalName.Buffer(), dataSourceName.Buffer())
                                }
                            }
                        }
                    }
                }
                if (!validSignalFound) {
                    if (ret) {
                        ret = dataSourcesDatabase.MoveToAncestor(1u);
                    }
                }
                s++;
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

bool RealTimeApplicationConfigurationBuilder::ResolveFunctionSignals(const SignalDirection direction) {
    const char8 *signalDirection = "InputSignals";
    if (direction == OutputSignals) {
        signalDirection = "OutputSignals";
    }

    bool ret = functionsDatabase.MoveAbsolute("Functions");
    uint32 numberOfFunctions = 0u;
    if (ret) {
        numberOfFunctions = functionsDatabase.GetNumberOfChildren();
        //For every function
        uint32 i;
        for (i = 0u; (i < numberOfFunctions) && (ret); i++) {
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
                uint32 s = 0u;
                while ((s < numberOfSignals) && (ret)) {
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
                    }
                    //Complement the properties with the information from the DataSource
                    StreamString dataSourceName;
                    if (ret) {
                        ret = functionsDatabase.Read("DataSource", dataSourceName);
                    }
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "DataSource was not defined for signal: %s in %s", signalName.Buffer(),
                                                functionName.Buffer())
                    }
                    //Move to the DataSource
                    StreamString dataSourceNumber;
                    if (ret) {
                        ret = FindDataSourceNumber(dataSourceName, dataSourceNumber);
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
                    }
                    bool deleted = false;
                    if (ret) {
                        ret = ResolveFunctionSignal(signalName.Buffer(), aliasName.Buffer(), functionName.Buffer(), dataSourceName.Buffer(),
                                                    initNumberOfSignals, deleted);
                    }
                    if (ret) {
                        if (deleted) {
                            numberOfSignals--;
                            s--;
                        }
                    }
                    s++;
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
    uint32 s;
    for (s = 0u; (s < numberOfSignalsInDataSource) && (ret) && ((!typeDefined) || (!atLeastOneSignalFound)); s++) {
        StreamString dataSourceSignalId = dataSourcesDatabase.GetChildName(s);
        ret = dataSourcesDatabase.MoveRelative(dataSourceSignalId.Buffer());
        StreamString dataSourceSignalName;
        if (ret) {
            ret = dataSourcesDatabase.Read("QualifiedName", dataSourceSignalName);
        }
        if (ret) {
            ret = dataSourceSignalName.Seek(0LLU);
        }
        bool signalFound = false;
        StreamString aliasNameStr = aliasName;
        if (ret) {
            ret = aliasNameStr.Seek(0LLU);
        }
        uint64 aliasNameSize = aliasNameStr.Size();
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
                    uint64 position = dataSourceSignalName.Position();
                    newNodeName += &(dataSourceSignalName.Buffer()[position]);
                    aliasNameStr += ".";
                    aliasNameStr += &(dataSourceSignalName.Buffer()[position]);

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
                        ret = newSignalId.Printf("%d", (numberOfFunctionSignals));
                        if (ret) {
                            ret = functionsDatabase.CreateRelative(newSignalId.Buffer());
                        }
                    }

                    if (ret) {

                        ret = functionsDatabase.Write("QualifiedName", newNodeName.Buffer());
                    }
                    if (ret) {
                        ret = functionsDatabase.Write("DataSource", dataSourceName);
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
                        ret = (StringHelper::Compare(sElementSignalDatabase.Buffer(), sElementDataSourceDatabase.Buffer()) == 0);
                    }
                    if (!ret) {
                        //Report mismatch!
                        StreamString fullPropertyName;
                        ret = fullPropertyName.Printf("%s.%s", aliasName, properties[p]);
                        if (ret) {
                            fullPropertyName = "Unknown";
                        }
                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Mismatch in signal with name: %s. %s asked for %s while %s asked for %s",
                                                fullPropertyName.Buffer(), functionName, sElementSignalDatabase.Buffer(), dataSourceName,
                                                sElementDataSourceDatabase.Buffer())
                    }
                }
                p++;
            }
            if (ret) {
                StreamString fullType;
                if (dataSourcesDatabase.Read("FullType", fullType)) {
                    StreamString exists;
                    if (functionsDatabase.Read("FullType", exists)) {
                        ret = functionsDatabase.Delete("FullType");
                    }
                    if (ret) {
                        ret = functionsDatabase.Write("FullType", fullType.Buffer());
                    }

                }
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
    // go up and delete the incomplete signal if needed
    if (ret) {
        ret = functionsDatabase.MoveToAncestor(1u);
        if (ret) {
            if (signalToDeleteName != NULL) {
                ret = functionsDatabase.Delete(signalToDeleteName);
                deleted = true;
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

bool RealTimeApplicationConfigurationBuilder::VerifyFunctionSignals(const SignalDirection direction) {
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

                        if (ret) {
                            ret = (type.Size() > 0u);
                        }
                        if (!ret) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Type was not defined for signal: %s in %s", signalName.Buffer(),
                                                    functionName.Buffer())
                        }
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
            if (ret) {
                //Move to next Function
                ret = functionsDatabase.MoveAbsolute("Functions");
            }
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

    bool ret = (realTimeApplication != NULL);
    if (ret) {
        ret = realTimeApplication->ConfigureArchitecture();
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
                //Look for a ReferenceContainer named Threads
                uint32 j;
                bool found = false;
                ReferenceT<ReferenceContainer> threadContainer;
                /*lint -e{613} NULL pointer checking done before entering here */
                for (j = 0u; (j < realTimeApplication->Size()) && (!found); j++) {
                    threadContainer = state->Get(j);
                    if (threadContainer.IsValid()) {
                        found = (StringHelper::Compare("Threads", threadContainer->GetName()) == 0);
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

                            ret = AddThreadCycleTime(threadFullName.Buffer());
                            //ReferenceT < GAM > *gams = thread->GetGAMs();
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
                                    ret = (functionsDatabase.GetType(stateName).IsVoid());
                                    if (ret) {
                                        ret = functionsDatabase.Write(stateName, threadName);
                                    }
                                    else {
                                        StreamString presentThread;
                                        ret = functionsDatabase.Read(stateName, presentThread);
                                        if (ret) {
                                            if (presentThread != threadName) {
                                                StreamString gamName;
                                                if (functionsDatabase.MoveToAncestor(1u)) {
                                                    if (!functionsDatabase.Read("QualifiedName", gamName)) {
                                                        gamName = "UnknownGAM";
                                                    }
                                                }
                                                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError,
                                                                        "The GAM %s is declared in more than one thread in %s", gamName.Buffer(), stateName)
                                            }
                                        }
                                    }
                                }
                            }
                            if (ret) {
                                ret = (syncSignals <= 1u);
                                if (!ret) {
                                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "More than one synchronising signal found in %s.%s",
                                                            stateName, threadName)
                                }
                            }
                        }
                    }
                }
                else {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "No Threads ReferenceContainer found in RealTimeState %s", stateName)
                }
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid RealTimeApplication set");
    }

    return ret;
}

bool RealTimeApplicationConfigurationBuilder::AddStateToGAM(const char8 * const gamName,
                                                            const char8 * const stateName,
                                                            const char8 * const threadName,
                                                            uint32 &syncSignals) {
    StreamString functionNumber;
    StreamString gamNameStr = gamName;
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
        ret = (functionsDatabase.GetType(&stateName[1]).IsVoid());
        if (ret) {
            ret = functionsDatabase.Write(&stateName[1], &threadName[1]);
        }
        else {
            StreamString presentThread;
            ret = functionsDatabase.Read(stateName, presentThread);
            if (ret) {
                if (presentThread != threadName) {
                    StreamString gamName;
                    if (functionsDatabase.MoveToAncestor(1u)) {
                        if (!functionsDatabase.Read("QualifiedName", gamName)) {
                            gamName = "UnknownGAM";
                        }
                    }
                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "The GAM %s is declared in more than one thread in %s", gamName.Buffer(),
                                            stateName)
                }
            }
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::AddStateToFunction(ConfigurationDatabase &local,
                                                                 const char8 * const functionName,
                                                                 const char8 * const stateName,
                                                                 const char8 * const threadName,
                                                                 uint32 &syncSignals) {
    StreamString functionPathStr = functionName;
    StreamString token = "+";
    char8 terminator;
    bool ret = local.MoveAbsolute("+Functions");
    if (!ret) {
        ret = local.MoveAbsolute("$Functions");
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
            ret = AddStateToGAM(gamName.Buffer(), stateName, threadName, syncSignals);
        }
        else {
            uint32 i;
            for (i = 0u; (i < numberOfGAMs) && (ret); i++) {
                const char8* gamId = output.GetChildName(i);
                ret = output.MoveRelative(gamId);

                StreamString gamName = functionName;
                gamName += ".";
                if (ret) {
                    ret = output.Read("QualifiedName", gamName);
                }
                if (ret) {
                    ret = AddStateToGAM(gamName.Buffer(), stateName, threadName, syncSignals);
                }
                if (ret) {
                    ret = output.MoveToAncestor(1u);
                }
            }
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::ResolveStatesFromConfiguration() {
    bool ret = globalDatabase.MoveAbsolute("+Functions");
    if (!ret) {
        ret = globalDatabase.MoveAbsolute("$Functions");
    }
    ConfigurationDatabase local;
    if (ret) {
        ret = local.AddToCurrentNode(globalDatabase);
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
            const char8 * stateName = globalDatabase.GetChildName(i);
            if (globalDatabase.MoveRelative(stateName)) {
                ret = globalDatabase.MoveRelative("+Threads");
                if (!ret) {
                    ret = globalDatabase.MoveAbsolute("$Threads");
                }
                if (ret) {
                    uint32 numberOfThreads = globalDatabase.GetNumberOfChildren();
                    uint32 j;
                    for (j = 0u; (j < numberOfThreads) && (ret); j++) {
                        const char8 * threadName = globalDatabase.GetChildName(j);
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
                                            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "More than one synchronising signal found in %s.%s",
                                                                    &stateName[1], &threadName[1])
                                        }
                                    }

                                }
                                else {
                                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "The Functions element must be defined as vector %s.%s",
                                                            &stateName[1], &threadName[1])
                                }
                            }
                            else {
                                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "The Functions element must be %s.%s", &stateName[1],
                                                        &threadName[1])
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

bool RealTimeApplicationConfigurationBuilder::AddThreadCycleTime(const char8 * const threadFullName) {
    bool ret = dataSourcesDatabase.MoveAbsolute("Data");
    uint32 numberOfDataSources = dataSourcesDatabase.GetNumberOfChildren();

    StreamString signalName = threadFullName;
    signalName += "_CycleTime";
    uint32 i;
    for (i = 0u; (i < numberOfDataSources) && (ret); i++) {
        StreamString dsId;
        dsId = dataSourcesDatabase.GetChildName(i);
        ret = dataSourcesDatabase.MoveRelative(dsId.Buffer());
        if (ret) {
            uint32 isTimingDataSource;
            if (dataSourcesDatabase.Read("IsTimingDataSource", isTimingDataSource)) {
                if (isTimingDataSource == 1u) {
                    ret = dataSourcesDatabase.MoveRelative("Signals");
                    if (ret) {
                        uint32 numberOfTimeSignals = dataSourcesDatabase.GetNumberOfChildren();
                        bool found = false;
                        uint32 j;
                        for (j = 0u; (j < numberOfTimeSignals) && (ret) && (!found); j++) {
                            StreamString signalId;
                            signalId = dataSourcesDatabase.GetChildName(j);
                            ret = dataSourcesDatabase.MoveRelative(signalId.Buffer());
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
                                ret = dataSourcesDatabase.MoveToAncestor(1u);
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
                                ret = WriteTimeSignalInfo(signalName.Buffer());
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
            }
        }
        if (ret) {
            ret = dataSourcesDatabase.MoveToAncestor(1u);
        }
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::WriteTimeSignalInfo(const char8 * const signalName) {
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
// VerifyStates
////////////////////////////////
////////////////////////////////

bool RealTimeApplicationConfigurationBuilder::VerifyStates() {
    bool ret = functionsDatabase.MoveAbsolute("Functions");
    uint32 numberOfFunctions = functionsDatabase.GetNumberOfChildren();
    uint32 i;
    for (i = 0u; (i < numberOfFunctions) && (ret); i++) {

        ret = functionsDatabase.MoveRelative(functionsDatabase.GetChildName(i));

        if (ret) {
            ret = functionsDatabase.MoveRelative("States");

            if (ret) {
                ret = functionsDatabase.GetNumberOfChildren() > 0u;
            }
            if (!ret) {
                StreamString gamName;
                if (!functionsDatabase.Read("QualifiedName", gamName)) {
                    gamName = "UnknownGAM";
                }
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The GAM %s is never called", gamName.Buffer())
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

bool RealTimeApplicationConfigurationBuilder::ResolveConsumersAndProducers(const bool consumers) {
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
            uint32 constN = n;
            ret = functionId.Printf("%d", constN);

            if (ret) {
                ret = functionsDatabase.MoveRelative(functionId.Buffer());
            }
            if (ret) {
                ret = functionsDatabase.Read("QualifiedName", functionName);
            }
            //For all the states
            if ((functionsDatabase.MoveRelative("States")) && (ret)) {
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
                            ret = functionsDatabase.MoveRelative(signalId.Buffer());

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
                                        StreamString operationTypeGAMNames = "GAMNames";
                                        operationTypeGAMNames += operationType;
                                        StreamString operationTypeSignal = "Signal";
                                        operationTypeSignal += operationType;
                                        AnyType existentArray = dataSourcesDatabase.GetType(operationTypeGAM.Buffer());
                                        StreamString *newGAMArray = NULL_PTR(StreamString *);
                                        StreamString *newGAMNamesArray = NULL_PTR(StreamString *);
                                        StreamString *newSignalArray = NULL_PTR(StreamString *);

                                        uint32 numberOfExistentElements = 0u;
                                        if (existentArray.GetDataPointer() != NULL_PTR(void *)) {
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
                                        newGAMArray[numberOfExistentElements] = functionId.Buffer();
                                        newGAMNamesArray[numberOfExistentElements] = functionName.Buffer();
                                        newSignalArray[numberOfExistentElements] = signalId.Buffer();
                                        Vector<StreamString> newGAMVector(newGAMArray, numberOfExistentElements + 1u);
                                        Vector<StreamString> newGAMNamesVector(newGAMNamesArray, numberOfExistentElements + 1u);
                                        Vector<StreamString> newSignalVector(newSignalArray, numberOfExistentElements + 1u);

                                        if (existentArray.GetDataPointer() != NULL_PTR(void *)) {
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
        bool isTimeDs = false;
        StreamString dataSourceId = dataSourcesDatabase.GetChildName(i);

        ret = dataSourcesDatabase.MoveRelative(dataSourceId.Buffer());
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
                    if (!dataSourcesDatabase.MoveRelative("States")) {
                        ret = dataSourcesDatabase.CreateRelative("States");
                    }
                }
                if (ret) {
                    uint32 numberOfStates = dataSourcesDatabase.GetNumberOfChildren();
                    for (uint32 k = 0u; (k < numberOfStates) && (ret); k++) {

                        ret = dataSourcesDatabase.MoveRelative(dataSourcesDatabase.GetChildName(k));
                        if (ret) {
                            // no Producers found... in this case the default value must be declared
                            AnyType prods = dataSourcesDatabase.GetType("GAMProducers");
                            if (isTimeDs) {
                                //the time data source must not have producers!!
                                ret = prods.IsVoid();
                                if (!ret) {
                                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The time signal %s in %s cannot be produced", dsSignalName.Buffer(),
                                                            dataSourceName.Buffer())
                                }
                            }
                            else {
                                if (!prods.IsVoid()) {
                                    if (prods.GetNumberOfElements(0u) > 1u) {
                                        // Check the range overlap
                                        ret = BuildProducersRanges();
                                        if (!ret) {
                                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                                                                    "Producers of %s in %s overlap write operations on the same memory area",
                                                                    dsSignalName.Buffer(), dataSourceName.Buffer())
                                        }
                                    }
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

bool RealTimeApplicationConfigurationBuilder::CheckProducersRanges(const uint32 * const rangesArray,
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
        for (i = 0u; (i < numberOfFunctions) && (ret); i++) {
            StreamString functionId;
            functionId = functionsDatabase.GetChildName(i);
            ret = functionsDatabase.MoveRelative(functionId.Buffer());

            StreamString functionName;
            if (ret) {
                ret = functionsDatabase.Read("QualifiedName", functionName);
            }

            const char8 * postfix[] = { "_ReadTime", "_ExecTime", "_WriteTime", NULL_PTR(const char8*) };
            for (uint32 k = 0u; (postfix[k] != NULL) && ret; k++) {
                StreamString signalNameStr = functionName;
                signalNameStr += postfix[k];
                uint32 numberOfPresentSignals = dataSourcesDatabase.GetNumberOfChildren();
                bool found = false;
                uint32 j;
                for (j = 0u; (j < numberOfPresentSignals) && (ret) && (!found); j++) {
                    StreamString timeStrId = dataSourcesDatabase.GetChildName(j);
                    ret = dataSourcesDatabase.MoveRelative(timeStrId.Buffer());

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
                    if (ret) {
                        ret = dataSourcesDatabase.MoveToAncestor(1u);
                    }
                }
                if ((!found) && (ret)) {
                    StreamString newSignalIdx;
                    uint32 nextIndex = dataSourcesDatabase.GetNumberOfChildren();
                    ret = newSignalIdx.Printf("%d", nextIndex);
                    if (ret) {
                        ret = dataSourcesDatabase.CreateRelative(newSignalIdx.Buffer());
                    }
                    if (ret) {
                        ret = WriteTimeSignalInfo(signalNameStr.Buffer());
                    }
                    if (ret) {
                        ret = dataSourcesDatabase.MoveToAncestor(1u);
                    }
                }
            }
            if (ret) {
                ret = functionsDatabase.MoveToAncestor(1u);

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

bool RealTimeApplicationConfigurationBuilder::ResolveFunctionSignalsMemorySize() {
    bool ret = ResolveFunctionSignalsMemorySize(InputSignals);
    if (ret) {
        ret = ResolveFunctionSignalsMemorySize(OutputSignals);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::ResolveFunctionSignalsMemorySize(const SignalDirection direction) {
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
        for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
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
                                                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError,
                                                                        "Illegal Ranges for signal %s in %s: minimum index must be <= maximum index",
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
                                                offsetMat(n, 0u) = (minIdx * signalTypeDescriptor.numberOfBits) / 8u;
                                                offsetMat(n, 1u) = rangeByteSize;
                                            }
                                        }
                                    }
                                    else {
                                        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Ranges must be a nx2 matrix for %s in %s",
                                                                signalName.Buffer(), functionName.Buffer())
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
                        ret = functionsDatabase.Write("ByteSize", signalNumberOfBytes);
                    }
                    if (ret) {
                        if (offsetMatrixBackend != NULL_PTR(uint32 *)) {
                            Matrix<uint32> offsetMat(offsetMatrixBackend, numberOfRanges, 2u);
                            ret = functionsDatabase.Write("ByteOffset", offsetMat);
                        }
                    }
                    if (offsetMatrixBackend != NULL_PTR(uint32 *)) {
                        delete[] offsetMatrixBackend;
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

bool RealTimeApplicationConfigurationBuilder::ResolveFunctionsMemory(const SignalDirection direction) {
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
                uint32 totalByteSize = 0u;
                uint32 numberOfSignals = functionsDatabase.GetNumberOfChildren();
                uint32 s;
                //For every signal in this function
                uint32 allocatedByteSize = 0u;
                for (s = 0u; (s < numberOfSignals) && (ret); s++) {
                    StreamString signalId;
                    StreamString signalName;
                    StreamString alias;
                    StreamString dataSourceName;
                    uint32 numberOfOffsetElements = 0u;
                    uint32 *offsetMatrixBackend = NULL_PTR(uint32 *);
                    uint32 samplesBackend = 1u;
                    uint32 byteSize = 0u;
                    float32 frequencyBackend = -1.0F;

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
                                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Invalid Samples for signal %s in %s", signalName.Buffer(),
                                                        functionName.Buffer())
                            }
                        }
                    }
                    if (ret) {
                        totalByteSize += (byteSize * samplesBackend);

                        if (!functionsDatabase.Read("Frequency", frequencyBackend)) {
                            frequencyBackend = -1.0F;
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
                                found = (StringHelper::Compare(thisDataSourceName.Buffer(), dataSourceName.Buffer()) == 0);
                            }
                            //Move to the next DataSource
                            if (!found) {
                                ret = functionsDatabase.MoveToAncestor(1u);
                            }
                        }
                        if (ret) {
                            if (!found) {
                                StreamString newDataSourceId;
                                ret = newDataSourceId.Printf("%d", numberOfDataSources);
                                if (ret) {
                                    ret = functionsDatabase.CreateRelative(newDataSourceId.Buffer());
                                }
                                if (ret) {
                                    ret = functionsDatabase.Write("DataSource", dataSourceName.Buffer());
                                }
                            }
                        }
                    }
                    uint32 existentByteSize = 0u;
                    if (ret) {
                        if (functionsDatabase.Read("ByteSize", existentByteSize)) {
                            ret = functionsDatabase.Delete("ByteSize");
                        }
                    }

                    //Store the relevant signals belonging to this DataSource
                    if (!functionsDatabase.MoveRelative("Signals")) {
                        ret = functionsDatabase.CreateRelative("Signals");
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
                        if (offsetMatrixBackend != NULL_PTR(void *)) {
                            Matrix<uint32> offsetMat(offsetMatrixBackend, numberOfOffsetElements, 2u);
                            ret = functionsDatabase.Write("ByteOffset", offsetMat);
                        }
                    }
                    if (offsetMatrixBackend != NULL_PTR(void *)) {
                        delete[] offsetMatrixBackend;
                    }
                    if (ret) {
                        ret = functionsDatabase.Write("Samples", samplesBackend);
                    }

                    if (ret) {
                        ret = functionsDatabase.Write("Frequency", frequencyBackend);
                    }

                    if (ret) {
                        ret = functionsDatabase.Write("GAMMemoryOffset", allocatedByteSize);
                    }

                    if (ret) {
                        ret = functionsDatabase.MoveToAncestor(2u);
                    }

                    if (ret) {
                        existentByteSize += byteSize * samplesBackend;
                        allocatedByteSize += byteSize * samplesBackend;
                        ret = functionsDatabase.Write("ByteSize", existentByteSize);
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
                ret = functionsDatabase.Write("ByteSize", totalByteSize);
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

bool RealTimeApplicationConfigurationBuilder::AssignFunctionsMemoryToDataSource(const SignalDirection direction) {
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
        for (i = 0u; (i < numberOfFunctions) && (ret); i++) {
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
                    //uint32 gamMemoryOffset = 0u;
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
                        uint32 numberOfDataSourceFunctions = dataSourcesDatabase.GetNumberOfChildren();
                        StreamString functionId;
                        bool found = false;
                        for (n = 0u; (n < numberOfDataSourceFunctions) && (ret) && (!found); n++) {
                            functionId = dataSourcesDatabase.GetChildName(n);
                            ret = dataSourcesDatabase.MoveRelative(functionId.Buffer());
                            StreamString thisFunctionName;
                            if (ret) {
                                ret = dataSourcesDatabase.Read("QualifiedName", thisFunctionName);
                            }
                            if (ret) {
                                found = (StringHelper::Compare(thisFunctionName.Buffer(), functionName.Buffer()) == 0);
                            }
                            //Move to the next DataSource
                            if (!found) {
                                ret = dataSourcesDatabase.MoveToAncestor(1u);
                            }
                        }
                        if (ret) {
                            if (!found) {
                                StreamString newFunctionId;
                                ret = newFunctionId.Printf("%d", numberOfDataSourceFunctions);
                                if (ret) {
                                    ret = dataSourcesDatabase.CreateRelative(newFunctionId.Buffer());
                                }
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
                                ret = newSignalId.Printf("%d", (n + numberOfSignalsAlreadyInDataSource));
                                if (ret) {
                                    ret = dataSourcesDatabase.CreateRelative(newSignalId.Buffer());
                                }
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
                        ret = dataSourcesDatabase.Write("ByteSize", byteSize);
                    }
                    /*if (ret) {
                     ret = dataSourcesDatabase.Write("GAMMemoryOffset", gamMemoryOffset);
                     }*/
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

    bool ret = dataSourcesDatabase.MoveAbsolute("Data");
    uint32 numberOfDataSources = dataSourcesDatabase.GetNumberOfChildren();
    if (ret) {
        ret = (realTimeApplication != NULL);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid RealTimeApplication set");
        }
    }

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
            /*lint -e{613} NULL pointer checking done before entering here */
            dataSource = realTimeApplication->Find(fullDsPath.Buffer());
            ret = dataSource.IsValid();
        }

        //Allow for empty DataSources...
        if (dataSourcesDatabase.MoveRelative("Functions")) {

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
    }

    return ret;
}

bool RealTimeApplicationConfigurationBuilder::AssignBrokersToSignals(const SignalDirection direction,
                                                                     ReferenceT<DataSourceI> dataSource) {
    const char8 * dirStr = "InputSignals";
    if (direction == OutputSignals) {
        dirStr = "OutputSignals";
    }

    bool ret = true;
    if (dataSourcesDatabase.MoveRelative(dirStr)) {
        uint32 numberOfSignals = dataSourcesDatabase.GetNumberOfChildren();
        uint32 i;
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
                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Unsupported broker for signal %s linked to %s", signalName.Buffer(),
                                            signalName.Buffer(), dataSource->GetName())
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
                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "The DataSource %s is not valid", qualifiedName.Buffer())
                }
            }
            if (ret) {
                ret = dataSource->SetConfiguredDatabase(dataSourcesDatabase);
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
                    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "The GAM %s is not valid", qualifiedName.Buffer())
                }
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
        ret = dataSourcesDatabase.MoveToRoot();
    }
    if (ret) {
        ret = dataSourcesDatabase.Copy(dataSourcesDatabaseOut);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::Set(ConfigurationDatabase &functionsDatabaseIn,
                                                  ConfigurationDatabase &dataSourcesDatabaseIn) {
    functionsDatabase.CleanUp();
    bool ret = functionsDatabaseIn.Copy(functionsDatabase);
    if (ret) {
        dataSourcesDatabase.CleanUp();
        ret = dataSourcesDatabaseIn.Copy(dataSourcesDatabase);
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::FindSignalName(StreamString signalName,
                                                             ConfigurationDatabase &database) const {
    bool ret = true;
    bool found = false;
    uint32 numberOfSignalsInDatabase = database.GetNumberOfChildren();
    uint32 j;
    for (j = 0u; (j < numberOfSignalsInDatabase) && (ret) && (!found); j++) {
        StreamString nodeId = database.GetChildName(j);
        ret = database.MoveRelative(nodeId.Buffer());
        if (ret) {
            StreamString databaseSignalName;
            ret = database.Read("QualifiedName", databaseSignalName);
            if (ret) {
                found = (StringHelper::Compare(signalName.Buffer(), databaseSignalName.Buffer()) == 0);
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
                done = (StringHelper::Compare(qualifiedName.Buffer(), functionName.Buffer()) == 0);
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

bool RealTimeApplicationConfigurationBuilder::CheckTypeCompatibility(StreamString &fullType,
                                                                     StreamString &otherFullType,
                                                                     StreamString &signalName,
                                                                     StreamString &dataSourceSignalName) const {

    bool ret = signalName.Seek(0LLU);
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
                    if (ret) {
                        bool equalTokens = (token1 == token2);
                        bool token1Node = (token1 == "Node");
                        bool token2Node = (token2 == "Node");
                        ret = (equalTokens) || (token1Node) || (token2Node);
                    }
                }
            }
        }
        token1 = "";
        token2 = "";
    }
    return ret;
}

bool RealTimeApplicationConfigurationBuilder::SignalIntrospectionToStructuredData(ConfigurationDatabase &signalDatabase,
                                                                                  const char8 * const typeName,
                                                                                  const char8 * const signalName,
                                                                                  const char8 * const alias,
                                                                                  const char8 * const dataSourceName,
                                                                                  const char8 * const syncSignalName,
                                                                                  const char8 * const fullTypeName,
                                                                                  const AnyType & ranges,
                                                                                  const AnyType & samples,
                                                                                  const AnyType & frequency,
                                                                                  StructuredDataI & data,
                                                                                  uint32 &signalNumber,
                                                                                  bool &syncSet,
                                                                                  const bool isFunctionDatabase) {
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
        uint32 numberOfMembers = 0u;
        if (intro != NULL_PTR(Introspection *)) {
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
            StreamString fullSignalName;
            ret = fullSignalName.Printf("%s.%s", signalName, entry.GetMemberName());
            StreamString fullAliasName = "";
            if (StringHelper::Length(alias) > 0u) {
                //Recursively build the signal name by adding all the nested structure path
                if (ret) {
                    ret = fullAliasName.Printf("%s.%s", alias, entry.GetMemberName());
                }
            }
            // loop in the aliases finding the match... if found replace
            if (signalDatabase.MoveRelative("MemberAliases")) {
                StreamString memberAlias;
                if (signalDatabase.Read(fullSignalName.Buffer(), memberAlias)) {
                    fullAliasName = memberAlias;
                    typeNameStr = "";
                    if (ret) {
                        ret = memberAlias.Seek(0LLU);
                    }
                    while (memberAlias.SkipTokens(1u, ".")) {
                        typeNameStr += "Node.";
                    }
                }
                if (ret) {
                    ret = signalDatabase.MoveToAncestor(1u);
                }
            }
            typeNameStr += entry.GetMemberTypeName();

            //If the member is still structured data, continue to discombobulate
            bool isStructured = entry.GetMemberTypeDescriptor().isStructuredData;
            if (isStructured) {
                ret = SignalIntrospectionToStructuredData(signalDatabase, entry.GetMemberTypeName(), fullSignalName.Buffer(), fullAliasName.Buffer(),
                                                          dataSourceName, syncSignalName, typeNameStr.Buffer(), ranges, samples, frequency, data, signalNumber,
                                                          syncSet, isFunctionDatabase);
            }
            else {
                if (ret) {
                    //Got to the final BasicType. Add a node with the signal number as the name...
                    StreamString signalNumberStr;
                    ret = signalNumberStr.Printf("Signals.%d", signalNumber);
                    if (ret) {
                        ret = data.CreateAbsolute(signalNumberStr.Buffer());
                    }
                }
                // loop in the default values and add it to the signal if found
                if (signalDatabase.MoveRelative("Defaults")) {
                    StreamString defaultValue;
                    if (signalDatabase.Read(fullSignalName.Buffer(), defaultValue)) {
                        ret = data.Write("Default", defaultValue.Buffer());
                    }
                    if (ret) {
                        ret = signalDatabase.MoveToAncestor(1u);
                    }
                }
                //Finally got to the BasicType. Write all the properties
                if (ret) {
                    ret = data.Write("QualifiedName", fullSignalName.Buffer());
                }

                if (ret) {
                    ret = data.Write("Type", entry.GetMemberTypeName());
                }
                if (ret) {
                    ret = data.Write("NumberOfDimensions", entry.GetNumberOfDimensions());
                }
                if (ret) {
                    uint32 totalElements = 1u;
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
                    }
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

bool RealTimeApplicationConfigurationBuilder::SearchGAMs(ConfigurationDatabase &inputDatabase,
                                                         ConfigurationDatabase &outputDatabase,
                                                         StreamString & fullPath,
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
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Specified GAM %s with no input nor output", fullPath.Buffer())
            }
        }
    }
    if (isGam) {
        isGam = inputDatabase.MoveToAncestor(1u);
    }
    if (isGam) {

        if (found) {
            ret = false;
            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Unsupported nested GAMs in path %s", fullPath.Buffer())
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

uint32 RealTimeApplicationConfigurationBuilder::GetNumberOfSyncSignals(const char8 * const direction,
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
        if (ok) {
            ok = functionsDatabase.MoveToAncestor(2u);
        }
    }
    return syncCounter;
}

bool RealTimeApplicationConfigurationBuilder::SearchDataSources(ConfigurationDatabase &inputDatabase,
                                                                ConfigurationDatabase &outputDatabase,
                                                                StreamString & fullPath,
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
            ret = outputDatabase.CreateRelative("Signals");
            if (ret) {
                if (inputDatabase.MoveRelative("Signals")) {
                    ret = inputDatabase.Copy(outputDatabase);
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

}

