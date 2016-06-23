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
#include "RealTimeApplicationConfigurationBuilder.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

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
        uint32 i = 0;
        for (i = 0; (i < numberOfGAMs) && (ret); i++) {
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
                                                                     const char8 * const signalDirection) {
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
    bool signalTypeDefined = false;

    StreamString signalType;
    signalTypeDefined = signalDatabase.Read("Type", signalType);
    if (signalTypeDefined) {
        signalTypeDefined = (signalType.Size() > 0u);
    }
    //Check if it is a pure node (i.e. a nested namespace which contains other signals)
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
                signalFullName.Printf("%s.%s", signalName, elementName.Buffer());
                ret = FlattenSignal(signalDatabase, signalFullName.Buffer(), resolvedSignal, signalNumber, forceWriteDataSource);
            }
        }
    }
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
                )
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
                ret = SignalIntrospectionToStructuredData(signalType.Buffer(), signalName, alias.Buffer(), dataSourceName.Buffer(), ranges, resolvedSignal,
                                                          signalNumber);
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
        //Loop and copy all known properties at this time.
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

bool RealTimeApplicationConfigurationBuilder::SignalIntrospectionToStructuredData(const char8 * const typeName,
                                                                                  const char8 * const signalName,
                                                                                  const char8 * const alias,
                                                                                  const char8 * const dataSourceName,
                                                                                  AnyType ranges,
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
        if (alias != NULL_PTR(const char8 *)) {
            if (StringHelper::Length(alias) > 0u) {
                //Recursively build the signal name by adding all the nested structure path
                fullAliasName.Printf("%s.%s", alias, entry.GetMemberName());
            }
        }
        //If the member is still structured data, continue to discombobulate
        if (entry.GetMemberTypeDescriptor().isStructuredData) {
            ret = SignalIntrospectionToStructuredData(entry.GetMemberTypeName(), fullSignalName.Buffer(), fullAliasName.Buffer(), dataSourceName, ranges, data,
                                                      signalNumber);
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
            if (ret) {
                if (dataSourceName != NULL_PTR(const char8 *)) {
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

}

