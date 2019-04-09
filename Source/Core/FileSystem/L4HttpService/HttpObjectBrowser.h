/**
 * @file HttpObjectBrowser.h
 * @brief Header file for class HttpObjectBrowser
 * @date 29/10/2018
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

 * @details This header file contains the declaration of the class HttpObjectBrowser
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4HTTPSERVICE_HTTPOBJECTBROWSER_H_
#define L4HTTPSERVICE_HTTPOBJECTBROWSER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HttpDataExportI.h"
#include "HttpRealmI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief HTTP browsing of any ReferenceContainer.
 *
 * @details Given a Root object, retrieves the contents of a target object. The path to the target object (w.r.t. to the Root) is encoded in the HttpProtocol (see HttpProtocol::GetUnmatchedId).
 *
 * The Root can be a path to any ReferenceContainer in the ObjectRegistryDatabase (e.g. Root = "A.B.C"), or the ObjectRegistryDatabase itself (Root = "/") or this instance (Root = ".").
 *
 * The following interaction possibilities are supported:
 *  - GetAsStructuredData is called:
 *    - The target is this instance and the Root is also this instance: calls ReferenceContainer::ExportsData (and thus lists all the child objects).
 *    - The target is this instance and the Root is another object: lists all the objects in target object.
 *    - The target is not this instance and the target implements HttpDataExportI: calls GetAsStructuredData on the target object.
 *    - The target is not this instance and the target does not implement HttpDataExportI: calls ExportData on the target object.
 *    - None of the above: reply HttpDataExportI::ReplyNotFound.
 *  -GetAsText is called:
 *    - The target is this instance: checks if there is an HttpDirectoryResource capable of handling the request and if so forwards it.
 *    - The target is not this instance and the target implements HttpDataExportI: calls GetAsText on the target object.
 *    - None of the above: reply HttpDataExportI::ReplyNotFound.
 * If GetAsText is called and the path points at any other
 *
 * @details The configuration syntax is (names are only given as an example):
 * <pre>
 * +HttpObjectBrowser1 = {
 *     Class = HttpObjectBrowser
 *     Root = "/" //Compulsory. The Root object. Can be a path to any ReferenceContainer in the ObjectRegistryDatabase (e.g. Root = "A.B.C"), or the ObjectRegistryDatabase itself (Root = "/") or this instance (Root = ".").
 *     Realm = ARealmImplementation //Optional. Points at an object that implements the HttpRealmI interface. If it exits every HTTP request will be validated by such object.
 *     CloseOnAuthFail = 1 //Optional (default = 1). Close the connection in case of an authentication failure?
 * }
 * </pre>
 */
class HttpObjectBrowser: public ReferenceContainer, public HttpDataExportI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    HttpObjectBrowser();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~HttpObjectBrowser();

    /**
     * @brief See ReferenceContainer::Purge.
     */
    virtual void Purge(ReferenceContainer &purgeList);

    /**
     * @brief Calls ReferenceContainer::Initialise and reads the Root and the CloseOnAuthFail parameters (see class description) .
     * @return true if the parameters are correctly specified and the Root is valid.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief See HttpDataExportI::GetAsStructuredData and the class description above.
     * @param[out] data see HttpDataExportI::GetAsStructuredData and the class description above.
     * @param[out] protocol see HttpDataExportI::GetAsStructuredData and the class description above.
     * @return see HttpDataExportI::GetAsStructuredData.
     */
    virtual bool GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol);

    /**
     * @brief See HttpDataExportI::GetAsText and the class description above.
     * @param[out] stream see HttpDataExportI::GetAsText and the class description above.
     * @param[out] protocol see HttpDataExportI::GetAsText and the class description above.
     * @return see HttpDataExportI::GetAsText.
     */
    virtual bool GetAsText(StreamI &stream, HttpProtocol &protocol);

private:

    /**
     * @brief Check if the request can access the resource (see HttpService and HttpRealmI).
     * @param[in] protocol the HTTP protocol associated to this request.
     * @return true if the request can access the resource.
     */
    virtual bool CheckSecurity(HttpProtocol &protocol);

    /**
     * @brief Finds the reference w.r.t. to the root.
     * @param[in] unmatchedPath the path to be found.
     * @return the found Reference (or an invalid Reference otherwise).
     */
    Reference FindReference(const char8 * const unmatchedPath);

    /**
     * @brief Finds the reference w.r.t. to the root.
     * @param[in] protocol the HTTP request.
     * @return the found Reference (or an invalid Reference otherwise).
     */
    Reference FindTarget(HttpProtocol &protocol);

    /**
     * The realm associated to this browser.
     */
    ReferenceT<HttpRealmI> realm;

    /**
     * Specifies if after an authentication
     * failure the connection has to be closed or not
     */
    uint8 closeOnAuthFail;

    /**
     * The root object. All requests w.r.t. to the root. Cannot be ReferenceT<ReferenceContainer> because it gives problems with root = Reference(ObjectRegistryDatabase::Instance()).
     */
    ReferenceContainer *root;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L4HTTPSERVICE_HTTPOBJECTBROWSER_H_ */

