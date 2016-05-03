/**
 * @file DataSourceContainer.h
 * @brief Header file for class DataSourceContainer
 * @date 11/04/2016
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

 * @details This header file contains the declaration of the class DataSourceContainer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCECONTAINER_H_
#define DATASOURCECONTAINER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "GAMSignalI.h"
#include "ReferenceT.h"
#include "GAM.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {


/**
 * @brief A container of DataSource objects.
 *
 * @details This object has to be inserted in the RealTimeApplication container
 * with the name "Data". It is a container for all the DataSource objects.
 *
 * @details The syntax in the configuration stream must be:
 *
 *     +Application = {\n
 *         +Data = {\n
 *             Class = DataSourceContainer\n
 *             +DataSource_Name = {\n
 *                 Class = DataSource
 *                 ...
 *             }\n
 *          }\n
 *     }\n
 */
class DLL_API DataSourceContainer: public ReferenceContainer {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    DataSourceContainer();

    /**
     * @brief Adds all the signals from a GAM GAMSignalsContainer into this container.
     *
     * @details For each GAMSignalsContainer contained inside the \a gam, loop on each
     * GAMSignalI and retrieves the path (GAMSignalI.GetPath) of the signal. Search for this path
     * inside each of the DataSource elements of this DataSourceContainer and checks if any of the found Reference is of type DataSourceSignalI.
     * If so then there is a match and the GAMSignalI is added to the found DataSourceSignalI either as a consumer (DataSourceSignalI.AddConsumer if this
     * GAMSignalI is an input to the GAM) or as a producer (DataSourceSignalI.AddProducer if this GAMSignalI is an output of the GAM). This is the typical
     * of GAMs which share signals directly with DataSource (i.e. drivers).
     *
     * If the path is not found and the GAMSignalI is not of type GAMSampledSignal then a new DataSourceSignal is created and added to the
     * DataSource specified in the path. This is the typical of GAMs which share signals between them.
     *
     * Finally two signals are automatically added to a DataSource named GAM_Times. The first signal is named GAM_NAME.AbsoluteUsecTime and the
     * second signal is named GAM_NAME.RelativeUsecTime, where GAM_NAME is the value returned by GAM::GetName. Both signals are of type uint64. The
     * former contains the number of microseconds elapsed since the beggining of the real-time cycle, while the latter contains the number of
     * microseconds from the beginning to the end of the execution of the \a gam.
     *
     * @param[in] gam is a Reference to the GAM containing all the data definitions to be inserted.
     *
     * @return true if all the GAM GAMSignalsContainer GAMSignalI elements can be successfully added either as a consumer or as a producer
     * to an existent DataSource element of this DataSourceContainer.
     * @pre
     *    gam.IsValid()
     */
    bool AddDataDefinition(ReferenceT<GAM> gam);

    /**
     * @brief Checks if for each DataSourceSignalI signal there is no more than one producer for each state.
     * @details No more than one producer per state is allowed for each data source. If no consumers are
     * defined for a data source, a warning will be generated (unused variable).
     * @return true if there is at most one GAM producer for each state in each DataSourceSignalI signal.
     */
    bool Verify();


    /**
     * @brief Prepares the environment for the next state.
     * @details Calls the function DataSourceSignalI::PrepareNextState() for all contained data source signals. Each specific
     * DataSourceSignalI can have its own implementation of the PrepareNextState(*) function.
     * @param[in] status contains information about the current and the next state.
     * @return true if DataSourceSignalI::PrepareNextState() is successful for all the contained data source signals.
     */
    bool PrepareNextState(const RealTimeStateInfo &status);


    /**
     * @brief Allocates the memory for each of the defined data sources.
     * @details Browses the tree and for each DataSource found calls the method DatSource::Allocate.
     * @return true if all DataSource::Allocate return true.
     */
    bool Allocate();


private:


    /**
     * @brief Adds a single GAMSignalI definition into the tree.
     * @param[in] definition is the definition to be inserted.
     * @param[in] gam is the GAM containing the definition.
     * @param[in] isProducer denotes if the GAM produces the variable.
     * @param[in] isConsumer denotes if the GAM consumes the variable.
     * @param[in] defaultPath is used when the path is not specified.
     * @return true if all the GAM GAMSignalsContainer GAMSignalI elements can be successfully added either as a consumer or as a producer
     * to an existent DataSource element of this DataSourceContainer.
     */
    bool AddSingleDataDefinition(ReferenceT<GAMSignalI> definition,
                                 ReferenceT<GAM> gam,
                                 const bool isProducer,
                                 const bool isConsumer,
                                 StreamString defaultPath = "");


};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCECONTAINER_H_ */

