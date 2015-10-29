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
 * @brief TODO
 * @details TODO
 */
//TODO
class ConfigurationDatabase: public StructuredDataI {
public:

    /**
     * TODO
     */
    ConfigurationDatabase();

    /**
     * TODO
     */
    virtual ~ConfigurationDatabase();

    /**
     * TODO
     */
    /*template<typename T>
    bool Read(const char * const name,
              T &value);*/

    virtual bool ReadAnyType(const char * const name, AnyType &value);

    /**
     * TODO
     */
    virtual bool Write(const char * const name,
                       const AnyType &value);

    /**
     * TODO
     */
    virtual bool Copy(StructuredDataI &destination,
                      bool fromRoot);

    /**
     * TODO
     */
    virtual bool MoveToAncestor(uint32 generations);

    /**
     * TODO. Delete empty nodes?
     */
    virtual bool Move(const char * const path,
                      bool relative);

    /**
     * TODO
     */
    virtual bool CreateNodes(const char * const path,
                             bool relative);

private:
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

/*template<typename T>
bool ConfigurationDatabase::Read(const char * const name,
                                 T &value, uint32 nOfElementsDim1 = 0, uint32 nOfElementsDim2 = 0, uint32 nOfElementsDim3 = 0) {

    //TODO might need to use optimised filter which works only on leaf name
    ReferenceContainerFilterObjectName filter(1, 0u, name);
    ReferenceContainer resultSingle;
    currentNode->Find(resultSingle, filter);
    printf("Looking for %s in %s\n", name, currentNode->GetName());
    bool ok = (resultSingle.Size() > 0);
    if (ok) {
        ReferenceT<AnyObject> objToRead = resultSingle.Get(0);
        printf("Something was found: %s\n", objToRead->GetName());
        AnyType anyTypeToConvert = value;
        ok = TypeConvert(anyTypeToConvert, objToRead->GetType());
        printf("Value is: %f\n", *(float32 *)objToRead->GetType().GetDataPointer());
        value = *(static_cast<T *> (anyTypeToConvert.GetDataPointer()));
    }

    return ok;
}*/


}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONFIGURATIONDATABASE_H_ */

