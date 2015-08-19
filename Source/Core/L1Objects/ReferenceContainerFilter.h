/**
 * @file ReferenceContainerSearchFilter.h
 * @brief Header file for class ReferenceContainerSearchFilter
 * @date 13/08/2015
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class ReferenceContainerSearchFilter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERFILTER_H_
#define SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERFILTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Reference.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * Namespace to define the possible modes of search.
 */
namespace ReferenceContainerFilterMode {
/**
 * @brief Store all the nodes leading to a given occurrence.
 * @details Only valid if \a occurrence != -1 in the constructor of ReferenceContainerSearchMode.
 */
static const uint32 PATH = 0x1u;
/**
 * @brief Search recursively in all the tree nodes.
 */
static const uint32 RECURSIVE = 0x2u;
/**
 * @brief If set, the search is performed from left to right, otherwise from right to left.
 * @details Note that to search he last occurrence of a given node it is sufficient to set
 * ReferenceContainerSearchMode(1, RECURSIVE | REVERSE)
 */
static const uint32 REVERSE = 0x4u;
/**
 * @brief If set the nodes that are found are deleted.
 * @details To destroy all occurrences \a occurrence must be set to -1 in in the constructor of ReferenceContainerSearchMode-
 * If REMOVE is set, PATH will be automatically unset.
 */
static const uint32 REMOVE = 0x8u;
}

class ReferenceContainer;
/**
 * @brief Defines the filtering criteria for the ReferenceContainer searches.
 * @details This class needs to be specialised to implement different search criteria.
 * The find function of the ReferenceContainer will walk the list and query, by calling Test,
 * if a given node has been found.
 *
 * The different types of searching modes are also stored by this class.
 *
 * Every time the Test function is to return it should call
 * IncrementFound so that the number of occurrences found is correctly tracked.
 */
class ReferenceContainerFilter {
public:
    /**
     * @brief Default constructor
     */
    ReferenceContainerFilter();



    /**
     * @brief Copy constructor
     */
    ReferenceContainerFilter(const ReferenceContainerFilter &filterCopy);


    /**
     * @brief Set the searching mode parameters.
     * @param occurenceNumber the ordinal occurrence number (i.e. find the first, the second, ...) of the finding of
     * a node which meets a given criteria or -1 to look for all occurrences. This parameter is indexed to 1.
     * @param modeToSet any ored combination of ReferenceContainerFilterMode::PATH, ReferenceContainerFilterMode::RECURSIVE,
     * ReferenceContainerFilterMode::REVERSE and ReferenceContainerFilterMode::REMOVE.
     */
    ReferenceContainerFilter(const int32 &occurrenceNumber,
                             const uint32 &modeToSet);
    /**
     * @brief Destructor. NOOP
     */
    virtual ~ReferenceContainerFilter() {

    }

    /**
     * @brief Tests if a given reference meets the searching criteria.
     * @details This function is to be implemented by the filter subclasses.
     * @param previouslyFound a container with the history of all the nodes that were previously found.
     * @param referenceToTest the Reference to be tested.
     * @return if the \a referenceToTest meets the searching criteria.
     */
    virtual bool Test(ReferenceContainer &previouslyFound,
                      Reference &referenceToTest) = 0;

    /**
     * @brief Informs that a new occurrence has been found.
     */
    void IncrementFound();

    /**
     * @brief Return true if the found nodes are to be deleted.
     * @return true if the found nodes are to be deleted, false otherwise.
     */
    bool IsRemove() const;

    /**
     * @brief Return true if the occurrence of the node being searched was found.
     * @return true if the occurrence of the node being searched was found, false otherwise.
     */
    bool IsFinished() const;

    /**
     * @brief Return true if the search is recursive in all the tree nodes.
     * @return true if the search is recursive in all the tree nodes, false otherwise.
     */
    virtual bool IsRecursive() const;

    /**
     * @brief Return true if all occurrences of a given pattern are to be searched.
     * @return true if all occurrences of a given pattern are to searched, false otherwise.
     */
    virtual bool IsSearchAll() const;

    /**
     * @brief Return true if all the nodes leading to a given occurrence are to be stored.
     * @return true true if all the nodes leading to a given occurrence are to be stored, false otherwise.
     */
    virtual bool IsStorePath() const;

    /**
     * @brief Return true if the tree should be searched from right to left.
     * @return true if the tree should be searched from right to left, false otherwise.
     */
    bool IsReverse() const;

    /**
     * @brief Return the search mode as a binary keyword.
     * @return the search mode as a binary keyword.
     */
    uint32 GetMode() const;

    /**
     * @brief Updates the search mode to a new binary keyword.
     * @param modeToSet any ored combination of ReferenceContainerFilterMode::PATH, ReferenceContainerFilterMode::RECURSIVE,
     * ReferenceContainerFilterMode::REVERSE and ReferenceContainerFilterMode::REMOVE.
     */
    void SetMode(const uint32 &modeToSet);

    /**
     * @brief Resets the number of occurrences to the value that was set when the filter was created.
     */
    void Reset();

    /**
     * @brief Return the number of occurrences that were set when the filter was created.
     * @return the number of occurrences that were set when the filter was created.
     */
    int32 GetOriginalSetOccurrence() const;

    /**
     * @brief Updates the occurrence value. This will trigger a Reset().
     * @param occurrence the new occurrence value that should be set when a Reset() is triggered.
     */
    void SetOriginalSetOccurrence(const int32 occurrenceToSet);

protected:
    /**
     * Ordinal occurrence number of the finding (i.e. the first, second, ...) or -1 to look for all occurrences.
     */
    int32 occurrence;

    /**
     * The originally set number of occurrences
     */
    int32 originallySetOccurrence;

private:
    /**
     * Recursive into all the nodes children.
     */
    bool recursive;
    /**
     * Search for left to right, or from right to left.
     */
    bool reverse;
    /**
     * Store all the nodes leading to a given occurrence (only valid if \a occurrence != -1).
     */
    bool storePath;
    /**
     * Delete the nodes that are found (all or the exact matching node depending on how occurrence is set).
     */
    bool removeFoundNodes;
    /**
     * The mode encoded in binary
     */
    uint32 mode;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERFILTER_H_ */

