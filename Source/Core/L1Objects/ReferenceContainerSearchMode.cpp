/**
 * @file ReferenceContainerSearchMode.cpp
 * @brief Source file for class ReferenceContainerSearchMode
 * @date 13/08/2015
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
 * the class ReferenceContainerSearchMode (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
ReferenceContainerSearchMode::ReferenceContainerSearchMode(const int32 &occurrenceNumber, const uint32 &mode) {
    occurrence = occurrenceNumber;
    storePath = false;
    recursive = false;
    deleteFoundNodes = false;
    reverse = false;

    if ((mode & PATH) == PATH) {
        storePath = true;
    }
    if ((mode & RECURSIVE) == RECURSIVE) {
        recursive = true;
    }
    if ((mode & REVERSE) == REVERSE) {
        reverse = true;
    }
    if ((mode & DELETE) == DELETE) {
        deleteFoundNodes = true;
    }
    if (occurrence == -1) {
        storePath = false;
    }
    if (storePath) {
        recursive = true;
    }
}

void ReferenceContainerSearchMode::IncrementFound() {
    if (occurrence > 0) {
        occurrence--;
    }
}

bool ReferenceContainerSearchMode::IsDelete() const {
    return deleteFoundNodes;
}

bool ReferenceContainerSearchMode::IsFinished() const {
    return (occurrence == 0);
}

bool ReferenceContainerSearchMode::IsRecursive() const {
    return recursive;
}

bool ReferenceContainerSearchMode::IsSearchAll() const {
    return (occurrence == -1);
}

bool ReferenceContainerSearchMode::IsStorePath() const {
    return storePath;
}

bool ReferenceContainerSearchMode::IsReverse() const {
    return reverse;
}

