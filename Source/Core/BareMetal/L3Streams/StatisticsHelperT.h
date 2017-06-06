/**
 * @file StatisticsHelperT.h
 * @brief Header file for class StatisticsHelperT
 * @date 21/04/2017
 * @author Andre' Torres
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

 * @details This header file contains the declaration of the class StatisticsHelperT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STATISTICSHELPERT_H_
#define STATISTICSHELPERT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <math.h>
#include <limits>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "CircularBufferT.h"
#include "SquareRootT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The class provides templated statistics computation.
 * @detail The class allocates circular buffers to store values of samples and
 * computes average, standard deviation, minimum and maximum over a moving time
 * window. As such, the sum of samples and sum of squares is computed upon calling
 * the PushSample() method in a way to minimize operations. The minimum or maximum
 * over the historical buffer are re-computed only when needed, i.e. in case the
 * oldest sample being pushed out of the moving time window is equal to the stored
 * minimum or maximum.
 * The implementation does not perform division, rather uses bit shift operation
 * for integer types, and pre-computes 1.0 / size for floating point types. As such,
 * the computation of average and standard deviation is only exact after the window
 * has been fully populated once with samples.
 * Consequent to that above, the window size for integer types is limited to power
 * of 2, e.g. 64, 1024, etc. In case a different window size is specified, the
 * highest power of 2 smaller than the specified size is being considered. No
 * limitation for floating point types.
 */

