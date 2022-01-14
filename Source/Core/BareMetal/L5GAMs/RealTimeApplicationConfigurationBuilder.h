/**
 * @file RealTimeApplicationConfigurationBuilder.h
 * @brief Header file for class RealTimeApplicationConfigurationBuilder
 * @date 11/12/2019
 * @author Giuesppe Ferro
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
#include "DataSourceI.h"
#include "RealTimeApplication.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

class RealTimeApplication;

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
 * <pre>
 * &NAME = VALUE
 * Where VALUE can be a node {} or can be a string representing a property
 * The & can be replaced by:
 *   an empty character, i.e. NAME = VALUE, => the NAME shall exist
 *   +, i.e. NAME+ = VALUE, => the NAME may exist
 *   *, i.e. NAME* = VALUE, => a list with at least one NAME shall exist. If NAME=NUMBER then the NAME must be a number.
 *   +*, i.e. NAME+* = VALUE, the NAME may exist. If it exists the * rules apply.
 *   |, i.e NAME|NAME = VALUE, one and only one of the specified NAME shall exist.
 *   +|, i.e NAME|NAME = VALUE, the NAME may exist. If it exists the | rules apply.
 * </pre>
 */
class DLL_API RealTimeApplicationConfigurationBuilder : public Object {
public:
    /**Ignore doxygen false positive*/
    /*lint -save -e19 false positive*/
    CLASS_REGISTER_DECLARATION();
    /*lint -restore */
    /**
     * @brief Default Constructor
     */
    RealTimeApplicationConfigurationBuilder();

    /**
     * @brief Constructor for RealTimeApplicationConfigurationBuilders which should work over an already existing RealTimeApplication.
     * @param[in] realTimeApplicationIn the RealTimeApplication associated to this RealTimeApplicationConfigurationBuilder.
     * @param[in] defaultDataSourceNameIn default DataSource name to be used when the DataSource in not defined in any of the signals.
     * @post
     *   realTimeApplication = realTimeApplicationIn
     *   defaultDataSourceName = defaultDataSourceNameIn
     */
    RealTimeApplicationConfigurationBuilder(RealTimeApplication &realTimeApplicationIn,
                                          const char8 * const defaultDataSourceNameIn);

    /**
     * @brief Constructor for RealTimeApplicationConfigurationBuilders which should work over a ConfigurationDatabase (i.e. without needing to access live objects).
     * @param[in] globalDatabaseIn the ConfigurationDatabase where to load all the information required to construct the application information..
     * @param[in] defaultDataSourceNameIn default DataSource name to be used when the DataSource in not defined in any of the signals.
     * @post
     *   realTimeApplication = realTimeApplicationIn
     *   defaultDataSourceName = defaultDataSourceNameIn
     */
    RealTimeApplicationConfigurationBuilder(ConfigurationDatabase &globalDatabaseIn,
                                          const char8 * const defaultDataSourceNameIn);

