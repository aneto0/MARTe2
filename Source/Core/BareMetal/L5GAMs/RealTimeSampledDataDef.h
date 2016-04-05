/**
 * @file RealTimeSampledDataDef.h
 * @brief Header file for class RealTimeSampledDataDef
 * @date 25/02/2016
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

 * @details This header file contains the declaration of the class RealTimeSampledDataDef
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMESAMPLEDDATADEF_H_
#define REALTIMESAMPLEDDATADEF_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RealTimeDataDefI.h"
#include "StreamString.h"
#include "StructuredDataI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Maps a final structure definition to the RealTimeDataSource.
 * @details The definition is final for data which is supposed to be
 * interfaced directly with the hardware.
 *
 * @details The definition express how many samples of data will be generated
 * in a specified number of cycles.
 */
class RealTimeSampledDataDef: public RealTimeDataDefI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     * @post
     *   GetSamples() == 0 &&
     *   GetSamplesPerCycle() == 0;
     */
    RealTimeSampledDataDef();

    /**
     * @see RealTimeDataDefI::MergeWithLocal(*)
     */
    virtual bool MergeWithLocal(StructuredDataI &localData);

    /**
     * @brief Reads the samples and cycles values to determine the number of samples per cycle.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see RealTimeDataDefI::Verify(*)
     */
    virtual bool Verify();


    int32 GetSamples() const;

    int32 GetSamplesPerCycle() const;

    virtual bool ToStructuredData( StructuredDataI & data);


private:


    /**
     * How many samples
     */
    int32 samples;

    /**
     * How many cycles
     */
    int32 samplesPerCycle;

    bool final;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMESAMPLEDDATADEF_H_ */

