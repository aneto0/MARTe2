/**
 * @file ReferenceContainer.h
 * @brief Header file for class ReferenceContainer
 * @date 13/08/2015
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

 * @details This header file contains the declaration of the class ReferenceContainer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_L1OBJECTS_REFERENCECONTAINER_H_
#define SOURCE_CORE_L1OBJECTS_REFERENCECONTAINER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "LinkedListHolder.h"
#include "MutexSem.h"
#include "TimeoutType.h"
#include "Object.h"
#include "Reference.h"
#include "LinkedListable.h"
#include "ReferenceContainerItem.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class ReferenceContainer;

namespace ReferenceContainerFilters {
class Interface {
public:
    virtual ~Interface() {

    }
    virtual bool Test(ReferenceContainer &previouslyFound,
                      Reference &referenceToTest) = 0;
};

class References: public ReferenceContainerFilters::Interface {
public:
    References(Reference refToSearch) {
        referenceToSearch = refToSearch;
    }

    virtual bool Test(ReferenceContainer &previouslyFound,
                      Reference &referenceToTest) {
        return (referenceToSearch == referenceToTest);
    }

private:
    Reference referenceToSearch;
};
}

namespace SearchModeType {
const int32 Last = -2;
const int32 Multiple = -1;
}

class SearchMode {
public:
    SearchMode(int32 idx,
               bool path,
               bool recurseNodes,
               bool deleteFound) {
        index = idx;
        if (idx == SearchModeType::Multiple) {
            storePath = false;
        }
        else {
            storePath = path;
        }
        if (storePath) {
            recursive = true;
        }
        else {
            recursive = recurseNodes;
        }

        deleteFoundNodes = deleteFound;
        lastFoundIndex = -1;
    }

    void IncrementFound() {
        if (index > 0) {
            index--;
        }
    }

    bool IsDelete() const {
        return deleteFoundNodes;
    }

    bool IsFinished() const {
        return (index == 0);
    }

    bool IsRecursive() const {
        return recursive;
    }

    bool IsSearchIndex() const {
        return index > -1;
    }

    bool IsSearchAll() const {
        return (index == SearchModeType::Multiple);
    }

    bool IsStorePath() const {
        return storePath;
    }

    bool IsSearchLast() const {
        return (index == SearchModeType::Last);
    }

    uint32 GetLastFoundIndex() const {
        return lastFoundIndex;
    }

    void SetLastFoundIndex(const uint32 &idx) {
        lastFoundIndex = idx;
    }
private:
    int32 index;
    int32 lastFoundIndex;
    bool recursive;
    bool deleteFoundNodes;
    bool storePath;
};

class ReferenceContainer: public Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * TODO
     */
    ReferenceContainer();

    /**
     * TODO
     */
    virtual ~ReferenceContainer();

    /**
     * TODO
     */
    bool Insert(Reference ref,
                const int32 &position = -1);

    /**
     * TODO
     */
    bool Delete(Reference ref);

    /**
     *
     * @param result
     * @param filter
     * @param mode
     * @return
     */
    bool Find(ReferenceContainer &result,
              ReferenceContainerFilters::Interface &filter,
              SearchMode &mode);

    bool IsContainer(const Reference &ref);

    uint32 Size() const;

    Reference Get(uint32 idx) {
        Reference ref;
        if (idx < list.ListSize()) {
            ref = ((ReferenceContainerItem *) list.ListPeek(idx))->GetReference();
        }
        return ref;
    }

private:
    /**
     * The list of references
     */
    LinkedListHolder list;

    /**
     * Protects multiple access to the internal resources
     */
    MutexSem mux;

    /**
     * Timeout
     */
    TimeoutType msecTimeout;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L1OBJECTS_REFERENCECONTAINER_H_ */

