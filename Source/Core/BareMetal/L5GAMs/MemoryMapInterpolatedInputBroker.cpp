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
#include "MemoryMapInterpolatedInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
MemoryMapInterpolatedInputBroker::MemoryMapInterpolatedInputBroker() {
    interpolationPeriod = 0.F;
    executionCounter = 0LLU;
    currentTime = 0.;
    t0 = 0.;
    t1 = 0.;
    timeSignal = NULL_PTR(void *);
    m = NULL_PTR(void **);
    y0 = NULL_PTR(void **);
    y1 = NULL_PTR(void **);
    numberOfElements = NULL_PTR(uint32 *);
    timeSignalType = VoidType;
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
    if (m != NULL_PTR(void **)) {
        uint32 i;
        for (i = 0; i < numberOfCopies; i++) {
            GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(m[i]);
        }
        delete m;
    }
}

bool MemoryMapInterpolatedInputBroker::Init(const SignalDirection direction, DataSourceI &dataSourceIn, const char8 * const functionName,
                                            void * const gamMemoryAddress) {
    bool ok = MemoryMapBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);

    if (ok) {
        numberOfElements = new uint32[numberOfCopies];
        m = new void *[numberOfCopies];
        y0 = new void *[numberOfCopies];
        y1 = new void *[numberOfCopies];
    }
    uint32 i;
    for (i = 0; i < numberOfCopies; i++) {
        uint32 byteSize = copyTable[i].type.numberOfBits / 8u;
        numberOfElements[i] = copyTable[i].copySize;
        numberOfElements[i] /= byteSize;
        m[i] = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(copyTable[i].copySize);
        y0[i] = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(copyTable[i].copySize);
        y1[i] = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(copyTable[i].copySize);
    }

    return ok;
}

void MemoryMapInterpolatedInputBroker::SetTimeSignal(void *timeSignalIn, TypeDescriptor timeSignalTypeIn, float64 interpolationPeriodIn) {
    timeSignal = timeSignalIn;
    timeSignalType = timeSignalTypeIn;
    interpolationPeriod = interpolationPeriodIn;
}

void MemoryMapInterpolatedInputBroker::UpdateTimes() {
    t0 = t1;
    if (timeSignalType == UnsignedInteger8Bit) {
        t1 = static_cast<float64>(*(static_cast<uint8 *>(timeSignal)));
    }
    else if (timeSignalType == UnsignedInteger16Bit) {
        t1 = static_cast<float64>(*(static_cast<uint16 *>(timeSignal)));
    }
    else if (timeSignalType == UnsignedInteger32Bit) {
        t1 = static_cast<float64>(*(static_cast<uint32 *>(timeSignal)));
    }
    else if (timeSignalType == UnsignedInteger64Bit) {
        t1 = static_cast<float64>(*(static_cast<uint64 *>(timeSignal)));
    }
    else if (timeSignalType == SignedInteger8Bit) {
        t1 = static_cast<float64>(*(static_cast<int8 *>(timeSignal)));
    }
    else if (timeSignalType == SignedInteger16Bit) {
        t1 = static_cast<float64>(*(static_cast<int16 *>(timeSignal)));
    }
    else if (timeSignalType == SignedInteger32Bit) {
        t1 = static_cast<float64>(*(static_cast<int32 *>(timeSignal)));
    }
    else if (timeSignalType == SignedInteger64Bit) {
        t1 = static_cast<float64>(*(static_cast<int64 *>(timeSignal)));
    }
    else if (timeSignalType == Float32Bit) {
        t1 = static_cast<float64>(*(static_cast<float32 *>(timeSignal)));
    }
    else if (timeSignalType == Float64Bit) {
        t1 = *(static_cast<float64 *>(timeSignal));
    }
    else {
//TODO log invalid time signal type
    }

}

void MemoryMapInterpolatedInputBroker::InitSegments() {
    uint32 i;
    float64 dt;
    if (executionCounter == 0LLU) {
        //kick-start the first assignment of y0
        dt = 1.0;
    }
    else {
        dt = (t1 - t0);
    }
    if (dt > 0.F) {
        for (i = 0; (i < numberOfCopies); i++) {
            if (copyTable[i].type == UnsignedInteger8Bit) {
                InitSegment<uint8>(i, dt);
            }
            else if (copyTable[i].type == UnsignedInteger16Bit) {
                InitSegment<uint16>(i, dt);
            }
            else if (copyTable[i].type == UnsignedInteger32Bit) {
                InitSegment<uint32>(i, dt);
            }
            else if (copyTable[i].type == UnsignedInteger64Bit) {
                InitSegment<uint64>(i, dt);
            }
            else if (copyTable[i].type == SignedInteger8Bit) {
                InitSegment<int8>(i, dt);
            }
            else if (copyTable[i].type == SignedInteger16Bit) {
                InitSegment<int16>(i, dt);
            }
            else if (copyTable[i].type == SignedInteger32Bit) {
                InitSegment<int32>(i, dt);
            }
            else if (copyTable[i].type == SignedInteger64Bit) {
                InitSegment<int64>(i, dt);
            }
            else if (copyTable[i].type == Float32Bit) {
                InitSegment<float32>(i, dt);
            }
            else if (copyTable[i].type == Float64Bit) {
                InitSegment<float64>(i, dt);
            }
        }
    }
}

bool MemoryMapInterpolatedInputBroker::Execute() {
    bool ok = true;
    uint32 i;
    if (executionCounter == 0LLU) {
        InitSegments();
    }
    if (ok) {
        executionCounter++;
        currentTime = static_cast<float64>(executionCounter) * interpolationPeriod;
        while (currentTime > t1) {
            //TODO need to check for infinite loop and exit with error
            ok = dataSource->Synchronise();
            UpdateTimes();
            InitSegments();
        }
    }
    for (i = 0; i < numberOfCopies; i++) {
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

