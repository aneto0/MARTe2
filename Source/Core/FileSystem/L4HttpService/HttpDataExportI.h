/**
 * @file HttpDataExportI.h
 * @brief Header file for class HttpDataExportI
 * @date 31/10/2018
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

 * @details This header file contains the declaration of the class HttpDataExportI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4HTTPSERVICE_HTTPDATAEXPORTI_H_
#define L4HTTPSERVICE_HTTPDATAEXPORTI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HttpProtocol.h"
#include "StreamStructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Classes that implement this interface know how to handle HTTP requests.
 * @details Most of the classes are expected to expose their internal data using the ExportData method.
 * This interface is expected to be used only in cases where the knowledge of the HTTP interface is absolutely required.
 */
class HttpDataExportI {
public:
    /**
     * @brief Constructor. NOOP.
     */
    HttpDataExportI();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~HttpDataExportI();

    /**
     * @brief Streams the data as a json structure.
     * @details This method already implements the setting of the appropriated HTTP header announcing that data will be chunked (see HttpChunkedStream).
     * Classes specialising this method will typically call it in order to prepare the HTTP header.
     * @param[out] data holds the tree mapping the json structure to be created and streamed.
     * @param[out] protocol writes the HTTP header  with the json and chunked options.
     * @return true if the header was correctly configured.
     */
    virtual bool GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol);

    /**
     * @brief Streams the data as text (text/html).
     * @details This method already implements the setting of the appropriated HTTP header announcing that data will be chunked (see HttpChunkedStream).
     * Classes specialising this method will typically call it in order to prepare the HTTP header.
     * @param[out] stream holds the tree mapping the json structure to be created and streamed.
     * @param[out] protocol writes the HTTP header with the text/html and chunked options.
     * @return true if the header was correctly configured.
     */
    virtual bool GetAsText(StreamI &stream, HttpProtocol &protocol);

protected:
    /**
     * @brief Helper function to be used when a given resource is not available.
     * @param[out] protocol writes the HTTP protocol header.
     * @return true if the header is successfully written.
     */
    bool ReplyNotFound(HttpProtocol &protocol) const;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L4HTTPSERVICE_HTTPDATAEXPORTI_H_ */

