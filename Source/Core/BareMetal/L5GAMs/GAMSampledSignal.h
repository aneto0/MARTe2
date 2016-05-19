/**
 * @file GAMSampledSignal.h
 * @brief Header file for class GAMSampledSignal
 * @date 11/04/2016
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class GAMSampledSignal
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMSAMPLEDSIGNAL_H_
#define GAMSAMPLEDSIGNAL_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAMSignalI.h"
#include "StreamString.h"
#include "StructuredDataI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The definition of a sampled GAM signal which can be directly interfaced with a
 * driver.
 *
 * @details The definition express how many samples of data will be generated
 * in a specified number of cycles.
 *
 * @details It is possible link a GAMSampledSignal only to a single data source signal.
 *
 * @details The syntax in the configuration stream has to be:
 *
 * +GAMSampledSignal_name = {\n
 *     Class = (child of GAMSignalI) \n
 *     Path = "the path of the variable in the RealTimeDataSource" (default "")\n
 *     Type = "the variable type" (default "")\n
 *     Default = "the variable default value" (default "")\n
 *     Dimensions = "the variable dimensions" (default "")\n
 *     Operation = "the variable operation" (default "")
 *     Cycles = "how many consecutive cycles the variable must be be read or write"
 *     Samples = "the samples blocks to e read or write"
 *     ...\n
 * }\n
 *
 * and it has to be contained in the [GAMSignalsContainer] declaration.
 */
class DLL_API GAMSampledSignal: public GAMSignalI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     * @post
     *   GetSamples() == ""
     */
    GAMSampledSignal();

    /**
     * @see GAMSignalI::MergeWithLocal(*)
     */
    virtual bool MergeWithLocal(StructuredDataI &localData);

    /**
     * @see ReferenceContainer::Initialise(*).
     * @details The following fields can be specified:
     *
     *   - Samples = "the blocks of samples to be read in each cycle"\n
     *   - IsFinal = "specifies if the definition is complete or needs to be integrated with a local one"\n
     *
     * See GAMSignalI::Initialise() for the other field which can be specified.\n
     * "IsFinal" is false by default. If it is specified true, this definition is supposed to be
     * completed, otherwise can be completed merging it with local configuration data (see MergeWithLocal(*))\n
     * The field "Samples" is a string expressing a nx3 matrix and each vector defines a block of samples to be read (write).
     * In particular in each vector "{a , b , c}" define the following two sample fractional indexes: a/c and b/c, where a/c is the
     * oldest sample to read and b/c is the most recent sample to read, in ascending order with zero being the most recent.
     * Let N = DataSourceSignal::GetNumberOfSamples() be the total number of samples in a signal S, where S[N-1] is the most recent sample.
     * Then, the following operation will be applied to access the specified samples of the S[N-(a/c)*N:N-(b/c)*N-1]\n
     * As an example, let N = 1000 and Samples = "{{1 , 0 , 2},{10, 9, 10}}"
     * In this case, the following signal will be read in runtime: s = { S(500), S(501), ... , S(999), S(0), ... , S(99) },
     * where S(i) denotes the i-th sample.\n
     * By default, if this field is not specified, the last sample produced will be read (write).
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see GAMSignalI::Verify(*)
     */
    virtual bool Verify();

    /**
     * @brief Retrieves the number of samples of this variable.
     * @return the number of samples of this variable.
     */
    const char8 *GetSamples() ;

    /**
     * @see P.
     * @param[out] data is the result of the conversion.
     * @return false in case of errors, true otherwise.
     */
    virtual bool ToStructuredData( StructuredDataI & data);


private:


    /**
     * The samples blocks
     */
    StreamString samples;


    /**
     * Specifies if the definition is final or can be merged with local data definitions
     */
    bool final;

};
}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMSAMPLEDSIGNAL_H_ */

