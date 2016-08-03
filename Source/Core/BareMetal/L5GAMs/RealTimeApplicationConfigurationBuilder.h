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
#include "DataSourceI.h"
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
 * &NAME = VALUE
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

    bool ConfigureAfterInitialisation();

    bool ConfigureBeforeInitialisation();

    /**
     * @brief Default constructor. Sets the RealTimeApplication
     * @param[in] realTimeApplicationIn the RealTimeApplication associated to this RealTimeApplicationConfigurationBuilder.
     * @param[in] defaultDataSourceNameIn default DataSource name to be used when the DataSource in not defined in any of the signals.
     * @post
     *   realTimeApplication = realTimeApplicationIn
     *   defaultDataSourceName = defaultDataSourceNameIn
     */
    RealTimeApplicationConfigurationBuilder(RealTimeApplication &realTimeApplicationIn,
                                            const char8 * const defaultDataSourceNameIn);

    RealTimeApplicationConfigurationBuilder(ConfigurationDatabase &globalDatabaseIn,
                                            const char8 * const defaultDataSourceNameIn);

    bool InitialiseSignalsDatabaseFromConfiguration();

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
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER = {
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 +Type = BasicType
     *                 +NumberOfDimensions = 0|1|2
     *                 +NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *                 +Samples = NUMBER > 0
     *                 +Frequency = NUMBER>0
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
     *              +Type = BasicType
     *              +NumberOfDimensions = 0|1|2
     *              +NumberOfElements = NUMBER>0
     *              +Frequency = -1|NUMBER>0
     *            }
     *          }
     *        }
     *      }
     */
    bool FlattenSignalsDatabases();

    /**
     * @brief Merges all the signals from the Functions into the corresponding DataSource.
     * @details For each signal (where the Type is defined) in every function, merge the signal to the corresponding DataSource.
     * If the signal does not exist in the DataSource it will be created.
     * Every signal property will be copied from the function signal definition into the DataSource. If the signal property already exists in
     *  the DataSource, the two properties must be consistent.
     * @return true if all the signals from the Functions can be merged into the corresponding DataSource.
     * @pre
     *   FlattenSignalsDatabases()
     * @post
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
     *                 +Type = BasicType
     *                 +NumberOfDimensions = 0|1|2
     *                 +NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *                 +Samples = NUMBER > 0
     *                 +Frequency = NUMBER>0
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
     *            *NUMBER = {
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              +NumberOfDimensions = 0|1|2
     *              +NumberOfElements = NUMBER>0
     *              +Frequency = -1|NUMBER>0
     *            }
     *          }
     *        }
     *      }
     */
    bool ResolveDataSources();

    /**
     * @brief Verify that for every signal in every DataSource, the Type is defined.
     * @details Add the default values for NumberOfElements=+1 and NumberOfDimensions=+0 if needed.
     * @return true if the Type is defined for all the signals in all the DataSource elements.
     * @pre
     *   ResolveDataSources()
     * @post
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER ={
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 +Type = BasicType
     *                 +NumberOfDimensions = 0|1|2
     *                 +NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *                 +Samples = NUMBER > 0
     *                 +Frequency = NUMBER>0
     *               }
     *            }
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *       *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            *NUMBER = {
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *              ByteSize = NUMBER > 0
     *              +Frequency = -1|NUMBER>0
     *            }
     *          }
     *        }
     *      }
     */
    bool VerifyDataSourcesSignals();

    /**
     * @brief Final merge of signals from the DataSources to the Functions.
     * @details For every signal in every Function merges it with the corresponding DataSource signal.
     * If the signal type was not originally defined in the Function it will get defined by the DataSource.
     * If incompatibilities are found in the signal definition in the DataSource or in the Function an error will be returned.
     * If the signal name is a subset of a nested structure (e.g. A.B in a structure that has A.B.C and A.B.D defined) all the subset will be copied (both
     * A.B.C and A.B.D in the previous example).
     * @return true if all the signals can be successfully merged.
     * @pre
     *   VerifyDataSourcesSignals()
     * @post
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
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *                 +Samples = NUMBER > 0
     *                 +Frequency = NUMBER>0
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
     *            *NUMBER = {
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *              +Frequency = -1|NUMBER>0
     *            }
     *          }
     *        }
     *      }
     */
    bool ResolveFunctionSignals();

    /**
     * @brief Verifies that for every signal in every function all the properties are defined.
     * @return true if the Type, the NumberOfDimensions and the NumberOfElements are defined for every signal in every Function.
     * @pre
     *   ResolveFunctionSignals()
     * @post
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
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *                 +Samples = NUMBER > 0
     *                 +Frequency = NUMBER>0
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
     *            *NUMBER = {
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *              +Frequency = -1|NUMBER>0
     *            }
     *          }
     *        }
     *      }
     */
    bool VerifyFunctionSignals();

    /**
     * @brief Declare all the states where a given Function is executed.
     * @details Add to the configuration Tree a States node holding all the states where a Function is executed.
     * @return true if for all the functions, all the states can be successfully added.
     * @pre
     *   VerifyFunctionSignals()
     * @post
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
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *                 +Samples = NUMBER > 0
     *                 +Frequency = NUMBER>0
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
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *              +Frequency = -1|NUMBER>0
     *            }
     *          }
     *        }
     *      }
     */
    //TODO Add the cycle times to the time data source
    bool ResolveStates();

    bool ResolveStatesFromConfiguration();

    /**
     * @brief Checks that every GAM has been called in at least one thread.
     * @return true if every GAM has been called in at least one thread.
     */
    bool VerifyStates();

    /**
     * @brief For every signal in every DataSource set the Consumer and Producer Functions.
     * @return true if for every signal in every DataSource the Consumer and Producer Functions can be successfully added.
     * @pre
     *   ResolveStates()
     * @post
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *              *NUMBER = {
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 Type = BasicType
     *                 NumberOfDimensions = 0|1|2
     *                 NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap) (max_idx<NumberOfElements)
     *                 +Samples = NUMBER > 0
     *                 +Frequency = NUMBER>0
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
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *              +Frequency = -1|NUMBER>0
     *              +States = {
     *                *StateN = {
     *                  GAMConsumers = { "0" ... "N" }, where 0 ... N are the indices of the Function as defined in the Functions node
     *                  GAMNamesConsumers = { "NAME1" ... "NAMEN" }, where NAME1...NAMEN are the names of the functions
     *                  SignalConsumers = { "0" ... "N" }, where 0 ... N are the indices of the Signals in the respective function
     *                  GAMProducers = { "0" ... "N" }, where 0 ... N are the indices of the Function as defined in the Functions node
     *                  GAMNamesProducers = { "NAME1" ... "NAMEN" }, where NAME1...NAMEN are the names of the functions
     *                  SignalProducers = { "0" ... "N" }, where 0 ... N are the indices of the Signals in the respective function
     *                }
     *              }
     *            }
     *          }
     *        }
     *      }
     */
    bool ResolveConsumersAndProducers();

    bool VerifyConsumersAndProducers();


    /**
     * @brief For every signal in every Function compute the memory size and the memory offset (if Ranges are defined).
     * @details The memory size is given by sizeof(Type) multiplied by all the Ranges. Illegal Ranges definitions (see post condition).
     * are trapped at this stage.
     * If the Ranges are defined, the memory offset is also computed and stored directly in bytes.
     * @return true if all the Ranges definitions are correctly defined and if the memory size can be successfully computed.
     * @pre
     *   ResolveConsumersAndProducers()
     * @post
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
     *                 ByteSize = NUMBER > 0
     *                 +ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ...}
     *                 +Samples = NUMBER > 0
     *                 +Frequency = NUMBER>0
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
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *              +Frequency = -1|NUMBER>0
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
     *            }
     *          }
     *        }
     *      }
     */
    bool ResolveFunctionSignalsMemorySize();

    /**
     * @brief For every Function compute the memory size and the memory offset for each DataSource which interfaces to the Function.
     * @details Computes the amount of memory grouped by DataSource and includes any signal specific ByteOffset information.
     * @return true if the total amount of memory can be successfully computed.
     * @pre
     *   ResolveFunctionSignalsMemorySize()
     * @post
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER  ={
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 Type = BasicType
     *                 NumberOfDimensions = 0|1|2
     *                 NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap) (max_idx<NumberOfElements)
     *                 ByteSize = NUMBER>0
     *                 +ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ...}
     *                 +Samples = NUMBER > 0
     *                 +Frequency = NUMBER>0
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
     *               Signals = {
     *                 *NUMBER = {
     *                   QualifiedName = "QualifiedName of the Signal"
     *                   +ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ... }
     *                   Frequency = -1|NUMBER>0
     *                   Samples = |NUMBER>0
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
     *            *NUMBER = {
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *              Frequency = -1|NUMBER>0
     *              Samples = -1|NUMBER>0
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
     *            }
     *          }
     *        }
     *      }
     */
    bool ResolveFunctionsMemory();

    /**
     * @brief For every DataSource add the memory information about each Function that interacts with the DataSource
     *  add the corresponding memory information.
     * @return true if the memory information can be successfully added to each DataSource definition.
     * @pre
     *   CalculateFunctionsMemory()
     * @post
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
     *                       Samples = -1|NUMBER>0
     *                    }
     *                 }
     *               }
     *             }
     *          }
     *        }
     *     }
     */
    bool AssignFunctionsMemoryToDataSource();

    /**
     * @brief For each GAM signal, the DataSource will write the name of the BrokerI to be used.
     * @return true if the BrokerI name can be successfully retrieved from the DataSource.
     * @pre
     *   AssignFunctionsMemoryToDataSource()
     * @post
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
     *                       Samples = -1|NUMBER>0
     *                       Broker = "Name of the Broker returned by the DataSource"
     *                    }
     *                 }
     *               }
     *             }
     *          }
     *        }
     *     }
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
     * @brief Copies the Function and DataSource databases.
     * @param[out] functionsDatabaseOut where to copy the Functions database into.
     * @param[out] dataSourcesDatabaseOut where to copy the DataSource database into.
     * @return true if both ConfigurationDatabase::Copy are successful.
     */
    bool Copy(ConfigurationDatabase &functionsDatabaseOut,
              ConfigurationDatabase &dataSourcesDatabaseOut);

    bool Set(ConfigurationDatabase &functionsDatabaseOut,
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

    ConfigurationDatabase globalDatabase;

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
    bool FlattenSignalsDatabase(const bool isFunctionsDatabase,
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
    bool ResolveDataSources(const SignalDirection direction);

    /**
     * @brief @see ResolveFunctionSignals()
     * @param[in] direction can be either InputSignals or OutputSignals.
     * @return @see ResolveFunctionSignals()
     */
    bool ResolveFunctionSignals(SignalDirection direction);

    /**
     * @brief Recursively adds a signal with an unknown type in the Function definition
     * @details If the type is not known in the Function definition this is obtained from the DataSource.
     * @param[in] signalName the name of the signal to be resolved.
     * @param[in] functionName the name of the Function to which this signal belongs to.
     * @param[in] dataSourceName the name of the DataSource to which this signal belongs to.
     * @param[in] numberOfFunctionSignals the number of signals that currently exist in the Function with name \a functionName.
     * @return true if the signal can be successfully resolved (which might include createing the signal).
     */
    bool ResolveFunctionSignal(const char8 * const signalName,
                               const char8 * const aliasName,
                               const char8 * const functionName,
                               const char8 * const dataSourceName,
                               uint32 &numberOfFunctionSignals,
                               bool &deleted);
    /**
     * @brief Final merge of signals from the DataSources to the Functions.
     * @details For every signal in every Function merge with the corresponding DataSource signal.
     * If the signal type was not originally defined in the Function it will get defined by the DataSource.
     * If incompatibilities are found in the signal definition in the DataSource or in the Function an error will be returned.
     * @param[in] direction can be either InputSignals or OutputSignals
     * @return true if all the signals can be successfully merged.
     */
    bool VerifyFunctionSignals(SignalDirection direction);

    /**
     * @brief @see ResolveConsumersAndProducers()
     * @param[in] consumers true if the function is being called to add Consumers, false if the function is being called to add Producers.
     * @return @see ResolveConsumersAndProducers()
     */
    bool ResolveConsumersAndProducers(bool consumers);

    bool BuildProducersRanges();

    bool CheckProducersRanges(uint32 *rangesArray,
                              uint32 numberOfElements);
    /**
     * @brief @see ResolveFunctionSignalsMemorySize()
     * @param[in] direction can be either InputSignals or OutputSignals
     * @return @see ResolveFunctionSignalsMemorySize()
     */
    bool ResolveFunctionSignalsMemorySize(SignalDirection direction);

    /**
     * @brief @see ResolveFunctionsMemory()
     * @param[in] direction can be either InputSignals or OutputSignals
     * @return @see ResolveFunctionsMemory()
     */
    bool ResolveFunctionsMemory(SignalDirection direction);

    /**
     * @brief @see AssignFunctionsMemoryToDataSource()
     * @param[in] direction can be either InputSignals or OutputSignals
     * @return @see AssignFunctionsMemoryToDataSource()
     */
    bool AssignFunctionsMemoryToDataSource(SignalDirection direction);

    /**
     * @brief @see AssignBrokersToSignals()
     * @param[in] direction can be either InputSignals or OutputSignals
     * @param[in] dataSource the DataSourceI to be queried.
     * @return @see AssignBrokersToSignals()
     */
    bool AssignBrokersToSignals(SignalDirection direction,
                                ReferenceT<DataSourceI> dataSource);

    /**
     * @brief Finds a signal with the name \a signalName in \a database.
     * @details Looks for a signal with a QualifiedName=signalName and leaves the database
     * pointing at node where the signal is found.
     * @param[in] signalName the name of the signal to be found.
     * @param[in] database the database where to look for the signal.
     * @return true if the signal is found.
     */
    bool FindSignalName(StreamString signalName,
                        ConfigurationDatabase &database);
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

    bool AddStateToFunction(ConfigurationDatabase &local,
                            const char8 *functionName,
                            const char8* stateName,
                            const char8 * threadName,
                            uint32 &syncSignals);

    bool AddStateToGAM(const char8* gamName,
                       const char8* stateName,
                       const char8 * threadName,
                       uint32 &syncSignals);

    bool AddSignalTime(const char8* tDsName);
    bool AddThreadCycleTime(const char8* threadFullName);

    bool WriteTimeSignalInfo(const char8*signalName);
    bool CheckTimeSignalInfo();

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEAPPLICATIONCONFIGURATIONBUILDER_H_ */

