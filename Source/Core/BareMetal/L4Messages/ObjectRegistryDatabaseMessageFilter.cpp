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
    ReferenceT<ReferenceContainer> rootNode;

    if (err.ErrorsCleared()) {
        function = messageToTest->GetFunction().GetList();
        params = messageToTest->Get(0u);
        if (params.IsValid()) {
            err.parametersError = !params->Read("Root", rootNodeStr);
            if (!err.ErrorsCleared()) {
                REPORT_ERROR(err, "Could not read the Root from where to purge");
            }
        }
        if (rootNodeStr.Size() == 0u) {
            rootNode = ReferenceT<ReferenceContainer>(ord);
            rootNodeStr = "root";
        }
        else {
            rootNode = ord->Find(rootNodeStr.Buffer());
        }
        if (rootNode.IsValid()) {
            REPORT_ERROR(ErrorManagement::Information, "Going to act on the ObjectRegistryDatabase node %s", rootNode->GetName());
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not find the node %s in the ObjectRegistryDatabase", rootNodeStr.Buffer());
        }
    }
    if (err.ErrorsCleared()) {
        if (function == "purge") {
            err.parametersError = (rootNode == ReferenceT<ReferenceContainer>(ord));
            if (err.ErrorsCleared()) {
                REPORT_ERROR(ErrorManagement::Information, "Going to purge node %s", rootNode->GetName());
                rootNode->Purge();
            }
            else {
                REPORT_ERROR(err, "Cannot purge the root node");
            }
        }
        else if (function == "load") {
            err.parametersError = !params.IsValid();
            if (err.ErrorsCleared()) {
                ReferenceT<ReferenceContainer> rc(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                err.parametersError = !rc->Initialise(*params.operator ->());
                uint32 i;
                for (i = 0u; (i < rc->Size()) && (err.ErrorsCleared()); i++) {
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

