/**
 * @file DataExportI.h
 * @brief Header file for class DataExportI
 * @date 14/09/2018
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class DataExportI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATAEXPORTI_H_
#define DATAEXPORTI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ProtocolI.h"
#include "StreamI.h"
#include "StreamStructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Interface to export relevant user-defined data using a given protocol (e.g. HTTP).
 * @details The data to be exported can be expressed either as a structure of as free text.
 */
class DataExportI {
public:

    /**
     * @brief Default constructor.
     */
    DataExportI();

    /**
     * @brief Destructor.
     */
    virtual ~DataExportI();

    /**
     * @brief Exports data to the StreamStructuredDataI passed in input.
     * @param[out] data contains the exported data in output.
     * @param[in] protocol the protocol used to exchange data.
     * @return true if the operation succeeds, false otherwise.
     */
    virtual bool GetAsStructuredData(StreamStructuredDataI &data, ProtocolI &protocol)=0;

    /**
     * @brief Exports data as text writing on the stream in input.
     * @param[out] stream the stream where to write.
     * @param[in] protocol the protocol used to exchange data.
     * @return true if the operation succeeds, false otherwise.
     */
    virtual bool GetAsText(StreamI &stream, ProtocolI &protocol) = 0;

    /**
     * @brief Returns the reply code checking the data provided by the protocol in input.
     * @param[in] the protocol used to exchange data.
     * @return the reply code.
     */
    virtual int32 GetReplyCode(ProtocolI &data) = 0;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATAEXPORTI_H_ */

