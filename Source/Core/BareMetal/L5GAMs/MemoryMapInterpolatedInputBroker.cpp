/**
 * @file MemoryMapInterpolatedInputBroker.cpp
 * @brief Source file for class MemoryMapInterpolatedInputBroker
 * @date 24/08/2017
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
 * the class MemoryMapInterpolatedInputBroker (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MemoryMapInterpolatedInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
MemoryMapInterpolatedInputBroker::MemoryMapInterpolatedInputBroker() {
    t0 = 0LLU;
    t1 = 0LLU;
    dataSourceTime = 0LLU;
    interpolationPeriod = 0LLU;
    dataSourceTime = NULL_PTR(uint64 *);
    interpolatedTime = NULL_PTR(uint64 *);
    m = NULL_PTR(float64 *);
    y0 = NULL_PTR(void **);
    y1 = NULL_PTR(void **);
    numberOfElements = NULL_PTR(uint32 *);
}

MemoryMapInterpolatedInputBroker::~MemoryMapInterpolatedInputBroker() {
    if (numberOfElements != NULL_PTR(uint32 *)) {
        delete numberOfElements;
    }
    if (y0 != NULL_PTR(void **)) {
        uint32 i;
        for (i = 0; i < numberOfCopies; i++) {
            GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(y0[i]);
        }
        delete y0;
    }
    if (y1 != NULL_PTR(void **)) {
        uint32 i;
        for (i = 0; i < numberOfCopies; i++) {
            GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(y1[i]);
        }
        delete y1;
    }
    if (m != NULL_PTR(float64 *)) {
        delete m;
    }
}

bool MemoryMapInterpolatedInputBroker::Init(const SignalDirection direction, DataSourceI &dataSourceIn, const char8 * const functionName,
                                            void * const gamMemoryAddress) {
    bool ok = MemoryMapBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);

    if (ok) {
        numberOfElements = new uint32[numberOfCopies];
        m = new float64[numberOfCopies];
        y0 = new void *[numberOfCopies];
        y1 = new void *[numberOfCopies];
    }
    uint32 i;
    for (i = 0; i < numberOfCopies; i++) {
        uint32 byteSize = copyTable[i].type.numberOfBits / 8u;
        numberOfElements[i] = copyTable[i].copySize;
        numberOfElements[i] /= byteSize;
        m[i] = 0;
        y0[i] = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(copyTable[i].copySize);
        y1[i] = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(copyTable[i].copySize);
    }

    return ok;
}

void MemoryMapInterpolatedInputBroker::SetTimeSignal(const uint64 * const dataSourceTimeIn, uint64 * const interpolatedTimeIn, const uint64 interpolationPeriodIn) {
    dataSourceTime = dataSourceTimeIn;
    interpolatedTime = interpolatedTimeIn;
    interpolationPeriod = interpolationPeriodIn;
}

void MemoryMapInterpolatedInputBroker::ChangeInterpolationWindows() {
    t0 = t1;
    t1 = *dataSourceTime;
    uint32 i;
    float64 dt;
    if (t1 == t0) {
        //kick-start the first assignment of y0
        dt = 1.0;
    }
    else {
        dt = static_cast<float64>(t1 - t0);
        dt /= 1e9;
    }
    if (dt > 0.F) {
        for (i = 0; (i < numberOfCopies); i++) {
            if (copyTable[i].type == UnsignedInteger8Bit) {
                ChangeInterpolationWindow<uint8>(i, dt);
            }
            else if (copyTable[i].type == UnsignedInteger16Bit) {
                ChangeInterpolationWindow<uint16>(i, dt);
            }
            else if (copyTable[i].type == UnsignedInteger32Bit) {
                ChangeInterpolationWindow<uint32>(i, dt);
            }
            else if (copyTable[i].type == UnsignedInteger64Bit) {
                ChangeInterpolationWindow<uint64>(i, dt);
            }
            else if (copyTable[i].type == SignedInteger8Bit) {
                ChangeInterpolationWindow<int8>(i, dt);
            }
            else if (copyTable[i].type == SignedInteger16Bit) {
                ChangeInterpolationWindow<int16>(i, dt);
            }
            else if (copyTable[i].type == SignedInteger32Bit) {
                ChangeInterpolationWindow<int32>(i, dt);
            }
            else if (copyTable[i].type == SignedInteger64Bit) {
                ChangeInterpolationWindow<int64>(i, dt);
            }
            else if (copyTable[i].type == Float32Bit) {
                ChangeInterpolationWindow<float32>(i, dt);
            }
            else if (copyTable[i].type == Float64Bit) {
                ChangeInterpolationWindow<float64>(i, dt);
            }
        }
    }
}

void MemoryMapInterpolatedInputBroker::Reset() {
    ChangeInterpolationWindows();
    *interpolatedTime = *dataSourceTime;
}

bool MemoryMapInterpolatedInputBroker::Execute() {
    bool ok = true;
    uint32 i;
    *interpolatedTime += interpolationPeriod;
    bool triggerChange = false;
    while ((ok) && (*interpolatedTime > *dataSourceTime)) {
        triggerChange = true;
        uint64 lastDataSourceTime = *dataSourceTime;
        ok = dataSource->Synchronise();
        if (ok) {
            //Time is not changing!
            ok = (lastDataSourceTime != *dataSourceTime);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "DataSource time is not changing. Current dataSourceTime is %d", *dataSourceTime);
        }
    }
    if ((ok) && (triggerChange)) {
        ChangeInterpolationWindows();
    }

    for (i = 0; (i < numberOfCopies) && (ok); i++) {
        if (copyTable[i].type == UnsignedInteger8Bit) {
            Interpolate<uint8>(i);
        }
        else if (copyTable[i].type == UnsignedInteger16Bit) {
            Interpolate<uint16>(i);
        }
        else if (copyTable[i].type == UnsignedInteger32Bit) {
            Interpolate<uint32>(i);
        }
        else if (copyTable[i].type == UnsignedInteger64Bit) {
            Interpolate<uint64>(i);
        }
        else if (copyTable[i].type == SignedInteger8Bit) {
            Interpolate<int8>(i);
        }
        else if (copyTable[i].type == SignedInteger16Bit) {
            Interpolate<int16>(i);
        }
        else if (copyTable[i].type == SignedInteger32Bit) {
            Interpolate<int32>(i);
        }
        else if (copyTable[i].type == SignedInteger64Bit) {
            Interpolate<int64>(i);
        }
        else if (copyTable[i].type == Float32Bit) {
            Interpolate<float32>(i);
        }
        else if (copyTable[i].type == Float64Bit) {
            Interpolate<float64>(i);
        }

    }

    return ok;
}
CLASS_REGISTER(MemoryMapInterpolatedInputBroker, "1.0")

}

