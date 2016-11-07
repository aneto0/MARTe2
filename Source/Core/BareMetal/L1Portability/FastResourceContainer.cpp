/**
 * @file FastResourceContainer.cpp
 * @brief Source file for class FastResourceContainer
 * @date 05/11/2016
 * @author Andre'Neto
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
 * the class FastResourceContainer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "FastResourceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

MARTe::FastResourceContainer::FastResourceContainer(const uint32 nOfElements,
                                                    const bool taken) {
    treeSize = nOfElements;
    treeHalf = 0u;
    buffers = NULL_PTR(FastResourceContainerData *);

    if (treeSize > 0u) {
        uint32 bit = 1u;
        while (bit < treeSize) {
            bit <<= 1;
        }
        treeHalf = (bit >> 1u);
        buffers = new FastResourceContainerData[treeSize];
        uint32 i;
        for (i = 0u; i < nOfElements; i++) {
            buffers[i].sem = 0;
            buffers[i].size = 0;
        }
        if (!taken) {
            for (i = 0u; i < nOfElements; i++) {
                Return(i);
            }
        }
    }
}

MARTe::FastResourceContainer::~FastResourceContainer() {
    if (buffers != NULL_PTR(FastResourceContainerData *)) {
        delete[] buffers;
    }
}

uint32 MARTe::FastResourceContainer::GetSize() const {
    uint32 size = 0u;
    if (buffers != NULL_PTR(FastResourceContainerData *)) {
        size = static_cast<uint32>(buffers[0].size);
    }
    return size;
}

uint32 MARTe::FastResourceContainer::Take() {
    uint32 pos = 0u;
    bool done = false;
    if (buffers != NULL_PTR(FastResourceContainerData *)) {
        if (buffers[0].size != 0) {
            uint32 pathChooser = 0u;
            uint32 returnChooser = 0u;
            uint32 bit = treeHalf;
            while ((bit <= treeHalf) && (!done)) {
                if (bit == 0u) {
                    if (Atomic::TestAndSet(&buffers[pos].sem)) {
                        uint32 tempBit = 1u;
                        uint32 temp = pos;
                        Atomic::Decrement(&buffers[temp].size);
                        while (tempBit <= treeHalf) {
                            if ((temp & tempBit) != 0u) {
                                temp -= tempBit;
                                Atomic::Decrement(&buffers[temp].size);
                            }
                            tempBit <<= 1;
                        }
                        done = true;
                    }
                    else {
                        bit = 1u;
                        if ((pathChooser & bit) != 0u) {
                            pos -= bit;
                        }
                    }
                }

                if (!done) {
                    // time to return one level up
                    if ((returnChooser & bit) != 0u) {

                        returnChooser &= ~bit;
                        bit <<= 1;
                        if ((pathChooser & bit) != 0u) {
                            pos -= bit;
                        }
                    }
                    else { // go down
                        if ((pathChooser & bit) != 0u) { // go right
                            pathChooser &= ~bit;
                            returnChooser |= bit; // next is up
                            int32 tot = buffers[pos].size;
                            int32 right = 0;
                            if ((pos + bit) < treeSize) {
                                uint32 newpos = pos + bit;
                                right = buffers[newpos].size;
                            }
                            if ((tot - right) > 0) {
                                bit >>= 1;
                            }
                        }
                        else { // go left
                            pathChooser |= bit; // next is right
                            int32 right = 0;
                            if ((pos + bit) < treeSize) {
                                uint32 newpos = pos + bit;
                                right = buffers[newpos].size;
                            }
                            if (right > 0) {
                                pos += bit;
                                bit >>= 1;
                            }
                        }
                    }
                }
            }
        }
        if (!done) {
            pos = 0xFFFFFFFFu;
        }
    }
    return pos;
}

void MARTe::FastResourceContainer::Return(uint32 pos) {
    if (buffers != NULL_PTR(FastResourceContainerData *)) {
        if (pos < treeSize) {
            buffers[pos].sem = 0;
            Atomic::Increment(&buffers[pos].size);

            uint32 bit = 1u;
            while (bit <= treeHalf) {
                if ((pos & bit) != 0u) {
                    pos -= bit;
                    Atomic::Increment(&buffers[pos].size);
                }
                bit <<= 1;
            }
        }
    }
}
}

