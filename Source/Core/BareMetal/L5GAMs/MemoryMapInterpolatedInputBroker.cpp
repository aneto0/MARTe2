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
MemoryMapInterpolatedInputBroker::MemoryMapInterpolatedInputBroker() :
        MemoryMapBroker() {
    x0 = 0LLU;
    x1 = 0LLU;
    interpolationPeriod = 0LLU;
    interpolatedXAxis = 0LLU;
    reset = false;
    dataSourceXAxis = NULL_PTR(uint64*);
    m = NULL_PTR(float64*);
    y0 = NULL_PTR(void**);
    y1 = NULL_PTR(void**);
    numberOfElements = NULL_PTR(uint32*);
}

/*lint -e{1551} memory is freed in the destructor*/
MemoryMapInterpolatedInputBroker::~MemoryMapInterpolatedInputBroker() {
    if (numberOfElements != NULL_PTR(uint32*)) {
        delete [] numberOfElements;
    }
    if (y0 != NULL_PTR(void**)) {
        uint32 i;
        for (i = 0u; i < numberOfCopies; i++) {
            GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(y0[i]);
        }
        delete [] y0;
    }
    if (y1 != NULL_PTR(void**)) {
        uint32 i;
        for (i = 0u; i < numberOfCopies; i++) {
            GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(y1[i]);
        }
        delete [] y1;
    }
    if (m != NULL_PTR(float64*)) {
        delete [] m;
    }
    /*lint -e{1740} the dataSourceXAxis is freed by the DataSourceI*/
}

bool MemoryMapInterpolatedInputBroker::Init(const SignalDirection direction,
                                            DataSourceI &dataSourceIn,
                                            const char8 *const functionName,
                                            void *const gamMemoryAddress) {
    bool ok = MemoryMapBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);

    if (ok) {
        numberOfElements = new uint32[numberOfCopies];
        m = new float64[numberOfCopies];
        y0 = new void*[numberOfCopies];
        y1 = new void*[numberOfCopies];
    }
    uint32 i;
    /*lint -e{613} copyTable cannot be NULL as otherwise MemoryMapBroker::Init would have failed => ok = false*/
    for (i = 0u; (i < numberOfCopies) && (ok); i++) {
        uint32 byteSize = static_cast<uint32>(copyTable[i].type.numberOfBits);
        byteSize /= 8u;
        numberOfElements[i] = copyTable[i].copySize;
        numberOfElements[i] /= byteSize;
        m[i] = 0.;
        y0[i] = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(copyTable[i].copySize);
        y1[i] = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(copyTable[i].copySize);
        ok = MemoryOperationsHelper::Set(y1[i], '\0', copyTable[i].copySize);
    }

    return ok;
}

void MemoryMapInterpolatedInputBroker::SetIndependentVariable(const uint64 *const dataSourceXAxisIn,
                                                              const uint64 interpolationPeriodIn) {
    dataSourceXAxis = dataSourceXAxisIn;
    interpolationPeriod = interpolationPeriodIn;
}

/*lint -e{613} copyTable should be NULL as otherwise MemoryMapBroker::Init would have failed => ok = false and this function should not be called*/
void MemoryMapInterpolatedInputBroker::ChangeInterpolationSegments() {
    if (dataSourceXAxis != NULL_PTR(uint64*)) {
        x0 = x1;
        x1 = *dataSourceXAxis;
        uint32 i;
        uint64 dt;
        if (x1 == x0) {
            //kick-start the first assignment of y0
            dt = 1LLU;
        }
        else {
            dt = (x1 - x0);
        }

        for (i = 0u; (i < numberOfCopies); i++) {
            if (copyTable[i].type == UnsignedInteger8Bit) {
                ChangeInterpolationSegment<uint8>(i, dt);
            }
            else if (copyTable[i].type == UnsignedInteger16Bit) {
                ChangeInterpolationSegment<uint16>(i, dt);
            }
            else if (copyTable[i].type == UnsignedInteger32Bit) {
                ChangeInterpolationSegment<uint32>(i, dt);
            }
            else if (copyTable[i].type == UnsignedInteger64Bit) {
                ChangeInterpolationSegment<uint64>(i, dt);
            }
            else if (copyTable[i].type == SignedInteger8Bit) {
                ChangeInterpolationSegment<int8>(i, dt);
            }
            else if (copyTable[i].type == SignedInteger16Bit) {
                ChangeInterpolationSegment<int16>(i, dt);
            }
            else if (copyTable[i].type == SignedInteger32Bit) {
                ChangeInterpolationSegment<int32>(i, dt);
            }
            else if (copyTable[i].type == SignedInteger64Bit) {
                ChangeInterpolationSegment<int64>(i, dt);
            }
            else if (copyTable[i].type == Float32Bit) {
                ChangeInterpolationSegment<float32>(i, dt);
            }
            else if (copyTable[i].type == Float64Bit) {
                ChangeInterpolationSegment<float64>(i, dt);
            }
            else {
                //Unreachable?
            }
        }
    }
}

void MemoryMapInterpolatedInputBroker::Reset() {
    bool ok = (dataSourceXAxis != NULL_PTR(uint64*));
    if (ok) {
        ChangeInterpolationSegments();
        interpolatedXAxis = *dataSourceXAxis;
        reset = true;
    }
}

/*lint -e{613} copyTable should be NULL as otherwise MemoryMapBroker::Init would have failed => ok = false and this function should not be called*/
bool MemoryMapInterpolatedInputBroker::Execute() {
    bool ok = (dataSourceXAxis != NULL_PTR(uint64*));
    uint32 i;
    if (reset) {
        //Just copy the first data point after a reset
        for (i = 0u; (i < numberOfCopies) && (ok); i++) {
            if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry*)) {
                ok = MemoryOperationsHelper::Copy(copyTable[i].gamPointer, copyTable[i].dataSourcePointer, copyTable[i].copySize);
            }
        }
        reset = false;
    }
    else {
        /*lint -e{613} NULL pointer checked above*/
        interpolatedXAxis += interpolationPeriod;
        //Do interpolation and if needed compute next segment.
        bool triggerChange = false;
        if (ok) {
            /*lint -e{613} NULL pointer checked above*/
            while ((ok) && (interpolatedXAxis > *dataSourceXAxis)) {
                triggerChange = true;
                uint64 lastDataSourceTime = *dataSourceXAxis;
                ok = dataSource->Synchronise();
                if (ok) {
                    //Time is not changing!
                    ok = (lastDataSourceTime != *dataSourceXAxis);
                }
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::FatalError, "DataSource x-axis is not changing (Have you called Reset?). Current dataSourceXAxis is %d",
                                 *dataSourceXAxis);
                }
            }
        }
        if ((ok) && (triggerChange)) {
            ChangeInterpolationSegments();
        }

        for (i = 0u; (i < numberOfCopies) && (ok); i++) {
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
            else {
                //Unreachable/unsupported type? Should be stopped by the DataSourceI before...
            }
        }
    }

    return ok;
}
CLASS_REGISTER(MemoryMapInterpolatedInputBroker, "1.0")

}

