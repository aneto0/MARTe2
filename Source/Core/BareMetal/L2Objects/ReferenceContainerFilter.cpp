/**
 * @file ReferenceContainerFilter.cpp
 * @brief Source file for class ReferenceContainerFilter
 * @date 14/08/2015
 * @author Andre Neto
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
 *
 * @details This source file contains the definition of all the methods for
 * the class ReferenceContainerFilter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

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

namespace MARTe {

ReferenceContainerFilter::ReferenceContainerFilter() {
    occurrence = 0;
    originallySetOccurrence = 0;
    mode = 0u;
}

ReferenceContainerFilter::ReferenceContainerFilter(const ReferenceContainerFilter &filterCopy) {
    occurrence = filterCopy.occurrence;
    originallySetOccurrence = filterCopy.originallySetOccurrence;
    mode = 0u;
    SetMode(filterCopy.GetMode());
}

ReferenceContainerFilter::ReferenceContainerFilter(const int32 &occurrenceNumber,
                                                   const uint32 &modeToSet) {
    occurrence = occurrenceNumber;
    originallySetOccurrence = occurrenceNumber;
    mode = 0u;
    SetMode(modeToSet);
}

ReferenceContainerFilter::~ReferenceContainerFilter() {
}

void ReferenceContainerFilter::IncrementFound() {
    if (occurrence > 0) {
        occurrence--;
    }
}

bool ReferenceContainerFilter::IsRemove() const {
    return ((mode & ReferenceContainerFilterMode::REMOVE) == ReferenceContainerFilterMode::REMOVE);
}

bool ReferenceContainerFilter::IsFinished() const {
    return (occurrence == 0);
}

bool ReferenceContainerFilter::IsRecursive() const {
    return ((mode & ReferenceContainerFilterMode::RECURSIVE) == ReferenceContainerFilterMode::RECURSIVE);
}

bool ReferenceContainerFilter::IsSearchAll() const {
    return (originallySetOccurrence == -1);
}

bool ReferenceContainerFilter::IsStorePath() const {
    return ((mode & ReferenceContainerFilterMode::PATH) == ReferenceContainerFilterMode::PATH);
}

bool ReferenceContainerFilter::IsReverse() const {
    return ((mode & ReferenceContainerFilterMode::REVERSE) == ReferenceContainerFilterMode::REVERSE);
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
    Reset();
    SetMode(GetMode());
}

void ReferenceContainerFilter::SetMode(const uint32& modeToSet) {
    mode = modeToSet;

    //set the recursive bit
    if (IsStorePath()) {
        if (IsSearchAll()) {
            REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "Cannot use PATH mode to search more than one reference. The filter will match the first occurrence");
            originallySetOccurrence = 1;
        }
        mode |= ReferenceContainerFilterMode::RECURSIVE;
    }
}

}
