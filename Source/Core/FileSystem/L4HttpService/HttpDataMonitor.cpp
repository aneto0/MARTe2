/**
 * @file HttpDataMonitor.cpp
 * @brief Source file for class HttpDataMonitor
 * @date 01/08/2025
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

 * @details This source file contains the definition of all the methods for
 * the class HttpDataMonitor (public, protected, and private). Be aware that some
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
#include "HttpChunkedStream.h"
#include "HttpDataMonitor.h"
#include "HttpDefinition.h"
#include "ObjectRegistryDatabase.h"
#include "StreamStructuredData.h"
#include "JsonPrinter.h"
#include "HighResolutionTimer.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

HttpDataMonitor::HttpDataMonitor() :
        ReferenceContainer(),
        HttpDataExportI() {
    plotName = NULL;
    componentPath = NULL;
    component=NULL;
    signalName = NULL;
    fullSignalName = NULL;
    numberOfPlots = 0u;
    numberOfSignals = NULL;
    configured = false;
    initialMillis = 0ull;
}

HttpDataMonitor::~HttpDataMonitor() {
    if (signalName != NULL) {
        for(uint32 i=0u; i<numberOfPlots; i++) {
            if(signalName[i]!=NULL) {
                delete [] signalName[i];
            }
        }
        delete [] signalName;
    }

    if (fullSignalName != NULL) {
        for(uint32 i=0u; i<numberOfPlots; i++) {
            if(fullSignalName[i]!=NULL) {
                delete [] fullSignalName[i];
            }
        }
        delete [] fullSignalName;
    }

    if (numberOfSignals != NULL) {
        delete [] numberOfSignals;
    }
    if (plotName != NULL) {
        delete [] plotName;
    }
    if (componentPath != NULL) {
        for(uint32 i=0u; i<numberOfPlots; i++) {
            if(componentPath[i]!=NULL) {
                delete [] componentPath[i];
            }
        }
        delete [] componentPath;
    }
    if (component != NULL) {
        for(uint32 i=0u; i<numberOfPlots; i++) {
            if(component[i]!=NULL) {
                delete [] component[i];
            }
        }
        delete [] component;
    }
}

bool HttpDataMonitor::Initialise(StructuredDataI &data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        //discover first
        numberOfPlots = 0u;
        for (uint32 i = 0u; (i < data.GetNumberOfChildren()) && ret; i++) {
            if (data.MoveRelative(data.GetChildName(i))) {
                numberOfPlots++;
                data.MoveToAncestor(1u);
            }
        }

        plotName = new StreamString[numberOfPlots];
        component = new Reference*[numberOfPlots];
        signalName = new StreamString*[numberOfPlots];
        fullSignalName = new StreamString*[numberOfPlots];
        componentPath = new StreamString*[numberOfPlots];
        numberOfSignals = new uint32[numberOfPlots];

        for (uint32 i = 0u; (i < numberOfPlots) && ret; i++) {
            component[i] = NULL;
            signalName[i] = NULL;
            fullSignalName[i] = NULL;
            componentPath[i] = NULL;
            numberOfSignals[i] = 0u;
        }

        uint32 n = 0u;
        for (uint32 i = 0u; (i < data.GetNumberOfChildren()) && ret; i++) {
            const char8 *childName = data.GetChildName(i);
            if (data.MoveRelative(childName)) {
                plotName[n] = childName;
                {
                    numberOfSignals[n] = data.GetNumberOfChildren();
                    signalName[n] = new StreamString[numberOfSignals[n]];
                    componentPath[n] = new StreamString[numberOfSignals[n]];
                    component[n] = new Reference[numberOfSignals[n]];
                    fullSignalName[n] = new StreamString[numberOfSignals[n]];
                    for (uint32 j = 0u; (j < data.GetNumberOfChildren()) && ret; j++) {
                        fullSignalName[n][j] = data.GetChildName(j);
                        ret = data.MoveRelative(fullSignalName[n][j].Buffer());
                        if (ret) {
                            ret = data.Read("Signal", signalName[n][j]);
                            if (ret) {
                                ret = data.Read("Component", componentPath[n][j]);
                                if (!ret) {
                                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to read Signal %d in Plot %d", j, n);
                                }
                            }
                            else{
                                REPORT_ERROR(ErrorManagement::FatalError, "Failed to read Signal %d in Plot %d", j, n);
                            }
                            data.MoveToAncestor(1u);
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::FatalError, "All plot children must be nodes");
                        }
                    }
                }
                n++;
                data.MoveToAncestor(1u);
            }
        }

        if (ret) {
            float64 timestampF = HighResolutionTimer::Counter() * HighResolutionTimer::Period();
            initialMillis = static_cast<uint64>(timestampF * 1000ull);
        }
    }
    return ret;
}

/*lint -e{613} sstream cannot be NULL as otherwise ok would be false*/
bool HttpDataMonitor::GetAsStructuredData(StreamStructuredDataI &data,
                                          HttpProtocol &protocol) {

    bool valid = true;
    if (!configured) {
        for (uint32 n = 0u; (n < numberOfPlots) && valid; n++) {
            for (uint32 i = 0u; (i < numberOfSignals[n]) && valid; i++) {
                component[n][i] = ObjectRegistryDatabase::Instance()->Find(componentPath[n][i].Buffer());
                valid = component[n][i].IsValid();
            }
        }
        configured = true;
    }
    StreamStructuredData<JsonPrinter> *sdata;
    bool ok = HttpDataExportI::GetAsStructuredData(data, protocol);
    if (ok) {
        sdata = dynamic_cast<StreamStructuredData<JsonPrinter>*>(&data);
        /*lint -e{665} StreamStructuredData<JsonPrinter> is only used to define the pointer type of the NULL_PTR*/
        ok = (sdata != NULL_PTR(StreamStructuredData<JsonPrinter>*));
    }
    if (ok) {
        //lint -e{644} sdata initialised otherwise ok would be false*/
        ok = sdata->GetPrinter()->PrintBegin();
    }
    if (ok) {
        ok = ReferenceContainer::ExportData(data);
    }
    if (ok) {
        if (data.CreateRelative("Plots")) {
            if (valid) {
                for (uint32 n = 0u; n < numberOfPlots; n++) {
                    if (data.CreateRelative(plotName[n].Buffer())) {
                        if (data.CreateRelative("Signals")) {
                            for (uint32 i = 0u; i < numberOfSignals[n]; i++) {
                                ConfigurationDatabase rawData;
                                component[n][i]->ExportData(rawData);
                                StreamString leaf = signalName[n][i];
                                StreamString dir = "";
                                const char8 *lastDot = StringHelper::SearchLastChar(signalName[n][i].Buffer(), '.');
                                if (lastDot != NULL) {
                                    leaf=&lastDot[1u];
                                    uint32 dirSize=static_cast<uint32>(lastDot-signalName[n][i].Buffer());
                                    dir=signalName[n][i];
                                    dir.SetSize(dirSize);
                                }
                                bool inDir = rawData.MoveAbsolute(dir.Buffer());
                                if (inDir || (dir.Size() == 0ull)) {
                                    //leaf is a node as well... read all signals inside
                                    if (rawData.MoveRelative(leaf.Buffer())) {
                                        for (uint32 j = 0u; j < rawData.GetNumberOfChildren(); j++) {
                                            const char8 *childName = rawData.GetChildName(j);
                                            AnyType val = rawData.GetType(childName);
                                            if (!val.IsVoid()) {
                                                StreamString signalNameX = fullSignalName[n][i];
                                                signalNameX += "@";
                                                signalNameX += childName;
                                                data.Write(signalNameX.Buffer(), val);
                                            }
                                        }
                                        rawData.MoveToAncestor(1u);
                                    }
                                    else {
                                        AnyType val = rawData.GetType(leaf.Buffer());
                                        if (!val.IsVoid()) {
                                            data.Write(fullSignalName[n][i].Buffer(), val);
                                        }
                                    }
                                }
                                rawData.MoveToRoot();
                            }
                            data.MoveToAncestor(1u);
                        }
                        float64 timestampF = HighResolutionTimer::Counter() * HighResolutionTimer::Period();
                        uint64 millis = static_cast<uint64>(timestampF * 1000ull) - initialMillis;
                        data.Write("__Timestamp", millis);
                        data.MoveToAncestor(1u);
                    }
                }
            }
            data.MoveToAncestor(1u);
        }

    }
    //Print the closing {
    if (ok) {
        ok = sdata->GetPrinter()->PrintEnd();
    }
    return ok;
}

/*lint -e{613} sstream cannot be NULL as otherwise ok would be false*/
bool HttpDataMonitor::GetAsText(StreamI &stream,
                                HttpProtocol &protocol) {
    if (!configured) {
        for (uint32 n = 0u; (n < numberOfPlots); n++) {
            for (uint32 i = 0u; (i < numberOfSignals[n]); i++) {
                component[n][i] = ObjectRegistryDatabase::Instance()->Find(componentPath[n][i].Buffer());
            }
        }
        configured = true;
    }
    bool ok = HttpDataExportI::GetAsText(stream, protocol);
    return ok;
}
CLASS_REGISTER(HttpDataMonitor, "1.0")
}
