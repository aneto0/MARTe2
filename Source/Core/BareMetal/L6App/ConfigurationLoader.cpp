/**
 * @file ConfigurationLoader.cpp
 * @brief Source file for class ConfigurationLoader
 * @author Andre' Neto
 * @date 03/02/2022
 *
 * @copyright Copyright 2019 EPFL SPC Lausanne
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
 * the class ConfigurationLoader (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationLoader.h"
#include "ObjectRegistryDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
ConfigurationLoader::ConfigurationLoader() :
        ReferenceContainer(), MessageI() {
    reloadLast = true;
    lastSeed = 0u;
}

ConfigurationLoader::~ConfigurationLoader() {
}

uint32 ConfigurationLoader::GetSeed() {
    uint32 ret = 0u;
    if (loaderHash.IsValid()) {
        ret = loaderHash->GetSeed();
    }
    return ret; 
}

bool ConfigurationLoader::Initialise(StructuredDataI & data) {
    bool ok = ReferenceContainer::Initialise(data);
    if (ok) {
        StreamString reloadLastStr;
        if (data.Read("ReloadLast", reloadLastStr)) {
            if (reloadLastStr == "true") {
                reloadLast = true;
            }
            else if (reloadLastStr == "false") {
                reloadLast = false;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "ReloadLast shall be either set to true or false. %s is not supported.", reloadLastStr.Buffer());
                ok = false;
            }
        }
    }
    if (ok) {
        //Load all the Messages
        for (uint32 c=0u; c<Size(); c++) {
            ReferenceT <ReferenceContainer> msgContainer = Get(c);
            if (!loaderHash.IsValid()) {
                loaderHash = Get(c);
            }
            if (msgContainer.IsValid()) {
                uint32 j;
                uint32 nOfMessages = msgContainer->Size();
                for (j = 0u; (j < nOfMessages) && (ok); j++) {
                    ReferenceT < Message > msg = msgContainer->Get(j);
                    ok = msg.IsValid();
                    if (ok) {
                        StreamString msgName = msg->GetName();
                        if (msgName == "PreConfiguration") {
                            preConfigMsg = msg;
                        }
                        else if (msgName == "FailedConfiguration") {
                            failedConfigMsg = msg;
                        }
                        else if (msgName == "PostConfiguration") {
                            postConfigMsg = msg;
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::ParametersError, "Message %s is not supported.", msgName.Buffer());
                            ok = false;
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Found an invalid Message in container %s", msgContainer->GetName());
                        ok = false;
                    }
                }
            }
        }
    }
    if (ok) {
        //Look for the loader class
        bool found = false;
        uint32 nObjs = ObjectRegistryDatabase::Instance()->Size();
        for (uint32 i=0u; (i<nObjs) && (!found); i++) {
            loader = ObjectRegistryDatabase::Instance()->Get(i);
            found = loader.IsValid();
        }
        ok = found;
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not find a valid Loader installed");
        }
    }
    return ok;
}

ErrorManagement::ErrorType ConfigurationLoader::ApplyConfiguration(StreamI &source, StreamString &errStream) {
    ErrorManagement::ErrorType ret = SendConfigurationMessage(preConfigMsg);
    if (ret.ErrorsCleared()) {
        //Make sure this class is not removed...
        Reference keepAlive = this;
        uint32 nOfObjs = ObjectRegistryDatabase::Instance()->Size();
        REPORT_ERROR(ErrorManagement::Debug, "Purging ObjectRegistryDatabase with %d objects", nOfObjs);
        ObjectRegistryDatabase::Instance()->Purge();
        nOfObjs = ObjectRegistryDatabase::Instance()->Size();
        REPORT_ERROR(ErrorManagement::Debug, "Purge ObjectRegistryDatabase. Number of left objects: %d", nOfObjs);
        ret = loader->Reconfigure(source, errStream);
        if (!ret.ErrorsCleared()) {
            bool sendFailedConfigMsg = !reloadLast;
            if (reloadLast) {
                ErrorManagement::ErrorType retReload = loader->ReloadLastValidConfiguration();
                if (!retReload.ErrorsCleared()) {
                    sendFailedConfigMsg = true;
                }
            }
            if (sendFailedConfigMsg) {
                (void) SendConfigurationMessage(failedConfigMsg);
            }
        }
    }
    if (ret.ErrorsCleared()) {
        ret = SendConfigurationMessage(postConfigMsg);
    }
    return ret;
}

ErrorManagement::ErrorType ConfigurationLoader::ApplyConfigurationWithHash(StreamString &source, const uint32 hash, StreamString &errStream) {
    ErrorManagement::ErrorType err;
    err.fatalError = !loaderHash.IsValid();
    if (err.ErrorsCleared()) {
        uint32 computedHash = loaderHash->ComputeHash(source.Buffer(), source.Size());
        err.fatalError = (computedHash != hash);
        if (!err.ErrorsCleared()) {
            (void)errStream.Printf("Computed hash (%d) does not match offered hash (%d)", computedHash, hash);
            REPORT_ERROR(ErrorManagement::FatalError, errStream.Buffer());
        }
    }
    if (err.ErrorsCleared()) {
        (void)source.Seek(0LLU);
        err = ApplyConfiguration(source, errStream);
    }
    return ErrorManagement::FatalError;
}

ErrorManagement::ErrorType ConfigurationLoader::SendConfigurationMessage(ReferenceT<Message> msg) {
    ErrorManagement::ErrorType err;
    if (msg.IsValid()) {
        msg->SetAsReply(false);
        err = MessageI::SendMessage(msg, this);
        if (!err.ErrorsCleared()) {
            StreamString destination = msg->GetDestination();
            StreamString function = msg->GetFunction();
            REPORT_ERROR(ErrorManagement::FatalError, "Could not send message to %s [%s]", destination.Buffer(), function.Buffer());
        }
    }
    return err;
}



CLASS_REGISTER(ConfigurationLoader, "1.0")

}

