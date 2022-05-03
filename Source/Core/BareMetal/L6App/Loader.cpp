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
}

Loader::~Loader() {
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

    StreamString parserType;
    StreamString parserError;
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
        if (parserType == "xml") {
            XMLParser parser(configuration, parsedConfiguration, &parserError);
            ret.initialisationError = !parser.Parse();
        }
        else if (parserType == "json") {
            JsonParser parser(configuration, parsedConfiguration, &parserError);
            ret.initialisationError = !parser.Parse();
        }
        else if (parserType == "cdb") {
            StandardParser parser(configuration, parsedConfiguration, &parserError);
            ret.initialisationError = !parser.Parse();
        }
        else {
            ret = ErrorManagement::ParametersError;
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Unknown Parser specified");
        }
        if (!ret) {
            StreamString errPrint;
            (void) errPrint.Printf("Failed to parse %s", parserError.Buffer());
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, errPrint.Buffer());
        }
    }
    if (ret.ErrorsCleared()) {
        ret.fatalError = !parsedConfiguration.MoveToRoot();
    }
    if (ret.ErrorsCleared()) {
        ret.initialisationError = !ObjectRegistryDatabase::Instance()->Initialise(parsedConfiguration);
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Failed to initialise the ObjectRegistryDatabase");
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

CLASS_REGISTER(Loader, "")

}

