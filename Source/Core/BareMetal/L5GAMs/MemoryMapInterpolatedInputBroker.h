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
 * @brief Input MemoryMapBroker implementation which allows to automatically interpolate samples from any DataSourceI.
 * @details This class interpolates the signals from the DataSourceI and copies the new values to the GAM memory.
 *
 * The DataSourceI shall the method SetTimeSignal and provide two nanoseconds time vectors. One where the time to be
 * interpolated is read from and another where the interpolated time is written into. The time to be interpolated shall be
 * a function with no zero derivative between any two consecutive points.
 *
 * @warning the Reset function shall be called before the first Execute.
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
     * @brief Interpolates the data for all the registered signals and copies to the GAM memory.
     * @details Interpolates between consecutive samples. While the current time (i.e. the time that is being interpolated) is greater than the last time
     * read from the data source a new interpolation segment is created. To construct the segment the next point from the data source is read and is used to
     * compute m=(y1-y0)/(t1-t0) where y0 is the current segment y1; t0 is the current segment t1; y1 and t1 are the new data read from the data source.
     * Every time a new segment is created the DataSourceI::Synchronise function is called and the DataSourceI is expected to increment the time vector.
     * @return true if all copies are successfully performed and if the time to be interpolated changes when a new interpolation segment is to be computed.
     * @pre
     *   Reset
     */
    virtual bool Execute();

    /**
     * @brief Sets the nanosecond time signals used by the broker to interpolate samples.
     * @param[in] dataSourceTimeIn the nanosecond time vector to be interpolated. It shall be updated by the DataSourceI every time the DataSourceI::Execute method is called.
     * @param[out] interpolatedTimeIn the nanosecond interpolated time vector. It is updated by the broker.
     * @param[in] interpolationPeriodIn the nanosecond interpolation period.
     */
    void SetTimeSignal(const uint64 * const dataSourceTimeIn, uint64 * interpolatedTimeIn, const uint64 interpolationPeriodIn);

    /**
     * @brief See MemoryMapBroker::Init
     */
    virtual bool Init(const SignalDirection direction,
            DataSourceI &dataSourceIn,
            const char8 * const functionName,
            void * const gamMemoryAddress);

    /**
     * @brief Resets the interpolated time to start counting from the first time value.
     */
    void Reset();

private:
    /**
     * @brief Interpolate between samples for any signal type.
     * @param[in] copyIdx the index of the signal to be updated.
     */
    template<typename valueType>
    void Interpolate(uint32 copyIdx);

    /**
     * Generate a new interpolation segment. To be performed every time the interpolated time is greater than the last time read from the data source.
     */
    template<typename valueType>
    void ChangeInterpolationWindow(uint32 copyIdx, float64 dt);

    /**
     * @brief Calls ChangeInterpolationWindow for all the broker signals.
     */
    void ChangeInterpolationWindows();

    /**
     * The interpolation period in nanoseconds
     */
    uint64 interpolationPeriod;

    /**
     * The beginning time of the interpolation segment
     */
    uint64 t0;

    /**
     * The ending time of the interpolation segment
     */
    uint64 t1;

    /**
     * The current data source time (not interpolated). It is the basis to compute the time windows.
     */
    const uint64 *dataSourceTime;

    /**
     * The current interpolated time
     */
    uint64 * interpolatedTime;

    /**
     * Pointers to the addresses of the y0 values of the current interpolation segment
     */
    void **y0;

    /**
     * Pointers to the addresses of the y1 values of the current interpolation segment
     */
    void **y1;

    /**
     * Pointers to the addresses of the m values
     */
    float64 *m;

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

template<typename valueType>
void MemoryMapInterpolatedInputBroker::Interpolate(uint32 copyIdx) {
uint32 i;
for (i = 0u; i < numberOfElements[copyIdx]; i++) {

    valueType *y0p = (valueType *) (y0[copyIdx]);
    float64 y = static_cast<float64>(y0p[i]);
    //How long as elapsed in this interpolation window
    float64 cttns = static_cast<float64>(*interpolatedTime - t0);
    //y = y0p + m * (t - t0), where y0p and t0p are the initial values for the interpolation period
    float64 newy = m[copyIdx] * cttns;
    newy /= 1e9;
    y += newy;
    valueType *dest = static_cast<valueType *>(copyTable[copyIdx].gamPointer);
    dest[i] = static_cast<valueType>(y);
}
}

template<typename valueType>
void MemoryMapInterpolatedInputBroker::ChangeInterpolationWindow(uint32 copyIdx, float64 dt) {
uint32 i;
for (i = 0u; i < numberOfElements[copyIdx]; i++) {
    valueType *y0p = (valueType *) (y0[copyIdx]);
    valueType *y1p = (valueType *) (y1[copyIdx]);
    valueType *y1pds = (valueType *) (copyTable[copyIdx].dataSourcePointer);

    y0p[i] = y1p[i];
    y1p[i] = y1pds[i];
    //Compute the derivative m = (y1-y0)/(t1-t0)
    m[copyIdx] = static_cast<float64>(y1p[i] - y0p[i]);
    m[copyIdx] /= dt;
}
}

}

#endif /* L5GAMS_MEMORYMAPINTERPOLATEDINPUTBROKER_H_ */

