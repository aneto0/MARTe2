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
ReferenceContainerFilter::ReferenceContainerFilter(const int32 &occurrenceNumber, const uint32 &mode) {
    occurrence = occurrenceNumber;
    storePath = false;
    recursive = false;
    deleteFoundNodes = false;
    reverse = false;

    if ((mode & ReferenceContainerFilterMode::PATH) == ReferenceContainerFilterMode::PATH) {
        storePath = true;
    }
    if ((mode & ReferenceContainerFilterMode::RECURSIVE) == ReferenceContainerFilterMode::RECURSIVE) {
        recursive = true;
    }
    if ((mode & ReferenceContainerFilterMode::REVERSE) == ReferenceContainerFilterMode::REVERSE) {
        reverse = true;
    }
    if ((mode & ReferenceContainerFilterMode::DELETE) == ReferenceContainerFilterMode::DELETE) {
        deleteFoundNodes = true;
    }
    if (occurrence == -1) {
        storePath = false;
    }
    if (storePath) {
        recursive = true;
    }
}

void ReferenceContainerFilter::IncrementFound() {
    if (occurrence > 0) {
        occurrence--;
    }
}

bool ReferenceContainerFilter::IsDelete() const {
    return deleteFoundNodes;
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


	
