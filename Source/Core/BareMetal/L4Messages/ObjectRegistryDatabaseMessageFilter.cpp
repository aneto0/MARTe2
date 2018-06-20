/**
 * @file ObjectRegistryDatabaseMessageFilter.cpp
 * @brief Source file for class ObjectRegistryDatabaseMessageFilter
 * @date 19/06/2018
 * @author Andre' Neto
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

 * @details This source file contains the definition of all the methods for
 * the class ObjectRegistryDatabaseMessageFilter (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ObjectRegistryDatabase.h"
#include "ObjectRegistryDatabaseMessageFilter.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ObjectRegistryDatabaseMessageFilter::ObjectRegistryDatabaseMessageFilter() :
        MessageFilter(true), Object() {

}

ObjectRegistryDatabaseMessageFilter::~ObjectRegistryDatabaseMessageFilter() {

}

ErrorManagement::ErrorType ObjectRegistryDatabaseMessageFilter::ConsumeMessage(ReferenceT<Message> &messageToTest) {
    ErrorManagement::ErrorType err;
    err.parametersError = !messageToTest.IsValid();

    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    StreamString function;
    ReferenceT<StructuredDataI> params;
    StreamString rootNodeStr;
    //Do not do this rootNode = ReferenceT<ReferenceContainer>(ord) as otherwise the smart pointer implementation would destroy the ObjectRegistryDatabase!
    ReferenceContainer *rootNode = NULL_PTR(ReferenceContainer *);

    if (err.ErrorsCleared()) {
        CCString functionStr = messageToTest->GetFunction();
        function = functionStr.GetList();
        params = messageToTest->Get(0u);
        if (params.IsValid()) {
            if(params->Read("Root", rootNodeStr)) {
                REPORT_ERROR(ErrorManagement::Information, "Going to act on %s", rootNodeStr.Buffer());
            }
        }
        if (rootNodeStr.Size() == 0u) {
            rootNode = ord;
            rootNodeStr = "root";
        }
        else {
            ReferenceT<ReferenceContainer> foundNode = ord->Find(rootNodeStr.Buffer());
            err.parametersError = !foundNode.IsValid();
            if (err.ErrorsCleared()) {
                rootNode = foundNode.operator ->();
            }
            else {
                REPORT_ERROR(err, "Could not find the node with name %s", rootNodeStr.Buffer());
            }
        }
    }
    //lint -e{613} the rootNode cannot be NULL as otherwise err.ErrorsCleared() would be false.
    if (err.ErrorsCleared()) {
        if (function == "purge") {
            rootNode->Purge();
            if (rootNode != ord) {
                err.fatalError = !ord->Delete(rootNodeStr.Buffer());
            }
        }
        else if (function == "load") {
            err.parametersError = !params.IsValid();
            if (err.ErrorsCleared()) {
                ReferenceT<ReferenceContainer> rc(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                err.parametersError = !rc->Initialise(*params.operator ->());
                uint32 i;
                uint32 nElements = rc->Size();
                for (i = 0u; (err.ErrorsCleared()) && (i < nElements); i++) {
                    err.parametersError = !rootNode->Insert(rc->Get(i));
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "No configuration data found in the message");
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Unsupported function", function.Buffer());
            err.parametersError = true;
        }
    }
    return err;

}

CLASS_REGISTER(ObjectRegistryDatabaseMessageFilter, "1.0")
}