    /**
     * @see ApplicationBuilderI::SetParameters
     */
    void SetParameters(RealTimeApplication &realTimeApplicationIn,const char8 * const defaultDataSourceNameIn);


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
     * <pre>
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
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
     *                 +Samples = NUMBER > 0, defines the number of (time) samples to be copied on each operation. The default value is one.
     *                 +Frequency = NUMBER>0, defines the cycle time frequency. Only and only one signal may define this property.
     *                 +Trigger = 0|1, defines if the signal should trigger the destination DataSourceI
     *                 +Default = "Default value as a string". The value to be used when the signal is not produced in a previous state.
     *                 +MemberAliases = {//Only valid for StructuredType signals
     *                    OriginalMemberName1 = NewMemberName1
     *                    ...
     *                 }
     *                 +Defaults = {//Only valid for StructuredType signals
     *                    MemberName1 = DefaultValue1
     *                    ...
     *                 }
     *               }
     *            }
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            +*NAME = {
     *              +Type = BasicType|StructuredType
     *              +NumberOfDimensions = 0|1|2
     *              +NumberOfElements = NUMBER>0
     *              +Frequency = -1|NUMBER>0, the frequency at which the signal is expected to be produced. -1 => the latest value available
     *                   (i.e. the frequency is not important), any other positive number is the desired frequency. The default is -1
     *              +Trigger = 0|1 . The default is 0
     *            }
     *          }
     *        }
     *        NUMBER = { //The DataSource to store the GAM timings is searched and added
     *           QualifiedName = "Name of the Timing DataSource as specified by the Scheduler"
     *           IsTimingDataSource = +1
     *              Signals = {
     *              }
     *           }
     *       }
     * </pre>
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
     * <pre>
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER = {
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 +FullType = BasicType (including nested path to the variable if it belongs to a structure).
     *                 +NumberOfDimensions = 0|1|2
     *                 +NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *                 +Samples = NUMBER > 0
     *                 +Frequency = NUMBER>0
     *                 +Trigger = 0|1
     *               }
     *            }
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            +*NUMBER ={
     *              QualifiedName = "QualifiedName of the signal"
     *              +FullType = BasicType (including nested path to the variable if it belongs to a structure).
     *              +NumberOfDimensions = 0|1|2
     *              +NumberOfElements = NUMBER>0
     *              +Frequency = -1|NUMBER>0
     *              +Trigger = 0|1
     *            }
     *          }
     *        }
     *        NUMBER = {
     *          QualifiedName = "Name of the Timing DataSource as specified by the Scheduler"
     *          IsTimingDataSource = +1
     *          Signals = {
     *          }
     *        }
     *      }
     * </pre>
     */
    bool FlattenSignalsDatabases();



    /**
     * @brief Declare all the states where a given Function is executed.
     * @details Add to the configuration Tree a States node holding all the states where a Function is executed.
     * @return true if for all the functions, all the states can be successfully added.
     * @pre
     *   VerifyFunctionSignals()
     * @post
     * <pre>
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER = {
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 FullType = BasicType
     *                 NumberOfDimensions = 0|1|2
     *                 NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *                 +Samples = NUMBER > 0
     *                 +Frequency = NUMBER>0
     *                 +Trigger = 0|1
     *               }
     *            }
     *         }
     *         States = {
     *           +*StateNameN = "ThreadNameN"
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            *NUMBER = {
     *              QualifiedName = "QualifiedName of the signal"
     *              FullType = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *              +Frequency = -1|NUMBER>0
     *              +Trigger = 0|1
     *            }
     *          }
     *        }
     *      }
     *      NUMBER = {
     *          QualifiedName = "Name of the Timing DataSource as specified by the Scheduler"
     *          IsTimingDataSource = +1
     *          Signals = {
     *          }
     *        }
     *      }
     * </pre>
     */
    bool ResolveStates();

    /**
     * @brief Merges all the signals from the Functions into the corresponding DataSource.
     *
     * @details For each signal (where the Type is defined) in every function, merge the signal to the corresponding DataSource.
     * If the signal does not exist in the DataSource it will be created.
     * Every signal property will be copied from the function signal definition into the DataSource. If the signal property already exists in
     * the DataSource, the two properties must be consistent.
     *
     * @details Computes the amount of memory grouped by DataSource and includes any signal specific ByteOffset information.
     *
     * @details For every signal in every DataSource set the Consumer and Producer Functions.
     *
     * @details For every signal in every Function compute the memory size and the memory offset (if Ranges are defined).
     * The memory size is given by sizeof(Type) multiplied by all the Ranges. Illegal Ranges definitions are trapped at this stage.
     *
     * @details For every DataSource adds the memory information about each Function that interacts with the DataSource
     * add the corresponding memory information.
     *
     * @details For each GAM signal, the DataSource writes the name of the BrokerI to be used.
     *
     * @return true if all the signals from the Functions can be merged into the corresponding DataSource.
     * @pre
     *   ResolveStates() or ResolveStatesFromConfiguration()
     * @post
     * <pre>
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER = {
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 Type = BasicType
     *                 NumberOfDimensions = 0|1|2
     *                 NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap) (max_idx<NumberOfElements)
     *                 ByteSize = NUMBER>0
     *                 ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ...}
     *                 +Samples = NUMBER > 0
     *                 +Frequency = NUMBER>0
     *                 +Trigger = 0|1
     *               }
     *            }
     *         }
     *         States = {
     *           +*StateNameN = "ThreadNameN"
     *         }
     *         Memory = {
     *           InputSignals|OutputSignals = {
     *             +*NUMBER = {
     *               DataSource = "QualifiedName of the DataSource"
     *               ByteSize = NUMBER>0
     *               GAMMemoryOffset = NUMBER>0
     *               Signals = {
     *                 *NUMBER = {
     *                   QualifiedName = "QualifiedName of the Signal"
     *                   +ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ... }
     *                   Frequency = -1|NUMBER>0
     *                   Trigger = 0|1
     *                   Samples = -1|NUMBER>0
     *                 }
     *               }
     *             }
     *           }
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            *NUMBER={
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              +NumberOfDimensions = 0|1|2
     *              +NumberOfElements = NUMBER>0
     *              +Frequency = -1|NUMBER>0
     *              +Trigger = 0|1
     *              +States = {
     *                *StateN = {
     *                  GAMConsumers = { "0" ... "N" }
     *                  GAMNamesConsumers = { "NAME1" ... "NAMEN" }
     *                  SignalConsumers = { "0" ... "N" }
     *                  GAMProducers = { "0" ... "N" }
     *                  GAMNamesProducers = { "NAME1" ... "NAMEN" }
     *                  SignalProducers = { "0" ... "N" }
     *                }
     *              }
     *              Functions+ = {
     *                *NUMBER = {
     *                  QualifiedName = "QualifiedName of the Function"
     *                  ByteSize = NUMBER>0
     *                  GAMMemoryOffset = NUMBER>0
     *                  Signals = {
     *                    *NUMBER = {
     *                      QualifiedName = "QualifiedName of the Signal"
     *                      +ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ... }
     *                       Frequency = -1|NUMBER>0
     *                       Trigger = 0|1
     *                       Samples = -1|NUMBER>0
     *                       Broker = "Name of the Broker returned by the DataSource"
     *                    }
     *                 }
     *               }
     *             }
     *          }
     *        }
     *     }
     * </pre>
     */
    bool ResolveDataSources();

    /**
     * @brief Verify that for every signal in every DataSource, the Type is defined.
     * @details Add the default values for NumberOfElements=+1 and NumberOfDimensions=+0 if needed. For every signal
     * in the data source, adds also its ByteSize with its size in byte of.
     * @return true if the Type is defined for all the signals in all the DataSource elements.
     * @pre
     *   ResolveDataSources()
     * @post
     * <pre>
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER = {
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 Type = BasicType
     *                 NumberOfDimensions = 0|1|2
     *                 NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap) (max_idx<NumberOfElements)
     *                 ByteSize = NUMBER>0
     *                 ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ...}
     *                 +Samples = NUMBER > 0
     *                 +Frequency = NUMBER>0
     *                 +Trigger = 0|1
     *               }
     *            }
     *         }
     *         States = {
     *           +*StateNameN = "ThreadNameN"
     *         }
     *         Memory = {
     *           InputSignals|OutputSignals = {
     *             +*NUMBER = {
     *               DataSource = "QualifiedName of the DataSource"
     *               ByteSize = NUMBER>0
     *               GAMMemoryOffset = NUMBER>0
     *               Signals = {
     *                 *NUMBER = {
     *                   QualifiedName = "QualifiedName of the Signal"
     *                   +ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ... }
     *                   Frequency = -1|NUMBER>0
     *                   Trigger = 0|1
     *                   Samples = -1|NUMBER>0
     *                 }
     *               }
     *             }
     *           }
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            *NUMBER={
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *              ByteSize = NUMBER>0
     *              +Frequency = -1|NUMBER>0
     *              +Trigger = 0|1
     *              +States = {
     *                *StateN = {
     *                  GAMConsumers = { "0" ... "N" }
     *                  GAMNamesConsumers = { "NAME1" ... "NAMEN" }
     *                  SignalConsumers = { "0" ... "N" }
     *                  GAMProducers = { "0" ... "N" }
     *                  GAMNamesProducers = { "NAME1" ... "NAMEN" }
     *                  SignalProducers = { "0" ... "N" }
     *                }
     *              }
     *              Functions+ = {
     *                *NUMBER = {
     *                  QualifiedName = "QualifiedName of the Function"
     *                  ByteSize = NUMBER>0
     *                  GAMMemoryOffset = NUMBER>0
     *                  Signals = {
     *                    *NUMBER = {
     *                      QualifiedName = "QualifiedName of the Signal"
     *                      +ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ... }
     *                       Frequency = -1|NUMBER>0
     *                       Trigger = 0|1
     *                       Samples = -1|NUMBER>0
     *                       Broker = "Name of the Broker returned by the DataSource"
     *                    }
     *                 }
     *               }
     *             }
     *          }
     *        }
     *     }
     * </pre>
     */
    bool VerifyDataSourcesSignals();


    /**
     * @brief As ResolveStates but using a ConfigurationDatabase as the data source (see ConfigureBeforeInitialision).
     * @return As in ResolveStates.
     */
    bool ResolveStatesFromConfiguration();


    /**
     * @brief Verify that there is at most one producer for each signal Range and that no no producing signals are declared in the TimingDataSource.
     * @return true if there is at most one producer for each signal Range and that no producing signals are declared in the TimingDataSource.
     */
    bool VerifyConsumersAndProducers();


    /**
     * @brief For each GAM signal, the DataSource will write the name of the BrokerI to be used.
     * @return true if the BrokerI name can be successfully retrieved from the DataSource.
     * @pre
     *   AssignFunctionsMemoryToDataSource()
     * @post
     * <pre>
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER = {
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 Type = BasicType
     *                 NumberOfDimensions = 0|1|2
     *                 NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap) (max_idx<NumberOfElements)
     *                 ByteSize = NUMBER>0
     *                 ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ...}
     *                 +Samples = NUMBER > 0
     *                 +Frequency = NUMBER>0
     *                 +Trigger = 0|1
     *               }
     *            }
     *         }
     *         States = {
     *           +*StateNameN = "ThreadNameN"
     *         }
     *         Memory = {
     *           InputSignals|OutputSignals = {
     *             +*NUMBER = {
     *               DataSource = "QualifiedName of the DataSource"
     *               ByteSize = NUMBER>0
     *               GAMMemoryOffset = NUMBER>0
     *               Signals = {
     *                 *NUMBER = {
     *                   QualifiedName = "QualifiedName of the Signal"
     *                   +ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ... }
     *                   Frequency = -1|NUMBER>0
     *                   Trigger = 0|1
     *                   Samples = -1|NUMBER>0
     *                 }
     *               }
     *             }
     *           }
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            *NUMBER={
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *              +Frequency = -1|NUMBER>0
     *              +Trigger = 0|1
     *              +States = {
     *                *StateN = {
     *                  GAMConsumers = { "0" ... "N" }
     *                  GAMNamesConsumers = { "NAME1" ... "NAMEN" }
     *                  SignalConsumers = { "0" ... "N" }
     *                  GAMProducers = { "0" ... "N" }
     *                  GAMNamesProducers = { "NAME1" ... "NAMEN" }
     *                  SignalProducers = { "0" ... "N" }
     *                }
     *              }
     *              Functions+ = {
     *                *NUMBER = {
     *                  QualifiedName = "QualifiedName of the Function"
     *                  ByteSize = NUMBER>0
     *                  GAMMemoryOffset = NUMBER>0
     *                  Signals = {
     *                    *NUMBER = {
     *                      QualifiedName = "QualifiedName of the Signal"
     *                      +ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ... }
     *                       Frequency = -1|NUMBER>0
     *                       Trigger = 0|1
     *                       Samples = -1|NUMBER>0
     *                       Broker = "Name of the Broker returned by the DataSource"
     *                    }
     *                 }
     *               }
     *             }
     *          }
     *        }
     *     }
     * </pre>
     */
    bool AssignBrokersToFunctions();

    /**
     * @brief For each DataSource calls DataSourceI::SetConfiguredDatabase followed by DataSourceI::AllocateMemory.
     * @details Calls DataSourceI::SetConfiguredDatabase on each DataSource under Data, passing the Signals{} and Functions{} branches.
     * @return true if DataSourceI::SetConfiguredDatabase returns true for all DataSources.
     */
    bool PostConfigureDataSources();

    /**
     * @brief For each GAM calls GAM::SetConfiguredDatabase
     * @details Calls calls::SetConfiguredDatabase on each GAM under Functions, passing the Functions{} branches.
     * @return true if calls::SetConfiguredDatabase returns true for all GAMs.
     */
    bool PostConfigureFunctions();

    /**
     * @brief Initialises the SignalDatabae (see InitialiseSignalsDatabase) using directly a
     *  ConfigurationDatabase as the data source.
     * @return (see InitialiseSignalsDatabase)
     */
    bool InitialiseSignalsDatabaseFromConfiguration();

    /**
     * @brief Calls RealTimeThread::ConfigureArchitecture on all the threads.
     * @return true if RealTimeThread::ConfigureArchitecture returns true on all RealTimeThread elements.
     */
    bool ConfigureThreads() const;

    /**
     * @brief Compiles all the information required to build a RealTimeApplication after the
     * ObjectRegistryDatabase::Initialise has been successfully called (i.e. it requires access to the live objects).
     * @return true if the following functions, called in order, return true:
     *   InitialiseSignalsDatabase(), FlattenSignalsDatabases(), ResolveDataSources(),
     * VerifyDataSourcesSignals(), ResolveFunctionSignals(), VerifyFunctionSignals(), ResolveStates(),
     * ResolveConsumersAndProducers(), VerifyConsumersAndProducers(), ResolveFunctionSignalsMemorySize(), ResolveFunctionsMemory(),
     * AssignFunctionsMemoryToDataSource)() and AssignBrokersToFunctions()
     * @post
     *   The functions PostConfigureDataSources() and PostConfigureFunctions() can now be called.
     */
    bool ConfigureAfterInitialisation();

    /**
     * @brief Compiles all the information required to build a RealTimeApplication without
     * requiring the ObjectRegistryDatabase::Initialise to be called to load all the Objects (i.e. it works only
     *  with configuration data).
     * @details This allows to generate compiled databases with Function and DataSource databases
     * that can be then exported to be later executed in e.g. another system (i.e. these can be directly fed to a
     * ObjectRegistryDatabase::Initialise without needing to execute a builder). (See Copy)
     * @return true if the following functions, called in order, return true:
     *   InitialiseSignalsDatabaseFromConfiguration(), FlattenSignalsDatabases(), ResolveDataSources(),
     * VerifyDataSourcesSignals(), ResolveFunctionSignals(), VerifyFunctionSignals(), ResolveStatesFromConfiguration(),
     * ResolveConsumersAndProducers(), VerifyConsumersAndProducers(), ResolveFunctionSignalsMemorySize(), ResolveFunctionsMemory()
     * and AssignFunctionsMemoryToDataSource();
     * @post
     *   The ObjectRegistryDatabase can now be initialised (typically this is to be performed in another application, see Set).
     *   The functions PostConfigureDataSources() and PostConfigureFunctions() can then be called.
     */
    bool ConfigureBeforeInitialisation();

    /**
     * @brief Copies the Function and DataSource databases.
     * @param[out] functionsDatabaseOut where to copy the Functions database into.
     * @param[out] dataSourcesDatabaseOut where to copy the DataSource database into.
     * @return true if both ConfigurationDatabase::Copy are successful.
     */
    bool Copy(ConfigurationDatabase &functionsDatabaseOut,
              ConfigurationDatabase &dataSourcesDatabaseOut);

    /**
     * @brief Sets the Functions and Data databases.
     * @param[in] functionsDatabaseIn where to copy the Functions database from.
     * @param[in] dataSourcesDatabaseIn where to copy the DataSource database from.
     * @return true if both ConfigurationDatabase::Copy are successful.
     */
    bool Set(ConfigurationDatabase &functionsDatabaseIn,
             ConfigurationDatabase &dataSourcesDatabaseIn);

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
     * ConfigurationDatabase that was already pre-initialised.
     */
    ConfigurationDatabase globalDatabase;

    /**
     * Allows to speed-up access to the databases
     */
    ConfigurationDatabase cachedIntrospections;

    /**
     * Allows to speed-up access to the databases
     */
    ConfigurationDatabase cachedDatabase;

    /**
     * Allows to cache the DataSource indexes
     */
    ConfigurationDatabase dataSourcesIndexesCache;

    /**
     * Allows to cache the Functions indexes
     */
    ConfigurationDatabase functionsIndexesCache;

    /**
     * Cache the datasource signal index
     */
    ConfigurationDatabase dataSourcesSignalIndexCache;

    /**
     * Cache the datasource function index
     */
    ConfigurationDatabase dataSourcesFunctionIndexesCache;

    /**
     * Allows to cache the Functions Memirt indexes
     */
    ConfigurationDatabase functionsMemoryIndexesCache;

    /**
     * The default DataSource name to be used if this is not defined in any of the signals.
     */
    StreamString defaultDataSourceName;

    /**
     * The RealTimeApplication associated to this RealTimeApplicationConfigurationBuilder.
     */
    RealTimeApplication *realTimeApplication;

    /**
     * @brief Flattens all the nested structures in a Database.
     * @details All the introspection elements are expanded and the signals are flatten, so that nested structured
     * signals are stored with a QualifiedName which includes the full path to the signal.
     *
     * For each signal a node with a consecutive number will be created and the fully qualified signal name (with its properties added to it).
     * The original signal definition will then be deleted.
     *
     * @param[in] signalDatabase the database to flatten.
     * @param[in] direction if signalDatabase = functionsDatabase this can be either InputSignals or OutputSignals,
     * otherwise it shall have the value None.
     * @return true if FlattenSignal returns true for all signals in the functionsDatabase and in the dataSourcesDatabase tree.
     */
    bool FlattenSignalsDatabase(ConfigurationDatabase &signalDatabase,
                                const SignalDirection direction);

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
    bool FlattenSignal(const bool isFunctionsDatabase,
                       const char8 * const signalName,
                       ConfigurationDatabase &resolvedSignal,
                       uint32 &signalNumber);

    /**
     * @brief Adds a signal from \a gamName into \a dataSourceName.
     * @details It assumes that the functionsDatabase and the dataSourcesDatabase are pointing at the correct signal already. If the signal
     *  already exits in the DataSource the definitions of the signal in the GAM and the one already existent in the DataSource will be merged.
     *  If incompatibilities are found an error will be returned.
     * @param[in] functionName name of the GAM where the signal is going to be copied from.
     * @param[in] dataSourceName name of the DataSource where the signal is going to be copied to.
     * @return true if the signal from the GAM \a gamName can be successfully added to the DataSource with name \a dataSourceName. It will
     *  return false if the signal already exists in the DataSource and incompatibilities between definitions are found.
     */
    bool AddSignalToDataSource(StreamString functionName,
                               StreamString dataSourceName);

    /**
     * @brief Merge signals from the Functions to the corresponding DataSources.
     * @details For each signal (where the Type is defined!) in each function call AddSignalToDataSource.
     * @param[in] direction can be either InputSignals or OutputSignals
     * @return true if all the calls to AddSignalToDataSource are successful.
     */

    /**
     * @brief @see ResolveFunctionSignals()
     * @param[in] direction can be either InputSignals or OutputSignals.
     * @return @see ResolveFunctionSignals()
     */

    /**
     * @brief Final merge of signals from the DataSources to the Functions.
     * @details For every signal in every Function merge with the corresponding DataSource signal.
     * If the signal type was not originally defined in the Function it will get defined by the DataSource.
     * If incompatibilities are found in the signal definition in the DataSource or in the Function an error will be returned.
     * @param[in] direction can be either InputSignals or OutputSignals
     * @return true if all the signals can be successfully merged.
     */

    /**
     * @brief @see ResolveConsumersAndProducers()
     * @param[in] consumers true if the function is being called to add Consumers, false if the function is being called to add Producers.
     * @return @see ResolveConsumersAndProducers()
     */
    bool ResolveConsumersAndProducers(const bool consumers,
                                      Vector<StreamString> &states,
                                      const char8 * const functionId,
                                      const char8 * const functionName,
                                      const char8 * const signalId);

    /**
     * @brief Allows to distribute the same Producer across different ranges, i.e. the same Signal might have more than one
     *  producer, provided that the Ranges do not overlap.
     * @return true if for the same producing signal the Ranges do not overlap;
     */
    bool BuildProducersRanges();

    /**
     * @brief Verifies if for the same signal the Ranges do not overlap.
     * @return true if for the same producing signal the Ranges do not overlap.
     */
    bool CheckProducersRanges(const uint32 * const rangesArray,
                              const uint32 numberOfElements) const;
    /**
     * @brief @see ResolveFunctionSignalsMemorySize()
     * @param[in] direction can be either InputSignals or OutputSignals
     * @return @see ResolveFunctionSignalsMemorySize()
     */
    bool ResolveFunctionSignalsMemorySize(const char8 * const functionName);

    /**
     * @brief @see ResolveFunctionsMemory()
     * @param[in] direction can be either InputSignals or OutputSignals
     * @return @see ResolveFunctionsMemory()
     */
    bool ResolveFunctionsMemory(const SignalDirection direction,
                                const ConfigurationDatabase &functionsDatabaseAfterMove,
                                uint32 &totalByteSize,
                                uint32 &allocatedByteSize,
                                const char8 * const functionName);

    /**
     * @brief @see AssignFunctionsMemoryToDataSource()
     * @param[in] direction can be either InputSignals or OutputSignals
     * @return @see AssignFunctionsMemoryToDataSource()
     */
    bool AssignFunctionsMemoryToDataSource(const SignalDirection direction, const char8 * const functionName);

    /**
     * @brief @see AssignBrokersToSignals()
     * @param[in] direction can be either InputSignals or OutputSignals
     * @param[in] dataSource the DataSourceI to be queried.
     * @return @see AssignBrokersToSignals()
     */
    bool AssignBrokersToSignals(const SignalDirection direction,
                                ReferenceT<DataSourceI> dataSource);

    /**
     * @brief Find the unique number associated to the DataSource with name = \a dataSourceName.
     * @param[in] dataSourceName the fully qualified name of the DataSource to search.
     * @param[out] dataSourceNumber the number associated to the DataSource with name = \a dataSourceName.
     * @return true iff a DataSource with name = \a dataSourceName exists.
     */
    bool FindDataSourceNumber(StreamString dataSourceName,
                              StreamString &dataSourceNumber);
    /**
     * @brief Find the unique number associated to the Function with name = \a functionName.
     * @param[in] functionName the fully qualified name of the Function to search.
     * @param[out] functionNumber the number associated to the Function with name = \a functionName.
     * @return true iff a Function with name = \a functionName exists.
     */
    bool FindFunctionNumber(StreamString functionName,
                            StreamString &functionNumber);

    /**
     * @brief Check that two types are compatible. Notice that the two types can belong to different structures  and
     *  they will be compatible iff the full structure path is equivalent.
     * @param[in] fullType the first type being checked.
     * @param[in] otherFullType the other type being checked.
     * @param[in] signalName the name of the GAM signal being checked.
     * @param[in] dataSourceSignalName the name of the DataSourceI signal being checked.
     */
    bool CheckTypeCompatibility(StreamString &fullType,
                                StreamString &otherFullType,
                                StreamString &signalName,
                                StreamString &dataSourceSignalName) const;

    /**
     * @brief Store in a ConfigurationDatabase (cachedIntrospections) the structured information (name, type, ...) of all the signals.
     * registered in the ClassRegistryDatabase. This information is then use to optimise the SignalIntrospectionToStructuredData
     */
    bool CacheAllSignalsIntrospections();

    /**
     * @brief Called by the CacheAllSignalsIntrospections for every type that is registered in the ClassRegistryDatabase.
     * @details This is a recursive method.
     * @param[in] typeName the type to register (it must be registered in the ClassRegistryDatabase)
     * @param[in] signalName the parent structure member name (including path to the root)
     * @param[in] fullTypeName the parent structure type name (including path to the root)
     * @param[out] data see SignalIntrospectionToStructuredData
     * @param[in] signalNumber incremented for each signal that is written to \a data
     * @return true if all the signal properties are written into \a data.
     */
    bool CacheSignalIntrospections(const char8 * const typeName, const char8 * const signalName,
                                   const char8 * const fullTypeName,
                                   ConfigurationDatabase & data,
                                   uint32 &signalNumber);

    /**
     * @brief Expands a structure into a flat database where each member of the structure is stored as an entry with a name in
     * the form x.y.z, where x is structure y, and y is a structure that contains the basic signal z.
     * @brief[in] typeName the signal type.
     * @brief[in] signalName the signal name.
     * @brief[in] dataSourceName the DataSourceI object name.
     * @brief[in] syncSignalName used in recursion. The name of the signal that provides synchronisation for the structured signal.
     * @brief[in] triggerSignalName used in recursion. The name of the signal that provides trigger for the structured signal.
     * @brief[in] fullTypeName used in recursion. The typeName including in path all the types leading to the signal.
     * @brief[in] ranges used in recursion. The signal ranges that was set for the structured signal (every member inherits this property).
     * @brief[in] samples used in recursion. The signal number of samples that was set for the structured signal (every member inherits this property).
     * @brief[in] frequency used in recursion. The signal frequency that was set for the structured signal.
     * @brief[in] trigger used in recursion. Value != Empty if the trigger was set.
     * @brief[out] data where to write the configuration information.
     * @brief[out] signalNumber used in recursion. The current signal index, w.r.t. to the number of signals declared for this structure.
     * @brief[out] syncSet used in recursion. True if a synchronising signal was already set.
     * @brief[out] triggerSet used in recursion. True if a triggering signal was already set.
     * @brief[out] isFunctionDatabase true if the function is being called in the context of the Functions ConfigurationDatabase.
     * @return true if all the structure signals can be successfully flattened.
     */
    bool SignalIntrospectionToStructuredData(ConfigurationDatabase &signalDatabase,
                                             const char8 * const typeName,
                                             const char8 * const signalName,
                                             const char8 * const alias,
                                             const char8 * const dataSourceName,
                                             const char8 * const syncSignalName,
                                             const char8 * const triggerSignalName,
                                             const char8 * const fullTypeName,
                                             const AnyType & ranges,
                                             const AnyType & samples,
                                             const AnyType & frequency,
                                             const AnyType & trigger,
                                             ConfigurationDatabase & data,
                                             uint32 &signalNumber,
                                             bool &syncSet,
                                             bool &triggerSet,
                                             const bool isFunctionDatabase);

    /**
     * @brief Search for all the declared GAM classes without using the GlobalObjectRegistryDatabase.
     * @param[in] inputDatabase where to look for the GAMs.
     * @param[out] inputDatabase where to write the information related to each GAM that is found.
     * @param[in] fullPath used in recursion, contains the full path from the root of the \a inputDatabase to the current GAM.
     * @param[in] index used in recursion, incremented for each GAM that is found.
     * @param[in] bool used in recursion to detect for nested GAMs, i.e. GAMs contained inside GAMs.
     * @return true if all the GAMs declared in \a inputDatabase are found.
     */
    bool SearchGAMs(ConfigurationDatabase &inputDatabase,
                    ConfigurationDatabase &outputDatabase,
                    StreamString & fullPath,
                    uint32 &index,
                    bool found);

    /**
     * @brief Finds the number of declared synchronous signals (i.e. where Frequency >= 0).
     * @param[in] direction the signal direction.
     * @param[ok] ok true if the Number of synchronous signals could be successfully queried.
     * @return the number of declared synchronous signals (i.e. where Frequency >= 0).
     */
    uint32 GetNumberOfSyncSignals(const char8 * const direction,
                                  bool &ok);

    /**
     * @brief Search for all the declared DataSourceI classes without using the GlobalObjectRegistryDatabase.
     * @param[in] inputDatabase where to look for the DataSources.
     * @param[out] inputDatabase where to write the information related to each DataSources that is found.
     * @param[in] fullPath used in recursion, contains the full path from the root of the \a inputDatabase to the current DataSources.
     * @param[in] index used in recursion, incremented for each DataSources that is found.
     * @param[in] timingDataSourceCounter used in recursion, the number of TimingDataSource objects found.
     * @return true if all the GAMs declared in \a inputDatabase are found.
     */
    bool SearchDataSources(ConfigurationDatabase &inputDatabase,
                           ConfigurationDatabase &outputDatabase,
                           StreamString & fullPath,
                           uint32 &index,
                           uint32 &timingDataSourceCounter);

    /**
     * @brief Adds the \a threadName and the \a stateName to the \a functionName and writes to \a local  ConfigurationDatabase.
     * @param[in] functionName the Function name.
     * @param[in] stateName the state name.
     * @param[in] threadName the thread name.
     * @param[out] syncSignals the number of Signals declared with Frequency >= 0.0.
     * @return true if all the states can be successfully assigned to \a functionName.
     */
    bool AddStateToFunction(ConfigurationDatabase &local,
                            const char8 * const functionName,
                            const char8 * const stateName,
                            const char8 * const threadName,
                            uint32 &syncSignals);
    /**
     * @brief Adds the \a threadName and the \a stateName to the \a gamName and writes to \a local  ConfigurationDatabase.
     * @param[in] gamNameIn the GAM name.
     * @param[in] stateName the state name.
     * @param[in] threadName the thread name.
     * @param[out] syncSignals the number of Signals declared with Frequency >= 0.0.
     * @return true if all the states can be successfully assigned to \a functionName.
     */
    bool AddStateToGAM(const char8 * const gamNameIn,
                       const char8 * const stateName,
                       const char8 * const threadName,
                       uint32 &syncSignals,
                       bool checkSameGamInMoreThreads = true);

    /**
     * @brief Adds to the TimingDataSource all the GAM timing signals (x__ReadTime, x_ExecTime and, x_WriteTime where x is the GAM name).
     * @return true if all the timing signals can be successfully added.
     */
    bool AddTimingSignals();

    /**
     * @brief Adds to the TimingDataSource all the RealTimeThread cycle time signals (x_CycleTime where x is the RealTimeThread name).
     * @return true if all the RealTimeThread cycle time signals can be successfully added.
     */
    bool AddThreadCycleTime(const char8 * const threadFullName);

    /**
     * @brief Writes all the properties related to the TimingDataSource signals.
     * @param[in] signalName the signal name to be updated.
     * @return true if the signal properties could be successfully added to all the signals.
     */
    bool WriteTimeSignalInfo(const char8 * const signalName);

    /**
     * @brief Verifies if all the TimingDataSource signals have the correct properties (type = uint32, NumberOfElements = 1).
     * @return true if all the TimingDataSource signals have the correct properties.
     */
    bool CheckTimeSignalInfo();

    /**
     * Clear the caches,
     */
    void CleanCaches();

    /*lint -e{1712} this class does not have a default constructor because either
     * the ConfigurationDatabase or the RealTimeApplication must be defined in construction.
     */

    /**
     * Specifies if the initialisation must be executed after or before the initialisation
     * of the MARTe environment
     */
    bool initialiseAfterInitialisation;

    /**
     * @brief Writes the default value to the target StructuredDataI (sdi)
     * @param[out] sdi the StructuredDataI where to write the Default value
     * @param[in] signalName the name of the signal containing the default value 
     * @param[in] dataSourceName the name of the DataSource to which this signal is connected
     * @param[in] signalTypeDescriptor the signal type
     * @param[in] numberOfDimensions the signal number of dimensions
     * @param[in] numberOfElements the signal number of elements
     * @param[in] numberOfBytes the signal number of bytes
     * @return true if the signal and its default value are compatible in terms of type and dimensions, and if the signal default value can be successfully written to the output StructuredDataI.
     */
    bool WriteDefault(StructuredDataI &sdi, const char8 * const signalName, const char8 * const dataSourceName, const TypeDescriptor &signalTypeDescriptor, uint32 numberOfDimensions, uint32 numberOfElements, uint32 signalNumberOfBytes) const;
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEAPPLICATIONCONFIGURATIONBUILDER_H_ */

