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
 * with the name "Data" and contains all the DataSource objects.
 *
 * @details Provides functions to configure the contained data sources on the base of
 * the GAM signals.
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
     * @brief Constructor
     */
    DataSourceContainer();

    /**
     * @brief Adds all the GAMSignalI defined in the GAM in input.
     *
     * @details Explores the SignalsContainer elements of \a gam and for each specific GAMSignalI
     * inserts an element in the path specified by the "Path" field of GAMSignalI using this object as root.
     * The final definition is a RealTimeDataSourceDef type and this function initialises its fields "Default"
     * and "Type" reading informations from the related GAMSignalI. Finally each RealTimeDataSourceDef
     * will contain a list of RealTimeDataDefRecords, one for each state where the variable will be used
     * and \a gam will be inserted as consumer or producer depending on the definition (SignalsContainer::IsInput or
     * SignalsContainer::IsOutput respectively).
     *
     * @details If some GAMSignalI definitions does not provide the field "Path" the path will be considered
     * equal to the name.
     *
     * @param[in] gam is a Reference to the GAM containing all the definitions to be inserted.
     * @return false in case of errors, true otherwise.
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
     * @details Calls the function DataSourceSignalI::PrepareNextState(*) for all contained data source signal. Each specific
     * DataSourceSignalI can have its own implementation of the PrepareNextState(*) function.
     * @param[in] status contains information about the current and the next state.
     * @return true if the default value (expressed in the configuration data) is compatible with the variable type, false
     * otherwise.
     */
    bool PrepareNextState(const RealTimeStateInfo &status);


    /**
     * @brief Allocates the memory for each data source defined.
     * @details Browses the tree and for each RealTimeDataSourceDef found allocates the memory for the specific type.
     * @return false in case of errors, true otherwise.
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
     * @return false in case of errors, false otherwise.
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

#endif /* SOURCE_CORE_BAREMETAL_L5GAMS_DATASOURCECONTAINER_H_ */

