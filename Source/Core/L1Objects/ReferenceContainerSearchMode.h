/**
 * @file ReferenceContainerSearchMode.h
 * @brief Header file for class ReferenceContainerSearchMode
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

 * @details This header file contains the declaration of the class ReferenceContainerSearchMode
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERSEARCHMODE_H_
#define SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERSEARCHMODE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Defines the possible search modes for the ReferenceContainer.
 */
class ReferenceContainerSearchMode {
public:
    /**
     * @brief Set the searching mode parameters.
     * @param occurenceNumber Ordinal occurrence number (i.e. find the first, the second, ...) of the finding of
     * a node which meets a given criteria or -1 to look for all occurrences. This parameter is indexed to 1.
     * @param path Store all the nodes leading to a given occurrence (only valid if \a occurrence != -1).
     * @param recursive Search recursively in all the tree nodes.
     * @param reverse If true the search is performed from left to right, otherwise from right to left. Note that to search
     * the last occurrence of a given node it is sufficient to set \a occurrence = 1 and \a reverse = true
     * @param storePath If true all the nodes leading to a given occurrence are stored (only valid if \a occurrence != -1).
     * If \a storePath = true the recursive setting will be automatically set to true.
     * @param deleteFound If true the nodes that are found are deleted (notice that to destroy all occurrences \a occurrence must be set to -1).
     * If \a deleteFound = true the \a storePath setting will be automatically set to false.
     */
    ReferenceContainerSearchMode(int32 occurrenceNumber,
                                 bool path,
                                 bool recurseNodes,
                                 bool reverseSearch,
                                 bool deleteFound);

    /**
     * @brief Informs that a new occurrence has been found.
     */
    void IncrementFound();

    /**
     * @brief Return true if the found nodes are to be deleted.
     * @return true if the found nodes are to be deleted, false otherwise.
     */
    bool IsDelete() const;

    /**
     * @brief Return true if the occurrence of the node being searched was found.
     * @return true if the occurrence of the node being searched was found, false otherwise.
     */
    bool IsFinished() const;

    /**
     * @brief Return true if the search is recursive in all the tree nodes.
     * @return true if the search is recursive in all the tree nodes, false otherwise.
     */
    bool IsRecursive() const;

    /**
     * @brief Return true if all occurrences of a given pattern are to be searched.
     * @return true if all occurrences of a given pattern are to searched, false otherwise.
     */
    bool IsSearchAll() const;

    /**
     * @brief Return true if all the nodes leading to a given occurrence are to be stored.
     * @return true true if all the nodes leading to a given occurrence are to be stored, false otherwise.
     */
    bool IsStorePath() const;

    /**
     * @brief Return true if the tree should be searched from right to left.
     * @return true if the tree should be searched from right to left, false otherwise.
     */
    bool IsReverse() const;

private:
    /**
     * Ordinal occurrence number of the finding (i.e. the first, second, ...) or -1 to look for all occurrences.
     */
    int32 occurrence;
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
    bool deleteFoundNodes;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERSEARCHMODE_H_ */

