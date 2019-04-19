/**
 * @file HttpDirectoryResource.h
 * @brief Header file for class HttpDirectoryResource
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

 * @details This header file contains the declaration of the class HttpDirectoryResource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4HTTPSERVICE_HTTPDIRECTORYRESOURCE_H_
#define L4HTTPSERVICE_HTTPDIRECTORYRESOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HttpDataExportI.h"
#include "File.h"
#include "Object.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief HTTP browsing of files and directories.
 *
 * @details The configuration syntax is (names are only given as an example):
 * <pre>
 * +HttpDirectoryResource1 = {
 *     Class = HttpDirectoryResource
 *     BaseDir = "/" //Compulsory. The base directory w.r.t. to which all the paths are evaluated.
 * }
 * </pre>
 */
class HttpDirectoryResource: public Object, public HttpDataExportI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
HttpDirectoryResource    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~HttpDirectoryResource();

    /**
     * @brief Calls Object::Initialise and reads the BaseDir parameters (see class description) .
     * @return true if the parameters are correctly specified.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Retrieves the contents of the directory or the details of a given file.
     * @param[out] data output where the directory contents or the file information is written into.
     * @param[in] protocol expected to contain a \a path parameter where to read the relative path (w.r.t. to the BaseDir defined above).
     * @return true if all the data write operations are successful.
     */
    virtual bool GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol);

    /**
     * @brief Retrieves the contents of a given file.
     * @param[out] stream output where the directory contents or the file information is written into.
     * @param[in] protocol expected to contain a \a path parameter where to read the relative path (w.r.t. to the BaseDir defined above).
     * @return true if all the data write operations are successful.
     */
    virtual bool GetAsText(StreamI &stream, HttpProtocol &protocol);

    /**
     * @brief Sets if replyNotFound should be set when a file is not found.
     */
    void SetReplyNotFound(bool replyNotFoundIn);

private:

    /**
     * @brief Helper function which check if a file has a given extension.
     * @param[in] fname the name of the file to check.
     * @param[in] ext the file name extension.
     * @return true if the file terminates with the extension.
     */
    bool CheckExtension(StreamString &fname, const char8 * const ext) const;

    /**
     * @brief Helper function which streams the filename over the provided stream.
     * @param[in] fname the name of the file to stream.
     * @param[out] protocol to write the Content-type.
     * @return true if the file can be successfully streamed.
     */
    bool ServeFile(StreamString &fname, HttpProtocol &protocol) const;

    /**
     * The base directory w.r.t. which all the paths are evaluated.
     */
    StreamString baseDir;

    /**
     * True if ReplyNotFound should be set when a file is not found.
     */
    bool replyNotFound;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L4HTTPSERVICE_HTTPDIRECTORYRESOURCE_H_ */

