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
 * @details This class interpolates the signals from the DataSourceI and copies the interpolated values to the GAM memory.
 *
 * The DataSourceI shall call the method SetIndependentVariable and provide one x-axis vector, containing the x-values that are to be used as the basis for the interpolation.
 * The independent variable vector (typically a time vector) shall not have zero derivative between any two consecutive points and will be used as the basis
 * to compute the interpolation segments for all the other DataSource signals.
 *
 * @warning the Reset function shall be called before the first Execute and the DataSourceI shall have its first data points (x0, y0)
 * loaded into its memory (i.e. all the pointers returned by DataSourceI::GetSignalMemoryBuffer shall have valid values).
 */
class DLL_API MemoryMapInterpolatedInputBroker: public MemoryMapBroker {
public:CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor. NOOP.
     */
    MemoryMapInterpolatedInputBroker();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~MemoryMapInterpolatedInputBroker();

    /**
     * @brief Interpolates the data for all the registered signals and copies them to the GAM memory.
     * @details Interpolates between consecutive samples. While the independent variable vector value (typically time) is greater than the last value
     * read from the data source a new interpolation segment is created for each signal.
     * To construct the segment, the next point from the data source is read and is used to
     * compute m=(y1-y0)/(x1-x0) where y0 is the current segment y1; x0 is the current segment x1; y1 and x1 are the new data read from the data source.
     * Every time a new segment is created the DataSourceI::Synchronise function is called and the DataSourceI is
     * expected to provide the next value of the independent variable vector (that is the next value of x1).
     *
     * After a Reset the first Execute will copy the current values from the DataSourceI memory (without attempting to interpolate).
     * @return true if all copies are successfully performed and if the interpolation x1 value changes when a new interpolation segment is to be computed.
     * @pre
     *   Reset
     */
    virtual bool Execute();

    /**
     * @brief Sets the independent variable to be used by the broker to interpolate samples.
     * @param[in] dataSourceXAxisIn the independent variable to compute the interpolation segments. It shall be updated by the DataSourceI every time the DataSourceI::Execute method is called.
     * @param[in] interpolationPeriodIn the interpolation period (how much the interpolatedXAxis is incremented every time a new interpolated sample is generated).
     */
    void SetIndependentVariable(const uint64 *const dataSourceXAxisIn,
                                const uint64 interpolationPeriodIn);

    /**
     * @brief See MemoryMapBroker::Init
     */
    virtual bool Init(const SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 *const functionName,
                      void *const gamMemoryAddress);

    /**
     * @brief Resets the interpolation vector to start counting from the first time value.
     */
    void Reset();

private:
    /**
     * @brief Interpolates between samples for any signal type.
     * @param[in] copyIdx the index of the signal to be updated.
     */
    template<typename valueType>
    void Interpolate(uint32 copyIdx);

    /**
     * @brief Generate a new interpolation segment. To be performed every time the interpolated vector is greater than the last time read from the data source.
     * @param[in] copyIdx the index of the signal to be updated.
     * @param[in] dx the interpolation segment length.
     */
    template<typename valueType>
    void ChangeInterpolationSegment(uint32 copyIdx,
                                    uint64 dx);

    /**
     * @brief Calls ChangeInterpolationSegment for all the broker signals.
     */
    void ChangeInterpolationSegments();

    /**
     * The interpolation period
     */
    uint64 interpolationPeriod;

    /**
     * The beginning xx point of the interpolation segment
     */
    uint64 x0;

    /**
     * The ending xx point of the interpolation segment
     */
    uint64 x1;

    /**
     * The current data source interpolation vector (not interpolated). It is the basis to compute the interpolation segments.
     */
    const uint64 *dataSourceXAxis;

    /**
     * The current interpolated vector
     */
    uint64 interpolatedXAxis;

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

    /**
     * Was the broker reset
     */
    bool reset;
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
        valueType *y0p = (valueType*) (y0[copyIdx]);
/**
 * Do not assume memory alignment... (see below)
        float64 y = static_cast<float64>(y0p[i]);
 */
        valueType ytemp;
        (void) MemoryOperationsHelper::Copy(&ytemp, &y0p[i], sizeof(valueType));
        float64 y = static_cast<float64>(ytemp);

        //How long as elapsed in this interpolation segment
        float64 cttns = static_cast<float64>(interpolatedXAxis - x0);
        //y = y0p + m * (t - x0), where y0p and t0p are the initial values for the interpolation period
        float64 newy = m[copyIdx] * cttns;
        y += newy;
        valueType *dest = static_cast<valueType*>(copyTable[copyIdx].gamPointer);
        //dest[i] = static_cast<valueType>(y);
        ytemp = static_cast<valueType>(y);
        (void) MemoryOperationsHelper::Copy(&dest[i], &ytemp, sizeof(valueType));
    }
}

template<typename valueType>
void MemoryMapInterpolatedInputBroker::ChangeInterpolationSegment(uint32 copyIdx,
                                                                  uint64 dx) {
    uint32 i;
    for (i = 0u; i < numberOfElements[copyIdx]; i++) {
        valueType *y0p = (valueType*) (y0[copyIdx]);
        valueType *y1p = (valueType*) (y1[copyIdx]);
        valueType *y1pds = (valueType*) (copyTable[copyIdx].dataSourcePointer);

        /**
         * Do not assume memory alignment... (see e.g. https://stackoverflow.com/questions/13804215/arm-memcpy-and-alignment)
         * This code was giving bus errors in arm targets when the memory was not aligned to four bytes (which may the case when
         * the GAM/DataSource signals interleave with different byte sizes).
         *      y0p[i] = y1p[i];
         *      y1p[i] = y1pds[i];
         */
        (void) MemoryOperationsHelper::Copy(&y0p[i], &y1p[i], sizeof(valueType));
        (void) MemoryOperationsHelper::Copy(&y1p[i], &y1pds[i], sizeof(valueType));
        valueType y0v;
        valueType y1v;
        (void) MemoryOperationsHelper::Copy(&y0v, &y0p[i], sizeof(valueType));
        (void) MemoryOperationsHelper::Copy(&y1v, &y1p[i], sizeof(valueType));
        //Compute the derivative m = (y1-y0)/(x1-x0)
        //m[copyIdx] = static_cast<float64>(y1p[i] - y0p[i]);
        m[copyIdx] = static_cast<float64>(y1v - y0v);
        m[copyIdx] /= dx;
    }
}

}

#endif /* L5GAMS_MEMORYMAPINTERPOLATEDINPUTBROKER_H_ */

