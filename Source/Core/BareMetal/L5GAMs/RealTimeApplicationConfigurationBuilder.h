/**
 * @file RealTimeApplicationConfigurationBuilder.h
 * @brief Header file for class RealTimeApplicationConfigurationBuilder
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

 * @details This header file contains the declaration of the class RealTimeApplicationConfigurationBuilder
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEAPPLICATIONCONFIGURATIONBUILDER_H_
#define REALTIMEAPPLICATIONCONFIGURATIONBUILDER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Helper class which expands and parses a RealTimeApplication configuration database.
 * @details This class takes a RealTimeApplication ConfigurationDatabase as defined by the
 * application user and expands into a fully defined and consistent ConfigurationDatabase that
 * can be used by the MARTe L5GAMs classes to build an application.
 *
 * In particular this class takes cares of: expanding all the nested introspection in both the
 * GAMs (i.e. Functions) and DataSources; completing the missing details in any of the signals
 * and verifying if all the GAM and DataSource signals definition is consistent.
 *
 * The expanded information is stored in two ConfigurationDatabase instances. One for the Functions and
 * another for the DataSources.
 *
 * The schema used below to define the pre and post condition is
 * NAME& = VALUE
 * Where VALUE can be a node {} or can be a string representing a property
 * The & can be replaced by:
 *   an empty character, i.e. NAME = VALUE, => the NAME shall exist
 *   +, i.e. NAME+ = VALUE, => the NAME may exist
 *   *, i.e. NAME* = VALUE, => a list with at least one NAME shall exist. If NAME=NUMBER then the NAME must be a number.
 *   +*, i.e. NAME+* = VALUE, the NAME may exist. If it exists the * rules apply.
 *   |, i.e NAME|NAME = VALUE, one and only one of the specified NAME shall exist.
 *   +|, i.e NAME|NAME = VALUE, the NAME may exist. If it exists the | rules apply.
 */
class DLL_API RealTimeApplicationConfigurationBuilder {
public:

    /**
     * @brief Default constructor. Sets the RealTimeApplication
     * @param[in] realTimeApplicationIn the RealTimeApplication associated to this RealTimeApplicationConfigurationBuilder.
     * @param[in] defaultDataSourceNameIn default DataSource name to be used when the DataSource in not defined in any of the signals.
     * @post
     *   realTimeApplication = realTimeApplicationIn
     *   defaultDataSourceName = defaultDataSourceNameIn
     */
    RealTimeApplicationConfigurationBuilder(ReferenceT<ReferenceContainer> realTimeApplicationIn,
                                            const char8 *defaultDataSourceNameIn);

    /**
     * @brief Adds all the GAM signals to the Functions database and adds all the DataSource signals to the DataSource database.
     * @details Looks for all the GAM instances and calls GAM::AddSignals. The GAM name is stored as a qualified name (i.e. each recursion into
     * the configuration tree is separated by a point and added to the GAM name). The same action is performed for each DataSource instance.
     *
     * Each GAM/DataSource that is added, is stored under the Functions/Data nodes and identified with an unique number.
     * @return true if all the GAM::AddSignals and all the DataSource::AddSignals were successful.
     * @pre
     *   functionsDatabase = ""
     *   dataSourcesDatabase = ""
     * @post
     *   functionsDatabase =
     *     Functions = {
     *       NUMBER* = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               NAME*={
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 +DataSource = "QualifiedName of the DataSource"
     *                 +Type = BasicType|StructuredType
     *                 +NumberOfDimensions = 0|1|2
     *                 +NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *               }
     *            }
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        NUMBER* = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            NAME+*={
     *              +Type = BasicType|StructuredType
     *              +NumberOfDimensions = 0|1|2
     *              +NumberOfElements = NUMBER>0
     *            }
     *          }
     *        }
     *      }
     */
    bool InitialiseSignalsDatabase();

    /**
     * @brief Flattens the Functions and DataSource databases.
     * @details All the introspection elements are expanded and the signals are flatten, so that nested structured
     * signals are stored with a QualifiedName which includes the full path to the signal. After this stage the
     * DataSource for all the Function signals must be known. If it not specified the defaultDataSource is assumed.
     * @return true if all the signals can be successfully flatten and if the DataSource is known for all the GAM signals.
     * @pre
     *   InitialiseSignalsDatabase()
     * @post
     *   functionsDatabase =
     *     Functions = {
     *       NUMBER* = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               NUMBER*={
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 +Type = BasicType
     *                 +NumberOfDimensions = 0|1|2
     *                 +NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *               }
     *            }
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        NUMBER* = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            NUMBER+*={
     *              +Type = BasicType
     *              +NumberOfDimensions = 0|1|2
     *              +NumberOfElements = NUMBER>0
     *            }
     *          }
     *        }
     *      }
     */
    bool FlattenSignalsDatabases();

