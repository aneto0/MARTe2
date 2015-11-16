/**
 * @file ConfigurationDatabase.h
 * @brief Header file for class ConfigurationDatabase
 * @date 27/10/2015
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

 * @details This header file contains the declaration of the class ConfigurationDatabase
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONFIGURATIONDATABASE_H_
#define CONFIGURATIONDATABASE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AnyObject.h"
#include "TypeConversion.h"
#include "StructuredDataI.h"
#include "ReferenceT.h"
#include "ReferenceContainer.h"
#include "ReferenceContainerFilterObjectName.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief An implementation of StructuredDataI where node paths are identified and separated by dots.
 * @details Paths are constructed by concatenating node names with dots (e.g. A.B.C.D), where D is a leaf
 * and A, B and C are nodes.
 */
class ConfigurationDatabase: public StructuredDataI {
public:

    /**
     * @brief Default constructor. NOOP.
     */
    ConfigurationDatabase();

    /**
     * TODO
     */
    virtual ~ConfigurationDatabase();

    /**
     * TODO
     */
    virtual bool Read(const char * const name,
                      const AnyType &value);

    /**
     * TODO
     */
    virtual AnyType GetType(const char * const name);

    /**
     * TODO
     */
    virtual bool Write(const char * const name,
                       const AnyType &value);

    /**
     * TODO
     */
    virtual bool Copy(StructuredDataI &destination);

    /**
     * TODO
     */
    virtual bool MoveToRoot();

    /**
     * TODO
     */
    virtual bool MoveToAncestor(uint32 generations);

    /**
     * TODO. Delete empty nodes?
     */
    virtual bool MoveAbsolute(const char * const path);

    virtual bool MoveRelative(const char * const path);

    /**
     * TODO
     */
    virtual bool CreateNodesAbsolute(const char * const path);

    virtual bool CreateNodesRelative(const char * const path);

    virtual bool AddToCurrentNode(Reference node);

private:
    /**
     * TODO
     */
    bool CreateNodes(const char * const path);

    /**
     * TODO
     */
    ReferenceT<ReferenceContainer> currentNode;

    /**
     * TODO
     */
    ReferenceT<ReferenceContainer> rootNode;

    /**
     * TODO
     */
    FastPollingMutexSem mux;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONFIGURATIONDATABASE_H_ */

