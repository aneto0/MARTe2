/**
 * @file BasicGAM.h
 * @brief Header file for class BasicGAM
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

 * @details This header file contains the declaration of the class BasicGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICGAM_H_
#define BASICGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BasicRealTimeDataSourceInputReader.h"
#include "BasicRealTimeDataSourceOutputWriter.h"
#include "GAMI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The Generic Acquisition Module.
 * @details In order to allow custom implementations, some of the methods
 * of this class are pure virtual. A generic derived class should have
 * specific IO structures and their definitions (see RealTimeDataDef)
 * can be expressed in the global configuration stream and / or in the local
 * configuration stream.
 *
 * @details The Initialise method (which can be override) configures the GAM reading
 * from the global configuration stream. If the definitions of the input and output
 * structures is incomplete (or also empty), the method ConfigureFunction(*) will merge
 * them with the ones declared in the local configuration.
 *
 * @details The syntax in the configuration stream has to be:
 * GAM_name = {\n
 *     Class = GAM_className\n
 *     RealTimeDataDefContainer_name = {\n
 *         Class = RealTimeDataDefContainer\n
 *         ...\n
 *     }\n
 *     ...\n
 * }\n
 */
class BasicGAM: public GAMI {
public:

    /**
     * @brief Constructor
     */
    BasicGAM();


    /**
     * @brief Links the GAM with RealTimeDataSource.
     * @details Configures the input (RealTimeDataInputReader) and output interfaces (RealTimeDataOutputWriter)
     * to communicate with the RealTimeDataSource.
     */
    virtual bool ConfigureDataSourceLinks();


    /**
     * @brief Retrieves the input interface with the RealTimeDataSource.
     * @return the input interface with the RealTimeDataSource.
     */
    virtual Reference GetInputReader() const;

    /**
     * @brief Retrieves the output interface with the RealTimeDataSource.
     * @return the output interface with the RealTimeDataSource.
     */
    virtual Reference GetOutputWriter() const;


    /**
     * @brief Return true if the GAM is synchronized, false otherwise.
     * @return true if the GAM is synchronized, false otherwise.
     */
    virtual bool IsSync() ;


protected:

    /**
     * The input interface with the RealTimeDataSource
     */
    ReferenceT<BasicRealTimeDataSourceInputReader> inputReader;

    /**
     * The output interface with the RealTimeDataSource
     */
    ReferenceT<BasicRealTimeDataSourceOutputWriter> outputWriter;

    //? IOData?
    //? context?

};

}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICGAM_H_ */