/*lint -e{1712} the implementation does not provide default constructor*/
/*lint -e{1733} the implementation does not provide a copy constructor*/
/*lint -esym(9107, MARTe::StatisticsHelperT*) [MISRA C++ Rule 3-1-1] required for template implementation*/
template<typename Type> class StatisticsHelperT {
public:

    /**
     * @brief Constructor.
     * @detail Allocates memory buffers to store samples, and squares of, in a moving
     * time window. In case of integer types, the actual window size will be the highest
     * power of 2 smaller or equal to the specified size.
     */
    StatisticsHelperT(const uint32 windowSize);

    /**
         * @brief Copy constructor.
         */
       //StatisticsHelperT(const StatisticsHelperT& other);

    /**
     * @brief Destructor. Frees allocated memory buffers.
     */
    virtual ~StatisticsHelperT();

    /**
     * @brief Initialiser. Clears out sample buffer and initialises accumulators.
     * @return true if buffers successfully cleared.
     */
    bool Reset();

    /**
     * @brief Accessor. Inserts new sample in the moving time window.
     * @detail The method inserts the sample in the historical buffer and re-computes the
     * sum of samples over the time window with two operations, i.e. to remove the oldest
     * sample leaving the buffer from the cumulative sum before adding the new one.
     * The minimum and maximum sample attributes are changed if the newly inserted sample
     * is respectively smaller or higher than the currently stored minimum or maximum. The
     * historycal buffer is fully parsed in case the oldest sample leaving the buffer is
     * equal to the recorded minimum or maximum.
     * The new sample is also squared and inserted into a second buffer, with similar sum
     * management.
     * The average, root mean square, and standard deviation are only computed when the
     * corresponding accessors are being called.
     * @return if buffer properly allocated.
     */
    bool PushSample(Type sample);

    /**
     * @brief Accessor. Retrieves the sample at index in the sample buffer.
     * @return sample at index. 0 if index out of bounds.
     * @detail If the index is out of bounds, raises error and returns the 0 casted to the respective type.
     */
    Type GetSample(uint32 index) const;

    /**
     * @brief Accessor. Retrieves the last inserted sample.
     * @return Last inserted sample. 0 if fails.
     * @detail If CircularBufferT<Type>::GetLast returns false raises error and returns the 0 casted to the respective type.
     */
    Type GetSample(void) const;

    /**
     * @brief Accessor. Returns the window size.
     * @return actual window size.
     */
    uint32 GetSize(void) const;

    /**
     * @brief Accessor. Returns the number of samples inserted in the buffer.
     * @return number of samples.
     */
    uint32 GetCounter(void) const;

    /**
     * @brief Average over the moving window of samples.
     * @detail The average is computed  in this method from the cumulative sum of samples
     * over the time window maintained/updated by the PushSample() method. The average is
     * computed by bit-shifting the accumulator in case of integer types or by multiplying
     * it with pre-computed (1.0 / size) for floating point types.
     * @return average.
     */
    Type GetAvg(void) const;

    /**
     * @brief Maximum over the moving window of samples. No computation involved.
     * @return maximum.
     */
    Type GetMax(void) const;

    /**
     * @brief Minimum over the moving window of samples. No computation involved.
     * @return minimum.
     */
    Type GetMin(void) const;

    /**
     * @brief Root mean squares over the moving window of samples.
     * @detail The root mean square is computed in this method from the cumulative sum of
     * squared samples over the time window maintained/updated by the PushSample() method.
     * The rms is computed by bit-shifting the accumulator in case of integer types or by
     * multiplying it with pre-computed (1.0 / size) for floating point types. And
     * eventually calling SquareRoot() of the result.
     * @return rms.
     */
    Type GetRms(void) const;

    /**
     * @brief Standard deviation over the moving window of samples.
     * @detail This is the most computation heavy accessor since it involves computing the
     * average, squaring it and substrating it from the average of the squared samples over
     * the time window.
     * @return standard deviation.
     */
    Type GetStd(void) const;

    /**
     * @brief Sum of all samples over the moving window of samples.
     * @return sum.
     */
    Type GetSum(void) const;

private:

    uint32 size; /* Window size */
    uint32 counter; /* Number of samples inserted */

    Type Xavg;
    Type Xdiv;
    Type Xmax;
    Type Xmin;
    Type Xrms;
    Type Xspl;
    Type Xstd;

    CircularBufferT<Type> * Xwin; /* Sample buffer */
    CircularBufferT<Type> * Xsq; /* Sample buffer (squared) */

    /**
     * @brief Average of squared samples over the moving window.
     * @return average of sample squares.
     */
    Type GetRmsSq(void) const;

    /**
     * @brief Finds the maximum of samples in the sample buffer.
     * @return maximum.
     */
    Type FindMax(void) const;

    /**
     * @brief Finds the minimum of samples in the sample buffer.
     * @return maximum.
     */
    Type FindMin(void) const;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/*lint -e{9117} [MISRA C++ Rule 5-0-4] signedness of 0 ignored in template method to avoid specializing for all integer types*/
template<typename Type> bool StatisticsHelperT<Type>::Reset() {

    /* Reset attributes */
    counter = 0u;

    Xspl = 0;
    Xavg = 0;
    Xmax = std::numeric_limits<Type>::min();
    Xmin = std::numeric_limits<Type>::max();
    Xrms = 0;
    Xstd = 0;

    /* Reset sample buffers */
    bool ok = Xwin->Initialise(Xspl);

    if (ok) {
        ok = Xsq->Initialise(Xspl);
    }

    return ok;

}

template<> inline bool StatisticsHelperT<float32>::Reset() { // Must be declared/defined before use in the constructor

    /* Reset attributes */
    counter = 0u;

    Xspl = 0.0F;
    Xavg = 0.0F;
    Xmax = -1.0F * std::numeric_limits<float32>::max();
    Xmin = std::numeric_limits<float32>::max();
    Xrms = 0.0F;
    Xstd = 0.0F;

    /* Reset sample buffers */
    bool ok = Xwin->Initialise(Xspl);

    if (ok) {
        ok = Xsq->Initialise(Xspl);
    }

    return ok;

}

template<> inline bool StatisticsHelperT<float64>::Reset() { // Must be declared/defined before use in the constructor

    /* Reset attributes */
    counter = 0u;

    Xspl = 0.0;
    Xavg = 0.0;
    Xmax = -1.0 * std::numeric_limits<float64>::max();
    Xmin = std::numeric_limits<float64>::max();
    Xrms = 0.0;
    Xstd = 0.0;

    /* Reset sample buffers */
    bool ok = Xwin->Initialise(Xspl);

    if (ok) {
        ok = Xsq->Initialise(Xspl);
    }

    return ok;

}

/*lint -e{1566} initialisation of the attributes in the Reset() method*/
/*lint -e{9117} [MISRA C++ Rule 5-0-4] signedness of 0 and 1 ignored in template method to avoid specializing for all integer types*/
template<typename Type> StatisticsHelperT<Type>::StatisticsHelperT(const uint32 windowSize) {

    size = 1u;
    Xdiv = 0;

    uint32 tmp = windowSize;

    while (tmp > 1u) {
        tmp >>= 1u;
        size <<= 1u;
        Xdiv += 1;
    }

    /* Instantiate sample buffers */
    Xwin = new CircularBufferT<Type>(size);
    Xsq = new CircularBufferT<Type>(size);

    if (!Reset()) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Unable to Reset instance");
    }

}

