/**
 * @file TimingDataSource.cpp
 * @brief Source file for class TimingDataSource
 * @date 01/08/2016
 * @author Giuseppe Ferro
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 REPORT_ERROR_PARAMETERS_OBJby the European Commission - subsequent versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class TimingDataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TimingDataSource.h"

#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

TimingDataSource::TimingDataSource() :
        GAMDataSource() {
}

TimingDataSource::~TimingDataSource() {
}

bool TimingDataSource::SetConfiguredDatabase(StructuredDataI & data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);
    uint32 nSignals = GetNumberOfSignals();
    uint32 nStates = 0u;
    uint32 n;
    for (n = 0u; (n < nSignals) && (ret); n++) {
        ret = GetSignalNumberOfStates(n, nStates);
        uint32 s;
        for (s = 0u; (s < nStates) && (ret); s++) {
            uint32 nProducers = 0u;
            DynamicCString stateName;
            ret = GetSignalStateName(n, s, stateName);
            if (ret) {
                ret = !GetSignalNumberOfProducers(n, stateName, nProducers);
                if (!ret) {
                	DynamicCString signalName;
                    if (!GetSignalName(n, signalName)) {
                        signalName = "";
                    }
                    COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError, "In TimingDataSource ",GetName()," state ",stateName," signal ",signalName," has a producer");
                }

            }
        }
    }
    return ret;
}

CLASS_REGISTER(TimingDataSource, "1.0")

}
