/**
 * @file RealTimeSampledData.cpp
 * @brief Source file for class RealTimeSampledData
 * @date 19/feb/2016
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
 * the class RealTimeSampledData (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeSampledData.h"
#include "RealTimeDataSource.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

RealTimeSampledData::RealTimeSampledData() {
    samples = 1;
    cycles = 1;

}

bool RealTimeSampledData::Verify() {
    bool ret = false;

    // check myself
    if (path != "") {
        StreamString typePath = path + ".Type";
        RealTimeDataSource *dataSource = RealTimeDataSource::Instance();
        StreamString testType;
        if (dataSource->Read(typePath.Buffer(), testType)) {
            if (testType == type) {
                ret = true;
            }
        }

        if (ret) {
            ret = false;
            StreamString samplesPath = path + ".Samples";
            RealTimeDataSource *dataSource = RealTimeDataSource::Instance();
            StreamString testSamples;
            if (dataSource->Read(samplesPath.Buffer(), testSamples)) {
                if (testSamples == samples) {
                    ret = true;
                }
            }
        }

        if (ret) {
            ret = false;
            StreamString cyclesPath = path + ".Cycles";
            RealTimeDataSource *dataSource = RealTimeDataSource::Instance();
            StreamString testCycles;
            if (dataSource->Read(cyclesPath.Buffer(), testCycles)) {
                if (testCycles == cycles) {
                    ret = true;
                }
            }
        }
    }

    return ret;
}

bool RealTimeSampledData::Initialise(StructuredDataI &data) {
    bool ret = data.Read("Path", path);

    if (ret) {
        ret = data.Read("Type", type);
    }

    if (ret) {
        ret = data.Read("Samples", samples);
    }

    if (ret) {
        ret = data.Read("Cycles", cycles);
    }
    return ret;
}