    /**
     * @brief Copies the Function and DataSource databases.
     * @param[out] functionsDatabaseOut where to copy the Functions database into.
     * @param[out] dataSourcesDatabaseOut where to copy the DataSource database into.
     * @return true if both ConfigurationDatabase::Copy are successful.
     */
    bool Copy(ConfigurationDatabase &functionsDatabaseOut,
              ConfigurationDatabase &dataSourcesDatabaseOut);

private:
    /**
     * ConfigurationDatabase with all the Functions information.
     */
    ConfigurationDatabase functionsDatabase;

    /**
     * ConfigurationDatabase with all the DataSource information.
     */
    ConfigurationDatabase dataSourcesDatabase;

    /**
     * ConfigurationDatabase realTimeApplicationConfigurationIn
     */

    /**
     * The default DataSource name to be used if this is not defined in any of the signals.
     */
    StreamString defaultDataSourceName;

    /**
     * The RealTimeApplication associated to this RealTimeApplicationConfigurationBuilder.
     */
    ReferenceT<ReferenceContainer> realTimeApplication;

    /**
     * @brief Flattens all the nested structures in a Database.
     * @details All the introspection elements are expanded and the signals are flatten, so that nested structured
     * signals are stored with a QualifiedName which includes the full path to the signal.
     *
     * For each signal a node with a consecutive number will be created and the fully qualified signal name (with its properties added to it).
     * The original signal definition will then be deleted.
     *
     * @param[in] signalDatabase the database to flatten.
     * @param[in] signalDirection if signalDatabase = functionsDatabase this can be either "InputSignals" or "OutputSignals",
     * otherwise it shall have the value "Signals".
     * @return true if FlattenSignal returns true for all signals in the functionsDatabase and in the dataSourcesDatabase tree.
     */
    bool FlattenSignalsDatabase(ConfigurationDatabase &signalDatabase,
                                const char8 * const signalDirection);

    /**
     * @brief Flattens a signal from the \a signalDatabase and stores it in the \a resolvedSignal database.
     * @details This function flattens the signal with name \a signalName. If the signal is part of a nested node (i.e. a signal
     * with a namespace), this recursively calls itself until it founds a signal which is final. If the signal is a registered type
     * then this function will call SignalIntrospectionToStructuredData which is recursive.
     *
     * @param[in] signalDatabase the database where to read the signal from (it must be pointing at the correct entry signal).
     * @param[in] signalName the name of the signal to be flatten.
     * @param[out] resolvedSignal the flatten signal will be written in this database.
     * @param[out] signalNumber this value is incremented for each signal that is added.
     * @param[in] forceWriteDataSource true if this signal belongs to a GAM. In this case and at this point the DataSource must be known.
     * If it is not known and if defaultDataSourceName exists then DataSource will be assumed to be defaultDataSourceName.
     * @return true if \a signalName can be successfully flatten and all of its members added to a new signal node in \a resolvedSignal.
     */
    bool FlattenSignal(ConfigurationDatabase &signalDatabase,
                       const char8 * const signalName,
                       ConfigurationDatabase &resolvedSignal,
                       uint32 &signalNumber,
                       bool forceWriteDataSource);

    /**
     * @brief Recursively flattens a nested structure into a list of signals.
     * @details For each signal a node with a consecutive number will be created and the
     * fully qualified signal name (with its properties added to it).
     * @param[in] typeName the type of the signal as registered in the ClassRegistryDatabase
     * @param[in] signalName the signal name that will prefix the fully qualified signal name
     * @param[in] alias where can this signal be found in the DataSource. If this parameter is defined, the qualified alias will be prefixed by this value.
     * @param[in] dataSourceName the qualified name of the DataSource where this signal can be found.
     * @param[in] ranges if this parameter is defined, the same ranges will be applied to all the nested signals.
     * @param[in] data target database where to write the flatten nested signals.
     * @param[out] signalNumber is incremented every time a new signal is added to \a data.
     * @return true if the typeName exists in the ClassRegistryDatabase and if all the signal properties can be successfully written to \a data.
     */
    bool SignalIntrospectionToStructuredData(const char8 * const typeName,
                                             const char8 * const signalName,
                                             const char8 * const alias,
                                             const char8 * const dataSourceName,
                                             AnyType ranges,
                                             StructuredDataI & data,
                                             uint32 &signalNumber);
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEAPPLICATIONCONFIGURATIONBUILDER_H_ */

