/**
 * @file RealTimeDataSourceContainer.h
 * @brief Header file for class RealTimeDataSourceContainer
 * @date 21/03/2016
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

 * @details This header file contains the declaration of the class RealTimeDataSourceContainer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEDATASOURCECONTAINER_H_
#define REALTIMEDATASOURCECONTAINER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "RealTimeDataDefI.h"
#include "BasicGAM.h"
#include "ReferenceT.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class RealTimeDataSourceContainer: public ReferenceContainer {

public:
    CLASS_REGISTER_DECLARATION()


    /**
     * @brief Constructor
     */
    RealTimeDataSourceContainer();

    /**
     * @brief Adds all the RealTimeDataDefI defined in the GAM in input.
     *
     * @details Explores the RealTimeDataDefContainer elements of \a gam and for each specific RealTimeDataDefI
     * inserts an element in the path specified by the "Path" field of RealTimeDataDefI using this object as root.
     * The final definition is a RealTimeDataSourceDef type and this function initialises its fields "defaultValue"
     * and "type" reading informations from the related RealTimeDataDefI. Finally each RealTimeDataSourceDef
     * will contain a list of RealTimeDataDefRecords, one for each state where the variable will be used
     * and \a gam will be inserted as consumer or producer depending on the definition (RealTimeDataDefContainer::IsInput or
     * RealTimeDataDefContainer::IsOutput respectively).
     *
     * @details If some RealTimeDataDefI definitions does not provide the field "Path" the path will be considered
     * equal to the name.
     *
     * @param[in] gam is a Reference to the GAM containing all the definitions to be inserted.
     * @return false in case of errors, true otherwise.
     */
    bool AddDataDefinition(ReferenceT<BasicGAM> gam);

    /**
     * @brief Checks if for each RealTimeDataSourceDef there no more than one producer for each state.
     * @details No more than one producer per state is allowed for each data source. If no consumers are
     * defined for a data source, a warning will be generated (unused variable).
     * @return true if there is at most one GAM producer for each state in each RealTimeDataSourceDef.
     */
    bool Verify();


    /**
     * @brief Prepares the environment for the next state.
     * @details If the data source is not used during the current state but will be used in the next, it will be
     * reset to its default value. The value of the variable will be preserved for the next state if it is used in the
     * current one. Since this function will be executed in a low priority thread in parallel with the
     * real-time execution, the operations on the data source memory will be executed on a currently unused buffer (double
     * buffer implementation) which will be available just after the state switch as the new data source to be used.
     * @param status contains information about the current and the next state.
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
     * @brief Adds a single RealTimeDataDefI definition into the tree.
     * @param[in] definition is the definition to be inserted.
     * @param[in] gam is the GAM containing the definition.
     * @param[in] isProducer denotes if the GAM produces the variable.
     * @param[in] isConsumer denotes if the GAM consumes the variable.
     * @param[in] defaultPath is used when the path is not specified.
     * @return false in case of errors, false otherwise.
     */
    bool AddSingleDataDefinition(ReferenceT<RealTimeDataDefI> definition,
                                 ReferenceT<BasicGAM> gam,
                                 const bool isProducer,
                                 const bool isConsumer,
                                 StreamString defaultPath = "");


};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEDATASOURCECONTAINER_H_ */

