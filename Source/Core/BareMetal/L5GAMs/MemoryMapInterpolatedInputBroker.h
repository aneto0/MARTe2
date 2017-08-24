/**
 * @file MemoryMapInterpolatedInputBroker.h
 * @brief Header file for class MemoryMapInterpolatedInputBroker
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

 * @details This header file contains the declaration of the class MemoryMapInterpolatedInputBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L5GAMS_MEMORYMAPINTERPOLATEDINPUTBROKER_H_
#define L5GAMS_MEMORYMAPINTERPOLATEDINPUTBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryMapInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Input MemoryMapBroker implementation which allows to generate interpolated samples.
 * @details This class interpolates the signals from the DataSourceI and copies to the GAM memory.
 * TODO add more details
 */
class DLL_API MemoryMapInterpolatedInputBroker: public MemoryMapBroker {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor. NOOP.
     */
MemoryMapInterpolatedInputBroker    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~MemoryMapInterpolatedInputBroker();

    /**
     * @brief TODO Sequentially copies all the signals from the DataSourceI memory to the GAM
     *  memory.
     * @return TODO true if all copies are successfully performed.
     */
    virtual bool Execute();

    /**
     * TODO
     */
    virtual bool Init(const SignalDirection direction,
            DataSourceI &dataSourceIn,
            const char8 * const functionName,
            void * const gamMemoryAddress);

    /**
     * TODO
     */
    void SetTimeSignal(void *timeSignalIn, TypeDescriptor timeSignalTypeIn, float64 interpolationPeriodIn);

    /**
     * TODO
     */
    void InitSegments();

private:
    /**
     * Interpolate between samples for any signal
     */
    template<typename valueType>
    void Interpolate(uint32 copyIdx);

    /**
     * Initialise the linear interpolation segment
     */
    template<typename valueType>
    void InitSegment(uint32 copyIdx, float64 dt);

    /**
     * TODO
     */
    void UpdateTimes();

    /**
     * The interpolation period in seconds
     */
    float64 interpolationPeriod;

    /**
     * The current time in seconds
     */
    float64 currentTime;

    /**
     * Execution counter
     */
    uint64 executionCounter;

    /**
     * The current time signal memory address;
     */
    void *timeSignal;

    /**
     * The time signal type
     */
    TypeDescriptor timeSignalType;

    /**
     * The beginning time of the interpolation segment
     */
    float64 t0;

    /**
     * The ending time of the interpolation segment
     */
    float64 t1;

    /**
     * Pointers to the addresses of the y0 values
     */
    void **y0;

    /**
     * Pointers to the addresses of the y1 values
     */
    void **y1;

    /**
     * Pointers to the addresses of the m values
     */
    void **m;

    /**
     * Accelerator for number of elements in any given signal
     */
    uint32 *numberOfElements;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {

#include <stdio.h>
template<typename valueType>
void MemoryMapInterpolatedInputBroker::Interpolate(uint32 copyIdx) {
uint32 i;
for (i = 0u; i < numberOfElements[copyIdx]; i++) {
    valueType *y0p = (valueType *) (y0[copyIdx]);
    float64 y = static_cast<float64>(y0p[i]);

    valueType *mcp = (valueType *) (m[copyIdx]);
    float64 mc = static_cast<float64>(mcp[i]);
    y += mc * currentTime;
    printf("%e %e %e\n", y, mc, currentTime);

    valueType *dest = static_cast<valueType *>(copyTable[copyIdx].gamPointer);
    dest[i] = static_cast<valueType>(y);
}
}

template<typename valueType>
void MemoryMapInterpolatedInputBroker::InitSegment(uint32 copyIdx, float64 dt) {
uint32 i;
for (i = 0u; i < numberOfElements[copyIdx]; i++) {
    valueType *y0p = (valueType *) (y0[copyIdx]);
    valueType *y1p = (valueType *) (y1[copyIdx]);
    valueType *y1pds = (valueType *) (copyTable[copyIdx].dataSourcePointer);

    y0p[i] = y1p[i];
    y1p[i] = y1pds[i];

    float64 md = static_cast<float64>(y1p[i] - y0p[i]);
    valueType *mp = (valueType *) (m[copyIdx]);
    mp[i] = static_cast<valueType>(md / dt);
    printf("%e\n", mp[i]);
}
}

}

#endif /* L5GAMS_MEMORYMAPINTERPOLATEDINPUTBROKER_H_ */

