/**
 * @file TimesDataSource.cpp
 * @brief Source file for class TimesDataSource
 * @date 01/ago/2016
 * @author pc
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
 * the class TimesDataSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TimesDataSource.h"
#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

TimesDataSource::TimesDataSource() :
        GAMDataSource() {
}

TimesDataSource::~TimesDataSource() {
}

bool TimesDataSource::SetConfiguredDatabase(StructuredDataI & data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);
    uint32 nSignals = GetNumberOfSignals();
    uint32 nStates = 0u;
    uint32 n;
    for (n = 0u; (n < nSignals) && (ret); n++) {
        ret = GetSignalNumberOfStates(n, nStates);
        uint32 s;
        for (s = 0u; (s < nStates) && (ret); s++) {
            uint32 nProducers = 0u;
            StreamString stateName;
            ret = GetSignalStateName(n, s, stateName);
            if (ret) {
                ret = !GetSignalNumberOfProducers(n, stateName.Buffer(), nProducers);
                if (!ret) {
                    StreamString signalName;
                    if (!GetSignalName(n, signalName)) {
                        signalName = "";
                    }
                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "In TimesDataSource %s, state %s, signal %s has a producer", GetName(),
                                            stateName.Buffer(), signalName.Buffer())
                }

            }
        }
    }
    return ret;
}

CLASS_REGISTER(TimesDataSource, "1.0")

}