/*lint -e{1566} initialisation of the attributes in the Reset() method*/
/*lint -e{9117} [MISRA C++ Rule 5-0-4] signedness of 0 and 1 ignored in template method to avoid specializing for all integer types*/
/*template<typename Type> StatisticsHelperT<Type>::StatisticsHelperT(const StatisticsHelperT& other) {

    size = other.size;
    Xdiv = other.Xdiv;

    Xwin = new CircularBufferT<Type>(size);
    Xsq = new CircularBufferT<Type>(size);

    if (!Reset()) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Unable to Reset instance");
    }

}
*/
/*lint -e{1566} initialisation of the attributes in the Reset() method*/
/*lint -e{1732} no assignment ever used */
/*lint -e{1733} no assignment ever used */
template<> inline StatisticsHelperT<float32>::StatisticsHelperT(const uint32 windowSize) {

    size = windowSize;
    Xdiv = 1.0F / static_cast<float32>(size);

    /* Instantiate sample buffers */
    Xwin = new CircularBufferT<float32>(size);
    Xsq = new CircularBufferT<float32>(size);

    if (!Reset()) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Unable to Reset instance");
    }

}

/*lint -e{1566} initialisation of the attributes in the Reset() method*/
/*lint -e{1732} no assignment ever used */
/*lint -e{1733} no assignment ever used */
template<> inline StatisticsHelperT<float64>::StatisticsHelperT(const uint32 windowSize) {

    size = windowSize;
    Xdiv = 1.0 / static_cast<float64>(size);

    /* Instantiate sample buffers */
    Xwin = new CircularBufferT<float64>(size);
    Xsq = new CircularBufferT<float64>(size);

    if (!Reset()) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Unable to Reset instance");
    }

}

/*lint -e{1551} no exception thrown deleting the CircularBufferT<> instance*/
/*lint -e{665} [MISRA C++ Rule 16-0-6] templated type passed as argument to MACRO*/
template<typename Type> StatisticsHelperT<Type>::~StatisticsHelperT() {

    if (Xwin != NULL_PTR(CircularBufferT<Type> *)) {
        delete Xwin;
        Xwin = NULL_PTR(CircularBufferT<Type> *);
    }

    if (Xsq != NULL_PTR(CircularBufferT<Type> *)) {
        delete Xsq;
        Xsq = NULL_PTR(CircularBufferT<Type> *);
    }

}

/*lint -e{534} ignore return value of GetData since intialisation error would have been caught earlier*/
template<typename Type> Type StatisticsHelperT<Type>::FindMax() const {

    Type max = std::numeric_limits<Type>::min();

    uint32 index;

    for (index = 0u; index < size; index++) {

        Type sample;
        Type * sample_p = &sample;

        /*lint -e{534} ignore return value of GetData since an intialisation error would make that this method is not called*/
        Xwin->GetData(sample_p, index);

        if (sample > max) {
            max = sample;
        }
    }

    return max;
}

template<> inline float32 StatisticsHelperT<float32>::FindMax() const { // Must be declared/defined before use

    float32 max = -1.0F * std::numeric_limits<float32>::max();

    uint32 index;

    for (index = 0u; index < size; index++) {

        float32 sample;
        float32 * sample_p = &sample;
        /*lint -e{534} ignore return value of GetData since an intialisation error would make that this method is not called*/
        Xwin->GetData(sample_p, index);

        if (sample > max) {
            max = sample;
        }
    }

    return max;
}

template<> inline float64 StatisticsHelperT<float64>::FindMax() const { // Must be declared/defined before use

    float64 max = -1.0 * std::numeric_limits<float64>::max();

    uint32 index;

    for (index = 0u; index < size; index++) {

        float64 sample;
        float64 * sample_p = &sample;

        /*lint -e{534} ignore return value of GetData since an intialisation error would make that this method is not called*/
        Xwin->GetData(sample_p, index);

        if (sample > max) {
            max = sample;
        }
    }

    return max;
}

