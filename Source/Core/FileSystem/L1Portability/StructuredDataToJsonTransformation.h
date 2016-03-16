/**
 * @file StructuredDataToJsonTransformation.h
 * @brief Header file for class StructuredDataToJsonTransformation
 * @date 08/03/2016
 * @author Ivan Herrero
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

 * @details This header file contains the declaration of the class StructuredDataToJsonTransformation
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRUCTUREDDATA_TO_JSON_TRANSFORMATION
#define STRUCTUREDDATA_TO_JSON_TRANSFORMATION

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StructuredDataI.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class DLL_API StructuredDataToJsonTransformation {

public:
    StructuredDataToJsonTransformation();
    virtual ~StructuredDataToJsonTransformation();
    //JSON defined at RFC 7159 ["The JavaScript Object Notation (JSON) Data Interchange Format"|https://tools.ietf.org/html/rfc7159]
    bool Execute (MARTe::StructuredDataI& input, MARTe::StreamString& output);
    bool FormatJson (MARTe::StreamString& input, MARTe::StreamString& output);
    bool FilterBlanks (MARTe::StreamString& input, MARTe::StreamString& output);
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STRUCTUREDDATA_TO_JSON_TRANSFORMATION */

