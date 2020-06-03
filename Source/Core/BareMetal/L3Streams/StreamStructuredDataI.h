/**
 * @file StreamStructuredDataI.h
 * @brief Header file for class StreamStructuredDataI
 * @date 27/09/2018
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

 * @details This header file contains the declaration of the class StreamStructuredDataI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STREAMSTRUCTUREDDATAI_H_
#define STREAMSTRUCTUREDDATAI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StructuredDataI.h"
#include "BufferedStreamI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief Adds a stream to the StructuredDataI interface.
 */
class StreamStructuredDataI: public StructuredDataI {
public:

    /**
     * @brief Constructor
     */
    StreamStructuredDataI();

    /**
     * @brief Destructor
     */
    virtual ~StreamStructuredDataI();

    /**
     * @brief Sets the internal stream
     * @param[in] streamIn the internal stream.
     */
    virtual void SetStream(BufferedStreamI &streamIn);

    /**
     * @brief Retrieves the internal stream.
     * @return the internal stream.
     */
    virtual BufferedStreamI *GetStream();

protected:

    /**
     * Pointer to the internal stream
     */
    BufferedStreamI *stream;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L3STREAMS_STREAMSTRUCTUREDDATAI_H_ */

