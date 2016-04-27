/**
 * @file GAMContextT.h
 * @brief Header file for class GAMContextT
 * @date 24/02/2016
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

 * @details This header file contains the declaration of the class GAMContextT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMCONTEXTT_H_
#define GAMCONTEXTT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Generic templetised context to be shared across different GAMs
 * @details Provides two different buffers in order to change cleanly the
 * from the previous to the next context. The function GAMGroup::PrepareNextState(*)
 * can work on the unused buffer to prepare the context for the next state while the
 * GAMs are still using the old buffer. When the state switch is triggered, all the GAMs
 * will work on the new buffer.
 */
template<typename T>
class DLL_API GAMContextT {

public:

    /**
     * @brief Constructor
     */
    GAMContextT();

    /**
     * @brief Full access to the context buffers.
     * @param[in] bufferIndex specifies the buffer which must
     * be returned (could be 0 or 1)
     * @pre
     *   (bufferIndex == 0) ||
     *   (bufferIndex == 1)
     */
    T &GetContext(uint8 bufferIndex);

private:

    T context[2];

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
GAMContextT<T>::GAMContextT(){

}


template<typename T>
T &GAMContextT<T>::GetContext(uint8 bufferIndex) {
    return context[bufferIndex];
}

}
#endif /* GAMCONTEXTT_H_ */

