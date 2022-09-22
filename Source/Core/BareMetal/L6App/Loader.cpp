/**
 * @file Loader.cpp
 * @brief Source file for class Loader
 * @date 04/04/2018
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
 * the class Loader (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "JsonParser.h"
#include "Loader.h"
#include "MessageI.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"
#include "XMLParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

Loader::Loader() :
        Object() {
    firstLoading = true;
    reloadLast = false;
}

//lint -e{1551} purge must be called in the destructor
Loader::~Loader() {
    keepAliveObjs.Purge();
}

ErrorManagement::ErrorType Loader::Configure(StructuredDataI &data, StreamI &configuration) {
    ErrorManagement::ErrorType ret = Object::Initialise(data);
    uint32 defaultCPUs = 0x1u;
    if (!data.Read("DefaultCPUs", defaultCPUs)) {
        REPORT_ERROR_STATIC(ErrorManagement::Warning, "DefaultCPUs not specified");
    }
    REPORT_ERROR_STATIC(ErrorManagement::Information, "DefaultCPUs set to %d", defaultCPUs);
    ProcessorType::SetDefaultCPUs(defaultCPUs);

    uint32 schedulerGranularity = 0u;
    if (data.Read("SchedulerGranularity", schedulerGranularity)) {
        Sleep::SetSchedulerGranularity(schedulerGranularity);
    }
    REPORT_ERROR_STATIC(ErrorManagement::Information, "SchedulerGranularity is %d", Sleep::GetSchedulerGranularity());

    StreamString buildTokens;
    if (data.Read("BuildTokens", buildTokens)) {
        uint32 i;
        uint32 nTokens = static_cast<uint32>(buildTokens.Size());
        for (i = 0u; (ret.ErrorsCleared()) && (i < nTokens); i++) {
            char8 token = buildTokens[i];
            ret.initialisationError = !ReferenceContainer::AddBuildToken(token);
        }
        if (ret.ErrorsCleared()) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Added build tokens %s", buildTokens.Buffer());
        }
        else {
            REPORT_ERROR_STATIC(ret, "Failed to add build token %s", buildTokens.Buffer());
        }
    }
    StreamString domainTokens;
    if (data.Read("DomainTokens", domainTokens)) {
        uint32 i;
        uint32 nTokens = static_cast<uint32>(domainTokens.Size());
        for (i = 0u; (ret.ErrorsCleared()) && (i < nTokens); i++) {
            char8 token = domainTokens[i];
            ret.initialisationError = !ReferenceContainer::AddDomainToken(token);
        }
        if (ret.ErrorsCleared()) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Added domain tokens %s", domainTokens.Buffer());
        }
        else {
            REPORT_ERROR_STATIC(ret, "Failed to add domain tokens %s", domainTokens.Buffer());
        }
    }
    //Read the parser type
    if (ret.ErrorsCleared()) {
        ret.parametersError = !data.Read("Parser", parserType);
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "No Parser specified");
        }
    }
    if (ret.ErrorsCleared()) {
        ret.initialisationError = !configuration.Seek(0LLU);
    }
    if (ret.ErrorsCleared()) {
        StreamString errStream;
        ret = Reconfigure(configuration, errStream);
    }
    if (ret.ErrorsCleared()) {
        ret = PostInit();
    }
    if (ret.ErrorsCleared()) {
        //Remove LoaderPostInit and the KeepAlive components, so that they are not potentially loaded as part of a Reconfiguration exercise 
        (void) parsedConfiguration.Delete("+LoaderPostInit");
        //lint -e{1788} -e{9007} keepAliveObjs size is not changed.
        for (uint32 i=0u; (ret.ErrorsCleared()) && (i<keepAliveObjs.Size()); i++) {
            StreamString objFullName;
            (void) objFullName.Printf("+%s", keepAliveObjs.Get(i)->GetName());
            ret.parametersError = !parsedConfiguration.Delete(objFullName.Buffer());
        }
    }
    if (ret.ErrorsCleared()) {
        if (data.Read("MessageDestination", messageDestination)) {
            ret.parametersError = !data.Read("MessageFunction", messageFunction);
            if (!ret) {
                REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "When the MessageDestination is set the MessageFunction shall also be set");
            }
        }
    }
    return ret;
}

ErrorManagement::ErrorType Loader::Reconfigure(StreamI &configuration, StreamString &errStream) {
    ErrorManagement::ErrorType ret;
    ConfigurationDatabase newParsedConfiguration;
    ret.fatalError = !newParsedConfiguration.MoveToRoot();
    if (ret.ErrorsCleared()) {
        if (parserType == "xml") {
            XMLParser parser(configuration, newParsedConfiguration, &errStream);
            ret.initialisationError = !parser.Parse();
        }
        else if (parserType == "json") {
            JsonParser parser(configuration, newParsedConfiguration, &errStream);
            ret.initialisationError = !parser.Parse();
        }
        else if (parserType == "cdb") {
            StandardParser parser(configuration, newParsedConfiguration, &errStream);
            ret.initialisationError = !parser.Parse();
        }
        else {
            ret = ErrorManagement::ParametersError;
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Unknown Parser specified");
        }
        if (!ret) {
            StreamString errPrint;
            (void) errPrint.Printf("Failed to parse %s", errStream.Buffer());
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, errPrint.Buffer());
        }
    }
    if (ret.ErrorsCleared()) {
        ret = Reconfigure(newParsedConfiguration, errStream);
    }
    return ret; 
}

ErrorManagement::ErrorType Loader::Reconfigure(StructuredDataI &configuration, StreamString &errStream) {
    return ReconfigureImpl(configuration, errStream, true);
}

ErrorManagement::ErrorType Loader::ReconfigureImpl(StructuredDataI &configuration, StreamString &errStream, const bool sendPostMsg) {
    ErrorManagement::ErrorType ret;
    if (!firstLoading) {
        ret = SendConfigurationMessage(preConfigMsg);
    }
    if (ret.ErrorsCleared()) {
        Reference keepAlive = this;
        uint32 nOfObjs = ObjectRegistryDatabase::Instance()->Size();
        REPORT_ERROR(ErrorManagement::Debug, "Purging ObjectRegistryDatabase with %d objects", nOfObjs);
        ObjectRegistryDatabase::Instance()->Purge();
        nOfObjs = ObjectRegistryDatabase::Instance()->Size();
        REPORT_ERROR(ErrorManagement::Debug, "Purge ObjectRegistryDatabase. Number of objects left: %d", nOfObjs);
        ret.fatalError = !configuration.MoveToRoot();
        //lint -e{1788} keepAlive variable is just to make sure this class is not removed...
    }
    if (ret.ErrorsCleared()) {
        ret.initialisationError = !ObjectRegistryDatabase::Instance()->Initialise(configuration);
        if (!ret.ErrorsCleared()) {
            (void)errStream.Printf("%s", "Failed to initialise the ObjectRegistryDatabase");
            REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, errStream.Buffer());
        }
    }
    if (ret.ErrorsCleared()) {
        ret.fatalError = !configuration.MoveToRoot();
    }
    if (ret.ErrorsCleared()) {
        ret.fatalError = !parsedConfiguration.MoveToRoot();
    }
    
    bool sendFailedConfigMsg = !reloadLast;
    bool applicationReloaded = false;
    if (firstLoading) {
        if (ret.ErrorsCleared()) {
            parsedConfiguration.Purge();
            ret.fatalError = !configuration.Copy(parsedConfiguration);
        }
    }
    else {
        if (ret.ErrorsCleared()) {
            ret = PostReconfigure();
        }
        if (ret.ErrorsCleared()) {//If !firstLoading only overwrite the last configuration here.
            parsedConfiguration.Purge();
            ret.fatalError = !configuration.Copy(parsedConfiguration);
        }
        if (!ret.ErrorsCleared()) { //Error occurred
            if (reloadLast) {//Attempt to reload the last configuration?
                REPORT_ERROR_STATIC(ErrorManagement::Information, "Reloading last valid configuration");
                ErrorManagement::ErrorType retReload = ReloadLastValidConfiguration();
                if (retReload.ErrorsCleared()) {
                    retReload = PostReconfigure();
                }
                applicationReloaded = retReload.ErrorsCleared();
                if (!retReload.ErrorsCleared()) {
                    sendFailedConfigMsg = true;
                }
            }
        }
        //Always reload the keep alive objects
        for (uint32 c=0u; c<keepAliveObjs.Size(); c++) {
            bool retIns = ObjectRegistryDatabase::Instance()->Insert(keepAliveObjs.Get(c));
            if (retIns) {
                REPORT_ERROR_STATIC(ErrorManagement::Information, "Inserted keep alive object with name %s", keepAliveObjs.Get(c)->GetName());
            }
            else {
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "Could not insert keep alive object with name %s", keepAliveObjs.Get(c)->GetName());
            }
        }
        if (!ret.ErrorsCleared()) { //ErrorOccurred. Send messages?
            if (sendFailedConfigMsg) {
                (void) SendConfigurationMessage(failedConfigMsg);
            }
            else {
                if (reloadLast) {
                    (void) SendConfigurationMessage(reloadedConfigurationMsg);
                }
            }
        }
    }
    //Send the post after the keep alive objects are added again (and iff there were no errors)
    if (ret.ErrorsCleared() || applicationReloaded) {
        if (sendPostMsg) {
            ret = SendConfigurationMessage(postConfigMsg);
        }
        if (ret.ErrorsCleared()) {
            if (applicationReloaded) {
                ret.recoverableError = true;
            }
        }
    }
 
    firstLoading = false;
    return ret; 
}

ErrorManagement::ErrorType Loader::Reconfigure(StreamString &configuration, StreamString &errStream, uint32 hash) {
    ErrorManagement::ErrorType err;
    err.fatalError = !loaderHash.IsValid();
    if (err.ErrorsCleared()) {
        //lint -e{864} no dependency in the order how the configuration variable is used.
        uint32 computedHash = loaderHash->ComputeHash(configuration.Buffer(), static_cast<uint32>(configuration.Size()));
        err.fatalError = (computedHash != hash);
        if (!err.ErrorsCleared()) {
            (void)errStream.Printf("Computed hash (%d) does not match offered hash (%d)", computedHash, hash);
            REPORT_ERROR(ErrorManagement::FatalError, errStream.Buffer());
        }
    }
    if (err.ErrorsCleared()) {
        (void)configuration.Seek(0LLU);
        err = Reconfigure(configuration, errStream);
    }
    return err;
}

uint32 Loader::GetSeed() {
    uint32 ret = 0u;
    if (loaderHash.IsValid()) {
        ret = loaderHash->GetSeed();
    }
    return ret; 
}

ErrorManagement::ErrorType Loader::ReloadLastValidConfiguration() {
    ErrorManagement::ErrorType ret;
    ObjectRegistryDatabase::Instance()->Purge();
    if (ret.ErrorsCleared()) {
        ret.fatalError = !parsedConfiguration.MoveToRoot();
    }
    if (ret.ErrorsCleared()) {
        ret.initialisationError = !ObjectRegistryDatabase::Instance()->Initialise(parsedConfiguration);
    }
    return ret;
}

ErrorManagement::ErrorType Loader::GetLastValidConfiguration(ConfigurationDatabase &output) {
    ErrorManagement::ErrorType ret;
    ret.fatalError = !parsedConfiguration.MoveToRoot();
    if (ret.ErrorsCleared()) {
        ret.fatalError = !parsedConfiguration.Copy(output);
    }
    return ret;
}

ErrorManagement::ErrorType Loader::PostInit() {
    ErrorManagement::ErrorType err;
    ReferenceT<ReferenceContainer> rc = ObjectRegistryDatabase::Instance()->Find("LoaderPostInit");
    if (rc.IsValid()) {
        postInitParameters = rc->Find("Parameters");
        if (postInitParameters.IsValid()) {
            StreamString reloadLastStr;
            if (postInitParameters->Read("ReloadLast", reloadLastStr)) {
                if (reloadLastStr == "true") {
                    reloadLast = true;
                }
                else if (reloadLastStr == "false") {
                    reloadLast = false;
                }
                else {
                    err.parametersError = true;
                    REPORT_ERROR(err, "ReloadLast shall be either set to true or false. %s is not supported.", reloadLastStr.Buffer());
                }
            }
            AnyType arrayDescription = postInitParameters->GetType("KeepAlive");
            if(arrayDescription.GetDataPointer() != NULL_PTR(void *)) {
                uint32 numberOfElements = arrayDescription.GetNumberOfElements(0u);
                if (numberOfElements > 0u) {
                    StreamString *keepAliveArray = new StreamString[numberOfElements];
                    Vector<StreamString> keepAliveVector(keepAliveArray, numberOfElements);
                    err.parametersError = !postInitParameters->Read("KeepAlive", keepAliveVector);
                    for (uint32 z=0u; (err.ErrorsCleared()) && (z<numberOfElements); z++) {
                        Reference r = ObjectRegistryDatabase::Instance()->Find(keepAliveArray[z].Buffer());
                        err.parametersError = !r.IsValid();
                        if (err.ErrorsCleared()) {
                            err.fatalError = !keepAliveObjs.Insert(r);
                        }
                        else {
                            REPORT_ERROR(err, "Object with name %s not found.", keepAliveArray[z].Buffer());
                        }
                    }
                    delete [] keepAliveArray;
                }
            }
        }
        if (err.ErrorsCleared()) {
            ReferenceT<ConfigurationLoaderHashI> hashI = rc->Find("Hash");
            loaderHash = hashI;
        }
        ReferenceT<ReferenceContainer> msgs = rc->Find("Messages");
        if (msgs.IsValid()) {
            //Load all the Messages
            //lint -e{9007} no side effects in msgs->Size()
            for (uint32 j=0u; (err.ErrorsCleared()) && (j<msgs->Size()); j++) {
                ReferenceT <Message> msg = msgs->Get(j);
                err.parametersError = !msg.IsValid();
                if (err.ErrorsCleared()) {
                    StreamString msgName = msg->GetName();
                    if (msgName == "PreConfiguration") {
                        preConfigMsg = msg;
                    }
                    else if (msgName == "PostConfiguration") {
                        postConfigMsg = msg;
                    }
                    else if (msgName == "FailedConfiguration") {
                        failedConfigMsg = msg;
                    }
                    else if (msgName == "ReloadedConfiguration") {
                        reloadedConfigurationMsg = msg;
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::Information, "Using custom Message with name %s.", msgName.Buffer());
                    }
                }
                else {
                    err.parametersError = true;
                    REPORT_ERROR(err, "Found an invalid Message in container %s", msgs->GetName());
                }
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::Information, "LoaderPostInit not set");
    }
    return err;
}

/*lint -e{1762} Member function cannot be made const */
ErrorManagement::ErrorType Loader::SendConfigurationMessage(ReferenceT<Message> msg) {
    ErrorManagement::ErrorType err;
    if (msg.IsValid()) {
        msg->SetAsReply(false);
        err = MessageI::SendMessageAndWaitReply(msg, this);
        if (!err.ErrorsCleared()) {
            StreamString destination = msg->GetDestination();
            StreamString function = msg->GetFunction();
            REPORT_ERROR(ErrorManagement::FatalError, "Could not send message to %s [%s]", destination.Buffer(), function.Buffer());
        }
    }
    return err;
}

ErrorManagement::ErrorType Loader::Start() {
    ErrorManagement::ErrorType ret;
    if (messageDestination.Size() > 0u) {
        ReferenceT<Message> message(new Message());
        ConfigurationDatabase msgConfig;
        ret.parametersError = !msgConfig.Write("Destination", messageDestination);
        if (ret.ErrorsCleared()) {
            ret.initialisationError = !msgConfig.Write("Function", messageFunction);
        }
        if (ret.ErrorsCleared()) {
            ret.initialisationError = !message->Initialise(msgConfig);
        }
        if (ret.ErrorsCleared()) {
            ret = MessageI::SendMessage(message);
        }

    }

    //////////////////////////////////
    return ret;
}

ErrorManagement::ErrorType Loader::Stop() {
    return ErrorManagement::NoError;
}

ErrorManagement::ErrorType Loader::PostReconfigure() {
    //NOOP
    return ErrorManagement::NoError;
}

CLASS_REGISTER(Loader, "")

}