template<typename Type> Type StatisticsHelperT<Type>::FindMin() const {

    Type min = std::numeric_limits<Type>::max();

    uint32 index;

    for (index = 0u; index < size; index++) {

        Type sample;
        Type * sample_p = &sample;

        /*lint -e{534} ignore return value of GetData since an intialisation error would make that this method is not called*/
        Xwin->GetData(sample_p, index);

        if (sample < min) {
            min = sample;
        }
    }

    return min;
}

template<typename Type> bool StatisticsHelperT<Type>::PushSample(Type sample) {

    bool ok = true;

    Xspl = sample;

    if (counter < size) {
        counter++;
    }

    Type oldest;

    /* Update sample buffer */
    if (ok) {
        ok = Xwin->PushData(*&Xspl, oldest);
    }

    if (ok) {
        /* Compute average */
        Xavg -= oldest; /* Remove oldest sample from the accumulator */
        Xavg += Xspl; /* Sum of all sample in time window */

        /* Update max/min, if necessary */
        if (Xspl > Xmax) {
            Xmax = Xspl;
        }
        else if (oldest == Xmax) { /* The removed sample was the max over the time window */
            Xmax = FindMax();
        }

        if (Xspl < Xmin) {
            Xmin = Xspl;
        }
        else if (oldest == Xmin) { /* The removed sample was the min over the time window */
            Xmin = FindMin();
        }
    }

    Type Xspl_sq = Xspl * Xspl; /* Square of the sample */

    /* Update sample buffer */
    if (ok) {
        ok = Xsq->PushData(*&Xspl_sq, oldest);
    }

    if (ok) {
        /* Compute root mean square */
        Xrms -= oldest; /* Remove oldest sample from the accumulator */
        Xrms += Xspl_sq; /* Sum of squares of all samples in time window */
    }

    return ok;
}

template<typename Type> Type StatisticsHelperT<Type>::GetSample(uint32 index) const {
    Type elementOut;
    Type * elementOut_p = &elementOut;
    if (!Xwin->GetData(elementOut_p, index)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Unable to GetSample");
        elementOut = static_cast<Type>(0);
    }
    return elementOut;
}
template<typename Type> Type StatisticsHelperT<Type>::GetSample(void) const {
    Type elementOut;
    if (!Xwin->GetLast(*&elementOut)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Unable to GetLast");
        elementOut = static_cast<Type>(0);
    }
    return elementOut;
}
template<typename Type> uint32 StatisticsHelperT<Type>::GetSize() const {
    return size;
}

template<typename Type> uint32 StatisticsHelperT<Type>::GetCounter() const {
    return counter;
}

template<typename Type> Type StatisticsHelperT<Type>::GetAvg() const {

    Type avg = Xavg >> Xdiv;

    return avg;
}

template<> inline float32 StatisticsHelperT<float32>::GetAvg() const {

    float32 avg = Xavg * Xdiv;

    return avg;
}

template<> inline float64 StatisticsHelperT<float64>::GetAvg() const {

    float64 avg = Xavg * Xdiv;

    return avg;
}

template<typename Type> Type StatisticsHelperT<Type>::GetRmsSq() const {

    Type rms_sq = Xrms >> Xdiv;

    return rms_sq;
}

template<> inline float32 StatisticsHelperT<float32>::GetRmsSq() const {

    float32 rms_sq = Xrms * Xdiv;

    return rms_sq;
}

template<> inline float64 StatisticsHelperT<float64>::GetRmsSq() const {

    float64 rms_sq = Xrms * Xdiv;

    return rms_sq;
}

template<typename Type> Type StatisticsHelperT<Type>::GetRms() const {

    Type rms_sq = GetRmsSq();
    Type rms = FastMath::SquareRoot<Type>(rms_sq);

    return rms;

}

template<typename Type> Type StatisticsHelperT<Type>::GetStd() const {

    Type avg = GetAvg();
    Type avg_sq = avg * avg;
    Type rms_sq = GetRmsSq();
    Type std = FastMath::SquareRoot<Type>(rms_sq - avg_sq);

    return std;

}

template<typename Type> Type StatisticsHelperT<Type>::GetSum() const {
    return Xavg;
}

template<typename Type> Type StatisticsHelperT<Type>::GetMax() const {
    return Xmax;
}

template<typename Type> Type StatisticsHelperT<Type>::GetMin() const {
    return Xmin;
}

} /* namespace MARTe */

#endif /* StatisticsHelperT_H_ */

