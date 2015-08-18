/**
 * @file ReferenceContainerFilter.cpp
 * @brief Source file for class ReferenceContainerFilter
 * @date 14/08/2015
 * @author aneto
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
 * the class ReferenceContainerFilter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainerFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
ReferenceContainerFilter::ReferenceContainerFilter() {
    occurrence = 0;
    storePath = false;
    recursive = false;
    removeFoundNodes = false;
    reverse = false;
    originallySetOccurrence = 0;
    mode = 0u;
}

ReferenceContainerFilter::ReferenceContainerFilter(const ReferenceContainerFilter &filterCopy) {
    (*this) = filterCopy;
}

ReferenceContainerFilter::ReferenceContainerFilter(const int32 &occurrenceNumber,
                                                   const uint32 &modeToSet) {
    occurrence = occurrenceNumber;
    originallySetOccurrence = occurrenceNumber;
    storePath = false;
    recursive = false;
    removeFoundNodes = false;
    reverse = false;
    mode = 0u;
    SetMode(modeToSet);
}

void ReferenceContainerFilter::IncrementFound() {
    if (occurrence > 0) {
        occurrence--;
    }
}

bool ReferenceContainerFilter::IsRemove() const {
    return removeFoundNodes;
}

bool ReferenceContainerFilter::IsFinished() const {
    return (occurrence == 0);
}

bool ReferenceContainerFilter::IsRecursive() const {
    return recursive;
}

bool ReferenceContainerFilter::IsSearchAll() const {
    return (occurrence == -1);
}

bool ReferenceContainerFilter::IsStorePath() const {
    return storePath;
}

bool ReferenceContainerFilter::IsReverse() const {
    return reverse;
}

uint32 ReferenceContainerFilter::GetMode() const {
    return mode;
}

void ReferenceContainerFilter::Reset() {
    occurrence = originallySetOccurrence;
}

int32 ReferenceContainerFilter::GetOriginalSetOccurrence() const {
    return originallySetOccurrence;
}

void ReferenceContainerFilter::SetOriginalSetOccurrence(const int32 occurrenceToSet) {
    originallySetOccurrence = occurrenceToSet;
    SetMode(GetMode());
    Reset();
}

void ReferenceContainerFilter::SetMode(const uint32& modeToSet) {
    mode = modeToSet;
    storePath = false;
    recursive = false;
    removeFoundNodes = false;
    reverse = false;

    if ((modeToSet & ReferenceContainerFilterMode::PATH) == ReferenceContainerFilterMode::PATH) {
        storePath = true;
    }
    if ((modeToSet & ReferenceContainerFilterMode::RECURSIVE) == ReferenceContainerFilterMode::RECURSIVE) {
        recursive = true;
    }
    if ((modeToSet & ReferenceContainerFilterMode::REVERSE) == ReferenceContainerFilterMode::REVERSE) {
        reverse = true;
    }
    if ((modeToSet & ReferenceContainerFilterMode::REMOVE) == ReferenceContainerFilterMode::REMOVE) {
        removeFoundNodes = true;
    }
    if (occurrence == -1) {
        storePath = false;
    }
    if (storePath) {
        recursive = true;
    }
}
